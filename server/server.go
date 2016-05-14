package main

import (
	"encoding/json"
	"index/suffixarray"
	"io/ioutil"
	"net/http"
	"os"
)

type Message struct {
	Filepath string
	Id       string `json:"msgid"`
	Str      string `json:"msgstr"`
}

type MessageFile struct {
	Messages []Message `json:"messages"`
}

type Index struct {
	Messages []Message
	Offsets  []int
	index    *suffixarray.Index
}

func (idx *Index) Lookup(needle string) []Message {
	results := []Message{}

	matches := map[int]struct{}{}

	for _, offset := range idx.index.Lookup([]byte(needle), -1) {
		matches[offset] = struct{}{}
	}

	for offset, _ := range matches {
		results = append(results, idx.Messages[idx.Offsets[offset]])
	}

	return results
}

func ReadJSON(path string, obj interface{}) error {
	file, err := os.Open(path)
	if err != nil {
		return err
	}

	defer file.Close()
	return json.NewDecoder(file).Decode(obj)
}

func ReadFiles(path string) []Message {
	messages := []Message{}
	files, _ := ioutil.ReadDir(path)
	for _, f := range files {
		fname := path + "/" + f.Name() + "/pt.po.json"
		msg := MessageFile{}
		ReadJSON(fname, &msg)

		for _, m := range msg.Messages {
			m.Filepath = f.Name()
			messages = append(messages, m)
		}
	}
	return messages
}

func BuildIndex() Index {
	idx := Index{
		Offsets: []int{},
	}

	idx.Messages = ReadFiles("../fetcher/files")

	data := []byte{}
	for i, msg := range idx.Messages {
		data = append(data, []byte(msg.Id)...)
		data = append(data, 0)
		data = append(data, []byte(msg.Str)...)
		data = append(data, 0)

		for len(data) != len(idx.Offsets) {
			idx.Offsets = append(idx.Offsets, i)
		}
	}

	idx.index = suffixarray.New(data)
	return idx
}

type Settings struct {
	Listen string `json:"listen"`
}

func main() {
	idx := BuildIndex()

	http.HandleFunc("/search", func(w http.ResponseWriter, r *http.Request) {
		json.NewEncoder(w).Encode(idx.Lookup(r.URL.Query().Get("term")))
	})

	settings := Settings{}
	ReadJSON("config/settings.json", &settings)

	http.ListenAndServe(settings.Listen, nil)
}

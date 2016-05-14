#include <stdio.h>
#include <gettext-po.h>
#include <string>

void error_fn (int a, po_message_t b, const char *c, size_t d, size_t e,
    int f, const char *g) {}

void error2_fn (int a, po_message_t b, const char *c, size_t d, size_t e,
    int f, const char *g, po_message_t h, const char *i, size_t j, size_t k,
    int l, const char *m) {}

std::string escape_json(const std::string& s) {
  std::string escaped;
  for (auto c : s) {
    if (c == '\n')
      escaped += "\\n";
    else if (c == '\t')
      escaped += "\\t";
    else if (c == '\\')
      escaped += "\\\\";
    else if (c == '"')
      escaped += "\\\"";
    else
      escaped += c;
  }

  return escaped;
}

int callback (FILE* fp, const char* msgid, const char* msgstr, bool first)
{
  if (!first) {
    fprintf(fp, ",");
  }
  fprintf(fp, "{\"msgid\":\"%s\",", escape_json(msgid).c_str());
  fprintf(fp, "\"msgstr\":\"%s\"}", escape_json(msgstr).c_str());
}

void po_iterate (FILE* out, const char* filename)
{
  struct po_xerror_handler derp;
  bool first = true;

  po_file_t             fp = NULL;
  po_message_iterator_t it = NULL;
  po_message_t          message = NULL;
  const char* const*    domains = NULL;
  const char* const*    domain = NULL;

  derp.xerror = &error_fn;
  derp.xerror2 = &error2_fn;

  if ((fp = po_file_read(filename, &derp)) == NULL) {
    perror("po_file_read");
    exit(-1);
  }

  domains = po_file_domains(fp);
  for (domain = domains; *domain; ++domain) {
    it = po_message_iterator(fp, *domain);

    while ((message = po_next_message(it)) != NULL) {
      const char* msgid = NULL;
      const char* msgstr = NULL;
      callback(out, po_message_msgid(message), po_message_msgstr(message), first);
      first = false;

      msgid = po_message_msgid_plural(message);
      for (int i = 0; (msgstr = po_message_msgstr_plural(message, i)); ++i) {
        callback(out, msgid, msgstr, first);
      }
    }
  }

  po_file_free(fp);
}

int main (int argc, char* argv[])
{
  std::string out = std::string(argv[1]) + ".json";
  FILE* fp = fopen(out.c_str(), "w+");

  fprintf(fp, "{\"messages\":[");
  po_iterate(fp, argv[1]);
  fprintf(fp, "]}");

  fclose(fp);
  return 0;
}

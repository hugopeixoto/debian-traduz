import React from 'react';
import Highlight from './highlight.jsx';
import debounce from 'debounce';

class Results extends React.Component {
  render() {
    return (
      <ul className="results">
        {this.props.results.map((r, i) =>
          <li key={i}>
            <span className="title">package <strong>{r.Filepath}</strong></span>
            <Highlight className="msgid" highlight={this.props.search} text={r.msgid} />
            <Highlight className="msgstr" highlight={this.props.search} text={r.msgstr} />
          </li>
        )}
      </ul>
    );
  }
}

export default class extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      term: "",
      results: [],
    }

    // derp?
    this.onChange = this.onChange.bind(this);
    this.search = debounce(this.search, 500);
  }

  render() {
    return (
      <div>
        <h1>Debian Traduz</h1>
        <input className="searchbox" autoFocus="true" type="text" onChange={this.onChange} />
        <Results search={this.state.term} results={this.state.results} />
      </div>
    );
  }

  onChange(ev) {
    this.search(ev.target.value);
  }

  search(term) {
    if (term.length > 2) {
      fetch(this.props.searchUrl + "?term=" + term).
        then(r => r.json()).
        then(r => r.sort((a, b) => a.Filepath.localeCompare(b.Filepath))).
        then(j => {
          this.setState({ term: term, results: j })
        });
    }
  }
}

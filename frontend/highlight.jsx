import React from 'react';

export default class extends React.Component {
  render() {
    return <span className={this.props.className} dangerouslySetInnerHTML={this.highlightedText()}/>;
  }

  highlightedText() {
    return {__html: this.props.text.replace(
        this.props.highlight,
        "<span class=\"highlighted\">" + this.props.highlight + "</span>") };
  }
}

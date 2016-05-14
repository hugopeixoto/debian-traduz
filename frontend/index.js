import React from 'react';
import ReactDOM from 'react-dom';
import Search from './search.jsx';

require('./search.scss');
require('file?name=index.html!./index.html');

var config = require('./config/settings.json');

ReactDOM.render(
    React.createFactory(Search)({ searchUrl: config['search-url'] }),
    document.getElementById('search'));

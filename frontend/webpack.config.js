module.exports = {
  entry: './index.js',
  output: { path: __dirname + '/output', filename: 'bundle.js' },
  module: {
    loaders: [
      { test: /.jsx?$/, loader: 'babel-loader', query: { presets: ['es2015', 'react'] } },
      { test: /\.css$/, loaders: ["style", "css"] },
      { test: /\.json$/, loaders: ["json"] },
      { test: /\.scss$/, loaders: ["style", "css", "sass"] },
    ]
  }
}

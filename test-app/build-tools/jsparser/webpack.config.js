const path = require("path");

module.exports = {
  entry: "./js_parser.js",
  target: "node",
  mode: "production",
  node: {
    __filename: false,
    __dirname: false,
  },
  output: {
    path: path.join(__dirname, "build"),
    filename: "js_parser.js",
  },
};

var utils = require("utils/utils");
var http;
function ensureHttp() {
    if (!http) {
        http = require("http");
    }
}
var definition = require("image-source");
function fromResource(name) {
    var image = new definition.ImageSource();
    return image.loadFromResource(name) ? image : null;
}
exports.fromResource = fromResource;
function fromFile(path) {
    var image = new definition.ImageSource();
    return image.loadFromFile(path) ? image : null;
}
exports.fromFile = fromFile;
function fromData(data) {
    var image = new definition.ImageSource();
    return image.loadFromData(data) ? image : null;
}
exports.fromData = fromData;
function fromBase64(source) {
    var image = new definition.ImageSource();
    return image.loadFromBase64(source) ? image : null;
}
exports.fromBase64 = fromBase64;
function fromNativeSource(source) {
    var image = new definition.ImageSource();
    return image.setNativeSource(source) ? image : null;
}
exports.fromNativeSource = fromNativeSource;
function fromUrl(url) {
    ensureHttp();
    return http.getImage(url);
}
exports.fromUrl = fromUrl;
function fromFileOrResource(path) {
    if (!isFileOrResourcePath(path)) {
        throw new Error("Path \"" + "\" is not a valid file or resource.");
    }
    if (path.indexOf(utils.RESOURCE_PREFIX) === 0) {
        return fromResource(path.substr(utils.RESOURCE_PREFIX.length));
    }
    return fromFile(path);
}
exports.fromFileOrResource = fromFileOrResource;
function isFileOrResourcePath(path) {
    return utils.isFileOrResourcePath(path);
}
exports.isFileOrResourcePath = isFileOrResourcePath;

var types = require("utils/types");
var common = require("./image-source-common");
global.moduleMerge(common, exports);
var utils;
function ensureUtils() {
    if (!utils) {
        utils = require("utils/utils");
    }
}
var fs;
function ensureFS() {
    if (!fs) {
        fs = require("file-system");
    }
}
var enums;
function ensureEnums() {
    if (!enums) {
        enums = require("ui/enums");
    }
}
var ImageSource = (function () {
    function ImageSource() {
    }
    ImageSource.prototype.loadFromResource = function (name) {
        this.android = null;
        ensureUtils();
        var res = utils.ad.getApplicationContext().getResources();
        if (res) {
            var identifier = res.getIdentifier(name, 'drawable', utils.ad.getApplication().getPackageName());
            if (0 < identifier) {
                var bitmapDrawable = res.getDrawable(identifier);
                if (bitmapDrawable && bitmapDrawable.getBitmap) {
                    this.android = bitmapDrawable.getBitmap();
                }
            }
        }
        return this.android != null;
    };
    ImageSource.prototype.fromResource = function (name) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            resolve(_this.loadFromResource(name));
        });
    };
    ImageSource.prototype.loadFromFile = function (path) {
        ensureFS();
        var fileName = types.isString(path) ? path.trim() : "";
        if (fileName.indexOf("~/") === 0) {
            fileName = fs.path.join(fs.knownFolders.currentApp().path, fileName.replace("~/", ""));
        }
        this.android = android.graphics.BitmapFactory.decodeFile(fileName, null);
        return this.android != null;
    };
    ImageSource.prototype.fromFile = function (path) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            resolve(_this.loadFromFile(path));
        });
    };
    ImageSource.prototype.loadFromData = function (data) {
        this.android = android.graphics.BitmapFactory.decodeStream(data);
        return this.android != null;
    };
    ImageSource.prototype.fromData = function (data) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            resolve(_this.loadFromData(data));
        });
    };
    ImageSource.prototype.loadFromBase64 = function (source) {
        if (types.isString(source)) {
            var bytes = android.util.Base64.decode(source, android.util.Base64.DEFAULT);
            this.android = android.graphics.BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
        }
        return this.android != null;
    };
    ImageSource.prototype.fromBase64 = function (data) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            resolve(_this.loadFromBase64(data));
        });
    };
    ImageSource.prototype.setNativeSource = function (source) {
        this.android = source;
        return source != null;
    };
    ImageSource.prototype.saveToFile = function (path, format, quality) {
        if (quality === void 0) { quality = 100; }
        if (!this.android) {
            return false;
        }
        var targetFormat = getTargetFormat(format);
        var outputStream = new java.io.BufferedOutputStream(new java.io.FileOutputStream(path));
        var res = this.android.compress(targetFormat, quality, outputStream);
        outputStream.close();
        return res;
    };
    ImageSource.prototype.toBase64String = function (format, quality) {
        if (quality === void 0) { quality = 100; }
        if (!this.android) {
            return null;
            ;
        }
        var targetFormat = getTargetFormat(format);
        var outputStream = new java.io.ByteArrayOutputStream();
        var base64Stream = new android.util.Base64OutputStream(outputStream, android.util.Base64.NO_WRAP);
        this.android.compress(targetFormat, quality, base64Stream);
        base64Stream.close();
        outputStream.close();
        return outputStream.toString();
    };
    Object.defineProperty(ImageSource.prototype, "height", {
        get: function () {
            if (this.android) {
                return this.android.getHeight();
            }
            return NaN;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ImageSource.prototype, "width", {
        get: function () {
            if (this.android) {
                return this.android.getWidth();
            }
            return NaN;
        },
        enumerable: true,
        configurable: true
    });
    return ImageSource;
}());
exports.ImageSource = ImageSource;
function getTargetFormat(format) {
    ensureEnums();
    switch (format) {
        case enums.ImageFormat.jpeg:
            return android.graphics.Bitmap.CompressFormat.JPEG;
        default:
            return android.graphics.Bitmap.CompressFormat.PNG;
    }
}

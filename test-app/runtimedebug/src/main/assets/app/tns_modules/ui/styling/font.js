var enums = require("ui/enums");
var common = require("./font-common");
var application;
function ensureApplication() {
    if (!application) {
        application = require("application");
    }
}
var types;
function ensureTypes() {
    if (!types) {
        types = require("utils/types");
    }
}
var trace;
function ensureTrace() {
    if (!trace) {
        trace = require("trace");
    }
}
var fs;
function ensureFS() {
    if (!fs) {
        fs = require("file-system");
    }
}
var typefaceCache = new Map();
var appAssets;
var FONTS_BASE_PATH = "/fonts/";
var Font = (function (_super) {
    __extends(Font, _super);
    function Font(family, size, style, weight) {
        _super.call(this, family, size, style, weight);
    }
    Font.prototype.withFontFamily = function (family) {
        return new Font(family, this.fontSize, this.fontStyle, this.fontWeight);
    };
    Font.prototype.withFontStyle = function (style) {
        return new Font(this.fontFamily, this.fontSize, style, this.fontWeight);
    };
    Font.prototype.withFontWeight = function (weight) {
        return new Font(this.fontFamily, this.fontSize, this.fontStyle, weight);
    };
    Font.prototype.withFontSize = function (size) {
        return new Font(this.fontFamily, size, this.fontStyle, this.fontWeight);
    };
    Font.prototype.getAndroidTypeface = function () {
        if (!this._typeface) {
            var fontStyle = 0;
            if (this.isBold) {
                fontStyle |= android.graphics.Typeface.BOLD;
            }
            if (this.isItalic) {
                fontStyle |= android.graphics.Typeface.ITALIC;
            }
            var typeFace = createTypeface(this);
            this._typeface = android.graphics.Typeface.create(typeFace, fontStyle);
        }
        return this._typeface;
    };
    Font.default = new Font(undefined, undefined, enums.FontStyle.normal, enums.FontWeight.normal);
    return Font;
}(common.Font));
exports.Font = Font;
function loadFontFromFile(fontFamily) {
    ensureApplication();
    appAssets = appAssets || application.android.context.getAssets();
    if (!appAssets) {
        return null;
    }
    ensureTypes();
    var result = typefaceCache.get(fontFamily);
    if (types.isUndefined(result)) {
        result = null;
        ensureTrace();
        ensureFS();
        var fontAssetPath;
        var basePath = fs.path.join(fs.knownFolders.currentApp().path, "fonts", fontFamily);
        if (fs.File.exists(basePath + ".ttf")) {
            fontAssetPath = FONTS_BASE_PATH + fontFamily + ".ttf";
        }
        else if (fs.File.exists(basePath + ".otf")) {
            fontAssetPath = FONTS_BASE_PATH + fontFamily + ".otf";
        }
        else {
            if (trace.enabled) {
                trace.write("Could not find font file for " + fontFamily, trace.categories.Error, trace.messageType.error);
            }
        }
        if (fontAssetPath) {
            try {
                fontAssetPath = fs.path.join(fs.knownFolders.currentApp().path, fontAssetPath);
                result = android.graphics.Typeface.createFromFile(fontAssetPath);
            }
            catch (e) {
                if (trace.enabled) {
                    trace.write("Error loading font asset: " + fontAssetPath, trace.categories.Error, trace.messageType.error);
                }
            }
        }
        typefaceCache.set(fontFamily, result);
    }
    return result;
}
function createTypeface(font) {
    var fonts = common.parseFontFamily(font.fontFamily);
    var result = null;
    if (fonts.length === 0) {
        return null;
    }
    for (var i = 0; i < fonts.length; i++) {
        switch (fonts[i].toLowerCase()) {
            case common.genericFontFamilies.serif:
                result = android.graphics.Typeface.create("serif" + getFontWeightSuffix(font.fontWeight), 0);
                break;
            case common.genericFontFamilies.sansSerif:
            case common.genericFontFamilies.system:
                result = android.graphics.Typeface.create("sans-serif" + getFontWeightSuffix(font.fontWeight), 0);
                break;
            case common.genericFontFamilies.monospace:
                result = android.graphics.Typeface.create("monospace" + getFontWeightSuffix(font.fontWeight), 0);
                break;
            default:
                result = loadFontFromFile(fonts[i]);
                break;
        }
        if (result) {
            return result;
        }
    }
    return null;
}
function getFontWeightSuffix(fontWeight) {
    switch (fontWeight) {
        case enums.FontWeight.thin:
            return android.os.Build.VERSION.SDK_INT >= 16 ? "-thin" : "";
        case enums.FontWeight.extraLight:
        case enums.FontWeight.light:
            return android.os.Build.VERSION.SDK_INT >= 16 ? "-light" : "";
        case enums.FontWeight.normal:
        case "400":
        case undefined:
        case null:
            return "";
        case enums.FontWeight.medium:
        case enums.FontWeight.semiBold:
            return android.os.Build.VERSION.SDK_INT >= 21 ? "-medium" : "";
        case enums.FontWeight.bold:
        case "700":
        case enums.FontWeight.extraBold:
            return "";
        case enums.FontWeight.black:
            return android.os.Build.VERSION.SDK_INT >= 21 ? "-black" : "";
        default:
            throw new Error("Invalid font weight: \"" + fontWeight + "\"");
    }
}

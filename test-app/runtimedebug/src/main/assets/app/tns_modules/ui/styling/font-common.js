var enums = require("ui/enums");
var converters = require("./converters");
var Font = (function () {
    function Font(family, size, style, weight) {
        this._fontFamily = family;
        this._fontSize = size;
        this._fontStyle = style;
        this._fontWeight = weight;
    }
    Object.defineProperty(Font.prototype, "fontFamily", {
        get: function () {
            return this._fontFamily;
        },
        set: function (value) {
            throw new Error("fontFamily is read-only");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Font.prototype, "fontStyle", {
        get: function () {
            return this._fontStyle;
        },
        set: function (value) {
            throw new Error("fontStyle is read-only");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Font.prototype, "fontWeight", {
        get: function () {
            return this._fontWeight;
        },
        set: function (value) {
            throw new Error("fontWeight is read-only");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Font.prototype, "fontSize", {
        get: function () {
            return this._fontSize;
        },
        set: function (value) {
            throw new Error("fontSize is read-only");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Font.prototype, "isBold", {
        get: function () {
            return this._fontWeight.toLowerCase() === enums.FontWeight.bold
                || this._fontWeight.toLowerCase() === "700";
        },
        set: function (value) {
            throw new Error("isBold is read-only");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Font.prototype, "isItalic", {
        get: function () {
            return this._fontStyle.toLowerCase() === enums.FontStyle.italic;
        },
        set: function (value) {
            throw new Error("isItalic is read-only");
        },
        enumerable: true,
        configurable: true
    });
    Font.prototype.getAndroidTypeface = function () {
        return undefined;
    };
    Font.prototype.getUIFont = function (defaultFont) {
        return undefined;
    };
    Font.prototype.withFontFamily = function (family) {
        throw new Error("This should be called on the derived class");
    };
    Font.prototype.withFontStyle = function (style) {
        throw new Error("This should be called on the derived class");
    };
    Font.prototype.withFontWeight = function (weight) {
        throw new Error("This should be called on the derived class");
    };
    Font.prototype.withFontSize = function (size) {
        throw new Error("This should be called on the derived class");
    };
    Font.equals = function (value1, value2) {
        if (!value1 && !value2) {
            return true;
        }
        if (!value1 || !value2) {
            return false;
        }
        return value1.fontFamily === value2.fontFamily &&
            value1.fontSize === value2.fontSize &&
            value1.fontStyle === value2.fontStyle &&
            value1.fontWeight === value2.fontWeight;
    };
    Font.parse = function (cssValue) {
        var parsed = parseFont(cssValue);
        var size = converters.fontSizeConverter(parsed.fontSize);
        size = !!size ? size : undefined;
        return new Font(parsed.fontFamily, size, parsed.fontStyle, parsed.fontWeight);
    };
    Font.default = undefined;
    return Font;
}());
exports.Font = Font;
function parseFontFamily(value) {
    var result = new Array();
    if (!value) {
        return result;
    }
    var split = value.split(",");
    for (var i = 0; i < split.length; i++) {
        var str = split[i].trim().replace(/['"]+/g, '');
        if (str) {
            result.push(str);
        }
    }
    return result;
}
exports.parseFontFamily = parseFontFamily;
var genericFontFamilies;
(function (genericFontFamilies) {
    genericFontFamilies.serif = "serif";
    genericFontFamilies.sansSerif = "sans-serif";
    genericFontFamilies.monospace = "monospace";
    genericFontFamilies.system = "system";
})(genericFontFamilies = exports.genericFontFamilies || (exports.genericFontFamilies = {}));
var styles = new Set();
["italic", "oblique"].forEach(function (val, i, a) { return styles.add(val); });
var weights = new Set();
["normal", "bold", "100", "200", "300", "400", "500", "600", "700", "800", "900"].forEach(function (val, i, a) { return weights.add(val); });
function parseFont(fontValue) {
    var result = {
        fontStyle: "normal",
        fontVariant: "normal",
        fontWeight: "normal",
    };
    var parts = fontValue.split(/\s+/);
    var part;
    while (part = parts.shift()) {
        if (part === "normal") {
        }
        else if (part === "small-caps") {
            result.fontVariant = part;
        }
        else if (styles.has(part)) {
            result.fontStyle = part;
        }
        else if (weights.has(part)) {
            result.fontWeight = part;
        }
        else if (!result.fontSize) {
            var sizes = part.split("/");
            result.fontSize = sizes[0];
            result.lineHeight = sizes.length > 1 ? sizes[1] : undefined;
        }
        else {
            result.fontFamily = part;
            if (parts.length) {
                result.fontFamily += " " + parts.join(" ");
            }
            break;
        }
    }
    return result;
}

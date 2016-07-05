var types = require("utils/types");
var knownColors = require("color/known-colors");
var AMP = "#";
var HEX_REGEX = /(^#[0-9A-F]{6}$)|(^#[0-9A-F]{3}$)|(^#[0-9A-F]{8}$)/i;
var Color = (function () {
    function Color() {
        if (arguments.length === 1) {
            var arg = arguments[0];
            if (types.isString(arg)) {
                if (isRgbOrRgba(arg)) {
                    this._argb = argbFromRgbOrRgba(arg);
                }
                else if (knownColors.isKnownName(arg)) {
                    this._hex = knownColors.getKnownColor(arg);
                    this._name = arg;
                    this._argb = this._argbFromString(this._hex);
                }
                else {
                    this._hex = this._normalizeHex(arg);
                    this._argb = this._argbFromString(this._hex);
                }
            }
            else if (types.isNumber(arg)) {
                this._argb = arg;
            }
            else {
                throw new Error("Expected 1 or 4 constructor parameters.");
            }
            this._parseComponents();
            if (!this._hex) {
                this._hex = this._buildHex();
            }
        }
        else if (arguments.length === 4) {
            this._a = arguments[0];
            this._r = arguments[1];
            this._g = arguments[2];
            this._b = arguments[3];
            this._buildArgb();
            this._hex = this._buildHex();
        }
        else {
            throw new Error("Expected 1 or 4 constructor parameters.");
        }
    }
    Object.defineProperty(Color.prototype, "a", {
        get: function () {
            return this._a;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "r", {
        get: function () {
            return this._r;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "g", {
        get: function () {
            return this._g;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "b", {
        get: function () {
            return this._b;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "argb", {
        get: function () {
            return this._argb;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "hex", {
        get: function () {
            return this._hex;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "name", {
        get: function () {
            return this._name;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "ios", {
        get: function () {
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Color.prototype, "android", {
        get: function () {
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    Color.prototype._argbFromString = function (hex) {
        return undefined;
    };
    Color.prototype.equals = function (value) {
        return this.argb === value.argb;
    };
    Color.equals = function (value1, value2) {
        if (!value1 && !value2) {
            return true;
        }
        if (!value1 || !value2) {
            return false;
        }
        return value1.equals(value2);
    };
    Color.isValid = function (value) {
        if (types.isNullOrUndefined(value) || value instanceof Color) {
            return true;
        }
        if (!types.isString(value)) {
            return false;
        }
        if (knownColors.isKnownName(value)) {
            return true;
        }
        return HEX_REGEX.test(value) || isRgbOrRgba(value);
    };
    Color.prototype._buildHex = function () {
        return AMP + this._componentToHex(this._a) + this._componentToHex(this._r) + this._componentToHex(this._g) + this._componentToHex(this._b);
    };
    Color.prototype._componentToHex = function (component) {
        var hex = component.toString(16);
        if (hex.length === 1) {
            hex = "0" + hex;
        }
        return hex;
    };
    Color.prototype._parseComponents = function () {
        if (types.isUndefined(this._argb)) {
            throw new Error("Missing the ARGB numeric value");
        }
        this._a = (this._argb >> 24) & 255;
        this._r = (this._argb >> 16) & 255;
        this._g = (this._argb >> 8) & 255;
        this._b = this._argb & 255;
    };
    Color.prototype._buildArgb = function () {
        this._argb = (this._a << 24) | (this._r << 16) | (this._g << 8) | this._b;
    };
    Color.prototype._normalizeHex = function (hexStr) {
        if (hexStr.charAt(0) === AMP && hexStr.length === 4) {
            hexStr = hexStr.charAt(0)
                + hexStr.charAt(1) + hexStr.charAt(1)
                + hexStr.charAt(2) + hexStr.charAt(2)
                + hexStr.charAt(3) + hexStr.charAt(3);
        }
        return hexStr;
    };
    return Color;
}());
exports.Color = Color;
function isRgbOrRgba(value) {
    var toLower = value.toLowerCase();
    return (toLower.indexOf("rgb(") === 0 || toLower.indexOf("rgba(") === 0) && toLower.indexOf(")") === (toLower.length - 1);
}
function argbFromRgbOrRgba(value) {
    var toLower = value.toLowerCase();
    var parts = toLower.replace("rgba(", "").replace("rgb(", "").replace(")", "").trim().split(",");
    var r = 255, g = 255, b = 255, a = 255;
    if (parts[0]) {
        r = parseInt(parts[0].trim());
    }
    if (parts[1]) {
        g = parseInt(parts[1].trim());
    }
    if (parts[2]) {
        b = parseInt(parts[2].trim());
    }
    if (parts[3]) {
        a = Math.round(parseFloat(parts[3].trim()) * 255);
    }
    return (a << 24) | (r << 16) | (g << 8) | b;
}

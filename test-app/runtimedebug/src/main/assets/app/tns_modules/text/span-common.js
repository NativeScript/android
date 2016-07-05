var colorModule = require("color");
var bindable = require("ui/core/bindable");
var types = require("utils/types");
var enums = require("ui/enums");
var Span = (function (_super) {
    __extends(Span, _super);
    function Span() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(Span.prototype, "fontFamily", {
        get: function () {
            return this._fontFamily;
        },
        set: function (value) {
            if (this._fontFamily !== value) {
                this._fontFamily = value;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "fontSize", {
        get: function () {
            return this._fontSize;
        },
        set: function (value) {
            var fSize;
            if (types.isString(value)) {
                fSize = parseInt(value);
            }
            else {
                fSize = value;
            }
            if (this._fontSize !== fSize) {
                this._fontSize = fSize;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Span.prototype._getColorValue = function (value) {
        var result;
        if (types.isString(value) && colorModule.Color.isValid(value)) {
            result = new colorModule.Color(value);
        }
        else {
            result = value;
        }
        return result;
    };
    Object.defineProperty(Span.prototype, "foregroundColor", {
        get: function () {
            return this._foregroundColor;
        },
        set: function (value) {
            var convertedColor = this._getColorValue(value);
            if (this._foregroundColor !== convertedColor) {
                this._foregroundColor = convertedColor;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "backgroundColor", {
        get: function () {
            return this._backgroundColor;
        },
        set: function (value) {
            var convertedColor = this._getColorValue(value);
            if (this._backgroundColor !== convertedColor) {
                this._backgroundColor = convertedColor;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "underline", {
        get: function () {
            return this._underline;
        },
        set: function (value) {
            var underlineIntValue;
            if (types.isString(value)) {
                underlineIntValue = parseInt(value);
            }
            else {
                underlineIntValue = value;
            }
            if (this._underline !== underlineIntValue) {
                this._underline = underlineIntValue;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "strikethrough", {
        get: function () {
            return this._strikethrough;
        },
        set: function (value) {
            var strikethroughIntValue;
            if (types.isString(value)) {
                strikethroughIntValue = parseInt(value);
            }
            else {
                strikethroughIntValue = value;
            }
            if (this._strikethrough !== strikethroughIntValue) {
                this._strikethrough = strikethroughIntValue;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "fontAttributes", {
        get: function () {
            return this._fontAttributes;
        },
        set: function (value) {
            if (this._fontAttributes !== value) {
                this._fontAttributes = value;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "spanModifiers", {
        get: function () {
            if (!this._spanModifiers) {
                this._spanModifiers = new Array();
            }
            return this._spanModifiers;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "text", {
        get: function () {
            return this._text;
        },
        set: function (value) {
            if (this._text !== value) {
                this._text = value;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Span.prototype, "parentFormattedString", {
        get: function () {
            return this._parentFormattedString;
        },
        set: function (value) {
            if (this._parentFormattedString !== value) {
                this._parentFormattedString = value;
                this.updateAndNotify();
            }
        },
        enumerable: true,
        configurable: true
    });
    Span.prototype.updateSpanModifiers = function (parent) {
        if (this._isInEditMode) {
            throw new Error("Cannot update span modifiers during update!");
        }
        this._spanModifiers = new Array();
    };
    Span.prototype.beginEdit = function () {
        this._isInEditMode = true;
    };
    Span.prototype.updateAndNotify = function () {
        if (!this._isInEditMode) {
            this.updateSpanModifiers(this.parentFormattedString);
            this.notify(this._createPropertyChangeData(".", this));
        }
    };
    Span.prototype.endEdit = function () {
        this._isInEditMode = false;
        this.updateAndNotify();
    };
    Span.prototype._applyXmlAttribute = function (attribute, value) {
        if (attribute === "fontAttributes") {
            if (value.indexOf(",")) {
                var fontAttr = value.split(",");
                var fontAttrValue;
                var j;
                for (j = 0; j < fontAttr.length; j++) {
                    fontAttrValue = Span.getFontAttributeFromString(fontAttr[j]);
                    this.fontAttributes |= fontAttrValue;
                }
            }
            else {
                this.fontAttributes |= value;
            }
            return true;
        }
        else {
            return false;
        }
    };
    Span.getFontAttributeFromString = function (fontAttr) {
        var fontAttrTrimmedAndLowerCase = fontAttr.trim().toLowerCase();
        if (fontAttrTrimmedAndLowerCase === "bold") {
            return enums.FontAttributes.Bold;
        }
        else if (fontAttrTrimmedAndLowerCase === "italic") {
            return enums.FontAttributes.Italic;
        }
        else {
            return enums.FontAttributes.Normal;
        }
    };
    return Span;
}(bindable.Bindable));
exports.Span = Span;

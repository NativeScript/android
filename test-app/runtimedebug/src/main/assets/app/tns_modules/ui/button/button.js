var common = require("./button-common");
var style = require("ui/styling/style");
var text_base_styler_1 = require("ui/text-base/text-base-styler");
global.moduleMerge(common, exports);
var Button = (function (_super) {
    __extends(Button, _super);
    function Button() {
        _super.call(this);
        this._isPressed = false;
    }
    Object.defineProperty(Button.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    Button.prototype._createUI = function () {
        var that = new WeakRef(this);
        this._android = new android.widget.Button(this._context);
        this._android.setOnClickListener(new android.view.View.OnClickListener({
            get owner() {
                return that.get();
            },
            onClick: function (v) {
                if (this.owner) {
                    this.owner._emit(common.Button.tapEvent);
                }
            }
        }));
        this._android.setOnTouchListener(new android.view.View.OnTouchListener({
            get owner() {
                return that.get();
            },
            onTouch: function (v, ev) {
                if (ev.getAction() === 0) {
                    this.owner._goToVisualState("highlighted");
                }
                else if (ev.getAction() === 1) {
                    this.owner._goToVisualState("normal");
                }
                return false;
            }
        }));
    };
    Button.prototype._onTextPropertyChanged = function (data) {
        if (this.android) {
            this.android.setText(data.newValue + "");
        }
    };
    Button.prototype._setFormattedTextPropertyToNative = function (value) {
        var newText = value ? value._formattedText : null;
        if (this.android) {
            if (newText) {
                if (!this._transformationMethod) {
                    this._transformationMethod = this.android.getTransformationMethod();
                }
                this.android.setTransformationMethod(null);
            }
            else {
                if (this._transformationMethod && !this.android.getTransformationMethod()) {
                    this.android.setTransformationMethod(this._transformationMethod);
                }
            }
            this.android.setText(newText);
        }
    };
    return Button;
}(common.Button));
exports.Button = Button;
var ButtonStyler = (function () {
    function ButtonStyler() {
    }
    ButtonStyler.registerHandlers = function () {
        var TextBaseStyler = text_base_styler_1.TextBaseStyler;
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setColorProperty, TextBaseStyler.resetColorProperty, TextBaseStyler.getNativeColorValue), "Button");
        style.registerHandler(style.fontInternalProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setFontInternalProperty, TextBaseStyler.resetFontInternalProperty, TextBaseStyler.getNativeFontInternalValue), "Button");
        style.registerHandler(style.textAlignmentProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setTextAlignmentProperty, TextBaseStyler.resetTextAlignmentProperty, TextBaseStyler.getNativeTextAlignmentValue), "Button");
        style.registerHandler(style.textDecorationProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setTextDecorationProperty, TextBaseStyler.resetTextDecorationProperty), "Button");
        style.registerHandler(style.textTransformProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setTextTransformProperty, TextBaseStyler.resetTextTransformProperty), "Button");
        style.registerHandler(style.whiteSpaceProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setWhiteSpaceProperty, TextBaseStyler.resetWhiteSpaceProperty), "Button");
        style.registerHandler(style.letterSpacingProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setLetterSpacingProperty, TextBaseStyler.resetLetterSpacingProperty, TextBaseStyler.getLetterSpacingProperty), "Button");
    };
    return ButtonStyler;
}());
exports.ButtonStyler = ButtonStyler;
ButtonStyler.registerHandlers();

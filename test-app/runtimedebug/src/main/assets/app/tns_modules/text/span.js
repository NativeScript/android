var spanCommon = require("./span-common");
var enums = require("ui/enums");
var utils = require("utils/utils");
var fontModule = require("ui/styling/font");
global.moduleMerge(spanCommon, exports);
var CustomTypefaceSpanClass;
function ensureCustomTypefaceSpanClass() {
    if (CustomTypefaceSpanClass) {
        return;
    }
    var CustomTypefaceSpan = (function (_super) {
        __extends(CustomTypefaceSpan, _super);
        function CustomTypefaceSpan(family, typeface) {
            _super.call(this, family);
            this.typeface = typeface;
            return global.__native(this);
        }
        CustomTypefaceSpan.prototype.updateDrawState = function (ds) {
            CustomTypefaceSpan.applyCustomTypeFace(ds, this.typeface);
        };
        CustomTypefaceSpan.prototype.updateMeasureState = function (paint) {
            CustomTypefaceSpan.applyCustomTypeFace(paint, this.typeface);
        };
        CustomTypefaceSpan.applyCustomTypeFace = function (paint, tf) {
            var oldStyle;
            var old = paint.getTypeface();
            if (old === null) {
                oldStyle = 0;
            }
            else {
                oldStyle = old.getStyle();
            }
            var fake = oldStyle & ~tf.getStyle();
            if ((fake & android.graphics.Typeface.BOLD) !== 0) {
                paint.setFakeBoldText(true);
            }
            if ((fake & android.graphics.Typeface.ITALIC) !== 0) {
                paint.setTextSkewX(-0.25);
            }
            paint.setTypeface(tf);
        };
        return CustomTypefaceSpan;
    }(android.text.style.TypefaceSpan));
    CustomTypefaceSpanClass = CustomTypefaceSpan;
}
var Span = (function (_super) {
    __extends(Span, _super);
    function Span() {
        _super.apply(this, arguments);
    }
    Span.prototype.updateSpanModifiers = function (parent) {
        _super.prototype.updateSpanModifiers.call(this, parent);
        var realFontFamily = this.fontFamily || (parent ? parent.fontFamily : undefined);
        if (realFontFamily) {
            var font = new fontModule.Font(realFontFamily, 0, (realFontAttributes & enums.FontAttributes.Italic) ? enums.FontStyle.italic : enums.FontStyle.normal, (realFontAttributes & enums.FontAttributes.Bold) ? enums.FontWeight.bold : enums.FontWeight.normal);
            ensureCustomTypefaceSpanClass();
            var typefaceSpan = new CustomTypefaceSpanClass(realFontFamily, font.getAndroidTypeface());
            this.spanModifiers.push(typefaceSpan);
        }
        var realFontSize = this.fontSize ||
            (parent ? parent.fontSize : undefined) ||
            (parent && parent.parent ? parent.parent.style.fontSize : undefined);
        if (realFontSize) {
            this.spanModifiers.push(new android.text.style.AbsoluteSizeSpan(realFontSize * utils.layout.getDisplayDensity()));
        }
        var realForegroundColor = this.foregroundColor ||
            (parent ? parent.foregroundColor : undefined) ||
            (parent && parent.parent ? parent.parent.style.color : undefined);
        if (realForegroundColor) {
            this.spanModifiers.push(new android.text.style.ForegroundColorSpan(realForegroundColor.android));
        }
        var realBackgroundColor = this.backgroundColor ||
            (parent ? parent.backgroundColor : undefined) ||
            (parent && parent.parent ? parent.parent.style.backgroundColor : undefined);
        if (realBackgroundColor) {
            this.spanModifiers.push(new android.text.style.BackgroundColorSpan(realBackgroundColor.android));
        }
        var realFontAttributes = this.fontAttributes || (parent ? parent.fontAttributes : undefined);
        if (realFontAttributes) {
            if ((realFontAttributes & enums.FontAttributes.Bold) && (realFontAttributes & enums.FontAttributes.Italic)) {
                this.spanModifiers.push(new android.text.style.StyleSpan(android.graphics.Typeface.BOLD_ITALIC));
            }
            else if (realFontAttributes & enums.FontAttributes.Bold) {
                this.spanModifiers.push(new android.text.style.StyleSpan(android.graphics.Typeface.BOLD));
            }
            else if (realFontAttributes & enums.FontAttributes.Italic) {
                this.spanModifiers.push(new android.text.style.StyleSpan(android.graphics.Typeface.ITALIC));
            }
        }
        var realUnderline = this.underline || (parent ? parent.underline : undefined);
        if (realUnderline) {
            this.spanModifiers.push(new android.text.style.UnderlineSpan());
        }
        var realStrikethrough = this.strikethrough || (parent ? parent.strikethrough : undefined);
        if (realStrikethrough) {
            this.spanModifiers.push(new android.text.style.StrikethroughSpan());
        }
    };
    return Span;
}(spanCommon.Span));
exports.Span = Span;

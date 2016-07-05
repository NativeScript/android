var utils = require("utils/utils");
var style = require("ui/styling/style");
var enums = require("ui/enums");
var TextBaseStyler = (function () {
    function TextBaseStyler() {
    }
    TextBaseStyler.setColorProperty = function (view, newValue) {
        view._nativeView.setTextColor(newValue);
    };
    TextBaseStyler.resetColorProperty = function (view, nativeValue) {
        view._nativeView.setTextColor(nativeValue);
    };
    TextBaseStyler.getNativeColorValue = function (view) {
        return view._nativeView.getTextColors().getDefaultColor();
    };
    TextBaseStyler.setFontInternalProperty = function (view, newValue, nativeValue) {
        var tv = view._nativeView;
        var fontValue = newValue;
        var typeface = fontValue.getAndroidTypeface();
        if (typeface) {
            tv.setTypeface(typeface);
        }
        else {
            tv.setTypeface(nativeValue.typeface);
        }
        if (fontValue.fontSize) {
            tv.setTextSize(fontValue.fontSize);
        }
        else {
            tv.setTextSize(android.util.TypedValue.COMPLEX_UNIT_PX, nativeValue.size);
        }
    };
    TextBaseStyler.resetFontInternalProperty = function (view, nativeValue) {
        var tv = view._nativeView;
        if (tv && nativeValue) {
            tv.setTypeface(nativeValue.typeface);
            tv.setTextSize(android.util.TypedValue.COMPLEX_UNIT_PX, nativeValue.size);
        }
    };
    TextBaseStyler.getNativeFontInternalValue = function (view) {
        var tv = view._nativeView;
        return {
            typeface: tv.getTypeface(),
            size: tv.getTextSize()
        };
    };
    TextBaseStyler.setTextAlignmentProperty = function (view, newValue) {
        var verticalGravity = view._nativeView.getGravity() & android.view.Gravity.VERTICAL_GRAVITY_MASK;
        switch (newValue) {
            case enums.TextAlignment.left:
                view._nativeView.setGravity(android.view.Gravity.LEFT | verticalGravity);
                break;
            case enums.TextAlignment.center:
                view._nativeView.setGravity(android.view.Gravity.CENTER_HORIZONTAL | verticalGravity);
                break;
            case enums.TextAlignment.right:
                view._nativeView.setGravity(android.view.Gravity.RIGHT | verticalGravity);
                break;
            default:
                break;
        }
    };
    TextBaseStyler.resetTextAlignmentProperty = function (view, nativeValue) {
        view._nativeView.setGravity(nativeValue);
    };
    TextBaseStyler.getNativeTextAlignmentValue = function (view) {
        return view._nativeView.getGravity();
    };
    TextBaseStyler.setTextDecorationProperty = function (view, newValue) {
        utils.ad.setTextDecoration(view._nativeView, newValue);
    };
    TextBaseStyler.resetTextDecorationProperty = function (view, nativeValue) {
        utils.ad.setTextDecoration(view._nativeView, enums.TextDecoration.none);
    };
    TextBaseStyler.setTextTransformProperty = function (view, newValue) {
        utils.ad.setTextTransform(view, newValue);
    };
    TextBaseStyler.resetTextTransformProperty = function (view, nativeValue) {
        utils.ad.setTextTransform(view, enums.TextTransform.none);
    };
    TextBaseStyler.setWhiteSpaceProperty = function (view, newValue) {
        utils.ad.setWhiteSpace(view._nativeView, newValue);
    };
    TextBaseStyler.resetWhiteSpaceProperty = function (view, nativeValue) {
        utils.ad.setWhiteSpace(view._nativeView, enums.WhiteSpace.normal);
    };
    TextBaseStyler.getLetterSpacingProperty = function (view) {
        return view.android.getLetterSpacing ? view.android.getLetterSpacing() : 0;
    };
    TextBaseStyler.setLetterSpacingProperty = function (view, newValue) {
        if (view.android.setLetterSpacing) {
            view.android.setLetterSpacing(utils.layout.toDeviceIndependentPixels(newValue));
        }
    };
    TextBaseStyler.resetLetterSpacingProperty = function (view, nativeValue) {
        if (view.android.setLetterSpacing) {
            view.android.setLetterSpacing(nativeValue);
        }
    };
    TextBaseStyler.registerHandlers = function () {
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setColorProperty, TextBaseStyler.resetColorProperty, TextBaseStyler.getNativeColorValue), "TextBase");
        style.registerHandler(style.fontInternalProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setFontInternalProperty, TextBaseStyler.resetFontInternalProperty, TextBaseStyler.getNativeFontInternalValue), "TextBase");
        style.registerHandler(style.textAlignmentProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setTextAlignmentProperty, TextBaseStyler.resetTextAlignmentProperty, TextBaseStyler.getNativeTextAlignmentValue), "TextBase");
        style.registerHandler(style.textDecorationProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setTextDecorationProperty, TextBaseStyler.resetTextDecorationProperty), "TextBase");
        style.registerHandler(style.textTransformProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setTextTransformProperty, TextBaseStyler.resetTextTransformProperty), "TextBase");
        style.registerHandler(style.whiteSpaceProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setWhiteSpaceProperty, TextBaseStyler.resetWhiteSpaceProperty), "TextBase");
        style.registerHandler(style.letterSpacingProperty, new style.StylePropertyChangedHandler(TextBaseStyler.setLetterSpacingProperty, TextBaseStyler.resetLetterSpacingProperty, TextBaseStyler.getLetterSpacingProperty), "TextBase");
    };
    return TextBaseStyler;
}());
exports.TextBaseStyler = TextBaseStyler;

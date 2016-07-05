var utils = require("utils/utils");
var common = require("./background-common");
var types = require("utils/types");
var cssValue = require("css-value");
var button;
var style;
function ensureLazyRequires() {
    if (!button) {
        button = require("ui/button");
    }
    if (!style) {
        style = require("./style");
    }
}
global.moduleMerge(common, exports);
var ad;
(function (ad) {
    var SDK;
    function getSDK() {
        if (!SDK) {
            SDK = android.os.Build.VERSION.SDK_INT;
        }
        return SDK;
    }
    var _defaultBackgrounds = new Map();
    function onBackgroundOrBorderPropertyChanged(v) {
        var nativeView = v._nativeView;
        var cache = v._nativeView;
        if (!nativeView) {
            return;
        }
        ensureLazyRequires();
        var clipPathValue = v.style._getValue(style.clipPathProperty);
        var backgroundValue = v.style._getValue(style.backgroundInternalProperty);
        var borderWidth = v.borderWidth;
        var bkg = nativeView.getBackground();
        var density = utils.layout.getDisplayDensity();
        if (v instanceof button.Button && !types.isNullOrUndefined(bkg) && types.isFunction(bkg.setColorFilter) &&
            v.borderWidth === 0 && v.borderRadius === 0 && !clipPathValue &&
            types.isNullOrUndefined(v.style._getValue(style.backgroundImageProperty)) &&
            !types.isNullOrUndefined(v.style._getValue(style.backgroundColorProperty))) {
            var backgroundColor = bkg.backgroundColor = v.style._getValue(style.backgroundColorProperty).android;
            bkg.setColorFilter(backgroundColor, android.graphics.PorterDuff.Mode.SRC_IN);
            bkg.backgroundColor = backgroundColor;
        }
        else if (v.borderWidth || v.borderRadius || clipPathValue || !backgroundValue.isEmpty()) {
            if (!(bkg instanceof org.nativescript.widgets.BorderDrawable)) {
                bkg = new org.nativescript.widgets.BorderDrawable(density);
                var viewClass = types.getClass(v);
                if (!(v instanceof button.Button) && !_defaultBackgrounds.has(viewClass)) {
                    _defaultBackgrounds.set(viewClass, nativeView.getBackground());
                }
                nativeView.setBackground(bkg);
            }
            bkg.refresh(v.borderWidth, v.borderColor ? v.borderColor.android : 0, v.borderRadius, clipPathValue, (backgroundValue.color && backgroundValue.color.android) ? backgroundValue.color.android : 0, (backgroundValue.image && backgroundValue.image.android) ? backgroundValue.image.android : null, (backgroundValue.image && backgroundValue.image.android) ? backgroundValue.image.width : 0, (backgroundValue.image && backgroundValue.image.android) ? backgroundValue.image.height : 0, backgroundValue.repeat, backgroundValue.position, backgroundValue.position ? createNativeCSSValueArray(backgroundValue.position) : null, backgroundValue.size, backgroundValue.size ? createNativeCSSValueArray(backgroundValue.size) : null);
            if ((v.borderWidth || v.borderRadius || clipPathValue) && getSDK() < 18) {
                cache.layerType = cache.getLayerType();
                cache.setLayerType(android.view.View.LAYER_TYPE_SOFTWARE, null);
            }
        }
        else {
            if (v instanceof button.Button) {
                var nativeButton = new android.widget.Button(nativeView.getContext());
                nativeView.setBackground(nativeButton.getBackground());
            }
            else {
                var viewClass = types.getClass(v);
                if (_defaultBackgrounds.has(viewClass)) {
                    nativeView.setBackground(_defaultBackgrounds.get(viewClass));
                }
            }
            if (cache.layerType !== undefined) {
                cache.setLayerType(cache.layerType, null);
                cache.layerType = undefined;
            }
        }
        nativeView.setPadding(Math.round((borderWidth + v.style.paddingLeft) * density), Math.round((borderWidth + v.style.paddingTop) * density), Math.round((borderWidth + v.style.paddingRight) * density), Math.round((borderWidth + v.style.paddingBottom) * density));
    }
    ad.onBackgroundOrBorderPropertyChanged = onBackgroundOrBorderPropertyChanged;
})(ad = exports.ad || (exports.ad = {}));
function createNativeCSSValueArray(css) {
    if (!css) {
        return null;
    }
    var cssValues = cssValue(css);
    var nativeArray = Array.create(org.nativescript.widgets.CSSValue, cssValues.length);
    for (var i = 0, length_1 = cssValues.length; i < length_1; i++) {
        nativeArray[i] = new org.nativescript.widgets.CSSValue(cssValues[i].type, cssValues[i].string, cssValues[i].unit, cssValues[i].value);
    }
    return nativeArray;
}

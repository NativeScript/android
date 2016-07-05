var utils = require("utils/utils");
var common = require("./wrap-layout-common");
var enums_1 = require("ui/enums");
global.moduleMerge(common, exports);
function setNativeOrientationProperty(data) {
    var wrapLayout = data.object;
    var nativeView = wrapLayout._nativeView;
    nativeView.setOrientation(data.newValue === enums_1.Orientation.vertical ? org.nativescript.widgets.Orientation.vertical : org.nativescript.widgets.Orientation.horizontal);
}
function setNativeItemWidthProperty(data) {
    var wrapLayout = data.object;
    var nativeView = wrapLayout._nativeView;
    nativeView.setItemWidth(data.newValue * utils.layout.getDisplayDensity());
}
function setNativeItemHeightProperty(data) {
    var wrapLayout = data.object;
    var nativeView = wrapLayout._nativeView;
    nativeView.setItemHeight(data.newValue * utils.layout.getDisplayDensity());
}
common.WrapLayout.orientationProperty.metadata.onSetNativeValue = setNativeOrientationProperty;
common.WrapLayout.itemWidthProperty.metadata.onSetNativeValue = setNativeItemWidthProperty;
common.WrapLayout.itemHeightProperty.metadata.onSetNativeValue = setNativeItemHeightProperty;
var WrapLayout = (function (_super) {
    __extends(WrapLayout, _super);
    function WrapLayout() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(WrapLayout.prototype, "android", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(WrapLayout.prototype, "_nativeView", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    WrapLayout.prototype._createUI = function () {
        this._layout = new org.nativescript.widgets.WrapLayout(this._context);
    };
    return WrapLayout;
}(common.WrapLayout));
exports.WrapLayout = WrapLayout;

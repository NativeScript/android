var utils = require("utils/utils");
var common = require("./absolute-layout-common");
var view_1 = require("ui/core/view");
global.moduleMerge(common, exports);
function setNativeProperty(data, setter) {
    var view = data.object;
    if (view instanceof view_1.View) {
        var nativeView = view._nativeView;
        var lp = nativeView.getLayoutParams() || new org.nativescript.widgets.CommonLayoutParams();
        if (lp instanceof org.nativescript.widgets.CommonLayoutParams) {
            setter(lp);
            nativeView.setLayoutParams(lp);
        }
    }
}
function setNativeLeftProperty(data) {
    setNativeProperty(data, function (lp) { lp.left = data.newValue * utils.layout.getDisplayDensity(); });
}
function setNativeTopProperty(data) {
    setNativeProperty(data, function (lp) { lp.top = data.newValue * utils.layout.getDisplayDensity(); });
}
common.AbsoluteLayout.leftProperty.metadata.onSetNativeValue = setNativeLeftProperty;
common.AbsoluteLayout.topProperty.metadata.onSetNativeValue = setNativeTopProperty;
var AbsoluteLayout = (function (_super) {
    __extends(AbsoluteLayout, _super);
    function AbsoluteLayout() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(AbsoluteLayout.prototype, "android", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AbsoluteLayout.prototype, "_nativeView", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    AbsoluteLayout.prototype._createUI = function () {
        this._layout = new org.nativescript.widgets.AbsoluteLayout(this._context);
    };
    return AbsoluteLayout;
}(common.AbsoluteLayout));
exports.AbsoluteLayout = AbsoluteLayout;

var common = require("./stack-layout-common");
var enums_1 = require("ui/enums");
global.moduleMerge(common, exports);
function setNativeOrientationProperty(data) {
    var stackLayout = data.object;
    var nativeView = stackLayout._nativeView;
    nativeView.setOrientation(data.newValue === enums_1.Orientation.vertical ? org.nativescript.widgets.Orientation.vertical : org.nativescript.widgets.Orientation.horizontal);
}
common.StackLayout.orientationProperty.metadata.onSetNativeValue = setNativeOrientationProperty;
var StackLayout = (function (_super) {
    __extends(StackLayout, _super);
    function StackLayout() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(StackLayout.prototype, "android", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(StackLayout.prototype, "_nativeView", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    StackLayout.prototype._createUI = function () {
        this._layout = new org.nativescript.widgets.StackLayout(this._context);
    };
    return StackLayout;
}(common.StackLayout));
exports.StackLayout = StackLayout;

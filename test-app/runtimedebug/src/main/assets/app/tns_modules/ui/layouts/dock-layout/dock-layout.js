var common = require("./dock-layout-common");
var enums_1 = require("ui/enums");
var view_1 = require("ui/core/view");
global.moduleMerge(common, exports);
function setNativeDockProperty(data) {
    var view = data.object;
    if (view instanceof view_1.View) {
        var nativeView = view._nativeView;
        var lp = nativeView.getLayoutParams() || new org.nativescript.widgets.CommonLayoutParams();
        if (lp instanceof org.nativescript.widgets.CommonLayoutParams) {
            switch (data.newValue) {
                case enums_1.Dock.left:
                    lp.dock = org.nativescript.widgets.Dock.left;
                    break;
                case enums_1.Dock.top:
                    lp.dock = org.nativescript.widgets.Dock.top;
                    break;
                case enums_1.Dock.right:
                    lp.dock = org.nativescript.widgets.Dock.right;
                    break;
                case enums_1.Dock.bottom:
                    lp.dock = org.nativescript.widgets.Dock.bottom;
                    break;
                default:
                    throw new Error("Invalid dock value: " + data.newValue + " on element: " + view);
            }
            nativeView.setLayoutParams(lp);
        }
    }
}
common.DockLayout.dockProperty.metadata.onSetNativeValue = setNativeDockProperty;
function setNativeStretchLastChildProperty(data) {
    var dockLayout = data.object;
    var nativeView = dockLayout._nativeView;
    nativeView.setStretchLastChild(data.newValue);
}
common.DockLayout.stretchLastChildProperty.metadata.onSetNativeValue = setNativeStretchLastChildProperty;
var DockLayout = (function (_super) {
    __extends(DockLayout, _super);
    function DockLayout() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(DockLayout.prototype, "android", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(DockLayout.prototype, "_nativeView", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    DockLayout.prototype._createUI = function () {
        this._layout = new org.nativescript.widgets.DockLayout(this._context);
    };
    return DockLayout;
}(common.DockLayout));
exports.DockLayout = DockLayout;

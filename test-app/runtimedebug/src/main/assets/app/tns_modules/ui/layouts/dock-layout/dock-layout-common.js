var platform = require("platform");
var enums_1 = require("ui/enums");
var layout_base_1 = require("ui/layouts/layout-base");
var view_1 = require("ui/core/view");
var proxy_1 = require("ui/core/proxy");
var dependency_observable_1 = require("ui/core/dependency-observable");
var special_properties_1 = require("ui/builder/special-properties");
var AffectsLayout = platform.device.os === platform.platformNames.android ? dependency_observable_1.PropertyMetadataSettings.None : dependency_observable_1.PropertyMetadataSettings.AffectsLayout;
function isDockValid(value) {
    return value === enums_1.Dock.left || value === enums_1.Dock.top || value === enums_1.Dock.right || value === enums_1.Dock.bottom;
}
function validateArgs(element) {
    if (!element) {
        throw new Error("element cannot be null or undefinied.");
    }
    return element;
}
special_properties_1.registerSpecialProperty("dock", function (instance, propertyValue) {
    DockLayout.setDock(instance, propertyValue);
});
var DockLayout = (function (_super) {
    __extends(DockLayout, _super);
    function DockLayout() {
        _super.apply(this, arguments);
    }
    DockLayout.onDockPropertyChanged = function (data) {
        var view = data.object;
        if (view instanceof view_1.View) {
            var layout = view.parent;
            if (layout instanceof DockLayout) {
                layout.onDockChanged(view, data.oldValue, data.newValue);
            }
        }
    };
    DockLayout.getDock = function (element) {
        return validateArgs(element)._getValue(DockLayout.dockProperty);
    };
    DockLayout.setDock = function (element, value) {
        validateArgs(element)._setValue(DockLayout.dockProperty, value);
    };
    Object.defineProperty(DockLayout.prototype, "stretchLastChild", {
        get: function () {
            return this._getValue(DockLayout.stretchLastChildProperty);
        },
        set: function (value) {
            this._setValue(DockLayout.stretchLastChildProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    DockLayout.prototype.onDockChanged = function (view, oldValue, newValue) {
    };
    DockLayout.dockProperty = new dependency_observable_1.Property("dock", "DockLayout", new proxy_1.PropertyMetadata(enums_1.Dock.left, undefined, DockLayout.onDockPropertyChanged, isDockValid));
    DockLayout.stretchLastChildProperty = new dependency_observable_1.Property("stretchLastChild", "DockLayout", new proxy_1.PropertyMetadata(true, AffectsLayout));
    return DockLayout;
}(layout_base_1.LayoutBase));
exports.DockLayout = DockLayout;

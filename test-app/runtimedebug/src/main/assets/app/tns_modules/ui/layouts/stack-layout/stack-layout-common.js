var platform = require("platform");
var layout_base_1 = require("ui/layouts/layout-base");
var enums_1 = require("ui/enums");
var proxy_1 = require("ui/core/proxy");
var dependency_observable_1 = require("ui/core/dependency-observable");
var AffectsLayout = platform.device.os === platform.platformNames.android ? dependency_observable_1.PropertyMetadataSettings.None : dependency_observable_1.PropertyMetadataSettings.AffectsLayout;
function validateOrientation(value) {
    return value === enums_1.Orientation.vertical || value === enums_1.Orientation.horizontal;
}
var StackLayout = (function (_super) {
    __extends(StackLayout, _super);
    function StackLayout() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(StackLayout.prototype, "orientation", {
        get: function () {
            return this._getValue(StackLayout.orientationProperty);
        },
        set: function (value) {
            this._setValue(StackLayout.orientationProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    StackLayout.orientationProperty = new dependency_observable_1.Property("orientation", "StackLayout", new proxy_1.PropertyMetadata(enums_1.Orientation.vertical, AffectsLayout, undefined, validateOrientation));
    return StackLayout;
}(layout_base_1.LayoutBase));
exports.StackLayout = StackLayout;

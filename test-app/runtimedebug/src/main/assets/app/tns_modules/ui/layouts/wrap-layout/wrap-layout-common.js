var platform = require("platform");
var layout_base_1 = require("ui/layouts/layout-base");
var enums_1 = require("ui/enums");
var proxy_1 = require("ui/core/proxy");
var dependency_observable_1 = require("ui/core/dependency-observable");
var AffectsLayout = platform.device.os === platform.platformNames.android ? dependency_observable_1.PropertyMetadataSettings.None : dependency_observable_1.PropertyMetadataSettings.AffectsLayout;
function isWidthHeightValid(value) {
    return (value >= 0.0 && value !== Number.POSITIVE_INFINITY);
}
function isValidOrientation(value) {
    return value === enums_1.Orientation.vertical || value === enums_1.Orientation.horizontal;
}
var WrapLayout = (function (_super) {
    __extends(WrapLayout, _super);
    function WrapLayout() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(WrapLayout.prototype, "orientation", {
        get: function () {
            return this._getValue(WrapLayout.orientationProperty);
        },
        set: function (value) {
            this._setValue(WrapLayout.orientationProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(WrapLayout.prototype, "itemWidth", {
        get: function () {
            return this._getValue(WrapLayout.itemWidthProperty);
        },
        set: function (value) {
            this._setValue(WrapLayout.itemWidthProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(WrapLayout.prototype, "itemHeight", {
        get: function () {
            return this._getValue(WrapLayout.itemHeightProperty);
        },
        set: function (value) {
            this._setValue(WrapLayout.itemHeightProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    WrapLayout.orientationProperty = new dependency_observable_1.Property("orientation", "WrapLayout", new proxy_1.PropertyMetadata(enums_1.Orientation.horizontal, AffectsLayout, undefined, isValidOrientation));
    WrapLayout.itemWidthProperty = new dependency_observable_1.Property("itemWidth", "WrapLayout", new proxy_1.PropertyMetadata(0, AffectsLayout, undefined, isWidthHeightValid));
    WrapLayout.itemHeightProperty = new dependency_observable_1.Property("itemHeight", "WrapLayout", new proxy_1.PropertyMetadata(0, AffectsLayout, undefined, isWidthHeightValid));
    return WrapLayout;
}(layout_base_1.LayoutBase));
exports.WrapLayout = WrapLayout;

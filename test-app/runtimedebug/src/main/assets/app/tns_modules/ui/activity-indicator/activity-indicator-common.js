var view_1 = require("ui/core/view");
var dependency_observable_1 = require("ui/core/dependency-observable");
var proxy_1 = require("ui/core/proxy");
var busyProperty = new dependency_observable_1.Property("busy", "ActivityIndicator", new proxy_1.PropertyMetadata(false));
var ActivityIndicator = (function (_super) {
    __extends(ActivityIndicator, _super);
    function ActivityIndicator() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(ActivityIndicator.prototype, "busy", {
        get: function () {
            return this._getValue(ActivityIndicator.busyProperty);
        },
        set: function (value) {
            this._setValue(ActivityIndicator.busyProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    ActivityIndicator.busyProperty = busyProperty;
    return ActivityIndicator;
}(view_1.View));
exports.ActivityIndicator = ActivityIndicator;

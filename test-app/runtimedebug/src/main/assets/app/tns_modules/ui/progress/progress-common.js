var view_1 = require("ui/core/view");
var dependency_observable_1 = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var Progress = (function (_super) {
    __extends(Progress, _super);
    function Progress() {
        _super.call(this);
        this.maxValue = 100;
        this.value = 0;
    }
    Object.defineProperty(Progress.prototype, "maxValue", {
        get: function () {
            return this._getValue(Progress.maxValueProperty);
        },
        set: function (newMaxValue) {
            this._setValue(Progress.maxValueProperty, newMaxValue);
            if (this.value > newMaxValue) {
                this.value = newMaxValue;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Progress.prototype, "value", {
        get: function () {
            return this._getValue(Progress.valueProperty);
        },
        set: function (value) {
            value = Math.min(value, this.maxValue);
            this._setValue(Progress.valueProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Progress.valueProperty = new dependency_observable_1.Property("value", "Progress", new proxy.PropertyMetadata(0));
    Progress.maxValueProperty = new dependency_observable_1.Property("maxValue", "Progress", new proxy.PropertyMetadata(100));
    return Progress;
}(view_1.View));
exports.Progress = Progress;

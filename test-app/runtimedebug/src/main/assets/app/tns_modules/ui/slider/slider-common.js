var view = require("ui/core/view");
var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var Slider = (function (_super) {
    __extends(Slider, _super);
    function Slider() {
        _super.call(this);
    }
    Object.defineProperty(Slider.prototype, "value", {
        get: function () {
            return this._getValue(Slider.valueProperty);
        },
        set: function (value) {
            var newValue = value;
            newValue = Math.max(newValue, this.minValue);
            newValue = Math.min(newValue, this.maxValue);
            this._setValue(Slider.valueProperty, newValue);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Slider.prototype, "minValue", {
        get: function () {
            return this._getValue(Slider.minValueProperty);
        },
        set: function (newValue) {
            this._setValue(Slider.minValueProperty, newValue);
            if (newValue > this.maxValue) {
                this._setValue(Slider.maxValueProperty, newValue);
            }
            if (newValue > this.value) {
                this._setValue(Slider.valueProperty, newValue);
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Slider.prototype, "maxValue", {
        get: function () {
            return this._getValue(Slider.maxValueProperty);
        },
        set: function (newValue) {
            this._setValue(Slider.maxValueProperty, newValue);
            if (newValue < this.minValue) {
                this._setValue(Slider.minValueProperty, newValue);
            }
            if (newValue < this.value) {
                this._setValue(Slider.valueProperty, newValue);
            }
        },
        enumerable: true,
        configurable: true
    });
    Slider.valueProperty = new dependencyObservable.Property("value", "Slider", new proxy.PropertyMetadata(0));
    Slider.minValueProperty = new dependencyObservable.Property("minValue", "Slider", new proxy.PropertyMetadata(0));
    Slider.maxValueProperty = new dependencyObservable.Property("maxValue", "Slider", new proxy.PropertyMetadata(100));
    return Slider;
}(view.View));
exports.Slider = Slider;

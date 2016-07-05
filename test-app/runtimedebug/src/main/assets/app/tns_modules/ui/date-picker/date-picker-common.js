var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var view = require("ui/core/view");
var DatePicker = (function (_super) {
    __extends(DatePicker, _super);
    function DatePicker() {
        _super.call(this);
    }
    Object.defineProperty(DatePicker.prototype, "year", {
        get: function () {
            return this._getValue(DatePicker.yearProperty);
        },
        set: function (value) {
            this._setValue(DatePicker.yearProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(DatePicker.prototype, "month", {
        get: function () {
            return this._getValue(DatePicker.monthProperty);
        },
        set: function (value) {
            this._setValue(DatePicker.monthProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(DatePicker.prototype, "day", {
        get: function () {
            return this._getValue(DatePicker.dayProperty);
        },
        set: function (value) {
            this._setValue(DatePicker.dayProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(DatePicker.prototype, "maxDate", {
        get: function () {
            return this._getValue(DatePicker.maxDateProperty);
        },
        set: function (value) {
            this._setValue(DatePicker.maxDateProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(DatePicker.prototype, "minDate", {
        get: function () {
            return this._getValue(DatePicker.minDateProperty);
        },
        set: function (value) {
            this._setValue(DatePicker.minDateProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(DatePicker.prototype, "date", {
        get: function () {
            return this._getValue(DatePicker.dateProperty);
        },
        set: function (value) {
            this._setValue(DatePicker.dateProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    DatePicker.yearProperty = new dependencyObservable.Property("year", "DatePicker", new proxy.PropertyMetadata(undefined));
    DatePicker.monthProperty = new dependencyObservable.Property("month", "DatePicker", new proxy.PropertyMetadata(undefined));
    DatePicker.dayProperty = new dependencyObservable.Property("day", "DatePicker", new proxy.PropertyMetadata(undefined));
    DatePicker.maxDateProperty = new dependencyObservable.Property("maxDate", "DatePicker", new proxy.PropertyMetadata(undefined));
    DatePicker.minDateProperty = new dependencyObservable.Property("minDate", "DatePicker", new proxy.PropertyMetadata(undefined));
    DatePicker.dateProperty = new dependencyObservable.Property("date", "DatePicker", new proxy.PropertyMetadata(undefined));
    return DatePicker;
}(view.View));
exports.DatePicker = DatePicker;

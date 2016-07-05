var common = require("./date-picker-common");
var types = require("utils/types");
function onYearPropertyChanged(data) {
    var picker = data.object;
    if (picker.android && picker.android.getYear() !== data.newValue) {
        updateNativeDate(picker);
    }
}
common.DatePicker.yearProperty.metadata.onSetNativeValue = onYearPropertyChanged;
function onMonthPropertyChanged(data) {
    var picker = data.object;
    if (picker.android && picker.android.getMonth() !== (data.newValue - 1)) {
        updateNativeDate(picker);
    }
}
common.DatePicker.monthProperty.metadata.onSetNativeValue = onMonthPropertyChanged;
function onDayPropertyChanged(data) {
    var picker = data.object;
    if (picker.android && picker.android.getDayOfMonth !== data.newValue) {
        updateNativeDate(picker);
    }
}
common.DatePicker.dayProperty.metadata.onSetNativeValue = onDayPropertyChanged;
function updateNativeDate(picker) {
    var year = types.isNumber(picker.year) ? picker.year : picker.android.getYear();
    var month = types.isNumber(picker.month) ? (picker.month - 1) : picker.android.getMonth();
    var day = types.isNumber(picker.day) ? picker.day : picker.android.getDayOfMonth();
    picker.date = new Date(year, month, day);
}
function onMaxDatePropertyChanged(data) {
    var picker = data.object;
    var newValue = data.newValue.getTime();
    if (picker.android && picker.android.getMaxDate() !== newValue) {
        picker.android.setMaxDate(newValue);
    }
}
common.DatePicker.maxDateProperty.metadata.onSetNativeValue = onMaxDatePropertyChanged;
function onMinDatePropertyChanged(data) {
    var picker = data.object;
    var newValue = data.newValue.getTime();
    if (picker.android && picker.android.getMinDate() !== newValue) {
        picker.android.setMinDate(newValue);
    }
}
common.DatePicker.minDateProperty.metadata.onSetNativeValue = onMinDatePropertyChanged;
function onDatePropertyChanged(data) {
    var picker = data.object;
    var newValue = data.newValue;
    if (picker.android && (picker.android.getDayOfMonth() !== newValue.getDay()
        || picker.android.getMonth() !== newValue.getMonth()
        || picker.android.getYear() !== newValue.getFullYear())) {
        picker.android.updateDate(newValue.getFullYear(), newValue.getMonth(), newValue.getDate());
    }
}
common.DatePicker.dateProperty.metadata.onSetNativeValue = onDatePropertyChanged;
global.moduleMerge(common, exports);
var DatePicker = (function (_super) {
    __extends(DatePicker, _super);
    function DatePicker() {
        _super.call(this);
        var that = new WeakRef(this);
        this._listener = new android.widget.DatePicker.OnDateChangedListener({
            get owner() {
                return that.get();
            },
            onDateChanged: function (picker, year, month, day) {
                if (this.owner) {
                    var dateIsChanged = false;
                    if (year !== this.owner.year) {
                        this.owner._onPropertyChangedFromNative(common.DatePicker.yearProperty, year);
                        dateIsChanged = true;
                    }
                    if ((month + 1) !== this.owner.month) {
                        this.owner._onPropertyChangedFromNative(common.DatePicker.monthProperty, month + 1);
                        dateIsChanged = true;
                    }
                    if (day !== this.owner.day) {
                        this.owner._onPropertyChangedFromNative(common.DatePicker.dayProperty, day);
                        dateIsChanged = true;
                    }
                    if (dateIsChanged) {
                        this.owner._onPropertyChangedFromNative(common.DatePicker.dateProperty, new Date(year, month, day));
                    }
                }
            }
        });
    }
    Object.defineProperty(DatePicker.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    DatePicker.prototype._createUI = function () {
        this._android = new android.widget.DatePicker(this._context);
        this._android.setCalendarViewShown(false);
        this._android.init(0, 0, 0, this._listener);
    };
    return DatePicker;
}(common.DatePicker));
exports.DatePicker = DatePicker;

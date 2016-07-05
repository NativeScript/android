var common = require("./time-picker-common");
global.moduleMerge(common, exports);
var TimePicker = (function (_super) {
    __extends(TimePicker, _super);
    function TimePicker() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(TimePicker.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    TimePicker.prototype._createUI = function () {
        this._android = new android.widget.TimePicker(this._context);
        var that = new WeakRef(this);
        this._listener = new android.widget.TimePicker.OnTimeChangedListener({
            get owner() {
                return that.get();
            },
            onTimeChanged: function (picker, hour, minute) {
                if (this.owner) {
                    var validTime = common.getValidTime(this.owner, hour, minute);
                    this.owner._setNativeValueSilently(validTime.hour, validTime.minute);
                    this.owner._onPropertyChangedFromNative(common.TimePicker.timeProperty, new Date(0, 0, 0, validTime.hour, validTime.minute));
                }
            }
        });
        this._android.setOnTimeChangedListener(this._listener);
        var c = java.util.Calendar.getInstance();
        if (this.hour === common.TimePicker.hourProperty.defaultValue) {
            this.hour = c.get(java.util.Calendar.HOUR_OF_DAY);
        }
        if (this.minute === common.TimePicker.minuteProperty.defaultValue) {
            this.minute = c.get(java.util.Calendar.MINUTE);
        }
        var validTime = common.getValidTime(this, this.hour, this.minute);
        this._setNativeValueSilently(validTime.hour, validTime.minute);
    };
    TimePicker.prototype._setNativeValueSilently = function (hour, minute) {
        if (this.android) {
            this.android.setOnTimeChangedListener(null);
            this.android.setCurrentHour(new java.lang.Integer(hour));
            this.android.setCurrentMinute(new java.lang.Integer(minute));
            this.minute = minute;
            this.hour = hour;
            this.android.setOnTimeChangedListener(this._listener);
        }
    };
    TimePicker.prototype._setNativeTime = function () {
        this._setNativeValueSilently(this.hour, this.minute);
    };
    return TimePicker;
}(common.TimePicker));
exports.TimePicker = TimePicker;

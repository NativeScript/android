var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var view = require("ui/core/view");
var types = require("utils/types");
function isHourValid(value) {
    return types.isNumber(value) && value >= 0 && value <= 23;
}
function isMinuteValid(value) {
    return types.isNumber(value) && value >= 0 && value <= 59;
}
function isMinuteIntervalValid(value) {
    return types.isNumber(value) && value >= 1 && value <= 30 && 60 % value === 0;
}
function getMinutes(hour) {
    return hour * 60;
}
function isGreaterThanMinTime(picker, hour, minute) {
    if (!types.isDefined(picker.minHour) || !types.isDefined(picker.minMinute)) {
        return true;
    }
    return getMinutes(types.isDefined(hour) ? hour : picker.hour) + (types.isDefined(minute) ? minute : picker.minute) >= getMinutes(picker.minHour) + picker.minMinute;
}
exports.isGreaterThanMinTime = isGreaterThanMinTime;
function isLessThanMaxTime(picker, hour, minute) {
    if (!types.isDefined(picker.maxHour) || !types.isDefined(picker.maxMinute)) {
        return true;
    }
    return getMinutes(types.isDefined(hour) ? hour : picker.hour) + (types.isDefined(minute) ? minute : picker.minute) <= getMinutes(picker.maxHour) + picker.maxMinute;
}
exports.isLessThanMaxTime = isLessThanMaxTime;
function isValidTime(picker) {
    return isGreaterThanMinTime(picker) && isLessThanMaxTime(picker);
}
exports.isValidTime = isValidTime;
function getValidTime(picker, hour, minute) {
    if (picker.minuteInterval > 1) {
        var minuteFloor = minute - (minute % picker.minuteInterval);
        minute = minuteFloor + (minute === minuteFloor + 1 ? picker.minuteInterval : 0);
        if (minute === 60) {
            hour++;
            minute = 0;
        }
    }
    var time = { hour: hour, minute: minute };
    if (!isGreaterThanMinTime(picker, hour, minute)) {
        time = { hour: picker.minHour, minute: picker.minMinute };
    }
    if (!isLessThanMaxTime(picker, hour, minute)) {
        time = { hour: picker.maxHour, minute: picker.maxMinute };
    }
    return time;
}
exports.getValidTime = getValidTime;
function toString(value) {
    return value < 10 ? "0" + value : "" + value;
}
function getMinMaxTimeErrorMessage(picker) {
    return "Min time: (" + toString(picker.minHour) + ":" + toString(picker.minMinute) + "), max time: (" + toString(picker.maxHour) + ":" + toString(picker.maxMinute) + ")";
}
function getErrorMessage(picker, propertyName, newValue) {
    return propertyName + " property value (" + toString(newValue) + ":" + toString(picker.minute) + ") is not valid. " + getMinMaxTimeErrorMessage(picker) + ".";
}
function onHourPropertyChanged(data) {
    var picker = data.object;
    if (isValidTime(picker)) {
        picker._setNativeTime();
        if (picker.time) {
            picker.time.setHours(picker.hour);
        }
        else {
            picker.time = new Date(0, 0, 0, picker.hour, picker.minute);
        }
    }
    else {
        throw new Error(getErrorMessage(picker, "Hour", data.newValue));
    }
}
function onMinutePropertyChanged(data) {
    var picker = data.object;
    if (isValidTime(picker)) {
        picker._setNativeTime();
        if (picker.time) {
            picker.time.setMinutes(picker.minute);
        }
        else {
            picker.time = new Date(0, 0, 0, picker.hour, picker.minute);
        }
    }
    else {
        throw new Error(getErrorMessage(picker, "Minute", data.newValue));
    }
}
function onTimePropertyChanged(data) {
    var picker = data.object;
    var newTime = data.newValue;
    picker.hour = newTime.getHours();
    picker.minute = newTime.getMinutes();
    if (isValidTime(picker)) {
        picker._setNativeTime();
    }
    else {
        throw new Error(getErrorMessage(picker, "Time", data.newValue));
    }
}
function onMinMinutePropertyChanged(data) {
    var picker = data.object;
    if (isValidTime(picker)) {
        picker._setNativeMinTime();
    }
    else {
        throw new Error(getErrorMessage(picker, "Minute", data.newValue));
    }
}
function onMaxMinutePropertyChanged(data) {
    var picker = data.object;
    if (isValidTime(picker)) {
        picker._setNativeMaxTime();
    }
    else {
        throw new Error(getErrorMessage(picker, "Minute", data.newValue));
    }
}
function onMinHourPropertyChanged(data) {
    var picker = data.object;
    if (isValidTime(picker)) {
        picker._setNativeMinTime();
    }
    else {
        throw new Error(getErrorMessage(picker, "Hour", data.newValue));
    }
}
function onMaxHourPropertyChanged(data) {
    var picker = data.object;
    if (isValidTime(picker)) {
        picker._setNativeMaxTime();
    }
    else {
        throw new Error(getErrorMessage(picker, "Hour", data.newValue));
    }
}
function onMinuteIntervalPropertyChanged(data) {
    var picker = data.object;
    picker._setNativeMinuteIntervalTime();
}
var TimePicker = (function (_super) {
    __extends(TimePicker, _super);
    function TimePicker() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(TimePicker.prototype, "hour", {
        get: function () {
            return this._getValue(TimePicker.hourProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.hourProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TimePicker.prototype, "minute", {
        get: function () {
            return this._getValue(TimePicker.minuteProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.minuteProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TimePicker.prototype, "time", {
        get: function () {
            return this._getValue(TimePicker.timeProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.timeProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TimePicker.prototype, "minuteInterval", {
        get: function () {
            return this._getValue(TimePicker.minuteIntervalProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.minuteIntervalProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TimePicker.prototype, "maxHour", {
        get: function () {
            return this._getValue(TimePicker.maxHourProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.maxHourProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TimePicker.prototype, "maxMinute", {
        get: function () {
            return this._getValue(TimePicker.maxMinuteProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.maxMinuteProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TimePicker.prototype, "minHour", {
        get: function () {
            return this._getValue(TimePicker.minHourProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.minHourProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TimePicker.prototype, "minMinute", {
        get: function () {
            return this._getValue(TimePicker.minMinuteProperty);
        },
        set: function (value) {
            this._setValue(TimePicker.minMinuteProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    TimePicker.prototype._setNativeTime = function () {
    };
    TimePicker.prototype._setNativeMinTime = function () {
    };
    TimePicker.prototype._setNativeMaxTime = function () {
    };
    TimePicker.prototype._setNativeMinuteIntervalTime = function () {
    };
    TimePicker.hourProperty = new dependencyObservable.Property("hour", "TimePicker", new proxy.PropertyMetadata(0, dependencyObservable.PropertyMetadataSettings.None, onHourPropertyChanged, isHourValid));
    TimePicker.minHourProperty = new dependencyObservable.Property("minHour", "TimePicker", new proxy.PropertyMetadata(0, dependencyObservable.PropertyMetadataSettings.None, onMinHourPropertyChanged, isHourValid));
    TimePicker.maxHourProperty = new dependencyObservable.Property("maxHour", "TimePicker", new proxy.PropertyMetadata(23, dependencyObservable.PropertyMetadataSettings.None, onMaxHourPropertyChanged, isHourValid));
    TimePicker.minuteProperty = new dependencyObservable.Property("minute", "TimePicker", new proxy.PropertyMetadata(0, dependencyObservable.PropertyMetadataSettings.None, onMinutePropertyChanged, isMinuteValid));
    TimePicker.minMinuteProperty = new dependencyObservable.Property("minMinute", "TimePicker", new proxy.PropertyMetadata(0, dependencyObservable.PropertyMetadataSettings.None, onMinMinutePropertyChanged, isMinuteValid));
    TimePicker.maxMinuteProperty = new dependencyObservable.Property("maxMinute", "TimePicker", new proxy.PropertyMetadata(59, dependencyObservable.PropertyMetadataSettings.None, onMaxMinutePropertyChanged, isMinuteValid));
    TimePicker.minuteIntervalProperty = new dependencyObservable.Property("minuteInterval", "TimePicker", new proxy.PropertyMetadata(1, dependencyObservable.PropertyMetadataSettings.None, onMinuteIntervalPropertyChanged, isMinuteIntervalValid));
    TimePicker.timeProperty = new dependencyObservable.Property("time", "TimePicker", new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.None, onTimePropertyChanged, isValidTime));
    return TimePicker;
}(view.View));
exports.TimePicker = TimePicker;

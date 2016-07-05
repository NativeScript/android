var types = require("utils/types");
var _wrappedIndex = 0;
var WrappedValue = (function () {
    function WrappedValue(value) {
        this._wrapped = value;
    }
    Object.defineProperty(WrappedValue.prototype, "wrapped", {
        get: function () {
            return this._wrapped;
        },
        set: function (value) {
            this._wrapped = value;
        },
        enumerable: true,
        configurable: true
    });
    WrappedValue.unwrap = function (value) {
        if (value && value.wrapped) {
            return value.wrapped;
        }
        return value;
    };
    WrappedValue.wrap = function (value) {
        var w = _wrappedValues[_wrappedIndex++ % 5];
        w.wrapped = value;
        return w;
    };
    return WrappedValue;
}());
exports.WrappedValue = WrappedValue;
var _wrappedValues = [
    new WrappedValue(null),
    new WrappedValue(null),
    new WrappedValue(null),
    new WrappedValue(null),
    new WrappedValue(null)
];
var Observable = (function () {
    function Observable(json) {
        this._observers = {};
        this.disableNotifications = {};
        if (json) {
            this._map = new Map();
            for (var prop in json) {
                if (json.hasOwnProperty(prop)) {
                    this._defineNewProperty(prop);
                    this.set(prop, json[prop]);
                }
            }
        }
    }
    Observable.prototype._defineNewProperty = function (propertyName) {
        Object.defineProperty(this, propertyName, {
            get: function () {
                return this._map.get(propertyName);
            },
            set: function (value) {
                this._map.set(propertyName, value);
                this.notify(this._createPropertyChangeData(propertyName, value));
            },
            enumerable: true,
            configurable: true
        });
    };
    Object.defineProperty(Observable.prototype, "typeName", {
        get: function () {
            return types.getClass(this);
        },
        enumerable: true,
        configurable: true
    });
    Observable.prototype.on = function (eventNames, callback, thisArg) {
        this.addEventListener(eventNames, callback, thisArg);
    };
    Observable.prototype.off = function (eventNames, callback, thisArg) {
        this.removeEventListener(eventNames, callback, thisArg);
    };
    Observable.prototype.addEventListener = function (eventNames, callback, thisArg) {
        if (!types.isString(eventNames)) {
            throw new TypeError("Events name(s) must be string.");
        }
        types.verifyCallback(callback);
        var events = eventNames.split(",");
        for (var i = 0, l = events.length; i < l; i++) {
            var event = events[i].trim();
            var list = this._getEventList(event, true);
            list.push({
                callback: callback,
                thisArg: thisArg
            });
        }
    };
    Observable.prototype.removeEventListener = function (eventNames, callback, thisArg) {
        if (!types.isString(eventNames)) {
            throw new TypeError("Events name(s) must be string.");
        }
        var events = eventNames.split(",");
        for (var i = 0, l = events.length; i < l; i++) {
            var event = events[i].trim();
            if (callback) {
                var list = this._getEventList(event, false);
                if (list) {
                    var index = this._indexOfListener(list, callback, thisArg);
                    if (index >= 0) {
                        list.splice(index, 1);
                    }
                    if (list.length === 0) {
                        delete this._observers[event];
                    }
                }
            }
            else {
                this._observers[event] = undefined;
                delete this._observers[event];
            }
        }
    };
    Observable.prototype.notifyPropertyChange = function (propertyName, newValue) {
        this.notify(this._createPropertyChangeData(propertyName, newValue));
    };
    Observable.prototype.set = function (name, value) {
        if (this[name] === value) {
            return;
        }
        var data = this._createPropertyChangeData(name, value);
        this._setCore(data);
        this.notify(data);
    };
    Observable.prototype.get = function (name) {
        return this[name];
    };
    Observable.prototype._setCore = function (data) {
        this.disableNotifications[data.propertyName] = true;
        var newValue = WrappedValue.unwrap(data.value);
        this[data.propertyName] = newValue;
        delete this.disableNotifications[data.propertyName];
    };
    Observable.prototype.notify = function (data) {
        if (this.disableNotifications[data.propertyName]) {
            return;
        }
        var observers = this._getEventList(data.eventName);
        if (!observers) {
            return;
        }
        var i;
        var entry;
        var observersLength = observers.length;
        for (i = observersLength - 1; i >= 0; i--) {
            entry = observers[i];
            if (entry.thisArg) {
                entry.callback.apply(entry.thisArg, [data]);
            }
            else {
                entry.callback(data);
            }
        }
    };
    Observable.prototype.hasListeners = function (eventName) {
        return eventName in this._observers;
    };
    Observable.prototype._createPropertyChangeData = function (name, value) {
        return {
            eventName: Observable.propertyChangeEvent,
            propertyName: name,
            object: this,
            value: value
        };
    };
    Observable.prototype._emit = function (eventNames) {
        var events = eventNames.split(",");
        for (var i = 0, l = events.length; i < l; i++) {
            var event = events[i].trim();
            this.notify({ eventName: event, object: this });
        }
    };
    Observable.prototype._getEventList = function (eventName, createIfNeeded) {
        if (!eventName) {
            throw new TypeError("EventName must be valid string.");
        }
        var list = this._observers[eventName];
        if (!list && createIfNeeded) {
            list = [];
            this._observers[eventName] = list;
        }
        return list;
    };
    Observable.prototype._indexOfListener = function (list, callback, thisArg) {
        var i;
        var entry;
        for (i = 0; i < list.length; i++) {
            entry = list[i];
            if (thisArg) {
                if (entry.callback === callback && entry.thisArg === thisArg) {
                    return i;
                }
            }
            else {
                if (entry.callback === callback) {
                    return i;
                }
            }
        }
        return -1;
    };
    Observable.prototype.toString = function () {
        return this.typeName;
    };
    Observable.propertyChangeEvent = "propertyChange";
    return Observable;
}());
exports.Observable = Observable;

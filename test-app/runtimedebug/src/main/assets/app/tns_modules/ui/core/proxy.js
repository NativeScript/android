var bindable = require("ui/core/bindable");
var dependencyObservable = require("ui/core/dependency-observable");
var types = require("utils/types");
var observable = require("data/observable");
var PropertyMetadata = (function (_super) {
    __extends(PropertyMetadata, _super);
    function PropertyMetadata(defaultValue, options, onChanged, onValidateValue, onSetNativeValue) {
        _super.call(this, defaultValue, options, onChanged, onValidateValue);
        this._onSetNativeValue = onSetNativeValue;
    }
    Object.defineProperty(PropertyMetadata.prototype, "onSetNativeValue", {
        get: function () {
            return this._onSetNativeValue;
        },
        set: function (value) {
            this._onSetNativeValue = value;
        },
        enumerable: true,
        configurable: true
    });
    return PropertyMetadata;
}(dependencyObservable.PropertyMetadata));
exports.PropertyMetadata = PropertyMetadata;
var ProxyObject = (function (_super) {
    __extends(ProxyObject, _super);
    function ProxyObject() {
        _super.apply(this, arguments);
        this._updatingJSPropertiesDict = {};
    }
    Object.defineProperty(ProxyObject.prototype, "android", {
        get: function () {
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ProxyObject.prototype, "ios", {
        get: function () {
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    ProxyObject.prototype._onPropertyChanged = function (property, oldValue, newValue) {
        _super.prototype._onPropertyChanged.call(this, property, oldValue, newValue);
        this._trySetNativeValue(property, oldValue, newValue);
    };
    ProxyObject.prototype._onPropertyChangedFromNative = function (property, newValue) {
        if (this._updatingJSPropertiesDict[property.name]) {
            return;
        }
        this._updatingJSPropertiesDict[property.name] = true;
        this._setValue(property, newValue);
        delete this._updatingJSPropertiesDict[property.name];
    };
    ProxyObject.prototype._syncNativeProperties = function () {
        var that = this;
        var eachPropertyCallback = function (property) {
            that._trySetNativeValue(property);
            return true;
        };
        this._eachSetProperty(eachPropertyCallback);
    };
    ProxyObject.prototype._canApplyNativeProperty = function () {
        return false;
    };
    ProxyObject.prototype._trySetNativeValue = function (property, oldValue, newValue) {
        if (this._updatingJSPropertiesDict[property.name]) {
            return;
        }
        if (!this._canApplyNativeProperty()) {
            return;
        }
        var metadata = property.metadata;
        if (!(metadata instanceof PropertyMetadata)) {
            return;
        }
        var proxyMetadata = metadata;
        if (proxyMetadata.onSetNativeValue) {
            if (types.isUndefined(newValue)) {
                newValue = this._getValue(property);
            }
            proxyMetadata.onSetNativeValue({
                object: this,
                property: property,
                eventName: observable.Observable.propertyChangeEvent,
                newValue: newValue,
                oldValue: oldValue
            });
        }
    };
    return ProxyObject;
}(bindable.Bindable));
exports.ProxyObject = ProxyObject;

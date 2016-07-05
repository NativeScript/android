var types = require("utils/types");
var observable = require("ui/core/dependency-observable");
var propertiesByName = {};
var propertiesByCssName = {};
var callbackByShorthandName = new Map();
var inheritableProperties = [];
function registerProperty(property) {
    if (propertiesByCssName[property.cssName]) {
        throw new Error("Property with name " + property.cssName + " is already registered!");
    }
    propertiesByCssName[property.cssName] = property;
    propertiesByName[property.name] = property;
    if (property.inheritable) {
        inheritableProperties.push(property);
    }
}
function withStyleProperty(name, value, resolvedCallback) {
    var property = getPropertyByCssName(name);
    if (property) {
        resolvedCallback(property, value);
    }
    else {
        var pairs = getShorthandPairs(name, value);
        if (pairs) {
            for (var j = 0; j < pairs.length; j++) {
                var pair = pairs[j];
                resolvedCallback(pair.property, pair.value);
            }
        }
        else {
            resolvedCallback(name, value);
        }
    }
}
exports.withStyleProperty = withStyleProperty;
function getShorthandPairs(name, value) {
    var callback = callbackByShorthandName.get(name);
    if (callback) {
        return callback(value);
    }
    return undefined;
}
exports.getShorthandPairs = getShorthandPairs;
function registerShorthandCallback(name, callback) {
    if (callbackByShorthandName.has(name)) {
        throw new Error("Shorthand callback already registered for property: " + name);
    }
    callbackByShorthandName.set(name, callback);
}
exports.registerShorthandCallback = registerShorthandCallback;
function getPropertyByName(name) {
    return propertiesByName[name];
}
exports.getPropertyByName = getPropertyByName;
function getPropertyByCssName(name) {
    return propertiesByCssName[name];
}
exports.getPropertyByCssName = getPropertyByCssName;
function eachProperty(callback) {
    types.verifyCallback(callback);
    for (var i = 0, keys = Object.keys(propertiesByName); i < keys.length; i++) {
        var key = keys[i];
        callback(propertiesByName[key]);
    }
}
exports.eachProperty = eachProperty;
function eachInheritableProperty(callback) {
    types.verifyCallback(callback);
    for (var i = 0; i < inheritableProperties.length; i++) {
        callback(inheritableProperties[i]);
    }
}
exports.eachInheritableProperty = eachInheritableProperty;
var Property = (function (_super) {
    __extends(Property, _super);
    function Property(name, cssName, metadata, valueConverter) {
        _super.call(this, name, "Style", metadata, valueConverter);
        this.cssName = cssName;
        this.cssName = cssName;
        registerProperty(this);
    }
    return Property;
}(observable.Property));
exports.Property = Property;

exports.checkKey = function (key) {
    if ("string" !== typeof key) {
        throw new Error("key: '" + key + "' must be a string");
    }
};
exports.ensureValidValue = function (value, valueType) {
    if (valueType !== typeof value) {
        throw new Error("value: '" + value + "' must be a " + valueType);
    }
};

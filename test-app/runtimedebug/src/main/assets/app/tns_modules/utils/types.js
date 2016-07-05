function isString(value) {
    return typeof value === "string" || value instanceof String;
}
exports.isString = isString;
function isNumber(value) {
    return typeof value === "number" || value instanceof Number;
}
exports.isNumber = isNumber;
function isFunction(value) {
    if (!value) {
        return false;
    }
    return typeof value === "function";
}
exports.isFunction = isFunction;
function isUndefined(value) {
    return typeof value === "undefined";
}
exports.isUndefined = isUndefined;
function isDefined(value) {
    return typeof value !== "undefined";
}
exports.isDefined = isDefined;
function isNullOrUndefined(value) {
    return (typeof value === "undefined") || (value === null);
}
exports.isNullOrUndefined = isNullOrUndefined;
function verifyCallback(value) {
    if (value && !isFunction(value)) {
        throw new TypeError("Callback must be a valid function.");
    }
}
exports.verifyCallback = verifyCallback;
var classInfosMap = new Map();
var funcNameRegex = /function (.{1,})\(/;
function getClass(object) {
    return getClassInfo(object).name;
}
exports.getClass = getClass;
function getClassInfo(object) {
    var constructor = object.constructor;
    var result = classInfosMap.get(constructor);
    if (!result) {
        result = new ClassInfo(constructor);
        classInfosMap.set(constructor, result);
    }
    return result;
}
exports.getClassInfo = getClassInfo;
function getBaseClasses(object) {
    var result = [];
    var info = getClassInfo(object);
    while (info) {
        result.push(info.name);
        info = info.baseClassInfo;
    }
    return result;
}
exports.getBaseClasses = getBaseClasses;
var ClassInfo = (function () {
    function ClassInfo(typeCosntructor) {
        this._typeCosntructor = typeCosntructor;
    }
    Object.defineProperty(ClassInfo.prototype, "name", {
        get: function () {
            if (!this._name) {
                var results = (funcNameRegex).exec(this._typeCosntructor.toString());
                this._name = (results && results.length > 1) ? results[1] : "";
            }
            return this._name;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ClassInfo.prototype, "baseClassInfo", {
        get: function () {
            if (isUndefined(this._baseClassInfo)) {
                this._baseClassInfo = ClassInfo._getBase(this);
                if (this._baseClassInfo && this._baseClassInfo.name === this.name) {
                    this._baseClassInfo = ClassInfo._getBase(this._baseClassInfo);
                }
            }
            return this._baseClassInfo;
        },
        enumerable: true,
        configurable: true
    });
    ClassInfo._getBase = function (info) {
        var result = null;
        var constructorProto = info._typeCosntructor.prototype;
        if (constructorProto.__proto__) {
            result = getClassInfo(constructorProto.__proto__);
        }
        return result;
    };
    return ClassInfo;
}());
exports.ClassInfo = ClassInfo;
function toUIString(obj) {
    return isNullOrUndefined(obj) ? "" : obj + "";
}
exports.toUIString = toUIString;

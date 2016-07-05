var types = require("utils/types");
exports.enabled = false;
var _categories = {};
var _writers = [];
var _eventListeners = [];
function enable() {
    exports.enabled = true;
}
exports.enable = enable;
function disable() {
    exports.enabled = false;
}
exports.disable = disable;
function isCategorySet(category) {
    return category in _categories;
}
exports.isCategorySet = isCategorySet;
function addWriter(writer) {
    _writers.push(writer);
}
exports.addWriter = addWriter;
function removeWriter(writer) {
    var index = _writers.indexOf(writer);
    if (index >= 0) {
        _writers.splice(index, 1);
    }
}
exports.removeWriter = removeWriter;
function clearWriters() {
    if (_writers.length > 0) {
        _writers.splice(0, _writers.length);
    }
}
exports.clearWriters = clearWriters;
function setCategories(categories) {
    _categories = {};
    addCategories(categories);
}
exports.setCategories = setCategories;
function addCategories(categories) {
    var split = categories.split(",");
    for (var i = 0; i < split.length; i++) {
        _categories[split[i].trim()] = true;
    }
}
exports.addCategories = addCategories;
function write(message, category, type) {
    var i;
    if (type === messageType.error) {
        for (i = 0; i < _writers.length; i++) {
            _writers[i].write(message, category, type);
        }
    }
    if (!exports.enabled) {
        return;
    }
    if (!(category in _categories)) {
        return;
    }
    for (i = 0; i < _writers.length; i++) {
        _writers[i].write(message, category, type);
    }
}
exports.write = write;
function notifyEvent(object, name, data) {
    if (!exports.enabled) {
        return;
    }
    var i, listener, filters;
    for (i = 0; i < _eventListeners.length; i++) {
        listener = _eventListeners[i];
        if (listener.filter) {
            filters = listener.filter.split(",");
            filters.forEach(function (value) {
                if (value.trim() === name) {
                    listener.on(object, name, data);
                }
            });
        }
        else {
            listener.on(object, name, data);
        }
    }
}
exports.notifyEvent = notifyEvent;
function addEventListener(listener) {
    _eventListeners.push(listener);
}
exports.addEventListener = addEventListener;
function removeEventListener(listener) {
    var index = _eventListeners.indexOf(listener);
    if (index >= 0) {
        _eventListeners.splice(index, 1);
    }
}
exports.removeEventListener = removeEventListener;
var messageType;
(function (messageType) {
    messageType.log = 0;
    messageType.info = 1;
    messageType.warn = 2;
    messageType.error = 3;
})(messageType = exports.messageType || (exports.messageType = {}));
var categories;
(function (categories) {
    categories.VisualTreeEvents = "VisualTreeEvents";
    categories.Layout = "Layout";
    categories.Style = "Style";
    categories.ViewHierarchy = "ViewHierarchy";
    categories.NativeLifecycle = "NativeLifecycle";
    categories.Debug = "Debug";
    categories.Navigation = "Navigation";
    categories.Test = "Test";
    categories.Binding = "Binding";
    categories.BindingError = "BindingError";
    categories.Error = "Error";
    categories.Animation = "Animation";
    categories.Transition = "Transition";
    categories.All = categories.VisualTreeEvents + "," + categories.Layout + "," + categories.Style + "," + categories.ViewHierarchy + "," + categories.NativeLifecycle + "," + categories.Debug + "," + categories.Navigation + "," + categories.Test + "," + categories.Binding + "," + categories.Error + "," + categories.Animation + "," + categories.Transition;
    categories.separator = ",";
    function concat() {
        var i;
        var result;
        for (i = 0; i < arguments.length; i++) {
            if (!result) {
                result = arguments[i];
                continue;
            }
            result = result.concat(categories.separator, arguments[i]);
        }
        return result;
    }
    categories.concat = concat;
})(categories = exports.categories || (exports.categories = {}));
var ConsoleWriter = (function () {
    function ConsoleWriter() {
    }
    ConsoleWriter.prototype.write = function (message, category, type) {
        if (!console) {
            return;
        }
        var msgType;
        if (types.isUndefined(type)) {
            msgType = messageType.log;
        }
        else {
            msgType = type;
        }
        switch (msgType) {
            case messageType.log:
                console.log(category + ": " + message);
                break;
            case messageType.info:
                console.info(category + ": " + message);
                break;
            case messageType.warn:
                console.warn(category + ": " + message);
                break;
            case messageType.error:
                console.error(category + ": " + message);
                break;
        }
    };
    return ConsoleWriter;
}());
addWriter(new ConsoleWriter());

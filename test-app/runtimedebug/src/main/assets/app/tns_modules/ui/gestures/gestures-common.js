var definition = require("ui/gestures");
(function (GestureTypes) {
    GestureTypes[GestureTypes["tap"] = 1] = "tap";
    GestureTypes[GestureTypes["doubleTap"] = 2] = "doubleTap";
    GestureTypes[GestureTypes["pinch"] = 4] = "pinch";
    GestureTypes[GestureTypes["pan"] = 8] = "pan";
    GestureTypes[GestureTypes["swipe"] = 16] = "swipe";
    GestureTypes[GestureTypes["rotation"] = 32] = "rotation";
    GestureTypes[GestureTypes["longPress"] = 64] = "longPress";
    GestureTypes[GestureTypes["touch"] = 128] = "touch";
})(exports.GestureTypes || (exports.GestureTypes = {}));
var GestureTypes = exports.GestureTypes;
(function (GestureStateTypes) {
    GestureStateTypes[GestureStateTypes["cancelled"] = 0] = "cancelled";
    GestureStateTypes[GestureStateTypes["began"] = 1] = "began";
    GestureStateTypes[GestureStateTypes["changed"] = 2] = "changed";
    GestureStateTypes[GestureStateTypes["ended"] = 3] = "ended";
})(exports.GestureStateTypes || (exports.GestureStateTypes = {}));
var GestureStateTypes = exports.GestureStateTypes;
(function (SwipeDirection) {
    SwipeDirection[SwipeDirection["right"] = 1] = "right";
    SwipeDirection[SwipeDirection["left"] = 2] = "left";
    SwipeDirection[SwipeDirection["up"] = 4] = "up";
    SwipeDirection[SwipeDirection["down"] = 8] = "down";
})(exports.SwipeDirection || (exports.SwipeDirection = {}));
var SwipeDirection = exports.SwipeDirection;
var TouchAction;
(function (TouchAction) {
    TouchAction.down = "down";
    TouchAction.up = "up";
    TouchAction.move = "move";
    TouchAction.cancel = "cancel";
})(TouchAction = exports.TouchAction || (exports.TouchAction = {}));
function observe(target, type, callback, context) {
    var observer = new definition.GesturesObserver(target, callback, context);
    observer.observe(type);
    return observer;
}
exports.observe = observe;
function toString(type, separator) {
    var types = new Array();
    if (type & definition.GestureTypes.tap) {
        types.push("tap");
    }
    if (type & definition.GestureTypes.doubleTap) {
        types.push("doubleTap");
    }
    if (type & definition.GestureTypes.pinch) {
        types.push("pinch");
    }
    if (type & definition.GestureTypes.pan) {
        types.push("pan");
    }
    if (type & definition.GestureTypes.swipe) {
        types.push("swipe");
    }
    if (type & definition.GestureTypes.rotation) {
        types.push("rotation");
    }
    if (type & definition.GestureTypes.longPress) {
        types.push("longPress");
    }
    if (type & definition.GestureTypes.touch) {
        types.push("touch");
    }
    return types.join(separator);
}
exports.toString = toString;
function fromString(type) {
    var t = type.trim().toLowerCase();
    if (t === "tap") {
        return definition.GestureTypes.tap;
    }
    else if (t === "doubletap") {
        return definition.GestureTypes.doubleTap;
    }
    else if (t === "pinch") {
        return definition.GestureTypes.pinch;
    }
    else if (t === "pan") {
        return definition.GestureTypes.pan;
    }
    else if (t === "swipe") {
        return definition.GestureTypes.swipe;
    }
    else if (t === "rotation") {
        return definition.GestureTypes.rotation;
    }
    else if (t === "longpress") {
        return definition.GestureTypes.longPress;
    }
    else if (t === "touch") {
        return definition.GestureTypes.touch;
    }
    return undefined;
}
exports.fromString = fromString;
var GesturesObserver = (function () {
    function GesturesObserver(target, callback, context) {
        this._target = target;
        this._callback = callback;
        this._context = context;
    }
    Object.defineProperty(GesturesObserver.prototype, "callback", {
        get: function () {
            return this._callback;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(GesturesObserver.prototype, "target", {
        get: function () {
            return this._target;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(GesturesObserver.prototype, "context", {
        get: function () {
            return this._context;
        },
        enumerable: true,
        configurable: true
    });
    GesturesObserver.prototype.androidOnTouchEvent = function (motionEvent) {
    };
    GesturesObserver.prototype.observe = function (type) {
    };
    GesturesObserver.prototype.disconnect = function () {
        if (this.target) {
            var list = this.target.getGestureObservers(this.type);
            if (list && list.length > 0) {
                for (var i = 0; i < list.length; i++) {
                    if (list[i].callback === this.callback) {
                        break;
                    }
                }
                list.length = 0;
                this.target._gestureObservers[this.type] = undefined;
                delete this.target._gestureObservers[this.type];
            }
        }
        this._target = null;
        this._callback = null;
        this._context = null;
    };
    return GesturesObserver;
}());
exports.GesturesObserver = GesturesObserver;

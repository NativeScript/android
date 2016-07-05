var common = require("./gestures-common");
var definition = require("ui/gestures");
var view = require("ui/core/view");
var trace = require("trace");
var utils = require("utils/utils");
global.moduleMerge(common, exports);
var SWIPE_THRESHOLD = 100;
var SWIPE_VELOCITY_THRESHOLD = 100;
var INVALID_POINTER_ID = -1;
var TO_DEGREES = (180 / Math.PI);
var GesturesObserver = (function (_super) {
    __extends(GesturesObserver, _super);
    function GesturesObserver() {
        _super.apply(this, arguments);
    }
    GesturesObserver.prototype.observe = function (type) {
        var _this = this;
        if (this.target) {
            this.type = type;
            this._onTargetLoaded = function (args) {
                if (trace.enabled) {
                    trace.write(_this.target + ".target loaded. android:" + _this.target._nativeView, "gestures");
                }
                _this._attach(_this.target, type);
            };
            this._onTargetUnloaded = function (args) {
                if (trace.enabled) {
                    trace.write(_this.target + ".target unloaded. android:" + _this.target._nativeView, "gestures");
                }
                _this._detach();
            };
            this.target.on(view.View.loadedEvent, this._onTargetLoaded);
            this.target.on(view.View.unloadedEvent, this._onTargetUnloaded);
            if (this.target.isLoaded) {
                this._attach(this.target, type);
            }
        }
    };
    GesturesObserver.prototype.disconnect = function () {
        this._detach();
        if (this.target) {
            this.target.off(view.View.loadedEvent, this._onTargetLoaded);
            this.target.off(view.View.unloadedEvent, this._onTargetUnloaded);
            this._onTargetLoaded = null;
            this._onTargetUnloaded = null;
        }
        _super.prototype.disconnect.call(this);
    };
    GesturesObserver.prototype._detach = function () {
        if (trace.enabled) {
            trace.write(this.target + "._detach() android:" + this.target._nativeView, "gestures");
        }
        this._notifyTouch = false;
        this._simpleGestureDetector = null;
        this._scaleGestureDetector = null;
        this._swipeGestureDetector = null;
        this._panGestureDetector = null;
        this._rotateGestureDetector = null;
        this._eventData = null;
    };
    GesturesObserver.prototype._attach = function (target, type) {
        if (trace.enabled) {
            trace.write(this.target + "._attach() android:" + this.target._nativeView, "gestures");
        }
        this._detach();
        if (type & definition.GestureTypes.tap || type & definition.GestureTypes.doubleTap || type & definition.GestureTypes.longPress) {
            ensureTapAndDoubleTapGestureListenerClass();
            this._simpleGestureDetector = new android.support.v4.view.GestureDetectorCompat(target._context, new TapAndDoubleTapGestureListenerClass(this, this.target, type));
        }
        if (type & definition.GestureTypes.pinch) {
            ensurePinchGestureListenerClass();
            this._scaleGestureDetector = new android.view.ScaleGestureDetector(target._context, new PinchGestureListenerClass(this, this.target));
        }
        if (type & definition.GestureTypes.swipe) {
            ensureSwipeGestureListenerClass();
            this._swipeGestureDetector = new android.support.v4.view.GestureDetectorCompat(target._context, new SwipeGestureListenerClass(this, this.target));
        }
        if (type & definition.GestureTypes.pan) {
            this._panGestureDetector = new CustomPanGestureDetector(this, this.target);
        }
        if (type & definition.GestureTypes.rotation) {
            this._rotateGestureDetector = new CustomRotateGestureDetector(this, this.target);
        }
        if (type & definition.GestureTypes.touch) {
            this._notifyTouch = true;
        }
    };
    GesturesObserver.prototype.androidOnTouchEvent = function (motionEvent) {
        if (this._notifyTouch) {
            if (!this._eventData) {
                this._eventData = new TouchGestureEventData();
            }
            this._eventData.prepare(this.target, motionEvent);
            _executeCallback(this, this._eventData);
        }
        if (this._simpleGestureDetector) {
            this._simpleGestureDetector.onTouchEvent(motionEvent);
        }
        if (this._scaleGestureDetector) {
            this._scaleGestureDetector.onTouchEvent(motionEvent);
        }
        if (this._swipeGestureDetector) {
            this._swipeGestureDetector.onTouchEvent(motionEvent);
        }
        if (this._panGestureDetector) {
            this._panGestureDetector.onTouchEvent(motionEvent);
        }
        if (this._rotateGestureDetector) {
            this._rotateGestureDetector.onTouchEvent(motionEvent);
        }
    };
    return GesturesObserver;
}(common.GesturesObserver));
exports.GesturesObserver = GesturesObserver;
function _getArgs(type, view, e) {
    return {
        type: type,
        view: view,
        android: e,
        ios: undefined,
        object: view,
        eventName: definition.toString(type),
    };
}
function _getSwipeArgs(direction, view, initialEvent, currentEvent) {
    return {
        type: definition.GestureTypes.swipe,
        view: view,
        android: { initial: initialEvent, current: currentEvent },
        direction: direction,
        ios: undefined,
        object: view,
        eventName: definition.toString(definition.GestureTypes.swipe),
    };
}
function _getPanArgs(deltaX, deltaY, view, state, initialEvent, currentEvent) {
    return {
        type: definition.GestureTypes.pan,
        view: view,
        android: { initial: initialEvent, current: currentEvent },
        deltaX: deltaX,
        deltaY: deltaY,
        ios: undefined,
        object: view,
        eventName: definition.toString(definition.GestureTypes.pan),
        state: state
    };
}
function _executeCallback(observer, args) {
    if (observer && observer.callback) {
        observer.callback.call(observer._context, args);
    }
}
var TapAndDoubleTapGestureListenerClass;
function ensureTapAndDoubleTapGestureListenerClass() {
    if (TapAndDoubleTapGestureListenerClass) {
        return;
    }
    var TapAndDoubleTapGestureListener = (function (_super) {
        __extends(TapAndDoubleTapGestureListener, _super);
        function TapAndDoubleTapGestureListener(observer, target, type) {
            _super.call(this);
            this._observer = observer;
            this._target = target;
            this._type = type;
            return global.__native(this);
        }
        TapAndDoubleTapGestureListener.prototype.onSingleTapUp = function (motionEvent) {
            if (this._type & definition.GestureTypes.tap) {
                var args = _getArgs(definition.GestureTypes.tap, this._target, motionEvent);
                _executeCallback(this._observer, args);
            }
            return true;
        };
        TapAndDoubleTapGestureListener.prototype.onDoubleTap = function (motionEvent) {
            if (this._type & definition.GestureTypes.doubleTap) {
                var args = _getArgs(definition.GestureTypes.doubleTap, this._target, motionEvent);
                _executeCallback(this._observer, args);
            }
            return true;
        };
        TapAndDoubleTapGestureListener.prototype.onDown = function (motionEvent) {
            return true;
        };
        TapAndDoubleTapGestureListener.prototype.onLongPress = function (motionEvent) {
            if (this._type & definition.GestureTypes.longPress) {
                var args = _getArgs(definition.GestureTypes.longPress, this._target, motionEvent);
                _executeCallback(this._observer, args);
            }
        };
        return TapAndDoubleTapGestureListener;
    }(android.view.GestureDetector.SimpleOnGestureListener));
    TapAndDoubleTapGestureListenerClass = TapAndDoubleTapGestureListener;
}
var PinchGestureEventData = (function () {
    function PinchGestureEventData(view, android, scale, object, state) {
        this.view = view;
        this.android = android;
        this.scale = scale;
        this.object = object;
        this.state = state;
        this.type = definition.GestureTypes.pinch;
        this.eventName = definition.toString(definition.GestureTypes.pinch);
    }
    PinchGestureEventData.prototype.getFocusX = function () {
        return this.android.getFocusX() / utils.layout.getDisplayDensity();
    };
    PinchGestureEventData.prototype.getFocusY = function () {
        return this.android.getFocusY() / utils.layout.getDisplayDensity();
    };
    return PinchGestureEventData;
}());
var PinchGestureListenerClass;
function ensurePinchGestureListenerClass() {
    if (PinchGestureListenerClass) {
        return;
    }
    var PinchGestureListener = (function (_super) {
        __extends(PinchGestureListener, _super);
        function PinchGestureListener(observer, target) {
            _super.call(this);
            this._observer = observer;
            this._target = target;
            this._density = utils.layout.getDisplayDensity();
            return global.__native(this);
        }
        PinchGestureListener.prototype.onScaleBegin = function (detector) {
            this._scale = detector.getScaleFactor();
            var args = new PinchGestureEventData(this._target, detector, this._scale, this._target, common.GestureStateTypes.began);
            _executeCallback(this._observer, args);
            return true;
        };
        PinchGestureListener.prototype.onScale = function (detector) {
            this._scale *= detector.getScaleFactor();
            var args = new PinchGestureEventData(this._target, detector, this._scale, this._target, common.GestureStateTypes.changed);
            _executeCallback(this._observer, args);
            return true;
        };
        PinchGestureListener.prototype.onScaleEnd = function (detector) {
            this._scale *= detector.getScaleFactor();
            var args = new PinchGestureEventData(this._target, detector, this._scale, this._target, common.GestureStateTypes.ended);
            _executeCallback(this._observer, args);
        };
        return PinchGestureListener;
    }(android.view.ScaleGestureDetector.SimpleOnScaleGestureListener));
    PinchGestureListenerClass = PinchGestureListener;
}
var SwipeGestureListenerClass;
function ensureSwipeGestureListenerClass() {
    if (SwipeGestureListenerClass) {
        return;
    }
    var SwipeGestureListener = (function (_super) {
        __extends(SwipeGestureListener, _super);
        function SwipeGestureListener(observer, target) {
            _super.call(this);
            this._observer = observer;
            this._target = target;
            return global.__native(this);
        }
        SwipeGestureListener.prototype.onDown = function (motionEvent) {
            return true;
        };
        SwipeGestureListener.prototype.onFling = function (initialEvent, currentEvent, velocityX, velocityY) {
            var result = false;
            var args;
            try {
                var deltaY = currentEvent.getY() - initialEvent.getY();
                var deltaX = currentEvent.getX() - initialEvent.getX();
                if (Math.abs(deltaX) > Math.abs(deltaY)) {
                    if (Math.abs(deltaX) > SWIPE_THRESHOLD
                        && Math.abs(velocityX) > SWIPE_VELOCITY_THRESHOLD) {
                        if (deltaX > 0) {
                            args = _getSwipeArgs(definition.SwipeDirection.right, this._target, initialEvent, currentEvent);
                            _executeCallback(this._observer, args);
                            result = true;
                        }
                        else {
                            args = _getSwipeArgs(definition.SwipeDirection.left, this._target, initialEvent, currentEvent);
                            _executeCallback(this._observer, args);
                            result = true;
                        }
                    }
                }
                else {
                    if (Math.abs(deltaY) > SWIPE_THRESHOLD
                        && Math.abs(velocityY) > SWIPE_VELOCITY_THRESHOLD) {
                        if (deltaY > 0) {
                            args = _getSwipeArgs(definition.SwipeDirection.down, this._target, initialEvent, currentEvent);
                            _executeCallback(this._observer, args);
                            result = true;
                        }
                        else {
                            args = _getSwipeArgs(definition.SwipeDirection.up, this._target, initialEvent, currentEvent);
                            _executeCallback(this._observer, args);
                            result = true;
                        }
                    }
                }
            }
            catch (ex) {
            }
            return result;
        };
        return SwipeGestureListener;
    }(android.view.GestureDetector.SimpleOnGestureListener));
    SwipeGestureListenerClass = SwipeGestureListener;
}
var CustomPanGestureDetector = (function () {
    function CustomPanGestureDetector(observer, target) {
        this.observer = observer;
        this.target = target;
        this.isTracking = false;
        this.density = utils.layout.getDisplayDensity();
    }
    CustomPanGestureDetector.prototype.onTouchEvent = function (event) {
        switch (event.getActionMasked()) {
            case android.view.MotionEvent.ACTION_UP:
            case android.view.MotionEvent.ACTION_CANCEL:
                this.trackStop(event, false);
                break;
            case android.view.MotionEvent.ACTION_DOWN:
            case android.view.MotionEvent.ACTION_POINTER_DOWN:
            case android.view.MotionEvent.ACTION_POINTER_UP:
                this.trackStop(event, true);
                break;
            case android.view.MotionEvent.ACTION_MOVE:
                if (!this.isTracking) {
                    this.trackStart(event);
                }
                this.trackChange(event);
                break;
        }
        return true;
    };
    CustomPanGestureDetector.prototype.trackStop = function (currentEvent, cahceEvent) {
        if (this.isTracking) {
            var args = _getPanArgs(this.deltaX, this.deltaY, this.target, common.GestureStateTypes.ended, null, currentEvent);
            _executeCallback(this.observer, args);
            this.deltaX = undefined;
            this.deltaY = undefined;
            this.isTracking = false;
        }
        if (cahceEvent) {
            this.lastEventCache = currentEvent;
        }
        else {
            this.lastEventCache = undefined;
        }
    };
    CustomPanGestureDetector.prototype.trackStart = function (currentEvent) {
        var inital = this.getEventCoordinates(this.lastEventCache ? this.lastEventCache : currentEvent);
        this.initialX = inital.x;
        this.initialY = inital.y;
        this.isTracking = true;
        var args = _getPanArgs(0, 0, this.target, common.GestureStateTypes.began, null, currentEvent);
        _executeCallback(this.observer, args);
    };
    CustomPanGestureDetector.prototype.trackChange = function (currentEvent) {
        var current = this.getEventCoordinates(currentEvent);
        this.deltaX = current.x - this.initialX;
        this.deltaY = current.y - this.initialY;
        var args = _getPanArgs(this.deltaX, this.deltaY, this.target, common.GestureStateTypes.changed, null, currentEvent);
        _executeCallback(this.observer, args);
    };
    CustomPanGestureDetector.prototype.getEventCoordinates = function (event) {
        var count = event.getPointerCount();
        if (count === 1) {
            return {
                x: event.getRawX() / this.density,
                y: event.getRawY() / this.density
            };
        }
        else {
            var offX = event.getRawX() - event.getX();
            var offY = event.getRawY() - event.getY();
            var res = { x: 0, y: 0 };
            for (var i = 0; i < count; i++) {
                res.x += event.getX(i) + offX;
                res.y += event.getY(i) + offY;
            }
            res.x /= (count * this.density);
            res.y /= (count * this.density);
            return res;
        }
    };
    return CustomPanGestureDetector;
}());
var CustomRotateGestureDetector = (function () {
    function CustomRotateGestureDetector(observer, target) {
        this.observer = observer;
        this.target = target;
        this.trackedPtrId1 = INVALID_POINTER_ID;
        this.trackedPtrId2 = INVALID_POINTER_ID;
    }
    Object.defineProperty(CustomRotateGestureDetector.prototype, "isTracking", {
        get: function () {
            return this.trackedPtrId1 !== INVALID_POINTER_ID && this.trackedPtrId2 !== INVALID_POINTER_ID;
        },
        enumerable: true,
        configurable: true
    });
    CustomRotateGestureDetector.prototype.onTouchEvent = function (event) {
        var pointerID = event.getPointerId(event.getActionIndex());
        var wasTracking = this.isTracking;
        switch (event.getActionMasked()) {
            case android.view.MotionEvent.ACTION_DOWN:
            case android.view.MotionEvent.ACTION_POINTER_DOWN:
                var assigned = false;
                if (this.trackedPtrId1 === INVALID_POINTER_ID && pointerID !== this.trackedPtrId2) {
                    this.trackedPtrId1 = pointerID;
                    assigned = true;
                }
                else if (this.trackedPtrId2 === INVALID_POINTER_ID && pointerID !== this.trackedPtrId1) {
                    this.trackedPtrId2 = pointerID;
                    assigned = true;
                }
                if (assigned && this.isTracking) {
                    this.angle = 0;
                    this.initalPointersAngle = this.getPointersAngle(event);
                    this.executeCallback(event, common.GestureStateTypes.began);
                }
                break;
            case android.view.MotionEvent.ACTION_MOVE:
                if (this.isTracking) {
                    this.updateAngle(event);
                    this.executeCallback(event, common.GestureStateTypes.changed);
                }
                break;
            case android.view.MotionEvent.ACTION_UP:
            case android.view.MotionEvent.ACTION_POINTER_UP:
                if (pointerID === this.trackedPtrId1) {
                    this.trackedPtrId1 = INVALID_POINTER_ID;
                }
                else if (pointerID === this.trackedPtrId2) {
                    this.trackedPtrId2 = INVALID_POINTER_ID;
                }
                if (wasTracking && !this.isTracking) {
                    this.executeCallback(event, common.GestureStateTypes.ended);
                }
                break;
            case android.view.MotionEvent.ACTION_CANCEL:
                this.trackedPtrId1 = INVALID_POINTER_ID;
                this.trackedPtrId2 = INVALID_POINTER_ID;
                if (wasTracking) {
                    this.executeCallback(event, common.GestureStateTypes.cancelled);
                }
                break;
        }
        return true;
    };
    CustomRotateGestureDetector.prototype.executeCallback = function (event, state) {
        var args = {
            type: definition.GestureTypes.rotation,
            view: this.target,
            android: event,
            rotation: this.angle,
            ios: undefined,
            object: this.target,
            eventName: definition.toString(definition.GestureTypes.rotation),
            state: state
        };
        _executeCallback(this.observer, args);
    };
    CustomRotateGestureDetector.prototype.updateAngle = function (event) {
        var newPointersAngle = this.getPointersAngle(event);
        var result = ((newPointersAngle - this.initalPointersAngle) * TO_DEGREES) % 360;
        if (result < -180) {
            result += 360;
        }
        if (result > 180) {
            result -= 360;
        }
        this.angle = result;
    };
    CustomRotateGestureDetector.prototype.getPointersAngle = function (event) {
        var firstX = event.getX(event.findPointerIndex(this.trackedPtrId1));
        var firstY = event.getY(event.findPointerIndex(this.trackedPtrId1));
        var secondX = event.getX(event.findPointerIndex(this.trackedPtrId2));
        var secondY = event.getY(event.findPointerIndex(this.trackedPtrId2));
        return Math.atan2((secondY - firstY), (secondX - firstX));
    };
    return CustomRotateGestureDetector;
}());
var Pointer = (function () {
    function Pointer(id, event) {
        this.event = event;
        this.ios = undefined;
        this.android = id;
    }
    Pointer.prototype.getX = function () {
        return this.event.getX(this.android) / utils.layout.getDisplayDensity();
    };
    Pointer.prototype.getY = function () {
        return this.event.getY(this.android) / utils.layout.getDisplayDensity();
    };
    return Pointer;
}());
var TouchGestureEventData = (function () {
    function TouchGestureEventData() {
        this.eventName = definition.toString(definition.GestureTypes.touch);
        this.type = definition.GestureTypes.touch;
        this.ios = undefined;
    }
    TouchGestureEventData.prototype.prepare = function (view, e) {
        this.view = view;
        this.object = view;
        this.android = e;
        this.action = this.getActionType(e);
        this._activePointers = undefined;
        this._allPointers = undefined;
    };
    TouchGestureEventData.prototype.getPointerCount = function () {
        return this.android.getPointerCount();
    };
    TouchGestureEventData.prototype.getActivePointers = function () {
        if (!this._activePointers) {
            this._activePointers = [new Pointer(this.android.getActionIndex(), this.android)];
        }
        return this._activePointers;
    };
    TouchGestureEventData.prototype.getAllPointers = function () {
        if (!this._allPointers) {
            this._allPointers = [];
            for (var i = 0; i < this.getPointerCount(); i++) {
                this._allPointers.push(new Pointer(i, this.android));
            }
        }
        return this._allPointers;
    };
    TouchGestureEventData.prototype.getX = function () {
        return this.getActivePointers()[0].getX();
    };
    TouchGestureEventData.prototype.getY = function () {
        return this.getActivePointers()[0].getY();
    };
    TouchGestureEventData.prototype.getActionType = function (e) {
        switch (e.getActionMasked()) {
            case android.view.MotionEvent.ACTION_DOWN:
            case android.view.MotionEvent.ACTION_POINTER_DOWN:
                return common.TouchAction.down;
            case android.view.MotionEvent.ACTION_MOVE:
                return common.TouchAction.move;
            case android.view.MotionEvent.ACTION_UP:
            case android.view.MotionEvent.ACTION_POINTER_UP:
                return common.TouchAction.up;
            case android.view.MotionEvent.ACTION_CANCEL:
                return common.TouchAction.cancel;
        }
        return "";
    };
    return TouchGestureEventData;
}());

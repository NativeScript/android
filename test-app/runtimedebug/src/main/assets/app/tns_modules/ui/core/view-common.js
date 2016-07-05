var types = require("utils/types");
var style = require("../styling/style");
var styling = require("ui/styling");
var trace = require("trace");
var gestures = require("ui/gestures");
var styleScope = require("../styling/style-scope");
var enums = require("ui/enums");
var utils = require("utils/utils");
var proxy_1 = require("ui/core/proxy");
var dependency_observable_1 = require("ui/core/dependency-observable");
var special_properties_1 = require("ui/builder/special-properties");
var style_1 = require("ui/styling/style");
var visualStateConstants = require("ui/styling/visual-state-constants");
var debug_1 = require("utils/debug");
var visualState;
function ensureVisualState() {
    if (!visualState) {
        visualState = require("../styling/visual-state");
    }
}
special_properties_1.registerSpecialProperty("class", function (instance, propertyValue) {
    instance.className = propertyValue;
});
function getEventOrGestureName(name) {
    return name.indexOf("on") === 0 ? name.substr(2, name.length - 2) : name;
}
function isEventOrGesture(name, view) {
    if (types.isString(name)) {
        var eventOrGestureName = getEventOrGestureName(name);
        var evt = eventOrGestureName + "Event";
        return view.constructor && evt in view.constructor ||
            gestures.fromString(eventOrGestureName.toLowerCase()) !== undefined;
    }
    return false;
}
exports.isEventOrGesture = isEventOrGesture;
function getViewById(view, id) {
    if (!view) {
        return undefined;
    }
    if (view.id === id) {
        return view;
    }
    var retVal;
    var descendantsCallback = function (child) {
        if (child.id === id) {
            retVal = child;
            return false;
        }
        return true;
    };
    eachDescendant(view, descendantsCallback);
    return retVal;
}
exports.getViewById = getViewById;
function eachDescendant(view, callback) {
    if (!callback || !view) {
        return;
    }
    var continueIteration;
    var localCallback = function (child) {
        continueIteration = callback(child);
        if (continueIteration) {
            child._eachChildView(localCallback);
        }
        return continueIteration;
    };
    view._eachChildView(localCallback);
}
exports.eachDescendant = eachDescendant;
function getAncestor(view, criterion) {
    var matcher = null;
    if (typeof criterion === "string") {
        matcher = function (view) { return view.typeName === criterion; };
    }
    else {
        matcher = function (view) { return view instanceof criterion; };
    }
    for (var parent_1 = view.parent; parent_1 != null; parent_1 = parent_1.parent) {
        if (matcher(parent_1)) {
            return parent_1;
        }
    }
    return null;
}
exports.getAncestor = getAncestor;
var viewIdCounter = 0;
function onCssClassPropertyChanged(data) {
    var view = data.object;
    if (types.isString(data.newValue)) {
        view._cssClasses = data.newValue.split(" ");
    }
    else {
        view._cssClasses.length = 0;
    }
}
var cssClassProperty = new dependency_observable_1.Property("cssClass", "View", new proxy_1.PropertyMetadata(undefined, dependency_observable_1.PropertyMetadataSettings.AffectsStyle, onCssClassPropertyChanged));
var classNameProperty = new dependency_observable_1.Property("className", "View", new proxy_1.PropertyMetadata(undefined, dependency_observable_1.PropertyMetadataSettings.AffectsStyle, onCssClassPropertyChanged));
var idProperty = new dependency_observable_1.Property("id", "View", new proxy_1.PropertyMetadata(undefined, dependency_observable_1.PropertyMetadataSettings.AffectsStyle));
var automationTextProperty = new dependency_observable_1.Property("automationText", "View", new proxy_1.PropertyMetadata(undefined));
var originXProperty = new dependency_observable_1.Property("originX", "View", new proxy_1.PropertyMetadata(0.5));
var originYProperty = new dependency_observable_1.Property("originY", "View", new proxy_1.PropertyMetadata(0.5));
var isEnabledProperty = new dependency_observable_1.Property("isEnabled", "View", new proxy_1.PropertyMetadata(true));
var isUserInteractionEnabledProperty = new dependency_observable_1.Property("isUserInteractionEnabled", "View", new proxy_1.PropertyMetadata(true));
var View = (function (_super) {
    __extends(View, _super);
    function View() {
        _super.call(this, {});
        this._isVisibleCache = true;
        this._measuredWidth = Number.NaN;
        this._measuredHeight = Number.NaN;
        this._oldWidthMeasureSpec = Number.NaN;
        this._oldHeightMeasureSpec = Number.NaN;
        this._oldLeft = 0;
        this._oldTop = 0;
        this._oldRight = 0;
        this._oldBottom = 0;
        this._isLayoutValid = false;
        this._isAddedToNativeVisualTree = false;
        this._cssClasses = [];
        this._gestureObservers = {};
        this._style = new style.Style(this);
        this._domId = viewIdCounter++;
        this._visualState = visualStateConstants.Normal;
    }
    View.prototype.getGestureObservers = function (type) {
        return this._gestureObservers[type];
    };
    View.prototype.observe = function (type, callback, thisArg) {
        if (!this._gestureObservers[type]) {
            this._gestureObservers[type] = [];
        }
        this._gestureObservers[type].push(gestures.observe(this, type, callback, thisArg));
    };
    View.prototype.addEventListener = function (arg, callback, thisArg) {
        if (types.isString(arg)) {
            arg = getEventOrGestureName(arg);
            var gesture = gestures.fromString(arg);
            if (gesture && !this._isEvent(arg)) {
                this.observe(gesture, callback, thisArg);
            }
            else {
                var events = arg.split(",");
                if (events.length > 0) {
                    for (var i = 0; i < events.length; i++) {
                        var evt = events[i].trim();
                        var gst = gestures.fromString(evt);
                        if (gst && !this._isEvent(arg)) {
                            this.observe(gst, callback, thisArg);
                        }
                        else {
                            _super.prototype.addEventListener.call(this, evt, callback, thisArg);
                        }
                    }
                }
                else {
                    _super.prototype.addEventListener.call(this, arg, callback, thisArg);
                }
            }
        }
        else if (types.isNumber(arg)) {
            this.observe(arg, callback, thisArg);
        }
    };
    View.prototype.removeEventListener = function (arg, callback, thisArg) {
        if (types.isString(arg)) {
            var gesture = gestures.fromString(arg);
            if (gesture && !this._isEvent(arg)) {
                this._disconnectGestureObservers(gesture);
            }
            else {
                var events = arg.split(",");
                if (events.length > 0) {
                    for (var i = 0; i < events.length; i++) {
                        var evt = events[i].trim();
                        var gst = gestures.fromString(evt);
                        if (gst && !this._isEvent(arg)) {
                            this._disconnectGestureObservers(gst);
                        }
                        else {
                            _super.prototype.removeEventListener.call(this, evt, callback, thisArg);
                        }
                    }
                }
                else {
                    _super.prototype.removeEventListener.call(this, arg, callback, thisArg);
                }
            }
        }
        else if (types.isNumber(arg)) {
            this._disconnectGestureObservers(arg);
        }
    };
    View.prototype._isEvent = function (name) {
        return this.constructor && name + "Event" in this.constructor;
    };
    View.prototype._disconnectGestureObservers = function (type) {
        var observers = this.getGestureObservers(type);
        if (observers) {
            for (var i = 0; i < observers.length; i++) {
                observers[i].disconnect();
            }
        }
    };
    View.prototype.getViewById = function (id) {
        return getViewById(this, id);
    };
    Object.defineProperty(View.prototype, "automationText", {
        get: function () {
            return this._getValue(View.automationTextProperty);
        },
        set: function (value) {
            this._setValue(View.automationTextProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "borderRadius", {
        get: function () {
            return this.style.borderRadius;
        },
        set: function (value) {
            this.style.borderRadius = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "borderWidth", {
        get: function () {
            return this.style.borderWidth;
        },
        set: function (value) {
            this.style.borderWidth = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "borderColor", {
        get: function () {
            return this.style.borderColor;
        },
        set: function (value) {
            this.style.borderColor = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "color", {
        get: function () {
            return this.style.color;
        },
        set: function (value) {
            this.style.color = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "backgroundColor", {
        get: function () {
            return this.style.backgroundColor;
        },
        set: function (value) {
            this.style.backgroundColor = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "backgroundImage", {
        get: function () {
            return this.style.backgroundImage;
        },
        set: function (value) {
            this.style.backgroundImage = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "minWidth", {
        get: function () {
            return this.style.minWidth;
        },
        set: function (value) {
            this.style.minWidth = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "minHeight", {
        get: function () {
            return this.style.minHeight;
        },
        set: function (value) {
            this.style.minHeight = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "width", {
        get: function () {
            return this.style.width;
        },
        set: function (value) {
            this.style.width = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "height", {
        get: function () {
            return this.style.height;
        },
        set: function (value) {
            this.style.height = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "margin", {
        get: function () {
            return this.style.margin;
        },
        set: function (value) {
            this.style.margin = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "marginLeft", {
        get: function () {
            return this.style.marginLeft;
        },
        set: function (value) {
            this.style.marginLeft = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "marginTop", {
        get: function () {
            return this.style.marginTop;
        },
        set: function (value) {
            this.style.marginTop = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "marginRight", {
        get: function () {
            return this.style.marginRight;
        },
        set: function (value) {
            this.style.marginRight = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "marginBottom", {
        get: function () {
            return this.style.marginBottom;
        },
        set: function (value) {
            this.style.marginBottom = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "horizontalAlignment", {
        get: function () {
            return this.style.horizontalAlignment;
        },
        set: function (value) {
            this.style.horizontalAlignment = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "verticalAlignment", {
        get: function () {
            return this.style.verticalAlignment;
        },
        set: function (value) {
            this.style.verticalAlignment = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "visibility", {
        get: function () {
            return this.style.visibility;
        },
        set: function (value) {
            this.style.visibility = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "opacity", {
        get: function () {
            return this.style.opacity;
        },
        set: function (value) {
            this.style.opacity = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "translateX", {
        get: function () {
            return this.style.translateX;
        },
        set: function (value) {
            this.style.translateX = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "translateY", {
        get: function () {
            return this.style.translateY;
        },
        set: function (value) {
            this.style.translateY = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "scaleX", {
        get: function () {
            return this.style.scaleX;
        },
        set: function (value) {
            this.style.scaleX = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "scaleY", {
        get: function () {
            return this.style.scaleY;
        },
        set: function (value) {
            this.style.scaleY = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "originX", {
        get: function () {
            return this._getValue(View.originXProperty);
        },
        set: function (value) {
            this._setValue(View.originXProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "originY", {
        get: function () {
            return this._getValue(View.originYProperty);
        },
        set: function (value) {
            this._setValue(View.originYProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "rotate", {
        get: function () {
            return this.style.rotate;
        },
        set: function (value) {
            this.style.rotate = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "isEnabled", {
        get: function () {
            return this._getValue(View.isEnabledProperty);
        },
        set: function (value) {
            this._setValue(View.isEnabledProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "page", {
        get: function () {
            if (this.parent) {
                return this.parent.page;
            }
            return null;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "isUserInteractionEnabled", {
        get: function () {
            return this._getValue(View.isUserInteractionEnabledProperty);
        },
        set: function (value) {
            this._setValue(View.isUserInteractionEnabledProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "id", {
        get: function () {
            return this._getValue(View.idProperty);
        },
        set: function (value) {
            this._setValue(View.idProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "cssClass", {
        get: function () {
            return this._getValue(View.cssClassProperty);
        },
        set: function (value) {
            this._setValue(View.cssClassProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "className", {
        get: function () {
            return this._getValue(View.cssClassProperty);
        },
        set: function (value) {
            this._setValue(View.cssClassProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "style", {
        get: function () {
            return this._style;
        },
        set: function (value) {
            throw new Error("View.style property is read-only.");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "isLayoutRequired", {
        get: function () {
            return true;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "isLayoutValid", {
        get: function () {
            return this._isLayoutValid;
        },
        set: function (value) {
            throw new Error("isLayoutValid is read-only property.");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "visualState", {
        get: function () {
            return this._visualState;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "cssType", {
        get: function () {
            return this.typeName.toLowerCase();
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "parent", {
        get: function () {
            return this._parent;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "isLoaded", {
        get: function () {
            return this._isLoaded;
        },
        enumerable: true,
        configurable: true
    });
    View.prototype.onLoaded = function () {
        this._isLoaded = true;
        this._loadEachChildView();
        this._applyStyleFromScope();
        this._emit("loaded");
    };
    View.prototype._loadEachChildView = function () {
        if (this._childrenCount > 0) {
            var eachChild = function (child) {
                child.onLoaded();
                return true;
            };
            this._eachChildView(eachChild);
        }
    };
    View.prototype.onUnloaded = function () {
        this._unloadEachChildView();
        this._isLoaded = false;
        this._emit("unloaded");
    };
    View.prototype._unloadEachChildView = function () {
        if (this._childrenCount > 0) {
            this._eachChildView(function (child) {
                if (child.isLoaded) {
                    child.onUnloaded();
                }
                return true;
            });
        }
    };
    View.prototype._onPropertyChanged = function (property, oldValue, newValue) {
        var _this = this;
        _super.prototype._onPropertyChanged.call(this, property, oldValue, newValue);
        if (this._childrenCount > 0) {
            var shouldUpdateInheritableProps = (property.inheritable && !(property instanceof styling.Property));
            if (shouldUpdateInheritableProps) {
                this._updatingInheritedProperties = true;
                this._eachChildView(function (child) {
                    child._setValue(property, _this._getValue(property), dependency_observable_1.ValueSource.Inherited);
                    return true;
                });
                this._updatingInheritedProperties = false;
            }
        }
        this._checkMetadataOnPropertyChanged(property.metadata);
    };
    View.prototype._isInheritedChange = function () {
        if (this._updatingInheritedProperties) {
            return true;
        }
        var parentView;
        parentView = (this.parent);
        while (parentView) {
            if (parentView._updatingInheritedProperties) {
                return true;
            }
            parentView = (parentView.parent);
        }
        return false;
    };
    View.prototype._checkMetadataOnPropertyChanged = function (metadata) {
        if (metadata.affectsLayout) {
            this.requestLayout();
        }
        if (metadata.affectsStyle) {
            this.style._resetCssValues();
            this._applyStyleFromScope();
            this._eachChildView(function (v) {
                v._checkMetadataOnPropertyChanged(metadata);
                return true;
            });
        }
    };
    View.prototype.measure = function (widthMeasureSpec, heightMeasureSpec) {
        this._setCurrentMeasureSpecs(widthMeasureSpec, heightMeasureSpec);
    };
    View.prototype.layout = function (left, top, right, bottom) {
        this._setCurrentLayoutBounds(left, top, right, bottom);
    };
    View.prototype.getMeasuredWidth = function () {
        return this._measuredWidth & utils.layout.MEASURED_SIZE_MASK;
    };
    View.prototype.getMeasuredHeight = function () {
        return this._measuredHeight & utils.layout.MEASURED_SIZE_MASK;
    };
    View.prototype.setMeasuredDimension = function (measuredWidth, measuredHeight) {
        this._measuredWidth = measuredWidth;
        this._measuredHeight = measuredHeight;
        if (trace.enabled) {
            trace.write(this + " :setMeasuredDimension: " + measuredWidth + ", " + measuredHeight, trace.categories.Layout);
        }
    };
    View.prototype.requestLayout = function () {
        this._isLayoutValid = false;
    };
    View.prototype.onMeasure = function (widthMeasureSpec, heightMeasureSpec) {
    };
    View.prototype.onLayout = function (left, top, right, bottom) {
    };
    View.prototype.layoutNativeView = function (left, top, right, bottom) {
    };
    View.resolveSizeAndState = function (size, specSize, specMode, childMeasuredState) {
        var result = size;
        switch (specMode) {
            case utils.layout.UNSPECIFIED:
                result = size;
                break;
            case utils.layout.AT_MOST:
                if (specSize < size) {
                    result = Math.round(specSize + 0.499) | utils.layout.MEASURED_STATE_TOO_SMALL;
                }
                break;
            case utils.layout.EXACTLY:
                result = specSize;
                break;
        }
        return Math.round(result + 0.499) | (childMeasuredState & utils.layout.MEASURED_STATE_MASK);
    };
    View.layoutChild = function (parent, child, left, top, right, bottom) {
        if (!child || !child._isVisible) {
            return;
        }
        var density = utils.layout.getDisplayDensity();
        var lp = child.style._getValue(style_1.nativeLayoutParamsProperty);
        var childTop;
        var childLeft;
        var childWidth = child.getMeasuredWidth();
        var childHeight = child.getMeasuredHeight();
        var vAlignment;
        if (lp.height >= 0 && lp.verticalAlignment === enums.VerticalAlignment.stretch) {
            vAlignment = enums.VerticalAlignment.center;
        }
        else {
            vAlignment = lp.verticalAlignment;
        }
        var marginTop = lp.topMargin;
        var marginBottom = lp.bottomMargin;
        var marginLeft = lp.leftMargin;
        var marginRight = lp.rightMargin;
        switch (vAlignment) {
            case enums.VerticalAlignment.top:
                childTop = top + marginTop * density;
                break;
            case enums.VerticalAlignment.center:
            case enums.VerticalAlignment.middle:
                childTop = top + (bottom - top - childHeight + (marginTop - marginBottom) * density) / 2;
                break;
            case enums.VerticalAlignment.bottom:
                childTop = bottom - childHeight - (marginBottom * density);
                break;
            case enums.VerticalAlignment.stretch:
            default:
                childTop = top + marginTop * density;
                childHeight = bottom - top - (marginTop + marginBottom) * density;
                break;
        }
        var hAlignment;
        if (lp.width >= 0 && lp.horizontalAlignment === enums.HorizontalAlignment.stretch) {
            hAlignment = enums.HorizontalAlignment.center;
        }
        else {
            hAlignment = lp.horizontalAlignment;
        }
        switch (hAlignment) {
            case enums.HorizontalAlignment.left:
                childLeft = left + marginLeft * density;
                break;
            case enums.HorizontalAlignment.center:
                childLeft = left + (right - left - childWidth + (marginLeft - marginRight) * density) / 2;
                break;
            case enums.HorizontalAlignment.right:
                childLeft = right - childWidth - (marginRight * density);
                break;
            case enums.HorizontalAlignment.stretch:
            default:
                childLeft = left + marginLeft * density;
                childWidth = right - left - (marginLeft + marginRight) * density;
                break;
        }
        var childRight = Math.round(childLeft + childWidth);
        var childBottom = Math.round(childTop + childHeight);
        childLeft = Math.round(childLeft);
        childTop = Math.round(childTop);
        if (trace.enabled) {
            trace.write(child.parent + " :layoutChild: " + child + " " + childLeft + ", " + childTop + ", " + childRight + ", " + childBottom, trace.categories.Layout);
        }
        child.layout(childLeft, childTop, childRight, childBottom);
    };
    View.measureChild = function (parent, child, widthMeasureSpec, heightMeasureSpec) {
        var measureWidth = 0;
        var measureHeight = 0;
        if (child && child._isVisible) {
            var width = utils.layout.getMeasureSpecSize(widthMeasureSpec);
            var widthMode = utils.layout.getMeasureSpecMode(widthMeasureSpec);
            var height = utils.layout.getMeasureSpecSize(heightMeasureSpec);
            var heightMode = utils.layout.getMeasureSpecMode(heightMeasureSpec);
            var childWidthMeasureSpec = View.getMeasureSpec(child, width, widthMode, true);
            var childHeightMeasureSpec = View.getMeasureSpec(child, height, heightMode, false);
            if (trace.enabled) {
                trace.write(child.parent + " :measureChild: " + child + " " + utils.layout.measureSpecToString(childWidthMeasureSpec) + ", " + utils.layout.measureSpecToString(childHeightMeasureSpec), trace.categories.Layout);
            }
            child.measure(childWidthMeasureSpec, childHeightMeasureSpec);
            measureWidth = child.getMeasuredWidth();
            measureHeight = child.getMeasuredHeight();
            var density = utils.layout.getDisplayDensity();
            var lp = child.style._getValue(style.nativeLayoutParamsProperty);
            measureWidth = Math.round(measureWidth + (lp.leftMargin + lp.rightMargin) * density);
            measureHeight = Math.round(measureHeight + (lp.topMargin + lp.bottomMargin) * density);
        }
        return { measuredWidth: measureWidth, measuredHeight: measureHeight };
    };
    View.getMeasureSpec = function (view, parentLength, parentSpecMode, horizontal) {
        var lp = view.style._getValue(style.nativeLayoutParamsProperty);
        var density = utils.layout.getDisplayDensity();
        var margins = horizontal ? lp.leftMargin + lp.rightMargin : lp.topMargin + lp.bottomMargin;
        margins = Math.round(margins * density);
        var resultSize = 0;
        var resultMode = 0;
        var measureLength = Math.max(0, parentLength - margins);
        var childLength = Math.round((horizontal ? lp.width : lp.height) * density);
        if (childLength >= 0) {
            if (parentSpecMode !== utils.layout.UNSPECIFIED) {
                resultSize = Math.min(parentLength, childLength);
            }
            else {
                resultSize = childLength;
            }
            resultMode = utils.layout.EXACTLY;
        }
        else {
            switch (parentSpecMode) {
                case utils.layout.EXACTLY:
                    resultSize = measureLength;
                    var stretched = horizontal ? lp.horizontalAlignment === enums.HorizontalAlignment.stretch : lp.verticalAlignment === enums.VerticalAlignment.stretch;
                    resultMode = stretched ? utils.layout.EXACTLY : utils.layout.AT_MOST;
                    break;
                case utils.layout.AT_MOST:
                    resultSize = measureLength;
                    resultMode = utils.layout.AT_MOST;
                    break;
                case utils.layout.UNSPECIFIED:
                    resultSize = 0;
                    resultMode = utils.layout.UNSPECIFIED;
                    break;
            }
        }
        return utils.layout.makeMeasureSpec(resultSize, resultMode);
    };
    View.prototype._setCurrentMeasureSpecs = function (widthMeasureSpec, heightMeasureSpec) {
        var changed = this._oldWidthMeasureSpec !== widthMeasureSpec || this._oldHeightMeasureSpec !== heightMeasureSpec;
        this._oldWidthMeasureSpec = widthMeasureSpec;
        this._oldHeightMeasureSpec = heightMeasureSpec;
        return changed;
    };
    View.prototype._getCurrentLayoutBounds = function () {
        return { left: this._oldLeft, top: this._oldTop, right: this._oldRight, bottom: this._oldBottom };
    };
    View.prototype._setCurrentLayoutBounds = function (left, top, right, bottom) {
        this._isLayoutValid = true;
        var boundsChanged = this._oldLeft !== left || this._oldTop !== top || this._oldRight !== right || this._oldBottom !== bottom;
        var sizeChanged = (this._oldRight - this._oldLeft !== right - left) || (this._oldBottom - this._oldTop !== bottom - top);
        this._oldLeft = left;
        this._oldTop = top;
        this._oldRight = right;
        this._oldBottom = bottom;
        return { boundsChanged: boundsChanged, sizeChanged: sizeChanged };
    };
    View.prototype._applyStyleFromScope = function () {
        var rootPage = this.page;
        if (!rootPage || !rootPage.isLoaded) {
            return;
        }
        var scope = rootPage._getStyleScope();
        scope.applySelectors(this);
    };
    View.prototype._applyInlineStyle = function (inlineStyle) {
        if (types.isString(inlineStyle)) {
            try {
                this.style._beginUpdate();
                styleScope.applyInlineSyle(this, inlineStyle);
            }
            finally {
                this.style._endUpdate();
            }
        }
    };
    View.prototype._onAttached = function (context) {
    };
    View.prototype._onDetached = function (force) {
    };
    View.prototype._createUI = function () {
    };
    View.prototype._onContextChanged = function () {
    };
    Object.defineProperty(View.prototype, "_childrenCount", {
        get: function () {
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    View.prototype._eachChildView = function (callback) {
    };
    View.prototype._childIndexToNativeChildIndex = function (index) {
        return index;
    };
    View.prototype._getNativeViewsCount = function () {
        return this._isAddedToNativeVisualTree ? 1 : 0;
    };
    View.prototype._eachLayoutView = function (callback) {
        return callback(this);
    };
    View.prototype._addToSuperview = function (superview, index) {
        return false;
    };
    View.prototype._removeFromSuperview = function () {
    };
    View.prototype._addView = function (view, atIndex) {
        if (trace.enabled) {
            trace.write(this + "._addView(" + view + ", " + atIndex + ")", trace.categories.ViewHierarchy);
        }
        if (!view) {
            throw new Error("Expecting a valid View instance.");
        }
        if (view._parent) {
            throw new Error("View already has a parent. View: " + view + " Parent: " + view._parent);
        }
        view._parent = this;
        this._addViewCore(view, atIndex);
        view._parentChanged(null);
    };
    View.prototype._addViewCore = function (view, atIndex) {
        this._propagateInheritableProperties(view);
        view.style._inheritStyleProperties();
        if (!view._isAddedToNativeVisualTree) {
            var nativeIndex = this._childIndexToNativeChildIndex(atIndex);
            view._isAddedToNativeVisualTree = this._addViewToNativeVisualTree(view, nativeIndex);
        }
        if (this._isLoaded) {
            view.onLoaded();
        }
    };
    View.prototype._propagateInheritableProperties = function (view) {
        view._inheritProperties(this);
    };
    View.prototype._inheritProperties = function (parentView) {
        var _this = this;
        parentView._eachSetProperty(function (property) {
            if (!(property instanceof styling.Property) && property.inheritable) {
                var baseValue = parentView._getValue(property);
                _this._setValue(property, baseValue, dependency_observable_1.ValueSource.Inherited);
            }
            return true;
        });
    };
    View.prototype._removeView = function (view) {
        if (trace.enabled) {
            trace.write(this + "._removeView(" + view + ")", trace.categories.ViewHierarchy);
        }
        if (view._parent !== this) {
            throw new Error("View not added to this instance. View: " + view + " CurrentParent: " + view._parent + " ExpectedParent: " + this);
        }
        this._removeViewCore(view);
        view._parent = undefined;
        view._parentChanged(this);
    };
    View.prototype._removeViewCore = function (view) {
        this._removeViewFromNativeVisualTree(view);
        if (view.isLoaded) {
            view.onUnloaded();
        }
        view._setValue(proxy_1.ProxyObject.bindingContextProperty, undefined, dependency_observable_1.ValueSource.Inherited);
        view._eachSetProperty(function (property) {
            if (!(property instanceof styling.Property) && property.inheritable) {
                view._resetValue(property, dependency_observable_1.ValueSource.Inherited);
            }
            return true;
        });
    };
    View.prototype._parentChanged = function (oldParent) {
    };
    View.prototype._addViewToNativeVisualTree = function (view, atIndex) {
        if (view._isAddedToNativeVisualTree) {
            throw new Error("Child already added to the native visual tree.");
        }
        return true;
    };
    View.prototype._removeViewFromNativeVisualTree = function (view) {
        view._isAddedToNativeVisualTree = false;
    };
    View.prototype._syncNativeProperties = function () {
        _super.prototype._syncNativeProperties.call(this);
        this.style._syncNativeProperties();
    };
    View.prototype._goToVisualState = function (state) {
        if (trace.enabled) {
            trace.write(this + " going to state: " + state, trace.categories.Style);
        }
        if (state === this._visualState || this._requestedVisualState === state) {
            return;
        }
        ensureVisualState();
        this._visualState = visualState.goToState(this, state);
        this._requestedVisualState = state;
    };
    View.prototype._applyXmlAttribute = function (attribute, value) {
        if (attribute === "style") {
            this._applyInlineStyle(value);
            return true;
        }
        return false;
    };
    View.prototype.setInlineStyle = function (style) {
        if (!types.isString(style)) {
            throw new Error("Parameter should be valid CSS string!");
        }
        this._applyInlineStyle(style);
    };
    View.prototype._updateLayout = function () {
    };
    Object.defineProperty(View.prototype, "_nativeView", {
        get: function () {
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "_isVisible", {
        get: function () {
            return this._isVisibleCache;
        },
        enumerable: true,
        configurable: true
    });
    View.prototype._shouldApplyStyleHandlers = function () {
        return !!this._nativeView;
    };
    View.prototype.focus = function () {
        return undefined;
    };
    View.prototype.getLocationInWindow = function () {
        return undefined;
    };
    View.prototype.getLocationOnScreen = function () {
        return undefined;
    };
    View.prototype.getLocationRelativeTo = function (otherView) {
        return undefined;
    };
    View.prototype.getActualSize = function () {
        var currentBounds = this._getCurrentLayoutBounds();
        if (!currentBounds) {
            return undefined;
        }
        return {
            width: utils.layout.toDeviceIndependentPixels(currentBounds.right - currentBounds.left),
            height: utils.layout.toDeviceIndependentPixels(currentBounds.bottom - currentBounds.top),
        };
    };
    View.prototype.animate = function (animation) {
        return this.createAnimation(animation).play();
    };
    View.prototype.createAnimation = function (animation) {
        var animationModule = require("ui/animation");
        var that = this;
        animation.target = that;
        return new animationModule.Animation([animation]);
    };
    View.prototype._registerAnimation = function (animation) {
        if (this._registeredAnimations === undefined) {
            this._registeredAnimations = new Array();
        }
        this._registeredAnimations.push(animation);
    };
    View.prototype._unregisterAnimation = function (animation) {
        if (this._registeredAnimations) {
            var index_1 = this._registeredAnimations.indexOf(animation);
            if (index_1 >= 0) {
                this._registeredAnimations.splice(index_1, 1);
            }
        }
    };
    View.prototype._unregisterAllAnimations = function () {
        if (this._registeredAnimations) {
            for (var _i = 0, _a = this._registeredAnimations; _i < _a.length; _i++) {
                var animation = _a[_i];
                animation.cancel();
            }
        }
    };
    View.prototype.toString = function () {
        var str = this.typeName;
        if (this.id) {
            str += "<" + this.id + ">";
        }
        else {
            str += "(" + this._domId + ")";
        }
        var source = debug_1.Source.get(this);
        if (source) {
            str += "@" + source + ";";
        }
        return str;
    };
    View.prototype._setNativeViewFrame = function (nativeView, frame) {
    };
    View.prototype._onStylePropertyChanged = function (property) {
    };
    View.prototype._canApplyNativeProperty = function () {
        return !!this._nativeView;
    };
    View.loadedEvent = "loaded";
    View.unloadedEvent = "unloaded";
    View.automationTextProperty = automationTextProperty;
    View.idProperty = idProperty;
    View.cssClassProperty = cssClassProperty;
    View.classNameProperty = classNameProperty;
    View.originXProperty = originXProperty;
    View.originYProperty = originYProperty;
    View.isEnabledProperty = isEnabledProperty;
    View.isUserInteractionEnabledProperty = isUserInteractionEnabledProperty;
    return View;
}(proxy_1.ProxyObject));
exports.View = View;

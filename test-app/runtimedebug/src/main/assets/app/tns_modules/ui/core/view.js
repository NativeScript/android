var viewCommon = require("./view-common");
var trace = require("trace");
var utils = require("utils/utils");
var types = require("utils/types");
var style = require("ui/styling/style");
var enums = require("ui/enums");
var background = require("ui/styling/background");
global.moduleMerge(viewCommon, exports);
var ANDROID = "_android";
var NATIVE_VIEW = "_nativeView";
var VIEW_GROUP = "_viewGroup";
function onAutomationTextPropertyChanged(data) {
    var view = data.object;
    view._nativeView.setContentDescription(data.newValue);
}
viewCommon.View.automationTextProperty.metadata.onSetNativeValue = onAutomationTextPropertyChanged;
function onIdPropertyChanged(data) {
    var view = data.object;
    view._nativeView.setTag(data.newValue);
}
viewCommon.View.idProperty.metadata.onSetNativeValue = onIdPropertyChanged;
function onOriginXPropertyChanged(data) {
    org.nativescript.widgets.OriginPoint.setX(data.object._nativeView, data.newValue);
}
viewCommon.View.originXProperty.metadata.onSetNativeValue = onOriginXPropertyChanged;
function onOriginYPropertyChanged(data) {
    org.nativescript.widgets.OriginPoint.setY(data.object._nativeView, data.newValue);
}
viewCommon.View.originYProperty.metadata.onSetNativeValue = onOriginYPropertyChanged;
function onIsEnabledPropertyChanged(data) {
    var view = data.object;
    view._nativeView.setEnabled(data.newValue);
}
viewCommon.View.isEnabledProperty.metadata.onSetNativeValue = onIsEnabledPropertyChanged;
function onIsUserInteractionEnabledPropertyChanged(data) {
    var view = data.object;
    view._updateOnTouchListener(data.newValue);
}
viewCommon.View.isUserInteractionEnabledProperty.metadata.onSetNativeValue = onIsUserInteractionEnabledPropertyChanged;
var View = (function (_super) {
    __extends(View, _super);
    function View() {
        _super.apply(this, arguments);
        this._disableUserInteractionListener = new android.view.View.OnTouchListener({
            onTouch: function (view, event) {
                return true;
            }
        });
    }
    View.prototype._updateOnTouchListener = function (isUserInteractionEnabled) {
        if (!isUserInteractionEnabled) {
            this._nativeView.setOnTouchListener(this._disableUserInteractionListener);
            return;
        }
        if (this._gesturesListener) {
            this._nativeView.setOnTouchListener(this._gesturesListener);
            return;
        }
        this._nativeView.setOnTouchListener(null);
    };
    Object.defineProperty(View.prototype, "gesturesListener", {
        set: function (value) {
            this._gesturesListener = value;
            this._updateOnTouchListener(this.isUserInteractionEnabled);
        },
        enumerable: true,
        configurable: true
    });
    View.prototype.observe = function (type, callback, thisArg) {
        _super.prototype.observe.call(this, type, callback, thisArg);
        if (this.isLoaded && !this.touchListenerIsSet) {
            this.setOnTouchListener();
        }
    };
    View.prototype.onLoaded = function () {
        _super.prototype.onLoaded.call(this);
        this.setOnTouchListener();
    };
    View.prototype.onUnloaded = function () {
        _super.prototype.onUnloaded.call(this);
        this._unregisterAllAnimations();
        if (this._nativeView && this._nativeView.setOnTouchListener) {
            this._nativeView.setOnTouchListener(null);
            this.touchListenerIsSet = false;
        }
    };
    View.prototype.hasGestureObservers = function () {
        return this._gestureObservers && Object.keys(this._gestureObservers).length > 0;
    };
    View.prototype.setOnTouchListener = function () {
        if (this._nativeView && this._nativeView.setOnTouchListener && this.hasGestureObservers()) {
            this.touchListenerIsSet = true;
            var that = new WeakRef(this);
            if (this._nativeView.setClickable) {
                this._nativeView.setClickable(true);
            }
            this._nativeView.setOnTouchListener(new android.view.View.OnTouchListener({
                onTouch: function (view, motionEvent) {
                    var owner = that.get();
                    if (!owner) {
                        return false;
                    }
                    for (var type in owner._gestureObservers) {
                        var list = owner._gestureObservers[type];
                        for (var i = 0; i < list.length; i++) {
                            list[i].androidOnTouchEvent(motionEvent);
                        }
                    }
                    if (!owner._nativeView || !owner._nativeView.onTouchEvent) {
                        return false;
                    }
                    return owner._nativeView.onTouchEvent(motionEvent);
                }
            }));
        }
    };
    View.prototype._addViewCore = function (view, atIndex) {
        if (this._context) {
            view._onAttached(this._context);
        }
        _super.prototype._addViewCore.call(this, view, atIndex);
    };
    View.prototype._removeViewCore = function (view) {
        _super.prototype._removeViewCore.call(this, view);
        if (view._context) {
            view._onDetached();
        }
    };
    View.prototype._onAttached = function (context) {
        if (!context) {
            throw new Error("Expected valid android.content.Context instance.");
        }
        if (trace.enabled) {
            trace.write(this + "._onAttached(context)", trace.categories.VisualTreeEvents);
        }
        if (this._context === context) {
            return;
        }
        if (this._context) {
            this._onDetached(true);
        }
        this._context = context;
        this._onContextChanged();
        trace.notifyEvent(this, "_onAttached");
        if (this._childrenCount > 0) {
            var that = this;
            var eachChild = function (child) {
                child._onAttached(context);
                if (!child._isAddedToNativeVisualTree) {
                    child._isAddedToNativeVisualTree = that._addViewToNativeVisualTree(child);
                }
                return true;
            };
            this._eachChildView(eachChild);
        }
    };
    View.prototype._onDetached = function (force) {
        if (trace.enabled) {
            trace.write(this + "._onDetached(force)", trace.categories.VisualTreeEvents);
        }
        if (this._childrenCount > 0) {
            var that = this;
            var eachChild = function (child) {
                if (child._isAddedToNativeVisualTree) {
                    that._removeViewFromNativeVisualTree(child);
                }
                if (child._context) {
                    child._onDetached(force);
                }
                return true;
            };
            this._eachChildView(eachChild);
        }
        this._clearAndroidReference();
        this._context = undefined;
        trace.notifyEvent(this, "_onDetached");
    };
    View.prototype._clearAndroidReference = function () {
        if (this[NATIVE_VIEW] === this[ANDROID]) {
            this[NATIVE_VIEW] = undefined;
        }
        if (this[VIEW_GROUP] === this[ANDROID]) {
            this[VIEW_GROUP] = undefined;
        }
        this[ANDROID] = undefined;
    };
    View.prototype._onContextChanged = function () {
        if (trace.enabled) {
            trace.write(this + "._onContextChanged", trace.categories.VisualTreeEvents);
        }
        this._createUI();
        if (this._nativeView && !this._nativeView.getLayoutParams()) {
            this._nativeView.setLayoutParams(new org.nativescript.widgets.CommonLayoutParams());
        }
        this._syncNativeProperties();
        trace.notifyEvent(this, "_onContextChanged");
    };
    Object.defineProperty(View.prototype, "_nativeView", {
        get: function () {
            return this.android;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "isLayoutRequired", {
        get: function () {
            return !this.isLayoutValid;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(View.prototype, "isLayoutValid", {
        get: function () {
            if (this._nativeView) {
                return !this._nativeView.isLayoutRequested();
            }
            return false;
        },
        enumerable: true,
        configurable: true
    });
    View.prototype.layoutNativeView = function (left, top, right, bottom) {
        if (this._nativeView) {
            this._nativeView.layout(left, top, right, bottom);
        }
    };
    View.prototype.requestLayout = function () {
        _super.prototype.requestLayout.call(this);
        if (this._nativeView) {
            return this._nativeView.requestLayout();
        }
    };
    View.prototype.measure = function (widthMeasureSpec, heightMeasureSpec) {
        _super.prototype.measure.call(this, widthMeasureSpec, heightMeasureSpec);
        this.onMeasure(widthMeasureSpec, heightMeasureSpec);
    };
    View.prototype.layout = function (left, top, right, bottom) {
        _super.prototype.layout.call(this, left, top, right, bottom);
        this.onLayout(left, top, right, bottom);
    };
    View.prototype.onMeasure = function (widthMeasureSpec, heightMeasureSpec) {
        var view = this._nativeView;
        if (view) {
            view.measure(widthMeasureSpec, heightMeasureSpec);
            this.setMeasuredDimension(view.getMeasuredWidth(), view.getMeasuredHeight());
        }
    };
    View.prototype.onLayout = function (left, top, right, bottom) {
        var view = this._nativeView;
        if (view) {
            this.layoutNativeView(left, top, right, bottom);
        }
    };
    View.prototype._getCurrentLayoutBounds = function () {
        if (this._nativeView) {
            return {
                left: this._nativeView.getLeft(),
                top: this._nativeView.getTop(),
                right: this._nativeView.getRight(),
                bottom: this._nativeView.getBottom()
            };
        }
        return _super.prototype._getCurrentLayoutBounds.call(this);
    };
    View.prototype.getMeasuredWidth = function () {
        if (this._nativeView) {
            return this._nativeView.getMeasuredWidth();
        }
        return _super.prototype.getMeasuredWidth.call(this);
    };
    View.prototype.getMeasuredHeight = function () {
        if (this._nativeView) {
            return this._nativeView.getMeasuredHeight();
        }
        return _super.prototype.getMeasuredHeight.call(this);
    };
    View.prototype.focus = function () {
        if (this._nativeView) {
            return this._nativeView.requestFocus();
        }
        return false;
    };
    View.prototype.getLocationInWindow = function () {
        if (!this._nativeView || !this._nativeView.getWindowToken()) {
            return undefined;
        }
        var nativeArray = Array.create("int", 2);
        this._nativeView.getLocationInWindow(nativeArray);
        return {
            x: utils.layout.toDeviceIndependentPixels(nativeArray[0]),
            y: utils.layout.toDeviceIndependentPixels(nativeArray[1]),
        };
    };
    View.prototype.getLocationOnScreen = function () {
        if (!this._nativeView || !this._nativeView.getWindowToken()) {
            return undefined;
        }
        var nativeArray = Array.create("int", 2);
        this._nativeView.getLocationOnScreen(nativeArray);
        return {
            x: utils.layout.toDeviceIndependentPixels(nativeArray[0]),
            y: utils.layout.toDeviceIndependentPixels(nativeArray[1]),
        };
    };
    View.prototype.getLocationRelativeTo = function (otherView) {
        if (!this._nativeView || !this._nativeView.getWindowToken() ||
            !otherView._nativeView || !otherView._nativeView.getWindowToken() ||
            this._nativeView.getWindowToken() !== otherView._nativeView.getWindowToken()) {
            return undefined;
        }
        var myArray = Array.create("int", 2);
        this._nativeView.getLocationOnScreen(myArray);
        var otherArray = Array.create("int", 2);
        otherView._nativeView.getLocationOnScreen(otherArray);
        return {
            x: utils.layout.toDeviceIndependentPixels(myArray[0] - otherArray[0]),
            y: utils.layout.toDeviceIndependentPixels(myArray[1] - otherArray[1]),
        };
    };
    View.resolveSizeAndState = function (size, specSize, specMode, childMeasuredState) {
        var result = size;
        switch (specMode) {
            case utils.layout.UNSPECIFIED:
                result = size;
                break;
            case utils.layout.AT_MOST:
                if (specSize < size) {
                    result = specSize | utils.layout.MEASURED_STATE_TOO_SMALL;
                }
                break;
            case utils.layout.EXACTLY:
                result = specSize;
                break;
        }
        return result | (childMeasuredState & utils.layout.MEASURED_STATE_MASK);
    };
    return View;
}(viewCommon.View));
exports.View = View;
var CustomLayoutView = (function (_super) {
    __extends(CustomLayoutView, _super);
    function CustomLayoutView() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(CustomLayoutView.prototype, "android", {
        get: function () {
            return this._viewGroup;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(CustomLayoutView.prototype, "_nativeView", {
        get: function () {
            return this._viewGroup;
        },
        enumerable: true,
        configurable: true
    });
    CustomLayoutView.prototype._createUI = function () {
        this._viewGroup = new org.nativescript.widgets.ContentLayout(this._context);
    };
    CustomLayoutView.prototype._addViewToNativeVisualTree = function (child, atIndex) {
        _super.prototype._addViewToNativeVisualTree.call(this, child);
        if (this._nativeView && child._nativeView) {
            if (types.isNullOrUndefined(atIndex) || atIndex >= this._nativeView.getChildCount()) {
                if (trace.enabled) {
                    trace.write(this + "._nativeView.addView(" + child + "._nativeView)", trace.categories.VisualTreeEvents);
                }
                this._nativeView.addView(child._nativeView);
            }
            else {
                if (trace.enabled) {
                    trace.write(this + "._nativeView.addView(" + child + "._nativeView, " + atIndex + ")", trace.categories.VisualTreeEvents);
                }
                this._nativeView.addView(child._nativeView, atIndex);
            }
            return true;
        }
        return false;
    };
    CustomLayoutView.prototype._removeViewFromNativeVisualTree = function (child) {
        _super.prototype._removeViewFromNativeVisualTree.call(this, child);
        if (this._nativeView && child._nativeView) {
            if (trace.enabled) {
                trace.write(this + "._nativeView.removeView(" + child + "._nativeView)", trace.categories.VisualTreeEvents);
            }
            this._nativeView.removeView(child._nativeView);
            trace.notifyEvent(child, "childInLayoutRemovedFromNativeVisualTree");
        }
    };
    return CustomLayoutView;
}(View));
exports.CustomLayoutView = CustomLayoutView;
var ViewStyler = (function () {
    function ViewStyler() {
    }
    ViewStyler.setBackgroundBorderProperty = function (view, newValue, defaultValue) {
        background.ad.onBackgroundOrBorderPropertyChanged(view);
    };
    ViewStyler.resetBackgroundBorderProperty = function (view, nativeValue) {
        background.ad.onBackgroundOrBorderPropertyChanged(view);
    };
    ViewStyler.setVisibilityProperty = function (view, newValue) {
        var androidValue = (newValue === enums.Visibility.visible) ? android.view.View.VISIBLE : android.view.View.GONE;
        view._nativeView.setVisibility(androidValue);
    };
    ViewStyler.resetVisibilityProperty = function (view, nativeValue) {
        view._nativeView.setVisibility(android.view.View.VISIBLE);
    };
    ViewStyler.setOpacityProperty = function (view, newValue) {
        view._nativeView.setAlpha(float(newValue));
    };
    ViewStyler.resetOpacityProperty = function (view, nativeValue) {
        view._nativeView.setAlpha(float(1.0));
    };
    ViewStyler.setMinWidthProperty = function (view, newValue) {
        view._nativeView.setMinimumWidth(Math.round(newValue * utils.layout.getDisplayDensity()));
    };
    ViewStyler.resetMinWidthProperty = function (view, nativeValue) {
        view._nativeView.setMinimumWidth(0);
    };
    ViewStyler.setMinHeightProperty = function (view, newValue) {
        view._nativeView.setMinimumHeight(Math.round(newValue * utils.layout.getDisplayDensity()));
    };
    ViewStyler.resetMinHeightProperty = function (view, nativeValue) {
        view._nativeView.setMinimumHeight(0);
    };
    ViewStyler.setNativeLayoutParamsProperty = function (view, params) {
        var nativeView = view._nativeView;
        var width = params.width * utils.layout.getDisplayDensity();
        var height = params.height * utils.layout.getDisplayDensity();
        if (width < 0) {
            width = -2;
        }
        if (height < 0) {
            height = -2;
        }
        var gravity = 0;
        switch (params.horizontalAlignment) {
            case enums.HorizontalAlignment.left:
                gravity |= android.view.Gravity.LEFT;
                break;
            case enums.HorizontalAlignment.center:
                gravity |= android.view.Gravity.CENTER_HORIZONTAL;
                break;
            case enums.HorizontalAlignment.right:
                gravity |= android.view.Gravity.RIGHT;
                break;
            case enums.HorizontalAlignment.stretch:
                gravity |= android.view.Gravity.FILL_HORIZONTAL;
                if (width < 0) {
                    width = -1;
                }
                break;
            default:
                throw new Error("Invalid horizontalAlignment value: " + params.horizontalAlignment);
        }
        switch (params.verticalAlignment) {
            case enums.VerticalAlignment.top:
                gravity |= android.view.Gravity.TOP;
                break;
            case enums.VerticalAlignment.center:
            case enums.VerticalAlignment.middle:
                gravity |= android.view.Gravity.CENTER_VERTICAL;
                break;
            case enums.VerticalAlignment.bottom:
                gravity |= android.view.Gravity.BOTTOM;
                break;
            case enums.VerticalAlignment.stretch:
                gravity |= android.view.Gravity.FILL_VERTICAL;
                if (height < 0) {
                    height = -1;
                }
                break;
            default:
                throw new Error("Invalid verticalAlignment value: " + params.verticalAlignment);
        }
        var lp = nativeView.getLayoutParams();
        lp.width = Math.round(width);
        lp.height = Math.round(height);
        if (lp instanceof org.nativescript.widgets.CommonLayoutParams) {
            lp.widthPercent = params.widthPercent;
            lp.heightPercent = params.heightPercent;
            lp.leftMarginPercent = params.leftMarginPercent;
            lp.topMarginPercent = params.topMarginPercent;
            lp.rightMarginPercent = params.rightMarginPercent;
            lp.bottomMarginPercent = params.bottomMarginPercent;
            lp.leftMargin = Math.round(params.leftMargin * utils.layout.getDisplayDensity());
            lp.topMargin = Math.round(params.topMargin * utils.layout.getDisplayDensity());
            lp.rightMargin = Math.round(params.rightMargin * utils.layout.getDisplayDensity());
            lp.bottomMargin = Math.round(params.bottomMargin * utils.layout.getDisplayDensity());
            lp.gravity = gravity;
        }
        else {
            var layoutParams = lp;
            if (types.isDefined(layoutParams.widthPercent)) {
                layoutParams.widthPercent = params.widthPercent;
            }
            if (types.isDefined(layoutParams.heightPercent)) {
                layoutParams.heightPercent = params.heightPercent;
            }
            if (types.isDefined(layoutParams.leftMarginPercent)) {
                layoutParams.leftMarginPercent = params.leftMarginPercent;
            }
            if (types.isDefined(layoutParams.topMarginPercent)) {
                layoutParams.topMarginPercent = params.topMarginPercent;
            }
            if (types.isDefined(layoutParams.rightMarginPercent)) {
                layoutParams.rightMarginPercent = params.rightMarginPercent;
            }
            if (types.isDefined(layoutParams.bottomMarginPercent)) {
                layoutParams.bottomMarginPercent = params.bottomMarginPercent;
            }
            if (types.isDefined(layoutParams.leftMargin)) {
                layoutParams.leftMargin = Math.round(params.leftMargin * utils.layout.getDisplayDensity());
            }
            if (types.isDefined(layoutParams.topMargin)) {
                layoutParams.topMargin = Math.round(params.topMargin * utils.layout.getDisplayDensity());
            }
            if (types.isDefined(layoutParams.rightMargin)) {
                layoutParams.rightMargin = Math.round(params.rightMargin * utils.layout.getDisplayDensity());
            }
            if (types.isDefined(layoutParams.bottomMargin)) {
                layoutParams.bottomMargin = Math.round(params.bottomMargin * utils.layout.getDisplayDensity());
            }
            if (types.isDefined(layoutParams.gravity)) {
                layoutParams.gravity = gravity;
            }
        }
        nativeView.setLayoutParams(lp);
    };
    ViewStyler.resetNativeLayoutParamsProperty = function (view, nativeValue) {
        ViewStyler.setNativeLayoutParamsProperty(view, style.nativeLayoutParamsProperty.defaultValue);
    };
    ViewStyler.setPaddingProperty = function (view, newValue) {
        var density = utils.layout.getDisplayDensity();
        var left = Math.round((newValue.left + view.borderWidth) * density);
        var top = Math.round((newValue.top + view.borderWidth) * density);
        var right = Math.round((newValue.right + view.borderWidth) * density);
        var bottom = Math.round((newValue.bottom + view.borderWidth) * density);
        view._nativeView.setPadding(left, top, right, bottom);
    };
    ViewStyler.resetPaddingProperty = function (view, nativeValue) {
        var density = utils.layout.getDisplayDensity();
        var left = Math.round((nativeValue.left + view.borderWidth) * density);
        var top = Math.round((nativeValue.top + view.borderWidth) * density);
        var right = Math.round((nativeValue.right + view.borderWidth) * density);
        var bottom = Math.round((nativeValue.bottom + view.borderWidth) * density);
        view._nativeView.setPadding(left, top, right, bottom);
    };
    ViewStyler.setRotateProperty = function (view, newValue) {
        view._nativeView.setRotation(newValue);
    };
    ViewStyler.resetRotateProperty = function (view, nativeValue) {
        view._nativeView.setRotation(float(0));
    };
    ViewStyler.setScaleXProperty = function (view, newValue) {
        view._nativeView.setScaleX(newValue);
    };
    ViewStyler.resetScaleXProperty = function (view, nativeValue) {
        view._nativeView.setScaleX(float(1.0));
    };
    ViewStyler.setScaleYProperty = function (view, newValue) {
        view._nativeView.setScaleY(newValue);
    };
    ViewStyler.resetScaleYProperty = function (view, nativeValue) {
        view._nativeView.setScaleY(float(1.0));
    };
    ViewStyler.setTranslateXProperty = function (view, newValue) {
        view._nativeView.setTranslationX(newValue * utils.layout.getDisplayDensity());
    };
    ViewStyler.resetTranslateXProperty = function (view, nativeValue) {
        view._nativeView.setTranslationX(float(0));
    };
    ViewStyler.setTranslateYProperty = function (view, newValue) {
        view._nativeView.setTranslationY(newValue * utils.layout.getDisplayDensity());
    };
    ViewStyler.resetTranslateYProperty = function (view, nativeValue) {
        view._nativeView.setTranslationY(float(0));
    };
    ViewStyler.getZIndexProperty = function (view) {
        return view.android.getZ ? view.android.getZ() : 0;
    };
    ViewStyler.setZIndexProperty = function (view, newValue) {
        if (view.android.setZ) {
            view.android.setZ(newValue);
            if (view.android instanceof android.widget.Button) {
                view.android.setStateListAnimator(null);
            }
        }
    };
    ViewStyler.resetZIndexProperty = function (view, nativeValue) {
        if (view.android.setZ) {
            view.android.setZ(nativeValue);
        }
    };
    ViewStyler.registerHandlers = function () {
        style.registerHandler(style.visibilityProperty, new style.StylePropertyChangedHandler(ViewStyler.setVisibilityProperty, ViewStyler.resetVisibilityProperty));
        style.registerHandler(style.opacityProperty, new style.StylePropertyChangedHandler(ViewStyler.setOpacityProperty, ViewStyler.resetOpacityProperty));
        style.registerHandler(style.minWidthProperty, new style.StylePropertyChangedHandler(ViewStyler.setMinWidthProperty, ViewStyler.resetMinWidthProperty));
        style.registerHandler(style.minHeightProperty, new style.StylePropertyChangedHandler(ViewStyler.setMinHeightProperty, ViewStyler.resetMinHeightProperty));
        var borderHandler = new style.StylePropertyChangedHandler(ViewStyler.setBackgroundBorderProperty, ViewStyler.resetBackgroundBorderProperty);
        style.registerHandler(style.backgroundInternalProperty, borderHandler);
        style.registerHandler(style.borderWidthProperty, borderHandler);
        style.registerHandler(style.borderColorProperty, borderHandler);
        style.registerHandler(style.borderRadiusProperty, borderHandler);
        style.registerHandler(style.clipPathProperty, borderHandler);
        style.registerHandler(style.nativeLayoutParamsProperty, new style.StylePropertyChangedHandler(ViewStyler.setNativeLayoutParamsProperty, ViewStyler.resetNativeLayoutParamsProperty));
        style.registerHandler(style.nativePaddingsProperty, new style.StylePropertyChangedHandler(ViewStyler.setPaddingProperty, ViewStyler.resetPaddingProperty), "TextBase");
        style.registerHandler(style.nativePaddingsProperty, new style.StylePropertyChangedHandler(ViewStyler.setPaddingProperty, ViewStyler.resetPaddingProperty), "Button");
        style.registerHandler(style.nativePaddingsProperty, new style.StylePropertyChangedHandler(ViewStyler.setPaddingProperty, ViewStyler.resetPaddingProperty), "LayoutBase");
        style.registerHandler(style.rotateProperty, new style.StylePropertyChangedHandler(ViewStyler.setRotateProperty, ViewStyler.resetRotateProperty));
        style.registerHandler(style.scaleXProperty, new style.StylePropertyChangedHandler(ViewStyler.setScaleXProperty, ViewStyler.resetScaleXProperty));
        style.registerHandler(style.scaleYProperty, new style.StylePropertyChangedHandler(ViewStyler.setScaleYProperty, ViewStyler.resetScaleYProperty));
        style.registerHandler(style.translateXProperty, new style.StylePropertyChangedHandler(ViewStyler.setTranslateXProperty, ViewStyler.resetTranslateXProperty));
        style.registerHandler(style.translateYProperty, new style.StylePropertyChangedHandler(ViewStyler.setTranslateYProperty, ViewStyler.resetTranslateYProperty));
        style.registerHandler(style.zIndexProperty, new style.StylePropertyChangedHandler(ViewStyler.setZIndexProperty, ViewStyler.resetZIndexProperty, ViewStyler.getZIndexProperty));
    };
    return ViewStyler;
}());
exports.ViewStyler = ViewStyler;
ViewStyler.registerHandlers();

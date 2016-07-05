var common = require("./scroll-view-common");
var utils = require("utils/utils");
var enums = require("ui/enums");
global.moduleMerge(common, exports);
common.orientationProperty.onValueChanged = function scrollViewOrientationChanged(data) {
    data.object._onOrientationChanged(data.oldValue, data.newValue);
};
var ScrollView = (function (_super) {
    __extends(ScrollView, _super);
    function ScrollView() {
        _super.apply(this, arguments);
        this._androidViewId = -1;
    }
    Object.defineProperty(ScrollView.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "_nativeView", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "horizontalOffset", {
        get: function () {
            if (!this._android) {
                return 0;
            }
            return this._android.getScrollX() / utils.layout.getDisplayDensity();
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "verticalOffset", {
        get: function () {
            if (!this._android) {
                return 0;
            }
            return this._android.getScrollY() / utils.layout.getDisplayDensity();
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "scrollableWidth", {
        get: function () {
            if (!this._android || this.orientation !== enums.Orientation.horizontal) {
                return 0;
            }
            return this._android.getScrollableLength() / utils.layout.getDisplayDensity();
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "scrollableHeight", {
        get: function () {
            if (!this._android || this.orientation !== enums.Orientation.vertical) {
                return 0;
            }
            return this._android.getScrollableLength() / utils.layout.getDisplayDensity();
        },
        enumerable: true,
        configurable: true
    });
    ScrollView.prototype.scrollToVerticalOffset = function (value, animated) {
        if (this._android && this.orientation === enums.Orientation.vertical) {
            value *= utils.layout.getDisplayDensity();
            if (animated) {
                this._android.smoothScrollTo(0, value);
            }
            else {
                this._android.scrollTo(0, value);
            }
        }
    };
    ScrollView.prototype.scrollToHorizontalOffset = function (value, animated) {
        if (this._android && this.orientation === enums.Orientation.horizontal) {
            value *= utils.layout.getDisplayDensity();
            if (animated) {
                this._android.smoothScrollTo(value, 0);
            }
            else {
                this._android.scrollTo(value, 0);
            }
        }
    };
    ScrollView.prototype._createUI = function () {
        if (this.orientation === enums.Orientation.horizontal) {
            this._android = new org.nativescript.widgets.HorizontalScrollView(this._context);
        }
        else {
            this._android = new org.nativescript.widgets.VerticalScrollView(this._context);
        }
        if (this._androidViewId < 0) {
            this._androidViewId = android.view.View.generateViewId();
        }
        this._android.setId(this._androidViewId);
    };
    ScrollView.prototype._onOrientationChanged = function (oldValue, newValue) {
        if (this._android) {
            var parent = this.parent;
            if (parent) {
                parent._removeView(this);
            }
            if (parent) {
                parent._addView(this);
            }
        }
    };
    ScrollView.prototype.attachNative = function () {
        var that = new WeakRef(this);
        this.handler = new android.view.ViewTreeObserver.OnScrollChangedListener({
            onScrollChanged: function () {
                var rootScrollView = that.get();
                if (rootScrollView && rootScrollView.android) {
                    rootScrollView.notify({
                        object: rootScrollView,
                        eventName: ScrollView.scrollEvent,
                        scrollX: rootScrollView.android.getScrollX() / utils.layout.getDisplayDensity(),
                        scrollY: rootScrollView.android.getScrollY() / utils.layout.getDisplayDensity()
                    });
                }
            }
        });
        this._android.getViewTreeObserver().addOnScrollChangedListener(this.handler);
    };
    ScrollView.prototype.dettachNative = function () {
        this._android.getViewTreeObserver().removeOnScrollChangedListener(this.handler);
        this.handler = null;
    };
    return ScrollView;
}(common.ScrollView));
exports.ScrollView = ScrollView;

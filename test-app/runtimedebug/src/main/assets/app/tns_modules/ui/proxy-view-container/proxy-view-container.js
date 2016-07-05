var types = require("utils/types");
var trace = require("trace");
var layout_base_1 = require("ui/layouts/layout-base");
var ProxyViewContainer = (function (_super) {
    __extends(ProxyViewContainer, _super);
    function ProxyViewContainer() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(ProxyViewContainer.prototype, "ios", {
        get: function () {
            return null;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ProxyViewContainer.prototype, "android", {
        get: function () {
            return null;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ProxyViewContainer.prototype, "_nativeView", {
        get: function () {
            return null;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ProxyViewContainer.prototype, "isLayoutRequested", {
        get: function () {
            return false;
        },
        enumerable: true,
        configurable: true
    });
    ProxyViewContainer.prototype._createUI = function () {
    };
    ProxyViewContainer.prototype._getNativeViewsCount = function () {
        var result = 0;
        this._eachChildView(function (cv) {
            result += cv._getNativeViewsCount();
            return true;
        });
        return result;
    };
    ProxyViewContainer.prototype._eachLayoutView = function (callback) {
        this._eachChildView(function (cv) {
            cv._eachLayoutView(callback);
            return true;
        });
    };
    ProxyViewContainer.prototype._addViewToNativeVisualTree = function (child, atIndex) {
        if (trace.enabled) {
            trace.write("ViewContainer._addViewToNativeVisualTree for a child " + child + " ViewContainer.parent: " + this.parent, trace.categories.ViewHierarchy);
        }
        _super.prototype._addViewToNativeVisualTree.call(this, child);
        var parent = this.parent;
        if (parent) {
            var baseIndex = 0;
            var insideIndex = 0;
            if (parent instanceof layout_base_1.LayoutBase) {
                baseIndex = parent._childIndexToNativeChildIndex(parent.getChildIndex(this));
            }
            if (types.isDefined(atIndex)) {
                insideIndex = this._childIndexToNativeChildIndex(atIndex);
            }
            else {
                insideIndex = this._getNativeViewsCount();
            }
            if (trace.enabled) {
                trace.write("ProxyViewContainer._addViewToNativeVisualTree at: " + atIndex + " base: " + baseIndex + " additional: " + insideIndex, trace.categories.ViewHierarchy);
            }
            return parent._addViewToNativeVisualTree(child, baseIndex + insideIndex);
        }
        return false;
    };
    ProxyViewContainer.prototype._removeViewFromNativeVisualTree = function (child) {
        if (trace.enabled) {
            trace.write("ProxyViewContainer._removeViewFromNativeVisualTree for a child " + child + " ViewContainer.parent: " + this.parent, trace.categories.ViewHierarchy);
        }
        _super.prototype._removeViewFromNativeVisualTree.call(this, child);
        var parent = this.parent;
        if (parent) {
            return parent._removeViewFromNativeVisualTree(child);
        }
    };
    ProxyViewContainer.prototype._addToSuperview = function (superview, atIndex) {
        var _this = this;
        var index = 0;
        this._eachChildView(function (cv) {
            if (!cv._isAddedToNativeVisualTree) {
                cv._isAddedToNativeVisualTree = _this._addViewToNativeVisualTree(cv, index++);
            }
            return true;
        });
        return true;
    };
    ProxyViewContainer.prototype._removeFromSuperview = function () {
        var _this = this;
        this._eachChildView(function (cv) {
            if (cv._isAddedToNativeVisualTree) {
                _this._removeViewFromNativeVisualTree(cv);
            }
            return true;
        });
    };
    ProxyViewContainer.prototype._registerLayoutChild = function (child) {
        if (this.parent instanceof layout_base_1.LayoutBase) {
            this.parent._registerLayoutChild(child);
        }
    };
    ProxyViewContainer.prototype._unregisterLayoutChild = function (child) {
        if (this.parent instanceof layout_base_1.LayoutBase) {
            this.parent._unregisterLayoutChild(child);
        }
    };
    ProxyViewContainer.prototype._parentChanged = function (oldParent) {
        var addingToParent = this.parent && !oldParent;
        var newLayout = this.parent;
        var oldLayout = oldParent;
        if (addingToParent && newLayout instanceof layout_base_1.LayoutBase) {
            this._eachChildView(function (child) {
                newLayout._registerLayoutChild(child);
                return true;
            });
        }
        else if (oldLayout instanceof layout_base_1.LayoutBase) {
            this._eachChildView(function (child) {
                oldLayout._unregisterLayoutChild(child);
                return true;
            });
        }
    };
    return ProxyViewContainer;
}(layout_base_1.LayoutBase));
exports.ProxyViewContainer = ProxyViewContainer;

var types = require("utils/types");
var view = require("ui/core/view");
var dependencyObservable = require("ui/core/dependency-observable");
var utils = require("utils/utils");
var style = require("ui/styling/style");
var dependency_observable_1 = require("ui/core/dependency-observable");
var proxy_1 = require("ui/core/proxy");
var clipToBoundsProperty = new dependency_observable_1.Property("clipToBounds", "LayoutBase", new proxy_1.PropertyMetadata(true, dependencyObservable.PropertyMetadataSettings.None));
function onClipToBoundsPropertyChanged(data) {
    var layout = data.object;
    layout._onClipToBoundsChanged(data.oldValue, data.newValue);
}
clipToBoundsProperty.metadata.onSetNativeValue = onClipToBoundsPropertyChanged;
var LayoutBase = (function (_super) {
    __extends(LayoutBase, _super);
    function LayoutBase() {
        _super.apply(this, arguments);
        this._subViews = new Array();
    }
    LayoutBase.prototype._addChildFromBuilder = function (name, value) {
        if (value instanceof view.View) {
            this.addChild(value);
        }
    };
    LayoutBase.prototype.getChildrenCount = function () {
        return this._subViews.length;
    };
    Object.defineProperty(LayoutBase.prototype, "_childrenCount", {
        get: function () {
            return this._subViews.length;
        },
        enumerable: true,
        configurable: true
    });
    LayoutBase.prototype.getChildAt = function (index) {
        return this._subViews[index];
    };
    LayoutBase.prototype.getChildIndex = function (child) {
        return this._subViews.indexOf(child);
    };
    LayoutBase.prototype.getChildById = function (id) {
        return view.getViewById(this, id);
    };
    LayoutBase.prototype._registerLayoutChild = function (child) {
    };
    LayoutBase.prototype._unregisterLayoutChild = function (child) {
    };
    LayoutBase.prototype.addChild = function (child) {
        this._subViews.push(child);
        this._addView(child);
        this._registerLayoutChild(child);
    };
    LayoutBase.prototype.insertChild = function (child, atIndex) {
        this._subViews.splice(atIndex, 0, child);
        this._addView(child, atIndex);
        this._registerLayoutChild(child);
    };
    LayoutBase.prototype.removeChild = function (child) {
        this._removeView(child);
        var index = this._subViews.indexOf(child);
        this._subViews.splice(index, 1);
        this._unregisterLayoutChild(child);
    };
    LayoutBase.prototype.removeChildren = function () {
        while (this.getChildrenCount() !== 0) {
            this.removeChild(this._subViews[this.getChildrenCount() - 1]);
        }
    };
    Object.defineProperty(LayoutBase.prototype, "padding", {
        get: function () {
            return this.style.padding;
        },
        set: function (value) {
            this.style.padding = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(LayoutBase.prototype, "paddingTop", {
        get: function () {
            return this.style.paddingTop;
        },
        set: function (value) {
            this.style.paddingTop = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(LayoutBase.prototype, "paddingRight", {
        get: function () {
            return this.style.paddingRight;
        },
        set: function (value) {
            this.style.paddingRight = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(LayoutBase.prototype, "paddingBottom", {
        get: function () {
            return this.style.paddingBottom;
        },
        set: function (value) {
            this.style.paddingBottom = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(LayoutBase.prototype, "paddingLeft", {
        get: function () {
            return this.style.paddingLeft;
        },
        set: function (value) {
            this.style.paddingLeft = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(LayoutBase.prototype, "clipToBounds", {
        get: function () {
            return this._getValue(LayoutBase.clipToBoundsProperty);
        },
        set: function (value) {
            this._setValue(LayoutBase.clipToBoundsProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    LayoutBase.prototype._onClipToBoundsChanged = function (oldValue, newValue) {
    };
    LayoutBase.prototype._childIndexToNativeChildIndex = function (index) {
        if (types.isUndefined(index)) {
            return undefined;
        }
        var result = 0;
        for (var i = 0; i < index && i < this._subViews.length; i++) {
            result += this._subViews[i]._getNativeViewsCount();
        }
        return result;
    };
    LayoutBase.prototype._eachChildView = function (callback) {
        var i;
        var length = this._subViews.length;
        var retVal;
        for (i = 0; i < length; i++) {
            retVal = callback(this._subViews[i]);
            if (retVal === false) {
                break;
            }
        }
    };
    LayoutBase.prototype.eachLayoutChild = function (callback) {
        var lastChild = null;
        this._eachChildView(function (cv) {
            cv._eachLayoutView(function (lv) {
                if (lastChild && lastChild._isVisible) {
                    callback(lastChild, false);
                }
                lastChild = lv;
            });
            return true;
        });
        if (lastChild && lastChild._isVisible) {
            callback(lastChild, true);
        }
    };
    LayoutBase.adjustChildrenLayoutParams = function (layoutBase, widthMeasureSpec, heightMeasureSpec) {
        var availableWidth = utils.layout.getMeasureSpecSize(widthMeasureSpec);
        var widthSpec = utils.layout.getMeasureSpecMode(widthMeasureSpec);
        var availableHeight = utils.layout.getMeasureSpecSize(heightMeasureSpec);
        var heightSpec = utils.layout.getMeasureSpecMode(heightMeasureSpec);
        for (var i = 0, count = layoutBase.getChildrenCount(); i < count; i++) {
            var child = layoutBase.getChildAt(i);
            var lp = child.style._getValue(style.nativeLayoutParamsProperty);
            if (widthSpec !== utils.layout.UNSPECIFIED) {
                if (lp.widthPercent > 0) {
                    lp.width = Math.round(availableWidth * lp.widthPercent);
                }
                if (lp.leftMarginPercent > 0) {
                    lp.leftMargin = Math.round(availableWidth * lp.leftMarginPercent);
                }
                if (lp.rightMarginPercent > 0) {
                    lp.rightMargin = Math.round(availableWidth * lp.rightMarginPercent);
                }
            }
            if (heightSpec !== utils.layout.UNSPECIFIED) {
                if (lp.heightPercent > 0) {
                    lp.height = Math.round(availableHeight * lp.heightPercent);
                }
                if (lp.topMarginPercent > 0) {
                    lp.topMargin = Math.round(availableHeight * lp.topMarginPercent);
                }
                if (lp.bottomMarginPercent > 0) {
                    lp.bottomMargin = Math.round(availableHeight * lp.bottomMarginPercent);
                }
            }
        }
    };
    LayoutBase.restoreOriginalParams = function (layoutBase) {
        for (var i = 0, count = layoutBase.getChildrenCount(); i < count; i++) {
            var child = layoutBase.getChildAt(i);
            var lp = child.style._getValue(style.nativeLayoutParamsProperty);
            if (lp.widthPercent > 0) {
                lp.width = -1;
            }
            if (lp.heightPercent > 0) {
                lp.height = -1;
            }
            if (lp.leftMarginPercent > 0) {
                lp.leftMargin = 0;
            }
            if (lp.topMarginPercent > 0) {
                lp.topMargin = 0;
            }
            if (lp.rightMarginPercent > 0) {
                lp.rightMargin = 0;
            }
            if (lp.bottomMarginPercent > 0) {
                lp.bottomMargin = 0;
            }
        }
    };
    LayoutBase.clipToBoundsProperty = clipToBoundsProperty;
    return LayoutBase;
}(view.CustomLayoutView));
exports.LayoutBase = LayoutBase;

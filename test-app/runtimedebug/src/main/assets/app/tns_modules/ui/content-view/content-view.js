var view = require("ui/core/view");
var utils = require("utils/utils");
var ContentView = (function (_super) {
    __extends(ContentView, _super);
    function ContentView() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(ContentView.prototype, "content", {
        get: function () {
            return this._content;
        },
        set: function (value) {
            var oldView = this._content;
            if (this._content) {
                this._removeView(this._content);
            }
            this._content = value;
            if (this._content) {
                this._addView(this._content);
            }
            this._onContentChanged(oldView, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ContentView.prototype, "layoutView", {
        get: function () {
            var result;
            if (this._content) {
                var first_1 = true;
                this._content._eachLayoutView(function (child) {
                    if (first_1) {
                        first_1 = false;
                        result = child;
                    }
                    else {
                        throw new Error("More than one layout child inside a ContentView");
                    }
                });
            }
            return result;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ContentView.prototype, "_childrenCount", {
        get: function () {
            if (this._content) {
                return 1;
            }
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    ContentView.prototype._onContentChanged = function (oldView, newView) {
    };
    ContentView.prototype._addChildFromBuilder = function (name, value) {
        if (value instanceof view.View) {
            this.content = value;
        }
    };
    ContentView.prototype._eachChildView = function (callback) {
        if (this._content) {
            callback(this._content);
        }
    };
    ContentView.prototype.onMeasure = function (widthMeasureSpec, heightMeasureSpec) {
        var result = view.View.measureChild(this, this.layoutView, widthMeasureSpec, heightMeasureSpec);
        var width = utils.layout.getMeasureSpecSize(widthMeasureSpec);
        var widthMode = utils.layout.getMeasureSpecMode(widthMeasureSpec);
        var height = utils.layout.getMeasureSpecSize(heightMeasureSpec);
        var heightMode = utils.layout.getMeasureSpecMode(heightMeasureSpec);
        var density = utils.layout.getDisplayDensity();
        var measureWidth = Math.max(result.measuredWidth, this.minWidth * density);
        var measureHeight = Math.max(result.measuredHeight, this.minHeight * density);
        var widthAndState = view.View.resolveSizeAndState(measureWidth, width, widthMode, 0);
        var heightAndState = view.View.resolveSizeAndState(measureHeight, height, heightMode, 0);
        this.setMeasuredDimension(widthAndState, heightAndState);
    };
    ContentView.prototype.onLayout = function (left, top, right, bottom) {
        view.View.layoutChild(this, this.layoutView, 0, 0, right - left, bottom - top);
    };
    return ContentView;
}(view.CustomLayoutView));
exports.ContentView = ContentView;

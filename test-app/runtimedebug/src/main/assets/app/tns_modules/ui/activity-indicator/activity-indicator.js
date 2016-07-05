var aiCommon = require("./activity-indicator-common");
var enums = require("ui/enums");
var style = require("ui/styling/style");
function onBusyPropertyChanged(data) {
    var indicator = data.object;
    if (!indicator.android) {
        return;
    }
    if (indicator.visibility === enums.Visibility.visible) {
        indicator.android.setVisibility(data.newValue ? android.view.View.VISIBLE : android.view.View.INVISIBLE);
    }
}
aiCommon.ActivityIndicator.busyProperty.metadata.onSetNativeValue = onBusyPropertyChanged;
global.moduleMerge(aiCommon, exports);
var ActivityIndicator = (function (_super) {
    __extends(ActivityIndicator, _super);
    function ActivityIndicator() {
        _super.apply(this, arguments);
    }
    ActivityIndicator.prototype._createUI = function () {
        this._android = new android.widget.ProgressBar(this._context);
        this._android.setVisibility(android.view.View.INVISIBLE);
        this._android.setIndeterminate(true);
    };
    Object.defineProperty(ActivityIndicator.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    return ActivityIndicator;
}(aiCommon.ActivityIndicator));
exports.ActivityIndicator = ActivityIndicator;
var ActivityIndicatorStyler = (function () {
    function ActivityIndicatorStyler() {
    }
    ActivityIndicatorStyler.setColorProperty = function (view, newValue) {
        var bar = view._nativeView;
        bar.getIndeterminateDrawable().setColorFilter(newValue, android.graphics.PorterDuff.Mode.SRC_IN);
    };
    ActivityIndicatorStyler.resetColorProperty = function (view, nativeValue) {
        var bar = view._nativeView;
        bar.getIndeterminateDrawable().clearColorFilter();
    };
    ActivityIndicatorStyler.setActivityIndicatorVisibilityProperty = function (view, newValue) {
        ActivityIndicatorStyler.setIndicatorVisibility(view.busy, newValue, view._nativeView);
    };
    ActivityIndicatorStyler.resetActivityIndicatorVisibilityProperty = function (view, nativeValue) {
        ActivityIndicatorStyler.setIndicatorVisibility(view.busy, enums.Visibility.visible, view._nativeView);
    };
    ActivityIndicatorStyler.setIndicatorVisibility = function (isBusy, visibility, nativeView) {
        if (visibility === enums.Visibility.collapsed || visibility === enums.Visibility.collapse) {
            nativeView.setVisibility(android.view.View.GONE);
        }
        else {
            nativeView.setVisibility(isBusy ? android.view.View.VISIBLE : android.view.View.INVISIBLE);
        }
    };
    ActivityIndicatorStyler.registerHandlers = function () {
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(ActivityIndicatorStyler.setColorProperty, ActivityIndicatorStyler.resetColorProperty), "ActivityIndicator");
        style.registerHandler(style.visibilityProperty, new style.StylePropertyChangedHandler(ActivityIndicatorStyler.setActivityIndicatorVisibilityProperty, ActivityIndicatorStyler.resetActivityIndicatorVisibilityProperty), "ActivityIndicator");
    };
    return ActivityIndicatorStyler;
}());
exports.ActivityIndicatorStyler = ActivityIndicatorStyler;
ActivityIndicatorStyler.registerHandlers();

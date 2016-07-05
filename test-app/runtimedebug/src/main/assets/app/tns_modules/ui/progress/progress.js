var common = require("./progress-common");
var style = require("ui/styling/style");
var R_ATTR_PROGRESS_BAR_STYLE_HORIZONTAL = 0x01010078;
function onValuePropertyChanged(data) {
    var progress = data.object;
    if (!progress.android) {
        return;
    }
    progress.android.setProgress(data.newValue);
}
function onMaxValuePropertyChanged(data) {
    var progress = data.object;
    if (!progress.android) {
        return;
    }
    progress.android.setMax(data.newValue);
}
common.Progress.valueProperty.metadata.onSetNativeValue = onValuePropertyChanged;
common.Progress.maxValueProperty.metadata.onSetNativeValue = onMaxValuePropertyChanged;
global.moduleMerge(common, exports);
var Progress = (function (_super) {
    __extends(Progress, _super);
    function Progress() {
        _super.apply(this, arguments);
    }
    Progress.prototype._createUI = function () {
        this._android = new android.widget.ProgressBar(this._context, null, R_ATTR_PROGRESS_BAR_STYLE_HORIZONTAL);
    };
    Object.defineProperty(Progress.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    return Progress;
}(common.Progress));
exports.Progress = Progress;
var ProgressStyler = (function () {
    function ProgressStyler() {
    }
    ProgressStyler.setColorProperty = function (view, newValue) {
        var bar = view._nativeView;
        bar.getProgressDrawable().setColorFilter(newValue, android.graphics.PorterDuff.Mode.SRC_IN);
    };
    ProgressStyler.resetColorProperty = function (view, nativeValue) {
        var bar = view._nativeView;
        bar.getProgressDrawable().clearColorFilter();
    };
    ProgressStyler.setBackgroundAndBorderProperty = function (view, newValue) {
        var bar = view._nativeView;
        var progressDrawable = bar.getProgressDrawable();
        if (progressDrawable.getNumberOfLayers && progressDrawable.getNumberOfLayers() > 0) {
            var backgroundDrawable = progressDrawable.getDrawable(0);
            if (backgroundDrawable) {
                backgroundDrawable.setColorFilter(newValue, android.graphics.PorterDuff.Mode.SRC_IN);
            }
        }
    };
    ProgressStyler.resetBackgroundAndBorderProperty = function (view, nativeValue) {
    };
    ProgressStyler.registerHandlers = function () {
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(ProgressStyler.setColorProperty, ProgressStyler.resetColorProperty), "Progress");
        style.registerHandler(style.backgroundColorProperty, new style.StylePropertyChangedHandler(ProgressStyler.setBackgroundAndBorderProperty, ProgressStyler.resetBackgroundAndBorderProperty), "Progress");
        style.registerHandler(style.borderWidthProperty, style.ignorePropertyHandler, "Progress");
        style.registerHandler(style.borderColorProperty, style.ignorePropertyHandler, "Progress");
        style.registerHandler(style.borderRadiusProperty, style.ignorePropertyHandler, "Progress");
        style.registerHandler(style.backgroundInternalProperty, style.ignorePropertyHandler, "Progress");
    };
    return ProgressStyler;
}());
exports.ProgressStyler = ProgressStyler;
ProgressStyler.registerHandlers();

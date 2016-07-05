var common = require("./switch-common");
var style = require("ui/styling/style");
function onCheckedPropertyChanged(data) {
    var swtch = data.object;
    if (!swtch.android) {
        return;
    }
    swtch.android.setChecked(data.newValue);
}
common.Switch.checkedProperty.metadata.onSetNativeValue = onCheckedPropertyChanged;
global.moduleMerge(common, exports);
var Switch = (function (_super) {
    __extends(Switch, _super);
    function Switch() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(Switch.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    Switch.prototype._createUI = function () {
        this._android = new android.widget.Switch(this._context);
        var that = new WeakRef(this);
        this._android.setOnCheckedChangeListener(new android.widget.CompoundButton.OnCheckedChangeListener({
            get owner() {
                return that.get();
            },
            onCheckedChanged: function (sender, isChecked) {
                if (this.owner) {
                    this.owner._onPropertyChangedFromNative(common.Switch.checkedProperty, isChecked);
                }
            }
        }));
    };
    return Switch;
}(common.Switch));
exports.Switch = Switch;
var SwitchStyler = (function () {
    function SwitchStyler() {
    }
    SwitchStyler.setColorProperty = function (view, newValue) {
        var sw = view._nativeView;
        var drawable = sw.getThumbDrawable();
        if (drawable) {
            drawable.setColorFilter(newValue, android.graphics.PorterDuff.Mode.SRC_IN);
        }
    };
    SwitchStyler.resetColorProperty = function (view, nativeValue) {
    };
    SwitchStyler.setBackgroundAndBorderProperty = function (view, newValue) {
        var sw = view._nativeView;
        var drawable = sw.getTrackDrawable();
        if (drawable) {
            drawable.setColorFilter(newValue, android.graphics.PorterDuff.Mode.SRC_IN);
        }
    };
    SwitchStyler.resetBackgroundAndBorderProperty = function (view, nativeValue) {
    };
    SwitchStyler.registerHandlers = function () {
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(SwitchStyler.setColorProperty, SwitchStyler.resetColorProperty), "Switch");
        style.registerHandler(style.backgroundColorProperty, new style.StylePropertyChangedHandler(SwitchStyler.setBackgroundAndBorderProperty, SwitchStyler.resetBackgroundAndBorderProperty), "Switch");
        style.registerHandler(style.borderWidthProperty, style.ignorePropertyHandler, "Switch");
        style.registerHandler(style.borderColorProperty, style.ignorePropertyHandler, "Switch");
        style.registerHandler(style.borderRadiusProperty, style.ignorePropertyHandler, "Switch");
        style.registerHandler(style.backgroundInternalProperty, style.ignorePropertyHandler, "Switch");
    };
    return SwitchStyler;
}());
exports.SwitchStyler = SwitchStyler;
SwitchStyler.registerHandlers();

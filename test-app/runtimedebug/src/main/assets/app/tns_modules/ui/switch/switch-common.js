var view = require("ui/core/view");
var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var Switch = (function (_super) {
    __extends(Switch, _super);
    function Switch() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(Switch.prototype, "checked", {
        get: function () {
            return this._getValue(Switch.checkedProperty);
        },
        set: function (value) {
            this._setValue(Switch.checkedProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Switch.checkedProperty = new dependencyObservable.Property("checked", "Switch", new proxy.PropertyMetadata(false));
    return Switch;
}(view.View));
exports.Switch = Switch;

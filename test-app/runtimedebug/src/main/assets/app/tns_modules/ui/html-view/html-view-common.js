var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var view = require("ui/core/view");
var HtmlView = (function (_super) {
    __extends(HtmlView, _super);
    function HtmlView() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(HtmlView.prototype, "html", {
        get: function () {
            return this._getValue(HtmlView.htmlProperty);
        },
        set: function (value) {
            this._setValue(HtmlView.htmlProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    HtmlView.htmlProperty = new dependencyObservable.Property("html", "HtmlView", new proxy.PropertyMetadata(false, dependencyObservable.PropertyMetadataSettings.AffectsLayout));
    return HtmlView;
}(view.View));
exports.HtmlView = HtmlView;

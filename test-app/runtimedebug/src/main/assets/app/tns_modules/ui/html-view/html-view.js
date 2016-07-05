var common = require("./html-view-common");
function onHtmlPropertyChanged(data) {
    var view = data.object;
    if (!view.android) {
        return;
    }
    var types = require("utils/types");
    if (types.isString(data.newValue)) {
        var mask = 15;
        if (data.newValue.search(/<a\s/i) >= 0) {
            mask = 0;
        }
        view.android.setAutoLinkMask(mask);
        view.android.setText(android.text.Html.fromHtml(data.newValue));
    }
    else {
        view.android.setText("");
    }
}
common.HtmlView.htmlProperty.metadata.onSetNativeValue = onHtmlPropertyChanged;
global.moduleMerge(common, exports);
var HtmlView = (function (_super) {
    __extends(HtmlView, _super);
    function HtmlView() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(HtmlView.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    HtmlView.prototype._createUI = function () {
        this._android = new android.widget.TextView(this._context);
        this._android.setLinksClickable(true);
        this._android.setMovementMethod(android.text.method.LinkMovementMethod.getInstance());
    };
    return HtmlView;
}(common.HtmlView));
exports.HtmlView = HtmlView;

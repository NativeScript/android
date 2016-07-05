var common = require("./label-common");
global.moduleMerge(common, exports);
var Label = (function (_super) {
    __extends(Label, _super);
    function Label() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(Label.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    Label.prototype._createUI = function () {
        this._android = new android.widget.TextView(this._context);
        this._android.setSingleLine(true);
        this._android.setEllipsize(android.text.TextUtils.TruncateAt.END);
    };
    return Label;
}(common.Label));
exports.Label = Label;

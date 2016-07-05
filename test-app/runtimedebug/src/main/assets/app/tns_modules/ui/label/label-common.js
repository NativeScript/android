var textBase = require("ui/text-base");
var enums_1 = require("ui/enums");
var Label = (function (_super) {
    __extends(Label, _super);
    function Label() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(Label.prototype, "textWrap", {
        get: function () {
            return this.style.whiteSpace === enums_1.WhiteSpace.normal;
        },
        set: function (value) {
            this.style.whiteSpace = value ? enums_1.WhiteSpace.normal : enums_1.WhiteSpace.nowrap;
        },
        enumerable: true,
        configurable: true
    });
    return Label;
}(textBase.TextBase));
exports.Label = Label;

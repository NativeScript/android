var formattedStringCommon = require("./formatted-string-common");
var types = require("utils/types");
global.moduleMerge(formattedStringCommon, exports);
var FormattedString = (function (_super) {
    __extends(FormattedString, _super);
    function FormattedString() {
        _super.apply(this, arguments);
    }
    FormattedString.prototype.createFormattedStringCore = function () {
        var ssb = new android.text.SpannableStringBuilder();
        var i;
        var spanStart = 0;
        var spanLength = 0;
        var spanText = "";
        for (i = 0; i < this.spans.length; i++) {
            var span = this.spans.getItem(i);
            spanText = types.toUIString(span.text);
            spanLength = spanText.length;
            if (spanLength !== 0) {
                ssb.insert(spanStart, spanText);
                span.updateSpanModifiers(this);
                var p;
                for (p = 0; p < span.spanModifiers.length; p++) {
                    ssb.setSpan(span.spanModifiers[p], spanStart, spanStart + spanLength, android.text.Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                }
                spanStart += spanLength;
            }
        }
        this._formattedText = ssb;
    };
    return FormattedString;
}(formattedStringCommon.FormattedString));
exports.FormattedString = FormattedString;

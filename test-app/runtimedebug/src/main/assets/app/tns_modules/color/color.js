var common = require("./color-common");
var Color = (function (_super) {
    __extends(Color, _super);
    function Color() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(Color.prototype, "android", {
        get: function () {
            return this.argb;
        },
        enumerable: true,
        configurable: true
    });
    Color.prototype._argbFromString = function (hex) {
        return android.graphics.Color.parseColor(hex);
    };
    return Color;
}(common.Color));
exports.Color = Color;

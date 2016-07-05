var view = require("ui/core/view");
var Placeholder = (function (_super) {
    __extends(Placeholder, _super);
    function Placeholder() {
        _super.apply(this, arguments);
    }
    Placeholder.creatingViewEvent = "creatingView";
    return Placeholder;
}(view.View));
exports.Placeholder = Placeholder;

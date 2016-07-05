var layout_base_1 = require("ui/layouts/layout-base");
var view_1 = require("ui/core/view");
var proxy_1 = require("ui/core/proxy");
var dependency_observable_1 = require("ui/core/dependency-observable");
var special_properties_1 = require("ui/builder/special-properties");
function validateArgs(element) {
    if (!element) {
        throw new Error("element cannot be null or undefinied.");
    }
    return element;
}
special_properties_1.registerSpecialProperty("left", function (instance, propertyValue) {
    AbsoluteLayout.setLeft(instance, !isNaN(+propertyValue) && +propertyValue);
});
special_properties_1.registerSpecialProperty("top", function (instance, propertyValue) {
    AbsoluteLayout.setTop(instance, !isNaN(+propertyValue) && +propertyValue);
});
var AbsoluteLayout = (function (_super) {
    __extends(AbsoluteLayout, _super);
    function AbsoluteLayout() {
        _super.apply(this, arguments);
    }
    AbsoluteLayout.isValid = function (value) {
        return isFinite(value);
    };
    AbsoluteLayout.onLeftPropertyChanged = function (data) {
        var view = data.object;
        if (view instanceof view_1.View) {
            var layout = view.parent;
            if (layout instanceof AbsoluteLayout) {
                layout.onLeftChanged(view, data.oldValue, data.newValue);
            }
        }
    };
    AbsoluteLayout.onTopPropertyChanged = function (data) {
        var view = data.object;
        if (view instanceof view_1.View) {
            var layout = view.parent;
            if (layout instanceof AbsoluteLayout) {
                layout.onTopChanged(view, data.oldValue, data.newValue);
            }
        }
    };
    AbsoluteLayout.getLeft = function (element) {
        return validateArgs(element)._getValue(AbsoluteLayout.leftProperty);
    };
    AbsoluteLayout.setLeft = function (element, value) {
        validateArgs(element)._setValue(AbsoluteLayout.leftProperty, value);
    };
    AbsoluteLayout.getTop = function (element) {
        return validateArgs(element)._getValue(AbsoluteLayout.topProperty);
    };
    AbsoluteLayout.setTop = function (element, value) {
        validateArgs(element)._setValue(AbsoluteLayout.topProperty, value);
    };
    AbsoluteLayout.prototype.onLeftChanged = function (view, oldValue, newValue) {
    };
    AbsoluteLayout.prototype.onTopChanged = function (view, oldValue, newValue) {
    };
    AbsoluteLayout.leftProperty = new dependency_observable_1.Property("left", "AbsoluteLayout", new proxy_1.PropertyMetadata(0, undefined, AbsoluteLayout.onLeftPropertyChanged, AbsoluteLayout.isValid));
    AbsoluteLayout.topProperty = new dependency_observable_1.Property("top", "AbsoluteLayout", new proxy_1.PropertyMetadata(0, undefined, AbsoluteLayout.onTopPropertyChanged, AbsoluteLayout.isValid));
    return AbsoluteLayout;
}(layout_base_1.LayoutBase));
exports.AbsoluteLayout = AbsoluteLayout;

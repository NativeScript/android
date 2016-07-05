var view_1 = require("ui/core/view");
var dependency_observable_1 = require("ui/core/dependency-observable");
var proxy_1 = require("ui/core/proxy");
var color_1 = require("color");
var platform_1 = require("platform");
var AffectsLayout = platform_1.isAndroid ? dependency_observable_1.PropertyMetadataSettings.None : dependency_observable_1.PropertyMetadataSettings.AffectsLayout;
var SearchBar = (function (_super) {
    __extends(SearchBar, _super);
    function SearchBar() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(SearchBar.prototype, "text", {
        get: function () {
            return this._getValue(SearchBar.textProperty);
        },
        set: function (value) {
            this._setValue(SearchBar.textProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(SearchBar.prototype, "hint", {
        get: function () {
            return this._getValue(SearchBar.hintProperty);
        },
        set: function (value) {
            this._setValue(SearchBar.hintProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(SearchBar.prototype, "textFieldBackgroundColor", {
        get: function () {
            return this._getValue(SearchBar.textFieldBackgroundColorProperty);
        },
        set: function (value) {
            this._setValue(SearchBar.textFieldBackgroundColorProperty, value instanceof color_1.Color ? value : new color_1.Color(value));
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(SearchBar.prototype, "textFieldHintColor", {
        get: function () {
            return this._getValue(SearchBar.textFieldHintColorProperty);
        },
        set: function (value) {
            this._setValue(SearchBar.textFieldHintColorProperty, value instanceof color_1.Color ? value : new color_1.Color(value));
        },
        enumerable: true,
        configurable: true
    });
    SearchBar.prototype.dismissSoftInput = function () {
    };
    SearchBar.submitEvent = "submit";
    SearchBar.clearEvent = "clear";
    SearchBar.textFieldBackgroundColorProperty = new dependency_observable_1.Property("textFieldBackgroundColor", "SearchBar", new proxy_1.PropertyMetadata(undefined));
    SearchBar.textFieldHintColorProperty = new dependency_observable_1.Property("textFieldHintColor", "SearchBar", new proxy_1.PropertyMetadata(undefined));
    SearchBar.hintProperty = new dependency_observable_1.Property("hint", "SearchBar", new proxy_1.PropertyMetadata(""));
    SearchBar.textProperty = new dependency_observable_1.Property("text", "SearchBar", new proxy_1.PropertyMetadata("", AffectsLayout));
    return SearchBar;
}(view_1.View));
exports.SearchBar = SearchBar;

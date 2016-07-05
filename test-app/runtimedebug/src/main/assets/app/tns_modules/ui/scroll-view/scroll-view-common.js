var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var enums = require("ui/enums");
var contentView = require("ui/content-view");
function isValidOrientation(value) {
    return value === enums.Orientation.vertical || value === enums.Orientation.horizontal;
}
exports.orientationProperty = new dependencyObservable.Property("orientation", "ScrollView", new proxy.PropertyMetadata(enums.Orientation.vertical, dependencyObservable.PropertyMetadataSettings.AffectsLayout, undefined, isValidOrientation));
var ScrollView = (function (_super) {
    __extends(ScrollView, _super);
    function ScrollView() {
        _super.apply(this, arguments);
        this._scrollChangeCount = 0;
    }
    Object.defineProperty(ScrollView.prototype, "orientation", {
        get: function () {
            return this._getValue(exports.orientationProperty);
        },
        set: function (value) {
            this._setValue(exports.orientationProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    ScrollView.prototype.addEventListener = function (arg, callback, thisArg) {
        _super.prototype.addEventListener.call(this, arg, callback, thisArg);
        if (arg === ScrollView.scrollEvent) {
            this._scrollChangeCount++;
            this.attach();
        }
    };
    ScrollView.prototype.removeEventListener = function (arg, callback, thisArg) {
        _super.prototype.addEventListener.call(this, arg, callback, thisArg);
        if (arg === ScrollView.scrollEvent) {
            this._scrollChangeCount--;
            this.dettach();
        }
    };
    ScrollView.prototype.onLoaded = function () {
        _super.prototype.onLoaded.call(this);
        this.attach();
    };
    ScrollView.prototype.onUnloaded = function () {
        _super.prototype.onUnloaded.call(this);
        this.dettach();
    };
    ScrollView.prototype.attach = function () {
        if (this._scrollChangeCount > 0 && this.isLoaded) {
            this.attachNative();
        }
    };
    ScrollView.prototype.dettach = function () {
        if (this._scrollChangeCount === 0 && this.isLoaded) {
            this.dettachNative();
        }
    };
    ScrollView.prototype.attachNative = function () {
    };
    ScrollView.prototype.dettachNative = function () {
    };
    Object.defineProperty(ScrollView.prototype, "horizontalOffset", {
        get: function () {
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "verticalOffset", {
        get: function () {
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "scrollableWidth", {
        get: function () {
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScrollView.prototype, "scrollableHeight", {
        get: function () {
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    ScrollView.prototype.scrollToVerticalOffset = function (value, animated) {
    };
    ScrollView.prototype.scrollToHorizontalOffset = function (value, animated) {
    };
    ScrollView.scrollEvent = "scroll";
    return ScrollView;
}(contentView.ContentView));
exports.ScrollView = ScrollView;

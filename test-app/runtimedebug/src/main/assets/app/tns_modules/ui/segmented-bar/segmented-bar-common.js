var view = require("ui/core/view");
var proxy = require("ui/core/proxy");
var dependencyObservable = require("ui/core/dependency-observable");
var color = require("color");
var bindable = require("ui/core/bindable");
var types;
function ensureTypes() {
    if (!types) {
        types = require("utils/types");
    }
}
var knownCollections;
(function (knownCollections) {
    knownCollections.items = "items";
})(knownCollections = exports.knownCollections || (exports.knownCollections = {}));
var CHILD_SEGMENTED_BAR_ITEM = "SegmentedBarItem";
var SegmentedBarItem = (function (_super) {
    __extends(SegmentedBarItem, _super);
    function SegmentedBarItem() {
        _super.apply(this, arguments);
        this._title = "";
    }
    Object.defineProperty(SegmentedBarItem.prototype, "title", {
        get: function () {
            return this._title;
        },
        set: function (value) {
            if (this._title !== value) {
                this._title = value;
                this._update();
            }
        },
        enumerable: true,
        configurable: true
    });
    SegmentedBarItem.prototype._update = function () {
    };
    return SegmentedBarItem;
}(bindable.Bindable));
exports.SegmentedBarItem = SegmentedBarItem;
var SegmentedBar = (function (_super) {
    __extends(SegmentedBar, _super);
    function SegmentedBar() {
        _super.apply(this, arguments);
    }
    SegmentedBar.prototype._addArrayFromBuilder = function (name, value) {
        if (name === "items") {
            this._setValue(SegmentedBar.itemsProperty, value);
        }
    };
    SegmentedBar.prototype._adjustSelectedIndex = function (items) {
        if (this.items) {
            if (this.items.length > 0) {
                ensureTypes();
                if (types.isUndefined(this.selectedIndex) || (this.selectedIndex > this.items.length - 1)) {
                    this._setValue(SegmentedBar.selectedIndexProperty, 0);
                }
            }
            else {
                this._setValue(SegmentedBar.selectedIndexProperty, undefined);
            }
        }
        else {
            this._setValue(SegmentedBar.selectedIndexProperty, undefined);
        }
    };
    Object.defineProperty(SegmentedBar.prototype, "selectedIndex", {
        get: function () {
            return this._getValue(SegmentedBar.selectedIndexProperty);
        },
        set: function (value) {
            this._setValue(SegmentedBar.selectedIndexProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(SegmentedBar.prototype, "items", {
        get: function () {
            return this._getValue(SegmentedBar.itemsProperty);
        },
        set: function (value) {
            this._setValue(SegmentedBar.itemsProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(SegmentedBar.prototype, "selectedBackgroundColor", {
        get: function () {
            return this._getValue(SegmentedBar.selectedBackgroundColorProperty);
        },
        set: function (value) {
            this._setValue(SegmentedBar.selectedBackgroundColorProperty, value instanceof color.Color ? value : new color.Color(value));
        },
        enumerable: true,
        configurable: true
    });
    SegmentedBar.prototype._onBindingContextChanged = function (oldValue, newValue) {
        _super.prototype._onBindingContextChanged.call(this, oldValue, newValue);
        if (this.items && this.items.length > 0) {
            var i = 0;
            var length = this.items.length;
            for (; i < length; i++) {
                this.items[i].bindingContext = newValue;
            }
        }
    };
    SegmentedBar.prototype._addChildFromBuilder = function (name, value) {
        if (name === CHILD_SEGMENTED_BAR_ITEM) {
            if (!this.items) {
                this.items = new Array();
            }
            this.items.push(value);
            this.insertTab(value);
        }
    };
    SegmentedBar.prototype.insertTab = function (tabItem, index) {
    };
    SegmentedBar.prototype.getValidIndex = function (index) {
        ensureTypes();
        var idx;
        var itemsLength = this.items ? this.items.length : 0;
        if (types.isNullOrUndefined(index)) {
            idx = itemsLength - 1;
        }
        else {
            if (index < 0 || index > itemsLength) {
                idx = itemsLength - 1;
            }
            else {
                idx = index;
            }
        }
        return idx;
    };
    SegmentedBar.selectedBackgroundColorProperty = new dependencyObservable.Property("selectedBackgroundColor", "SegmentedBar", new proxy.PropertyMetadata(undefined));
    SegmentedBar.selectedIndexProperty = new dependencyObservable.Property("selectedIndex", "SegmentedBar", new proxy.PropertyMetadata(undefined));
    SegmentedBar.itemsProperty = new dependencyObservable.Property("items", "SegmentedBar", new proxy.PropertyMetadata(undefined));
    SegmentedBar.selectedIndexChangedEvent = "selectedIndexChanged";
    return SegmentedBar;
}(view.View));
exports.SegmentedBar = SegmentedBar;

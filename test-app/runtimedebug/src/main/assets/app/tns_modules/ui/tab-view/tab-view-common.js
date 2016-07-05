var view_1 = require("ui/core/view");
var dependency_observable_1 = require("ui/core/dependency-observable");
var bindable_1 = require("ui/core/bindable");
var platform_1 = require("platform");
var proxy_1 = require("ui/core/proxy");
var types = require("utils/types");
var trace = require("trace");
var color = require("color");
var AffectsLayout = platform_1.isAndroid ? dependency_observable_1.PropertyMetadataSettings.None : dependency_observable_1.PropertyMetadataSettings.AffectsLayout;
exports.traceCategory = "TabView";
var TabViewItem = (function (_super) {
    __extends(TabViewItem, _super);
    function TabViewItem() {
        _super.apply(this, arguments);
        this._title = "";
    }
    Object.defineProperty(TabViewItem.prototype, "title", {
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
    Object.defineProperty(TabViewItem.prototype, "view", {
        get: function () {
            return this._view;
        },
        set: function (value) {
            if (this._view !== value) {
                if (this._view) {
                    throw new Error("Changing the view of an already loaded TabViewItem is not currently supported.");
                }
                this._view = value;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TabViewItem.prototype, "iconSource", {
        get: function () {
            return this._iconSource;
        },
        set: function (value) {
            if (this._iconSource !== value) {
                this._iconSource = value;
                this._update();
            }
        },
        enumerable: true,
        configurable: true
    });
    TabViewItem.prototype._update = function () {
    };
    return TabViewItem;
}(bindable_1.Bindable));
exports.TabViewItem = TabViewItem;
var TAB_VIEW = "TabView";
var ITEMS = "items";
var SELECTED_INDEX = "selectedIndex";
var SELECTED_COLOR = "selectedColor";
var TABS_BACKGROUND_COLOR = "tabsBackgroundColor";
var knownCollections;
(function (knownCollections) {
    knownCollections.items = "items";
})(knownCollections = exports.knownCollections || (exports.knownCollections = {}));
var itemsProperty = new dependency_observable_1.Property(ITEMS, TAB_VIEW, new proxy_1.PropertyMetadata(undefined, AffectsLayout));
var selectedIndexProperty = new dependency_observable_1.Property(SELECTED_INDEX, TAB_VIEW, new proxy_1.PropertyMetadata(undefined, AffectsLayout));
var selectedColorProperty = new dependency_observable_1.Property(SELECTED_COLOR, TAB_VIEW, new proxy_1.PropertyMetadata(undefined));
var tabsBackgroundColorProperty = new dependency_observable_1.Property(TABS_BACKGROUND_COLOR, TAB_VIEW, new proxy_1.PropertyMetadata(undefined));
selectedIndexProperty.metadata.onSetNativeValue = function (data) {
    var tabView = data.object;
    tabView._onSelectedIndexPropertyChangedSetNativeValue(data);
};
itemsProperty.metadata.onSetNativeValue = function (data) {
    var tabView = data.object;
    tabView._onItemsPropertyChangedSetNativeValue(data);
};
var TabView = (function (_super) {
    __extends(TabView, _super);
    function TabView() {
        _super.apply(this, arguments);
    }
    TabView.prototype._addArrayFromBuilder = function (name, value) {
        if (name === ITEMS) {
            this.items = value;
        }
    };
    Object.defineProperty(TabView.prototype, "items", {
        get: function () {
            return this._getValue(TabView.itemsProperty);
        },
        set: function (value) {
            this._setValue(TabView.itemsProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    TabView.prototype._onItemsPropertyChangedSetNativeValue = function (data) {
        if (trace.enabled) {
            trace.write("TabView.__onItemsPropertyChangedSetNativeValue(" + data.oldValue + " -> " + data.newValue + ");", exports.traceCategory);
        }
        if (data.oldValue) {
            this._removeTabs(data.oldValue);
        }
        if (data.newValue) {
            this._addTabs(data.newValue);
        }
        this._updateSelectedIndexOnItemsPropertyChanged(data.newValue);
    };
    TabView.prototype._updateSelectedIndexOnItemsPropertyChanged = function (newItems) {
        if (trace.enabled) {
            trace.write("TabView._updateSelectedIndexOnItemsPropertyChanged(" + newItems + ");", exports.traceCategory);
        }
        var newItemsCount = 0;
        if (newItems) {
            newItemsCount = newItems.length;
        }
        if (newItemsCount === 0) {
            this.selectedIndex = undefined;
        }
        else if (types.isUndefined(this.selectedIndex) || this.selectedIndex >= newItemsCount) {
            this.selectedIndex = 0;
        }
    };
    TabView.prototype._removeTabs = function (oldItems) {
        var i;
        var length = oldItems.length;
        var oldItem;
        for (i = 0; i < length; i++) {
            oldItem = oldItems[i];
            if (!oldItem) {
                throw new Error("TabViewItem at index " + i + " is undefined.");
            }
            if (!oldItem.view) {
                throw new Error("TabViewItem at index " + i + " does not have a view.");
            }
            this._removeView(oldItem.view);
        }
    };
    TabView.prototype._addTabs = function (newItems) {
        var i;
        var length = newItems.length;
        var newItem;
        for (i = 0; i < length; i++) {
            newItem = newItems[i];
            if (!newItem) {
                throw new Error("TabViewItem at index " + i + " is undefined.");
            }
            if (!newItem.view) {
                throw new Error("TabViewItem at index " + i + " does not have a view.");
            }
            this._addView(newItem.view, i);
        }
    };
    Object.defineProperty(TabView.prototype, "selectedIndex", {
        get: function () {
            return this._getValue(TabView.selectedIndexProperty);
        },
        set: function (value) {
            this._setValue(TabView.selectedIndexProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TabView.prototype, "selectedColor", {
        get: function () {
            return this._getValue(TabView.selectedColorProperty);
        },
        set: function (value) {
            this._setValue(TabView.selectedColorProperty, value instanceof color.Color ? value : new color.Color(value));
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TabView.prototype, "tabsBackgroundColor", {
        get: function () {
            return this._getValue(TabView.tabsBackgroundColorProperty);
        },
        set: function (value) {
            this._setValue(TabView.tabsBackgroundColorProperty, value instanceof color.Color ? value : new color.Color(value));
        },
        enumerable: true,
        configurable: true
    });
    TabView.prototype._onSelectedIndexPropertyChangedSetNativeValue = function (data) {
        var index = this.selectedIndex;
        if (types.isUndefined(index)) {
            return;
        }
        if (types.isDefined(this.items)) {
            if (index < 0 || index >= this.items.length) {
                this.selectedIndex = undefined;
                throw new Error("SelectedIndex should be between [0, items.length)");
            }
        }
    };
    Object.defineProperty(TabView.prototype, "_selectedView", {
        get: function () {
            var _items = this.items;
            var _selectedIndex = this.selectedIndex;
            if (!_items) {
                return undefined;
            }
            if (_items.length === 0) {
                return undefined;
            }
            if (_selectedIndex === undefined) {
                return undefined;
            }
            return _items[_selectedIndex].view;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TabView.prototype, "_childrenCount", {
        get: function () {
            if (this.items) {
                return this.items.length;
            }
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    TabView.prototype._eachChildView = function (callback) {
        var _items = this.items;
        if (!_items) {
            return;
        }
        var i;
        var length = _items.length;
        var item;
        var retVal;
        for (i = 0; i < length; i++) {
            item = _items[i];
            if (item.view) {
                retVal = callback(item.view);
                if (retVal === false) {
                    break;
                }
            }
        }
    };
    TabView.prototype._onBindingContextChanged = function (oldValue, newValue) {
        _super.prototype._onBindingContextChanged.call(this, oldValue, newValue);
        if (this.items && this.items.length > 0) {
            var i = 0;
            var length = this.items.length;
            for (; i < length; i++) {
                this.items[i].bindingContext = newValue;
            }
        }
    };
    TabView.prototype._getAndroidTabView = function () {
        return undefined;
    };
    TabView.prototype._updateIOSTabBarColorsAndFonts = function () {
    };
    TabView.itemsProperty = itemsProperty;
    TabView.selectedIndexProperty = selectedIndexProperty;
    TabView.selectedColorProperty = selectedColorProperty;
    TabView.tabsBackgroundColorProperty = tabsBackgroundColorProperty;
    TabView.selectedIndexChangedEvent = "selectedIndexChanged";
    return TabView;
}(view_1.View));
exports.TabView = TabView;

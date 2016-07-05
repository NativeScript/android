var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var view = require("ui/core/view");
var types = require("utils/types");
var trace = require("trace");
exports.traceCategory = "ListPicker";
var ListPicker = (function (_super) {
    __extends(ListPicker, _super);
    function ListPicker() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(ListPicker.prototype, "selectedIndex", {
        get: function () {
            return this._getValue(ListPicker.selectedIndexProperty);
        },
        set: function (value) {
            this._setValue(ListPicker.selectedIndexProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ListPicker.prototype, "items", {
        get: function () {
            return this._getValue(ListPicker.itemsProperty);
        },
        set: function (value) {
            this._setValue(ListPicker.itemsProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    ListPicker.prototype._getItemAsString = function (index) {
        if (!this.items || !this.items.length) {
            return " ";
        }
        if (types.isDefined(this.items)) {
            var item = this.items.getItem ? this.items.getItem(index) : this.items[index];
            return types.isString(item) ? item : (types.isDefined(item) ? item.toString() : index.toString());
        }
        return index.toString();
    };
    ListPicker.prototype._onSelectedIndexPropertyChanged = function (data) {
        if (trace.enabled) {
            trace.write("ListPicker._onSelectedIndexPropertyChanged(" + data.oldValue + " => " + data.newValue + ");", exports.traceCategory);
        }
        var index = this.selectedIndex;
        if (types.isUndefined(index)) {
            return;
        }
        if (types.isDefined(this.items)) {
            if (index < 0 || index >= this.items.length) {
                this.selectedIndex = undefined;
                throw new Error("selectedIndex should be between [0, items.length - 1]");
            }
        }
    };
    ListPicker.prototype._onItemsPropertyChanged = function (data) {
    };
    ListPicker.prototype._updateSelectedIndexOnItemsPropertyChanged = function (newItems) {
        if (trace.enabled) {
            trace.write("ListPicker._updateSelectedIndexOnItemsPropertyChanged(" + newItems + ");", exports.traceCategory);
        }
        var newItemsCount = 0;
        if (newItems && newItems.length) {
            newItemsCount = newItems.length;
        }
        if (newItemsCount === 0) {
            this.selectedIndex = undefined;
        }
        else if (types.isUndefined(this.selectedIndex) || this.selectedIndex >= newItemsCount) {
            this.selectedIndex = 0;
        }
    };
    ListPicker.selectedIndexProperty = new dependencyObservable.Property("selectedIndex", "ListPicker", new proxy.PropertyMetadata(undefined));
    ListPicker.itemsProperty = new dependencyObservable.Property("items", "ListPicker", new proxy.PropertyMetadata(undefined));
    return ListPicker;
}(view.View));
exports.ListPicker = ListPicker;
function onSelectedIndexPropertyChanged(data) {
    var picker = data.object;
    picker._onSelectedIndexPropertyChanged(data);
}
function onItemsPropertyChanged(data) {
    var picker = data.object;
    picker._onItemsPropertyChanged(data);
}
ListPicker.selectedIndexProperty.metadata.onSetNativeValue = onSelectedIndexPropertyChanged;
ListPicker.itemsProperty.metadata.onSetNativeValue = onItemsPropertyChanged;

var layout_base_1 = require("ui/layouts/layout-base");
var view_1 = require("ui/core/view");
var bindable_1 = require("ui/core/bindable");
var proxy_1 = require("ui/core/proxy");
var dependency_observable_1 = require("ui/core/dependency-observable");
var special_properties_1 = require("ui/builder/special-properties");
var numberUtils = require("../../../utils/number-utils");
var types;
function ensureTypes() {
    if (!types) {
        types = require("utils/types");
    }
}
function validateArgs(element) {
    if (!element) {
        throw new Error("element cannot be null or undefinied.");
    }
    return element;
}
var GridUnitType;
(function (GridUnitType) {
    GridUnitType.auto = "auto";
    GridUnitType.pixel = "pixel";
    GridUnitType.star = "star";
})(GridUnitType = exports.GridUnitType || (exports.GridUnitType = {}));
special_properties_1.registerSpecialProperty("row", function (instance, propertyValue) {
    GridLayout.setRow(instance, !isNaN(+propertyValue) && +propertyValue);
});
special_properties_1.registerSpecialProperty("col", function (instance, propertyValue) {
    GridLayout.setColumn(instance, !isNaN(+propertyValue) && +propertyValue);
});
special_properties_1.registerSpecialProperty("colSpan", function (instance, propertyValue) {
    GridLayout.setColumnSpan(instance, !isNaN(+propertyValue) && +propertyValue);
});
special_properties_1.registerSpecialProperty("rowSpan", function (instance, propertyValue) {
    GridLayout.setRowSpan(instance, !isNaN(+propertyValue) && +propertyValue);
});
var ItemSpec = (function (_super) {
    __extends(ItemSpec, _super);
    function ItemSpec() {
        _super.call(this);
        this._actualLength = 0;
        if (arguments.length === 0) {
            this._value = 1;
            this._unitType = GridUnitType.star;
        }
        else if (arguments.length === 2) {
            ensureTypes();
            if (types.isNumber(arguments[0]) && types.isString(arguments[1])) {
                if (arguments[0] < 0 || (arguments[1] !== GridUnitType.auto && arguments[1] !== GridUnitType.star && arguments[1] !== GridUnitType.pixel)) {
                    throw new Error("Invalid values.");
                }
                this._value = arguments[0];
                this._unitType = arguments[1];
            }
            else {
                throw new Error("Arguments must be number and string.");
            }
        }
        else {
            throw new Error("ItemSpec expects 0 or 2 arguments");
        }
        this.index = -1;
    }
    ItemSpec.create = function (value, type) {
        var spec = new ItemSpec();
        spec._value = value;
        spec._unitType = type;
        return spec;
    };
    Object.defineProperty(ItemSpec.prototype, "actualLength", {
        get: function () {
            return this._actualLength;
        },
        set: function (value) {
            throw new Error("actualLength is read-only property");
        },
        enumerable: true,
        configurable: true
    });
    ItemSpec.equals = function (value1, value2) {
        return (value1.gridUnitType === value2.gridUnitType) && (value1.value === value2.value) && (value1.owner === value2.owner) && (value1.index === value2.index);
    };
    Object.defineProperty(ItemSpec.prototype, "gridUnitType", {
        get: function () {
            return this._unitType;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ItemSpec.prototype, "isAbsolute", {
        get: function () {
            return this._unitType === GridUnitType.pixel;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ItemSpec.prototype, "isAuto", {
        get: function () {
            return this._unitType === GridUnitType.auto;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ItemSpec.prototype, "isStar", {
        get: function () {
            return this._unitType === GridUnitType.star;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ItemSpec.prototype, "value", {
        get: function () {
            return this._value;
        },
        enumerable: true,
        configurable: true
    });
    return ItemSpec;
}(bindable_1.Bindable));
exports.ItemSpec = ItemSpec;
var GridLayout = (function (_super) {
    __extends(GridLayout, _super);
    function GridLayout() {
        _super.apply(this, arguments);
        this._rows = new Array();
        this._cols = new Array();
    }
    GridLayout.getColumn = function (element) {
        return validateArgs(element)._getValue(GridLayout.columnProperty);
    };
    GridLayout.setColumn = function (element, value) {
        validateArgs(element)._setValue(GridLayout.columnProperty, value);
    };
    GridLayout.getColumnSpan = function (element) {
        return validateArgs(element)._getValue(GridLayout.columnSpanProperty);
    };
    GridLayout.setColumnSpan = function (element, value) {
        validateArgs(element)._setValue(GridLayout.columnSpanProperty, value);
    };
    GridLayout.getRow = function (element) {
        return validateArgs(element)._getValue(GridLayout.rowProperty);
    };
    GridLayout.setRow = function (element, value) {
        validateArgs(element)._setValue(GridLayout.rowProperty, value);
    };
    GridLayout.getRowSpan = function (element) {
        return validateArgs(element)._getValue(GridLayout.rowSpanProperty);
    };
    GridLayout.setRowSpan = function (element, value) {
        validateArgs(element)._setValue(GridLayout.rowSpanProperty, value);
    };
    GridLayout.prototype.addRow = function (itemSpec) {
        GridLayout.validateItemSpec(itemSpec);
        itemSpec.owner = this;
        this._rows.push(itemSpec);
        this._onRowAdded(itemSpec);
        this.invalidate();
    };
    GridLayout.prototype.addColumn = function (itemSpec) {
        GridLayout.validateItemSpec(itemSpec);
        itemSpec.owner = this;
        this._cols.push(itemSpec);
        this._onColumnAdded(itemSpec);
        this.invalidate();
    };
    GridLayout.prototype.removeRow = function (itemSpec) {
        if (!itemSpec) {
            throw new Error("Value is null.");
        }
        var index = this._rows.indexOf(itemSpec);
        if (itemSpec.owner !== this || index < 0) {
            throw new Error("Row is not child of this GridLayout");
        }
        itemSpec.index = -1;
        this._rows.splice(index, 1);
        this._onRowRemoved(itemSpec, index);
        this.invalidate();
    };
    GridLayout.prototype.removeColumn = function (itemSpec) {
        if (!itemSpec) {
            throw new Error("Value is null.");
        }
        var index = this._cols.indexOf(itemSpec);
        if (itemSpec.owner !== this || index < 0) {
            throw new Error("Column is not child of this GridLayout");
        }
        itemSpec.index = -1;
        this._cols.splice(index, 1);
        this._onColumnRemoved(itemSpec, index);
        this.invalidate();
    };
    GridLayout.prototype.removeColumns = function () {
        for (var i = this._cols.length - 1; i >= 0; i--) {
            var colSpec = this._cols[i];
            this._onColumnRemoved(colSpec, i);
            colSpec.index = -1;
        }
        this._cols.length = 0;
        this.invalidate();
    };
    GridLayout.prototype.removeRows = function () {
        for (var i = this._rows.length - 1; i >= 0; i--) {
            var rowSpec = this._rows[i];
            this._onRowRemoved(rowSpec, i);
            rowSpec.index = -1;
        }
        this._rows.length = 0;
        this.invalidate();
    };
    GridLayout.prototype.onRowChanged = function (element, oldValue, newValue) {
        this.invalidate();
    };
    GridLayout.prototype.onRowSpanChanged = function (element, oldValue, newValue) {
        this.invalidate();
    };
    GridLayout.prototype.onColumnChanged = function (element, oldValue, newValue) {
        this.invalidate();
    };
    GridLayout.prototype.onColumnSpanChanged = function (element, oldValue, newValue) {
        this.invalidate();
    };
    GridLayout.prototype._onRowAdded = function (itemSpec) {
    };
    GridLayout.prototype._onColumnAdded = function (itemSpec) {
    };
    GridLayout.prototype._onRowRemoved = function (itemSpec, index) {
    };
    GridLayout.prototype._onColumnRemoved = function (itemSpec, index) {
    };
    GridLayout.prototype.getColumns = function () {
        return this._cols.slice();
    };
    GridLayout.prototype.getRows = function () {
        return this._rows.slice();
    };
    Object.defineProperty(GridLayout.prototype, "columnsInternal", {
        get: function () {
            return this._cols;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(GridLayout.prototype, "rowsInternal", {
        get: function () {
            return this._rows;
        },
        enumerable: true,
        configurable: true
    });
    GridLayout.prototype.invalidate = function () {
        this.requestLayout();
    };
    GridLayout.prototype._applyXmlAttribute = function (attributeName, attributeValue) {
        if (attributeName === "columns") {
            this._setColumns(attributeValue);
            return true;
        }
        else if (attributeName === "rows") {
            this._setRows(attributeValue);
            return true;
        }
        return _super.prototype._applyXmlAttribute.call(this, attributeName, attributeValue);
    };
    GridLayout.parseItemSpecs = function (value) {
        var result = new Array();
        var arr = value.split(/[\s,]+/);
        for (var i = 0; i < arr.length; i++) {
            var str = arr[i].trim();
            if (str.length > 0) {
                result.push(GridLayout.convertGridLength(arr[i].trim()));
            }
        }
        return result;
    };
    GridLayout.convertGridLength = function (value) {
        if (value === "auto") {
            return ItemSpec.create(1, GridUnitType.auto);
        }
        else if (value.indexOf("*") !== -1) {
            var starCount = parseInt(value.replace("*", "") || "1");
            return ItemSpec.create(starCount, GridUnitType.star);
        }
        else if (!isNaN(parseInt(value))) {
            return ItemSpec.create(parseInt(value), GridUnitType.pixel);
        }
        else {
            throw new Error("Cannot parse item spec from string: " + value);
        }
    };
    GridLayout.onRowPropertyChanged = function (data) {
        var element = GridLayout.getView(data.object);
        var grid = element.parent;
        if (grid instanceof GridLayout) {
            grid.onRowChanged(element, data.oldValue, data.newValue);
        }
    };
    GridLayout.onColumnPropertyChanged = function (data) {
        var element = GridLayout.getView(data.object);
        var grid = element.parent;
        if (grid instanceof GridLayout) {
            grid.onColumnChanged(element, data.oldValue, data.newValue);
        }
    };
    GridLayout.onRowSpanPropertyChanged = function (data) {
        var element = GridLayout.getView(data.object);
        var grid = element.parent;
        if (grid instanceof GridLayout) {
            grid.onRowSpanChanged(element, data.oldValue, data.newValue);
        }
    };
    GridLayout.onColumnSpanPropertyChanged = function (data) {
        var element = GridLayout.getView(data.object);
        var grid = element.parent;
        if (grid instanceof GridLayout) {
            grid.onColumnSpanChanged(element, data.oldValue, data.newValue);
        }
    };
    GridLayout.validateItemSpec = function (itemSpec) {
        if (!itemSpec) {
            throw new Error("Value cannot be undefined.");
        }
        if (itemSpec.owner) {
            throw new Error("itemSpec is already added to GridLayout.");
        }
    };
    GridLayout.getView = function (object) {
        if (object instanceof view_1.View) {
            return object;
        }
        throw new Error("Element is not View or its descendant.");
    };
    GridLayout.prototype._setColumns = function (value) {
        this.removeColumns();
        var columns = GridLayout.parseItemSpecs(value);
        for (var i = 0, count = columns.length; i < count; i++) {
            this.addColumn(columns[i]);
        }
    };
    GridLayout.prototype._setRows = function (value) {
        this.removeRows();
        var rows = GridLayout.parseItemSpecs(value);
        for (var i = 0, count = rows.length; i < count; i++) {
            this.addRow(rows[i]);
        }
    };
    GridLayout.columnProperty = new dependency_observable_1.Property("Column", "GridLayout", new proxy_1.PropertyMetadata(0, dependency_observable_1.PropertyMetadataSettings.None, GridLayout.onColumnPropertyChanged, numberUtils.notNegative));
    GridLayout.columnSpanProperty = new dependency_observable_1.Property("ColumnSpan", "GridLayout", new proxy_1.PropertyMetadata(1, dependency_observable_1.PropertyMetadataSettings.None, GridLayout.onColumnSpanPropertyChanged, numberUtils.greaterThanZero));
    GridLayout.rowProperty = new dependency_observable_1.Property("Row", "GridLayout", new proxy_1.PropertyMetadata(0, dependency_observable_1.PropertyMetadataSettings.None, GridLayout.onRowPropertyChanged, numberUtils.notNegative));
    GridLayout.rowSpanProperty = new dependency_observable_1.Property("RowSpan", "GridLayout", new proxy_1.PropertyMetadata(1, dependency_observable_1.PropertyMetadataSettings.None, GridLayout.onRowSpanPropertyChanged, numberUtils.greaterThanZero));
    return GridLayout;
}(layout_base_1.LayoutBase));
exports.GridLayout = GridLayout;

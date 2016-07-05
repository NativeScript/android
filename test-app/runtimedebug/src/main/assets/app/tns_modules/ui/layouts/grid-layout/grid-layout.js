var utils = require("utils/utils");
var common = require("./grid-layout-common");
var view_1 = require("ui/core/view");
global.moduleMerge(common, exports);
function setNativeProperty(data, setter) {
    var view = data.object;
    if (view instanceof view_1.View) {
        var nativeView = view._nativeView;
        var lp = nativeView.getLayoutParams() || new org.nativescript.widgets.CommonLayoutParams();
        if (lp instanceof org.nativescript.widgets.CommonLayoutParams) {
            setter(lp);
            nativeView.setLayoutParams(lp);
        }
    }
}
function setNativeRowProperty(data) {
    setNativeProperty(data, function (lp) { lp.row = data.newValue; });
}
function setNativeRowSpanProperty(data) {
    setNativeProperty(data, function (lp) { lp.rowSpan = data.newValue; });
}
function setNativeColumnProperty(data) {
    setNativeProperty(data, function (lp) { lp.column = data.newValue; });
}
function setNativeColumnSpanProperty(data) {
    setNativeProperty(data, function (lp) { lp.columnSpan = data.newValue; });
}
common.GridLayout.rowProperty.metadata.onSetNativeValue = setNativeRowProperty;
common.GridLayout.rowSpanProperty.metadata.onSetNativeValue = setNativeRowSpanProperty;
common.GridLayout.columnProperty.metadata.onSetNativeValue = setNativeColumnProperty;
common.GridLayout.columnSpanProperty.metadata.onSetNativeValue = setNativeColumnSpanProperty;
function createNativeSpec(itemSpec) {
    switch (itemSpec.gridUnitType) {
        case common.GridUnitType.auto:
            return new org.nativescript.widgets.ItemSpec(itemSpec.value, org.nativescript.widgets.GridUnitType.auto);
        case common.GridUnitType.star:
            return new org.nativescript.widgets.ItemSpec(itemSpec.value, org.nativescript.widgets.GridUnitType.star);
        case common.GridUnitType.pixel:
            return new org.nativescript.widgets.ItemSpec(itemSpec.value * utils.layout.getDisplayDensity(), org.nativescript.widgets.GridUnitType.pixel);
        default:
            throw new Error("Invalid gridUnitType: " + itemSpec.gridUnitType);
    }
}
var ItemSpec = (function (_super) {
    __extends(ItemSpec, _super);
    function ItemSpec() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(ItemSpec.prototype, "actualLength", {
        get: function () {
            if (this.nativeSpec) {
                return Math.round(this.nativeSpec.getActualLength() / utils.layout.getDisplayDensity());
            }
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    return ItemSpec;
}(common.ItemSpec));
exports.ItemSpec = ItemSpec;
var GridLayout = (function (_super) {
    __extends(GridLayout, _super);
    function GridLayout() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(GridLayout.prototype, "android", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(GridLayout.prototype, "_nativeView", {
        get: function () {
            return this._layout;
        },
        enumerable: true,
        configurable: true
    });
    GridLayout.prototype._createUI = function () {
        var _this = this;
        this._layout = new org.nativescript.widgets.GridLayout(this._context);
        this.getRows().forEach(function (itemSpec, index, rows) { _this._onRowAdded(itemSpec); }, this);
        this.getColumns().forEach(function (itemSpec, index, rows) { _this._onColumnAdded(itemSpec); }, this);
    };
    GridLayout.prototype._onRowAdded = function (itemSpec) {
        if (this._layout) {
            var nativeSpec = createNativeSpec(itemSpec);
            itemSpec.nativeSpec = nativeSpec;
            this._layout.addRow(nativeSpec);
        }
    };
    GridLayout.prototype._onColumnAdded = function (itemSpec) {
        if (this._layout) {
            var nativeSpec = createNativeSpec(itemSpec);
            itemSpec.nativeSpec = nativeSpec;
            this._layout.addColumn(nativeSpec);
        }
    };
    GridLayout.prototype._onRowRemoved = function (itemSpec, index) {
        itemSpec.nativeSpec = null;
        if (this._layout) {
            this._layout.removeRowAt(index);
        }
    };
    GridLayout.prototype._onColumnRemoved = function (itemSpec, index) {
        itemSpec.nativeSpec = null;
        if (this._layout) {
            this._layout.removeColumnAt(index);
        }
    };
    GridLayout.prototype.invalidate = function () {
    };
    return GridLayout;
}(common.GridLayout));
exports.GridLayout = GridLayout;

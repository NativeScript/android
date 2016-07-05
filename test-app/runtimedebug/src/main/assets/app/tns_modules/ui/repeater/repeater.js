var proxy = require("ui/core/proxy");
var dependencyObservable = require("ui/core/dependency-observable");
var viewModule = require("ui/core/view");
var layoutBaseModule = require("ui/layouts/layout-base");
var utils = require("utils/utils");
var trace = require("trace");
var platform = require("platform");
var stackLayout = require("ui/layouts/stack-layout");
var types = require("utils/types");
var builder = require("ui/builder");
var observableArray = require("data/observable-array");
var weakEvents = require("ui/core/weak-event-listener");
var label;
function ensureLabel() {
    if (!label) {
        label = require("ui/label");
    }
}
var ITEMS = "items";
var ITEMTEMPLATE = "itemTemplate";
var LAYOUT = "layout";
var REPEATER = "Repeater";
var knownTemplates;
(function (knownTemplates) {
    knownTemplates.itemTemplate = "itemTemplate";
})(knownTemplates = exports.knownTemplates || (exports.knownTemplates = {}));
function onItemsPropertyChanged(data) {
    var repeater = data.object;
    repeater._onItemsPropertyChanged(data);
}
function onItemTemplatePropertyChanged(data) {
    var repeater = data.object;
    repeater._onItemTemplatePropertyChanged(data);
}
function onItemsLayoutPropertyPropertyChanged(data) {
    var repeater = data.object;
    repeater._onItemsLayoutPropertyPropertyChanged(data);
}
var Repeater = (function (_super) {
    __extends(Repeater, _super);
    function Repeater() {
        _super.call(this);
        this._isDirty = false;
        if (platform.device.os === platform.platformNames.ios) {
            this._ios = UIView.new();
        }
        this.itemsLayout = new stackLayout.StackLayout();
    }
    Object.defineProperty(Repeater.prototype, "items", {
        get: function () {
            return this._getValue(Repeater.itemsProperty);
        },
        set: function (value) {
            this._setValue(Repeater.itemsProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Repeater.prototype, "itemTemplate", {
        get: function () {
            return this._getValue(Repeater.itemTemplateProperty);
        },
        set: function (value) {
            this._setValue(Repeater.itemTemplateProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Repeater.prototype, "itemsLayout", {
        get: function () {
            return this._getValue(Repeater.itemsLayoutProperty);
        },
        set: function (value) {
            this._setValue(Repeater.itemsLayoutProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Repeater.prototype.onLoaded = function () {
        if (trace.enabled) {
            trace.write("Repeater.onLoaded()", "Repeater");
        }
        if (this._isDirty) {
            this.refresh();
        }
        _super.prototype.onLoaded.call(this);
    };
    Repeater.prototype._requestRefresh = function () {
        if (trace.enabled) {
            trace.write("Repeater._requestRefresh()", "Repeater");
        }
        this._isDirty = true;
        if (this.isLoaded) {
            this.refresh();
        }
    };
    Repeater.prototype.refresh = function () {
        if (trace.enabled) {
            trace.write("Repeater.refresh()", "Repeater");
        }
        if (this.itemsLayout) {
            this.itemsLayout.removeChildren();
        }
        if (types.isNullOrUndefined(this.items) || !types.isNumber(this.items.length)) {
            return;
        }
        var length = this.items.length;
        for (var i = 0; i < length; i++) {
            var viewToAdd = !types.isNullOrUndefined(this.itemTemplate) ? builder.parse(this.itemTemplate, this) : this._getDefaultItemContent(i);
            var dataItem = this._getDataItem(i);
            viewToAdd.bindingContext = dataItem;
            this.itemsLayout.addChild(viewToAdd);
        }
        this._isDirty = false;
    };
    Repeater.prototype._onItemsPropertyChanged = function (data) {
        if (trace.enabled) {
            trace.write("Repeater._onItemsPropertyChanged(" + data.oldValue + " => " + data.newValue + ")", "Repeater");
        }
        if (data.oldValue instanceof observableArray.ObservableArray) {
            weakEvents.removeWeakEventListener(data.oldValue, observableArray.ObservableArray.changeEvent, this._onItemsChanged, this);
        }
        if (data.newValue instanceof observableArray.ObservableArray) {
            weakEvents.addWeakEventListener(data.newValue, observableArray.ObservableArray.changeEvent, this._onItemsChanged, this);
        }
        this._requestRefresh();
    };
    Repeater.prototype._onItemTemplatePropertyChanged = function (data) {
        if (trace.enabled) {
            trace.write("Repeater._onItemTemplatePropertyChanged(" + data.oldValue + " => " + data.newValue + ")", "Repeater");
        }
        this._requestRefresh();
    };
    Repeater.prototype._onItemsLayoutPropertyPropertyChanged = function (data) {
        if (trace.enabled) {
            trace.write("Repeater._onItemsLayoutPropertyPropertyChanged(" + data.oldValue + " => " + data.newValue + ")", "Repeater");
        }
        if (data.oldValue instanceof layoutBaseModule.LayoutBase) {
            this._removeView(data.oldValue);
        }
        if (data.newValue instanceof layoutBaseModule.LayoutBase) {
            this._addView(data.newValue);
        }
        this._requestRefresh();
    };
    Repeater.prototype._onItemsChanged = function (data) {
        if (trace.enabled) {
            trace.write("Repeater._onItemsChanged(" + data + ")", "Repeater");
        }
        this._requestRefresh();
    };
    Repeater.prototype._getDefaultItemContent = function (index) {
        ensureLabel();
        var lbl = new label.Label();
        lbl.bind({
            targetProperty: "text",
            sourceProperty: "$value"
        });
        return lbl;
    };
    Repeater.prototype._getDataItem = function (index) {
        return this.items.getItem ? this.items.getItem(index) : this.items[index];
    };
    Object.defineProperty(Repeater.prototype, "ios", {
        get: function () {
            return this._ios;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Repeater.prototype, "_childrenCount", {
        get: function () {
            var count = 0;
            if (this.itemsLayout) {
                count++;
            }
            return count;
        },
        enumerable: true,
        configurable: true
    });
    Repeater.prototype._eachChildView = function (callback) {
        if (this.itemsLayout) {
            callback(this.itemsLayout);
        }
    };
    Repeater.prototype.onLayout = function (left, top, right, bottom) {
        viewModule.View.layoutChild(this, this.itemsLayout, 0, 0, right - left, bottom - top);
    };
    Repeater.prototype.onMeasure = function (widthMeasureSpec, heightMeasureSpec) {
        var result = viewModule.View.measureChild(this, this.itemsLayout, widthMeasureSpec, heightMeasureSpec);
        var width = utils.layout.getMeasureSpecSize(widthMeasureSpec);
        var widthMode = utils.layout.getMeasureSpecMode(widthMeasureSpec);
        var height = utils.layout.getMeasureSpecSize(heightMeasureSpec);
        var heightMode = utils.layout.getMeasureSpecMode(heightMeasureSpec);
        var widthAndState = viewModule.View.resolveSizeAndState(result.measuredWidth, width, widthMode, 0);
        var heightAndState = viewModule.View.resolveSizeAndState(result.measuredHeight, height, heightMode, 0);
        this.setMeasuredDimension(widthAndState, heightAndState);
    };
    Repeater.itemsProperty = new dependencyObservable.Property(ITEMS, REPEATER, new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.AffectsLayout, onItemsPropertyChanged));
    Repeater.itemTemplateProperty = new dependencyObservable.Property(ITEMTEMPLATE, REPEATER, new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.AffectsLayout, onItemTemplatePropertyChanged));
    Repeater.itemsLayoutProperty = new dependencyObservable.Property(LAYOUT, REPEATER, new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.AffectsLayout, onItemsLayoutPropertyPropertyChanged));
    return Repeater;
}(viewModule.CustomLayoutView));
exports.Repeater = Repeater;

var observable = require("data/observable");
var types = require("utils/types");
var CHANGE = "change", UPDATE = "update", DELETE = "delete", ADD = "add";
var ChangeType = (function () {
    function ChangeType() {
    }
    ChangeType.Add = "add";
    ChangeType.Delete = "delete";
    ChangeType.Update = "update";
    ChangeType.Splice = "splice";
    return ChangeType;
}());
exports.ChangeType = ChangeType;
var VirtualArray = (function (_super) {
    __extends(VirtualArray, _super);
    function VirtualArray(length) {
        if (length === void 0) { length = 0; }
        _super.call(this);
        this._length = length;
        this._cache = {};
        this._requestedIndexes = [];
        this._loadedIndexes = [];
    }
    Object.defineProperty(VirtualArray.prototype, "length", {
        get: function () {
            return this._length;
        },
        set: function (value) {
            if (this._length !== value) {
                var index = this._length;
                var count = value - this._length;
                this._length = value;
                this.notify({
                    eventName: CHANGE, object: this,
                    action: count > 0 ? ADD : DELETE,
                    index: index,
                    removed: new Array(count < 0 ? Math.abs(count) : 0),
                    addedCount: count > 0 ? count : 0
                });
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(VirtualArray.prototype, "loadSize", {
        get: function () {
            return this._loadSize;
        },
        set: function (value) {
            this._loadSize = value;
        },
        enumerable: true,
        configurable: true
    });
    VirtualArray.prototype.getItem = function (index) {
        var item = this._cache[index];
        if (types.isUndefined(item)) {
            if (index >= 0 && index < this.length && this._requestedIndexes.indexOf(index) < 0 && this._loadedIndexes.indexOf(index) < 0) {
                this.requestItems(index);
            }
        }
        return item;
    };
    VirtualArray.prototype.setItem = function (index, value) {
        if (this._cache[index] !== value) {
            this.load(index, [value]);
        }
    };
    VirtualArray.prototype.load = function (index, items) {
        var i;
        for (i = 0; i < items.length; i++) {
            var itemIndex = index + i;
            this._cache[itemIndex] = items[i];
            this._requestedIndexes.splice(this._requestedIndexes.indexOf(itemIndex), 1);
            if (this._loadedIndexes.indexOf(itemIndex) < 0) {
                this._loadedIndexes.push(itemIndex);
            }
        }
        if (this._requestedIndexes.length > 0) {
            for (i = 0; i < this.loadSize - items.length; i++) {
                this._requestedIndexes.splice(this._requestedIndexes.indexOf(index + i), 1);
            }
        }
        this.notify({
            eventName: CHANGE, object: this,
            action: UPDATE,
            index: index,
            removed: new Array(items.length),
            addedCount: items.length
        });
    };
    VirtualArray.prototype.requestItems = function (index) {
        var indexesToLoad = [];
        var pageIndex = this._loadSize > 0 ? this._loadSize * Math.floor(index / this._loadSize) : index;
        var count = 0;
        var start = -1;
        for (var i = 0; i < this.loadSize; i++) {
            var itemIndex = pageIndex + i;
            if (itemIndex >= this._length) {
                break;
            }
            if (this._loadedIndexes.indexOf(itemIndex) < 0) {
                if (start < 0) {
                    start = itemIndex;
                }
                indexesToLoad.push(itemIndex);
                if (this._requestedIndexes.indexOf(itemIndex) < 0) {
                    this._requestedIndexes.push(itemIndex);
                }
                count++;
            }
            else {
                if (count > 0) {
                    this.notify({
                        eventName: VirtualArray.itemsLoadingEvent, object: this,
                        index: start,
                        count: count
                    });
                }
                start = -1;
                count = 0;
            }
        }
        if (start >= 0 && count > 0) {
            this.notify({
                eventName: VirtualArray.itemsLoadingEvent, object: this,
                index: start,
                count: count
            });
        }
    };
    VirtualArray.changeEvent = CHANGE;
    VirtualArray.itemsLoadingEvent = "itemsLoading";
    return VirtualArray;
}(observable.Observable));
exports.VirtualArray = VirtualArray;

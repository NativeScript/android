var observable = require("data/observable");
var common = require("./list-view-common");
var stackLayout = require("ui/layouts/stack-layout");
var proxy_view_container_1 = require("ui/proxy-view-container");
var layoutBase = require("ui/layouts/layout-base");
var color;
function ensureColor() {
    if (!color) {
        color = require("color");
    }
}
var ITEMLOADING = common.ListView.itemLoadingEvent;
var LOADMOREITEMS = common.ListView.loadMoreItemsEvent;
var ITEMTAP = common.ListView.itemTapEvent;
global.moduleMerge(common, exports);
function onSeparatorColorPropertyChanged(data) {
    var bar = data.object;
    if (!bar.android) {
        return;
    }
    ensureColor();
    if (data.newValue instanceof color.Color) {
        bar.android.setDivider(new android.graphics.drawable.ColorDrawable(data.newValue.android));
        bar.android.setDividerHeight(1);
    }
}
common.ListView.separatorColorProperty.metadata.onSetNativeValue = onSeparatorColorPropertyChanged;
var ListView = (function (_super) {
    __extends(ListView, _super);
    function ListView() {
        _super.apply(this, arguments);
        this._androidViewId = -1;
        this._realizedItems = new Map();
    }
    ListView.prototype._createUI = function () {
        this._android = new android.widget.ListView(this._context);
        this._android.setCacheColorHint(android.graphics.Color.TRANSPARENT);
        if (this._androidViewId < 0) {
            this._androidViewId = android.view.View.generateViewId();
        }
        this._android.setId(this._androidViewId);
        ensureListViewAdapterClass();
        this.android.setAdapter(new ListViewAdapterClass(this));
        var that = new WeakRef(this);
        this.android.setOnItemClickListener(new android.widget.AdapterView.OnItemClickListener({
            onItemClick: function (parent, convertView, index, id) {
                var owner = that.get();
                if (owner) {
                    owner.notify({ eventName: ITEMTAP, object: owner, index: index, view: owner._getRealizedView(convertView, index) });
                }
            }
        }));
    };
    Object.defineProperty(ListView.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    ListView.prototype.refresh = function () {
        if (!this._android || !this._android.getAdapter()) {
            return;
        }
        this._realizedItems.forEach(function (view, nativeView, map) {
            if (!(view.bindingContext instanceof observable.Observable)) {
                view.bindingContext = null;
            }
        });
        this.android.getAdapter().notifyDataSetChanged();
    };
    ListView.prototype.scrollToIndex = function (index) {
        if (this._android) {
            this._android.setSelection(index);
        }
    };
    ListView.prototype._onDetached = function (force) {
        _super.prototype._onDetached.call(this, force);
        this.clearRealizedCells();
    };
    Object.defineProperty(ListView.prototype, "_childrenCount", {
        get: function () {
            return this._realizedItems.size;
        },
        enumerable: true,
        configurable: true
    });
    ListView.prototype._eachChildView = function (callback) {
        this._realizedItems.forEach(function (view, nativeView, map) {
            if (view.parent instanceof ListView) {
                callback(view);
            }
            else {
                if (view.parent) {
                    callback(view.parent);
                }
            }
        });
    };
    ListView.prototype._getRealizedView = function (convertView, index) {
        if (!convertView) {
            return this._getItemTemplateContent(index);
        }
        return this._realizedItems.get(convertView);
    };
    ListView.prototype.clearRealizedCells = function () {
        var _this = this;
        this._realizedItems.forEach(function (view, nativeView, map) {
            if (view.parent) {
                if (!(view.parent instanceof ListView)) {
                    _this._removeView(view.parent);
                }
                view.parent._removeView(view);
            }
        });
        this._realizedItems.clear();
    };
    return ListView;
}(common.ListView));
exports.ListView = ListView;
var ListViewAdapterClass;
function ensureListViewAdapterClass() {
    if (ListViewAdapterClass) {
        return;
    }
    var ListViewAdapter = (function (_super) {
        __extends(ListViewAdapter, _super);
        function ListViewAdapter(listView) {
            _super.call(this);
            this._listView = listView;
            return global.__native(this);
        }
        ListViewAdapter.prototype.getCount = function () {
            return this._listView && this._listView.items ? this._listView.items.length : 0;
        };
        ListViewAdapter.prototype.getItem = function (i) {
            if (this._listView && this._listView.items && i < this._listView.items.length) {
                return this._listView.items.getItem ? this._listView.items.getItem(i) : this._listView.items[i];
            }
            return null;
        };
        ListViewAdapter.prototype.getItemId = function (i) {
            return long(i);
        };
        ListViewAdapter.prototype.hasStableIds = function () {
            return true;
        };
        ListViewAdapter.prototype.getView = function (index, convertView, parent) {
            if (!this._listView) {
                return null;
            }
            var totalItemCount = this._listView.items ? this._listView.items.length : 0;
            if (index === (totalItemCount - 1)) {
                this._listView.notify({ eventName: LOADMOREITEMS, object: this._listView });
            }
            var view = this._listView._getRealizedView(convertView, index);
            var args = {
                eventName: ITEMLOADING, object: this._listView, index: index, view: view,
                android: parent,
                ios: undefined
            };
            this._listView.notify(args);
            if (!args.view) {
                args.view = this._listView._getDefaultItemContent(index);
            }
            if (args.view) {
                if (this._listView.rowHeight > -1) {
                    args.view.height = this._listView.rowHeight;
                }
                else {
                    args.view.height = Number.NaN;
                }
                this._listView._prepareItem(args.view, index);
                if (!args.view.parent) {
                    if (args.view instanceof layoutBase.LayoutBase &&
                        !(args.view instanceof proxy_view_container_1.ProxyViewContainer)) {
                        this._listView._addView(args.view);
                        convertView = args.view.android;
                    }
                    else {
                        var sp = new stackLayout.StackLayout();
                        sp.addChild(args.view);
                        this._listView._addView(sp);
                        convertView = sp.android;
                    }
                }
                this._listView._realizedItems.set(convertView, args.view);
            }
            return convertView;
        };
        return ListViewAdapter;
    }(android.widget.BaseAdapter));
    ListViewAdapterClass = ListViewAdapter;
}

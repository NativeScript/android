var common = require("./tab-view-common");
var trace = require("trace");
var types = require("utils/types");
var utils = require("utils/utils");
var color = require("color");
var style = require("ui/styling/style");
var imageSource;
function ensureImageSource() {
    if (!imageSource) {
        imageSource = require("image-source");
    }
}
var VIEWS_STATES = "_viewStates";
var ACCENT_COLOR = "colorAccent";
var PRIMARY_COLOR = "colorPrimary";
var DEFAULT_ELEVATION = 4;
global.moduleMerge(common, exports);
var TabViewItem = (function (_super) {
    __extends(TabViewItem, _super);
    function TabViewItem() {
        _super.apply(this, arguments);
    }
    TabViewItem.prototype._update = function () {
        if (this._parent) {
            this._parent._updateTabForItem(this);
        }
    };
    return TabViewItem;
}(common.TabViewItem));
exports.TabViewItem = TabViewItem;
var PagerAdapterClass;
function ensurePagerAdapterClass() {
    if (PagerAdapterClass) {
        return;
    }
    var PagerAdapterClassInner = (function (_super) {
        __extends(PagerAdapterClassInner, _super);
        function PagerAdapterClassInner(owner, items) {
            _super.call(this);
            this.owner = owner;
            this.items = items;
            return global.__native(this);
        }
        PagerAdapterClassInner.prototype.getCount = function () {
            return this.items ? this.items.length : 0;
        };
        PagerAdapterClassInner.prototype.getPageTitle = function (index) {
            if (index < 0 || index >= this.items.length) {
                return "";
            }
            return this.items[index].title;
        };
        PagerAdapterClassInner.prototype.instantiateItem = function (container, index) {
            if (trace.enabled) {
                trace.write("TabView.PagerAdapter.instantiateItem; container: " + container + "; index: " + index, common.traceCategory);
            }
            var item = this.items[index];
            if (item.view.parent !== this.owner) {
                this.owner._addView(item.view);
            }
            if (this[VIEWS_STATES]) {
                if (trace.enabled) {
                    trace.write("TabView.PagerAdapter.instantiateItem; restoreHierarchyState: " + item.view, common.traceCategory);
                }
                item.view._nativeView.restoreHierarchyState(this[VIEWS_STATES]);
            }
            container.addView(item.view._nativeView);
            return item.view._nativeView;
        };
        PagerAdapterClassInner.prototype.destroyItem = function (container, index, _object) {
            if (trace.enabled) {
                trace.write("TabView.PagerAdapter.destroyItem; container: " + container + "; index: " + index + "; _object: " + _object, common.traceCategory);
            }
            var item = this.items[index];
            var nativeView = item.view._nativeView;
            if (nativeView.toString() !== _object.toString()) {
                throw new Error("Expected " + nativeView.toString() + " to equal " + _object.toString());
            }
            container.removeView(nativeView);
            if (item.view.parent === this.owner) {
                this.owner._removeView(item.view);
            }
        };
        PagerAdapterClassInner.prototype.isViewFromObject = function (view, _object) {
            return view === _object;
        };
        PagerAdapterClassInner.prototype.saveState = function () {
            if (trace.enabled) {
                trace.write("TabView.PagerAdapter.saveState", common.traceCategory);
            }
            var owner = this.owner;
            if (!owner || owner._childrenCount === 0) {
                return null;
            }
            if (!this[VIEWS_STATES]) {
                this[VIEWS_STATES] = new android.util.SparseArray();
            }
            var viewStates = this[VIEWS_STATES];
            var childCallback = function (view) {
                var nativeView = view._nativeView;
                if (nativeView && nativeView.isSaveFromParentEnabled && nativeView.isSaveFromParentEnabled()) {
                    nativeView.saveHierarchyState(viewStates);
                }
                return true;
            };
            owner._eachChildView(childCallback);
            var bundle = new android.os.Bundle();
            bundle.putSparseParcelableArray(VIEWS_STATES, viewStates);
            return bundle;
        };
        PagerAdapterClassInner.prototype.restoreState = function (state, loader) {
            if (trace.enabled) {
                trace.write("TabView.PagerAdapter.restoreState", common.traceCategory);
            }
            var bundle = state;
            bundle.setClassLoader(loader);
            this[VIEWS_STATES] = bundle.getSparseParcelableArray(VIEWS_STATES);
        };
        return PagerAdapterClassInner;
    }(android.support.v4.view.PagerAdapter));
    ;
    PagerAdapterClass = PagerAdapterClassInner;
}
var PageChangedListenerClass;
function ensurePageChangedListenerClass() {
    if (PageChangedListenerClass) {
        return;
    }
    var PageChangedListener = (function (_super) {
        __extends(PageChangedListener, _super);
        function PageChangedListener(owner) {
            _super.call(this);
            this._owner = owner;
            return global.__native(this);
        }
        PageChangedListener.prototype.onPageSelected = function (position) {
            this._owner.selectedIndex = position;
        };
        return PageChangedListener;
    }(android.support.v4.view.ViewPager.SimpleOnPageChangeListener));
    PageChangedListenerClass = PageChangedListener;
}
function selectedColorPropertyChanged(data) {
    var tabLayout = data.object._getAndroidTabView();
    if (tabLayout && data.newValue instanceof color.Color) {
        tabLayout.setSelectedIndicatorColors([data.newValue.android]);
    }
}
common.TabView.selectedColorProperty.metadata.onSetNativeValue = selectedColorPropertyChanged;
function tabsBackgroundColorPropertyChanged(data) {
    var tabLayout = data.object._getAndroidTabView();
    if (tabLayout && data.newValue instanceof color.Color) {
        tabLayout.setBackgroundColor(data.newValue.android);
    }
}
common.TabView.tabsBackgroundColorProperty.metadata.onSetNativeValue = tabsBackgroundColorPropertyChanged;
var TabView = (function (_super) {
    __extends(TabView, _super);
    function TabView() {
        _super.apply(this, arguments);
        this._androidViewId = -1;
    }
    Object.defineProperty(TabView.prototype, "android", {
        get: function () {
            return this._grid;
        },
        enumerable: true,
        configurable: true
    });
    TabView.prototype._createUI = function () {
        if (trace.enabled) {
            trace.write("TabView._createUI(" + this + ");", common.traceCategory);
        }
        this._grid = new org.nativescript.widgets.GridLayout(this._context);
        this._grid.addRow(new org.nativescript.widgets.ItemSpec(1, org.nativescript.widgets.GridUnitType.auto));
        this._grid.addRow(new org.nativescript.widgets.ItemSpec(1, org.nativescript.widgets.GridUnitType.star));
        this._tabLayout = new org.nativescript.widgets.TabLayout(this._context);
        this._grid.addView(this._tabLayout);
        this.setElevation();
        var accentColor = utils.ad.resources.getPalleteColor(ACCENT_COLOR, this._context);
        if (accentColor) {
            this._tabLayout.setSelectedIndicatorColors([accentColor]);
        }
        var primaryColor = utils.ad.resources.getPalleteColor(PRIMARY_COLOR, this._context);
        if (primaryColor) {
            this._tabLayout.setBackgroundColor(primaryColor);
        }
        this._viewPager = new android.support.v4.view.ViewPager(this._context);
        var lp = new org.nativescript.widgets.CommonLayoutParams();
        lp.row = 1;
        this._viewPager.setLayoutParams(lp);
        this._grid.addView(this._viewPager);
        if (this._androidViewId < 0) {
            this._androidViewId = android.view.View.generateViewId();
        }
        this._grid.setId(this._androidViewId);
        ensurePageChangedListenerClass();
        this._pageChagedListener = new PageChangedListenerClass(this);
        this._viewPager.addOnPageChangeListener(this._pageChagedListener);
    };
    TabView.prototype.setElevation = function () {
        var compat = android.support.v4.view.ViewCompat;
        if (compat.setElevation) {
            var val = DEFAULT_ELEVATION * utils.layout.getDisplayDensity();
            compat.setElevation(this._grid, val);
            compat.setElevation(this._tabLayout, val);
        }
    };
    TabView.prototype._onItemsPropertyChangedSetNativeValue = function (data) {
        var _this = this;
        if (trace.enabled) {
            trace.write("TabView._onItemsPropertyChangedSetNativeValue(" + data.oldValue + " ---> " + data.newValue + ");", common.traceCategory);
        }
        if (data.oldValue) {
            var oldItems = data.oldValue;
            oldItems.forEach(function (oldItem) {
                oldItem._parent = null;
            });
            this._viewPager.setAdapter(null);
            this._pagerAdapter = null;
            this._tabLayout.setItems(null, null);
        }
        if (data.newValue) {
            var items = data.newValue;
            var tabItems = new Array();
            items.forEach(function (item, idx, arr) {
                if (types.isNullOrUndefined(item.view)) {
                    throw new Error("View of TabViewItem at index " + idx + " is " + item.view);
                }
                item._parent = _this;
                if (item.view.parent !== _this) {
                    _this._addView(item.view, idx);
                }
                tabItems.push(_this.createTabItem(item));
            });
            ensurePagerAdapterClass();
            this._pagerAdapter = new PagerAdapterClass(this, data.newValue);
            this._viewPager.setAdapter(this._pagerAdapter);
            this._tabLayout.setItems(tabItems, this._viewPager);
        }
        this._updateSelectedIndexOnItemsPropertyChanged(data.newValue);
    };
    TabView.prototype._updateTabForItem = function (item) {
        if (this.items && this.items.length > 0) {
            var index = this.items.indexOf(item);
            if (index >= 0) {
                this._tabLayout.updateItemAt(index, this.createTabItem(item));
            }
        }
    };
    TabView.prototype._onSelectedIndexPropertyChangedSetNativeValue = function (data) {
        if (trace.enabled) {
            trace.write("TabView._onSelectedIndexPropertyChangedSetNativeValue(" + data.oldValue + " ---> " + data.newValue + ");", common.traceCategory);
        }
        _super.prototype._onSelectedIndexPropertyChangedSetNativeValue.call(this, data);
        var index = data.newValue;
        if (!types.isNullOrUndefined(index)) {
            var viewPagerSelectedIndex = this._viewPager.getCurrentItem();
            if (viewPagerSelectedIndex !== index) {
                if (trace.enabled) {
                    trace.write("TabView this._viewPager.setCurrentItem(" + index + ", true);", common.traceCategory);
                }
                this._viewPager.setCurrentItem(index, true);
            }
        }
        var args = { eventName: TabView.selectedIndexChangedEvent, object: this, oldIndex: data.oldValue, newIndex: data.newValue };
        this.notify(args);
    };
    TabView.prototype.createTabItem = function (item) {
        var result = new org.nativescript.widgets.TabItemSpec();
        result.title = item.title;
        if (item.iconSource) {
            if (item.iconSource.indexOf(utils.RESOURCE_PREFIX) === 0) {
                result.iconId = utils.ad.resources.getDrawableId(item.iconSource.substr(utils.RESOURCE_PREFIX.length));
            }
            else {
                ensureImageSource();
                var is = imageSource.fromFileOrResource(item.iconSource);
                if (is) {
                    result.iconDrawable = new android.graphics.drawable.BitmapDrawable(is.android);
                }
            }
        }
        return result;
    };
    TabView.prototype._getAndroidTabView = function () {
        return this._tabLayout;
    };
    return TabView;
}(common.TabView));
exports.TabView = TabView;
var TabViewStyler = (function () {
    function TabViewStyler() {
    }
    TabViewStyler.setColorProperty = function (v, newValue) {
        var tab = v;
        if (tab.items && tab.items.length > 0) {
            var tabLayout = tab._getAndroidTabView();
            for (var i = 0; i < tab.items.length; i++) {
                tabLayout.getTextViewForItemAt(i).setTextColor(newValue);
            }
        }
    };
    TabViewStyler.resetColorProperty = function (v, nativeValue) {
        if (types.isNullOrUndefined(nativeValue)) {
            return;
        }
        var tab = v;
        if (tab.items && tab.items.length > 0) {
            var tabLayout = tab._getAndroidTabView();
            for (var i = 0; i < tab.items.length; i++) {
                tabLayout.getTextViewForItemAt(i).setTextColor(nativeValue);
            }
        }
    };
    TabViewStyler.getColorProperty = function (v) {
        var tab = v;
        var tv = tab._getAndroidTabView().getTextViewForItemAt(0);
        if (tv) {
            return tv.getTextColors().getDefaultColor();
        }
        else {
            return null;
        }
    };
    TabViewStyler.setFontInternalProperty = function (v, newValue, nativeValue) {
        var tab = v;
        var fontValue = newValue;
        var typeface = fontValue.getAndroidTypeface();
        if (tab.items && tab.items.length > 0) {
            var tabLayout = tab._getAndroidTabView();
            for (var i = 0; i < tab.items.length; i++) {
                var tv = tabLayout.getTextViewForItemAt(i);
                if (typeface) {
                    tv.setTypeface(typeface);
                }
                else {
                    tv.setTypeface(nativeValue.typeface);
                }
                if (fontValue.fontSize) {
                    tv.setTextSize(fontValue.fontSize);
                }
                else {
                    tv.setTextSize(android.util.TypedValue.COMPLEX_UNIT_PX, nativeValue.size);
                }
            }
        }
    };
    TabViewStyler.resetFontInternalProperty = function (v, nativeValue) {
        var tab = v;
        if (tab.items && tab.items.length > 0) {
            var tabLayout = tab._getAndroidTabView();
            for (var i = 0; i < tab.items.length; i++) {
                var tv = tabLayout.getTextViewForItemAt(i);
                tv.setTypeface(nativeValue.typeface);
                tv.setTextSize(android.util.TypedValue.COMPLEX_UNIT_PX, nativeValue.size);
            }
        }
    };
    TabViewStyler.getNativeFontInternalValue = function (v) {
        var tab = v;
        var tv = tab._getAndroidTabView().getTextViewForItemAt(0);
        if (tv) {
            return {
                typeface: tv.getTypeface(),
                size: tv.getTextSize()
            };
        }
        else {
            return null;
        }
    };
    TabViewStyler.registerHandlers = function () {
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(TabViewStyler.setColorProperty, TabViewStyler.resetColorProperty, TabViewStyler.getColorProperty), "TabView");
        style.registerHandler(style.fontInternalProperty, new style.StylePropertyChangedHandler(TabViewStyler.setFontInternalProperty, TabViewStyler.resetFontInternalProperty, TabViewStyler.getNativeFontInternalValue), "TabView");
    };
    return TabViewStyler;
}());
exports.TabViewStyler = TabViewStyler;
TabViewStyler.registerHandlers();

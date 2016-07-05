var common = require("./segmented-bar-common");
var types = require("utils/types");
var style = require("ui/styling/style");
var R_ID_TABS = 0x01020013;
var R_ID_TABCONTENT = 0x01020011;
var R_ATTR_STATE_SELECTED = 0x010100a1;
global.moduleMerge(common, exports);
function onSelectedIndexPropertyChanged(data) {
    var view = data.object;
    if (!view.android || !view.items) {
        return;
    }
    var index = data.newValue;
    if (types.isNumber(index)) {
        if (index >= 0 && index <= view.items.length - 1) {
            view.android.setCurrentTab(index);
            view.notify({ eventName: SegmentedBar.selectedIndexChangedEvent, object: view, oldIndex: data.oldValue, newIndex: data.newValue });
        }
        else {
            view.selectedIndex = undefined;
            throw new Error("selectedIndex should be between [0, " + (view.items.length - 1) + "]");
        }
    }
}
common.SegmentedBar.selectedIndexProperty.metadata.onSetNativeValue = onSelectedIndexPropertyChanged;
function onItemsPropertyChanged(data) {
    var view = data.object;
    if (!view.android) {
        return;
    }
    var oldItems = data.oldValue;
    if (oldItems && oldItems.length) {
        for (var i = 0; i < oldItems.length; i++) {
            oldItems[i]._parent = null;
        }
    }
    view.android.clearAllTabs();
    var newItems = data.newValue;
    view._adjustSelectedIndex(newItems);
    if (newItems && newItems.length) {
        for (var i = 0; i < newItems.length; i++) {
            view.insertTab(newItems[i], i);
        }
        if (types.isNumber(view.selectedIndex) && view.android.getCurrentTab() !== view.selectedIndex) {
            view.android.setCurrentTab(view.selectedIndex);
        }
        var tabHost = view.android;
        var tabIndex;
        if (view.selectedBackgroundColor) {
            ensureSegmentedBarColorDrawableClass();
            for (tabIndex = 0; tabIndex < tabHost.getTabWidget().getTabCount(); tabIndex++) {
                var vg = tabHost.getTabWidget().getChildTabViewAt(tabIndex);
                var stateDrawable = new android.graphics.drawable.StateListDrawable();
                var arr = Array.create("int", 1);
                arr[0] = R_ATTR_STATE_SELECTED;
                var colorDrawable = new SegmentedBarColorDrawableClass(view.selectedBackgroundColor.android);
                stateDrawable.addState(arr, colorDrawable);
                stateDrawable.setBounds(0, 15, vg.getRight(), vg.getBottom());
                vg.setBackgroundDrawable(stateDrawable);
            }
        }
        for (tabIndex = 0; tabIndex < tabHost.getTabWidget().getTabCount(); tabIndex++) {
            var tabChild = tabHost.getTabWidget().getChildTabViewAt(tabIndex);
            var t = tabChild.getChildAt(1);
            if (view.color) {
                t.setTextColor(view.color.android);
            }
            t.setMaxLines(1);
            t.setEllipsize(android.text.TextUtils.TruncateAt.END);
        }
    }
}
common.SegmentedBar.itemsProperty.metadata.onSetNativeValue = onItemsPropertyChanged;
var SegmentedBarColorDrawableClass;
function ensureSegmentedBarColorDrawableClass() {
    if (SegmentedBarColorDrawableClass) {
        return;
    }
    var SegmentedBarColorDrawable = (function (_super) {
        __extends(SegmentedBarColorDrawable, _super);
        function SegmentedBarColorDrawable(arg) {
            _super.call(this, arg);
            return global.__native(this);
        }
        SegmentedBarColorDrawable.prototype.draw = function (canvas) {
            var p = new android.graphics.Paint();
            p.setColor(this.getColor());
            p.setStyle(android.graphics.Paint.Style.FILL);
            canvas.drawRect(0, this.getBounds().height() - 15, this.getBounds().width(), this.getBounds().height(), p);
        };
        return SegmentedBarColorDrawable;
    }(android.graphics.drawable.ColorDrawable));
    SegmentedBarColorDrawableClass = SegmentedBarColorDrawable;
}
var SegmentedBarItem = (function (_super) {
    __extends(SegmentedBarItem, _super);
    function SegmentedBarItem() {
        _super.apply(this, arguments);
    }
    SegmentedBarItem.prototype._update = function () {
        if (this._parent && this._parent.android) {
            var tabIndex = this._parent.items.indexOf(this);
            var titleTextViewId = 16908310;
            var titleTextView = this._parent.android.getTabWidget().getChildAt(tabIndex).findViewById(titleTextViewId);
            titleTextView.setText(this.title || "");
        }
    };
    return SegmentedBarItem;
}(common.SegmentedBarItem));
exports.SegmentedBarItem = SegmentedBarItem;
var SegmentedBar = (function (_super) {
    __extends(SegmentedBar, _super);
    function SegmentedBar() {
        _super.apply(this, arguments);
    }
    SegmentedBar.prototype._createUI = function () {
        ensureTabHostClass();
        this._android = new TabHostClass(this._context, null);
        if (types.isNumber(this.selectedIndex) && this._android.getCurrentTab() !== this.selectedIndex) {
            this._android.setCurrentTab(this.selectedIndex);
        }
        var that = new WeakRef(this);
        this._listener = new android.widget.TabHost.OnTabChangeListener({
            onTabChanged: function (id) {
                var bar = that.get();
                if (bar) {
                    bar.selectedIndex = parseInt(id);
                }
            }
        });
        var tabHostLayout = new android.widget.LinearLayout(this._context);
        tabHostLayout.setOrientation(android.widget.LinearLayout.VERTICAL);
        var tabWidget = new android.widget.TabWidget(this._context);
        tabWidget.setId(R_ID_TABS);
        tabHostLayout.addView(tabWidget);
        var frame = new android.widget.FrameLayout(this._context);
        frame.setId(R_ID_TABCONTENT);
        frame.setVisibility(android.view.View.GONE);
        tabHostLayout.addView(frame);
        this._android.addView(tabHostLayout);
        this._android.setup();
    };
    Object.defineProperty(SegmentedBar.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    SegmentedBar.prototype.insertTab = function (tabItem, index) {
        _super.prototype.insertTab.call(this, tabItem, index);
        tabItem._parent = this;
        var tab = this.android.newTabSpec(this.getValidIndex(index) + "");
        tab.setIndicator(tabItem.title || "");
        var that = this;
        tab.setContent(new android.widget.TabHost.TabContentFactory({
            createTabContent: function (tag) {
                var tv = new android.widget.TextView(that._context);
                tv.setVisibility(android.view.View.GONE);
                return tv;
            }
        }));
        this.android.addTab(tab);
        this.resetNativeListener();
    };
    SegmentedBar.prototype.resetNativeListener = function () {
        this.android.setOnTabChangedListener(null);
        this.android.setOnTabChangedListener(this._listener);
    };
    return SegmentedBar;
}(common.SegmentedBar));
exports.SegmentedBar = SegmentedBar;
var TabHostClass;
function ensureTabHostClass() {
    if (TabHostClass) {
        return;
    }
    var OurTabHost = (function (_super) {
        __extends(OurTabHost, _super);
        function OurTabHost(context, attrs) {
            _super.call(this, context, attrs);
            return global.__native(this);
        }
        OurTabHost.prototype.onAttachedToWindow = function () {
        };
        return OurTabHost;
    }(android.widget.TabHost));
    TabHostClass = OurTabHost;
}
var SegmentedBarStyler = (function () {
    function SegmentedBarStyler() {
    }
    SegmentedBarStyler.setColorProperty = function (v, newValue) {
        var tabHost = v._nativeView;
        for (var tabIndex = 0; tabIndex < tabHost.getTabWidget().getTabCount(); tabIndex++) {
            var tab = tabHost.getTabWidget().getChildTabViewAt(tabIndex);
            var t = tab.getChildAt(1);
            t.setTextColor(newValue);
        }
    };
    SegmentedBarStyler.resetColorProperty = function (v, nativeValue) {
        var tabHost = v._nativeView;
        for (var tabIndex = 0; tabIndex < tabHost.getTabWidget().getTabCount(); tabIndex++) {
            var tab = tabHost.getTabWidget().getChildTabViewAt(tabIndex);
            var t = tab.getChildAt(1);
            t.setTextColor(nativeValue);
        }
    };
    SegmentedBarStyler.getColorProperty = function (v) {
        var tabHost = v._nativeView;
        var textView = new android.widget.TextView(tabHost.getContext());
        return textView.getCurrentTextColor();
    };
    SegmentedBarStyler.setFontInternalProperty = function (v, newValue, nativeValue) {
        var tabHost = v._nativeView;
        var fontValue = newValue;
        for (var tabIndex = 0; tabIndex < tabHost.getTabWidget().getTabCount(); tabIndex++) {
            var tab = tabHost.getTabWidget().getChildTabViewAt(tabIndex);
            var t = tab.getChildAt(1);
            var typeface = fontValue.getAndroidTypeface();
            if (typeface) {
                t.setTypeface(typeface);
            }
            else {
                t.setTypeface(nativeValue.typeface);
            }
            if (fontValue.fontSize) {
                t.setTextSize(fontValue.fontSize);
            }
            else {
                t.setTextSize(android.util.TypedValue.COMPLEX_UNIT_PX, nativeValue.size);
            }
        }
    };
    SegmentedBarStyler.resetFontInternalProperty = function (v, nativeValue) {
        var tabHost = v._nativeView;
        for (var tabIndex = 0; tabIndex < tabHost.getTabWidget().getTabCount(); tabIndex++) {
            var tab = tabHost.getTabWidget().getChildTabViewAt(tabIndex);
            var t = tab.getChildAt(1);
            t.setTypeface(nativeValue.typeface);
            t.setTextSize(nativeValue.size);
        }
    };
    SegmentedBarStyler.getFontInternalProperty = function (v) {
        var tabHost = v._nativeView;
        var textView = new android.widget.TextView(tabHost.getContext());
        return {
            typeface: textView.getTypeface(),
            size: textView.getTextSize()
        };
    };
    SegmentedBarStyler.registerHandlers = function () {
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(SegmentedBarStyler.setColorProperty, SegmentedBarStyler.resetColorProperty, SegmentedBarStyler.getColorProperty), "SegmentedBar");
        style.registerHandler(style.fontInternalProperty, new style.StylePropertyChangedHandler(SegmentedBarStyler.setFontInternalProperty, SegmentedBarStyler.resetFontInternalProperty, SegmentedBarStyler.getFontInternalProperty), "SegmentedBar");
    };
    return SegmentedBarStyler;
}());
exports.SegmentedBarStyler = SegmentedBarStyler;
SegmentedBarStyler.registerHandlers();

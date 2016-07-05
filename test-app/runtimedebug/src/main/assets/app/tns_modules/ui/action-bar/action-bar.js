var common = require("./action-bar-common");
var frame = require("ui/frame");
var types = require("utils/types");
var enums = require("ui/enums");
var application = require("application");
var style = require("ui/styling/style");
var R_ID_HOME = 0x0102002c;
var ACTION_ITEM_ID_OFFSET = 1000;
global.moduleMerge(common, exports);
var trace;
function ensureTrace() {
    if (!trace) {
        trace = require("trace");
    }
}
var utils;
function ensureUtils() {
    if (!utils) {
        utils = require("utils/utils");
    }
}
var imageSource;
function ensureImageSource() {
    if (!imageSource) {
        imageSource = require("image-source");
    }
}
var actionItemIdGenerator = ACTION_ITEM_ID_OFFSET;
function generateItemId() {
    actionItemIdGenerator++;
    return actionItemIdGenerator;
}
var ActionItem = (function (_super) {
    __extends(ActionItem, _super);
    function ActionItem() {
        _super.call(this);
        this._androidPosition = {
            position: enums.AndroidActionItemPosition.actionBar,
            systemIcon: undefined
        };
        this._itemId = generateItemId();
    }
    Object.defineProperty(ActionItem.prototype, "android", {
        get: function () {
            return this._androidPosition;
        },
        set: function (value) {
            throw new Error("ActionItem.android is read-only");
        },
        enumerable: true,
        configurable: true
    });
    ActionItem.prototype._getItemId = function () {
        return this._itemId;
    };
    return ActionItem;
}(common.ActionItem));
exports.ActionItem = ActionItem;
var AndroidActionBarSettings = (function () {
    function AndroidActionBarSettings(actionBar) {
        this._iconVisibility = enums.AndroidActionBarIconVisibility.auto;
        this._actionBar = actionBar;
    }
    Object.defineProperty(AndroidActionBarSettings.prototype, "icon", {
        get: function () {
            return this._icon;
        },
        set: function (value) {
            if (value !== this._icon) {
                this._icon = value;
                this._actionBar._onIconPropertyChanged();
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AndroidActionBarSettings.prototype, "iconVisibility", {
        get: function () {
            return this._iconVisibility;
        },
        set: function (value) {
            if (value !== this._iconVisibility) {
                this._iconVisibility = value;
                this._actionBar._onIconPropertyChanged();
            }
        },
        enumerable: true,
        configurable: true
    });
    return AndroidActionBarSettings;
}());
exports.AndroidActionBarSettings = AndroidActionBarSettings;
var NavigationButton = (function (_super) {
    __extends(NavigationButton, _super);
    function NavigationButton() {
        _super.apply(this, arguments);
    }
    return NavigationButton;
}(ActionItem));
exports.NavigationButton = NavigationButton;
var ActionBar = (function (_super) {
    __extends(ActionBar, _super);
    function ActionBar() {
        _super.call(this);
        this._appResources = application.android.context.getResources();
        this._android = new AndroidActionBarSettings(this);
    }
    Object.defineProperty(ActionBar.prototype, "android", {
        get: function () {
            return this._android;
        },
        set: function (value) {
            throw new Error("ActionBar.android is read-only");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ActionBar.prototype, "_nativeView", {
        get: function () {
            return this._toolbar;
        },
        enumerable: true,
        configurable: true
    });
    ActionBar.prototype._createUI = function () {
        this._toolbar = new android.support.v7.widget.Toolbar(this._context);
        var owner = this;
        this._toolbar.setOnMenuItemClickListener(new android.support.v7.widget.Toolbar.OnMenuItemClickListener({
            onMenuItemClick: function (item) {
                var itemId = item.getItemId();
                return owner._onAndroidItemSelected(itemId);
            }
        }));
    };
    ActionBar.prototype.onLoaded = function () {
        _super.prototype.onLoaded.call(this);
        this.update();
    };
    ActionBar.prototype.update = function () {
        if (!this._toolbar) {
            return;
        }
        if (!this.page.frame || !this.page.frame._getNavBarVisible(this.page)) {
            this._toolbar.setVisibility(android.view.View.GONE);
            return;
        }
        this._toolbar.setVisibility(android.view.View.VISIBLE);
        this._addActionItems();
        this._updateTitleAndTitleView();
        this._updateIcon();
        this._updateNavigationButton();
    };
    ActionBar.prototype._onAndroidItemSelected = function (itemId) {
        if (this.navigationButton && itemId === R_ID_HOME) {
            this.navigationButton._raiseTap();
            return true;
        }
        var menuItem = undefined;
        var items = this.actionItems.getItems();
        for (var i = 0; i < items.length; i++) {
            if (items[i]._getItemId() === itemId) {
                menuItem = items[i];
                break;
            }
        }
        if (menuItem) {
            menuItem._raiseTap();
            return true;
        }
        return false;
    };
    ActionBar.prototype._updateNavigationButton = function () {
        var navButton = this.navigationButton;
        if (navButton && common.isVisible(navButton)) {
            if (navButton.android.systemIcon) {
                var systemResourceId = getSystemResourceId(navButton.android.systemIcon);
                if (systemResourceId) {
                    this._toolbar.setNavigationIcon(systemResourceId);
                }
            }
            else if (navButton.icon) {
                var drawableOrId = getDrawableOrResourceId(navButton.icon, this._appResources);
                this._toolbar.setNavigationIcon(drawableOrId);
            }
            this._toolbar.setNavigationOnClickListener(new android.view.View.OnClickListener({
                onClick: function (v) {
                    if (navButton) {
                        navButton._raiseTap();
                    }
                }
            }));
        }
        else {
            this._toolbar.setNavigationIcon(null);
        }
    };
    ActionBar.prototype._updateIcon = function () {
        var visibility = getIconVisibility(this.android.iconVisibility);
        if (visibility) {
            var icon = this.android.icon;
            if (types.isDefined(icon)) {
                var drawableOrId = getDrawableOrResourceId(icon, this._appResources);
                if (drawableOrId) {
                    this._toolbar.setLogo(drawableOrId);
                }
            }
            else {
                var defaultIcon = application.android.nativeApp.getApplicationInfo().icon;
                this._toolbar.setLogo(defaultIcon);
            }
        }
        else {
            this._toolbar.setLogo(null);
        }
    };
    ActionBar.prototype._updateTitleAndTitleView = function () {
        if (!this.titleView) {
            var title = this.title;
            if (types.isDefined(title)) {
                this._toolbar.setTitle(title);
            }
            else {
                var appContext = application.android.context;
                var appInfo = appContext.getApplicationInfo();
                var appLabel = appContext.getPackageManager().getApplicationLabel(appInfo);
                if (appLabel) {
                    this._toolbar.setTitle(appLabel);
                }
            }
        }
    };
    ActionBar.prototype._addActionItems = function () {
        var menu = this._toolbar.getMenu();
        var items = this.actionItems.getVisibleItems();
        menu.clear();
        for (var i = 0; i < items.length; i++) {
            var item = items[i];
            var menuItem = menu.add(android.view.Menu.NONE, item._getItemId(), android.view.Menu.NONE, item.text + "");
            if (item.actionView && item.actionView.android) {
                item.android.position = enums.AndroidActionItemPosition.actionBar;
                menuItem.setActionView(item.actionView.android);
                ActionBar._setOnClickListener(item);
            }
            else if (item.android.systemIcon) {
                var systemResourceId = getSystemResourceId(item.android.systemIcon);
                if (systemResourceId) {
                    menuItem.setIcon(systemResourceId);
                }
            }
            else if (item.icon) {
                var drawableOrId = getDrawableOrResourceId(item.icon, this._appResources);
                if (drawableOrId) {
                    menuItem.setIcon(drawableOrId);
                }
                else {
                    throw new Error("Error loading icon from " + item.icon);
                }
            }
            var showAsAction = getShowAsAction(item);
            menuItem.setShowAsAction(showAsAction);
        }
    };
    ActionBar._setOnClickListener = function (item) {
        item.actionView.android.setOnClickListener(new android.view.View.OnClickListener({
            onClick: function (v) {
                item._raiseTap();
            }
        }));
    };
    ActionBar.prototype._onTitlePropertyChanged = function () {
        var topFrame = frame.topmost();
        if (this._toolbar && topFrame && topFrame.currentPage === this.page) {
            this._updateTitleAndTitleView();
        }
    };
    ActionBar.prototype._onIconPropertyChanged = function () {
        var topFrame = frame.topmost();
        if (this._toolbar && topFrame && topFrame.currentPage === this.page) {
            this._updateIcon();
        }
    };
    ActionBar.prototype._clearAndroidReference = function () {
        this._toolbar = undefined;
    };
    ActionBar.prototype._addViewToNativeVisualTree = function (child, atIndex) {
        _super.prototype._addViewToNativeVisualTree.call(this, child);
        if (this._toolbar && child._nativeView) {
            if (types.isNullOrUndefined(atIndex) || atIndex >= this._nativeView.getChildCount()) {
                this._toolbar.addView(child._nativeView);
            }
            else {
                this._toolbar.addView(child._nativeView, atIndex);
            }
            return true;
        }
        return false;
    };
    ActionBar.prototype._removeViewFromNativeVisualTree = function (child) {
        _super.prototype._removeViewFromNativeVisualTree.call(this, child);
        if (this._toolbar && child._nativeView) {
            this._toolbar.removeView(child._nativeView);
            ensureTrace();
            trace.notifyEvent(child, "childInLayoutRemovedFromNativeVisualTree");
        }
    };
    return ActionBar;
}(common.ActionBar));
exports.ActionBar = ActionBar;
function getDrawableOrResourceId(icon, resources) {
    if (!types.isString(icon)) {
        return undefined;
    }
    ensureUtils();
    if (icon.indexOf(utils.RESOURCE_PREFIX) === 0) {
        var resourceId = resources.getIdentifier(icon.substr(utils.RESOURCE_PREFIX.length), 'drawable', application.android.packageName);
        if (resourceId > 0) {
            return resourceId;
        }
    }
    else {
        var drawable;
        ensureImageSource();
        var is = imageSource.fromFileOrResource(icon);
        if (is) {
            drawable = new android.graphics.drawable.BitmapDrawable(is.android);
        }
        return drawable;
    }
    return undefined;
}
function getShowAsAction(menuItem) {
    switch (menuItem.android.position) {
        case enums.AndroidActionItemPosition.actionBarIfRoom:
            return android.view.MenuItem.SHOW_AS_ACTION_IF_ROOM;
        case enums.AndroidActionItemPosition.popup:
            return android.view.MenuItem.SHOW_AS_ACTION_NEVER;
        case enums.AndroidActionItemPosition.actionBar:
        default:
            return android.view.MenuItem.SHOW_AS_ACTION_ALWAYS;
    }
}
function getIconVisibility(iconVisibility) {
    switch (iconVisibility) {
        case enums.AndroidActionBarIconVisibility.always:
            return true;
        case enums.AndroidActionBarIconVisibility.auto:
        case enums.AndroidActionBarIconVisibility.never:
        default:
            return false;
    }
}
function getSystemResourceId(systemIcon) {
    return android.content.res.Resources.getSystem().getIdentifier(systemIcon, "drawable", "android");
}
var ActionBarStyler = (function () {
    function ActionBarStyler() {
    }
    ActionBarStyler.setColorProperty = function (v, newValue) {
        var toolbar = v._nativeView;
        toolbar.setTitleTextColor(newValue);
    };
    ActionBarStyler.resetColorProperty = function (v, nativeValue) {
        if (types.isNullOrUndefined(nativeValue)) {
            nativeValue = android.graphics.Color.BLACK;
        }
        v._nativeView.setTitleTextColor(nativeValue);
    };
    ActionBarStyler.registerHandlers = function () {
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(ActionBarStyler.setColorProperty, ActionBarStyler.resetColorProperty), "ActionBar");
    };
    return ActionBarStyler;
}());
exports.ActionBarStyler = ActionBarStyler;
ActionBarStyler.registerHandlers();

var view_1 = require("ui/core/view");
var page_1 = require("ui/page");
var types_1 = require("utils/types");
var trace = require("trace");
var file_name_resolver_1 = require("file-system/file-name-resolver");
var fs = require("file-system");
var platform = require("platform");
var builder;
function ensureBuilder() {
    if (!builder) {
        builder = require("ui/builder");
    }
}
var frameStack = [];
function buildEntryFromArgs(arg) {
    var entry;
    if (arg instanceof page_1.Page) {
        throw new Error("Navigating to a Page instance is no longer supported. Please navigate by using either a module name or a page factory function.");
    }
    else if (types_1.isString(arg)) {
        entry = {
            moduleName: arg
        };
    }
    else if (types_1.isFunction(arg)) {
        entry = {
            create: arg
        };
    }
    else {
        entry = arg;
    }
    return entry;
}
function reloadPage() {
    var frame = exports.topmost();
    if (frame) {
        if (frame.currentPage && frame.currentPage.modal) {
            frame.currentPage.modal.closeModal();
        }
        var currentEntry = frame._currentEntry.entry;
        var newEntry = {
            animated: false,
            clearHistory: true,
            context: currentEntry.context,
            create: currentEntry.create,
            moduleName: currentEntry.moduleName,
            backstackVisible: currentEntry.backstackVisible
        };
        frame.navigate(newEntry);
    }
}
exports.reloadPage = reloadPage;
function resolvePageFromEntry(entry) {
    var page;
    if (entry.create) {
        page = entry.create();
        if (!(page && page instanceof page_1.Page)) {
            throw new Error("Failed to create Page with entry.create() function.");
        }
    }
    else if (entry.moduleName) {
        var currentAppPath = fs.knownFolders.currentApp().path;
        var moduleNamePath = fs.path.join(currentAppPath, entry.moduleName);
        var moduleExports;
        if (global.moduleExists(entry.moduleName)) {
            if (trace.enabled) {
                trace.write("Loading pre-registered JS module: " + entry.moduleName, trace.categories.Navigation);
            }
            moduleExports = global.loadModule(entry.moduleName);
        }
        else {
            var moduleExportsResolvedPath = file_name_resolver_1.resolveFileName(moduleNamePath, "js");
            if (moduleExportsResolvedPath) {
                if (trace.enabled) {
                    trace.write("Loading JS file: " + moduleExportsResolvedPath, trace.categories.Navigation);
                }
                moduleExportsResolvedPath = moduleExportsResolvedPath.substr(0, moduleExportsResolvedPath.length - 3);
                moduleExports = global.loadModule(moduleExportsResolvedPath);
            }
        }
        if (moduleExports && moduleExports.createPage) {
            if (trace.enabled) {
                trace.write("Calling createPage()", trace.categories.Navigation);
            }
            page = moduleExports.createPage();
        }
        else {
            page = pageFromBuilder(moduleNamePath, moduleExports);
        }
        if (!(page && page instanceof page_1.Page)) {
            throw new Error("Failed to load Page from entry.moduleName: " + entry.moduleName);
        }
        var cssFileName = file_name_resolver_1.resolveFileName(moduleNamePath, "css");
        if (cssFileName && !page["cssFile"]) {
            page.addCssFile(cssFileName);
        }
    }
    return page;
}
exports.resolvePageFromEntry = resolvePageFromEntry;
function pageFromBuilder(moduleNamePath, moduleExports) {
    var page;
    var element;
    var fileName = file_name_resolver_1.resolveFileName(moduleNamePath, "xml");
    if (fileName) {
        if (trace.enabled) {
            trace.write("Loading XML file: " + fileName, trace.categories.Navigation);
        }
        ensureBuilder();
        element = builder.load(fileName, moduleExports);
        if (element instanceof page_1.Page) {
            page = element;
        }
    }
    return page;
}
var Frame = (function (_super) {
    __extends(Frame, _super);
    function Frame() {
        _super.call(this);
        this._isInFrameStack = false;
        this._backStack = new Array();
        this._navigationQueue = new Array();
    }
    Frame.prototype.canGoBack = function () {
        return this._backStack.length > 0;
    };
    Frame.prototype.goBack = function (backstackEntry) {
        if (trace.enabled) {
            trace.write("GO BACK", trace.categories.Navigation);
        }
        if (!this.canGoBack()) {
            return;
        }
        if (!backstackEntry) {
            backstackEntry = this._backStack.pop();
        }
        else {
            var backIndex = this._backStack.indexOf(backstackEntry);
            if (backIndex < 0) {
                return;
            }
            this._backStack.splice(backIndex);
        }
        var navigationContext = {
            entry: backstackEntry,
            isBackNavigation: true
        };
        this._navigationQueue.push(navigationContext);
        if (this._navigationQueue.length === 1) {
            this._processNavigationContext(navigationContext);
        }
        else {
            if (trace.enabled) {
                trace.write("Going back scheduled", trace.categories.Navigation);
            }
        }
    };
    Frame.prototype.navigate = function (param) {
        if (trace.enabled) {
            trace.write("NAVIGATE", trace.categories.Navigation);
        }
        var entry = buildEntryFromArgs(param);
        var page = resolvePageFromEntry(entry);
        this._pushInFrameStack();
        var backstackEntry = {
            entry: entry,
            resolvedPage: page,
            navDepth: undefined,
            fragmentTag: undefined,
            isBack: undefined,
            isNavigation: true
        };
        var navigationContext = {
            entry: backstackEntry,
            isBackNavigation: false
        };
        this._navigationQueue.push(navigationContext);
        if (this._navigationQueue.length === 1) {
            this._processNavigationContext(navigationContext);
        }
        else {
            if (trace.enabled) {
                trace.write("Navigation scheduled", trace.categories.Navigation);
            }
        }
    };
    Frame.prototype._processNavigationQueue = function (page) {
        if (this._navigationQueue.length === 0) {
            return;
        }
        var entry = this._navigationQueue[0].entry;
        var currentNavigationPage = entry.resolvedPage;
        if (page !== currentNavigationPage) {
            throw new Error("Corrupted navigation stack; page: " + page + "; currentNavigationPage: " + currentNavigationPage);
        }
        this._navigationQueue.shift();
        if (this._navigationQueue.length > 0) {
            var navigationContext = this._navigationQueue[0];
            this._processNavigationContext(navigationContext);
        }
        this._updateActionBar();
    };
    Frame.prototype.navigationQueueIsEmpty = function () {
        return this._navigationQueue.length === 0;
    };
    Frame._isEntryBackstackVisible = function (entry) {
        if (!entry) {
            return false;
        }
        var backstackVisibleValue = entry.entry.backstackVisible;
        var backstackHidden = types_1.isDefined(backstackVisibleValue) && !backstackVisibleValue;
        return !backstackHidden;
    };
    Frame.prototype._updateActionBar = function (page) {
    };
    Frame.prototype._processNavigationContext = function (navigationContext) {
        if (navigationContext.isBackNavigation) {
            this.performGoBack(navigationContext);
        }
        else {
            this.performNavigation(navigationContext);
        }
    };
    Frame.prototype.performNavigation = function (navigationContext) {
        var navContext = navigationContext.entry;
        if (navigationContext.entry.entry.clearHistory) {
            this._backStack.length = 0;
        }
        else if (Frame._isEntryBackstackVisible(this._currentEntry)) {
            this._backStack.push(this._currentEntry);
        }
        this._onNavigatingTo(navContext, navigationContext.isBackNavigation);
        this._navigateCore(navContext);
    };
    Frame.prototype.performGoBack = function (navigationContext) {
        var navContext = navigationContext.entry;
        this._onNavigatingTo(navContext, navigationContext.isBackNavigation);
        this._goBackCore(navContext);
    };
    Frame.prototype._goBackCore = function (backstackEntry) {
        if (trace.enabled) {
            trace.write("GO BACK CORE(" + this._backstackEntryTrace(backstackEntry) + "); currentPage: " + this.currentPage, trace.categories.Navigation);
        }
    };
    Frame.prototype._navigateCore = function (backstackEntry) {
        if (trace.enabled) {
            trace.write("NAVIGATE CORE(" + this._backstackEntryTrace(backstackEntry) + "); currentPage: " + this.currentPage, trace.categories.Navigation);
        }
    };
    Frame.prototype._onNavigatingTo = function (backstackEntry, isBack) {
        if (this.currentPage) {
            this.currentPage.onNavigatingFrom(isBack);
        }
        backstackEntry.resolvedPage.onNavigatingTo(backstackEntry.entry.context, isBack);
    };
    Object.defineProperty(Frame.prototype, "animated", {
        get: function () {
            return this._animated;
        },
        set: function (value) {
            this._animated = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame.prototype, "transition", {
        get: function () {
            return this._transition;
        },
        set: function (value) {
            this._transition = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame.prototype, "backStack", {
        get: function () {
            return this._backStack.slice();
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame.prototype, "currentPage", {
        get: function () {
            if (this._currentEntry) {
                return this._currentEntry.resolvedPage;
            }
            return null;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame.prototype, "currentEntry", {
        get: function () {
            if (this._currentEntry) {
                return this._currentEntry.entry;
            }
            return null;
        },
        enumerable: true,
        configurable: true
    });
    Frame.prototype._pushInFrameStack = function () {
        if (this._isInFrameStack) {
            return;
        }
        frameStack.push(this);
        this._isInFrameStack = true;
    };
    Frame.prototype._popFromFrameStack = function () {
        if (!this._isInFrameStack) {
            return;
        }
        var top = _topmost();
        if (top !== this) {
            throw new Error("Cannot pop a Frame which is not at the top of the navigation stack.");
        }
        frameStack.pop();
        this._isInFrameStack = false;
    };
    Object.defineProperty(Frame.prototype, "_childrenCount", {
        get: function () {
            if (this.currentPage) {
                return 1;
            }
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    Frame.prototype._eachChildView = function (callback) {
        if (this.currentPage) {
            callback(this.currentPage);
        }
    };
    Frame.prototype._getIsAnimatedNavigation = function (entry) {
        if (entry && types_1.isDefined(entry.animated)) {
            return entry.animated;
        }
        if (types_1.isDefined(this.animated)) {
            return this.animated;
        }
        return Frame.defaultAnimatedNavigation;
    };
    Frame.prototype._getNavigationTransition = function (entry) {
        if (entry) {
            if (platform.device.os === platform.platformNames.ios && types_1.isDefined(entry.transitioniOS)) {
                return entry.transitioniOS;
            }
            if (platform.device.os === platform.platformNames.android && types_1.isDefined(entry.transitionAndroid)) {
                return entry.transitionAndroid;
            }
            if (types_1.isDefined(entry.transition)) {
                return entry.transition;
            }
        }
        if (types_1.isDefined(this.transition)) {
            return this.transition;
        }
        return Frame.defaultTransition;
    };
    Object.defineProperty(Frame.prototype, "navigationBarHeight", {
        get: function () {
            return 0;
        },
        enumerable: true,
        configurable: true
    });
    Frame.prototype._getNavBarVisible = function (page) {
        throw new Error();
    };
    Frame.prototype._addViewToNativeVisualTree = function (child) {
        return true;
    };
    Frame.prototype._removeViewFromNativeVisualTree = function (child) {
        child._isAddedToNativeVisualTree = false;
    };
    Frame.prototype._printFrameBackStack = function () {
        var length = this.backStack.length;
        var i = length - 1;
        console.log("Frame Back Stack: ");
        while (i >= 0) {
            var backstackEntry = this.backStack[i--];
            console.log("\t" + backstackEntry.resolvedPage);
        }
    };
    Frame.prototype._backstackEntryTrace = function (b) {
        var result = "" + b.resolvedPage;
        var backstackVisible = Frame._isEntryBackstackVisible(b);
        if (!backstackVisible) {
            result += " | INVISIBLE";
        }
        if (b.entry.clearHistory) {
            result += " | CLEAR HISTORY";
        }
        var animated = this._getIsAnimatedNavigation(b.entry);
        if (!animated) {
            result += " | NOT ANIMATED";
        }
        var t = this._getNavigationTransition(b.entry);
        if (t) {
            result += " | Transition[" + JSON.stringify(t) + "]";
        }
        return result;
    };
    Frame.androidOptionSelectedEvent = "optionSelected";
    Frame.defaultAnimatedNavigation = true;
    return Frame;
}(view_1.CustomLayoutView));
exports.Frame = Frame;
var _topmost = function () {
    if (frameStack.length > 0) {
        return frameStack[frameStack.length - 1];
    }
    return undefined;
};
exports.topmost = _topmost;
function goBack() {
    var top = _topmost();
    if (top.canGoBack()) {
        top.goBack();
        return true;
    }
    if (frameStack.length > 1) {
        top._popFromFrameStack();
    }
    return false;
}
exports.goBack = goBack;
function stack() {
    return frameStack;
}
exports.stack = stack;

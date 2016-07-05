require("globals");
var observable = require("data/observable");
var frame = require("ui/frame");
var builder;
function ensureBuilder() {
    if (!builder) {
        builder = require("ui/builder");
    }
}
var styleScope = undefined;
var events = new observable.Observable();
global.moduleMerge(events, exports);
exports.launchEvent = "launch";
exports.suspendEvent = "suspend";
exports.resumeEvent = "resume";
exports.exitEvent = "exit";
exports.lowMemoryEvent = "lowMemory";
exports.uncaughtErrorEvent = "uncaughtError";
exports.orientationChangedEvent = "orientationChanged";
exports.cssFile = "app.css";
exports.appSelectors = [];
exports.additionalSelectors = [];
exports.cssSelectors = [];
exports.cssSelectorVersion = 0;
exports.keyframes = {};
exports.resources = {};
exports.onUncaughtError = undefined;
exports.onLaunch = undefined;
exports.onSuspend = undefined;
exports.onResume = undefined;
exports.onExit = undefined;
exports.onLowMemory = undefined;
exports.android = undefined;
exports.ios = undefined;
function loadCss(cssFile) {
    if (!cssFile) {
        return undefined;
    }
    var result;
    var fs = require("file-system");
    if (!styleScope) {
        styleScope = require("ui/styling/style-scope");
    }
    var cssFileName = fs.path.join(fs.knownFolders.currentApp().path, cssFile);
    if (fs.File.exists(cssFileName)) {
        var file = fs.File.fromPath(cssFileName);
        var applicationCss = file.readTextSync();
        if (applicationCss) {
            result = parseCss(applicationCss, cssFileName);
        }
    }
    return result;
}
exports.loadCss = loadCss;
function mergeCssSelectors(module) {
    module.cssSelectors = module.appSelectors.slice();
    module.cssSelectors.push.apply(module.cssSelectors, module.additionalSelectors);
    module.cssSelectorVersion++;
}
exports.mergeCssSelectors = mergeCssSelectors;
function parseCss(cssText, cssFileName) {
    if (!styleScope) {
        styleScope = require("ui/styling/style-scope");
    }
    return styleScope.StyleScope.createSelectorsFromCss(cssText, cssFileName, exports.keyframes);
}
exports.parseCss = parseCss;
function __onLiveSync() {
    if (global.errorPage) {
        global.errorPage.closeModal();
        global.errorPage = undefined;
    }
    try {
        var fileResolver = require("file-system/file-name-resolver");
        fileResolver.clearCache();
        loadCss();
        frame.reloadPage();
    }
    catch (ex) {
        ensureBuilder();
        global.errorPage = builder.parse("<Page><ScrollView><Label text=\"" + ex + "\" textWrap=\"true\" style=\"color: red;\" /></ScrollView></Page>");
        global.errorPage.showModal();
    }
}
exports.__onLiveSync = __onLiveSync;

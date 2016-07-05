var view = require("ui/core/view");
var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var utils = require("utils/utils");
var trace = require("trace");
var fs;
function ensureFS() {
    if (!fs) {
        fs = require("file-system");
    }
}
var urlProperty = new dependencyObservable.Property("url", "WebView", new proxy.PropertyMetadata(""));
function onUrlPropertyChanged(data) {
    var webView = data.object;
    if (webView._suspendLoading) {
        return;
    }
    webView._loadUrl(data.newValue);
}
urlProperty.metadata.onSetNativeValue = onUrlPropertyChanged;
var srcProperty = new dependencyObservable.Property("src", "WebView", new proxy.PropertyMetadata(""));
function onSrcPropertyChanged(data) {
    var webView = data.object;
    if (webView._suspendLoading) {
        return;
    }
    webView.stopLoading();
    var src = data.newValue;
    if (trace.enabled) {
        trace.write("WebView._loadSrc(" + src + ")", trace.categories.Debug);
    }
    if (utils.isFileOrResourcePath(src)) {
        ensureFS();
        if (src.indexOf("~/") === 0) {
            src = fs.path.join(fs.knownFolders.currentApp().path, src.replace("~/", ""));
        }
        if (fs.File.exists(src)) {
            var file = fs.File.fromPath(src);
            var content = file.readTextSync();
            webView._loadFileOrResource(src, content);
        }
    }
    else if (src.toLowerCase().indexOf("http://") === 0 || src.toLowerCase().indexOf("https://") === 0) {
        webView._loadHttp(src);
    }
    else {
        webView._loadData(src);
    }
}
srcProperty.metadata.onSetNativeValue = onSrcPropertyChanged;
var WebView = (function (_super) {
    __extends(WebView, _super);
    function WebView() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(WebView.prototype, "url", {
        get: function () {
            return this._getValue(WebView.urlProperty);
        },
        set: function (value) {
            this._setValue(WebView.urlProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(WebView.prototype, "src", {
        get: function () {
            return this._getValue(WebView.srcProperty);
        },
        set: function (value) {
            this._setValue(WebView.srcProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    WebView.prototype._onLoadFinished = function (url, error) {
        this._suspendLoading = true;
        this.url = url;
        this._suspendLoading = false;
        var args = {
            eventName: WebView.loadFinishedEvent,
            object: this,
            url: url,
            navigationType: undefined,
            error: error
        };
        this.notify(args);
    };
    WebView.prototype._onLoadStarted = function (url, navigationType) {
        var args = {
            eventName: WebView.loadStartedEvent,
            object: this,
            url: url,
            navigationType: navigationType,
            error: undefined
        };
        this.notify(args);
    };
    Object.defineProperty(WebView.prototype, "canGoBack", {
        get: function () {
            throw new Error("This member is abstract.");
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(WebView.prototype, "canGoForward", {
        get: function () {
            throw new Error("This member is abstract.");
        },
        enumerable: true,
        configurable: true
    });
    WebView.loadStartedEvent = "loadStarted";
    WebView.loadFinishedEvent = "loadFinished";
    WebView.navigationTypes = [
        "linkClicked",
        "formSubmitted",
        "backForward",
        "reload",
        "formResubmitted",
        "other"
    ];
    WebView.urlProperty = urlProperty;
    WebView.srcProperty = srcProperty;
    return WebView;
}(view.View));
exports.WebView = WebView;

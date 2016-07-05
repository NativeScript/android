var file_system_1 = require("file-system");
exports.debug = true;
var applicationRootPath;
function ensureAppRootPath() {
    if (!applicationRootPath) {
        applicationRootPath = file_system_1.knownFolders.currentApp().path;
        applicationRootPath = applicationRootPath.substr(0, applicationRootPath.length - "app/".length);
    }
}
var Source = (function () {
    function Source(uri, line, column) {
        ensureAppRootPath();
        if (uri.length > applicationRootPath.length && uri.substr(0, applicationRootPath.length) === applicationRootPath) {
            this._uri = "file://" + uri.substr(applicationRootPath.length);
        }
        else {
            this._uri = uri;
        }
        this._line = line;
        this._column = column;
    }
    Object.defineProperty(Source.prototype, "uri", {
        get: function () { return this._uri; },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Source.prototype, "line", {
        get: function () { return this._line; },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Source.prototype, "column", {
        get: function () { return this._column; },
        enumerable: true,
        configurable: true
    });
    Source.prototype.toString = function () {
        return this._uri + ":" + this._line + ":" + this._column;
    };
    Source.get = function (object) {
        return object[Source._source];
    };
    Source.set = function (object, src) {
        object[Source._source] = src;
    };
    Source._source = Symbol("source");
    return Source;
}());
exports.Source = Source;
var ScopeError = (function (_super) {
    __extends(ScopeError, _super);
    function ScopeError(child, message) {
        _super.call(this, message);
        if (!child) {
            throw new Error("Required child error!");
        }
        this._child = child;
        this._message = message;
    }
    Object.defineProperty(ScopeError.prototype, "child", {
        get: function () { return this._child; },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScopeError.prototype, "message", {
        get: function () {
            if (this._message && this._childMessage) {
                return this._message + "\n \u21B3" + this._childMessage.replace("\n", "\n  ");
            }
            return this._message || this._childMessage || undefined;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScopeError.prototype, "name", {
        get: function () { return this.child.name; },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScopeError.prototype, "stack", {
        get: function () { return this.child.stack; },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(ScopeError.prototype, "_childMessage", {
        get: function () {
            return this.child.message;
        },
        enumerable: true,
        configurable: true
    });
    ScopeError.prototype.toString = function () { return "Error: " + this.message; };
    return ScopeError;
}(Error));
exports.ScopeError = ScopeError;
var SourceError = (function (_super) {
    __extends(SourceError, _super);
    function SourceError(child, source, message) {
        _super.call(this, child, message ? message + " @" + source + "" : source + "");
        this._source = source;
    }
    Object.defineProperty(SourceError.prototype, "source", {
        get: function () { return this._source; },
        enumerable: true,
        configurable: true
    });
    return SourceError;
}(ScopeError));
exports.SourceError = SourceError;

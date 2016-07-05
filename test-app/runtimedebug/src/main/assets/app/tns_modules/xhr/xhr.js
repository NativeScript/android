var http = require("http");
var types = require("utils/types");
var XMLHttpRequestResponseType;
(function (XMLHttpRequestResponseType) {
    XMLHttpRequestResponseType.empty = "";
    XMLHttpRequestResponseType.text = "text";
    XMLHttpRequestResponseType.json = "json";
})(XMLHttpRequestResponseType || (XMLHttpRequestResponseType = {}));
var XMLHttpRequest = (function () {
    function XMLHttpRequest() {
        this.UNSENT = 0;
        this.OPENED = 1;
        this.HEADERS_RECEIVED = 2;
        this.LOADING = 3;
        this.DONE = 4;
        this._responseType = "";
        this._listeners = new Map();
        this._readyState = this.UNSENT;
    }
    XMLHttpRequest.prototype.open = function (method, url, async, user, password) {
        if (types.isString(method) && types.isString(url)) {
            this._options = { url: url, method: method };
            this._options.headers = {};
            if (types.isString(user)) {
                this._options.headers["user"] = user;
            }
            if (types.isString(password)) {
                this._options.headers["password"] = password;
            }
            this._setReadyState(this.OPENED);
        }
    };
    XMLHttpRequest.prototype.abort = function () {
        this._errorFlag = true;
        this._response = null;
        this._responseTextReader = null;
        this._headers = null;
        this._status = null;
        if (this._readyState === this.UNSENT || this._readyState === this.OPENED || this._readyState === this.DONE) {
            this._readyState = this.UNSENT;
        }
        else {
            this._setReadyState(this.DONE);
        }
    };
    XMLHttpRequest.prototype.send = function (data) {
        var _this = this;
        this._errorFlag = false;
        this._response = null;
        this._responseTextReader = null;
        this._headers = null;
        this._status = null;
        if (types.isDefined(this._options)) {
            if (types.isString(data) && this._options.method !== 'GET') {
                this._options.content = data;
            }
            else if (data instanceof FormData) {
                this._options.content = data.toString();
            }
            http.request(this._options).then(function (r) {
                if (!_this._errorFlag) {
                    _this._loadResponse(r);
                }
            }).catch(function (e) {
                _this._errorFlag = true;
                _this._setReadyState(_this.DONE, e);
            });
        }
    };
    XMLHttpRequest.prototype._loadResponse = function (r) {
        this._status = r.statusCode;
        this._response = r.content.raw;
        this._headers = r.headers;
        this._setReadyState(this.HEADERS_RECEIVED);
        this._setReadyState(this.LOADING);
        this._setResponseType();
        if (this.responseType === XMLHttpRequestResponseType.json) {
            this._prepareJsonResponse(r);
        }
        else if (this.responseType === XMLHttpRequestResponseType.empty ||
            this.responseType === XMLHttpRequestResponseType.text) {
            this._responseTextReader = function () { return r.content.toString(); };
        }
        this._setReadyState(this.DONE);
    };
    XMLHttpRequest.prototype._prepareJsonResponse = function (r) {
        var _this = this;
        this._responseTextReader = function () { return r.content.toString(); };
        this._response = JSON.parse(this.responseText);
        Object.defineProperty(this._response, "toString", {
            configurable: true,
            enumerable: false,
            writable: true,
            value: function () { return _this.responseText; }
        });
    };
    XMLHttpRequest.prototype._setResponseType = function () {
        var header = this.getResponseHeader('Content-Type');
        var contentType = header && header.toLowerCase();
        if (contentType) {
            if (contentType.indexOf('application/json') >= 0) {
                this.responseType = XMLHttpRequestResponseType.json;
            }
            else if (contentType.indexOf('text/plain') >= 0) {
                this.responseType = XMLHttpRequestResponseType.text;
            }
        }
        else {
            this.responseType = XMLHttpRequestResponseType.text;
        }
    };
    XMLHttpRequest.prototype.addEventListener = function (eventName, handler) {
        if (eventName !== 'load' && eventName !== 'error') {
            throw new Error('Event not supported: ' + eventName);
        }
        var handlers = this._listeners.get(eventName) || [];
        handlers.push(handler);
        this._listeners.set(eventName, handlers);
    };
    XMLHttpRequest.prototype.removeEventListener = function (eventName, toDetach) {
        var handlers = this._listeners.get(eventName) || [];
        handlers = handlers.filter(function (handler) { return handler !== toDetach; });
        this._listeners.set(eventName, handlers);
    };
    XMLHttpRequest.prototype.emitEvent = function (eventName) {
        var args = [];
        for (var _i = 1; _i < arguments.length; _i++) {
            args[_i - 1] = arguments[_i];
        }
        var handlers = this._listeners.get(eventName) || [];
        handlers.forEach(function (handler) {
            handler.apply(void 0, args);
        });
    };
    XMLHttpRequest.prototype.setRequestHeader = function (header, value) {
        if (types.isDefined(this._options) && types.isString(header) && types.isString(value)) {
            this._options.headers[header] = value;
        }
    };
    XMLHttpRequest.prototype.getAllResponseHeaders = function () {
        if (this._readyState < 2 || this._errorFlag) {
            return "";
        }
        var result = "";
        for (var i in this._headers) {
            if (i !== "set-cookie" && i !== "set-cookie2") {
                result += i + ": " + this._headers[i] + "\r\n";
            }
        }
        return result.substr(0, result.length - 2);
    };
    XMLHttpRequest.prototype.getResponseHeader = function (header) {
        if (types.isString(header) && this._readyState > 1
            && this._headers
            && !this._errorFlag) {
            header = header.toLowerCase();
            for (var i in this._headers) {
                if (i.toLowerCase() === header) {
                    return this._headers[i];
                }
            }
        }
        return null;
    };
    XMLHttpRequest.prototype.overrideMimeType = function (mime) {
    };
    Object.defineProperty(XMLHttpRequest.prototype, "readyState", {
        get: function () {
            return this._readyState;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(XMLHttpRequest.prototype, "responseType", {
        get: function () {
            return this._responseType;
        },
        set: function (value) {
            if (value === XMLHttpRequestResponseType.empty || value in XMLHttpRequestResponseType) {
                this._responseType = value;
            }
            else {
                throw new Error("Response type of '" + value + "' not supported.");
            }
        },
        enumerable: true,
        configurable: true
    });
    XMLHttpRequest.prototype._setReadyState = function (value, error) {
        if (this._readyState !== value) {
            this._readyState = value;
            if (types.isFunction(this.onreadystatechange)) {
                this.onreadystatechange();
            }
        }
        if (this._readyState === this.DONE) {
            if (this._errorFlag) {
                if (types.isFunction(this.onerror)) {
                    this.onerror(error);
                }
                this.emitEvent('error', error);
            }
            else {
                if (types.isFunction(this.onload)) {
                    this.onload();
                }
                this.emitEvent('load');
            }
        }
    };
    Object.defineProperty(XMLHttpRequest.prototype, "responseText", {
        get: function () {
            if (types.isFunction(this._responseTextReader)) {
                return this._responseTextReader();
            }
            return "";
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(XMLHttpRequest.prototype, "response", {
        get: function () {
            return this._response;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(XMLHttpRequest.prototype, "status", {
        get: function () {
            return this._status;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(XMLHttpRequest.prototype, "statusText", {
        get: function () {
            if (this._readyState === this.UNSENT || this._readyState === this.OPENED || this._errorFlag) {
                return "";
            }
            return statuses[this._status];
        },
        enumerable: true,
        configurable: true
    });
    return XMLHttpRequest;
}());
exports.XMLHttpRequest = XMLHttpRequest;
var statuses = {
    100: "Continue",
    101: "Switching Protocols",
    200: "OK",
    201: "Created",
    202: "Accepted",
    203: "Non - Authoritative Information",
    204: "No Content",
    205: "Reset Content",
    206: "Partial Content",
    300: "Multiple Choices",
    301: "Moved Permanently",
    302: "Found",
    303: "See Other",
    304: "Not Modified",
    305: "Use Proxy",
    307: "Temporary Redirect",
    400: "Bad Request",
    401: "Unauthorized",
    402: "Payment Required",
    403: "Forbidden",
    404: "Not Found",
    405: "Method Not Allowed",
    406: "Not Acceptable",
    407: "Proxy Authentication Required",
    408: "Request Timeout",
    409: "Conflict",
    410: "Gone",
    411: "Length Required",
    412: "Precondition Failed",
    413: "Request Entity Too Large",
    414: "Request - URI Too Long",
    415: "Unsupported Media Type",
    416: "Requested Range Not Satisfiable",
    417: "Expectation Failed",
    500: "Internal Server Error",
    501: "Not Implemented",
    502: "Bad Gateway",
    503: "Service Unavailable",
    504: "Gateway Timeout",
    505: "HTTP Version Not Supported"
};
var FormData = (function () {
    function FormData() {
        this._data = new Map();
    }
    FormData.prototype.append = function (name, value) {
        this._data.set(name, value);
    };
    FormData.prototype.toString = function () {
        var arr = new Array();
        this._data.forEach(function (value, name, map) {
            arr.push(encodeURIComponent(name) + "=" + encodeURIComponent(value));
        });
        return arr.join("&");
    };
    return FormData;
}());
exports.FormData = FormData;

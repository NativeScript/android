var observable = require("data/observable");
var Cache = (function (_super) {
    __extends(Cache, _super);
    function Cache() {
        _super.apply(this, arguments);
        this.maxRequests = 5;
        this._enabled = true;
        this._pendingDownloads = {};
        this._queue = [];
        this._currentDownloads = 0;
    }
    Cache.prototype.enableDownload = function () {
        if (this._enabled) {
            return;
        }
        this._enabled = true;
        var request;
        while (this._queue.length > 0 && this._currentDownloads < this.maxRequests) {
            request = this._queue.pop();
            if (!(request.key in this._pendingDownloads)) {
                this._download(request);
            }
        }
    };
    Cache.prototype.disableDownload = function () {
        if (!this._enabled) {
            return;
        }
        this._enabled = false;
    };
    Cache.prototype.push = function (request) {
        this._addRequest(request, true);
    };
    Cache.prototype.enqueue = function (request) {
        this._addRequest(request, false);
    };
    Cache.prototype._addRequest = function (request, onTop) {
        if (request.key in this._pendingDownloads) {
            var existingRequest = this._pendingDownloads[request.key];
            this._mergeRequests(existingRequest, request);
        }
        else {
            var queueRequest;
            for (var i = 0; i < this._queue.length; i++) {
                if (this._queue[i].key === request.key) {
                    queueRequest = this._queue[i];
                    break;
                }
            }
            if (queueRequest) {
                this._mergeRequests(queueRequest, request);
            }
            else {
                if (this._shouldDownload(request, onTop)) {
                    this._download(request);
                }
            }
        }
    };
    Cache.prototype._mergeRequests = function (existingRequest, newRequest) {
        if (existingRequest.completed) {
            if (newRequest.completed) {
                var existingCompleted = existingRequest.completed;
                var stackCompleted = function (result, key) {
                    existingCompleted(result, key);
                    newRequest.completed(result, key);
                };
                existingRequest.completed = stackCompleted;
            }
        }
        else {
            existingRequest.completed = newRequest.completed;
        }
    };
    Cache.prototype.get = function (key) {
        throw new Error("Abstract");
    };
    Cache.prototype.set = function (key, image) {
        throw new Error("Abstract");
    };
    Cache.prototype.remove = function (key) {
        throw new Error("Abstract");
    };
    Cache.prototype.clear = function () {
        throw new Error("Abstract");
    };
    Cache.prototype._downloadCore = function (request) {
        throw new Error("Abstract");
    };
    Cache.prototype._onDownloadCompleted = function (key, image) {
        var request = this._pendingDownloads[key];
        if (request.key && image) {
            this.set(request.key, image);
        }
        this._currentDownloads--;
        if (request.completed) {
            request.completed(image, request.key);
        }
        if (this.hasListeners(Cache.downloadedEvent)) {
            this.notify({
                eventName: Cache.downloadedEvent,
                object: this,
                key: key,
                image: image
            });
        }
        delete this._pendingDownloads[request.key];
        this._updateQueue();
    };
    Cache.prototype._shouldDownload = function (request, onTop) {
        if (this.get(request.key) || request.key in this._pendingDownloads) {
            return false;
        }
        if (this._currentDownloads >= this.maxRequests || !this._enabled) {
            if (onTop) {
                this._queue.push(request);
            }
            else {
                this._queue.unshift(request);
            }
            return false;
        }
        return true;
    };
    Cache.prototype._download = function (request) {
        this._currentDownloads++;
        this._pendingDownloads[request.key] = request;
        this._downloadCore(request);
    };
    Cache.prototype._updateQueue = function () {
        if (!this._enabled || this._queue.length === 0 || this._currentDownloads === this.maxRequests) {
            return;
        }
        var request = this._queue.pop();
        this._download(request);
    };
    Cache.downloadedEvent = "downloaded";
    return Cache;
}(observable.Observable));
exports.Cache = Cache;

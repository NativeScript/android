var common = require("./image-cache-common");
var LruBitmapCacheClass;
function ensureLruBitmapCacheClass() {
    if (LruBitmapCacheClass) {
        return;
    }
    var LruBitmapCache = (function (_super) {
        __extends(LruBitmapCache, _super);
        function LruBitmapCache(cacheSize) {
            _super.call(this, cacheSize);
            return global.__native(this);
        }
        LruBitmapCache.prototype.sizeOf = function (key, bitmap) {
            var result = Math.round(bitmap.getByteCount() / 1024);
            return result;
        };
        return LruBitmapCache;
    }(android.util.LruCache));
    ;
    LruBitmapCacheClass = LruBitmapCache;
}
var Cache = (function (_super) {
    __extends(Cache, _super);
    function Cache() {
        _super.call(this);
        ensureLruBitmapCacheClass();
        var maxMemory = java.lang.Runtime.getRuntime().maxMemory() / 1024;
        var cacheSize = maxMemory / 8;
        this._cache = new LruBitmapCacheClass(cacheSize);
        var that = new WeakRef(this);
        this._callback = new org.nativescript.widgets.Async.CompleteCallback({
            onComplete: function (result, context) {
                var instance = that.get();
                if (instance) {
                    instance._onDownloadCompleted(context, result);
                }
            }
        });
    }
    Cache.prototype._downloadCore = function (request) {
        org.nativescript.widgets.Async.DownloadImage(request.url, this._callback, request.key);
    };
    Cache.prototype.get = function (key) {
        var result = this._cache.get(key);
        return result;
    };
    Cache.prototype.set = function (key, image) {
        this._cache.put(key, image);
    };
    Cache.prototype.remove = function (key) {
        this._cache.remove(key);
    };
    Cache.prototype.clear = function () {
        this._cache.evictAll();
    };
    return Cache;
}(common.Cache));
exports.Cache = Cache;

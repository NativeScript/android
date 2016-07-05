var timeoutHandler;
var timeoutCallbacks = {};
var timerId = 0;
function createHandlerAndGetId() {
    if (!timeoutHandler) {
        timeoutHandler = new android.os.Handler(android.os.Looper.getMainLooper());
    }
    timerId++;
    return timerId;
}
function setTimeout(callback, milliseconds) {
    if (milliseconds === void 0) { milliseconds = 0; }
    var id = createHandlerAndGetId();
    var zoneBound = zonedCallback(callback);
    var runnable = new java.lang.Runnable({
        run: function () {
            zoneBound();
            if (timeoutCallbacks[id]) {
                delete timeoutCallbacks[id];
            }
        }
    });
    if (!timeoutCallbacks[id]) {
        timeoutCallbacks[id] = runnable;
    }
    timeoutHandler.postDelayed(runnable, long(milliseconds));
    return id;
}
exports.setTimeout = setTimeout;
function clearTimeout(id) {
    if (timeoutCallbacks[id]) {
        timeoutHandler.removeCallbacks(timeoutCallbacks[id]);
        delete timeoutCallbacks[id];
    }
}
exports.clearTimeout = clearTimeout;
function setInterval(callback, milliseconds) {
    if (milliseconds === void 0) { milliseconds = 0; }
    var id = createHandlerAndGetId();
    var handler = timeoutHandler;
    var zoneBound = zonedCallback(callback);
    var runnable = new java.lang.Runnable({
        run: function () {
            zoneBound();
            if (timeoutCallbacks[id]) {
                handler.postDelayed(runnable, long(milliseconds));
            }
        }
    });
    if (!timeoutCallbacks[id]) {
        timeoutCallbacks[id] = runnable;
    }
    timeoutHandler.postDelayed(runnable, long(milliseconds));
    return id;
}
exports.setInterval = setInterval;
exports.clearInterval = clearTimeout;

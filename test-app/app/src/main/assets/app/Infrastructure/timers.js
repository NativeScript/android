var timeoutHandler;
var timeoutCallbacks = {};
function createHadlerAndGetId() {
    if (!timeoutHandler) {
        timeoutHandler = new android.os.Handler(android.os.Looper.getMainLooper());
    }
    return new Date().getUTCMilliseconds();
}
function setTimeout(callback, milliseconds) {
    if (milliseconds === void 0) { milliseconds = 0; }
    var id = createHadlerAndGetId();
    var runnable = new java.lang.Runnable({
        run: function () {
            callback();
            if (timeoutCallbacks && timeoutCallbacks[id]) {
                timeoutCallbacks[id] = null;
            }
        }
    });
    if (!timeoutCallbacks[id]) {
        timeoutCallbacks[id] = runnable;
    }
    timeoutHandler.postDelayed(runnable, long(milliseconds));
    return id;
}
global.setTimeout = setTimeout;
function clearTimeout(id) {
    if (timeoutCallbacks[id]) {
        timeoutHandler.removeCallbacks(timeoutCallbacks[id]);
        timeoutCallbacks[id] = null;
    }
}
global.clearTimeout = clearTimeout;
function setInterval(callback, milliseconds) {
    if (milliseconds === void 0) { milliseconds = 0; }
    var id = createHadlerAndGetId();
    var handler = timeoutHandler;
    var runnable = new java.lang.Runnable({
        run: function () {
            callback();
            handler.postDelayed(runnable, long(milliseconds));
        }
    });
    if (!timeoutCallbacks[id]) {
        timeoutCallbacks[id] = runnable;
    }
    timeoutHandler.postDelayed(runnable, long(milliseconds));
    return id;
}
global.setInterval = setInterval;
global.clearInterval = clearTimeout;

var fpsNative = require("fps-meter/fps-native");
var callbacks = {}, idCounter = 0, _minFps = 1000, framesRendered = 0, frameStartTime = 0;
function doFrame(currentTimeMillis) {
    var fps = 0;
    if (frameStartTime > 0) {
        var timeSpan = (currentTimeMillis - frameStartTime);
        framesRendered++;
        if (timeSpan > 1000) {
            fps = framesRendered * 1000 / timeSpan;
            if (fps < _minFps) {
                _minFps = fps;
            }
            notify(fps);
            frameStartTime = currentTimeMillis;
            framesRendered = 0;
        }
    }
    else {
        frameStartTime = currentTimeMillis;
    }
}
var native;
function ensureNative() {
    if (!native) {
        native = new fpsNative.FPSCallback(doFrame);
    }
}
function reset() {
    _minFps = 1000;
    frameStartTime = 0;
    framesRendered = 0;
}
exports.reset = reset;
function running() {
    if (!native) {
        return false;
    }
    return native.running;
}
exports.running = running;
function minFps() {
    return _minFps;
}
exports.minFps = minFps;
function start() {
    ensureNative();
    native.start();
}
exports.start = start;
function stop() {
    if (!native) {
        return;
    }
    native.stop();
    reset();
}
exports.stop = stop;
function addCallback(callback) {
    var id = idCounter;
    callbacks[id] = callback;
    idCounter++;
    return id;
}
exports.addCallback = addCallback;
function removeCallback(id) {
    if (id in callbacks) {
        delete callbacks[id];
    }
}
exports.removeCallback = removeCallback;
function notify(fps) {
    var callback;
    for (var id in callbacks) {
        callback = callbacks[id];
        callback(fps, _minFps);
    }
}

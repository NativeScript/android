// The compat globals are assembled per isolate, so a worker's globals must be
// its own module instance's functions.
onmessage = function () {
    var android = require("ns:interop/android");
    var runtime = require("ns:runtime");
    postMessage({
        casts: ["long", "byte", "short", "char", "float", "double"].every(function (name) {
            return globalThis[name] === android[name];
        }),
        newArray: Array.create === android.newArray,
        escapeException: interop.escapeException === runtime.escapeException,
        arrayLength: android.newArray("int", 4).length,
    });
};

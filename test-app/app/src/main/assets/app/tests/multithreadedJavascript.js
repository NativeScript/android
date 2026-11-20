// Whether this runtime runs a Java->JS call on the Java thread that makes it, which is what
// the app's android.enableMultithreadedJavascript turns on. Found by trying it once: without
// it, a call from a background Java thread is posted to the runtime's own thread instead.
// The result is reported on the main looper, so Jasmine never continues on another thread.
var detected;

module.exports = function multithreadedJavascript() {
    if (detected === undefined) {
        detected = new Promise(function (resolve) {
            var mainThread = java.lang.Thread.currentThread().getId();
            var mainHandler = new android.os.Handler(android.os.Looper.getMainLooper());
            new java.lang.Thread(new java.lang.Runnable({
                run: function () {
                    var enabled = java.lang.Thread.currentThread().getId() !== mainThread;
                    mainHandler.post(new java.lang.Runnable({
                        run: function () {
                            resolve(enabled);
                        }
                    }));
                }
            })).start();
        });
    }
    return detected;
};

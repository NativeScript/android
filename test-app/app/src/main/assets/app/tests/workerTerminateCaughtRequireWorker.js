// Entry for testWorkerTerminateCaughtRequire. [0] tells the parent the
// require below is about to start; [1] is only ever set by code that ran
// after that require was interrupted.
onmessage = function (msg) {
    var shared = new Int32Array(msg.data);
    Atomics.store(shared, 0, 1);
    try {
        // Spins at module scope, so the parent's terminate() lands inside it.
        require("./workerTerminateDuringLoadWorker.js");
    } catch (e) {
    }
    Atomics.store(shared, 1, 1);
};

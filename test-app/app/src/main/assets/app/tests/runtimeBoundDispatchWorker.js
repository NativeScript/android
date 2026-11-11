// Entry for testRuntimeBoundDispatch: creates JS-implemented Java objects in this
// worker's runtime and hands them to the spec through a Java-side holder.
var Holder = com.tns.tests.RuntimeBoundHolder;

function threadName() {
    return java.lang.Thread.currentThread().getName();
}

// Captured by the implementations below, so it can only be reported by code
// that runs in this worker's isolate.
var RUNTIME_LABEL = "runtimeBoundDispatchWorker";

Holder.put("runnable", new java.lang.Runnable({
    run: function () {
        Holder.put("ranIn", RUNTIME_LABEL);
        Holder.put("ranOn", threadName());
    }
}));

Holder.put("comparable", new java.lang.Comparable({
    compareTo: function () {
        return 42;
    }
}));

onmessage = function (msg) {
    if (msg.data === "busy-then-close") {
        // Anything the spec queues on this looper while it sleeps is
        // discarded by close(): Looper.quit() drops pending messages.
        postMessage("busy");
        java.lang.Thread.sleep(1000);
        close();
    }
};

postMessage(threadName());

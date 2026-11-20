// A JS-implemented Java object belongs to the runtime that created it: Java->JS
// calls on it run there, whichever thread or runtime makes the call, and calls
// that can no longer run there are dropped rather than thrown or left hanging.
describe("Runtime-bound Java->JS dispatch", function () {
    var Holder = com.tns.tests.RuntimeBoundHolder;
    var WORKER_LABEL = "runtimeBoundDispatchWorker";

    // With multithreaded JS the call runs on the calling thread under the
    // worker isolate's lock; without it, the call is posted to the worker's
    // looper and the caller waits.
    var multithreaded = !!require("../package.json").android.enableMultithreadedJavascript;

    function expectRanInWorker(workerThread, callerThread) {
        expect(Holder.get("ranIn")).toBe(WORKER_LABEL);
        expect(Holder.get("ranOn")).toBe(multithreaded ? callerThread : workerThread);
        Holder.put("ranIn", "");
        Holder.put("ranOn", "");
    }

    var originalTimeout;
    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 30000;
    });
    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
        Holder.clear();
    });

    // Calls onReady once the worker has created its objects.
    function startWorker(onReady) {
        var worker = new Worker("./runtimeBoundDispatchWorker.js");
        worker.onmessage = function (msg) {
            worker.onmessage = null;
            onReady(worker, msg.data);
        };
        return worker;
    }

    it("runs a worker's object on that worker when this runtime also wraps it", function (done) {
        startWorker(function (worker, workerThread) {
            // Wrapping it puts the instance in this runtime's object maps too.
            var runnable = Holder.get("runnable");
            runnable.run();
            expectRanInWorker(workerThread, "main");

            Holder.run("runnable");
            expectRanInWorker(workerThread, "main");

            expect(Holder.compare("comparable")).toBe(42);

            worker.terminate();
            done();
        });
    });

    it("runs a worker's object on that worker when called from a plain Java thread", function (done) {
        startWorker(function (worker, workerThread) {
            Holder.runOnNewThread("runnable");
            expectRanInWorker(workerThread, "RuntimeBoundHolder-caller");

            worker.terminate();
            done();
        });
    });

    it("drops calls to an object whose worker has terminated", function (done) {
        startWorker(function (worker) {
            worker.terminate();

            var deadline = Date.now() + 10000;
            (function poll() {
                // Until teardown completes a call may still be answered.
                var result;
                try {
                    result = Holder.compare("comparable");
                } catch (e) {
                    fail("a call to a terminated worker's object threw: " + e);
                    done();
                    return;
                }

                if (result === 0) {
                    done();
                } else if (Date.now() > deadline) {
                    fail("calls were still answered 10s after terminate()");
                    done();
                } else {
                    setTimeout(poll, 50);
                }
            })();
        });
    });

    // Callers on other threads take the worker's isolate while it terminates: each call is
    // answered or dropped, never run against an isolate that is being torn down.
    it("answers or drops calls other threads make while the worker terminates", function (done) {
        if (!multithreaded) {
            // Calls are posted to the worker's looper in this mode, not run under its lock.
            done();
            return;
        }

        function waitFor(condition, what, then) {
            var deadline = Date.now() + 10000;
            (function poll() {
                if (condition()) {
                    then();
                } else if (Date.now() > deadline) {
                    Holder.stopComparing(10000);
                    fail("timed out waiting for " + what);
                    done();
                } else {
                    setTimeout(poll, 20);
                }
            })();
        }

        startWorker(function (worker) {
            Holder.startComparing("comparable", 4);
            waitFor(function () {
                return Holder.hasCompared(42);
            }, "a call answered by the live worker", function () {
                worker.terminate();
                waitFor(function () {
                    return Holder.hasCompared(0);
                }, "a call dropped after terminate()", function () {
                    var results = Array.prototype.slice.call(Holder.stopComparing(10000));
                    results.forEach(function (result) {
                        expect([42, 0]).toContain(result);
                    });
                    done();
                });
            });
        });
    });

    it("releases a caller whose queued call is discarded when the worker closes", function (done) {
        if (multithreaded) {
            // Calls are never queued on the worker's looper in this mode.
            done();
            return;
        }

        startWorker(function (worker) {
            worker.onmessage = function (msg) {
                if (msg.data !== "busy") {
                    return;
                }

                // The worker is asleep in its handler, so this call queues
                // behind it and is discarded by the close() that follows.
                var start = Date.now();
                expect(Holder.compare("comparable")).toBe(0);
                expect(Date.now() - start).toBeLessThan(10000);
                done();
            };
            worker.postMessage("busy-then-close");
        });
    });
});

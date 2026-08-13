// The addons are compiled into libNativeScript.so for local Debug builds
// only; on any other runtime flavor the suite skips rather than fails.
var napiTestModuleAvailable = true;
try {
    require("napitestmodule");
} catch (e) {
    napiTestModuleAvailable = false;
}

(napiTestModuleAvailable ? describe : xdescribe)("Node-API addon", function () {
    // Conditional so the disabled suite's declaration body stays throw-free
    // (jasmine executes it even for xdescribe).
    var napi = napiTestModuleAvailable ? require("napitestmodule") : {};

    it("exports the addon's functions", function () {
        expect(typeof napi).toBe("object");
        expect(typeof napi.echoString).toBe("function");
        expect(typeof napi.doubleNumber).toBe("function");
        expect(typeof napi.negateBool).toBe("function");
        expect(typeof napi.transformObject).toBe("function");
        expect(typeof napi.transformArray).toBe("function");
        expect(typeof napi.throwError).toBe("function");
        expect(typeof napi.wrapValue).toBe("function");
        expect(typeof napi.unwrapValue).toBe("function");
        expect(typeof napi.finalizerRan).toBe("function");
        expect(typeof napi.resetFinalizerFlag).toBe("function");
        expect(typeof napi.holdRef).toBe("function");
        expect(typeof napi.getRef).toBe("function");
        expect(typeof napi.releaseRef).toBe("function");
        expect(typeof napi.startAsyncWork).toBe("function");
        expect(typeof napi.startCancelledWork).toBe("function");
        expect(typeof napi.startTsfn).toBe("function");
        expect(typeof napi.pushTsfn).toBe("function");
        expect(typeof napi.probeTsfnAbort).toBe("function");
        expect(typeof napi.invokeViaMakeCallback).toBe("function");
        expect(typeof napi.exerciseCleanupHooks).toBe("function");
    });

    it("is instantiated once per env", function () {
        expect(require("napitestmodule")).toBe(napi);
    });

    it("round-trips a string", function () {
        expect(napi.echoString("hello")).toBe("hello");
        expect(napi.echoString("")).toBe("");
        expect(napi.echoString("ünïcödé ☃")).toBe("ünïcödé ☃");
    });

    it("round-trips a number", function () {
        expect(napi.doubleNumber(21)).toBe(42);
        expect(napi.doubleNumber(-1.5)).toBe(-3);
    });

    it("round-trips a bool", function () {
        expect(napi.negateBool(true)).toBe(false);
        expect(napi.negateBool(false)).toBe(true);
    });

    it("reads a property and builds a new object", function () {
        var result = napi.transformObject({ value: 4, ignored: "x" });
        expect(result.value).toBe(8);
        expect(result.tag).toBe("napi");
        expect(result.ignored).toBeUndefined();
    });

    it("reads an array and builds a new array", function () {
        var result = napi.transformArray([7, 8, 9]);
        expect(Array.isArray(result)).toBe(true);
        expect(result.length).toBe(2);
        expect(result[0]).toBe(3);
        expect(result[1]).toBe(7);

        expect(napi.transformArray([])).toEqual([0, 0]);
    });

    it("throws a catchable Error carrying a code", function () {
        var error;
        try {
            napi.throwError();
        } catch (e) {
            error = e;
        }
        expect(error instanceof Error).toBe(true);
        expect(error.message).toBe("napi test failure");
        expect(error.code).toBe("ERR_TEST_CODE");
    });

    describe("napi_define_properties", function () {
        it("defines a value property", function () {
            var descriptor = Object.getOwnPropertyDescriptor(napi, "moduleName");
            expect(descriptor.value).toBe("napitestmodule");
            expect(descriptor.get).toBeUndefined();
            expect(descriptor.enumerable).toBe(true);
        });

        it("defines an accessor property", function () {
            var descriptor = Object.getOwnPropertyDescriptor(napi, "wrapCount");
            expect(typeof descriptor.get).toBe("function");
            expect(descriptor.value).toBeUndefined();

            var before = napi.wrapCount;
            napi.wrapValue({}, 1);
            expect(napi.wrapCount).toBe(before + 1);
        });
    });

    describe("napi_wrap", function () {
        it("unwraps the payload it wrapped", function () {
            var target = {};
            expect(napi.wrapValue(target, 3.5)).toBe(target);
            expect(napi.unwrapValue(target)).toBe(3.5);
        });

        it("runs the finalizer once the wrapper is collected", function (done) {
            napi.resetFinalizerFlag();
            expect(napi.finalizerRan()).toBe(false);

            (function () {
                napi.wrapValue({}, 11);
            })();

            // Conservative stack scanning keeps the dead wrapper alive until
            // the loop below overwrites the frame that held it.
            __collect();
            var sink = 0;
            for (var i = 0; i < 200000; i++) {
                sink += i % 7;
            }
            __collect();

            expect(sink).toBeGreaterThan(0);

            // Node-API finalizers are queued from the weak callback and drained
            // on a later event-loop entry, never inside the collection itself.
            expect(napi.finalizerRan()).toBe(false);

            // The drain shares the internal lane with V8's own GC tasks and
            // runs one entry per pass, so there is no ordering guarantee
            // against timers — poll instead of assuming the first tick.
            var attempts = 0;
            (function poll() {
                if (napi.finalizerRan() || ++attempts > 50) {
                    expect(napi.finalizerRan()).toBe(true);
                    done();
                    return;
                }
                setTimeout(poll, 0);
            })();
        });
    });

    describe("napi_make_callback", function () {
        it("calls the function and returns its result", function () {
            var seen;
            var result = napi.invokeViaMakeCallback(function (value) {
                seen = value;
                return value + 1;
            }, 41);

            expect(seen).toBe(41);
            expect(result).toBe(42);
        });

        it("propagates a throw from the callback", function () {
            var error;
            try {
                napi.invokeViaMakeCallback(function () {
                    throw new Error("boom");
                }, 1);
            } catch (e) {
                error = e;
            }

            expect(error instanceof Error).toBe(true);
            expect(error.message).toBe("boom");
        });
    });

    describe("napi_async_work", function () {
        it("executes off the JS thread and completes on it", function (done) {
            napi.startAsyncWork(21, function (status, result, ranOffJsThread) {
                expect(status).toBe("ok");
                expect(result).toBe(42);
                expect(ranOffJsThread).toBe(true);
                done();
            });
        });

        it("completes cancelled work with a cancelled status", function (done) {
            // Whether the cancel wins the race against the queue is not ours to
            // decide; both outcomes have to hold up.
            var cancelStatus = napi.startCancelledWork(function (status) {
                if (cancelStatus === "ok") {
                    expect(status).toBe("cancelled");
                } else {
                    expect(cancelStatus).toBe("generic_failure");
                    expect(status).toBe("ok");
                }
                done();
            });
        });
    });

    describe("napi_threadsafe_function", function () {
        it("delivers every value in order, then finalizes", function (done) {
            var values = [];

            napi.startTsfn(
                5,
                0,
                function (value) {
                    values.push(value);
                },
                function (lastCallStatus) {
                    expect(lastCallStatus).toBe("ok");
                    expect(values).toEqual([1, 2, 3, 4, 5]);
                    done();
                }
            );
        });

        it("blocks the producer on a full queue instead of dropping values", function (done) {
            var values = [];

            napi.startTsfn(
                50,
                2,
                function (value) {
                    values.push(value);
                },
                function (lastCallStatus) {
                    expect(lastCallStatus).toBe("ok");
                    expect(values.length).toBe(50);
                    expect(values[0]).toBe(1);
                    expect(values[49]).toBe(50);
                    done();
                }
            );
        });

        it("accepts a call made from inside its own callback", function (done) {
            var values = [];
            var pushStatus;

            napi.startTsfn(
                3,
                0,
                function (value) {
                    values.push(value);
                    if (value === 1) {
                        pushStatus = napi.pushTsfn(100);
                    }
                },
                function () {
                    expect(pushStatus).toBe("ok");
                    expect(values.length).toBe(4);
                    // The producer may already have queued 2 and 3 by then, so
                    // only "after the first value" is guaranteed.
                    expect(values.indexOf(100)).toBeGreaterThan(0);
                    done();
                }
            );
        });

        it("reports closing after an abort and drops the queued call", function (done) {
            var delivered = [];
            var probe = napi.probeTsfnAbort(function (value) {
                delivered.push(value);
            });

            expect(probe.queued).toBe("ok");
            expect(probe.released).toBe("ok");
            expect(probe.afterAbort).toBe("closing");

            setTimeout(function () {
                expect(delivered).toEqual([]);
                done();
            }, 0);
        });
    });

    describe("cleanup hooks", function () {
        it("rejects removing a hook that is no longer registered", function () {
            expect(napi.exerciseCleanupHooks()).toBe("invalid_arg");
        });
    });

    describe("workers", function () {
        it("instantiates the addon separately in each isolate", function (done) {
            var worker = new Worker("./napiEvalWorker.js");
            worker.onmessage = function (msg) {
                worker.terminate();
                expect(msg.data.doubled).toBe(42);
                expect(msg.data.cached).toBe(true);
                done();
            };
            worker.postMessage({
                eval:
                    "var m = require('napitestmodule'); " +
                    "postMessage({ doubled: m.doubleNumber(21), cached: require('napitestmodule') === m });"
            });
        });
    });

    describe("napi_create_reference", function () {
        afterEach(function () {
            napi.releaseRef();
        });

        it("holds and returns the referenced value", function () {
            var held = { id: "held" };
            napi.holdRef(held);
            expect(napi.getRef()).toBe(held);
            expect(napi.getRef()).toBe(held);
        });

        it("returns undefined once released", function () {
            napi.holdRef({ id: "held" });
            expect(napi.releaseRef()).toBe(true);
            expect(napi.getRef()).toBeUndefined();
            expect(napi.releaseRef()).toBe(false);
        });
    });
});

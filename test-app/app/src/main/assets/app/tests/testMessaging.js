// Android-side regression specs for the messaging tier. The shared suites under
// app/shared cover the specified behavior; these pin runtime edges that need
// native wrappers, a worker that never settles, or a collection.
describe("Messaging runtime edges", function () {
    var parkedEntry = "./messaging/parkedWorker.mjs";
    // Delivery goes through the event loop; SETTLE is long enough that an
    // event which was going to arrive would have.
    var SETTLE = 400;

    describe("transfer lists", function () {
        it("rejects a buffer a getter detached while the graph was being written", function () {
            var buffer = new ArrayBuffer(16);
            var error = null;
            try {
                structuredClone({ get x() { buffer.transfer(); return 1; } }, { transfer: [buffer] });
            } catch (e) {
                error = e;
            }
            expect(error).not.toBeNull();
            expect(error.name).toBe("DataCloneError");
        });

        it("hands a listed port over through the cloned value", function (done) {
            var channel = new MessageChannel();
            var clone = structuredClone(channel.port2, { transfer: [channel.port2] });
            expect(clone instanceof MessagePort).toBe(true);
            expect(clone).not.toBe(channel.port2);
            clone.addEventListener("message", function (event) {
                expect(event.data).toBe("through");
                clone.close();
                channel.port1.close();
                done();
            });
            channel.port1.postMessage("through");
        });

        it("closes a listed port the cloned value never names", function (done) {
            var channel = new MessageChannel();
            var closed = false;
            channel.port1.addEventListener("close", function () { closed = true; });
            structuredClone({}, { transfer: [channel.port2] });
            setTimeout(function () {
                expect(closed).toBe(true);
                channel.port1.close();
                done();
            }, SETTLE);
        });

        it("closes a port transferred to a worker that is terminated before its entry settles", function (done) {
            var worker = new Worker(parkedEntry);
            var channel = new MessageChannel();
            var closed = false;
            channel.port1.addEventListener("close", function () { closed = true; });
            worker.postMessage(channel.port2, [channel.port2]);
            worker.terminate();
            setTimeout(function () {
                expect(closed).toBe(true);
                channel.port1.close();
                done();
            }, SETTLE);
        });
    });

    describe("handler attributes", function () {
        it("enables a port when onmessage is first set to null", function (done) {
            var channel = new MessageChannel();
            channel.port2.postMessage("consumed");
            channel.port1.onmessage = null;
            setTimeout(function () {
                var received = 0;
                channel.port1.addEventListener("message", function () { received++; });
                setTimeout(function () {
                    expect(received).toBe(0);
                    channel.port1.close();
                    channel.port2.close();
                    done();
                }, SETTLE);
            }, SETTLE);
        });

        it("keeps a port disabled until a handler or listener arrives", function (done) {
            var channel = new MessageChannel();
            channel.port2.postMessage("kept");
            setTimeout(function () {
                var received = 0;
                channel.port1.addEventListener("message", function () { received++; });
                setTimeout(function () {
                    expect(received).toBe(1);
                    channel.port1.close();
                    channel.port2.close();
                    done();
                }, SETTLE);
            }, SETTLE);
        });
    });

    describe("MessagePort surface", function () {
        it("runs an onclose handler when the port is closed", function () {
            var channel = new MessageChannel();
            var seen = null;
            channel.port1.onclose = function (event) { seen = event.type; };
            channel.port1.close();
            expect(seen).toBe("close");
            channel.port2.close();
        });
    });

    describe("BroadcastChannel", function () {
        it("treats the empty name as a channel like any other", function (done) {
            var a = new BroadcastChannel("");
            var b = new BroadcastChannel("");
            var c = new BroadcastChannel("");
            var got = [];
            a.onmessage = function (event) { got.push(event.data); };
            b.close();
            setTimeout(function () {
                c.postMessage("still open");
                setTimeout(function () {
                    expect(got).toEqual(["still open"]);
                    a.close();
                    c.close();
                    done();
                }, SETTLE);
            }, SETTLE);
        });
    });

    describe("node:worker_threads", function () {
        var wt = require("node:worker_threads");

        it("exposes the emitter surface on parentPort", function (done) {
            // The shim resolves the entry from the app root, not from the
            // requiring test file, hence the ~/ form.
            var worker = new wt.Worker("~/tests/messaging/parentPortWorker.js");
            var got = [];
            worker.on("message", function (value) {
                got.push(value);
                if (got.length === 3) {
                    expect(got).toEqual([{ once: 1 }, { on: 1 }, { on: 2 }]);
                    worker.terminate();
                    done();
                }
            });
            worker.on("error", function (error) {
                fail("worker error: " + error.message);
                worker.terminate();
                done();
            });
            worker.postMessage(1);
            worker.postMessage(2);
        });

        it("lets the same listener be on() and once() at the same time", function (done) {
            var worker = new wt.Worker("~/tests/messaging/parentPortOnceWorker.js");
            var got = [];
            worker.on("message", function (value) {
                got.push(value);
                if (got.length === 3) {
                    setTimeout(function () {
                        // Three messages: the once() registration fires only
                        // for the first, the on() one for all three.
                        expect(got).toEqual([1, 2, 3, 4]);
                        worker.terminate();
                        done();
                    }, SETTLE);
                }
            });
            worker.on("error", function (error) {
                fail("worker error: " + error.message);
                worker.terminate();
                done();
            });
            worker.postMessage("a");
            worker.postMessage("b");
            worker.postMessage("c");
        });

        it("relays transferred ports to parentPort message events", function (done) {
            var worker = new wt.Worker("~/tests/messaging/parentPortPortsWorker.js");
            var channel = new MessageChannel();
            worker.on("message", function (value) {
                expect(value).toBe(1);
                channel.port1.close();
                worker.terminate();
                done();
            });
            worker.on("error", function (error) {
                fail("worker error: " + error.message);
                worker.terminate();
                done();
            });
            worker.postMessage(channel.port2, [channel.port2]);
        });

        it("forwards the option bag to the runtime's Worker", function () {
            // androidPriority is the runtime Worker's own option: the shim does
            // not know it, so a rejection proves the bag reached the native
            // constructor.
            expect(function () {
                new wt.Worker("~/tests/messaging/parentPortWorker.js", {
                    androidPriority: "turbo",
                });
            }).toThrow();
        });
    });

    describe("worker error reporting", function () {
        // A worker boots on its own thread, so the first error arrives whenever
        // the runner gets to it; specs wait for it and only then settle for
        // duplicates.
        var originalTimeout;
        beforeEach(function () {
            originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
            jasmine.DEFAULT_TIMEOUT_INTERVAL = 60000;
        });
        afterEach(function () {
            jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
        });

        it("reports an error the Worker object left unhandled to the parent scope", function (done) {
            var seen = [];
            var worker = null;
            var listener = function (event) {
                seen.push(event);
                event.preventDefault();
                if (seen.length === 1) {
                    setTimeout(finish, SETTLE);
                }
            };
            var finish = function () {
                removeEventListener("error", listener);
                expect(seen.length).toBe(1);
                expect(seen[0].message).toContain("boom from worker");
                expect(seen[0].error instanceof Error).toBe(true);
                worker.terminate();
                done();
            };
            addEventListener("error", listener);
            worker = new Worker("./messaging/throwingWorker.js");
        });

        it("forwards the error a throwing scope onerror raised for a rejection, once", function (done) {
            var worker = new Worker("./messaging/rejectingWorker.js");
            var messages = [];
            worker.onerror = function (event) {
                messages.push(event.message);
                event.preventDefault();
                if (messages.length === 1) {
                    setTimeout(function () {
                        expect(messages.length).toBe(1);
                        expect(messages[0]).toContain("thrown by scope onerror");
                        worker.terminate();
                        done();
                    }, SETTLE);
                }
            };
        });
    });

    describe("AbortSignal handler attribute accounting", function () {
        function pollGC(predicate, cb) {
            var turns = 0;
            (function poll() {
                __collect();
                if (predicate() || turns >= 100) {
                    cb();
                    return;
                }
                turns++;
                setTimeout(poll, 20);
            })();
        }

        it("a timeout signal whose onabort was only ever set to null is collectable", function (done) {
            var wr = (function () {
                var signal = AbortSignal.timeout(60000);
                signal.onabort = null;
                return new WeakRef(signal);
            })();
            pollGC(function () { return wr.deref() === undefined; }, function () {
                expect(wr.deref()).toBeUndefined();
                done();
            });
        });

        it("a timeout signal whose onabort was cleared again is collectable", function (done) {
            var wr = (function () {
                var signal = AbortSignal.timeout(60000);
                signal.onabort = function () {};
                signal.onabort = null;
                return new WeakRef(signal);
            })();
            pollGC(function () { return wr.deref() === undefined; }, function () {
                expect(wr.deref()).toBeUndefined();
                done();
            });
        });

        it("a timeout signal with an onabort handler survives GC and still aborts", function (done) {
            var reasonName = null;
            (function () {
                AbortSignal.timeout(300).onabort = function (event) {
                    reasonName = event.target.reason.name;
                };
            })();
            __collect();
            pollGC(function () { return reasonName !== null; }, function () {
                expect(reasonName).toBe("TimeoutError");
                done();
            });
        });
    });
});

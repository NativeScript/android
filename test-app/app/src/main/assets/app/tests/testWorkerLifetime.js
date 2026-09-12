// Worker lifetime under GC. A running worker's JS wrapper is a GC root, so it
// behaves like any other strongly held object: weak collections keyed on it
// keep their entries, and it keeps answering messages nobody holds a reference
// to it for. Once the worker ends — terminate() or its own close() — the root
// is dropped and the wrapper becomes collectable.

describe("Worker lifetime", function () {
    // GC polling and worker teardown are far slower on a device than the
    // jasmine default allows for.
    var originalTimeout;
    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 30000;
    });
    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
    });

    var WORKER_COUNT = 4;
    var PAYLOAD_SIZE = 64;

    // A collection per loop turn: weak-collection clearing needs turns after
    // the collect, so nothing here asserts synchronously after __collect().
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

    // Reached through a call rather than a closure, so the worker it derefs
    // cannot end up in a scope the caller's later callbacks keep alive.
    function terminateWorker(ref) {
        var worker = ref.deref();
        if (worker !== undefined) {
            worker.terminate();
        }
    }

    function postToWorker(ref, message) {
        var worker = ref.deref();
        if (worker !== undefined) {
            worker.postMessage(message);
        }
    }

    // Enough allocation to put V8 part-way through an incremental/concurrent
    // mark, so the collection that follows finishes a mark that was already
    // running rather than starting an atomic one.
    function churn() {
        var sink = null;
        for (var i = 0; i < 24; i++) {
            var block = new Array(8192);
            for (var j = 0; j < 8192; j++) {
                block[j] = { j: j, s: "churn-" + j };
            }
            sink = block;
        }
        return sink !== null;
    }

    function makePayload(id) {
        var payload = new Array(PAYLOAD_SIZE);
        for (var i = 0; i < PAYLOAD_SIZE; i++) {
            payload[i] = "payload-" + id + "-" + i;
        }
        return payload;
    }

    it("a live Worker survives GC as a WeakMap key", function (done) {
        // Nothing outside this map holds the values: an entry whose key stays
        // alive while its value is not marked is what leaves a dangling value
        // slot behind.
        var sideTable = new WeakMap();
        var refs = [];
        var replies = 0;

        for (var i = 0; i < WORKER_COUNT; i++) {
            refs.push((function (index) {
                var worker = new Worker("./eventLoopEchoWorker.js");
                // A second entry reachable only through the first one's value,
                // so resolving these takes more than one ephemeron pass.
                var link = { id: index };
                sideTable.set(link, { deep: index, payload: makePayload("deep" + index) });
                sideTable.set(worker, { id: index, link: link, payload: makePayload(index) });
                worker.onmessage = function () { replies++; };
                worker.postMessage("ping");
                return new WeakRef(worker);
            })(i));
        }

        var round = 0;
        function spin() {
            churn();
            // async execution runs the collection from a task, so V8 treats the
            // stack as pointer-free and the workers are genuinely unreachable
            // for it — a conservative scan of this frame would not let them be.
            __collect({ execution: "async" }).then(function () {
                __collect();

                // Only some turns touch the workers: a turn that does not leaves
                // them dead for a whole mark cycle.
                if (round % 3 === 0) {
                    for (var i = 0; i < refs.length; i++) {
                        postToWorker(refs[i], "ping-" + round);
                    }
                }

                round++;
                if (round < 15) {
                    setTimeout(spin, 20);
                    return;
                }

                for (var k = 0; k < refs.length; k++) {
                    var survivor = refs[k].deref();
                    expect(survivor).not.toBeUndefined();
                    if (survivor === undefined) {
                        continue;
                    }
                    var entry = sideTable.get(survivor);
                    expect(entry).not.toBeUndefined();
                    if (entry !== undefined) {
                        expect(entry.id).toBe(k);
                        expect(entry.payload.length).toBe(PAYLOAD_SIZE);
                        expect(entry.payload[PAYLOAD_SIZE - 1]).toBe(
                            "payload-" + k + "-" + (PAYLOAD_SIZE - 1));
                        var deep = sideTable.get(entry.link);
                        expect(deep).not.toBeUndefined();
                        if (deep !== undefined) {
                            expect(deep.deep).toBe(k);
                            expect(deep.payload.length).toBe(PAYLOAD_SIZE);
                        }
                    }
                }
                expect(replies).toBeGreaterThan(0);

                for (var t = 0; t < refs.length; t++) {
                    terminateWorker(refs[t]);
                }
                done();
            });
        }
        spin();
    });

    it("an unreferenced live Worker still answers messages", function (done) {
        var reply = null;
        var ref = (function () {
            var worker = new Worker("./eventLoopEchoWorker.js");
            worker.onmessage = function (event) { reply = event.data; };
            worker.postMessage("hello");
            return new WeakRef(worker);
        })();

        pollGC(function () { return reply !== null; }, function () {
            expect(reply).toBe("hello");
            expect(ref.deref()).not.toBeUndefined();
            terminateWorker(ref);
            done();
        });
    });

    it("a terminated Worker becomes collectable", function (done) {
        var ended = false;
        var ref = (function () {
            var worker = new Worker("./eventLoopEchoWorker.js");
            worker.addEventListener("nsworkerended", function () { ended = true; });
            worker.postMessage("ping");
            return new WeakRef(worker);
        })();

        setTimeout(function () {
            terminateWorker(ref);
            // The root outlives terminate() by design: it goes only when the
            // thread reports its end, so collectability is observable no
            // earlier than that.
            pollGC(function () { return ended; }, function () {
                expect(ended).toBe(true);
                pollGC(function () { return ref.deref() === undefined; }, function () {
                    expect(ref.deref()).toBeUndefined();
                    done();
                });
            });
        }, 150);
    });

    it("a Worker that closed itself becomes collectable", function (done) {
        var ended = false;
        var ref = (function () {
            var worker = new Worker("./workerLifetimeCloseWorker.js");
            worker.addEventListener("nsworkerended", function () { ended = true; });
            worker.postMessage("close");
            return new WeakRef(worker);
        })();

        pollGC(function () { return ended; }, function () {
            expect(ended).toBe(true);
            pollGC(function () { return ref.deref() === undefined; }, function () {
                expect(ref.deref()).toBeUndefined();
                done();
            });
        });
    });
});

describe("node:worker_threads Worker exit", function () {
    // GC polling and worker teardown are far slower on a device than the
    // jasmine default allows for.
    var originalTimeout;
    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 30000;
    });
    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
    });

    var wt = require("node:worker_threads");

    it("emits 'exit' once when the worker closes itself", function (done) {
        var worker = new wt.Worker("~/tests/workerLifetimeCloseWorker.js");
        var codes = [];
        worker.on("exit", function (code) { codes.push(code); });
        worker.postMessage("go");

        setTimeout(function () {
            expect(codes).toEqual([0]);
            done();
        }, 2000);
    });

    it("emits 'exit' once on terminate(), after the thread ended, and resolves then", function (done) {
        var worker = new wt.Worker("~/tests/eventLoopEchoWorker.js");
        var codes = [];
        worker.on("exit", function (code) { codes.push(code); });

        setTimeout(function () {
            var resolved = null;
            worker.terminate().then(function (code) {
                resolved = code;
                // 'exit' precedes the promise settling.
                expect(codes).toEqual([0]);
            });
            setTimeout(function () {
                expect(resolved).toBe(0);
                expect(codes).toEqual([0]);
                worker.terminate().then(function (code) {
                    expect(code).toBe(0);
                    expect(codes).toEqual([0]);
                    done();
                });
            }, 2000);
        }, 150);
    });
});

describe("Worker teardown with a transferred port in flight", function () {
    // GC polling and worker teardown are far slower on a device than the
    // jasmine default allows for.
    var originalTimeout;
    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 30000;
    });
    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
    });

    // The parent worker's loop still holds a message carrying a port whose
    // sibling that worker owns; dropping it during shutdown posts the sibling's
    // close sentinel back into the loop being shut down.
    it("ends a terminated worker whose dropped message sentinels a port it owns", function (done) {
        var worker = new Worker("./messaging/deadlockParent.js");
        var ended = false;
        worker.addEventListener("nsworkerended", function () { ended = true; });
        worker.onerror = function (event) {
            fail("worker error: " + event.message);
            return true;
        };
        worker.onmessage = function (event) {
            expect(event.data).toBe("ready");
            worker.terminate();
            var deadline = Date.now() + 10000;
            (function poll() {
                if (ended || Date.now() > deadline) {
                    expect(ended).toBe(true);
                    done();
                    return;
                }
                setTimeout(poll, 50);
            })();
        };
    });
});

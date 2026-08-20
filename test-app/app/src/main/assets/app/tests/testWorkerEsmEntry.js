// An ES module worker entry takes the same RunModule branch — and the same
// boot evaluation options — the app's main entry takes, so these pin that
// destination even though the suite cannot re-drive the app's own boot.
// A worker specifier is resolved through Java resolvePath whatever route the
// entry ends up taking, so app-root-absolute, relative and extension-less
// paths all reach an `.mjs` entry.
describe("worker ES module entries", function () {
    var originalTimeout;

    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 15000;
    });

    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
    });

    it("runs a synchronous ES module worker entry, statics and all", function (done) {
        var worker = new Worker("~/tests/esmEntrySyncWorker.mjs");
        worker.onmessage = function (msg) {
            expect(msg.data).toBe("esm-entry:ping");
            worker.terminate();
            done();
        };
        worker.postMessage("ping");
    });

    it("runs an ES module worker entry whose top-level await parks past the yield window",
       function (done) {
        // The park is non-nestable, so the in-place window cannot settle it:
        // the entry finishes from the real event loop afterwards, and the
        // message queue enables on settle rather than being lost.
        var worker = new Worker("~/tests/esmEntryTlaWorker.mjs");
        worker.onmessage = function (msg) {
            expect(msg.data).toBe("tla-entry:ok:ping");
            worker.terminate();
            done();
        };
        worker.postMessage("ping");
    });

    it("runs an ES module worker entry whose top-level await parks on a JS timer",
       function (done) {
        var worker = new Worker("~/tests/esmEntryTimerWorker.mjs");
        worker.onmessage = function (msg) {
            expect(msg.data).toBe("timer-entry:ok:ping");
            worker.terminate();
            done();
        };
        worker.postMessage("ping");
    });

    it("runs an ES module worker entry spawned through a relative path", function (done) {
        var worker = new Worker("./esmEntryRelativeWorker.mjs");
        worker.onmessage = function (msg) {
            expect(msg.data).toBe("relative-entry:ping");
            worker.terminate();
            done();
        };
        worker.postMessage("ping");
    });

    // An http(s) worker specifier bypasses the filesystem check entirely: the
    // entry is fetched, compiled and registered under its canonical URL key on
    // the worker's own thread, which is also the key the settle gate probes.
    it("runs a worker whose entry is an http URL", function (done) {
        var origin = "http://127.0.0.1:" + com.tns.tests.ModuleTestServer.ensureStarted();
        var worker = new Worker(origin + "/esm/worker-entry.mjs");
        worker.onmessage = function (msg) {
            expect(msg.data).toBe("http-worker-entry:ping");
            worker.terminate();
            done();
        };
        worker.postMessage("ping");
    });

    // Extension resolution tries `.js` before `.mjs`, and no `.js` sibling
    // exists, so the ES module entry is what answers. Its top-level await also
    // parks past the yield window, so the message posted here proves the
    // settle-gated queue engages on a resolved specifier too.
    it("runs an extension-less ES module worker entry past its top-level await",
       function (done) {
        var worker = new Worker("./esmEntryResolvedWorker");
        worker.onmessage = function (msg) {
            expect(msg.data).toBe("resolved-entry:ok:ping");
            worker.terminate();
            done();
        };
        worker.postMessage("ping");
    });

    // The worker isolate is published before its entry runs, so terminate()
    // can interrupt an entry that is still evaluating - here one parked on a
    // promise that never settles, inside the pump that waits for it.
    it("survives terminate() while an entry is parked in top-level await", function (done) {
        var ITERATIONS = 3;
        var FALLBACK_TERMINATE = 700;
        var SETTLE_AFTER = 700;
        var errors = [];

        function iteration(remaining) {
            if (remaining === 0) {
                expect(errors).toEqual([]);
                // A worker spawned after the terminated ones still works.
                var next = new Worker("~/tests/esmEntrySyncWorker.mjs");
                next.onmessage = function (msg) {
                    expect(msg.data).toBe("esm-entry:ping");
                    next.terminate();
                    done();
                };
                next.postMessage("ping");
                return;
            }

            var worker = new Worker("./esmEntryNeverSettlesWorker.mjs");
            var terminated = false;

            function terminateOnce() {
                if (terminated) {
                    return;
                }
                terminated = true;
                worker.terminate();
                setTimeout(function () {
                    iteration(remaining - 1);
                }, SETTLE_AFTER);
            }

            worker.onerror = function (e) {
                errors.push(String((e && e.message) || e));
            };
            worker.onmessage = function (msg) {
                expect(msg.data).toBe("never-settles:started");
                terminateOnce();
            };
            // The evaluation pump is bounded, so a start message that never
            // arrives must not push the terminate past the window it targets.
            setTimeout(terminateOnce, FALLBACK_TERMINATE);
        }

        iteration(ITERATIONS);
    });

    // WHATWG parity: the worker's message queue is enabled when its entry
    // script finishes evaluating, and from then on messages dispatch whether
    // or not a handler exists. A handler registered later (from a timer)
    // misses messages delivered in between — exactly as on the web.
    it("drops messages dispatched before a late-registered onmessage, like the web", function (done) {
        var worker = new Worker("./lateHandlerWorker.js");
        var received = [];
        worker.onmessage = function (msg) {
            received.push(msg.data);
            if (msg.data === "ready") {
                worker.postMessage("second");
            } else {
                expect(received).toEqual(["ready", "late:second"]);
                worker.terminate();
                done();
            }
        };
        // Posted before the entry finishes evaluating: buffered, then
        // dispatched into a global with no handler yet — dropped.
        worker.postMessage("early");
    });
});

// A worker inherits a copy of its parent's loader vocabulary, taken on the
// parent's thread as the worker is constructed and installed before the worker
// loads any module.
describe("worker loader-vocabulary inheritance", function () {
    var originalTimeout;

    function setLeaf(target) {
        require("ns:module").configureLoader({
            importMap: { imports: { "ns-worker-leaf": target } },
        });
    }

    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 15000;
    });

    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
        // The map is isolate-wide state, so it must not outlive this describe.
        require("ns:module").configureLoader({ importMap: { imports: {} } });
    });

    it("gives a worker spawned after configureLoader the parent's map", function (done) {
        setLeaf("~/esm/vocab/leafA.mjs");

        var worker = new Worker("./importMapWorker.js");
        worker.onmessage = function (msg) {
            expect(msg.data.ok ? "resolved" : "failed: " + msg.data.error).toBe("resolved");
            expect(msg.data.name).toBe("vocab-a");
            worker.terminate();
            done();
        };
        worker.postMessage("ns-worker-leaf");
    });

    it("leaves a running worker on the map it was spawned with", function (done) {
        setLeaf("~/esm/vocab/leafB.mjs");

        var worker = new Worker("./importMapWorker.js");
        worker.onmessage = function (msg) {
            expect(msg.data.ok ? "resolved" : "failed: " + msg.data.error).toBe("resolved");
            expect(msg.data.name).toBe("vocab-b");
            worker.terminate();
            done();
        };

        // Reconfigure the parent only after the worker exists, then ask it to
        // resolve: the worker answers from the copy taken at its spawn.
        setLeaf("~/esm/vocab/leafC.mjs");
        worker.postMessage("ns-worker-leaf");
    });

    it("still applies a later configureLoader on the parent's own isolate", function (done) {
        setLeaf("~/esm/vocab/leafC.mjs");
        import("ns-worker-leaf").then(function (mod) {
            expect(mod.name).toBe("vocab-c");
            done();
        }).catch(function (error) {
            expect("rejected: " + String((error && error.message) || error)).toBe("resolved");
            done();
        });
    });
});

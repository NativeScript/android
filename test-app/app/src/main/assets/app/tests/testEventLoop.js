// V8 delivers these resolutions as platform foreground tasks, so they only
// settle if the runtime pumps its foreground task runner (EventLoopHandler).
describe("event loop foreground tasks", function () {
    it("resolves Atomics.waitAsync when notified on the same thread", function (done) {
        const sab = new SharedArrayBuffer(4);
        const i32 = new Int32Array(sab);

        const result = Atomics.waitAsync(i32, 0, 0);
        expect(result.async).toBe(true);

        result.value.then(value => {
            expect(value).toBe("ok");
            done();
        }).catch(e => {
            done.fail("Atomics.waitAsync promise rejected: " + e);
        });

        const woken = Atomics.notify(i32, 0);
        expect(woken).toBe(1);
    });

    it("resolves Atomics.waitAsync with 'timed-out' after the timeout", function (done) {
        const sab = new SharedArrayBuffer(4);
        const i32 = new Int32Array(sab);

        const result = Atomics.waitAsync(i32, 0, 0, 50);
        expect(result.async).toBe(true);

        result.value.then(value => {
            expect(value).toBe("timed-out");
            done();
        }).catch(e => {
            done.fail("Atomics.waitAsync promise rejected: " + e);
        });
    });

    it("resolves Atomics.waitAsync synchronously on value mismatch", function () {
        const sab = new SharedArrayBuffer(4);
        const i32 = new Int32Array(sab);
        i32[0] = 42;

        const result = Atomics.waitAsync(i32, 0, 0);
        expect(result.async).toBe(false);
        expect(result.value).toBe("not-equal");
    });

    it("keeps ordinary promise chains working alongside foreground tasks", function (done) {
        const sab = new SharedArrayBuffer(4);
        const i32 = new Int32Array(sab);
        const order = [];

        Atomics.waitAsync(i32, 0, 0).value.then(() => {
            order.push("waitAsync");
            return Promise.resolve();
        }).then(() => {
            order.push("chained");
            expect(order).toEqual(["waitAsync", "chained"]);
            done();
        }).catch(e => {
            done.fail("promise chain failed: " + e);
        });

        Atomics.notify(i32, 0);
    });
});

// The ordered lane rides the Java MessageQueue, so these callbacks must be
// strict macrotasks: after the current turn's microtasks, FIFO with timers.
describe("event loop ordered macrotasks", function () {
    it("__ns__queueMacrotask runs the callback asynchronously", function (done) {
        let ran = false;
        __ns__queueMacrotask(() => {
            ran = true;
            done();
        });
        expect(ran).toBe(false);
    });

    it("runs after the current turn's microtasks", function (done) {
        const order = [];
        __ns__queueMacrotask(() => {
            order.push("macrotask");
            expect(order).toEqual(["microtask", "macrotask"]);
            done();
        });
        Promise.resolve().then(() => order.push("microtask"));
    });

    // native timers (__ns__*): the app-level `setTimeout` global in this test
    // app is an old Handler-based polyfill, not the runtime timers
    it("stays FIFO-ordered with native setTimeout(0)", function (done) {
        const order = [];
        __ns__queueMacrotask(() => order.push("macro1"));
        __ns__setTimeout(() => order.push("timeout"), 0);
        __ns__queueMacrotask(() => {
            order.push("macro2");
            expect(order).toEqual(["macro1", "timeout", "macro2"]);
            done();
        });
    });

    it("rejects non-function arguments", function () {
        expect(() => __ns__queueMacrotask("nope")).toThrowError(TypeError);
        expect(() => __ns__queueMacrotask()).toThrowError(TypeError);
    });

    it("runs on the main thread when posted from a background JS thread", function (done) {
        const mainThreadId = java.lang.Thread.currentThread().getId();
        new java.lang.Thread(new java.lang.Runnable({
            run() {
                expect(java.lang.Thread.currentThread().getId()).not.toEqual(mainThreadId);
                __ns__queueMacrotask(() => {
                    expect(java.lang.Thread.currentThread().getId()).toEqual(mainThreadId);
                    done();
                });
            }
        })).start();
    });
});

// clearTimeout leaves a tombstone in the merged ordered domain, so the
// cleared timer's already-queued token consumes its own slot as a no-op
// instead of running a later-scheduled item ahead of Java messages queued
// between the two tokens' positions.
describe("event loop ordered tombstones", function () {
    it("cleared timeout's token does not run a later timer ahead of java posts", function (done) {
        const order = [];
        const handler = new android.os.Handler(android.os.Looper.myLooper());
        const t1 = __ns__setTimeout(() => order.push("cleared"), 0);
        __ns__clearTimeout(t1);
        handler.post(new java.lang.Runnable({
            run: () => order.push("java")
        }));
        __ns__setTimeout(() => {
            order.push("t2");
            expect(order).toEqual(["java", "t2"]);
            done();
        }, 0);
    });

    it("cleared timeout's token does not run a queued macrotask ahead of java posts", function (done) {
        const order = [];
        const handler = new android.os.Handler(android.os.Looper.myLooper());
        const t1 = __ns__setTimeout(() => order.push("cleared"), 0);
        __ns__clearTimeout(t1);
        handler.post(new java.lang.Runnable({
            run: () => order.push("java")
        }));
        __ns__queueMacrotask(() => {
            order.push("macro");
            expect(order).toEqual(["java", "macro"]);
            done();
        });
    });
});

// Long (>=32ms) timers carry an identified token whose clear removes the
// queued wakeup; short timers carry a native claim cell whose clear is a
// single CAS. Both must keep exact clear semantics under any thread.
describe("event loop token cancellation", function () {
    it("cleared identified (long) timeout never fires and later timers are unaffected", function (done) {
        let fired = false;
        const t = __ns__setTimeout(() => { fired = true; }, 100);
        __ns__clearTimeout(t);
        __ns__setTimeout(() => {
            expect(fired).toBe(false);
            done();
        }, 150);
    });

    it("background-thread clear racing dispatch neither jumps java posts nor ghost-fires", function (done) {
        let remaining = 30;
        (function iter() {
            const order = [];
            const handler = new android.os.Handler(android.os.Looper.myLooper());
            const t1 = __ns__setTimeout(() => order.push("t1"), 0);
            new java.lang.Thread(new java.lang.Runnable({
                run() {
                    __ns__clearTimeout(t1);
                }
            })).start();
            handler.post(new java.lang.Runnable({
                run: () => order.push("java")
            }));
            __ns__setTimeout(() => {
                order.push("t2");
                const observed = order.join(">");
                // t1 either fired before the clear landed (at its own legal
                // slot, ahead of "java") or never; t2 must never jump "java"
                expect(observed === "java>t2" || observed === "t1>java>t2").toBe(true);
                if (--remaining === 0) {
                    done();
                } else {
                    iter();
                }
            }, 5);
        })();
    });

    it("clearing an identified interval stops it", function (done) {
        let ticks = 0;
        const iv = __ns__setInterval(() => {
            ticks++;
            if (ticks === 2) {
                __ns__clearInterval(iv);
                __ns__setTimeout(() => {
                    expect(ticks).toBe(2);
                    done();
                }, 120);
            }
        }, 40);
    });
});

describe("event loop internal lane", function () {
    // Regression for the eventfd unit-accounting bug: a worker reply's wakeup
    // arriving while an overdue waitAsync timeout is still unsignaled must not
    // be spent on the timeout entry, or the reply starves.
    it("delivers worker messages whose wakeup raced an overdue waitAsync timeout", function (done) {
        const worker = new Worker("./eventLoopEchoWorker.js");
        let warm = false;
        worker.onmessage = function (msg) {
            if (msg.data === "warmup") {
                warm = true;
                const i32 = new Int32Array(new SharedArrayBuffer(4));
                Atomics.waitAsync(i32, 0, 0, 50);
                worker.postMessage("ping");
                // block the looper until both the timeout and the reply are
                // pending, so their wakeups are serviced from the same poll
                const start = Date.now();
                while (Date.now() - start < 150) { }
            } else {
                expect(warm).toBe(true);
                expect(msg.data).toBe("ping");
                worker.terminate();
                done();
            }
        };
        worker.postMessage("warmup");
    });

    it("keeps event loops healthy across worker churn", function (done) {
        let remaining = 8;
        (function cycle() {
            const worker = new Worker("./eventLoopEchoWorker.js");
            worker.onmessage = function () {
                worker.terminate();
                if (--remaining === 0) {
                    __ns__queueMacrotask(done);
                } else {
                    cycle();
                }
            };
            worker.postMessage("alive");
        })();
    });
});

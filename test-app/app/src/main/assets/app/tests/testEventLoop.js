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

    it("stays FIFO-ordered with setTimeout(0)", function (done) {
        const order = [];
        __ns__queueMacrotask(() => order.push("macro1"));
        setTimeout(() => order.push("timeout"), 0);
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
});

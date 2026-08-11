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
        });

        Atomics.notify(i32, 0);
    });
});

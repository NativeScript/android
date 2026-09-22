// The addon is compiled into libNativeScript.so for local Debug builds only;
// on any other runtime flavor the suite skips rather than fails.
var napiFastModuleAvailable = true;
try {
    require("napifastcallsmodule");
} catch (e) {
    napiFastModuleAvailable = false;
}

(napiFastModuleAvailable ? describe : xdescribe)("Node-API fast calls", function () {
    var napi = require("napifastcallsmodule");

    beforeEach(function () {
        napi.resetCounts();
    });

    // V8 picks the path; everything below that is not explicitly about which
    // path ran must hold either way.
    function hammer(fn, iterations) {
        var last;
        for (var i = 0; i < iterations; i++) {
            last = fn(i);
        }
        return last;
    }

    it("reports fast calls as available on this runtime", function () {
        // Android runs V8 with a JIT unless the app asks for --jitless.
        expect(napi.fastCallsAvailable()).toBe(true);
    });

    it("exports both-path functions", function () {
        expect(typeof napi.addInt32).toBe("function");
        expect(typeof napi.scale).toBe("function");
        expect(typeof napi.divide).toBe("function");
        expect(typeof napi.byteLength).toBe("function");
        expect(typeof napi.clamped).toBe("function");
        expect(typeof napi.slowOnlyAddInt32).toBe("function");
    });

    describe("semantics", function () {
        it("adds int32s", function () {
            expect(napi.addInt32(1, 2)).toBe(3);
            expect(napi.addInt32(-5, 5)).toBe(0);
            expect(napi.addInt32(2147483647, 0)).toBe(2147483647);
        });

        it("scales doubles", function () {
            expect(napi.scale(2)).toBe(5);
            expect(napi.scale(0.5)).toBe(1.25);
            expect(napi.scale(-1)).toBe(-2.5);
        });

        it("measures one-byte strings", function () {
            expect(napi.byteLength("")).toBe(0);
            expect(napi.byteLength("hello")).toBe(5);
        });

        it("agrees on two-byte strings, which never take the fast path", function () {
            // V8 declines the fast path for a two-byte string, so this is the
            // slow path's answer and it has to match the fast one's contract:
            // the number of UTF-8 bytes.
            expect(napi.byteLength("snowman ☃")).toBe(11);
        });

        it("applies the clamp flag to a uint8 parameter", function () {
            expect(napi.clamped(12)).toBe(12);
            expect(napi.clamped(-40)).toBe(0);
            expect(napi.clamped(4000)).toBe(255);
        });

        it("behaves like a plain function when registered slow-only", function () {
            expect(napi.slowOnlyAddInt32(20, 22)).toBe(42);
            hammer(function (i) { return napi.slowOnlyAddInt32(i, 1); }, 2000);
            // No fast function was registered, so no call can ever be fast.
            expect(napi.fastCallCount()).toBe(0);
            expect(napi.slowCallCount()).toBeGreaterThan(0);
        });

        it("cannot be used as a constructor", function () {
            expect(function () { return new napi.addInt32(1, 2); }).toThrow();
        });
    });

    describe("path selection", function () {
        it("runs the slow path for a cold call", function () {
            expect(napi.addInt32(1, 2)).toBe(3);
            expect(napi.slowCallCount()).toBe(1);
            expect(napi.fastCallCount()).toBe(0);
        });

        it("runs the fast path once the caller tiers up", function () {
            // A trivial monomorphic caller is exactly what TurboFan optimizes
            // first; the fast call is emitted only from optimized code.
            var sum = 0;
            function hot(i) {
                return napi.addInt32(i, 1);
            }
            for (var i = 0; i < 300000; i++) {
                sum += hot(i);
            }

            expect(sum).toBeGreaterThan(0);
            expect(napi.fastCallCount()).toBeGreaterThan(0);
            // Every call is accounted for on exactly one of the two paths.
            expect(napi.fastCallCount() + napi.slowCallCount()).toBe(300000);
        });

        it("produces identical results on both paths", function () {
            var cold = napi.scale(3);

            function hot(i) {
                return napi.scale(3);
            }
            for (var i = 0; i < 300000; i++) {
                hot(i);
            }

            expect(napi.fastCallCount()).toBeGreaterThan(0);
            expect(hot(0)).toBe(cold);
        });
    });

    describe("fallible fast functions", function () {
        it("divides on both paths", function () {
            expect(napi.divide(10, 4)).toBe(2.5);

            function hot(i) {
                return napi.divide(10, 4);
            }
            for (var i = 0; i < 300000; i++) {
                hot(i);
            }
            expect(napi.fastCallCount()).toBeGreaterThan(0);
            expect(napi.divide(10, 4)).toBe(2.5);
        });

        it("throws a catchable RangeError from the slow path", function () {
            var error;
            try {
                napi.divide(1, 0);
            } catch (e) {
                error = e;
            }

            expect(error instanceof RangeError).toBe(true);
            expect(error.message).toBe("division by zero");
            expect(error.code).toBe("ERR_DIV_ZERO");
            expect(napi.slowCallCount()).toBe(1);
        });

        it("throws from inside the fast path, through the shim", function () {
            // Which path serves any individual call is V8's choice, so the
            // throwing calls are mixed into the warm-up loop rather than made
            // once at the end: the addon counts the throws it raises from the
            // fast function itself, which is the only way to tell that the
            // shim ran under optimized code at all.
            function hot(divisor) {
                return napi.divide(1, divisor);
            }

            var caught = 0;
            var results = 0;
            for (var i = 0; i < 300000; i++) {
                try {
                    results += hot(i % 1000 === 0 ? 0 : 2);
                } catch (e) {
                    caught++;
                    if (!(e instanceof RangeError) ||
                        e.message !== "division by zero" ||
                        e.code !== "ERR_DIV_ZERO") {
                        throw e;
                    }
                }
            }

            expect(caught).toBe(300);
            expect(results).toBeGreaterThan(0);
            expect(napi.fastCallCount()).toBeGreaterThan(0);
            // The throw shim opened a handle scope and threw while V8 was in
            // optimized code, and the caller kept running afterwards.
            expect(napi.fastThrowCount()).toBeGreaterThan(0);
        });

        it("keeps working after a throw", function () {
            try {
                napi.divide(1, 0);
            } catch (e) {
                // ignored
            }
            expect(napi.divide(9, 3)).toBe(3);
        });
    });
});

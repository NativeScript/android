describe("AbortController / AbortSignal", function () {
    it("controller exposes a stable, initially-live signal", function () {
        const controller = new AbortController();
        const signal = controller.signal;
        expect(signal instanceof AbortSignal).toBe(true);
        expect(signal instanceof EventTarget).toBe(true);
        expect(controller.signal).toBe(signal);
        expect(signal.aborted).toBe(false);
        expect(signal.reason).toBeUndefined();
        expect(signal.onabort).toBeNull();
    });

    it("AbortSignal constructor is not user-invocable", function () {
        expect(function () { new AbortSignal(); }).toThrowError(TypeError);
        expect(function () { new AbortSignal({}); }).toThrowError(TypeError);
    });

    it("abort() flips state before firing a single abort event", function () {
        const controller = new AbortController();
        const signal = controller.signal;
        const seen = [];
        signal.addEventListener("abort", function (event) {
            seen.push({
                type: event.type,
                target: event.target,
                aborted: signal.aborted,
                reasonName: signal.reason && signal.reason.name,
            });
        });
        controller.abort();
        controller.abort(); // second abort is a no-op
        expect(seen.length).toBe(1);
        expect(seen[0].type).toBe("abort");
        expect(seen[0].target).toBe(signal);
        expect(seen[0].aborted).toBe(true);
        expect(seen[0].reasonName).toBe("AbortError");
        expect(signal.aborted).toBe(true);
        expect(signal.reason instanceof Error).toBe(true);
        expect(signal.reason.name).toBe("AbortError");
    });

    it("abort(reason) keeps the given reason by identity, including null", function () {
        const custom = { my: "reason" };
        const c1 = new AbortController();
        c1.abort(custom);
        expect(c1.signal.reason).toBe(custom);

        const c2 = new AbortController();
        c2.abort(null);
        expect(c2.signal.aborted).toBe(true);
        expect(c2.signal.reason).toBeNull();
    });

    it("throwIfAborted throws the exact reason once aborted", function () {
        const controller = new AbortController();
        expect(function () { controller.signal.throwIfAborted(); }).not.toThrow();
        const reason = new Error("boom");
        controller.abort(reason);
        try {
            controller.signal.throwIfAborted();
            fail("expected throwIfAborted to throw");
        } catch (e) {
            expect(e).toBe(reason);
        }
    });

    it("listeners added after abort never fire; once listeners fire once", function () {
        const controller = new AbortController();
        const signal = controller.signal;
        let onceCalls = 0;
        signal.addEventListener("abort", function () { onceCalls++; }, { once: true });
        controller.abort();
        expect(onceCalls).toBe(1);

        let lateCalls = 0;
        signal.addEventListener("abort", function () { lateCalls++; });
        controller.abort();
        expect(lateCalls).toBe(0);
    });

    it("onabort follows event handler semantics (set / replace / clear)", function () {
        const controller = new AbortController();
        const signal = controller.signal;
        const calls = [];
        const first = function () { calls.push("first"); };
        const second = function (event) {
            calls.push("second");
            expect(this).toBe(signal);
            expect(event.type).toBe("abort");
        };
        signal.onabort = first;
        expect(signal.onabort).toBe(first);
        signal.onabort = second; // replacement keeps a single registration
        signal.onabort = 42; // primitives clear the handler
        expect(signal.onabort).toBeNull();
        signal.onabort = second;
        controller.abort();
        expect(calls).toEqual(["second"]);
    });

    it("a cleared onabort does not fire", function () {
        const controller = new AbortController();
        let called = false;
        controller.signal.onabort = function () { called = true; };
        controller.signal.onabort = null;
        controller.abort();
        expect(called).toBe(false);
    });

    it("a throwing abort listener does not stop the remaining listeners", function () {
        const previousHook = global.__onUncaughtError;
        const uncaught = [];
        global.__onUncaughtError = function (error) { uncaught.push(error); };
        try {
            const controller = new AbortController();
            const seen = [];
            controller.signal.addEventListener("abort", function () {
                seen.push("thrower");
                throw new Error("listener boom");
            });
            controller.signal.addEventListener("abort", function () {
                seen.push("survivor");
            });
            controller.abort();
            expect(seen).toEqual(["thrower", "survivor"]);
        } finally {
            global.__onUncaughtError = previousHook;
        }
    });

    it("AbortSignal.abort() returns a pre-aborted signal", function () {
        const signal = AbortSignal.abort();
        expect(signal instanceof AbortSignal).toBe(true);
        expect(signal.aborted).toBe(true);
        expect(signal.reason.name).toBe("AbortError");

        const custom = new Error("custom");
        expect(AbortSignal.abort(custom).reason).toBe(custom);
    });

    it("AbortSignal.timeout aborts asynchronously with a TimeoutError", function (done) {
        const signal = AbortSignal.timeout(10);
        expect(signal.aborted).toBe(false);
        signal.addEventListener("abort", function () {
            expect(signal.aborted).toBe(true);
            expect(signal.reason instanceof Error).toBe(true);
            expect(signal.reason.name).toBe("TimeoutError");
            done();
        });
    });

    it("AbortSignal.timeout validates the delay", function () {
        expect(function () { AbortSignal.timeout("10"); }).toThrowError(TypeError);
        expect(function () { AbortSignal.timeout(); }).toThrowError(TypeError);
        expect(function () { AbortSignal.timeout(-1); }).toThrowError(RangeError);
        expect(function () { AbortSignal.timeout(1.5); }).toThrowError(RangeError);
        expect(function () { AbortSignal.timeout(NaN); }).toThrowError(RangeError);
        expect(function () { AbortSignal.timeout(4294967296); }).toThrowError(RangeError);
    });

    it("AbortSignal.any([]) never aborts", function () {
        const signal = AbortSignal.any([]);
        expect(signal instanceof AbortSignal).toBe(true);
        expect(signal.aborted).toBe(false);
    });

    it("AbortSignal.any rejects non-iterables and non-signals", function () {
        expect(function () { AbortSignal.any(null); }).toThrowError(TypeError);
        expect(function () { AbortSignal.any(undefined); }).toThrowError(TypeError);
        expect(function () { AbortSignal.any(5); }).toThrowError(TypeError);
        expect(function () { AbortSignal.any([{}]); }).toThrowError(TypeError);
        expect(function () { AbortSignal.any([null]); }).toThrowError(TypeError);
    });

    it("AbortSignal.any returns an aborted signal when an input is already aborted", function () {
        const live = new AbortController();
        const reason = new Error("already done");
        const combined = AbortSignal.any([live.signal, AbortSignal.abort(reason)]);
        expect(combined.aborted).toBe(true);
        expect(combined.reason).toBe(reason);
    });

    it("AbortSignal.any adopts the first aborting source's reason and fires once", function () {
        const a = new AbortController();
        const b = new AbortController();
        const combined = AbortSignal.any([a.signal, b.signal]);
        expect(combined.aborted).toBe(false);
        let fires = 0;
        combined.addEventListener("abort", function () { fires++; });

        const reason = new Error("b first");
        b.abort(reason);
        expect(combined.aborted).toBe(true);
        expect(combined.reason).toBe(reason);
        expect(fires).toBe(1);

        a.abort(); // the other source aborting later must not re-fire
        expect(combined.reason).toBe(reason);
        expect(fires).toBe(1);
    });

    it("every affected signal flips state before any abort event fires", function () {
        const controller = new AbortController();
        const combined = AbortSignal.any([controller.signal]);
        const observed = [];
        controller.signal.addEventListener("abort", function () {
            observed.push(["source", combined.aborted]);
        });
        combined.addEventListener("abort", function () {
            observed.push(["combined", controller.signal.aborted]);
        });
        controller.abort();
        expect(observed).toEqual([["source", true], ["combined", true]]);
    });

    it("AbortSignal.any flattens composite inputs to their sources", function () {
        const a = new AbortController();
        const b = new AbortController();
        const inner = AbortSignal.any([a.signal]);
        const outer = AbortSignal.any([inner, b.signal]);
        const reason = new Error("via inner");
        a.abort(reason);
        expect(inner.aborted).toBe(true);
        expect(outer.aborted).toBe(true);
        expect(outer.reason).toBe(reason);
    });

    it("AbortSignal.any accepts any iterable of signals", function () {
        const controller = new AbortController();
        const combined = AbortSignal.any(new Set([controller.signal]));
        controller.abort();
        expect(combined.aborted).toBe(true);
    });

    it("interfaces carry WebIDL toStringTag and enumerable members", function () {
        const controller = new AbortController();
        expect(Object.prototype.toString.call(controller)).toBe("[object AbortController]");
        expect(Object.prototype.toString.call(controller.signal)).toBe("[object AbortSignal]");
        expect(Object.getOwnPropertyDescriptor(AbortSignal.prototype, "aborted").enumerable).toBe(true);
        expect(Object.getOwnPropertyDescriptor(AbortController.prototype, "abort").enumerable).toBe(true);
        expect(Object.keys(controller.signal)).toEqual([]);
    });

    it("accessors brand-check their receiver", function () {
        const abortedGetter = Object.getOwnPropertyDescriptor(AbortSignal.prototype, "aborted").get;
        expect(function () { abortedGetter.call({}); }).toThrowError(TypeError);
        const signalGetter = Object.getOwnPropertyDescriptor(AbortController.prototype, "signal").get;
        expect(function () { signalGetter.call({}); }).toThrowError(TypeError);
    });
});

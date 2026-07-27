describe("WHATWG error events", function () {
    // Many tests exercise the global error path, which ends in the
    // __onUncaughtError / __onDiscardedError hooks when a listener does not
    // preventDefault(). Install spies for every test and restore the previous
    // hooks in afterEach. Also track listeners added on the global target so
    // they never leak into other suites (the internal EventTarget backing the
    // global is process-wide).
    var previousUncaughtHook;
    var previousDiscardedHook;
    var uncaught;
    var discarded;
    var addedGlobalListeners;

    beforeEach(function () {
        previousUncaughtHook = global.__onUncaughtError;
        previousDiscardedHook = global.__onDiscardedError;
        uncaught = [];
        discarded = [];
        global.__onUncaughtError = function (error) {
            uncaught.push(error);
        };
        global.__onDiscardedError = function (error) {
            discarded.push(error);
        };
        addedGlobalListeners = [];
    });

    afterEach(function () {
        global.__onUncaughtError = previousUncaughtHook;
        global.__onDiscardedError = previousDiscardedHook;
        for (var i = 0; i < addedGlobalListeners.length; i++) {
            var l = addedGlobalListeners[i];
            global.removeEventListener(l.type, l.handler);
        }
        addedGlobalListeners = [];
    });

    function onGlobal(type, handler) {
        global.addEventListener(type, handler);
        addedGlobalListeners.push({ type: type, handler: handler });
    }

    // Wait a couple of quiet looper turns before asserting a NON-event.
    function afterQuietTurns(cb) {
        setTimeout(function () {
            setTimeout(cb, 25);
        }, 25);
    }

    it("reportError fires an 'error' listener with an ErrorEvent carrying error and message", function (done) {
        var err = new Error("x");
        var received = null;
        onGlobal("error", function (e) {
            received = e;
            e.preventDefault();
        });

        global.reportError(err);

        expect(received).not.toBeNull();
        expect(received instanceof ErrorEvent).toBe(true);
        expect(received.type).toBe("error");
        expect(received.error).toBe(err);
        expect(received.message).toBe("x");
        // preventDefault() in the listener must suppress the __onUncaughtError hook.
        afterQuietTurns(function () {
            expect(uncaught.length).toBe(0);
            done();
        });
    });

    it("reportError without preventDefault still invokes __onUncaughtError (back-compat)", function () {
        var err = new Error("back-compat");
        var received = null;
        onGlobal("error", function (e) {
            received = e;
        });

        global.reportError(err);

        expect(received).not.toBeNull();
        expect(received.error).toBe(err);
        expect(uncaught.length).toBe(1);
        expect(uncaught[0]).toBe(err);
    });

    it("reportError throws TypeError when called with no arguments", function () {
        expect(function () {
            global.reportError();
        }).toThrowError(TypeError);
    });

    it("a discarded Java exception dispatches an 'error' event before __onDiscardedError", function () {
        var received = null;
        onGlobal("error", function (e) {
            received = e;
        });

        var test = new com.tns.tests.DiscardedExceptionTest();
        test.reportSupressedException();

        expect(received).not.toBeNull();
        expect(received instanceof ErrorEvent).toBe(true);
        expect(received.error).not.toBeNull();
        expect(received.error.message).toBe("Exception to suppress");
        // Unprevented, so the existing hook still fires (back-compat).
        expect(discarded.length).toBe(1);
        expect(discarded[0]).toBe(received.error);
    });

    it("preventDefault() on the 'error' event suppresses __onDiscardedError", function () {
        var received = null;
        onGlobal("error", function (e) {
            received = e;
            e.preventDefault();
        });

        var test = new com.tns.tests.DiscardedExceptionTest();
        test.reportSupressedException();

        expect(received).not.toBeNull();
        expect(discarded.length).toBe(0);
    });

    describe("constructors and EventTarget semantics", function () {
        it("Event is spec-sane and cancelable via preventDefault", function () {
            var e = new Event("x", { cancelable: true });
            expect(e.type).toBe("x");
            expect(e.cancelable).toBe(true);
            expect(e.bubbles).toBe(false);
            expect(e.defaultPrevented).toBe(false);
            e.preventDefault();
            expect(e.defaultPrevented).toBe(true);
        });

        it("a non-cancelable Event ignores preventDefault", function () {
            var e = new Event("x");
            e.preventDefault();
            expect(e.defaultPrevented).toBe(false);
        });

        it("ErrorEvent exposes message/error/filename/lineno/colno", function () {
            var err = new Error("boom");
            var e = new ErrorEvent("error", { message: "m", error: err });
            expect(e instanceof Event).toBe(true);
            expect(e.message).toBe("m");
            expect(e.error).toBe(err);
            expect(e.filename).toBe("");
            expect(e.lineno).toBe(0);
            expect(e.colno).toBe(0);
        });

        it("PromiseRejectionEvent exposes promise/reason", function () {
            var p = Promise.reject(1);
            p.catch(function () {});
            var r = { some: "reason" };
            var e = new PromiseRejectionEvent("unhandledrejection", { promise: p, reason: r });
            expect(e instanceof Event).toBe(true);
            expect(e.promise).toBe(p);
            expect(e.reason).toBe(r);
        });

        it("dispatchEvent returns !defaultPrevented", function () {
            var target = new EventTarget();
            target.addEventListener("t", function (e) { e.preventDefault(); });
            expect(target.dispatchEvent(new Event("t", { cancelable: true }))).toBe(false);

            var target2 = new EventTarget();
            target2.addEventListener("t", function () {});
            expect(target2.dispatchEvent(new Event("t", { cancelable: true }))).toBe(true);
        });

        it("once:true listener fires exactly once", function () {
            var target = new EventTarget();
            var count = 0;
            target.addEventListener("t", function () { count++; }, { once: true });
            target.dispatchEvent(new Event("t"));
            target.dispatchEvent(new Event("t"));
            expect(count).toBe(1);
        });

        it("removeEventListener stops future dispatches", function () {
            var target = new EventTarget();
            var count = 0;
            var handler = function () { count++; };
            target.addEventListener("t", handler);
            target.dispatchEvent(new Event("t"));
            target.removeEventListener("t", handler);
            target.dispatchEvent(new Event("t"));
            expect(count).toBe(1);
        });

        it("listeners run in registration order", function () {
            var target = new EventTarget();
            var order = [];
            target.addEventListener("t", function () { order.push(1); });
            target.addEventListener("t", function () { order.push(2); });
            target.addEventListener("t", function () { order.push(3); });
            target.dispatchEvent(new Event("t"));
            expect(order).toEqual([1, 2, 3]);
        });

        it("stopImmediatePropagation stops remaining listeners", function () {
            var target = new EventTarget();
            var order = [];
            target.addEventListener("t", function (e) { order.push(1); e.stopImmediatePropagation(); });
            target.addEventListener("t", function () { order.push(2); });
            target.dispatchEvent(new Event("t"));
            expect(order).toEqual([1]);
        });

        it("a throwing listener does not stop later listeners", function () {
            var target = new EventTarget();
            var order = [];
            target.addEventListener("t", function () { order.push(1); throw new Error("listener boom"); });
            target.addEventListener("t", function () { order.push(2); });
            target.dispatchEvent(new Event("t"));
            expect(order).toEqual([1, 2]);
        });
    });

    it("reportError still fires listeners after globalThis.dispatchEvent is overwritten", function (done) {
        var err = new Error("resilient");
        var received = null;
        onGlobal("error", function (e) {
            received = e;
            e.preventDefault();
        });

        var originalDispatch = globalThis.dispatchEvent;
        globalThis.dispatchEvent = function () { return true; };
        try {
            global.reportError(err);
        } finally {
            globalThis.dispatchEvent = originalDispatch;
        }

        expect(received).not.toBeNull();
        expect(received.error).toBe(err);
        afterQuietTurns(function () {
            expect(uncaught.length).toBe(0);
            done();
        });
    });
});

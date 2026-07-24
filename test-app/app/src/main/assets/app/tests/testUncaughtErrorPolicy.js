describe("uncaughtErrorPolicy (default: report)", function () {
    // Under the default "report" policy, an uncaught JS throw in a
    // native-initiated callback is contained: reported through the `error`
    // event and the __onUncaughtError hook, while the native caller resumes
    // with a default value and the app keeps running. JS-initiated chains
    // (JS -> Java -> JS) keep propagating to the outer JS catch.
    var previousUncaughtHook;
    var uncaught;
    var addedGlobalListeners;

    beforeEach(function () {
        previousUncaughtHook = global.__onUncaughtError;
        uncaught = [];
        global.__onUncaughtError = function (error) {
            uncaught.push(error);
        };
        addedGlobalListeners = [];
    });

    afterEach(function () {
        global.__onUncaughtError = previousUncaughtHook;
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

    function uncaughtSeen(err) {
        return uncaught.indexOf(err) !== -1;
    }

    function pollUntil(predicate, cb) {
        var turns = 0;
        (function poll() {
            if (predicate() || turns >= 25) {
                cb();
                return;
            }
            turns++;
            setTimeout(poll, 10);
        })();
    }

    function afterQuietTurns(cb) {
        setTimeout(function () {
            setTimeout(cb, 20);
        }, 20);
    }

    it("contains an uncaught throw in a native-initiated callback and keeps the app alive", function (done) {
        var reason = new Error("contained-post-throw");
        var received = null;
        onGlobal("error", function (e) {
            if (e.error === reason) {
                received = e;
            }
        });

        var runnable = new java.lang.Runnable({
            run: function () {
                throw reason;
            }
        });
        new android.os.Handler(android.os.Looper.myLooper()).post(runnable);

        pollUntil(function () { return received !== null; }, function () {
            expect(received).not.toBeNull();
            // The event carries the actual thrown value.
            expect(received.error).toBe(reason);
            // The combined `stackTrace` string is populated BEFORE dispatch,
            // so listeners can read it (not only e.error.stack).
            expect(typeof received.error.stackTrace).toBe("string");
            expect(received.error.stackTrace.length).toBeGreaterThan(0);
            // Unprevented, so the legacy hook fired too.
            expect(uncaughtSeen(reason)).toBe(true);
            // And the app is still running.
            expect(1 + 1).toBe(2);
            done();
        });
    });

    it("preventDefault() on the error event suppresses the legacy hook", function (done) {
        var reason = new Error("contained-prevented-throw");
        var ran = false;
        onGlobal("error", function (e) {
            if (e.error === reason) {
                e.preventDefault();
            }
        });

        var runnable = new java.lang.Runnable({
            run: function () {
                ran = true;
                throw reason;
            }
        });
        new android.os.Handler(android.os.Looper.myLooper()).post(runnable);

        pollUntil(function () { return ran; }, function () {
            afterQuietTurns(function () {
                expect(uncaughtSeen(reason)).toBe(false);
                done();
            });
        });
    });

    it("contains an uncaught throw in a setTimeout callback", function (done) {
        var reason = new Error("contained-timer-throw");
        setTimeout(function () {
            throw reason;
        }, 1);

        pollUntil(function () { return uncaughtSeen(reason); }, function () {
            expect(uncaughtSeen(reason)).toBe(true);
            expect(1 + 1).toBe(2);
            done();
        });
    });

    it("a contained throw in a primitive-returning override yields the type's default", function (done) {
        var reason = new Error("contained-compare-throw");
        var doneRan = false;
        var comparator = new java.util.Comparator({
            compare: function () {
                throw reason;
            }
        });
        var doneCb = new java.lang.Runnable({
            run: function () {
                doneRan = true;
            }
        });
        com.tns.tests.UncaughtErrorPolicyTest.compareOnLooper(comparator, doneCb);

        pollUntil(function () { return doneRan; }, function () {
            // The Java caller received int's default instead of an NPE.
            expect(com.tns.tests.UncaughtErrorPolicyTest.lastCompareResult).toBe(0);
            expect(uncaughtSeen(reason)).toBe(true);
            done();
        });
    });

    it("propagates a JS-initiated chain back to the outer JS catch with identity", function () {
        var reason = new Error("chain-identity");
        var caught = null;
        try {
            com.tns.tests.UncaughtErrorPolicyTest.invoke(new java.lang.Runnable({
                run: function () {
                    throw reason;
                }
            }));
        } catch (e) {
            caught = e;
        }
        // The exception crossed JS -> Java -> JS and surfaced as the very
        // same JS object - not contained, not wrapped.
        expect(caught).toBe(reason);
        expect(uncaughtSeen(reason)).toBe(false);
    });

    // uncaughtErrorPolicy: "throw" is not exercised automatically because a
    // real crash would kill the test runner. To smoke it manually, set
    // { "uncaughtErrorPolicy": "throw" } in the app's package.json and throw
    // from a native-initiated callback (or leave a promise rejection
    // unhandled) - the app must terminate with a com.tns.NativeScriptException
    // whose stack trace points at the JS frames. Default-off behavior is
    // covered by every other suite here.
});

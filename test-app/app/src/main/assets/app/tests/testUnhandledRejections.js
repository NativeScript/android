describe("unhandled promise rejections", function () {
    // Unhandled rejections are tracked per-isolate and reported once per
    // looper turn through the same uncaught-error machinery exposed via
    // global.__onUncaughtError. Each test installs a temporary hook and
    // restores the previous one in afterEach no matter what. Assertions are
    // marker-based (matching this suite's own reasons) so stray rejections
    // from other suites can never interfere.
    var previousHook;
    var reported;
    var addedGlobalListeners;

    beforeEach(function () {
        previousHook = global.__onUncaughtError;
        reported = [];
        global.__onUncaughtError = function (error) {
            reported.push(error);
        };
        addedGlobalListeners = [];
    });

    afterEach(function () {
        global.__onUncaughtError = previousHook;
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

    function reportedSeen(reason) {
        return reported.indexOf(reason) !== -1;
    }

    // The drain happens on a looper turn, so poll across a few turns until the
    // predicate holds (or give up after a bounded number of turns).
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

    // Wait a couple of looper turns to confirm something did NOT happen.
    function afterQuietTurns(cb) {
        setTimeout(function () {
            setTimeout(cb, 20);
        }, 20);
    }

    it("reports an unhandled Promise.reject through __onUncaughtError", function (done) {
        var reason = new Error("unhandled-promise-reject");
        Promise.reject(reason);
        pollUntil(function () { return reportedSeen(reason); }, function () {
            expect(reportedSeen(reason)).toBe(true);
            done();
        });
    });

    it("does not report when .catch is attached synchronously in the same turn", function (done) {
        var reason = new Error("handled-same-turn");
        var p = Promise.reject(reason);
        p.catch(function () {});
        afterQuietTurns(function () {
            expect(reportedSeen(reason)).toBe(false);
            done();
        });
    });

    it("reports an uncaught throw from an async function", function (done) {
        var reason = new Error("async-function-throw");
        (async () => {
            throw reason;
        })();
        pollUntil(function () { return reportedSeen(reason); }, function () {
            expect(reportedSeen(reason)).toBe(true);
            done();
        });
    });

    it("reports an unhandled rejection thrown from a .then callback", function (done) {
        var reason = new Error("then-callback-throw");
        Promise.resolve().then(function () {
            throw reason;
        });
        pollUntil(function () { return reportedSeen(reason); }, function () {
            expect(reportedSeen(reason)).toBe(true);
            done();
        });
    });

    it("ignores a late .catch attached after the rejection was already reported", function (done) {
        var reason = new Error("late-catch");
        var p = Promise.reject(reason);
        pollUntil(function () { return reportedSeen(reason); }, function () {
            expect(reportedSeen(reason)).toBe(true);
            var countBefore = reported.length;
            // Attaching a handler after the report was already delivered must
            // not crash or report again.
            p.catch(function () {});
            afterQuietTurns(function () {
                expect(reported.length).toBe(countBefore);
                done();
            });
        });
    });

    it("unhandledrejection listener receives reason and promise; preventDefault suppresses the hook", function (done) {
        var reason = new Error("rejected-with-listener");
        var received = null;
        onGlobal("unhandledrejection", function (e) {
            if (e.reason === reason) {
                received = e;
                e.preventDefault();
            }
        });

        Promise.reject(reason);

        pollUntil(function () { return received !== null; }, function () {
            expect(received).not.toBeNull();
            expect(received instanceof PromiseRejectionEvent).toBe(true);
            expect(received.type).toBe("unhandledrejection");
            expect(received.reason).toBe(reason);
            expect(typeof received.promise.then).toBe("function");
            // The drain sets a combined `stackTrace` on the (object) reason
            // BEFORE dispatching the event, so a listener sees it.
            expect(typeof received.reason.stackTrace).toBe("string");
            expect(received.reason.stackTrace.length).toBeGreaterThan(0);
            afterQuietTurns(function () {
                expect(reportedSeen(reason)).toBe(false);
                done();
            });
        });
    });

    it("fires rejectionhandled when a handler is attached after the rejection was reported", function (done) {
        var reason = new Error("late-handler");
        var rejectionHandled = null;
        onGlobal("rejectionhandled", function (e) {
            if (e.reason === reason) {
                rejectionHandled = e;
            }
        });
        // Prevent the report so it does not hit the hook; the promise still
        // counts as reported and becomes outstanding for rejectionhandled
        // purposes.
        var reportedPromise = null;
        onGlobal("unhandledrejection", function (e) {
            if (e.reason === reason) {
                reportedPromise = e.promise;
                e.preventDefault();
            }
        });

        var p = Promise.reject(reason);

        pollUntil(function () { return reportedPromise !== null; }, function () {
            // Attach a late handler a couple turns after the report.
            setTimeout(function () {
                p.catch(function () {});
                pollUntil(function () { return rejectionHandled !== null; }, function () {
                    expect(rejectionHandled).not.toBeNull();
                    expect(rejectionHandled instanceof PromiseRejectionEvent).toBe(true);
                    expect(rejectionHandled.type).toBe("rejectionhandled");
                    expect(typeof rejectionHandled.promise.then).toBe("function");
                    expect(rejectionHandled.promise).toBe(reportedPromise);
                    // The original rejection reason is retained past reporting
                    // and carried on the rejectionhandled event, per spec.
                    expect(rejectionHandled.reason).toBe(reason);
                    done();
                });
            }, 20);
        });
    });
});

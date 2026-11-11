// Wraps a done-style spec body that raises global `error` events or unhandled
// rejections on purpose. Jasmine fails the running spec on those unless its
// global error spy is installed, which it is for as long as the body runs; the
// spy is passed as the body's second argument. Jasmine's spy never cancels the
// event, so listeners, preventDefault() and the __onUncaughtError fallback behave
// as they do in production.
//
// The body must call done() only after every error it expects was dispatched:
// one that arrives later is charged to whatever spec is running then.
exports.raisesGlobalErrors = function (body) {
    return function () {
        var context = this;
        return jasmine.spyOnGlobalErrorsAsync(function (globalErrorSpy) {
            return new Promise(function (resolve, reject) {
                function done(reason) {
                    if (reason === undefined) {
                        resolve();
                    } else {
                        reject(reason instanceof Error ? reason : new Error(String(reason)));
                    }
                }
                done.fail = function (reason) {
                    done(reason === undefined ? new Error("Failed") : reason);
                };
                body.call(context, done, globalErrorSpy);
            });
        });
    };
};

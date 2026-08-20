// Settles only if due JS timers run while the module evaluation promise is
// pending: the resolution rides the ordered lane (a Java Handler token), which
// no Handler dispatch can deliver while the pump's JS frames hold the thread.
// The runtime primitive is used directly: the test app's global setTimeout is
// a Java-Handler polyfill the pump cannot dispatch, and this fixture pins the
// runtime's own ordered-lane timers.
export const value = await new Promise(function (resolve) {
    __ns__setTimeout(function () { resolve("timer-ok"); }, 10);
});

// An ES module worker entry whose top-level await parks on a JS timer: the
// resolution rides the worker looper's ordered lane, which the entry pump
// drains directly, so the entry settles inside the yield window. The runtime
// primitive is used directly: the test app wires global setTimeout on the
// main thread only, and as a Java-Handler polyfill the pump could not
// dispatch it anyway — this fixture pins the runtime's own ordered-lane
// timers.
const settled = await new Promise(function (resolve) {
    __ns__setTimeout(function () { resolve("ok"); }, 10);
});

globalThis.onmessage = function (msg) {
    postMessage("timer-entry:" + settled + ":" + msg.data);
};

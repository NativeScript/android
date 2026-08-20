// An ES module worker entry with top-level await, parked on a NON-nestable
// foreground task: the in-place yield window cannot settle it, so the entry
// finishes from the real event loop and the message queue enables on settle.
const i32 = new Int32Array(new SharedArrayBuffer(4));
const wait = Atomics.waitAsync(i32, 0, 0);
Atomics.notify(i32, 0);

const settled = await wait.value;

globalThis.onmessage = function (msg) {
    postMessage("tla-entry:" + settled + ":" + msg.data);
};

// Reached through an extension-less worker specifier: no `.js` sibling exists,
// so only this file can answer. The top-level await parks on a NON-nestable
// foreground task, which the in-place yield window cannot settle, so a message
// posted at construction has to wait on the settle-gated queue.
const i32 = new Int32Array(new SharedArrayBuffer(4));
const wait = Atomics.waitAsync(i32, 0, 0);
Atomics.notify(i32, 0);

const settled = await wait.value;

globalThis.onmessage = function (msg) {
    postMessage("resolved-entry:" + settled + ":" + msg.data);
};

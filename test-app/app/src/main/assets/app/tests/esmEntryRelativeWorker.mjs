// Reached through a relative worker specifier. Module code is strict, so the
// message handler is installed on globalThis rather than by bare assignment.
globalThis.onmessage = function (msg) {
    postMessage("relative-entry:" + msg.data);
};

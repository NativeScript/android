// An ES module worker entry that never finishes evaluating. The parent is told
// the entry body started before the park, so its terminate() lands inside the
// entry's bounded evaluation pump rather than after it.
postMessage("never-settles:started");

await new Promise(function () {});

globalThis.onmessage = function () {
    postMessage("never-settles:unreachable");
};

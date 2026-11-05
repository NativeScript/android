// A fresh isolate: the DOMException constructed inside the getter below is
// the first one this isolate has ever seen, and it is born while the clone
// that carries it is already being written.
var graph = {
    get inner() {
        return new DOMException("first in this isolate", "AbortError");
    },
};
var clone = structuredClone(graph);
postMessage({
    isDomException: clone.inner instanceof DOMException,
    name: clone.inner.name,
    message: clone.inner.message,
});

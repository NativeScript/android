// A fresh isolate: the markAsUncloneable call in the getter below is the first
// one this isolate has seen, and it happens while the clone that reaches the
// marked object is already being written.
var markAsUncloneable = require("node:worker_threads").markAsUncloneable;
var graph = {
    get inner() {
        var marked = { a: 1 };
        markAsUncloneable(marked);
        return marked;
    },
};
var result;
try {
    structuredClone(graph);
    result = { threw: false };
} catch (e) {
    result = { threw: true, name: e && e.name };
}
postMessage(result);

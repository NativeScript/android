// A fresh isolate: the getter makes this isolate's first markAsUncloneable
// call mid-clone, on an object it then keeps to itself, so the marked object
// the clone must reject is one it only reaches afterwards.
var markAsUncloneable = require("node:worker_threads").markAsUncloneable;
var marked = { a: 1 };
var graph = {
    get first() {
        markAsUncloneable(marked);
        return { plain: true };
    },
    second: { plain: true },
    third: [{ plain: true }, { plain: true }],
    last: marked,
};
var result;
try {
    structuredClone(graph);
    result = { threw: false };
} catch (e) {
    result = { threw: true, name: e && e.name };
}
postMessage(result);

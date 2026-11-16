onmessage = function (msg) {
    var data = msg.data;
    var loaded;
    try {
        loaded = data.isModule
            ? import(data.path).then(function (ns) { return ns.value; })
            : Promise.resolve(require(data.path).value);
    } catch (e) {
        loaded = Promise.reject(e);
    }
    loaded.then(function (value) {
        postMessage({ value: value });
    }, function (e) {
        postMessage({ error: String(e) });
    });
};

onmessage = function (event) {
    var port = event.data.port;
    postMessage(port, [port]);
    Atomics.store(event.data.flag, 0, 1);
};

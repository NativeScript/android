var parentPort = require("node:worker_threads").parentPort;
parentPort.on("message", function (value) {
    parentPort.postMessage({ received: value === undefined ? "undefined" : typeof value });
});

var parentPort = require("node:worker_threads").parentPort;
parentPort.once("message", function (value) {
    parentPort.postMessage({ once: value });
});
parentPort.on("message", function (value) {
    parentPort.postMessage({ on: value });
});

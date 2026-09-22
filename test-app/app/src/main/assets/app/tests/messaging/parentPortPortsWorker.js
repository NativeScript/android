var parentPort = require("node:worker_threads").parentPort;
parentPort.addEventListener("message", function (event) {
    parentPort.postMessage(event.ports.length);
});

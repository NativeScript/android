var parentPort = require("node:worker_threads").parentPort;
parentPort.on("message", function () {
    throw new TypeError("thrown by a parentPort listener");
});

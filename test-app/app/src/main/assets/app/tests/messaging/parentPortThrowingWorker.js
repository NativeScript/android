var parentPort = require("node:worker_threads").parentPort;
parentPort.on("message", function () {
    throw new Error("thrown by a parentPort listener");
});

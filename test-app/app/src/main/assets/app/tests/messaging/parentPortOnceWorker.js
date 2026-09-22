var parentPort = require("node:worker_threads").parentPort;
var count = 0;
function listener() {
    count++;
    parentPort.postMessage(count);
}
parentPort.on("message", listener);
parentPort.once("message", listener);

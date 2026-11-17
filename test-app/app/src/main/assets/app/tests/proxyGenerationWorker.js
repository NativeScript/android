// Entry for testConcurrentProxyGeneration: waits for "go", then generates and calls
// this worker's proxies.
var generateAndCallProxies = require("./proxyGenerationJob");

onmessage = function (msg) {
    postMessage(generateAndCallProxies(msg.data));
};

postMessage("ready");

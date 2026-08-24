// A worker is a fresh isolate, which is what makes the access order testable:
// the parent realm has already materialized TextEncoder/TextDecoder by the
// time any spec runs. Nothing here may touch either name before the handler,
// or the requested order is lost.
onmessage = function (msg) {
    var order = msg.data;
    var results = { order: order };

    if (order === "global-first") {
        var globalEncoder = globalThis.TextEncoder;
        var globalDecoder = globalThis.TextDecoder;
        var nsUtil = require("ns:util");
        var nodeUtil = require("node:util");
        results.encoder = nsUtil.TextEncoder === globalEncoder && nodeUtil.TextEncoder === globalEncoder;
        results.decoder = nsUtil.TextDecoder === globalDecoder && nodeUtil.TextDecoder === globalDecoder;
        results.roundTrip = new nsUtil.TextDecoder().decode(new nodeUtil.TextEncoder().encode("ok"));
    } else {
        var util = require("ns:util");
        var node = require("node:util");
        var utilEncoder = util.TextEncoder;
        var utilDecoder = node.TextDecoder;
        results.encoder = globalThis.TextEncoder === utilEncoder && node.TextEncoder === utilEncoder;
        results.decoder = globalThis.TextDecoder === utilDecoder && util.TextDecoder === utilDecoder;
        results.roundTrip = new node.TextDecoder().decode(new util.TextEncoder().encode("ok"));
    }

    postMessage(results);
};

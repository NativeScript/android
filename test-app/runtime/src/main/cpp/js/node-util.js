"use strict";

// The `node:util` compatibility shim: a documented subset of Node's util,
// backed by `ns:util` (docs/ns-builtin-modules.md). Compiled the first time
// `node:util` is resolved, so an app that never touches the `node:` scheme
// never pays for it.
//
// Every adaptation to Node's API belongs in this file — argument shapes,
// option names, aliases, anything that has to track Node rather than the
// runtime. The `ns:` modules stay free of compatibility knowledge, so the two
// surfaces can diverge without either one carrying the other's baggage.

const { ObjectFreeze } = primordials;
const util = require("ns:util");
const { inspect, format } = util;

// Node exposes the two encoding interfaces on util; they stay lazy here for
// the same reason they are lazy on ns:util.
module.exports = ObjectFreeze({
  inspect,
  format,
  get TextEncoder() {
    return util.TextEncoder;
  },
  get TextDecoder() {
    return util.TextDecoder;
  },
});

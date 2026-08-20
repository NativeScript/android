"use strict";

// The `node:module` compatibility shim: the documented subset of Node's
// module API, backed by `ns:module` (docs/ns-builtin-modules.md). Compiled
// the first time `node:module` is resolved, so an app that never touches the
// `node:` scheme never pays for it.
//
// Only `createRequire` is re-exported. `createPumpingRequire` is a
// NativeScript extension with no Node counterpart and stays on `ns:module`,
// so code written against this shim keeps running on Node unchanged.

const { ObjectFreeze } = primordials;
const { createRequire } = require("ns:module");

module.exports = ObjectFreeze({ createRequire });

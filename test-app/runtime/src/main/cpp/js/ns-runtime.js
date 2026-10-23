"use strict";

// The `ns:runtime` builtin module: runtime-level configuration and runtime
// plumbing. See docs/ns-builtin-modules.md for the contract and the key
// registry — keys, their value domains, and their scope (process-wide vs
// per-isolate) are defined and validated on the native side, so this file
// stays a thin, frozen surface.

const { setConfig, getConfig, escapeException } = binding;
const { ObjectFreeze } = primordials;

exports.setConfig = setConfig;
exports.getConfig = getConfig;
exports.escapeException = escapeException;
ObjectFreeze(exports);

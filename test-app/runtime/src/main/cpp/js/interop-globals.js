"use strict";

// The legacy interop globals, assembled from the modules that own them:
// `interop.escapeException` from ns:runtime, the numeric casts and
// `Array.create` from ns:interop/android. Each global holds the module's own
// function object, never a wrapper, so the two spellings cannot drift.
//
// Runs eagerly from Runtime::PrepareV8Runtime, before any app code:
// @nativescript/core reads these at module top level. They stay ordinary
// writable, enumerable, configurable data properties.

const { ObjectDefineProperty } = primordials;
const { escapeException } = require("ns:runtime");
const android = require("ns:interop/android");

function define(target, name, value) {
  ObjectDefineProperty(target, name, {
    value,
    writable: true,
    enumerable: true,
    configurable: true,
  });
}

define(globalThis, "interop", { escapeException });

define(globalThis, "long", android.long);
define(globalThis, "byte", android.byte);
define(globalThis, "short", android.short);
define(globalThis, "char", android.char);
define(globalThis, "float", android.float);
define(globalThis, "double", android.double);

define(Array, "create", android.newArray);

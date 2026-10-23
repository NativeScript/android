"use strict";

// The `ns:interop/android` builtin module: Java-specific interop helpers
// (docs/ns-builtin-modules.md). The numeric casts mark a JS value as a Java
// primitive for overload resolution and argument conversion; `newArray`
// creates a Java array of a primitive type name or a Java class.

const { long, byte, short, char, float, double, newArray } = binding;
const { ObjectFreeze } = primordials;

exports.long = long;
exports.byte = byte;
exports.short = short;
exports.char = char;
exports.float = float;
exports.double = double;
exports.newArray = newArray;
ObjectFreeze(exports);

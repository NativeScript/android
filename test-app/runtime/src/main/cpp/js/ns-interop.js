"use strict";

// The `ns:interop` builtin module: the cross-platform interop surface
// (docs/ns-builtin-modules.md). It exists on every platform with whatever
// members that platform supports; Android has none of the native-memory
// pieces iOS exports here, so the namespace is empty. Java-specific helpers
// live in `ns:interop/android`.

const { ObjectFreeze } = primordials;

ObjectFreeze(exports);

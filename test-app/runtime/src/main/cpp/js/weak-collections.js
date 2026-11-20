"use strict";

// Canary wrapper lifetime mode only (run by WeakRef.cpp): WeakMap, WeakSet,
// WeakRef and FinalizationRegistry key a Java object by its lifetime key
// instead of its wrapper. The wrapper is a canary that dies with its JS
// references while the Java object lives on, and a later wrapper for the same
// object is a new JS object; keyed by the wrapper, entries and targets would be
// lost with it. Every other value passes through unchanged.

const { unwrap, wrap } = binding;
const { ObjectDefineProperty, TypeError } = primordials;
const {
  FinalizationRegistry: NativeFinalizationRegistry,
  WeakMap: NativeWeakMap,
  WeakRef: NativeWeakRef,
  WeakSet: NativeWeakSet,
} = globalThis;

// unwrap(value, true) makes the key on first use; lookups pass false so they
// never create state for an object that was never a key.
class WeakMap extends NativeWeakMap {
  get(key) {
    return super.get(unwrap(key, false));
  }

  set(key, value) {
    super.set(unwrap(key, true), value);
    return this;
  }

  has(key) {
    return super.has(unwrap(key, false));
  }

  delete(key) {
    return super.delete(unwrap(key, false));
  }
}

// V8 ships the upsert proposal; inherited, these would key on the wrapper.
if (typeof NativeWeakMap.prototype.getOrInsert === "function") {
  ObjectDefineProperty(WeakMap.prototype, "getOrInsert", {
    value: function getOrInsert(key, value) {
      return NativeWeakMap.prototype.getOrInsert.call(this, unwrap(key, true), value);
    },
    writable: true,
    configurable: true,
  });
}
if (typeof NativeWeakMap.prototype.getOrInsertComputed === "function") {
  ObjectDefineProperty(WeakMap.prototype, "getOrInsertComputed", {
    value: function getOrInsertComputed(key, callbackfn) {
      if (typeof callbackfn !== "function") {
        throw new TypeError("WeakMap.prototype.getOrInsertComputed: callback is not a function");
      }
      // The callback sees the key it was given, not the lifetime key.
      return NativeWeakMap.prototype.getOrInsertComputed.call(this, unwrap(key, true), () =>
        callbackfn(key),
      );
    },
    writable: true,
    configurable: true,
  });
}

class WeakSet extends NativeWeakSet {
  add(value) {
    super.add(unwrap(value, true));
    return this;
  }

  has(value) {
    return super.has(unwrap(value, false));
  }

  delete(value) {
    return super.delete(unwrap(value, false));
  }
}

class WeakRef extends NativeWeakRef {
  constructor(target) {
    const key = unwrap(target, true);
    super(key);
    // The target itself, not only its key, stays alive until the job ends.
    if (key !== target) {
      new NativeWeakRef(target);
    }
  }

  // The live wrapper, a new one if the old one died while the Java object
  // lived, or undefined once the Java object is gone. Like a native WeakRef
  // target, the wrapper it returns stays alive until the job ends, so a second
  // deref in the same job sees the same object.
  deref() {
    const result = wrap(super.deref());
    if (typeof result === "object" && result !== null) {
      new NativeWeakRef(result);
    }
    return result;
  }
}

// weak-ref.js gave the native prototype `get` (an alias of `deref`) and a
// deprecated `clear`; inherited as is, `get` would hand out the lifetime key.
ObjectDefineProperty(WeakRef.prototype, "get", {
  value: WeakRef.prototype.deref,
  writable: true,
  configurable: true,
});

class FinalizationRegistry extends NativeFinalizationRegistry {
  constructor(cleanupCallback) {
    super(cleanupCallback);
  }

  register(target, heldValue, unregisterToken) {
    // Checked on the original values: unwrapping would make them differ.
    if (target === heldValue) {
      throw new TypeError("FinalizationRegistry.prototype.register: target and holdings must not be same");
    }
    super.register(
      unwrap(target, true),
      heldValue,
      unregisterToken === undefined ? undefined : unwrap(unregisterToken, true),
    );
  }

  unregister(unregisterToken) {
    return super.unregister(unwrap(unregisterToken, false));
  }
}

for (const [name, value] of [
  ["WeakMap", WeakMap],
  ["WeakSet", WeakSet],
  ["WeakRef", WeakRef],
  ["FinalizationRegistry", FinalizationRegistry],
]) {
  ObjectDefineProperty(globalThis, name, {
    value,
    writable: true,
    configurable: true,
    enumerable: false,
  });
}

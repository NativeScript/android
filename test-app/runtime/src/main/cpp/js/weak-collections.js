"use strict";

// Canary wrapper lifetime mode only (run by WeakRef.cpp): WeakMap, WeakSet,
// WeakRef and FinalizationRegistry key a Java object by its lifetime key
// instead of its wrapper. The wrapper is a canary that dies with its JS
// references while the Java object lives on, and a later wrapper for the same
// object is a new JS object; keyed by the wrapper, entries and targets would be
// lost with it. Every other value passes through unchanged.
//
// They are subclasses of the native collections, so what subclassing shows is
// left as it is: Object.getPrototypeOf(WeakMap.prototype) is the native
// prototype, and WeakMap.toString() is not native code.

const { unwrap, wrap } = binding;
const {
  FinalizationRegistry: NativeFinalizationRegistry,
  FinalizationRegistryPrototypeRegister,
  FinalizationRegistryPrototypeUnregister,
  ObjectDefineProperty,
  ObjectFreeze,
  SymbolKeyFor,
  WeakMap: NativeWeakMap,
  WeakMapPrototypeDelete,
  WeakMapPrototypeGet,
  WeakMapPrototypeGetOrInsert,
  WeakMapPrototypeGetOrInsertComputed,
  WeakMapPrototypeHas,
  WeakMapPrototypeSet,
  WeakRef: NativeWeakRef,
  WeakRefPrototypeDeref,
  WeakSet: NativeWeakSet,
  WeakSetPrototypeAdd,
  WeakSetPrototypeDelete,
  WeakSetPrototypeHas,
} = primordials;

// Only an object can be a canary; anything else goes to the native method as is.
function isObject(value) {
  return (typeof value === "object" && value !== null) || typeof value === "function";
}

// The values the native collections accept as keys and targets.
function canBeHeldWeakly(value) {
  return isObject(value) || (typeof value === "symbol" && SymbolKeyFor(value) === undefined);
}

// unwrap(value, true) makes the key on first use; lookups pass false so they
// never create state for an object that was never a key. A key is made only
// once the native method can no longer reject the call, so a failed call leaves
// no state behind: the receiver is checked first with a method that cannot
// insert anything.
function lookupKey(value) {
  return isObject(value) ? unwrap(value, false) : value;
}

// Unregistering it finds nothing: it is never a token.
const receiverProbe = ObjectFreeze({ __proto__: null });

class WeakMap extends NativeWeakMap {
  get(key) {
    return WeakMapPrototypeGet(this, lookupKey(key));
  }

  set(key, value) {
    if (isObject(key)) {
      WeakMapPrototypeHas(this, undefined);
      key = unwrap(key, true);
    }
    WeakMapPrototypeSet(this, key, value);
    return this;
  }

  has(key) {
    return WeakMapPrototypeHas(this, lookupKey(key));
  }

  delete(key) {
    return WeakMapPrototypeDelete(this, lookupKey(key));
  }
}

// V8 ships the upsert proposal; inherited, these would key on the wrapper.
if (WeakMapPrototypeGetOrInsert !== undefined) {
  ObjectDefineProperty(WeakMap.prototype, "getOrInsert", {
    value: function getOrInsert(key, value) {
      if (isObject(key)) {
        WeakMapPrototypeHas(this, undefined);
        key = unwrap(key, true);
      }
      return WeakMapPrototypeGetOrInsert(this, key, value);
    },
    writable: true,
    configurable: true,
  });
}
if (WeakMapPrototypeGetOrInsertComputed !== undefined) {
  ObjectDefineProperty(WeakMap.prototype, "getOrInsertComputed", {
    value: function getOrInsertComputed(key, callbackfn) {
      if (!isObject(key) || typeof callbackfn !== "function") {
        return WeakMapPrototypeGetOrInsertComputed(this, key, callbackfn);
      }
      const existing = unwrap(key, false);
      if (WeakMapPrototypeHas(this, existing)) {
        return WeakMapPrototypeGet(this, existing);
      }
      // Before the key exists, so a callback that throws leaves none; it sees
      // the key it was given, not the lifetime key.
      const value = callbackfn(key);
      WeakMapPrototypeSet(this, unwrap(key, true), value);
      return value;
    },
    writable: true,
    configurable: true,
  });
}

class WeakSet extends NativeWeakSet {
  add(value) {
    if (isObject(value)) {
      WeakSetPrototypeHas(this, undefined);
      value = unwrap(value, true);
    }
    WeakSetPrototypeAdd(this, value);
    return this;
  }

  has(value) {
    return WeakSetPrototypeHas(this, lookupKey(value));
  }

  delete(value) {
    return WeakSetPrototypeDelete(this, lookupKey(value));
  }
}

class WeakRef extends NativeWeakRef {
  constructor(target) {
    const key = isObject(target) ? unwrap(target, true) : target;
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
    const target = WeakRefPrototypeDeref(this);
    if (!isObject(target)) {
      return target;
    }
    const result = wrap(target);
    // The native deref already keeps a target it returns as is.
    if (result !== target && result !== undefined) {
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

  // The default keeps `length` at 2, as on the native method.
  register(target, heldValue, unregisterToken = undefined) {
    // Checked on the original values: unwrapping would make target and
    // heldValue differ. A call the native method rejects gets its error.
    if (!canBeHeldWeakly(target) || target === heldValue ||
        (unregisterToken !== undefined && !canBeHeldWeakly(unregisterToken))) {
      return FinalizationRegistryPrototypeRegister(this, target, heldValue, unregisterToken);
    }
    FinalizationRegistryPrototypeUnregister(this, receiverProbe);
    FinalizationRegistryPrototypeRegister(
      this,
      isObject(target) ? unwrap(target, true) : target,
      heldValue,
      isObject(unregisterToken) ? unwrap(unregisterToken, true) : unregisterToken,
    );
  }

  unregister(unregisterToken) {
    return FinalizationRegistryPrototypeUnregister(this, lookupKey(unregisterToken));
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

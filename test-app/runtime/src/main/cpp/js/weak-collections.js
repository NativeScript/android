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

const { maybeCanary, unwrap, wrap, takeRekeys, pendingRekeys } = binding;
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

// unwrap(value, true) makes the key on first use. A key is made only once the
// native method can no longer reject the call, so a failed call leaves no key
// behind: the receiver is checked first with a method that cannot insert
// anything.
//
// A canary is never stored as a key itself (its lifetime key is), so every
// method first tries the value as is. canaryToKey holds every canary that
// stands for a key and that this file has seen: the ones keyFor unwrapped, and
// the ones takeRekeys returned. pendingRekeys[0] counts the live canaries the
// runtime built for keyed objects since takeRekeys last ran; while it is 0, a
// value missing from canaryToKey was never a key, and no native code runs.
const canaryToKey = new NativeWeakMap();

function takePendingRekeys() {
  const pairs = takeRekeys();
  for (let i = 0; i < pairs.length; i += 2) {
    WeakMapPrototypeSet(canaryToKey, pairs[i], pairs[i + 1]);
  }
}

// The lifetime key value stands for, or undefined when that is value itself.
function otherKey(value) {
  if (!isObject(value)) {
    return undefined;
  }
  const key = WeakMapPrototypeGet(canaryToKey, value);
  if (key !== undefined || pendingRekeys[0] === 0) {
    return key;
  }
  takePendingRekeys();
  return WeakMapPrototypeGet(canaryToKey, value);
}

// The key to store for value, an object, made on first use.
function keyFor(value) {
  // A V8 fast call: a plain JS object reaches neither canaryToKey nor unwrap.
  if (!maybeCanary(value)) {
    return value;
  }
  const known = otherKey(value);
  if (known !== undefined) {
    return known;
  }
  const key = unwrap(value, true);
  if (key !== value) {
    WeakMapPrototypeSet(canaryToKey, value, key);
  }
  return key;
}

// Unregistering it finds nothing: it is never a token.
const receiverProbe = ObjectFreeze({ __proto__: null });

class WeakMap extends NativeWeakMap {
  get(key) {
    const direct = WeakMapPrototypeGet(this, key);
    if (direct !== undefined) {
      return direct;
    }
    const other = otherKey(key);
    return other === undefined ? undefined : WeakMapPrototypeGet(this, other);
  }

  set(key, value) {
    // Also checks the receiver before a key is made.
    if (isObject(key) && !WeakMapPrototypeHas(this, key)) {
      key = keyFor(key);
    }
    WeakMapPrototypeSet(this, key, value);
    return this;
  }

  has(key) {
    if (WeakMapPrototypeHas(this, key)) {
      return true;
    }
    const other = otherKey(key);
    return other !== undefined && WeakMapPrototypeHas(this, other);
  }

  delete(key) {
    if (WeakMapPrototypeDelete(this, key)) {
      return true;
    }
    const other = otherKey(key);
    return other !== undefined && WeakMapPrototypeDelete(this, other);
  }
}

// V8 ships the upsert proposal; inherited, these would key on the wrapper.
if (WeakMapPrototypeGetOrInsert !== undefined) {
  ObjectDefineProperty(WeakMap.prototype, "getOrInsert", {
    value: function getOrInsert(key, value) {
      if (isObject(key) && !WeakMapPrototypeHas(this, key)) {
        key = keyFor(key);
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
      if (WeakMapPrototypeHas(this, key)) {
        return WeakMapPrototypeGet(this, key);
      }
      const existing = otherKey(key);
      if (existing !== undefined && WeakMapPrototypeHas(this, existing)) {
        return WeakMapPrototypeGet(this, existing);
      }
      // Before the key exists, so a callback that throws leaves none; it sees
      // the key it was given, not the lifetime key.
      const value = callbackfn(key);
      WeakMapPrototypeSet(this, keyFor(key), value);
      return value;
    },
    writable: true,
    configurable: true,
  });
}

class WeakSet extends NativeWeakSet {
  add(value) {
    // Also checks the receiver before a key is made.
    if (isObject(value) && !WeakSetPrototypeHas(this, value)) {
      value = keyFor(value);
    }
    WeakSetPrototypeAdd(this, value);
    return this;
  }

  has(value) {
    if (WeakSetPrototypeHas(this, value)) {
      return true;
    }
    const other = otherKey(value);
    return other !== undefined && WeakSetPrototypeHas(this, other);
  }

  delete(value) {
    if (WeakSetPrototypeDelete(this, value)) {
      return true;
    }
    const other = otherKey(value);
    return other !== undefined && WeakSetPrototypeDelete(this, other);
  }
}

class WeakRef extends NativeWeakRef {
  // A NativeWeakRef to the canary this was made with or last dereferenced to,
  // or undefined when the target is not a key. A live canary is always what
  // wrap returns for its key: Java cannot collect the object while it lives,
  // and a released canary stays its key's object. Only the constructor and
  // deref make one: a NativeWeakRef keeps its target until the job ends, which
  // they do by spec anyway.
  #canary;

  constructor(target) {
    const key = isObject(target) ? keyFor(target) : target;
    super(key);
    if (key !== target) {
      this.#canary = new NativeWeakRef(target);
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
    // False for a native WeakRef given this method through call().
    const cached = #canary in this;
    if (cached) {
      const last = this.#canary;
      if (last === undefined) {
        return target;
      }
      const live = WeakRefPrototypeDeref(last);
      if (live !== undefined) {
        return live;
      }
    }
    const result = wrap(target);
    // The native deref already keeps a target it returns as is.
    if (result !== target && result !== undefined) {
      const ref = new NativeWeakRef(result);
      if (cached) {
        this.#canary = ref;
      }
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
      isObject(target) ? keyFor(target) : target,
      heldValue,
      isObject(unregisterToken) ? keyFor(unregisterToken) : unregisterToken,
    );
  }

  unregister(unregisterToken) {
    if (FinalizationRegistryPrototypeUnregister(this, unregisterToken)) {
      return true;
    }
    const other = otherKey(unregisterToken);
    return other !== undefined && FinalizationRegistryPrototypeUnregister(this, other);
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

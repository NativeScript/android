"use strict";

// Terminal formatter for console.* (util.inspect-lite). Everything here runs
// while user code may have tampered any prototype, so all intrinsic access
// goes through primordials, brands come from Object.prototype.toString rather
// than constructors, and getters are NEVER invoked. Output is budgeted: depth,
// per-collection entry caps, string caps, and a hard total-size cap make it
// impossible for a single console.log to hang the app on a huge graph.
//
// Proxies are the one structural exception to "never runs user code": a proxy
// is indistinguishable from its target here, so the key and descriptor reads
// below fire its traps. Trap output is spent against the same budget as any
// other value and whatever a trap throws is caught by inspect().

const { getNativeWrapperHint } = binding;
const {
  ArrayBufferIsView,
  ArrayBufferPrototypeGetByteLength,
  ArrayIsArray,
  ArrayPrototypePush,
  DataViewPrototypeGetByteLength,
  DatePrototypeGetTime,
  DatePrototypeToISOString,
  FunctionPrototypeToString,
  JSONStringify,
  MapIteratorPrototypeNext,
  MapPrototypeEntries,
  MapPrototypeGetSize,
  NumberIsNaN,
  ObjectDefineProperty,
  ObjectGetOwnPropertyDescriptor,
  ObjectGetOwnPropertySymbols,
  ObjectGetPrototypeOf,
  ObjectIs,
  ObjectKeys,
  FunctionPrototypeCall,
  ObjectPrototype,
  ObjectPrototypePropertyIsEnumerable,
  ObjectPrototypeToString,
  RegExpPrototypeTest,
  RegExpPrototypeToString,
  Set,
  SetIteratorPrototypeNext,
  SetPrototypeAdd,
  SetPrototypeDelete,
  SetPrototypeGetSize,
  SetPrototypeHas,
  SetPrototypeValues,
  String,
  StringPrototypeIndexOf,
  StringPrototypeSlice,
  SymbolPrototypeToString,
  TypedArrayPrototypeGetLength,
} = primordials;

const DEFAULT_DEPTH = 2;
const MAX_ENTRIES = 100;       // array elements / object keys / map+set entries
const MAX_STRING = 10000;      // characters of a single string value
const MAX_TOTAL = 16384;       // hard cap for one inspect() result

// Thrown to unwind out of a deep graph the moment the total budget is spent;
// never escapes inspect().
const BUDGET_EXHAUSTED = { budget: true };

function inspect(value, options) {
  const ctx = {
    depth: options && typeof options.depth === "number" ? options.depth : DEFAULT_DEPTH,
    remaining: MAX_TOTAL,
    // In-progress ancestors only (entries are removed on exit), so shared
    // acyclic references print normally and only true cycles say [Circular].
    ancestors: new Set(),
    truncated: false,
  };
  let result;
  try {
    result = formatValue(ctx, value, 0);
  } catch (e) {
    if (e === BUDGET_EXHAUSTED) {
      ctx.truncated = true;
      result = ctx.out !== undefined ? ctx.out : "";
    } else {
      // A formatter bug must never take the log call down with it.
      try {
        result = "[inspect failed: " + safeString(e) + "]";
      } catch (ignored) {
        result = "[inspect failed]";
      }
    }
  }
  if (ctx.truncated) {
    result = result + "... (output truncated)";
  }
  return result;
}

function spend(ctx, text) {
  ctx.remaining -= text.length;
  if (ctx.remaining < 0) {
    ctx.out = StringPrototypeSlice(text, 0, text.length + ctx.remaining);
    throw BUDGET_EXHAUSTED;
  }
  return text;
}

function safeString(value) {
  try {
    return String(value);
  } catch (ignored) {
    return "?";
  }
}

function quoteString(ctx, str) {
  let clipped = str;
  let suffix = "";
  if (clipped.length > MAX_STRING) {
    clipped = StringPrototypeSlice(clipped, 0, MAX_STRING);
    suffix = "... " + (str.length - MAX_STRING) + " more characters";
  }
  // JSONStringify escapes quotes/control characters; a string exotic enough to
  // defeat it (lone surrogates are fine) just falls back to raw.
  let quoted;
  try {
    quoted = JSONStringify(clipped);
  } catch (ignored) {
    quoted = "'" + clipped + "'";
  }
  return quoted + suffix;
}

function formatValue(ctx, value, depth) {
  // Primitives.
  if (value === null) return "null";
  const type = typeof value;
  if (type === "undefined") return "undefined";
  if (type === "string") return quoteString(ctx, value);
  if (type === "number") return ObjectIs(value, -0) ? "-0" : String(value);
  if (type === "boolean") return value ? "true" : "false";
  if (type === "bigint") return String(value) + "n";
  if (type === "symbol") return SymbolPrototypeToString(value);

  if (type === "function") return formatFunction(value);

  // Objects. Native wrappers first: never walk into a native-backed graph.
  const hint = getNativeWrapperHint(value);
  if (hint !== undefined) {
    return "[" + hint + "]";
  }

  if (SetPrototypeHas(ctx.ancestors, value)) {
    return "[Circular]";
  }
  if (depth > ctx.depth) {
    const brand = ObjectPrototypeToString(value);
    return brand === "[object Object]" ? "[Object]" : "[" + StringPrototypeSlice(brand, 8, -1) + "]";
  }

  SetPrototypeAdd(ctx.ancestors, value);
  try {
    return formatObject(ctx, value, depth);
  } finally {
    SetPrototypeDelete(ctx.ancestors, value);
  }
}

function formatFunction(fn) {
  let name = "";
  const desc = ObjectGetOwnPropertyDescriptor(fn, "name");
  if (desc !== undefined && typeof desc.value === "string") {
    name = desc.value;
  }
  let isClass = false;
  try {
    const src = FunctionPrototypeToString(fn);
    isClass = StringPrototypeIndexOf(src, "class") === 0;
  } catch (ignored) {
    // Some callables (bound functions render fine; revoked proxies do not)
    // refuse toString; treat them as plain functions.
  }
  if (isClass) {
    return name === "" ? "[class (anonymous)]" : "[class " + name + "]";
  }
  return name === "" ? "[Function (anonymous)]" : "[Function: " + name + "]";
}

function formatObject(ctx, value, depth) {
  const brand = ObjectPrototypeToString(value);

  if (ArrayIsArray(value)) {
    return formatArray(ctx, value, depth);
  }
  if (brand === "[object Map]") {
    return formatMapLike(ctx, value, depth, true);
  }
  if (brand === "[object Set]") {
    return formatMapLike(ctx, value, depth, false);
  }
  if (brand === "[object Date]") {
    const time = DatePrototypeGetTime(value);
    return NumberIsNaN(time) ? "Invalid Date" : DatePrototypeToISOString(value);
  }
  if (brand === "[object RegExp]") {
    return RegExpPrototypeToString(value);
  }
  if (brand === "[object Error]") {
    return formatError(ctx, value);
  }
  if (brand === "[object Promise]") {
    return "Promise {}";
  }
  if (ArrayBufferIsView(value)) {
    // TypedArray/DataView: brand carries the concrete type name; sizes come
    // from the captured accessor getters, not the (tamperable) prototype.
    const name = StringPrototypeSlice(brand, 8, -1);
    let size;
    try {
      size = brand === "[object DataView]"
          ? DataViewPrototypeGetByteLength(value)
          : TypedArrayPrototypeGetLength(value);
    } catch (ignored) {
      return name;
    }
    return name + "(" + size + ")";
  }
  if (brand === "[object ArrayBuffer]" || brand === "[object SharedArrayBuffer]") {
    let size;
    try {
      size = ArrayBufferPrototypeGetByteLength(value);
    } catch (ignored) {
      return StringPrototypeSlice(brand, 8, -1);
    }
    return StringPrototypeSlice(brand, 8, -1) + "(" + size + ")";
  }

  // NativeScript core (ViewBase, Observable, ...) and app classes override
  // toString for short debug forms; the old console honored that, so a custom
  // toString wins over structural rendering. This is the second deliberate
  // exception to the no-user-code rule (with error.stack), guarded and capped.
  const customToString = findCustomToString(value);
  if (customToString !== undefined) {
    try {
      const str = FunctionPrototypeCall(customToString, value);
      if (typeof str === "string" && str !== "") {
        return spend(ctx, str.length > MAX_STRING ? StringPrototypeSlice(str, 0, MAX_STRING) : str);
      }
    } catch (ignored) {
    }
  }

  return formatPlainObject(ctx, value, depth);
}

// A toString override that is NOT Object.prototype's: own property first, then
// up the chain, stopping at Object.prototype. Data properties only — a
// toString defined as an accessor is not worth invoking a getter for. The hop
// cap only bounds Proxy getPrototypeOf traps fabricating endless chains; it
// must clear real hierarchies (core layout views sit ~10 prototypes deep).
function findCustomToString(value) {
  let target = value;
  for (let i = 0; target !== null && target !== ObjectPrototype && i < 32; i++) {
    const desc = ObjectGetOwnPropertyDescriptor(target, "toString");
    if (desc !== undefined) {
      return typeof desc.value === "function" ? desc.value : undefined;
    }
    target = ObjectGetPrototypeOf(target);
  }
  return undefined;
}

function formatError(ctx, err) {
  // Exception to the no-getters rule: V8 materializes `stack` through a lazy
  // own accessor, and the stack is the whole point of printing an error. The
  // guarded read carries the same exposure as every other error-reporting
  // path in the runtime that touches `.stack`.
  try {
    const stack = err.stack;
    if (typeof stack === "string" && stack !== "") {
      return spend(ctx, stack);
    }
  } catch (ignored) {
  }
  const messageDesc = ObjectGetOwnPropertyDescriptor(err, "message");
  const message = messageDesc !== undefined && typeof messageDesc.value === "string"
      ? messageDesc.value : "";
  let name = "Error";
  const nameDesc = ObjectGetOwnPropertyDescriptor(err, "name");
  if (nameDesc !== undefined && typeof nameDesc.value === "string") {
    name = nameDesc.value;
  }
  return message === "" ? name : name + ": " + message;
}

function formatArray(ctx, arr, depth) {
  const parts = [];
  const len = arr.length;
  const shown = len > MAX_ENTRIES ? MAX_ENTRIES : len;
  let emptyRun = 0;
  for (let i = 0; i < shown; i++) {
    const desc = ObjectGetOwnPropertyDescriptor(arr, i);
    if (desc === undefined) {
      emptyRun++;
      continue;
    }
    if (emptyRun > 0) {
      ArrayPrototypePush(parts, "<" + emptyRun + " empty items>");
      emptyRun = 0;
    }
    ArrayPrototypePush(parts, formatProperty(ctx, desc, depth));
  }
  if (emptyRun > 0) {
    ArrayPrototypePush(parts, "<" + emptyRun + " empty items>");
  }
  if (len > shown) {
    ArrayPrototypePush(parts, "... " + (len - shown) + " more items");
  }
  return spend(ctx, parts.length === 0 ? "[]" : "[ " + join(parts) + " ]");
}

function formatMapLike(ctx, coll, depth, isMap) {
  const parts = [];
  // The count comes from the size accessor so that iteration can stop at the
  // cap: walking a million-entry Map just to label it would defeat the budget.
  const total = isMap ? MapPrototypeGetSize(coll) : SetPrototypeGetSize(coll);
  const shown = total > MAX_ENTRIES ? MAX_ENTRIES : total;
  const iter = isMap ? MapPrototypeEntries(coll) : SetPrototypeValues(coll);
  const next = isMap ? MapIteratorPrototypeNext : SetIteratorPrototypeNext;
  for (let i = 0; i < shown; i++) {
    const step = next(iter);
    if (step.done) break;
    if (isMap) {
      const entry = step.value;
      ArrayPrototypePush(parts,
          formatValue(ctx, entry[0], depth + 1) + " => " + formatValue(ctx, entry[1], depth + 1));
    } else {
      ArrayPrototypePush(parts, formatValue(ctx, step.value, depth + 1));
    }
  }
  if (total > shown) {
    ArrayPrototypePush(parts, "... " + (total - shown) + " more items");
  }
  const label = (isMap ? "Map(" : "Set(") + total + ")";
  return spend(ctx, parts.length === 0 ? label + " {}" : label + " { " + join(parts) + " }");
}

function formatPlainObject(ctx, value, depth) {
  const parts = [];
  const keys = ObjectKeys(value);
  const shown = keys.length > MAX_ENTRIES ? MAX_ENTRIES : keys.length;
  for (let i = 0; i < shown; i++) {
    const key = keys[i];
    const desc = ObjectGetOwnPropertyDescriptor(value, key);
    if (desc === undefined) continue;
    ArrayPrototypePush(parts, formatKey(key) + ": " + formatProperty(ctx, desc, depth));
  }
  if (keys.length > shown) {
    ArrayPrototypePush(parts, "... " + (keys.length - shown) + " more properties");
  }
  const symbols = ObjectGetOwnPropertySymbols(value);
  for (let i = 0; i < symbols.length && i < MAX_ENTRIES; i++) {
    if (ObjectPrototypePropertyIsEnumerable(value, symbols[i])) {
      const desc = ObjectGetOwnPropertyDescriptor(value, symbols[i]);
      if (desc === undefined) continue;
      ArrayPrototypePush(parts,
          "[" + SymbolPrototypeToString(symbols[i]) + "]: " + formatProperty(ctx, desc, depth));
    }
  }

  let prefix = "";
  const proto = ObjectGetPrototypeOf(value);
  if (proto === null) {
    prefix = "[Object: null prototype] ";
  } else {
    const ctorDesc = ObjectGetOwnPropertyDescriptor(proto, "constructor");
    const ctor = ctorDesc !== undefined ? ctorDesc.value : undefined;
    if (typeof ctor === "function") {
      const nameDesc = ObjectGetOwnPropertyDescriptor(ctor, "name");
      const name = nameDesc !== undefined ? nameDesc.value : undefined;
      if (typeof name === "string" && name !== "" && name !== "Object") {
        prefix = name + " ";
      }
    }
  }

  return spend(ctx, parts.length === 0 ? prefix + "{}" : prefix + "{ " + join(parts) + " }");
}

// A log call must never execute user code: accessors render as tags.
function formatProperty(ctx, desc, depth) {
  if (desc.get !== undefined) {
    return desc.set !== undefined ? "[Getter/Setter]" : "[Getter]";
  }
  if (desc.set !== undefined) {
    return "[Setter]";
  }
  return formatValue(ctx, desc.value, depth + 1);
}

const IDENT = /^[A-Za-z_$][A-Za-z0-9_$]*$/;
function formatKey(key) {
  return RegExpPrototypeTest(IDENT, key) ? key : "'" + key + "'";
}

function join(parts) {
  let out = "";
  for (let i = 0; i < parts.length; i++) {
    out += (i === 0 ? "" : ", ") + parts[i];
  }
  return out;
}

// Testability + app-level escape hatch (util.inspect-lite); installed eagerly
// by Console::Init running this builtin.
ObjectDefineProperty(global, "__inspect", {
  writable: false,
  enumerable: false,
  configurable: false,
  value: inspect,
});

module.exports = inspect;

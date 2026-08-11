"use strict";

// Snapshot of the intrinsics the other builtins depend on, taken before any
// user code can reach the globals. Runs first and is handed to every other
// builtin as the fourth fixed parameter.
//
// Instance methods are exposed "uncurried" (Node's idiom): the receiver
// becomes the first argument, so `ArrayPrototypeSlice(list, 0)` reads the
// captured Array.prototype.slice directly instead of a property of whatever
// `list.slice` resolves to at call time.

const FunctionPrototypeCall = Function.prototype.call;
const FunctionPrototypeBind = Function.prototype.bind;
const FunctionPrototypeApply = Function.prototype.apply;

// bind() with `this` pinned to call(): uncurryThis(fn) === fn.call.bind(fn),
// but without reading `fn.call`.
const uncurryThis = FunctionPrototypeBind.bind(FunctionPrototypeCall);

// Named `intrinsics` rather than `primordials`: the latter is this file's own
// (unused) parameter.
const intrinsics = {
  // Constructors.
  Date,
  Map,
  Proxy,
  String,
  TypeError,

  // Statics.
  ArrayIsArray: Array.isArray,
  JSONStringify: JSON.stringify,
  ObjectCreate: Object.create,
  ObjectDefineProperty: Object.defineProperty,
  ObjectKeys: Object.keys,

  // Instance methods, uncurried.
  ArrayPrototypeForEach: uncurryThis(Array.prototype.forEach),
  ArrayPrototypeIndexOf: uncurryThis(Array.prototype.indexOf),
  ArrayPrototypePush: uncurryThis(Array.prototype.push),
  ArrayPrototypeSlice: uncurryThis(Array.prototype.slice),
  ArrayPrototypeSplice: uncurryThis(Array.prototype.splice),
  DatePrototypeToJSON: uncurryThis(Date.prototype.toJSON),
  FunctionPrototypeApply: uncurryThis(FunctionPrototypeApply),
  FunctionPrototypeCall: uncurryThis(FunctionPrototypeCall),
  MapPrototypeDelete: uncurryThis(Map.prototype.delete),
  MapPrototypeGet: uncurryThis(Map.prototype.get),
  MapPrototypeSet: uncurryThis(Map.prototype.set),
  PromisePrototypeCatch: uncurryThis(Promise.prototype.catch),
  PromisePrototypeThen: uncurryThis(Promise.prototype.then),
};

Object.setPrototypeOf(intrinsics, null);
module.exports = Object.freeze(intrinsics);

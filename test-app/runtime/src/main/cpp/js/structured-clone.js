"use strict";

// WHATWG structuredClone(value, { transfer }): the argument coercion and the
// WebIDL sequence handling for `transfer`; the clone itself is native
// (v8::ValueSerializer round-tripped in this isolate).
//
// Deviation from the HTML spec, forced by the platform: ArrayBuffers and
// MessagePorts are transferable, nothing else is. ImageBitmap and the
// native/interop wrapper objects have no serialization form in this runtime,
// so they are rejected rather than half-supported. Clone failures are "DataCloneError"
// DOMExceptions, from here and from the native serializer alike
// (StructuredSerialization.cpp builds the same class).

const { clone } = binding;
const {
  ArrayBufferPrototypeGetByteLength,
  ArrayPrototypePush,
  FunctionPrototypeCall,
  ObjectPrototypeIsPrototypeOf,
  SymbolIterator,
  TypeError,
} = primordials;

var g = globalThis;

let DOMException;
function dataCloneError(message) {
  if (DOMException === undefined) {
    ({ DOMException } = require("internal/dom-exception"));
  }
  return new DOMException(message, "DataCloneError");
}

// Brand check through the captured byteLength getter: it is the one thing only
// a real ArrayBuffer has, and it cannot be faked by a `Symbol.toStringTag` or a
// forged prototype. SharedArrayBuffer has its own getter and so fails here,
// which is what the spec wants — a SAB is not transferable.
function isArrayBuffer(value) {
  if (value === null || typeof value !== "object") {
    return false;
  }
  try {
    ArrayBufferPrototypeGetByteLength(value);
    return true;
  } catch (notAnArrayBuffer) {
    return false;
  }
}

// The port check runs only for entries the ArrayBuffer test already rejected,
// so a transfer list of buffers never runs the messaging builtin.
let MessagePort;
function isMessagePort(value) {
  if (value === null || typeof value !== "object") {
    return false;
  }
  if (MessagePort === undefined) {
    ({ MessagePort } = require("internal/message-channel"));
  }
  return ObjectPrototypeIsPrototypeOf(MessagePort.prototype, value);
}

// WebIDL `sequence<object>` conversion: only an object with a callable
// @@iterator qualifies, which is why a string primitive is a TypeError even
// though strings are iterable.
function toTransferList(value) {
  if (value === null || (typeof value !== "object" && typeof value !== "function")) {
    throw new TypeError("structuredClone: transfer is not iterable");
  }
  var method = value[SymbolIterator];
  if (typeof method !== "function") {
    throw new TypeError("structuredClone: transfer is not iterable");
  }

  var iterator = FunctionPrototypeCall(method, value);
  if (iterator === null || typeof iterator !== "object") {
    throw new TypeError("structuredClone: transfer is not iterable");
  }

  // The iterator record captures `next` once, when it is created — re-reading
  // it per step would expose a `next` that changes mid-iteration.
  var next = iterator.next;
  if (typeof next !== "function") {
    throw new TypeError("structuredClone: transfer is not iterable");
  }

  var list = [];
  for (;;) {
    var step = FunctionPrototypeCall(next, iterator);
    if (step === null || typeof step !== "object") {
      throw new TypeError("structuredClone: transfer iterator returned a non-object");
    }
    if (step.done) {
      break;
    }
    var item = step.value;
    if (!isArrayBuffer(item) && !isMessagePort(item)) {
      throw dataCloneError("structuredClone: value in transfer list is not transferable");
    }
    ArrayPrototypePush(list, item);
  }
  return list;
}

// `options` is defaulted rather than merely optional so that the function's
// reported arity is 1, as the IDL requires.
g.structuredClone = function structuredClone(value, options = undefined) {
  if (arguments.length < 1) {
    throw new TypeError("structuredClone: 1 argument required, but only 0 present");
  }

  var transfer;
  if (options !== undefined && options !== null) {
    if (typeof options !== "object" && typeof options !== "function") {
      throw new TypeError("structuredClone: options is not an object");
    }
    var requested = options.transfer;
    if (requested !== undefined) {
      transfer = toTransferList(requested);
    }
  }

  return clone(value, transfer);
};

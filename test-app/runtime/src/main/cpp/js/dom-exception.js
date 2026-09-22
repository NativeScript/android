"use strict";
// DOMException (Web IDL Standard §4.3), the error type the web platform uses
// for named failures. Modeled on Node's per-context implementation: a plain
// class whose prototype is grafted onto Error.prototype, with the readonly
// name/message/code attributes as branded prototype getters (the private
// fields double as the Web IDL brand check).
//
// Lazy builtin: LazyGlobals places the global, and sibling builtins reach the
// constructor through require("internal/dom-exception") at throw time, so
// this file never runs in an app that never touches a DOMException.
//
// [Serializable]: the constructor stamps every instance with a native private
// brand (binding.markCloneable), which the serialization delegates in
// StructuredSerialization.cpp claim through V8's IsHostObject hook — name,
// message and stack travel across structuredClone and worker postMessage,
// Node's JSTransferable approach reduced to the one class.
const {
  ErrorCaptureStackTrace,
  ErrorPrototype,
  ObjectDefineProperty,
  ObjectSetPrototypeOf,
  SymbolToStringTag,
} = primordials;

const { markCloneable } = binding;

// Web IDL §4.3.4: the closed table of names with a legacy code. Any name
// outside it — including every post-table spec name — has code 0.
const nameToCode = {
  __proto__: null,
  IndexSizeError: 1,
  DOMStringSizeError: 2,
  HierarchyRequestError: 3,
  WrongDocumentError: 4,
  InvalidCharacterError: 5,
  NoDataAllowedError: 6,
  NoModificationAllowedError: 7,
  NotFoundError: 8,
  NotSupportedError: 9,
  InUseAttributeError: 10,
  InvalidStateError: 11,
  SyntaxError: 12,
  InvalidModificationError: 13,
  NamespaceError: 14,
  InvalidAccessError: 15,
  ValidationError: 16,
  TypeMismatchError: 17,
  SecurityError: 18,
  NetworkError: 19,
  AbortError: 20,
  URLMismatchError: 21,
  QuotaExceededError: 22,
  TimeoutError: 23,
  InvalidNodeTypeError: 24,
  DataCloneError: 25,
};

class DOMException {
  #name;
  #message;

  // The `= ""` / `= "Error"` defaults also give the constructor the arity the
  // IDL requires (0: both arguments optional).
  constructor(message = "", name = "Error") {
    this.#message = `${message}`;
    this.#name = `${name}`;
    ErrorCaptureStackTrace(this, DOMException);
    markCloneable(this);
  }

  get name() {
    return this.#name;
  }

  get message() {
    return this.#message;
  }

  get code() {
    const code = nameToCode[this.#name];
    return code === undefined ? 0 : code;
  }
}

// Web IDL inheritance: DOMException.prototype's parent is Error.prototype, so
// instanceof Error holds and Error.prototype.toString renders
// "name: message" through the getters above.
ObjectSetPrototypeOf(DOMException.prototype, ErrorPrototype);

// Class getters are non-enumerable; the IDL attributes are enumerable.
for (const key of ["name", "message", "code"]) {
  ObjectDefineProperty(DOMException.prototype, key, {
    __proto__: null,
    enumerable: true,
  });
}

ObjectDefineProperty(DOMException.prototype, SymbolToStringTag, {
  __proto__: null,
  configurable: true,
  value: "DOMException",
});

// The legacy code constants, on the interface object and on the prototype
// (Web IDL §3.7.5: both, { writable: false, enumerable: true,
// configurable: false }).
const constants = {
  __proto__: null,
  INDEX_SIZE_ERR: 1,
  DOMSTRING_SIZE_ERR: 2,
  HIERARCHY_REQUEST_ERR: 3,
  WRONG_DOCUMENT_ERR: 4,
  INVALID_CHARACTER_ERR: 5,
  NO_DATA_ALLOWED_ERR: 6,
  NO_MODIFICATION_ALLOWED_ERR: 7,
  NOT_FOUND_ERR: 8,
  NOT_SUPPORTED_ERR: 9,
  INUSE_ATTRIBUTE_ERR: 10,
  INVALID_STATE_ERR: 11,
  SYNTAX_ERR: 12,
  INVALID_MODIFICATION_ERR: 13,
  NAMESPACE_ERR: 14,
  INVALID_ACCESS_ERR: 15,
  VALIDATION_ERR: 16,
  TYPE_MISMATCH_ERR: 17,
  SECURITY_ERR: 18,
  NETWORK_ERR: 19,
  ABORT_ERR: 20,
  URL_MISMATCH_ERR: 21,
  QUOTA_EXCEEDED_ERR: 22,
  TIMEOUT_ERR: 23,
  INVALID_NODE_TYPE_ERR: 24,
  DATA_CLONE_ERR: 25,
};

for (const key in constants) {
  const descriptor = {
    __proto__: null,
    value: constants[key],
    writable: false,
    enumerable: true,
    configurable: false,
  };
  ObjectDefineProperty(DOMException, key, descriptor);
  ObjectDefineProperty(DOMException.prototype, key, descriptor);
}

module.exports = { DOMException };

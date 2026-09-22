"use strict";
// MessagePort / MessageChannel (HTML Standard §9.4) over the native messaging
// core (Messaging.cpp).
//
// The wrappers native code hands out — from createChannel, and from the
// deserializer for every port that arrives in a message — are bare objects
// carrying an internal field. `adoptPort` is what turns one into a
// MessagePort, and it is the only way an instance comes into being, which is
// why the constructor throws. Native must run it over every port wrapper it
// materializes that does not reach JS through emitMessage.
//
// Port enabling is HTML's: a port starts delivering when it gets its first
// 'message' listener — addEventListener or the onmessage attribute, including
// an `onmessage = null` first write — and stops when the last one goes. The
// events builtin's kListenerChanged hook is what reports those transitions.
// 'close' is delivered even to a port that was never started, so a port whose
// sibling died always learns about it.
const {
  createChannel,
  postMessage: postMessageToPort,
  start: startPort,
  stop: stopPort,
  close: closePort,
  drainOne,
  setEmitMessage,
} = binding;

const {
  ArrayIsArray,
  ArrayPrototypePush,
  FunctionPrototypeCall,
  ObjectCreate,
  ObjectDefineProperty,
  ObjectPrototypeHasOwnProperty,
  ObjectSetPrototypeOf,
  SymbolIterator,
  SymbolToStringTag,
  TypeError,
  WeakSet,
  WeakSetPrototypeAdd,
  WeakSetPrototypeDelete,
  WeakSetPrototypeHas,
} = primordials;

const {
  Event,
  EventTarget,
  defineEventHandler,
  kHandlerAssigned,
  kListenerChanged,
} = require("internal/events");

const addEventListener = EventTarget.prototype.addEventListener;
const dispatchEvent = EventTarget.prototype.dispatchEvent;

let MessageEvent;
function getMessageEvent() {
  if (MessageEvent === undefined) {
    ({ MessageEvent } = require("internal/message-event"));
  }
  return MessageEvent;
}

// WebIDL sequence<object>. Entries are handed to the native transfer-list
// collector unexamined: it owns the transferability rules and the
// DataCloneError messages that go with them.
function toTransferList(value) {
  if (value === undefined || value === null) {
    return undefined;
  }
  if (ArrayIsArray(value)) {
    return value;
  }
  if (typeof value !== "object" && typeof value !== "function") {
    throw new TypeError("postMessage: transfer is not iterable");
  }
  // The HTML overload: a second argument that is not itself iterable is the
  // StructuredSerializeOptions dictionary carrying the sequence.
  const source =
    typeof value[SymbolIterator] === "function" ? value : value.transfer;
  if (source === undefined || source === null) {
    return undefined;
  }
  if (ArrayIsArray(source)) {
    return source;
  }
  if (typeof source !== "object" && typeof source !== "function") {
    throw new TypeError("postMessage: transfer is not iterable");
  }
  const method = source[SymbolIterator];
  if (typeof method !== "function") {
    throw new TypeError("postMessage: transfer is not iterable");
  }
  return drainIterable(method, source);
}

function drainIterable(method, value) {
  const iterator = FunctionPrototypeCall(method, value);
  if (iterator === null || typeof iterator !== "object") {
    throw new TypeError("postMessage: transfer is not iterable");
  }
  const next = iterator.next;
  if (typeof next !== "function") {
    throw new TypeError("postMessage: transfer is not iterable");
  }
  const list = [];
  for (;;) {
    const step = FunctionPrototypeCall(next, iterator);
    if (step === null || typeof step !== "object") {
      throw new TypeError("postMessage: transfer iterator returned a non-object");
    }
    if (step.done) {
      break;
    }
    ArrayPrototypePush(list, step.value);
  }
  return list;
}

// Ports the native side is currently delivering to. The set is the idempotence
// guard for start/stop: the hook below sees every count transition, an explicit
// start() sees none.
const startedPorts = new WeakSet();

function listenerChanged(port, type, count) {
  if (type !== "message") {
    return;
  }
  if (count > 0) {
    if (!WeakSetPrototypeHas(startedPorts, port)) {
      WeakSetPrototypeAdd(startedPorts, port);
      startPort(port);
    }
  } else if (WeakSetPrototypeHas(startedPorts, port)) {
    WeakSetPrototypeDelete(startedPorts, port);
    stopPort(port);
  }
}

// HTML: the first time onmessage is set the port is enabled, as if start() had
// been called, even when the value assigned is null and adds no listener.
function handlerAssigned(port, type) {
  if (type !== "message" || WeakSetPrototypeHas(startedPorts, port)) {
    return;
  }
  WeakSetPrototypeAdd(startedPorts, port);
  startPort(port);
}

class MessagePort extends EventTarget {
  constructor() {
    throw new TypeError("Illegal constructor");
  }

  postMessage(value, transfer) {
    postMessageToPort(this, value, toTransferList(transfer));
  }

  start() {
    if (!WeakSetPrototypeHas(startedPorts, this)) {
      WeakSetPrototypeAdd(startedPorts, this);
      startPort(this);
    }
  }

  close(callback) {
    if (typeof callback === "function") {
      FunctionPrototypeCall(addEventListener, this, "close", callback, { once: true });
    }
    closePort(this);
  }
}

defineEventHandler(MessagePort.prototype, "message");
defineEventHandler(MessagePort.prototype, "messageerror");
defineEventHandler(MessagePort.prototype, "close");

ObjectDefineProperty(MessagePort.prototype, kHandlerAssigned, {
  __proto__: null,
  value: handlerAssigned,
  writable: false,
  enumerable: false,
  configurable: false,
});

ObjectDefineProperty(MessagePort.prototype, kListenerChanged, {
  __proto__: null,
  value: listenerChanged,
  writable: false,
  enumerable: false,
  configurable: false,
});

ObjectDefineProperty(MessagePort.prototype, SymbolToStringTag, {
  __proto__: null,
  value: "MessagePort",
  configurable: true,
});

for (const key of ["postMessage", "start", "close"]) {
  ObjectDefineProperty(MessagePort.prototype, key, {
    __proto__: null,
    enumerable: true,
  });
}

function adoptPort(port) {
  if (ObjectPrototypeHasOwnProperty(port, "_listeners")) {
    return port;
  }
  ObjectSetPrototypeOf(port, MessagePort.prototype);
  // The EventTarget base would install this as an own enumerable field; a port
  // is a platform object, so keep it out of Object.keys(port).
  ObjectDefineProperty(port, "_listeners", {
    __proto__: null,
    value: ObjectCreate(null),
    writable: true,
    enumerable: false,
    configurable: true,
  });
  return port;
}

class MessageChannel {
  constructor() {
    const pair = createChannel();
    this.port1 = adoptPort(pair[0]);
    this.port2 = adoptPort(pair[1]);
  }
}

ObjectDefineProperty(MessageChannel.prototype, SymbolToStringTag, {
  __proto__: null,
  value: "MessageChannel",
  configurable: true,
});

function receiveMessageOnPort(port) {
  const result = drainOne(port);
  return result === null ? undefined : result;
}

// The per-isolate delivery callout. Native invokes it with the receiving port
// wrapper as the receiver; `type` is "message", "messageerror" or "close".
function emitMessage(data, ports, type) {
  if (type === "close") {
    FunctionPrototypeCall(dispatchEvent, this, new Event("close"));
    return;
  }
  const list = [];
  if (ports !== undefined && ports !== null) {
    for (let i = 0; i < ports.length; i++) {
      ArrayPrototypePush(list, adoptPort(ports[i]));
    }
  }
  const MessageEventCtor = getMessageEvent();
  FunctionPrototypeCall(
    dispatchEvent,
    this,
    new MessageEventCtor(type, { data, ports: list })
  );
}

setEmitMessage(emitMessage);

module.exports = { MessagePort, MessageChannel, receiveMessageOnPort, adoptPort };

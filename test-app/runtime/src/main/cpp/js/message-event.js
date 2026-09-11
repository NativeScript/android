"use strict";
// MessageEvent (HTML Standard §9.2.5), the event every messaging surface in
// the runtime delivers: MessagePort, BroadcastChannel, Worker and the worker
// global scope.
//
// Lazy builtin: LazyGlobals places the global and the messaging builtins
// require this file at first delivery, so an app that never receives a message
// never runs it. Event/EventTarget come from require("internal/events") rather
// than the globals, which by then are whatever user code left behind.
const {
  ArrayPrototypePush,
  ArrayPrototypeSlice,
  FunctionPrototypeCall,
  ObjectDefineProperty,
  ObjectFreeze,
  SymbolIterator,
  SymbolToStringTag,
  TypeError,
} = primordials;

const { Event } = require("internal/events");

// WebIDL sequence<MessagePort>. Entry types are not checked here: the ports an
// event carries come from the native deserializer, and a hand-built event's
// `ports` is inert data.
function toPortSequence(value) {
  if (value === null || (typeof value !== "object" && typeof value !== "function")) {
    throw new TypeError("MessageEvent: ports is not iterable");
  }
  const method = value[SymbolIterator];
  if (typeof method !== "function") {
    throw new TypeError("MessageEvent: ports is not iterable");
  }
  const iterator = FunctionPrototypeCall(method, value);
  if (iterator === null || typeof iterator !== "object") {
    throw new TypeError("MessageEvent: ports is not iterable");
  }
  const next = iterator.next;
  if (typeof next !== "function") {
    throw new TypeError("MessageEvent: ports is not iterable");
  }
  const list = [];
  for (;;) {
    const step = FunctionPrototypeCall(next, iterator);
    if (step === null || typeof step !== "object") {
      throw new TypeError("MessageEvent: ports iterator returned a non-object");
    }
    if (step.done) {
      break;
    }
    ArrayPrototypePush(list, step.value);
  }
  return list;
}

class MessageEvent extends Event {
  #data;
  #origin;
  #lastEventId;
  #source;
  #ports;

  constructor(type, init = undefined) {
    if (arguments.length < 1) {
      throw new TypeError("MessageEvent: 1 argument required, but only 0 present");
    }
    if (init !== undefined && init !== null &&
        typeof init !== "object" && typeof init !== "function") {
      throw new TypeError("MessageEvent: eventInitDict is not an object");
    }
    super(type, init);
    const options = init === undefined || init === null ? {} : init;
    this.#data = options.data !== undefined ? options.data : null;
    this.#origin = options.origin !== undefined ? `${options.origin}` : "";
    this.#lastEventId =
      options.lastEventId !== undefined ? `${options.lastEventId}` : "";
    this.#source = options.source !== undefined ? options.source : null;
    this.#ports =
      options.ports !== undefined && options.ports !== null
        ? toPortSequence(options.ports)
        : [];
  }

  get data() {
    return this.#data;
  }

  get origin() {
    return this.#origin;
  }

  get lastEventId() {
    return this.#lastEventId;
  }

  get source() {
    return this.#source;
  }

  get ports() {
    // A frozen copy per read: freezing the backing array in place would let a
    // caller's reference alias the event's own state.
    return ObjectFreeze(ArrayPrototypeSlice(this.#ports));
  }

  initMessageEvent(
    type,
    bubbles = false,
    cancelable = false,
    data = null,
    origin = "",
    lastEventId = "",
    source = null,
    ports = []
  ) {
    if (arguments.length < 1) {
      throw new TypeError("initMessageEvent: 1 argument required, but only 0 present");
    }
    // Event's initialize steps are a no-op while the event is being
    // dispatched; currentTarget is what marks that window.
    if (this.currentTarget !== null) {
      return;
    }
    this.type = `${type}`;
    this.bubbles = !!bubbles;
    this.cancelable = !!cancelable;
    this.defaultPrevented = false;
    this.target = null;
    this._stopPropagation = false;
    this._stopImmediate = false;
    this.#data = data;
    this.#origin = `${origin}`;
    this.#lastEventId = `${lastEventId}`;
    this.#source = source;
    this.#ports = ports === null ? [] : toPortSequence(ports);
  }
}

// Class members are non-enumerable; the IDL attributes and operations are not.
for (const key of ["data", "origin", "lastEventId", "source", "ports", "initMessageEvent"]) {
  ObjectDefineProperty(MessageEvent.prototype, key, {
    __proto__: null,
    enumerable: true,
  });
}

ObjectDefineProperty(MessageEvent.prototype, SymbolToStringTag, {
  __proto__: null,
  value: "MessageEvent",
  configurable: true,
});

module.exports = { MessageEvent };

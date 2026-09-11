"use strict";
// BroadcastChannel (HTML Standard §9.5): every channel constructed with the
// same name joins one process-wide group, workers included — "same user agent"
// is the app process here.
//
// A channel owns a hidden MessagePort in that named group. The port is started
// and strongly held from construction (native holds the wrapper, the wrapper
// holds the relay listener, the relay holds the channel), so an unclosed
// channel stays deliverable whether or not app code keeps a reference — and
// close() is what ends that.
const { createBroadcastPort, postMessage: postMessageToPort, close: closePort } =
  binding;

const {
  FunctionPrototypeCall,
  ObjectDefineProperty,
  SymbolToStringTag,
  TypeError,
} = primordials;

const { EventTarget, defineEventHandler } = require("internal/events");
const { adoptPort } = require("internal/message-channel");

const addEventListener = EventTarget.prototype.addEventListener;
const dispatchEvent = EventTarget.prototype.dispatchEvent;

let MessageEvent;
function getMessageEvent() {
  if (MessageEvent === undefined) {
    ({ MessageEvent } = require("internal/message-event"));
  }
  return MessageEvent;
}

let DOMException;
function getDOMException() {
  if (DOMException === undefined) {
    ({ DOMException } = require("internal/dom-exception"));
  }
  return DOMException;
}

class BroadcastChannel extends EventTarget {
  #name;
  #port;

  constructor(name) {
    if (arguments.length < 1) {
      throw new TypeError("BroadcastChannel: 1 argument required, but only 0 present");
    }
    super();
    ObjectDefineProperty(this, "_listeners", {
      __proto__: null,
      value: this._listeners,
      writable: true,
      enumerable: false,
      configurable: true,
    });
    this.#name = `${name}`;
    const port = adoptPort(createBroadcastPort(this.#name));
    this.#port = port;
    const channel = this;
    const relay = function (event) {
      FunctionPrototypeCall(
        dispatchEvent,
        channel,
        new (getMessageEvent())(event.type, { data: event.data })
      );
    };
    FunctionPrototypeCall(addEventListener, port, "message", relay);
    FunctionPrototypeCall(addEventListener, port, "messageerror", relay);
  }

  get name() {
    return this.#name;
  }

  postMessage(message) {
    if (arguments.length < 1) {
      throw new TypeError("postMessage: 1 argument required, but only 0 present");
    }
    if (this.#port === undefined) {
      throw new (getDOMException())(
        "BroadcastChannel is closed.",
        "InvalidStateError"
      );
    }
    // No transfer list: the spec's postMessage takes the message alone, and a
    // fan-out message could not hand one object to every destination anyway.
    postMessageToPort(this.#port, message, undefined);
  }

  close() {
    if (this.#port === undefined) {
      return;
    }
    const port = this.#port;
    this.#port = undefined;
    closePort(port);
  }
}

defineEventHandler(BroadcastChannel.prototype, "message");
defineEventHandler(BroadcastChannel.prototype, "messageerror");

for (const key of ["name", "postMessage", "close"]) {
  ObjectDefineProperty(BroadcastChannel.prototype, key, {
    __proto__: null,
    enumerable: true,
  });
}

ObjectDefineProperty(BroadcastChannel.prototype, SymbolToStringTag, {
  __proto__: null,
  value: "BroadcastChannel",
  configurable: true,
});

module.exports = { BroadcastChannel };

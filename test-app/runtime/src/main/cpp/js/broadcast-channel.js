"use strict";
// BroadcastChannel (HTML Standard §9.5): every channel constructed with the
// same name joins one process-wide group, workers included — "same user agent"
// is the app process here.
//
// A channel owns a hidden port in that named group, and native delivers
// straight to the channel: one MessageEvent per message, nothing dispatched on
// the port.
//
// Lifetime follows the spec's GC rule. While the channel is open and has a
// 'message' or 'messageerror' listener, native holds it strongly as the port's
// delivery target; that is the only strong path to a channel app code dropped.
// Without such a listener nothing native reaches it, a message arriving is
// discarded unread, and once app code lets go the channel is collected and the
// registry below takes its port out of the group.
const {
  createBroadcastPort,
  postMessage: postMessageToPort,
  close: closePort,
  setBroadcastTarget,
} = binding;

const {
  FinalizationRegistry,
  FinalizationRegistryPrototypeRegister,
  FinalizationRegistryPrototypeUnregister,
  ObjectDefineProperty,
  SymbolToStringTag,
  TypeError,
} = primordials;

const { EventTarget, defineEventHandler, kListenerChanged } =
  require("internal/events");

let DOMException;
function getDOMException() {
  if (DOMException === undefined) {
    ({ DOMException } = require("internal/dom-exception"));
  }
  return DOMException;
}

const portRegistry = new FinalizationRegistry(closePort);

const kHeardMessage = 1;
const kHeardMessageError = 2;

let listenerChanged;

class BroadcastChannel extends EventTarget {
  #name;
  #port;
  // kHeardMessage | kHeardMessageError, for whichever has a listener.
  #heard = 0;

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
    this.#port = createBroadcastPort(this.#name);
    FinalizationRegistryPrototypeRegister(portRegistry, this, this.#port, this);
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
    FinalizationRegistryPrototypeUnregister(portRegistry, this);
    closePort(port);
  }

  static {
    listenerChanged = (channel, type, count) => {
      // The hook is found through the prototype, so it also runs for objects
      // that merely inherit from it.
      if (!(#port in channel) || channel.#port === undefined) {
        return;
      }
      let bit;
      if (type === "message") {
        bit = kHeardMessage;
      } else if (type === "messageerror") {
        bit = kHeardMessageError;
      } else {
        return;
      }
      const before = channel.#heard;
      const after = count > 0 ? before | bit : before & ~bit;
      channel.#heard = after;
      if ((before === 0) !== (after === 0)) {
        setBroadcastTarget(channel.#port, after === 0 ? undefined : channel);
      }
    };
  }
}

defineEventHandler(BroadcastChannel.prototype, "message");
defineEventHandler(BroadcastChannel.prototype, "messageerror");

ObjectDefineProperty(BroadcastChannel.prototype, kListenerChanged, {
  __proto__: null,
  value: listenerChanged,
  writable: false,
  enumerable: false,
  configurable: false,
});

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

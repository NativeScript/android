"use strict";
// AbortController / AbortSignal (DOM Standard §3.2) with the AbortSignal
// abort / timeout / any statics, modeled on Node's
// internal/abort_controller.js.
//
// Deliberate deviations from Node:
// - No DOMException in this runtime: the default abort and timeout reasons
//   are Error instances with `name` patched ("AbortError" / "TimeoutError"),
//   the same stand-in performance.js and structured-clone.js use.
// - No WeakRef bookkeeping: a timeout() timer holds its signal strongly until
//   it fires (retention is bounded by the delay, and looper timers don't
//   gate process liveness the way Node's do), and any() links source ->
//   dependent strongly, unlinking as soon as either side aborts.
const {
  ArrayPrototypeIndexOf,
  ArrayPrototypePush,
  ArrayPrototypeSplice,
  Error,
  FunctionPrototypeCall,
  NumberIsInteger,
  ObjectDefineProperty,
  ObjectGetOwnPropertyDescriptor,
  RangeError,
  SymbolIterator,
  SymbolToStringTag,
  TypeError,
} = primordials;
var g = globalThis;
// Init order (PrepareV8Runtime): events.js ran immediately before this builtin,
// and the timer natives are template globals present from context creation;
// captured before user code can replace them.
const EventTarget = g.EventTarget;
const Event = g.Event;
const setTimeout = g.setTimeout;
const dispatchEvent = EventTarget.prototype.dispatchEvent;
const addEventListener = EventTarget.prototype.addEventListener;
const removeEventListener = EventTarget.prototype.removeEventListener;

// Construction token: AbortSignal instances come only from the factories in
// this module (the controller, and the abort/timeout/any statics).
const kInternal = {};

function abortError() {
  const e = new Error("This operation was aborted");
  e.name = "AbortError";
  return e;
}

function timeoutError() {
  const e = new Error("The operation was aborted due to timeout");
  e.name = "TimeoutError";
  return e;
}

let createAbortSignal;
let signalAbort;

class AbortSignal extends EventTarget {
  #aborted = false;
  #reason = undefined;
  // Event handler attribute state (HTML semantics: registered as a plain
  // listener on the first non-null assignment, so its slot in the listener
  // order is where it was first set; cleared assignments free the slot).
  #onabort = null;
  #onabortWrapper = null;
  // any() linkage. #sources: the plain signals a live composite follows
  // (null on plain signals and once aborted — composites never nest, any()
  // flattens). #dependents: the live composites following this signal.
  #composite = false;
  #sources = null;
  #dependents = null;

  constructor(token) {
    if (token !== kInternal) {
      throw new TypeError("Illegal constructor");
    }
    super();
    // The EventTarget base installs `_listeners` as an own enumerable field;
    // keep it out of Object.keys(signal)/JSON.stringify(signal).
    ObjectDefineProperty(this, "_listeners", {
      value: this._listeners,
      writable: true,
      enumerable: false,
      configurable: true,
    });
  }

  get aborted() {
    return this.#aborted;
  }

  get reason() {
    return this.#reason;
  }

  throwIfAborted() {
    if (this.#aborted) {
      throw this.#reason;
    }
  }

  get onabort() {
    return this.#onabort;
  }

  set onabort(handler) {
    // TreatNonObjectAsNull: objects and functions are stored, any other value
    // clears the handler; only a function is invoked at dispatch time.
    const value =
      typeof handler === "function" ||
      (handler !== null && typeof handler === "object")
        ? handler
        : null;
    if (value !== null && this.#onabort === null) {
      if (this.#onabortWrapper === null) {
        const self = this;
        this.#onabortWrapper = function (event) {
          const cb = self.#onabort;
          if (typeof cb === "function") {
            FunctionPrototypeCall(cb, self, event);
          }
        };
      }
      FunctionPrototypeCall(
        addEventListener,
        this,
        "abort",
        this.#onabortWrapper
      );
    } else if (value === null && this.#onabort !== null) {
      FunctionPrototypeCall(
        removeEventListener,
        this,
        "abort",
        this.#onabortWrapper
      );
    }
    this.#onabort = value;
  }

  static abort(reason) {
    return createAbortSignal(
      true,
      reason === undefined ? abortError() : reason
    );
  }

  static timeout(delay) {
    if (typeof delay !== "number") {
      throw new TypeError('The "delay" argument must be of type number');
    }
    if (!NumberIsInteger(delay) || delay < 0 || delay > 4294967295) {
      throw new RangeError(
        'The value of "delay" is out of range. It must be an integer >= 0 ' +
          "and <= 4294967295. Received " +
          delay
      );
    }
    const signal = createAbortSignal(false, undefined);
    setTimeout(function () {
      signalAbort(signal, timeoutError());
    }, delay);
    return signal;
  }

  static any(signals) {
    // WebIDL sequence<AbortSignal>: an iterable of AbortSignals converts
    // (so a Set works and a string does not); anything else is a TypeError,
    // never a silent no-op.
    if (
      signals === null ||
      (typeof signals !== "object" && typeof signals !== "function") ||
      typeof signals[SymbolIterator] !== "function"
    ) {
      throw new TypeError("signals is not iterable");
    }
    const list = [];
    for (const s of signals) {
      if (
        s === null ||
        (typeof s !== "object" && typeof s !== "function") ||
        !(#aborted in s)
      ) {
        throw new TypeError(
          "signals must contain only AbortSignal instances"
        );
      }
      ArrayPrototypePush(list, s);
    }
    const result = new AbortSignal(kInternal);
    result.#composite = true;
    // The first aborted input wins, before any linking happens; no abort
    // event fires because nothing can be listening on `result` yet.
    for (let i = 0; i < list.length; i++) {
      if (list[i].#aborted) {
        result.#aborted = true;
        result.#reason = list[i].#reason;
        return result;
      }
    }
    result.#sources = [];
    for (let i = 0; i < list.length; i++) {
      const s = list[i];
      if (s.#composite) {
        // A live composite's sources are all live (it would have aborted with
        // them otherwise); one with no sources can never abort and
        // contributes nothing.
        const underlying = s.#sources;
        for (let j = 0; j < underlying.length; j++) {
          AbortSignal.#link(result, underlying[j]);
        }
      } else {
        AbortSignal.#link(result, s);
      }
    }
    return result;
  }

  static #link(result, source) {
    if (ArrayPrototypeIndexOf(result.#sources, source) !== -1) {
      return;
    }
    ArrayPrototypePush(result.#sources, source);
    if (source.#dependents === null) {
      source.#dependents = [];
    }
    ArrayPrototypePush(source.#dependents, result);
  }

  static {
    createAbortSignal = (aborted, reason) => {
      const signal = new AbortSignal(kInternal);
      signal.#aborted = aborted;
      signal.#reason = reason;
      return signal;
    };

    // Detach an aborted composite from the sources it was following so a
    // long-lived source doesn't retain it (and its listeners) forever.
    const unlink = (signal) => {
      const sources = signal.#sources;
      if (sources === null) {
        return;
      }
      signal.#sources = null;
      for (let i = 0; i < sources.length; i++) {
        const dependents = sources[i].#dependents;
        if (dependents !== null) {
          const idx = ArrayPrototypeIndexOf(dependents, signal);
          if (idx !== -1) {
            ArrayPrototypeSplice(dependents, idx, 1);
          }
        }
      }
    };

    const fireAbort = (signal) => {
      // Captured dispatch: controller.abort() must keep working even if app
      // code replaced signal.dispatchEvent.
      FunctionPrototypeCall(dispatchEvent, signal, new Event("abort"));
    };

    // https://dom.spec.whatwg.org/#abortsignal-signal-abort — every affected
    // signal (the source and its dependent composites) flips its state before
    // the first abort event fires.
    signalAbort = (signal, reason) => {
      if (signal.#aborted) {
        return;
      }
      signal.#aborted = true;
      signal.#reason = reason;
      unlink(signal);
      const dependents = signal.#dependents;
      signal.#dependents = null;
      const toAbort = [];
      if (dependents !== null) {
        for (let i = 0; i < dependents.length; i++) {
          const d = dependents[i];
          if (!d.#aborted) {
            d.#aborted = true;
            d.#reason = reason;
            ArrayPrototypePush(toAbort, d);
          }
        }
      }
      fireAbort(signal);
      for (let i = 0; i < toAbort.length; i++) {
        unlink(toAbort[i]);
        fireAbort(toAbort[i]);
      }
    };
  }
}

class AbortController {
  #signal = createAbortSignal(false, undefined);

  get signal() {
    return this.#signal;
  }

  abort(reason) {
    signalAbort(
      this.#signal,
      reason === undefined ? abortError() : reason
    );
  }
}

// WebIDL shape: interface members are enumerable prototype properties, static
// operations are enumerable own properties of the interface object, and the
// class string is a configurable, non-writable Symbol.toStringTag; class
// syntax alone yields non-enumerable members.
function finishInterface(ctor, tag, members, staticMembers) {
  const proto = ctor.prototype;
  ObjectDefineProperty(proto, SymbolToStringTag, {
    value: tag,
    writable: false,
    enumerable: false,
    configurable: true,
  });
  for (let i = 0; i < members.length; i++) {
    const desc = ObjectGetOwnPropertyDescriptor(proto, members[i]);
    desc.enumerable = true;
    ObjectDefineProperty(proto, members[i], desc);
  }
  for (let i = 0; i < staticMembers.length; i++) {
    const desc = ObjectGetOwnPropertyDescriptor(ctor, staticMembers[i]);
    desc.enumerable = true;
    ObjectDefineProperty(ctor, staticMembers[i], desc);
  }
}
finishInterface(
  AbortSignal,
  "AbortSignal",
  ["aborted", "reason", "onabort", "throwIfAborted"],
  ["abort", "timeout", "any"]
);
finishInterface(AbortController, "AbortController", ["signal", "abort"], []);

g.AbortController = AbortController;
g.AbortSignal = AbortSignal;

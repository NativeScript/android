"use strict";
// AbortController / AbortSignal (DOM Standard §3.2) with the AbortSignal
// abort / timeout / any statics, modeled on Node's
// internal/abort_controller.js.
//
// GC contract (Node-equivalent, see docs/abort-signal.md):
// - A timeout() timer and every any() link hold only WeakRefs, so a signal
//   nobody can observe is collectable before its abort would ever fire, and
//   per-request composites never accumulate on a long-lived source.
// - Weakness alone would drop aborts for signals that are listened-to but
//   otherwise unreachable, so gcPersistentSignals strong-holds exactly the
//   signals whose abort someone can still observe: live timeout signals and
//   live non-empty composites while they have abort listeners, and timeout
//   sources a composite follows until their timer fires. The listener
//   accounting comes from the events builtin's kListenerChanged hook, which
//   fires from every listener-list mutation path and cannot be bypassed
//   from app code.
//
// The default abort and timeout reasons are DOMExceptions ("AbortError" /
// "TimeoutError"), required from the internal tier on first use so an app
// that never aborts never runs the dom-exception builtin.
const {
  ArrayPrototypeIndexOf,
  ArrayPrototypePush,
  ArrayPrototypeSplice,
  FinalizationRegistry,
  FinalizationRegistryPrototypeRegister,
  FinalizationRegistryPrototypeUnregister,
  FunctionPrototypeCall,
  NumberIsInteger,
  ObjectDefineProperty,
  ObjectGetOwnPropertyDescriptor,
  RangeError,
  Set,
  SetPrototypeAdd,
  SetPrototypeDelete,
  SymbolIterator,
  SymbolToStringTag,
  TypeError,
  WeakRef,
  WeakRefPrototypeDeref,
} = primordials;
var g = globalThis;
// Init order (PrepareV8Runtime): events.js ran immediately before this builtin,
// and the timer natives are template globals present from context creation;
// captured before user code can replace them.
const EventTarget = g.EventTarget;
const Event = g.Event;
const setTimeout = g.setTimeout;
const clearTimeout = g.clearTimeout;
const dispatchEvent = EventTarget.prototype.dispatchEvent;
// Published by events.js: the symbol under which EventTargetImpl looks up
// the listener-mutation hook, and the shared event-handler-attribute helper
// (whose wrapper registration routes through the same hook). events.js
// already ran (Events::Init), so this require is a cache hit; a miss would
// run it on demand rather than fail.
const { defineEventHandler, kListenerChanged } = require("internal/events");

// Construction token: AbortSignal instances come only from the factories in
// this module (the controller, and the abort/timeout/any statics).
const kInternal = {};

let DOMException;
function getDOMException() {
  if (DOMException === undefined) {
    ({ DOMException } = require("internal/dom-exception"));
  }
  return DOMException;
}

function abortError() {
  return new (getDOMException())("This operation was aborted", "AbortError");
}

function timeoutError() {
  return new (getDOMException())(
    "The operation was aborted due to timeout",
    "TimeoutError"
  );
}

// The strong holds described in the header. Entries leave on abort, on the
// last abort-listener removal, or when a composite loses its last source
// (at which point nothing can ever abort it).
const gcPersistentSignals = new Set();

// Cancels the pending native timer of a collected timeout() signal.
const timerRegistry = new FinalizationRegistry(function (timerId) {
  clearTimeout(timerId);
});

let createAbortSignal;
let signalAbort;
let listenerChanged;
let dependentPrune;
let sourcePrune;
// Prune callbacks arrive as posted GC-cleanup tasks; the registries are
// constructed after the static block assigns the callbacks.
let dependentPruneRegistry;
let sourcePruneRegistry;

class AbortSignal extends EventTarget {
  #aborted = false;
  #reason = undefined;
  #isTimeout = false;
  // any() linkage, all WeakRefs. #sources: the plain sources a live
  // composite follows (null on plain signals and once aborted — composites
  // never nest, any() flattens). #dependents: the live composites following
  // this signal. #selfRef: the one WeakRef identity other signals hold for
  // this one; doubles as the unregister token for the prune registries.
  #composite = false;
  #sources = null;
  #dependents = null;
  #selfRef = null;

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
    signal.#isTimeout = true;
    // The timer closes over a WeakRef so an unobservable signal is
    // collectable before it fires; the registry cancels the native timer if
    // that happens. While the signal has abort listeners the hook below
    // strong-holds it, so a pending observable abort is never dropped.
    const ref = new WeakRef(signal);
    const timerId = setTimeout(function () {
      const s = WeakRefPrototypeDeref(ref);
      if (s !== undefined) {
        FinalizationRegistryPrototypeUnregister(timerRegistry, s);
        signalAbort(s, timeoutError());
      }
    }, delay);
    FinalizationRegistryPrototypeRegister(timerRegistry, signal, timerId, signal);
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
    const resultRef = (result.#selfRef = new WeakRef(result));
    for (let i = 0; i < list.length; i++) {
      const s = list[i];
      if (s.#composite) {
        // A live composite's sources are all live (it would have aborted
        // with them otherwise); one with no sources can never abort and
        // contributes nothing.
        const underlying = s.#sources;
        for (let j = 0; j < underlying.length; j++) {
          const src = WeakRefPrototypeDeref(underlying[j]);
          if (src !== undefined) {
            AbortSignal.#link(result, resultRef, src);
          }
        }
      } else {
        AbortSignal.#link(result, resultRef, s);
      }
    }
    return result;
  }

  static #link(result, resultRef, source) {
    let sourceRef = source.#selfRef;
    if (sourceRef === null) {
      sourceRef = source.#selfRef = new WeakRef(source);
    }
    if (ArrayPrototypeIndexOf(result.#sources, sourceRef) !== -1) {
      return;
    }
    ArrayPrototypePush(result.#sources, sourceRef);
    if (source.#dependents === null) {
      source.#dependents = [];
    }
    ArrayPrototypePush(source.#dependents, resultRef);
    // A timeout source followed only weakly would be collectable once app
    // code drops it, silently never aborting the composite: hold it until
    // its timer fires (retention bounded by the delay).
    if (source.#isTimeout && !source.#aborted) {
      SetPrototypeAdd(gcPersistentSignals, source);
    }
    FinalizationRegistryPrototypeRegister(
      dependentPruneRegistry, result, { sourceRef, resultRef }, resultRef);
    FinalizationRegistryPrototypeRegister(
      sourcePruneRegistry, source, { sourceRef, resultRef }, resultRef);
  }

  static {
    createAbortSignal = (aborted, reason) => {
      const signal = new AbortSignal(kInternal);
      signal.#aborted = aborted;
      signal.#reason = reason;
      return signal;
    };

    // Detach an aborted composite from the sources it was following, and
    // drop both prune registrations (their WeakRefs are dead weight once the
    // links are gone).
    const unlink = (signal) => {
      const sources = signal.#sources;
      if (sources === null) {
        return;
      }
      signal.#sources = null;
      const selfRef = signal.#selfRef;
      if (selfRef !== null) {
        FinalizationRegistryPrototypeUnregister(dependentPruneRegistry, selfRef);
        FinalizationRegistryPrototypeUnregister(sourcePruneRegistry, selfRef);
      }
      for (let i = 0; i < sources.length; i++) {
        const source = WeakRefPrototypeDeref(sources[i]);
        if (source !== undefined && source.#dependents !== null) {
          const idx = ArrayPrototypeIndexOf(source.#dependents, selfRef);
          if (idx !== -1) {
            ArrayPrototypeSplice(source.#dependents, idx, 1);
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
      SetPrototypeDelete(gcPersistentSignals, signal);
      unlink(signal);
      const dependents = signal.#dependents;
      signal.#dependents = null;
      const toAbort = [];
      if (dependents !== null) {
        for (let i = 0; i < dependents.length; i++) {
          const d = WeakRefPrototypeDeref(dependents[i]);
          if (d !== undefined && !d.#aborted) {
            d.#aborted = true;
            d.#reason = reason;
            ArrayPrototypePush(toAbort, d);
          }
        }
      }
      fireAbort(signal);
      for (let i = 0; i < toAbort.length; i++) {
        const d = toAbort[i];
        SetPrototypeDelete(gcPersistentSignals, d);
        unlink(d);
        fireAbort(d);
      }
    };

    // Listener accounting (events.js kListenerChanged hook, installed on the
    // prototype below): a live timeout signal or non-empty composite is
    // strong-held exactly while an abort listener could observe its abort.
    listenerChanged = (signal, type, count) => {
      if (type !== "abort") {
        return;
      }
      if (signal.#aborted) {
        SetPrototypeDelete(gcPersistentSignals, signal);
        return;
      }
      const needsPersist =
        signal.#isTimeout ||
        (signal.#composite &&
          signal.#sources !== null &&
          signal.#sources.length > 0);
      if (!needsPersist) {
        return;
      }
      if (count > 0) {
        SetPrototypeAdd(gcPersistentSignals, signal);
      } else {
        SetPrototypeDelete(gcPersistentSignals, signal);
      }
    };

    // A collected composite leaves each surviving source's dependent list,
    // and its remaining pair registrations go with it.
    dependentPrune = ({ sourceRef, resultRef }) => {
      FinalizationRegistryPrototypeUnregister(sourcePruneRegistry, resultRef);
      const source = WeakRefPrototypeDeref(sourceRef);
      if (source === undefined || source.#dependents === null) {
        return;
      }
      const idx = ArrayPrototypeIndexOf(source.#dependents, resultRef);
      if (idx !== -1) {
        ArrayPrototypeSplice(source.#dependents, idx, 1);
      }
    };

    // A collected source leaves the composite's source list; a composite
    // with no sources left can never abort, so it stops being strong-held
    // even if listeners remain.
    sourcePrune = ({ sourceRef, resultRef }) => {
      const composite = WeakRefPrototypeDeref(resultRef);
      if (composite === undefined || composite.#sources === null) {
        return;
      }
      const idx = ArrayPrototypeIndexOf(composite.#sources, sourceRef);
      if (idx !== -1) {
        ArrayPrototypeSplice(composite.#sources, idx, 1);
      }
      if (composite.#sources.length === 0) {
        SetPrototypeDelete(gcPersistentSignals, composite);
      }
    };
  }
}

dependentPruneRegistry = new FinalizationRegistry(dependentPrune);
sourcePruneRegistry = new FinalizationRegistry(sourcePrune);

ObjectDefineProperty(AbortSignal.prototype, kListenerChanged, {
  value: listenerChanged,
  writable: false,
  enumerable: false,
  configurable: false,
});

defineEventHandler(AbortSignal.prototype, "abort");

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

"use strict";
const {
  ArrayPrototypeIndexOf,
  ArrayPrototypePush,
  ArrayPrototypeSlice,
  ArrayPrototypeSplice,
  FunctionPrototypeCall,
  ObjectCreate,
  ObjectDefineProperty,
  String,
} = primordials;
var g = globalThis;

function Event(type, opts) {
  opts = opts || {};
  this.type = String(type);
  this.bubbles = !!opts.bubbles;
  this.cancelable = !!opts.cancelable;
  this.composed = !!opts.composed;
  this.defaultPrevented = false;
  this.target = null;
  this.currentTarget = null;
  this._stopPropagation = false;
  this._stopImmediate = false;
}
Event.prototype.preventDefault = function () {
  if (this.cancelable) { this.defaultPrevented = true; }
};
Event.prototype.stopPropagation = function () { this._stopPropagation = true; };
Event.prototype.stopImmediatePropagation = function () {
  this._stopPropagation = true;
  this._stopImmediate = true;
};

// A listener that throws must not stop other listeners: route the thrown
// value to the native fatal tail instead of ever recursively dispatching
// another `error` event from inside dispatch. The error-events layer
// installs the real reporter through this file's exports (before any user
// code runs); until then a thrown listener is swallowed.
var reportListenerError = function (e) {};
function setListenerErrorReporter(fn) {
  reportListenerError = fn;
}

// Event name -> handler-attribute wrapper (see defineEventHandler), stored on
// the target's own listener bag under a symbol so it cannot collide with an
// event type. Deliberately NOT a WeakMap keyed by the target: the wrappers
// live with the target, as Node keeps them, and stay independent of how the
// collector treats weak-collection entries of objects that native code keeps
// alive. Each wrapper carries a `delta` that the listener count is corrected
// by: the wrapper occupies one slot in the listener list from its first
// assignment onwards, but a cleared handler is not a listener.
var kHandlers = Symbol("handlers");

function handlersOf(target) {
  var bag = target._listeners;
  return bag === undefined ? undefined : bag[kHandlers];
}

// Internal listener-mutation hook. A target (in practice: AbortSignal and
// MessagePort, on their prototypes) may carry a function under this symbol;
// it is called with (target, type, newCount) from every path that changes a
// listener list — add, remove, the once-splice inside dispatch, and a handler
// attribute going active or inert. The key travels only through
// require("internal/events"), so the accounting cannot be bypassed the way an
// overridable addEventListener could.
var kListenerChanged = Symbol("listenerChanged");
// Called on the first handler-attribute assignment, whatever the value: HTML
// enables a MessagePort the first time onmessage is set, even to null, which
// the listener count cannot express. Later assignments only move the count.
var kHandlerAssigned = Symbol("handlerAssigned");
function notifyListenerChanged(target, type, count) {
  var hook = target[kListenerChanged];
  if (hook === undefined) { return; }
  var wrappers = handlersOf(target);
  if (wrappers !== undefined) {
    var wrapper = wrappers[type];
    if (wrapper !== undefined) { count += wrapper.delta; }
  }
  hook(target, type, count);
}

function EventTargetImpl() { this._listeners = ObjectCreate(null); }

// A target whose prototype was grafted onto EventTarget.prototype rather than
// built by the constructor — Worker, MessagePort — has no bag until it needs
// one. Non-enumerable, because those are platform objects.
function listenersOf(target) {
  var bag = target._listeners;
  if (bag === undefined) {
    bag = ObjectCreate(null);
    ObjectDefineProperty(target, "_listeners", {
      value: bag,
      writable: true,
      enumerable: false,
      configurable: true,
    });
  }
  return bag;
}

EventTargetImpl.prototype.addEventListener = function (type, callback, options) {
  if (callback === null || callback === undefined) { return; }
  type = String(type);
  var capture = false, once = false;
  if (typeof options === "boolean") {
    capture = options;
  } else if (options && typeof options === "object") {
    capture = !!options.capture;
    once = !!options.once;
  }
  var bag = listenersOf(this);
  var list = bag[type];
  if (!list) { list = bag[type] = []; }
  for (var i = 0; i < list.length; i++) {
    if (list[i].callback === callback && list[i].capture === capture) { return; }
  }
  ArrayPrototypePush(list, { callback: callback, once: once, capture: capture });
  notifyListenerChanged(this, type, list.length);
};
EventTargetImpl.prototype.removeEventListener = function (type, callback, options) {
  type = String(type);
  var capture = false;
  if (typeof options === "boolean") {
    capture = options;
  } else if (options && typeof options === "object") {
    capture = !!options.capture;
  }
  var bag = this._listeners;
  var list = bag === undefined ? undefined : bag[type];
  if (!list) { return; }
  for (var i = 0; i < list.length; i++) {
    if (list[i].callback === callback && list[i].capture === capture) {
      ArrayPrototypeSplice(list, i, 1);
      notifyListenerChanged(this, type, list.length);
      return;
    }
  }
};
function dispatch(target, event, rethrow) {
  event.target = target;
  event.currentTarget = target;
  var thrown;
  var hasThrown = false;
  var bag = target._listeners;
  var list = bag === undefined ? undefined : bag[event.type];
  if (list) {
    // Snapshot so listeners added during dispatch are not invoked and
    // registration order is preserved.
    var snapshot = ArrayPrototypeSlice(list);
    for (var i = 0; i < snapshot.length; i++) {
      var entry = snapshot[i];
      var idx = ArrayPrototypeIndexOf(list, entry);
      if (idx === -1) { continue; }  // removed since snapshot
      if (entry.once) {
        ArrayPrototypeSplice(list, idx, 1);
        notifyListenerChanged(target, event.type, list.length);
      }
      var cb = entry.callback;
      try {
        if (typeof cb === "function") {
          FunctionPrototypeCall(cb, target, event);
        } else if (cb && typeof cb.handleEvent === "function") {
          cb.handleEvent(event);
        }
      } catch (e) {
        if (rethrow && !hasThrown) {
          thrown = e;
          hasThrown = true;
        } else {
          reportListenerError(e);
        }
      }
      if (event._stopImmediate) { break; }
    }
  }
  event.currentTarget = null;
  if (hasThrown) { throw thrown; }
  return !event.defaultPrevented;
}

EventTargetImpl.prototype.dispatchEvent = function (event) {
  return dispatch(this, event, false);
};

// Dispatch whose first listener exception reaches the caller instead of the
// uncaught-error reporter. Worker message delivery needs it: the native frame
// that called in owns the worker's error chain (the scope's `onerror`, then
// the parent's), and throwing back into it is the only way there.
function dispatchEventRethrowing(target, event) {
  return dispatch(target, event, true);
}

// Internal EventTarget instance backing the global. globalThis's prototype
// is intentionally NOT made an EventTarget; only the three methods are
// bound onto it.
var globalTarget = new EventTargetImpl();
g.addEventListener = function (type, callback, options) {
  return globalTarget.addEventListener(type, callback, options);
};
g.removeEventListener = function (type, callback, options) {
  return globalTarget.removeEventListener(type, callback, options);
};
g.dispatchEvent = function (event) {
  return globalTarget.dispatchEvent(event);
};

function EventTarget() { FunctionPrototypeCall(EventTargetImpl, this); }
EventTarget.prototype.addEventListener = EventTargetImpl.prototype.addEventListener;
EventTarget.prototype.removeEventListener = EventTargetImpl.prototype.removeEventListener;
EventTarget.prototype.dispatchEvent = EventTargetImpl.prototype.dispatchEvent;

g.Event = Event;
g.EventTarget = EventTarget;

// Event handler IDL attributes (HTML §8.1.7.2), Node's defineEventHandler.
// The handler is never registered directly: a wrapper listener takes its slot
// on the first assignment and stays there, so `onfoo` fires at the position it
// was FIRST set at even after being replaced or cleared, interleaved correctly
// with addEventListener registrations. A cleared handler leaves the wrapper in
// place but inert, which is why the wrapper carries the count correction the
// listener-changed hook applies.
var addListener = EventTargetImpl.prototype.addEventListener;

function makeEventHandler(handler, cancelOnTruthy) {
  function eventHandler(event) {
    if (typeof eventHandler.handler !== "function") { return; }
    var result = FunctionPrototypeCall(eventHandler.handler, this, event);
    // Special error event handling (HTML §8.1.7.3): only for `onerror`, a
    // truthy return cancels the event. It is the one way a handler
    // attribute's return value is observable, so it is also how "the worker
    // error was handled" leaves dispatch.
    if (cancelOnTruthy && result) { event.preventDefault(); }
    return result;
  }
  eventHandler.handler = handler;
  // A wrapper holds one listener slot for good; an inactive handler cancels
  // its own slot out of the count the listener-changed hook receives.
  eventHandler.delta = typeof handler === "function" ? 0 : -1;
  return eventHandler;
}

function defineEventHandler(target, name, event, cancelOnTruthy) {
  if (event === undefined) { event = name; }
  var propName = "on" + name;

  function get() {
    var wrappers = handlersOf(this);
    if (wrappers === undefined) { return null; }
    var wrapper = wrappers[event];
    return wrapper === undefined ? null : wrapper.handler;
  }

  function set(value) {
    // [LegacyTreatNonObjectAsNull]: anything neither callable nor an object
    // clears the handler.
    if (typeof value !== "function" && (typeof value !== "object" || value === null)) {
      value = null;
    }
    var bag = listenersOf(this);
    var wrappers = bag[kHandlers];
    if (wrappers === undefined) {
      wrappers = bag[kHandlers] = ObjectCreate(null);
    }
    var wrapper = wrappers[event];
    if (wrapper === undefined) {
      // First assignment ever, `null` included: the slot is claimed now and
      // kept, interleaved with addEventListener registrations at this point.
      wrapper = wrappers[event] = makeEventHandler(value, cancelOnTruthy);
      FunctionPrototypeCall(addListener, this, event, wrapper);
      var assigned = this[kHandlerAssigned];
      if (assigned !== undefined) { assigned(this, event); }
      return;
    }
    var wasActive = typeof wrapper.handler === "function";
    var isActive = typeof value === "function";
    wrapper.handler = value;
    if (wasActive === isActive) { return; }
    // Absolute, never cumulative: the correction is the whole slot or nothing,
    // so the count the hook sees returns to zero when the last active listener
    // goes.
    wrapper.delta = isActive ? 0 : -1;
    var list = bag[event];
    notifyListenerChanged(this, event, list ? list.length : 0);
  }

  ObjectDefineProperty(get, "name", { value: "get " + propName, configurable: true });
  ObjectDefineProperty(set, "name", { value: "set " + propName, configurable: true });
  ObjectDefineProperty(target, propName, {
    get: get,
    set: set,
    enumerable: true,
    configurable: true,
  });
}

// CustomEvent (DOM Standard §2.4): Event carrying an app-supplied `detail`.
// Defined here so it extends the same Event the globals hold, but NOT
// installed eagerly — the lazy-global tier (LazyGlobals) places it from this
// file's exports on the first read of the name, sharing the init-time run
// through the exports cache.
function CustomEvent(type, opts) {
  FunctionPrototypeCall(Event, this, type, opts);
  opts = opts || {};
  // `detail` is readonly in the IDL, unlike the base Event's mutable-by-need
  // fields (target/defaultPrevented change during dispatch).
  ObjectDefineProperty(this, "detail", {
    value: opts.detail !== undefined ? opts.detail : null,
    writable: false,
    enumerable: true,
    configurable: true,
  });
}
CustomEvent.prototype = ObjectCreate(Event.prototype);
ObjectDefineProperty(CustomEvent.prototype, "constructor", {
  value: CustomEvent,
  writable: true,
  configurable: true,
});

// Consumed by Events::Init, the lazy-global tier (CustomEvent), and sibling
// builtins via require("internal/events"). The module system refuses that
// specifier, so the capabilities here never reach app code.
module.exports = {
  // The EventTarget instance backing the global listener methods; Events::Init
  // caches it so native dispatch survives app code overwriting
  // globalThis.dispatchEvent.
  globalEventTarget: globalTarget,
  CustomEvent: CustomEvent,
  kListenerChanged: kListenerChanged,
  kHandlerAssigned: kHandlerAssigned,
  setListenerErrorReporter: setListenerErrorReporter,
  // The base classes and the handler-attribute helper, for the lazy builtins
  // that may not read them off the globals user code can replace.
  Event: Event,
  EventTarget: EventTarget,
  defineEventHandler: defineEventHandler,
  dispatchEventRethrowing: dispatchEventRethrowing,
};

"use strict";
const {
  ArrayPrototypeIndexOf,
  ArrayPrototypePush,
  ArrayPrototypeSlice,
  ArrayPrototypeSplice,
  FunctionPrototypeCall,
  ObjectCreate,
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
// installs the real reporter via _installListenerErrorReporter (before any
// user code runs); until then a thrown listener is swallowed.
var reportListenerError = function (e) {};

// Internal listener-mutation hook. A target (in practice: AbortSignal, on
// its prototype) may carry a function under this symbol; it is called with
// (target, type, newCount) from every path that changes a listener list —
// add, remove, and the once-splice inside dispatch. The key never reaches
// app code (handed to the abort-signal builtin through a one-shot below),
// so the accounting cannot be bypassed the way an overridable
// addEventListener could.
var kListenerChanged = Symbol("listenerChanged");
function notifyListenerChanged(target, type, count) {
  var hook = target[kListenerChanged];
  if (hook !== undefined) { hook(target, type, count); }
}

function EventTargetImpl() { this._listeners = ObjectCreate(null); }
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
  var list = this._listeners[type];
  if (!list) { list = this._listeners[type] = []; }
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
  var list = this._listeners[type];
  if (!list) { return; }
  for (var i = 0; i < list.length; i++) {
    if (list[i].callback === callback && list[i].capture === capture) {
      ArrayPrototypeSplice(list, i, 1);
      notifyListenerChanged(this, type, list.length);
      return;
    }
  }
};
EventTargetImpl.prototype.dispatchEvent = function (event) {
  event.target = this;
  event.currentTarget = this;
  var list = this._listeners[event.type];
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
        notifyListenerChanged(this, event.type, list.length);
      }
      var cb = entry.callback;
      try {
        if (typeof cb === "function") {
          FunctionPrototypeCall(cb, this, event);
        } else if (cb && typeof cb.handleEvent === "function") {
          cb.handleEvent(event);
        }
      } catch (e) {
        reportListenerError(e);
      }
      if (event._stopImmediate) { break; }
    }
  }
  event.currentTarget = null;
  return !event.defaultPrevented;
};

// Internal EventTarget instance backing the global. globalThis's prototype
// is intentionally NOT made an EventTarget; only the three methods are
// bound onto it.
var globalTarget = new EventTargetImpl();
// Called by the error-events layer to install the native listener-error
// reporter into this closure. One-shot: the backing target leaks to app
// code via event.target, so the hook removes itself after the install
// (which happens during runtime init, before any user code runs).
globalTarget._installListenerErrorReporter = function (fn) {
  reportListenerError = fn;
  delete globalTarget._installListenerErrorReporter;
};
// One-shot handoff of the listener-mutation hook key to the abort-signal
// builtin, which Events::Init runs next with this export as its binding.
// Same lifecycle as _installListenerErrorReporter: consumed during runtime
// init, gone before user code can reach the backing target.
globalTarget._takeListenerChangedKey = function () {
  delete globalTarget._takeListenerChangedKey;
  return kListenerChanged;
};
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

module.exports = globalTarget;

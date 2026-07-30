"use strict";

const { globalTarget, nativeReportFatal } = binding;
const { FunctionPrototypeCall, ObjectCreate, String, TypeError } = primordials;
var g = globalThis;
var Event = g.Event;

function ErrorEvent(type, opts) {
  opts = opts || {};
  FunctionPrototypeCall(Event, this, type, opts);
  this.message = opts.message !== undefined ? String(opts.message) : "";
  this.filename = opts.filename !== undefined ? String(opts.filename) : "";
  this.lineno = opts.lineno !== undefined ? (opts.lineno | 0) : 0;
  this.colno = opts.colno !== undefined ? (opts.colno | 0) : 0;
  this.error = opts.error !== undefined ? opts.error : null;
}
ErrorEvent.prototype = ObjectCreate(Event.prototype);
ErrorEvent.prototype.constructor = ErrorEvent;

function PromiseRejectionEvent(type, opts) {
  opts = opts || {};
  FunctionPrototypeCall(Event, this, type, opts);
  this.promise = opts.promise;
  this.reason = opts.reason;
}
PromiseRejectionEvent.prototype = ObjectCreate(Event.prototype);
PromiseRejectionEvent.prototype.constructor = PromiseRejectionEvent;

// A listener that throws must not stop other listeners: route the thrown
// value to the native fatal tail instead of ever recursively dispatching
// another `error` event from inside dispatch.
globalTarget._installListenerErrorReporter(function (e) {
  try { nativeReportFatal(e, (e && e.stack) || ""); } catch (ignored) {}
});

g.reportError = function (e) {
  if (arguments.length === 0) {
    throw new TypeError("Failed to execute 'reportError': 1 argument required, but only 0 present.");
  }
  var ev = new ErrorEvent("error", {
    message: (e && e.message !== undefined && e.message !== null) ? String(e.message) : String(e),
    error: e,
    cancelable: true
  });
  if (globalTarget.dispatchEvent(ev)) {
    nativeReportFatal(e, (e && e.stack) || "");
  }
};

g.ErrorEvent = ErrorEvent;
g.PromiseRejectionEvent = PromiseRejectionEvent;

// Closures called by C++. They never look up globalThis.dispatchEvent, so
// they keep working even if app code overwrites it.
function dispatchErrorEvent(error, message, stack) {
  var ev = new ErrorEvent("error", {
    message: message !== undefined && message !== null ? String(message) : "",
    error: error,
    cancelable: true
  });
  globalTarget.dispatchEvent(ev);
  return ev.defaultPrevented;
}
function dispatchUnhandledRejection(promise, reason) {
  var ev = new PromiseRejectionEvent("unhandledrejection", {
    promise: promise,
    reason: reason,
    cancelable: true
  });
  globalTarget.dispatchEvent(ev);
  return ev.defaultPrevented;
}
function dispatchRejectionHandled(promise, reason) {
  var ev = new PromiseRejectionEvent("rejectionhandled", {
    promise: promise,
    reason: reason,
    cancelable: false
  });
  globalTarget.dispatchEvent(ev);
}
function dispatchNativeUncaughtError(error, message, stack) {
  var ev = new ErrorEvent("nativeuncaughterror", {
    message: message !== undefined && message !== null ? String(message) : "",
    error: error,
    cancelable: true
  });
  globalTarget.dispatchEvent(ev);
  return ev.defaultPrevented;
}

module.exports = [dispatchErrorEvent, dispatchUnhandledRejection, dispatchRejectionHandled, dispatchNativeUncaughtError];

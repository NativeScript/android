"use strict";
// Worker (HTML Standard §10.2.6) and the worker global scope (§10.2.1) as
// EventTargets: both deliver MessageEvents instead of the runtime's historical
// direct call of an `onmessage` property, and the Worker object receives the
// worker's unhandled errors as ErrorEvents. The worker global scope's own
// `onerror` stays a direct call with the error — a documented NativeScript
// contract, not the web's event.
//
// Eager, because the handler attributes have to exist before app code assigns
// one. MessageEvent itself is pulled in on the first delivery, so a worker
// nobody talks to never runs that builtin.
const {
  Error,
  ErrorCaptureStackTrace,
  EvalError,
  ObjectDefineProperty,
  ObjectSetPrototypeOf,
  RangeError,
  ReferenceError,
  SyntaxError,
  TypeError,
  URIError,
} = primordials;

const {
  EventTarget,
  defineEventHandler,
  dispatchEventRethrowing,
  globalEventTarget,
} = require("internal/events");

const g = globalThis;

let MessageEvent;
function getMessageEvent() {
  if (MessageEvent === undefined) {
    ({ MessageEvent } = require("internal/message-event"));
  }
  return MessageEvent;
}

// ErrorEvent is installed by the error-events builtin, which
// Runtime::PrepareV8Runtime runs AFTER this one — so the constructor can
// only be taken on the first error delivery, not at init.
let ErrorEvent;
function getErrorEvent() {
  if (ErrorEvent === undefined) {
    ErrorEvent = g.ErrorEvent;
  }
  return ErrorEvent;
}

// The delivery callout, invoked by native with the receiving target as `this`:
// the Worker object on the parent isolate, the global scope's EventTarget
// inside a worker. `ports` is the array of MessagePorts the message
// transferred, or undefined when it carried none.
//
// A handler that throws propagates back into the calling native frame: that
// is what feeds the worker's onerror chain — the worker scope's handler
// first, then the parent's — which the cross-runtime worker suite asserts.
function emitMessage(data, ports, type) {
  const MessageEventCtor = getMessageEvent();
  dispatchEventRethrowing(this, new MessageEventCtor(type, { data, ports }));
}

// The key the worker's rebuilt error travels under on the `error` event, for
// node:worker_threads, whose listeners receive the error rather than the
// event. Reached only through require("internal/worker-events").
const kWorkerError = Symbol("workerError");

// A built-in error name rebuilds with its own constructor, as Node's does, so
// `instanceof TypeError` holds on the parent.
const errorConstructors = {
  __proto__: null,
  Error,
  EvalError,
  RangeError,
  ReferenceError,
  SyntaxError,
  TypeError,
  URIError,
};

// The worker's error, rebuilt from the name and message the worker read off
// the thrown value. Any other name, a subclass's or a DOMException's, stays an
// own `name` on an Error. Without the worker's stack, the stack holds the
// header alone rather than the frames that rebuilt it.
function rebuildError(name, message, stack) {
  const ErrorConstructor = errorConstructors[name];
  const error =
    ErrorConstructor === undefined ? new Error(message) : new ErrorConstructor(message);
  if (ErrorConstructor === undefined) {
    ObjectDefineProperty(error, "name", {
      __proto__: null,
      value: name,
      writable: true,
      configurable: true,
    });
  }
  if (stack) {
    error.stack = stack;
  } else {
    ErrorCaptureStackTrace(error, emitError);
  }
  return error;
}

// The parent-side error delivery callout, invoked by native with the Worker
// object as `this` once the worker scope has left the error unhandled. Only
// primitives cross the isolate boundary, so the event carries no `error`
// object; `stackTrace` is this runtime's addition to the ErrorEvent fields.
//
// Returns the rebuilt error when the event was not handled, for native to
// report on the parent's global scope, and undefined when it was: a truthy
// return from the `onerror` attribute cancels the event (HTML §8.1.7.3), as
// does preventDefault() from any listener.
function emitError(message, filename, lineno, stackTrace, errorName, errorMessage) {
  const ErrorEventCtor = getErrorEvent();
  const event = new ErrorEventCtor("error", {
    message,
    filename,
    lineno,
    cancelable: true,
  });
  event.stackTrace = stackTrace;
  const error = rebuildError(errorName, errorMessage, stackTrace);
  ObjectDefineProperty(event, kWorkerError, { __proto__: null, value: error });
  dispatchEventRethrowing(this, event);
  return event.defaultPrevented ? undefined : error;
}

ObjectSetPrototypeOf(g.Worker.prototype, EventTarget.prototype);
defineEventHandler(g.Worker.prototype, "message");
defineEventHandler(g.Worker.prototype, "messageerror");
defineEventHandler(g.Worker.prototype, "error", "error", true);

// The global scope's handler attributes are defined against the EventTarget
// backing the global listener methods, which is what native dispatches on and
// what globalThis.addEventListener registers with — so a handler and an
// addEventListener registration interleave in assignment order. globalThis
// only forwards.
defineEventHandler(globalEventTarget, "message");
defineEventHandler(globalEventTarget, "messageerror");
for (const name of ["onmessage", "onmessageerror"]) {
  ObjectDefineProperty(g, name, {
    __proto__: null,
    get() {
      return globalEventTarget[name];
    },
    set(value) {
      globalEventTarget[name] = value;
    },
    enumerable: true,
    configurable: true,
  });
}

module.exports = { emitMessage, emitError, kWorkerError };

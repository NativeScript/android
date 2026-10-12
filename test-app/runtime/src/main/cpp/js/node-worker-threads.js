"use strict";

// The `node:worker_threads` compatibility shim. The channel half —
// MessagePort, MessageChannel, BroadcastChannel, receiveMessageOnPort — is the
// real thing, shared with the globals of the same name. The thread half is a
// bridge over the runtime's own Worker: this runtime has no thread pool, no
// stdio plumbing and no per-thread environment, so what cannot be honoured
// throws with the option or function named rather than degrading silently.
// See docs/worker-threads.md for the real-vs-shim table.

const {
  isMainThread,
  threadId,
  markAsUntransferable,
  isMarkedAsUntransferable,
  markAsUncloneable,
  setEnvironmentData,
  getEnvironmentData,
} = binding;

const {
  ArrayPrototypeIndexOf,
  ArrayPrototypePush,
  ArrayPrototypeSlice,
  ArrayPrototypeSplice,
  Error,
  FunctionPrototypeCall,
  ObjectCreate,
  ObjectDefineProperty,
  ObjectFreeze,
  Promise,
  PromisePrototypeThen,
  PromiseResolve,
  SymbolFor,
  SymbolToStringTag,
  TypeError,
} = primordials;

const {
  MessagePort,
  MessageChannel,
  receiveMessageOnPort,
} = require("internal/message-channel");
const { BroadcastChannel } = require("internal/broadcast-channel");
const {
  EventTarget,
  defineEventHandler,
  globalEventTarget,
} = require("internal/events");

let MessageEvent;
function getMessageEvent() {
  if (MessageEvent === undefined) {
    ({ MessageEvent } = require("internal/message-event"));
  }
  return MessageEvent;
}

const g = globalThis;
// The platform constructor this shim wraps, and the worker scope's channel
// back to its parent.
const NativeWorker = g.Worker;
const globalPostMessage = g.postMessage;

const addEventListener = EventTarget.prototype.addEventListener;
const removeEventListener = EventTarget.prototype.removeEventListener;
const dispatchEvent = EventTarget.prototype.dispatchEvent;

// Runs `fn` after the caller returns. Node reports 'online' and 'exit' from
// the thread's own lifecycle; the runtime's Worker has no equivalent signal,
// so both are reported off a microtask instead.
function soon(fn) {
  PromisePrototypeThen(PromiseResolve(), fn);
}

function notSupported(name) {
  throw new Error(`${name} is not supported in this runtime`);
}

// Worker options that carry meaning this runtime cannot honour. The three
// stdio ones default to false, so only an explicit request is an error.
const rejectedOptions = ["workerData", "env", "eval", "transferList"];
const rejectedStdio = ["stdin", "stdout", "stderr"];

class WorkerEmitter {
  #listeners = ObjectCreate(null);

  on(type, listener) {
    if (typeof listener !== "function") {
      throw new TypeError('The "listener" argument must be of type function');
    }
    const key = `${type}`;
    const list = this.#listeners[key] || (this.#listeners[key] = []);
    ArrayPrototypePush(list, { listener, once: false });
    return this;
  }

  once(type, listener) {
    if (typeof listener !== "function") {
      throw new TypeError('The "listener" argument must be of type function');
    }
    const key = `${type}`;
    const list = this.#listeners[key] || (this.#listeners[key] = []);
    ArrayPrototypePush(list, { listener, once: true });
    return this;
  }

  // Node removes the most recently added registration of a listener.
  removeListener(type, listener) {
    const list = this.#listeners[`${type}`];
    if (list === undefined) {
      return this;
    }
    for (let i = list.length - 1; i >= 0; i--) {
      if (list[i].listener === listener) {
        ArrayPrototypeSplice(list, i, 1);
        return this;
      }
    }
    return this;
  }

  off(type, listener) {
    return this.removeListener(type, listener);
  }

  emit(type, arg) {
    const list = this.#listeners[type];
    if (list === undefined) {
      return;
    }
    const snapshot = ArrayPrototypeSlice(list);
    for (let i = 0; i < snapshot.length; i++) {
      const entry = snapshot[i];
      if (entry.once) {
        const index = ArrayPrototypeIndexOf(list, entry);
        if (index !== -1) {
          ArrayPrototypeSplice(list, index, 1);
        }
      }
      FunctionPrototypeCall(entry.listener, this, arg);
    }
  }
}

class Worker extends WorkerEmitter {
  #worker;
  #exited = false;
  // Every terminate() promise settles when the thread's end is reported.
  #exitWaiters = [];

  constructor(filename, options) {
    super();
    if (options !== undefined && options !== null) {
      for (let i = 0; i < rejectedOptions.length; i++) {
        if (options[rejectedOptions[i]] !== undefined) {
          throw new TypeError(
            `Worker option '${rejectedOptions[i]}' is not supported in this runtime`
          );
        }
      }
      for (let i = 0; i < rejectedStdio.length; i++) {
        if (options[rejectedStdio[i]]) {
          throw new TypeError(
            `Worker option '${rejectedStdio[i]}' is not supported in this runtime`
          );
        }
      }
    }

    // The runtime's own options (androidPriority, resourceLimits) ride
    // along; the native constructor ignores keys it does not know.
    const worker =
      options === undefined || options === null
        ? new NativeWorker(`${filename}`)
        : new NativeWorker(`${filename}`, options);
    this.#worker = worker;
    const self = this;
    worker.onmessage = function (event) {
      self.emit("message", event.data);
    };
    worker.onmessageerror = function (event) {
      self.emit("messageerror", event.data);
    };
    worker.onerror = function (error) {
      self.emit("error", error);
    };
    // The runtime's end-of-worker event: the one place 'exit' comes from, for
    // a worker's own close() and for terminate() alike, so nothing the worker
    // sent before it ended can follow 'exit'.
    FunctionPrototypeCall(
      addEventListener,
      worker,
      "nsworkerended",
      function () {
        self.#reportExit();
      }
    );
    soon(function () {
      self.emit("online", undefined);
    });
  }

  // Node emits 'exit' once and settles terminate() after it. The code is
  // always 0: this runtime has no thread exit status to report, and the
  // cross-runtime suite pins that for every end a worker can take.
  #reportExit() {
    if (this.#exited) {
      return;
    }
    this.#exited = true;
    this.emit("exit", 0);
    const waiters = this.#exitWaiters;
    this.#exitWaiters = [];
    for (let i = 0; i < waiters.length; i++) {
      waiters[i](0);
    }
  }

  postMessage(value, transfer) {
    this.#worker.postMessage(value, transfer);
  }

  // Resolves with the exit code once the thread has actually ended. A parent
  // that is itself tearing down never delivers that signal, so the promise
  // stays pending there, as it does in Node when the parent dies.
  terminate() {
    if (this.#exited) {
      return PromiseResolve(0);
    }
    this.#worker.terminate();
    const self = this;
    return new Promise(function (resolve) {
      ArrayPrototypePush(self.#exitWaiters, resolve);
    });
  }
}

ObjectDefineProperty(Worker.prototype, SymbolToStringTag, {
  __proto__: null,
  value: "Worker",
  configurable: true,
});

// The worker scope's end of the parent channel. Not a MessagePort: it is not
// transferable and it has no queue of its own, it forwards to the worker
// globals the runtime already provides. close() is a no-op — a worker ends
// through its own close()/terminate().
class ParentPort extends EventTarget {
  // Node's parentPort is an EventEmitter as well: on("message") receives the
  // payload, not the event. Each listener is registered through its own
  // wrapper so removal by the original function still works.
  #wrappers = ObjectCreate(null);

  postMessage(value, transfer) {
    FunctionPrototypeCall(globalPostMessage, g, value, transfer);
  }

  start() {}

  close() {}

  on(type, listener) {
    return this.#add(`${type}`, listener, false);
  }

  addListener(type, listener) {
    return this.#add(`${type}`, listener, false);
  }

  once(type, listener) {
    return this.#add(`${type}`, listener, true);
  }

  off(type, listener) {
    this.#remove(`${type}`, listener);
    return this;
  }

  removeListener(type, listener) {
    this.#remove(`${type}`, listener);
    return this;
  }

  #add(type, listener, once) {
    if (typeof listener !== "function") {
      throw new TypeError('The "listener" argument must be of type function');
    }
    const self = this;
    const entry = { listener, wrapper: undefined };
    // A once registration detaches its own entry, not whichever entry happens
    // to hold the same listener: the same function may be on() and once() at
    // the same time.
    entry.wrapper = function (event) {
      if (once) {
        self.#removeEntry(type, entry);
      }
      const arg = type === "message" || type === "messageerror" ? event.data : event;
      FunctionPrototypeCall(listener, self, arg);
    };
    const list = this.#wrappers[type] || (this.#wrappers[type] = []);
    ArrayPrototypePush(list, entry);
    FunctionPrototypeCall(addEventListener, this, type, entry.wrapper);
    return this;
  }

  // Node removes the most recently added registration of a listener.
  #remove(type, listener) {
    const list = this.#wrappers[type];
    if (list === undefined) {
      return;
    }
    for (let i = list.length - 1; i >= 0; i--) {
      if (list[i].listener === listener) {
        this.#removeEntry(type, list[i]);
        return;
      }
    }
  }

  #removeEntry(type, entry) {
    const list = this.#wrappers[type];
    if (list === undefined) {
      return;
    }
    const index = ArrayPrototypeIndexOf(list, entry);
    if (index === -1) {
      return;
    }
    ArrayPrototypeSplice(list, index, 1);
    FunctionPrototypeCall(removeEventListener, this, type, entry.wrapper);
  }
}

defineEventHandler(ParentPort.prototype, "message");
defineEventHandler(ParentPort.prototype, "messageerror");

ObjectDefineProperty(ParentPort.prototype, SymbolToStringTag, {
  __proto__: null,
  value: "MessagePort",
  configurable: true,
});

let parentPort = null;
if (!isMainThread) {
  parentPort = new ParentPort();
  const relay = function (event) {
    FunctionPrototypeCall(
      dispatchEvent,
      parentPort,
      new (getMessageEvent())(event.type, { data: event.data, ports: event.ports })
    );
  };
  FunctionPrototypeCall(addEventListener, globalEventTarget, "message", relay);
  FunctionPrototypeCall(addEventListener, globalEventTarget, "messageerror", relay);
}

module.exports = ObjectFreeze({
  BroadcastChannel,
  MessageChannel,
  MessagePort,
  // Exported so an `options.env === SHARE_ENV` spelling still resolves; this
  // runtime has one environment and never copies it.
  SHARE_ENV: SymbolFor("nodejs.worker_threads.SHARE_ENV"),
  Worker,
  getEnvironmentData,
  isInternalThread: false,
  isMainThread,
  isMarkedAsUntransferable,
  markAsUncloneable,
  markAsUntransferable,
  moveMessagePortToContext() {
    notSupported("moveMessagePortToContext");
  },
  parentPort,
  postMessageToThread() {
    notSupported("postMessageToThread");
  },
  receiveMessageOnPort,
  resourceLimits: {},
  setEnvironmentData,
  threadId,
  threadName: undefined,
  // No workerData: the Worker constructor rejects the option that would carry
  // it, so there is never anything to hand a worker.
  workerData: null,
});

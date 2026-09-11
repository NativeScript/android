# Messaging and `node:worker_threads`

The runtime implements HTML's messaging primitives — `MessagePort`,
`MessageChannel`, `BroadcastChannel` and `MessageEvent` — and exposes them both
as globals and through a `node:worker_threads` module.

```js
const channel = new MessageChannel();
channel.port1.onmessage = (event) => console.log(event.data);
channel.port2.postMessage({ hello: "world" });

const worker = new Worker("./worker.js");
worker.postMessage({ port: channel.port2 }, [channel.port2]);
```

## Surface

`MessagePort`, `MessageChannel`, `BroadcastChannel` and `MessageEvent` are
**lazy globals**: the name is placed on the first read of it, so an app that
never mentions one never pays for it. They are ordinary globals once read —
`instanceof`, subclassing and property access all behave normally.

`require("node:worker_threads")` (or `import` of the same specifier) returns a
frozen module. Its channel half is not a re-implementation: the classes it
exports are the very objects the globals of those names hold, so
`require("node:worker_threads").MessagePort === globalThis.MessagePort`.

`MessagePort` has no constructor — `new MessagePort()` throws a `TypeError`.
Ports come from a `MessageChannel` or arrive on a message.

## `node:worker_threads` exports

"Real" means genuine behaviour, and for a class the same object the global of
that name holds. "Shim" means a bridge over the runtime's own `Worker`, which
has no thread pool, no stdio plumbing and no per-thread environment. "Throws"
means deliberately unsupported.

| export | status | notes |
|---|---|---|
| `MessagePort` | real | The global `MessagePort`. |
| `MessageChannel` | real | The global `MessageChannel`. |
| `BroadcastChannel` | real | The global `BroadcastChannel`. The process-wide registry described below. |
| `receiveMessageOnPort(port)` | real | Synchronously pops one queued message, `{ message }` or `undefined`. Works on a port that was never started; a close sentinel at the head closes the port and reports `undefined`. |
| `isMainThread` | real | `false` inside a runtime worker. |
| `threadId` | real | `0` on the main isolate, the worker's id (from 1) inside one. |
| `isInternalThread` | real | Always `false`; this runtime has no internal threads. |
| `markAsUntransferable(obj)` | real | Brands `obj` so listing it in a transfer list is a `DataCloneError`. |
| `isMarkedAsUntransferable(obj)` | real | Reads that brand. |
| `markAsUncloneable(obj)` | real | Brands `obj` so serializing it at all is a `DataCloneError`, in `structuredClone` and every `postMessage` alike. |
| `setEnvironmentData(key, value)` | real, deviates | Clones and stores process-wide. No per-thread snapshot — see below. Passing `undefined` (or omitting the value) deletes the key. |
| `getEnvironmentData(key)` | real, deviates | Deserializes a fresh copy per read, on any isolate. |
| `resourceLimits` | shim | Always `{}`; the runtime imposes no per-worker limits and reports none. |
| `SHARE_ENV` | shim | Exported so the spelling resolves, but inert — see below. |
| `threadName` | shim | Always `undefined`. |
| `workerData` | shim | Always `null` — see below. |
| `parentPort` | shim | `null` on the main isolate. Inside a worker, a `MessagePort`-shaped `EventTarget` over the worker's existing parent channel: `postMessage` forwards to the global `postMessage`, `message`/`messageerror` are re-dispatched from the worker global scope, `start()` and `close()` are no-ops. It is **not** a real port: not transferable, no queue of its own. |
| `Worker` | shim | A class over the runtime's global `Worker` with a small Node-style emitter (`on`/`once`/`off`/`removeListener`) for `message`, `messageerror`, `error`, `online` and `exit`. `postMessage(value, transfer)` and `terminate()` forward. `online` is emitted off a microtask after construction, not from the thread. Unsupported options throw a `TypeError` naming the option: `workerData`, `env`, `eval`, `transferList`, and `stdin`/`stdout`/`stderr` when explicitly truthy. The runtime's own `Worker` options (`androidPriority`) ride along untouched — the native constructor ignores keys it does not know. |
| `postMessageToThread` | throws | `Error: postMessageToThread is not supported in this runtime`. |
| `moveMessagePortToContext` | throws | `Error: moveMessagePortToContext is not supported in this runtime`. |
| `locks` | absent | Web Locks are not implemented; the property does not exist. |

## Documented deviations

### `setEnvironmentData` has no per-thread snapshot

Node copies the environment-data store into a worker when it is spawned, so a
later write on the parent is invisible to it. Here the store is one
process-global map, and a worker reads it live: a `setEnvironmentData` call
made *after* a worker started is visible to that worker.

Values are cloned on the way in and deserialized fresh on each read, so
mutating the object you passed does not reach a reader, and two readers never
share one object.

### `exit` comes only from `terminate()`

The runtime has no thread-exit signal — nothing reports that a worker's isolate
finished. `terminate()` therefore resolves with `0` and emits `exit` with code
`0` on the way, and that is the only path that emits it. A worker that ends by
its own `close()` produces no `exit`.

### A worker error carries no `error` object, and the worker scope's `onerror` is not an event

An error the worker scope leaves unhandled reaches the parent as a real
`ErrorEvent` dispatched on the `Worker`, so `worker.onerror` and
`addEventListener("error", …)` both fire, interleaved in the order they were
installed. Two things differ from a browser:

- Only primitives cross the isolate boundary, so `event.error` is always
  `null`; the worker's stack comes through as `event.stackTrace`, a string
  alongside the standard `message`, `filename` and `lineno`.
- Inside the worker, `onerror` is still a direct call taking the thrown value —
  not an `ErrorEvent`, and not reachable through `addEventListener`. Returning
  truthy from it handles the error and stops it from reaching the parent, which
  is the same "handled" contract `worker.onerror` has on the parent side (a
  truthy return there cancels the event, as does `preventDefault()` from any
  listener).

### Inside a worker, `event.target` is not `globalThis`

`globalThis` is not itself an `EventTarget` here. It forwards
`addEventListener`, `removeEventListener` and `dispatchEvent` to an internal
`EventTarget` that backs the worker global scope, and native delivery
dispatches on that internal target — which is what keeps app code from
intercepting message delivery by replacing `globalThis.dispatchEvent`. The
consequence is visible on the event: `event.target` inside a worker's message
handler is that internal target, not `globalThis`.

### `SHARE_ENV` is a no-op

It is exported so that an `options.env === SHARE_ENV` spelling resolves rather
than being a `ReferenceError`. There is one process environment and it is never
copied, so nothing distinguishes sharing it from not. (`env` is a rejected
`Worker` option regardless.)

### No `workerData`

There is no channel that would carry it: the `Worker` constructor rejects the
`workerData` option outright, so the export is permanently `null`. Send an
opening `postMessage` instead.

### `BroadcastChannel`'s registry is process-global

"Same user agent", in the spec's terms, is the app process. Every
`BroadcastChannel` built with the same name joins one group regardless of which
isolate constructed it, so a worker and the main isolate reach each other by
name alone. A channel is receiving from the moment it is constructed and stays
strongly held until `close()`.

## `MessagePort` lifetime

The GC model is Node's, not the browser's: **a port is held strongly by the
runtime from creation until it is closed.** An unreferenced-but-unclosed port
does not go away, and neither does its channel, its queue, or anything the
queue's messages hold. Close the ports you are done with.

```js
const { port1, port2 } = new MessageChannel();
port1.onmessage = handle;
// ... later
port1.close();
```

Closing behaves as one channel-wide event:

- `close()` sends a `close` event — a plain `Event`, not a `MessageEvent` — to
  the port being closed **and** to its sibling. A channel with one end left is
  no channel, so both ends learn about it. (A named `BroadcastChannel` group is
  different: members join and leave it freely, so only the leaving member gets
  the event.)
- The `close` event reaches a port that was never started. Enabling is about
  *messages*; a port whose sibling died always learns about it.
- On the port being closed the event fires synchronously, inside `close()`.
  On the sibling it orders behind whatever was already queued to it, so
  messages already sent are still delivered first.
- `postMessage` on a closed port is a **silent no-op**. It still serializes:
  the transfer list's side effects and its errors do not depend on delivery, so
  a bad transfer list throws and a good one detaches its buffers, and only then
  is the message dropped.
- `port.close(callback)` registers `callback` as a one-shot `close` listener
  before closing.

## Port enabling

Delivery follows HTML's port-enable rules rather than starting automatically:

- A port starts delivering on its **first `message` listener** — either
  `addEventListener("message", …)` or an `onmessage` attribute assignment. The
  first `onmessage` write counts even when it is `onmessage = null`: it is the
  assignment, not the handler, that claims the listener slot.
- It stops when the last `message` listener goes away, and messages queue again
  until one returns.
- `port.start()` enables delivery for code that only uses `addEventListener`
  and wants control over when the queue drains. As in Node, it does not pin
  the port on: removing the last `message` listener stops delivery again until
  a listener returns or `start()` is called once more.
- `receiveMessageOnPort(port)` bypasses all of it and pops one message
  synchronously.

`BroadcastChannel` has no enable step; it receives from construction.

## Transfer support matrix

A transfer list moves ownership instead of copying. It is the second argument
to `port.postMessage` / `worker.postMessage`, and `options.transfer` for
`structuredClone`.

| value | in a transfer list | in the message graph |
|---|---|---|
| `ArrayBuffer` | transferable — the receiver gets the original backing store, the sender's buffer is detached (`byteLength` 0, every view over it zero-length) | cloned |
| `MessagePort` | transferable — the sender's port is closed as a handle while its queue and channel membership travel to the receiver, so a sender on the far end keeps queueing into it while it is in flight | `DataCloneError` unless it is also listed |
| `SharedArrayBuffer` | **not** transferable — `DataCloneError` | *shared*: the receiver builds a second `SharedArrayBuffer` over the same memory, and writes through either are visible through the other |
| everything else | `DataCloneError` | per the [structured clone rules](structured-clone.md) |

### Rejections

Every one of these is a `DOMException` named `DataCloneError`, so both
`e.name === "DataCloneError"` and `instanceof DOMException` detect them.

| condition | message |
|---|---|
| the port doing the posting is in its own transfer list | `Transfer list contains source port` |
| a listed port is already detached (closed, or transferred away) | `MessagePort in transfer list is already detached` |
| the same port listed twice | `Transfer list contains duplicate MessagePort` |
| the same `ArrayBuffer` listed twice | `The transfer list contains the same ArrayBuffer twice` |
| a listed `ArrayBuffer` is detached or not detachable | `An ArrayBuffer in the transfer list is detached and cannot be transferred` |
| a listed value branded by `markAsUntransferable` | `Cannot transfer object of unsupported type.` |
| anything else in the list (a non-object included) | `Found invalid value in transferList.` |
| a port reachable in the message but not listed | `Object that needs transfer was found in message but not listed in transferList` |
| a value branded by `markAsUncloneable`, anywhere in the graph | `Cannot clone object of unsupported type.` |

The duplicate-port message ends in the constructor name of the listed object,
so a subclass of `MessagePort` names itself there.

Those are the checks the native collector runs. What reaches it depends on the
entry point, and a list argument of the wrong *shape* is a `TypeError` rather
than a `DataCloneError`:

- `port.postMessage(value, transfer)` does the WebIDL sequence conversion in
  JavaScript, so an array, any iterable, or a `{ transfer }` dictionary all
  work. Anything else is `TypeError: postMessage: transfer is not iterable`.
- `worker.postMessage(value, transfer)` is native all the way down and takes an
  actual array; omitting it or passing `undefined`/`null` means "transfer
  nothing", and any other value is
  `TypeError: The transfer list must be an array`.
- `structuredClone(value, { transfer })` accepts any iterable and screens each
  entry in its own wrapper first, so an untransferable entry there is still a
  `DataCloneError` but carries that wrapper's message,
  `structuredClone: value in transfer list is not transferable`, rather than
  `Found invalid value in transferList.`

### Nothing changes hands until the whole graph is written

Validation and serialization run to completion before a single buffer is
detached or a single port is handed over. A `DataCloneError` from the middle of
a graph therefore leaves **every port and every buffer in the list exactly as
it found them** — still open, still holding their memory — so a failed
`postMessage` can be corrected and retried.

The listed ports and buffers are re-checked after the write as well, because
writing the graph runs user getters and one of them may have closed a listed
port or detached a listed buffer; those late failures are the same
`MessagePort in transfer list is already detached` and `An ArrayBuffer in the
transfer list is detached and cannot be transferred`. What they undo is the
transfer — nothing is detached, nothing changes hands — not what the getters
did on the way there: a port a getter closed stays closed.

A listed port that the value itself never names still travels, but on arrival
it has no way out: a `message` event hands it over in `event.ports`, while
`structuredClone` and `receiveMessageOnPort` return only the value. Those two
close such a port as soon as it arrives, so its sibling learns the channel is
gone instead of queueing into a port nothing can ever read.

## Worker messages

The runtime's own `Worker` and the worker global scope are `EventTarget`s that
deliver real `MessageEvent`s, so `worker.onmessage`, `worker.addEventListener`,
and the same pair on `globalThis` inside a worker, all work and interleave in
installation order. Handlers keep receiving the payload as `event.data`.

`worker.postMessage` differs from `port.postMessage` in one respect: an
interop/native object anywhere in the graph is delivered as an empty object
rather than raising a `DataCloneError`, which is long-standing behaviour app
code relies on. Transfer is not part of that leniency — a port in a worker
transfer list is validated exactly as it is everywhere else, since degrading a
transfer would strand the port's sibling.

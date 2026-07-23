# Error handling

The runtime implements the WHATWG error model at the global level: uncaught JavaScript exceptions and unhandled promise rejections are dispatched as cancelable events on `globalThis`, Java exceptions round-trip into JavaScript with the original `Throwable` attached, and `interop.escapeException` forwards a JavaScript throw to the Java caller as the **original** Java exception. Unlike iOS, a truly-uncaught exception crashes the app by default (the Java default uncaught-exception handler ends the process) — `preventDefault()` and `discardUncaughtJsExceptions` are the opt-outs. Unhandled rejections and `reportError` only report; they never crash.

## Quick reference

| Situation | Default behavior |
|---|---|
| Uncaught JS exception during a Java→JS call (overridden method, interface implementation) | Becomes a real `com.tns.NativeScriptException` thrown to the Java caller. If nothing catches it, the thread's uncaught-exception handler reports it (cancelable `error` event → `__onUncaughtError` hook → error activity in debug builds) and the process exits — unless a listener called `preventDefault()`. |
| Unhandled promise rejection | Tracked per isolate, reported once per looper turn: cancelable `unhandledrejection` event → `__onUncaughtError` hook, logcat entry prefixed `Unhandled promise rejection:`. The app keeps running. |
| `.catch()` added after the report | `rejectionhandled` event (non-cancelable), carrying the original reason. |
| Java exception during a JS→Java call | Surfaced to JS as an `Error` carrying the original as `error.nativeException`. |
| `throw interop.escapeException(x)` in JS called from Java | The original Java `Throwable` carried by `x` is rethrown **unwrapped** to the Java caller (JS trace attached as a suppressed `com.tns.JavaScriptStackTrace`); with no underlying `Throwable`, a `com.tns.NativeScriptException` whose stack trace is the JS frames. |
| `reportError(x)` | Routed through the same pipeline as an uncaught error; never crashes. |

## JavaScript API

### Global error events

```js
globalThis.addEventListener("error", (e) => {
  // e is an ErrorEvent: { message, error, filename, lineno, colno }
  // (filename/lineno/colno are not populated yet)
  console.log(e.message, e.error);
  e.preventDefault(); // marks the error handled: no hook, no crash, no error activity
});

globalThis.addEventListener("unhandledrejection", (e) => {
  // e is a PromiseRejectionEvent: { promise, reason }
  console.log(e.reason);
  e.preventDefault();
});

globalThis.addEventListener("rejectionhandled", (e) => {
  // fired (as a task, on a following looper turn) when a handler is attached
  // to a promise whose rejection was already reported; carries the original
  // reason. Not cancelable.
});
```

Notes:

- `error` and `unhandledrejection` are `cancelable`; `preventDefault()` suppresses every downstream consequence (legacy hooks, logcat report, the error activity, the process crash).
- Events fire even if app code overwrites `globalThis.dispatchEvent` — native dispatch goes through closures captured at startup.
- A listener that throws does not stop the remaining listeners; the thrown value is routed to the fatal reporting tail directly (never recursively dispatched as another `error` event).
- The events also fire on worker globals. A worker's unhandled rejection dispatches `unhandledrejection` on the worker's own global first; only when unprevented does it continue to the worker-global `onerror` and then to the parent's `worker.onerror`, mirroring uncaught worker errors.

### `reportError`

Routes a caught-but-fatal error through the exact same pipeline as an uncaught exception:

```js
reportError(new Error("something unrecoverable"));
```

### Event classes

`Event`, `EventTarget`, `ErrorEvent` and `PromiseRejectionEvent` are installed as global constructors. `Event`/`EventTarget` are general-purpose (registration order, `once`/`capture` options, `stopImmediatePropagation`, `handleEvent` objects) and usable for your own eventing:

```js
const target = new EventTarget();
target.addEventListener("tick", (e) => { /* ... */ }, { once: true });
target.dispatchEvent(new Event("tick")); // returns !defaultPrevented
```

### What lands on the events

The stacks live on the error/reason **value**, not on the event — and the thrown value can be anything, so shape-check before use:

| You wrote | `e.error` / `e.reason` is | JS stack | Native exception |
|---|---|---|---|
| `throw new Error("x")` | that `Error` | `e.error.stack` | — |
| called a Java method that threw, without try/catch | an `Error` with `message` from the Java exception's message | `e.error.stack` (the JS call site); `e.error.stackTrace` combines it with the Java frames | `e.error.nativeException` — the original `Throwable` (call `.getClass()`, `.getMessage()`, `.getCause()`, ... on it) |

### Catching native exceptions

```js
try {
  someJavaObject.methodThatThrowsIOException();
} catch (e) {
  e.nativeException instanceof java.io.IOException; // true
  e.nativeException.getMessage();                   // the Java message
  e.stackTrace;                                     // combined JS + Java stack as a string
}
```

### Forwarding a throw to native: `interop.escapeException`

A plain JS throw inside a Java-invoked callback already escapes to the Java caller — as a `com.tns.NativeScriptException`. That is the right default, but when the caller is waiting for a *concrete* exception type, the wrapper doesn't match its `catch`. Branding the throw forwards the **original** Java exception instead:

```js
const listener = new some.api.Listener({
  onEvent() {
    try {
      riskyJavaCall(); // throws java.io.IOException
    } catch (e) {
      throw interop.escapeException(e); // the Java caller catches the ORIGINAL IOException
    }
  },
});
```

Semantics:

- `escapeException(err)` returns a JS `Error` (message/stack copied), so it behaves like a normal throw in pure-JS paths; the brand is an isolate-private symbol that user code cannot forge. Passing an already-branded value is a no-op; calling with no argument throws `TypeError`.
- If `err` is (or carries via `.nativeException`) a Java `Throwable`, the **original object** is rethrown at the boundary — a Java `catch (IOException e)` above the caller matches, and `Throwable` identity is preserved (same object, untouched class/stack/cause chain). The JS journey rides along as a suppressed `com.tns.JavaScriptStackTrace` (see the native section).
- Otherwise a `com.tns.NativeScriptException` is thrown as usual, but with its stack trace replaced by frames synthesized from the JS stack, so crash reporters group it by where it actually happened in JS.
- The `escapeException()` call site's stack is recorded too — for non-Error values (`escapeException("boom")`) it is the only stack available.
- Branded escapes bypass `discardUncaughtJsExceptions` (an explicit forward request must reach the caller).

## Native (Java) API

### Catching escaped exceptions

```java
try {
    listener.onEvent(); // implemented in JS
} catch (java.io.IOException e) {
    // For rethrown originals: e is the very same object the JS code caught.
    // For synthesized escapes: catch com.tns.NativeScriptException instead -
    // its message is the JS error's message and its stack trace is the JS frames.
}
```

### JS stack traces on Java exceptions: `com.tns.JavaScriptStackTrace`

An escaped original exception carries its JavaScript journey as a suppressed throwable, so it renders automatically in `printStackTrace()`, logcat fatal logs and crash reporters:

```
java.io.IOException: original-io-exception
    at com.example.SomeApi.riskyJavaCall(SomeApi.java:42)
    ...
    Suppressed: com.tns.JavaScriptStackTrace: Error: original-io-exception
        at <js>.onEvent(main-view-model.js:17)
        ...
```

`JavaScriptStackTrace` is never thrown — only attached — and its stack trace elements are synthesized from the V8 frames. Crash-SDK integrations can look it up and read the raw stacks:

```java
for (Throwable suppressed : caught.getSuppressed()) {
    if (suppressed instanceof com.tns.JavaScriptStackTrace) {
        com.tns.JavaScriptStackTrace jsTrace = (com.tns.JavaScriptStackTrace) suppressed;
        String originStack = jsTrace.getJavaScriptStack();   // where the JS error was created
        String escapeStack = jsTrace.getEscapeSiteStack();   // where interop.escapeException() was called
    }
}
```

| Exception | Where the JS stack lives |
|---|---|
| Rethrown original `Throwable` | suppressed `com.tns.JavaScriptStackTrace` (identity, stack and cause chain of the original are untouched) |
| Synthesized escape (`com.tns.NativeScriptException`) | the exception's own stack trace elements are the JS frames; the message is the JS error's message |

`JavaScriptStackTrace` and its two accessors are the stable contract for crash-SDK integrations; other exception paths may adopt the carrier in the future.

## Configuration

| Flag (app `package.json`, default off) | Effect |
|---|---|
| `discardUncaughtJsExceptions` | JS exceptions escaping an overridden-method call are swallowed on the Java side and reported through `__onDiscardedError` instead of crashing the app. Branded `interop.escapeException` throws bypass it. |

There is no `crashOnUncaughtJsExceptions` flag (unlike iOS): crashing on truly-uncaught exceptions is already Android's default.

Terminal-path decision table:

| Condition | legacy hook called | process crash |
|---|---|---|
| uncaught exception, default | `__onUncaughtError` | yes |
| uncaught exception, `discardUncaughtJsExceptions` | `__onDiscardedError` | no |
| uncaught exception, listener called `preventDefault()` | none | no |
| unhandled rejection / `reportError`, unprevented | `__onUncaughtError` | no |
| unhandled rejection / `reportError`, `preventDefault()` | none | no |

## Crash reporter integration

JS side — attach both the JS and native exception from one listener:

```js
globalThis.addEventListener("error", (e) => {
  const err = e.error;
  const native = err && err.nativeException;
  crashReporter.capture(err instanceof Error ? err : new Error(e.message), {
    nativeClass: native ? native.getClass().getName() : undefined,
    nativeMessage: native ? native.getMessage() : undefined,
  });
  // e.preventDefault(); // only if the reporter fully owns error handling
});
```

Java side — for exceptions that never pass through the JS event layer (escaped originals crashing a thread), walk `getSuppressed()` for `com.tns.JavaScriptStackTrace` to attach the JS frames. For embedders with a custom `Thread.UncaughtExceptionHandler`: `Runtime.passUncaughtExceptionToJs(...)` returns `true` when a listener called `preventDefault()` — honor it by not killing the process (see `NativeScriptUncaughtExceptionHandler`).

## Legacy hooks (deprecated)

`global.__onUncaughtError` and `global.__onDiscardedError` keep working exactly as before and are what `@nativescript/core` currently installs (surfaced as `Application.uncaughtErrorEvent` / `discardedErrorEvent`). They are invoked only when no event listener called `preventDefault()`. New code should prefer `globalThis.addEventListener("error" | "unhandledrejection", ...)`.

## Behavior details

- Every error is reported exactly once: either the JS→Java boundary (synchronous throws during Java-invoked JS), the rejection drain (once per looper turn, scheduled on the runtime's `ALooper`), or `reportError` — never two of them for the same error.
- A rejection that gets a handler before the end-of-turn drain is never reported (and produces no `rejectionhandled` either).
- The `error` event for uncaught exceptions fires when the exception is reported to JS (from the uncaught-exception handler via `passUncaughtExceptionToJs`, or the discard path) — after the Java stack has already unwound. `preventDefault()` prevents the crash, but on the main thread the app's looper has exited by then; for background and JS-only threads the process genuinely keeps running.
- Worker isolates run the same machinery: each worker has its own tracker, drain, and event layer.

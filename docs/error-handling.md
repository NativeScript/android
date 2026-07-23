# Error handling

The runtime implements the WHATWG error model at the global level: uncaught JavaScript exceptions and unhandled promise rejections are dispatched as cancelable events on `globalThis`, Java exceptions round-trip into JavaScript with the original `Throwable` attached, and `interop.escapeException` forwards a JavaScript throw to the Java caller as the **original** Java exception. Following the web's model — an erroring page doesn't crash the browser — **an uncaught error never crashes the app by default**: it is reported (event → hook → log) and execution continues. Crashing is opt-in via `uncaughtErrorPolicy: "throw"`; suppressing a report entirely is per-error via `preventDefault()`.

## Quick reference

| Situation | Default behavior |
|---|---|
| Uncaught JS exception in a **native-initiated** callback (the OS invoking an overridden method or interface implementation, a posted `Runnable`, a timer, `__runOnMainThread`, a frame callback) | **Contained at the boundary**: reported (cancelable `error` event → `__onUncaughtError` hook → logcat), the Java caller receives the default value for the return type, and the app keeps running. |
| Uncaught JS exception in a **JS-initiated** chain (JS → Java → JS callback throws) | **Propagates back to the outer JS `catch`** as the very same JS error object — correct JavaScript semantics; each JS frame on the way gets its chance to catch. Only if it reaches an outermost native-initiated boundary is it contained. |
| Unhandled promise rejection | Tracked per isolate, reported once per looper turn: cancelable `unhandledrejection` event → `__onUncaughtError` hook, logcat entry prefixed `Unhandled promise rejection:`. The app keeps running. |
| `.catch()` added after the report | `rejectionhandled` event (non-cancelable), carrying the original reason. |
| Java exception during a JS→Java call | Surfaced to JS as an `Error` carrying the original as `error.nativeException`. |
| `throw interop.escapeException(x)` in JS called from Java | Never contained. The original Java `Throwable` carried by `x` is rethrown **unwrapped** to the Java caller (JS trace attached as a suppressed `com.tns.JavaScriptStackTrace`); with no underlying `Throwable`, a `com.tns.NativeScriptException` whose stack trace is the JS frames. |
| `reportError(x)` | Routed through the same pipeline as an uncaught error; never crashes. |
| `uncaughtErrorPolicy: "throw"` | Restores the pre-9.1 behavior: unprevented uncaught errors are thrown to the native layer as real Java exceptions (which typically ends the process via the default uncaught-exception handler). |

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
| `throw new Error("x")` | that `Error` (the actual thrown value) | `e.error.stack` | — |
| called a Java method that threw, without try/catch | an `Error` with `message` from the Java exception's message | `e.error.stack` (the JS call site); `e.error.stackTrace` combines it with the Java frames | `e.error.nativeException` — the original `Throwable` (call `.getClass()`, `.getMessage()`, `.getCause()`, ... on it) |
| `throw new java.io.IOException("x")` — a directly-thrown wrapped `Throwable` | the wrapped `Throwable` itself — not an `Error`, no `.stack` | — | `e.error` directly (`instanceof java.io.IOException`) |

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
- If `err` is (or carries via `.nativeException`) a Java `Throwable`, the **original object** is rethrown at the boundary — a Java `catch (IOException e)` above the caller matches, and `Throwable` identity is preserved (same object, untouched class/stack/cause chain). The JS journey rides along as a suppressed `com.tns.JavaScriptStackTrace` (see the native section). This includes directly-constructed exceptions:

```js
// The caller catches THIS exact IOException - no wrapper. Without the brand,
// a directly-thrown wrapped Throwable behaves like any other uncaught throw:
// contained (reported, caller resumes) in a native-initiated callback, or -
// in a JS-initiated chain - propagated to the outer JS catch (and, if it
// reaches Java code with no JS below, wrapped in a com.tns.NativeScriptException
// with the IOException as its cause).
throw interop.escapeException(new java.io.IOException("x"));
```
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

One policy key in the app's `package.json` (root level) governs what happens to an **unprevented** uncaught error or unhandled rejection:

| `uncaughtErrorPolicy` | Effect |
|---|---|
| `"report"` (default) | Report (event → hook → log) and continue. Never crashes. |
| `"throw"` | After the (cancelable) event, the error is thrown to the native layer as a real Java exception — `com.tns.NativeScriptException` with the JS frames as its stack trace — and reported through the thread's uncaught-exception path. This typically ends the process (the pre-9.1 default), though a native catch or a custom `UncaughtExceptionHandler` above the boundary can still intercept it, which is why the policy names the mechanism, not a guaranteed crash. Unhandled rejections are thrown from a clean frame on the runtime's looper. |

Deprecated (kept for the transition, both emit a logcat warning):

| Flag | Behavior |
|---|---|
| `discardUncaughtJsExceptions: true` | Legacy quiet routing: contained reports call `__onDiscardedError` instead of `__onUncaughtError` and skip the logcat report. Branded `interop.escapeException` throws bypass it. |
| `discardUncaughtJsExceptions: false` | Ignored (this used to mean "crash on uncaught errors" — set `uncaughtErrorPolicy: "throw"` for that). |

Terminal-path decision table:

| Condition | legacy hook called | process crash |
|---|---|---|
| uncaught error, default (`"report"`) | `__onUncaughtError` | no |
| uncaught error, `"report"` + `discardUncaughtJsExceptions: true` | `__onDiscardedError` | no |
| uncaught error, listener called `preventDefault()` | none | no |
| uncaught error / unhandled rejection, `"throw"`, unprevented | `__onUncaughtError` (via the uncaught-exception path) | yes, normally |
| unhandled rejection / `reportError`, `"report"`, unprevented | `__onUncaughtError` | no |
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

Java side — for exceptions that never pass through the JS event layer (escaped originals crashing a thread, `"throw"`-policy fatals), walk `getSuppressed()` for `com.tns.JavaScriptStackTrace` to attach the JS frames. For embedders with a custom `Thread.UncaughtExceptionHandler`: `Runtime.passUncaughtExceptionToJs(...)` returns `true` when a listener called `preventDefault()` — honor it by not killing the process (see `NativeScriptUncaughtExceptionHandler`).

## Legacy hooks (deprecated)

`global.__onUncaughtError` and `global.__onDiscardedError` keep working exactly as before and are what `@nativescript/core` currently installs (surfaced as `Application.uncaughtErrorEvent` / `discardedErrorEvent`). They are invoked only when no event listener called `preventDefault()`. New code should prefer `globalThis.addEventListener("error" | "unhandledrejection", ...)`.

## Behavior details

- **Containment is boundary-outermost.** The runtime tracks the depth of in-flight JS→Java calls; a throw with JS frames waiting below the boundary propagates (so `try { javaApi.call(cb) } catch` works, with the original JS error object restored across the crossing), and is contained only at an outermost native-initiated entry. `interop.escapeException` and `uncaughtErrorPolicy: "throw"` are the two ways an error crosses that outermost boundary.
- **Contained callbacks return type defaults.** A throwing overridden method hands its Java caller `null` for reference types and `0`/`false` for primitives (the runtime substitutes the default before unboxing, so no `NullPointerException` from the binding). The error is loudly reported *before* the caller resumes, so logcat shows the real failure ahead of any downstream symptom. If the Java contract genuinely needs the exception, use `interop.escapeException`.
- Every error is reported exactly once: either the containment point, the rejection drain (once per looper turn, scheduled on the runtime's `ALooper`), `reportError`, or — under `"throw"` — the thread's uncaught-exception path. Never two of them for the same error.
- A rejection that gets a handler before the end-of-turn drain is never reported (and produces no `rejectionhandled` either).
- Module/script evaluation (`runModule`/`runScript`, app bootstrap) is not contained — an app whose main module fails to load still fails loudly.
- Worker isolates run the same machinery: each worker has its own tracker, drain, event layer and containment.

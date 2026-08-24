# AbortController / AbortSignal

The runtime installs the DOM Standard's abort primitives as globals in every
isolate (main and workers): `AbortController`, and `AbortSignal` with the
`abort`, `timeout` and `any` statics. The implementation is
`test-app/runtime/src/main/cpp/js/abort-signal.js`, evaluated during `Events::Init`
right after the `Event`/`EventTarget` builtin it is layered on, so the
interfaces exist before any user code runs. `AbortSignal` extends the
runtime's `EventTarget`; `new AbortSignal()` throws `TypeError: Illegal
constructor` — instances come from a controller or one of the statics.

## Surface

- `new AbortController()` — `controller.signal` (stable identity) and
  `controller.abort(reason?)`.
- `signal.aborted`, `signal.reason`, `signal.throwIfAborted()`, and the
  `abort` event (`addEventListener("abort", …)` or the `onabort` handler
  attribute with HTML event-handler semantics).
- `AbortSignal.abort(reason?)` — an already-aborted signal; no event fires.
- `AbortSignal.timeout(delay)` — aborts with a `TimeoutError`-named reason
  after `delay` ms. `delay` must be an integer in `[0, 2^32 − 1]`
  (`TypeError` for non-numbers, `RangeError` otherwise), matching Node's
  validation.
- `AbortSignal.any(signals)` — a composite signal that aborts with the first
  source's reason. Accepts any iterable whose members are all `AbortSignal`s
  (`TypeError` otherwise). Composites are flattened: `any([any([a]), b])`
  follows `a` and `b` directly. Per spec, every affected signal's
  `aborted`/`reason` flips before the first `abort` event fires.

## Deviations from Node / the web

- **No `DOMException`.** As with [structuredClone](structured-clone.md) and
  the [Performance API](performance.md), default reasons are `Error`
  instances with `name` patched: `"AbortError"` (default abort) and
  `"TimeoutError"` (timeout). `instanceof DOMException` checks cannot work;
  match on `reason.name`.
- **No `WeakRef` bookkeeping.** Node wraps timeout signals and `any()`
  linkage in `WeakRef`s/`FinalizationRegistry`s so an unobserved signal can
  be collected early and timers never keep the process alive. Here a
  `timeout()` timer holds its signal strongly until it fires — retention is
  bounded by the delay, and looper timers don't gate process liveness — and
  `any()` links source → dependent strongly, unlinking as soon as either
  side aborts. The visible behavior is the same; only collection timing
  differs.
- Abort events carry no `isTrusted` flag (the runtime's `Event` doesn't
  model it).

Listener errors during the abort dispatch go through the runtime's standard
listener-error pipeline (see [error handling](error-handling.md)); a throwing
listener never prevents the remaining listeners from running.

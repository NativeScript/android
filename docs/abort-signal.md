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

## GC contract

The implementation is GC-transparent the way Node's is: internal references
never keep an unobservable signal alive, and never let an observable abort
be dropped.

- A `timeout()` timer closes over a `WeakRef`, so a signal nobody can
  observe is collectable before it fires; a `FinalizationRegistry` cancels
  the pending native timer when that happens.
- `any()` links are `WeakRef`s in both directions (source → dependent and
  dependent → source), with prune registries clearing dead entries — so
  per-request composites never accumulate on a long-lived source, and a
  collected source leaves its composites' source lists (a composite whose
  sources are all gone can never abort and stops being retained).
- Weakness alone would silently drop the abort of a signal that is
  listened-to but otherwise unreachable, so a strong `gcPersistentSignals`
  set holds exactly the signals whose abort someone can still observe: live
  timeout signals and live non-empty composites while they have `abort`
  listeners (`onabort` counts — it registers a real listener), plus timeout
  sources a composite follows, until their timer fires. The listener
  accounting comes from an internal symbol-keyed hook the events builtin
  calls from every listener-list mutation path (add, remove, and `once`
  removal during dispatch); the key is handed to the abort builtin in a
  one-shot during init and never reaches app code, so the accounting cannot
  be bypassed via a captured `EventTarget.prototype.addEventListener`.

Entries leave the persistent set on abort, on the last abort-listener
removal, or when a composite loses its last source.

## Deviations from Node / the web

- **No `DOMException`.** As with [structuredClone](structured-clone.md) and
  the [Performance API](performance.md), default reasons are `Error`
  instances with `name` patched: `"AbortError"` (default abort) and
  `"TimeoutError"` (timeout). `instanceof DOMException` checks cannot work;
  match on `reason.name`.
- Abort events carry no `isTrusted` flag (the runtime's `Event` doesn't
  model it).

Listener errors during the abort dispatch go through the runtime's standard
listener-error pipeline (see [error handling](error-handling.md)); a throwing
listener never prevents the remaining listeners from running.

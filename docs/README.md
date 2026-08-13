# Runtime documentation

- [Node-API](node-api.md) — the standard `napi_*` C ABI for native addons:
  building against the prefab package, registering and requiring addons,
  threading and finalizer contracts, and the documented divergences from Node
  (shared with the iOS runtime).
- [Performance API](performance.md) — WHATWG `performance` (hr-time, user
  timing, performance timeline with `PerformanceObserver`), per-isolate time
  origins for workers, the native clock hook that future `requestAnimationFrame`
  work must share, and the documented spec deviations.
- [Error handling](error-handling.md) — global `error`/`unhandledrejection` events, `reportError`, catching Java exceptions in JS (`error.nativeException`), forwarding JS throws to Java callers (`interop.escapeException`), JS stacks on Java exceptions (`com.tns.JavaScriptStackTrace`), configuration flags, and crash-reporter integration.
- [structuredClone](structured-clone.md) — the WHATWG `structuredClone(value, { transfer })` global: what clones, how graph identity and cycles are preserved, `ArrayBuffer` transfer, and the `DataCloneError`-named `Error` that stands in for `DOMException`.
- [Implementing additional Chrome DevTools protocol Domains](extending-inspector.md)

## Knowledge

Notes on work that is done, kept because the reasoning is expensive to
reconstruct rather than because anything needs doing.

- [V8 10.3 → 14.9 migration](knowledge/v8-14-migration.md) — the API changes and
  their site counts, why each non-default gn arg exists, the accessor rules that
  are not mechanical, and the traps that only show up at runtime.

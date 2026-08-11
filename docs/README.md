# Runtime documentation

- [Error handling](error-handling.md) — global `error`/`unhandledrejection` events, `reportError`, catching Java exceptions in JS (`error.nativeException`), forwarding JS throws to Java callers (`interop.escapeException`), JS stacks on Java exceptions (`com.tns.JavaScriptStackTrace`), configuration flags, and crash-reporter integration.
- [structuredClone](structured-clone.md) — the WHATWG `structuredClone(value, { transfer })` global: what clones, how graph identity and cycles are preserved, `ArrayBuffer` transfer, and the `DataCloneError`-named `Error` that stands in for `DOMException`.
- [Implementing additional Chrome DevTools protocol Domains](extending-inspector.md)

## Knowledge

Notes on work that is done, kept because the reasoning is expensive to
reconstruct rather than because anything needs doing.

- [V8 10.3 → 14.9 migration](knowledge/v8-14-migration.md) — the API changes and
  their site counts, why each non-default gn arg exists, the accessor rules that
  are not mechanical, and the traps that only show up at runtime.

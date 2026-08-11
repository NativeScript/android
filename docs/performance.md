# Performance API

The runtime implements the WHATWG/WinterTC-standard Performance surface: [High
Resolution Time](https://w3c.github.io/hr-time/), [User Timing Level
3](https://w3c.github.io/user-timing/) and the [Performance
Timeline](https://w3c.github.io/performance-timeline/) with
`PerformanceObserver`.

## Surface

Globals (own, writable, enumerable, configurable properties of `globalThis`,
in main and worker isolates alike): `performance`, `Performance`,
`PerformanceEntry`, `PerformanceMark`, `PerformanceMeasure`,
`PerformanceObserver`, `PerformanceObserverEntryList`.

- `performance.now()` — double milliseconds since the isolate's time origin,
  monotonic (V8's platform clock, `CLOCK_MONOTONIC`-based: it does not tick
  while the device is suspended), full double precision with no coarsening.
- `performance.timeOrigin` — readonly accessor; wall-clock milliseconds since
  the Unix epoch, sampled once when the isolate's runtime is created. Each
  worker gets its own time origin at worker-thread start, so
  `timeOrigin + now()` approximates `Date.now()` per isolate while the device
  stays awake; it drifts behind after device suspend (the monotonic clock does
  not tick then) and diverges under wall-clock adjustments.
- `performance.toJSON()`, `Symbol.toStringTag`, and `Performance extends
  EventTarget` per spec; `performance`, `PerformanceEntry`,
  `PerformanceMeasure` and `PerformanceObserverEntryList` are not
  user-constructible (`new` throws `TypeError`); `new PerformanceMark(name,
  options)` is constructible per spec but does not buffer the entry.
- User timing: `mark(name, {startTime, detail})`, `measure(name,
  startOrOptions, endMark)` with the full Level 3 options algebra (`{start,
  end, duration, detail}`, mark names or timestamps, over-/under-constraint
  errors), `clearMarks(name?)`, `clearMeasures(name?)`.
- Timeline: `getEntries()`, `getEntriesByType(type)`, `getEntriesByName(name,
  type?)` return copies sorted chronologically by `startTime` (stable for
  ties).
- Observers: `new PerformanceObserver(cb)`, `observe({entryTypes})` or
  `observe({type, buffered})`, `disconnect()`, `takeRecords()`, static frozen
  `PerformanceObserver.supportedEntryTypes === ["mark", "measure"]`.

## Architecture

All spec logic lives in the `internal/performance.js` builtin
(`test-app/runtime/src/main/cpp/js/performance.js`), shared with the iOS
runtime: the native side hands it only `{ now(), timeOrigin }`, so the same
file runs unchanged on both runtimes and should be kept in sync with iOS's
copy.

The native clock is owned by `Runtime` (`Runtime::PerformanceNowMillis()`,
`Runtime::TimeOriginMillis()`, `Runtime::TimeOriginMonotonicMillis()`,
captured in `Runtime::PrepareV8Runtime`) and exposed to native callers through
`tns::Performance::NowMillis(isolate)`
(`test-app/runtime/src/main/cpp/Performance.h`). Any native producer of
JS-visible timestamps must read the clock through that hook rather than
sampling its own, so every timestamp shares `performance.timeOrigin` as its
base.

## Frame callbacks

`__postFrameCallback(fn[, delayMillis])` / `__removeFrameCallback(fn)`
(`test-app/runtime/src/main/cpp/FrameCallbacks.h`) schedule `fn` for the next
display frame. `fn` receives **two** arguments:

```js
__postFrameCallback((frameTimeNanos, performanceMillis) => { … });
```

- `frameTimeNanos` — the platform's raw frame time: `CLOCK_MONOTONIC`
  nanoseconds, the `System.nanoTime()` base. Unchanged from earlier runtimes,
  which passed it as the only argument.
- `performanceMillis` — the same instant on this isolate's performance
  timeline, so it compares directly with `performance.now()`. Converted
  natively through `Performance::MonotonicNanosToTimelineMillis()`, which
  subtracts `Runtime::TimeOriginMonotonicMillis()` — Choreographer stamps
  frames on the very clock the time origin is captured on, so the mapping is
  exact rather than an approximation resampled in JS.

Two implementations sit behind that one surface: the NDK's `AChoreographer`
(API 24+, resolved with `dlsym`) and `android.view.Choreographer` through
`com.tns.FrameCallbacks` for API 21–23, where the NDK API does not exist.
Scheduling is per calling thread, so a worker schedules against its own looper.
Both paths produce the same two arguments with the same exactness.

## Deviations from the specs

- **Buffers are unbounded.** Per spec for user timing. `detail` is
  structured-cloned at entry creation (per spec — an uncloneable `detail`
  throws the `DataCloneError`-named error, see
  [structuredClone](structured-clone.md)), so entries hold snapshots, but a
  long-lived app marking in a loop should still clear entries periodically.
- **Observer callbacks run from a microtask**, not a queued task: delivery is
  asynchronous relative to `mark()`/`measure()` but precedes timer callbacks
  scheduled in the same turn. Callback exceptions are routed to
  `reportError`, so one throwing observer does not starve the others.
- **No `DOMException`.** Errors the specs express as `DOMException` — the
  `SyntaxError` for a missing mark name, the `InvalidModificationError` for
  switching an observer between the `entryTypes` and `type` forms — are
  `Error` instances with `name` patched. `err.name` checks work;
  `instanceof DOMException` does not.
- Browser-only surface is absent: no resource/navigation timing, no
  `eventCounts`, and no `PerformanceTiming`-attribute resolution in
  `measure()`.

// HMRSupport.h
#pragma once

#include <string>
#include <vector>

// Forward declare v8 types to keep this header lightweight and avoid
// requiring V8 headers at include sites.
namespace v8 {
class Isolate;
template <class T> class Local;
class Object;
class Function;
class Context;
class Value;
class Promise;
}

namespace tns {

// HMRSupport: Isolated helpers for minimal HMR (import.meta.hot) support.
//
// This module contains:
// - Per-module hot data store
// - Registration for accept/disable callbacks
// - Active dev-session state and helpers
// - Initializer to attach import.meta.hot to a module's import.meta
//
// Note: Triggering/dispatch is handled by the HMR system elsewhere.

// Retrieve or create the per-module hot data object.
v8::Local<v8::Object> GetOrCreateHotData(v8::Isolate* isolate, const std::string& key);

// Register accept and dispose callbacks for a module key.
void RegisterHotAccept(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb);
void RegisterHotDispose(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb);

// Register prune callbacks for a module key. Per Vite spec these fire when the
// module is removed from the dependency graph (NOT on every update — that is
// dispose). The registry is plumbed end-to-end; a per-module HMR client drains
// it via `__nsRunHmrPrune`.
void RegisterHotPrune(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb);

// Optional: expose read helpers (may be useful for debugging/integration)
std::vector<v8::Local<v8::Function>> GetHotAcceptCallbacks(v8::Isolate* isolate, const std::string& key);
std::vector<v8::Local<v8::Function>> GetHotDisposeCallbacks(v8::Isolate* isolate, const std::string& key);
std::vector<v8::Local<v8::Function>> GetHotPruneCallbacks(v8::Isolate* isolate, const std::string& key);

// `import.meta.hot` implementation — Vite-spec compliant API surface.
//
// Per-module API exposed on every imported module:
// - `hot.data`             — per-module persistent object across HMR updates
// - `hot.accept(deps?, cb?)` — register a self-accepting handler (deps arg accepted but currently ignored)
// - `hot.dispose(cb)`      — register a cleanup callback fired when this module is replaced
// - `hot.prune(cb)`        — register a callback fired when this module is removed from the dep graph
// - `hot.decline()`        — opt this module out of HMR (next update touching it triggers full reload)
// - `hot.invalidate(msg?)` — request a full app reload from this module (delegates to `__nsReloadDevApp`)
// - `hot.on(event, cb)`    — listen to HMR events (Vite standard `vite:beforeUpdate` / `vite:afterUpdate` /
//                            `vite:beforeFullReload` / `vite:beforePrune` / `vite:invalidate` / `vite:error`,
//                            plus custom events the HMR client dispatches via `__NS_DISPATCH_HOT_EVENT__`)
// - `hot.off(event, cb)`   — unregister a listener previously added with `hot.on`
// - `hot.send(event, data)` — send a custom message to the dev server; delegated to a JS-installed
//                            `globalThis.__nsHmrSendToServer(event, data)` so the WebSocket-owning JS layer
//                            keeps sole responsibility for the transport (runtime stays transport-agnostic)
//
// `modulePath` is used to derive the per-module canonical key for `hot.data` and callback registries.
void InitializeImportMetaHot(v8::Isolate* isolate,
                             v8::Local<v8::Context> context,
                             v8::Local<v8::Object> importMeta,
                             const std::string& modulePath);

// ─────────────────────────────────────────────────────────────
// Dev session helpers

struct DevSessionState {
    bool active = false;
    bool started = false;
    std::string sessionId;
    std::string origin;
    std::string entryUrl;
    std::string clientUrl;
    std::string wsUrl;
    std::string platform;
    std::string runtimeConfigUrl;
    bool fullReload = false;
    bool cssHmr = false;
};

// Read and validate the JS dev-session config object.
bool ReadDevSessionConfig(v8::Isolate* isolate,
                          v8::Local<v8::Context> context,
                          v8::Local<v8::Object> config,
                          DevSessionState* out,
                          std::string* errorMessage);

// Active dev-session storage.
void ResetActiveDevSession();
DevSessionState GetActiveDevSessionSnapshot();
void StoreActiveDevSession(const DevSessionState& session);
bool HasDevSessionChanged(const DevSessionState& previous,
                          const DevSessionState& next);
std::vector<std::string> CollectSessionModuleUrls(const DevSessionState& session);
bool ApplyDevRuntimeConfigFromUrl(const std::string& url,
                                  std::string* errorMessage);

// Runtime global helpers for the deterministic dev session boot path.
void ApplyDevSessionGlobals(v8::Isolate* isolate,
                            v8::Local<v8::Context> context,
                            const DevSessionState& session);
void SetDevSessionBootComplete(v8::Isolate* isolate,
                               v8::Local<v8::Context> context,
                               bool value);

// ─────────────────────────────────────────────────────────────
// HTTP loader helpers (used by dev/HMR and general-purpose HTTP module loading)
//
// Normalize an HTTP(S) URL into a stable module registry/cache key.
// - Always strips URL fragments.
// - For NativeScript dev endpoints, normalizes known cache busters (e.g. t/v/import)
//   and normalizes some versioned bridge paths.
// - For non-dev/public URLs, preserves the full query string as part of the cache key.
std::string CanonicalizeHttpUrlKey(const std::string& url);

// Resolve a relative/root-absolute import specifier against a parent URL
// using plain string manipulation. Only relative (`./`, `../`) and
// root-absolute (`/`) specifiers are resolved here; bare specifiers
// return the empty string, and already-absolute http(s) URLs are
// returned unchanged. Returns the empty string when `parentUrl` is not
// http(s).
std::string ResolveImportSpecifierAgainstUrl(const std::string& specifier,
                                              const std::string& parentUrl);

// Minimal text fetch for HTTP ESM loader. Returns true on 2xx with non-empty body.
// - out: response body
// - contentType: Content-Type header if present
// - status: HTTP status code
//
// On a fast path, returns from the in-memory speculative-prefetch cache
// without touching the network. On the slow path, performs a synchronous
// fetch and additionally schedules background prefetches for the body's
// static imports so subsequent HttpFetchText calls hit the cache. See
// the prefetcher block in HMRSupport.cpp for full design notes.
bool HttpFetchText(const std::string& url, std::string& out, std::string& contentType, int& status);

// Return the most recent low-level fetch error reason for the calling
// thread, or an empty string if the last fetch succeeded (or no fetch
// has run on this thread yet).
//
// Format is grep-friendly and intended for splicing into the JS-side
// error message that `ModuleInternalCallbacks` throws when
// `HttpFetchText` returns `status=0`:
//
//   stage=get-response-code class=java.net.ConnectException msg=...
//
// The slot is thread-local because each isolate has its own JS thread;
// concurrent fetches on different threads cannot clobber each other.
// "Take" semantics — the slot is cleared on read so a stale reason
// can never leak into a later, successful fetch.
std::string TakeLastHttpFetchErrorReason();

// Drop all entries in the speculative-prefetch cache. Safe to call from
// any thread. Used by Runtime teardown and by HMR cache-poison scenarios
// where the dev server has indicated a graph version bump.
void ClearHttpModulePrefetchCache();

// Register a "yield" callback that `HttpFetchText` should invoke around its
// synchronous network turn so the caller can pump its own runloop (e.g. the
// JS-thread runloop so a placeholder UI can repaint during cold-boot).
//
// Default: a no-op (`NoopHttpFetchYield`). Android's main NativeScript
// isolate runs JS on the UI thread, so there is no separate JS-thread
// runloop to pump here; a host that drives its own loop can install a real
// pump (e.g. one calling ALooper_pollOnce(0)) via this hook.
//
// Pass `nullptr` to disable any yielding (used by hosts that drive their own
// run loop or by tests that want bit-for-bit deterministic fetch timing).
// Safe to call from any thread; reads use acquire/release ordering.
void RegisterHttpFetchYield(void (*callback)());

// Drop a specific URL set from the speculative-prefetch cache. Safe
// to call from any thread; missing keys are silently ignored. Used by
// `InvalidateModules` so that an HMR eviction also purges any stale
// HTTP body the previous prefetch wave (or kickstart) left behind.
// Without this, the kickstart's "skip if URL already cached"
// early-out, plus `HttpFetchText`'s destructive-read fast path, would
// happily serve V8 a stale body from the prior save — visible to the
// user as a 1-cycle lag between save and visual update.
void EvictHttpModulePrefetchCacheUrls(const std::vector<std::string>& urls);

// Kickstart an HMR-driven module prefetch
// rooted at `seedUrl`. Walks the static-import graph in parallel (up to
// `maxConcurrent` simultaneous HTTP fetches), storing every reachable
// module body in the speculative-prefetch cache. Blocks the calling
// thread until the BFS has fully drained or `timeoutSeconds` elapses.
//
// Designed to be invoked from JS (via `__nsKickstartHmrPrefetch`)
// immediately before the Angular HMR client re-imports the entry —
// by the time V8 walks the dep tree, every reachable body is already
// in `g_prefetchCache` and the walk runs at memory speed instead of
// network speed (turning a ~3s 200-fetch refresh into ~250ms).
//
// Returns `true` when the BFS drained cleanly. On timeout or seed
// fetch failure returns `false`; callers should treat that as "no
// kickstart speedup this round" and fall back to V8's normal
// synchronous walk, which always succeeds independently.
//
// `outFetchedCount` (optional) receives the number of distinct URLs
// fetched. `outElapsedMs` (optional) receives wall-clock time.
bool KickstartHmrPrefetchSync(const std::string& seedUrl,
                              int maxConcurrent,
                              double timeoutSeconds,
                              size_t* outFetchedCount,
                              uint64_t* outElapsedMs);

// Multi-URL kickstart for HMR cycles. Unlike the legacy seed-rooted
// variant above, this one fetches ONLY the explicit URL list it was
// given (no body scanning, no BFS recursion).
//
// This is the right shape for HMR: the dev server's
// `collectAngularEvictionUrls` already computed the inverse-dep
// closure of the changed file; re-discovering it via in-process
// scanning would just duplicate that work and re-fetch modules V8
// has already compiled. By feeding the precomputed list directly we
// turn N sequential `LoadHttpModuleForUrl` calls (the importer chain
// during V8's ResolveModuleCallback walk) into a single parallel
// wave that completes before V8 starts walking.
//
// Same semantics as `KickstartHmrPrefetchSync` for everything else:
// blocks the calling thread until the wave drains or `timeoutSeconds`
// elapses; cleared/blocked URLs are filtered up front; partial
// success is reported as success (the V8 walk falls back to
// per-module HttpFetchText for anything we couldn't pre-fill).
bool KickstartHmrPrefetchUrlsSync(const std::vector<std::string>& urls,
                                  int maxConcurrent,
                                  double timeoutSeconds,
                                  size_t* outFetchedCount,
                                  uint64_t* outElapsedMs);

// Clear all HMR-related v8::Global handles (g_hotData, g_hotAccept, g_hotDispose).
// MUST be called inside Runtime::~Runtime() before isolate disposal to prevent
// crashes during static destructor cleanup (__cxa_finalize_ranges).
void CleanupHMRGlobals();
// ─────────────────────────────────────────────────────────────
// Custom HMR event support

// Register a custom event listener (called by import.meta.hot.on())
void RegisterHotEventListener(v8::Isolate* isolate, const std::string& event, v8::Local<v8::Function> cb);

// Unregister a listener previously added with `RegisterHotEventListener`. The
// callback is matched by V8 strict equality (same `Function` reference). If
// `cb` matches multiple registered listeners (the same closure was registered
// twice), every match is removed — mirrors `EventTarget.removeEventListener`
// semantics for repeated registrations.
void RemoveHotEventListener(v8::Isolate* isolate, const std::string& event, v8::Local<v8::Function> cb);

// Get all listeners for a custom event
std::vector<v8::Local<v8::Function>> GetHotEventListeners(v8::Isolate* isolate, const std::string& event);

// Dispatch a custom event to all registered listeners
// This should be called when the HMR WebSocket receives framework-specific events
void DispatchHotEvent(v8::Isolate* isolate, v8::Local<v8::Context> context, const std::string& event, v8::Local<v8::Value> data);

// Initialize the global event dispatcher function (__NS_DISPATCH_HOT_EVENT__)
// This exposes a JavaScript-callable function that the HMR client can use to dispatch events
void InitializeHotEventDispatcher(v8::Isolate* isolate, v8::Local<v8::Context> context);

// Drain and execute `import.meta.hot.dispose(cb)` callbacks for the given module
// keys. If `keys` is empty, drains every registered callback across every module
// (the right behaviour for whole-app HMR reboots like Angular's
// `__reboot_ng_modules__`, where the entire JS realm's side effects are being
// thrown away). Each callback is invoked with that module's `hot.data` object so
// users can persist state across the reload (matches Vite spec).
//
// Callbacks are removed from the registry after execution so a second drain in
// the same cycle is a clean no-op. Per-callback failures are logged (when
// script-loading logs are enabled) but never propagate — one bad disposer must
// not break the HMR cycle for everyone else.
//
// Returns the number of callbacks successfully executed.
int RunHotDisposeCallbacks(v8::Isolate* isolate, v8::Local<v8::Context> context,
                           const std::vector<std::string>& keys);

// Initialize the global `__nsRunHmrDispose([keys?])` function so the HMR client
// (e.g. @nativescript/vite's Angular HMR client) can drain dispose callbacks
// from JS. Mirrors the `InitializeHotEventDispatcher` pattern. Should be called
// once per main isolate during runtime init, gated on dev mode.
//
// JS signature: `__nsRunHmrDispose(keys?: string[]) => number`
//   - `keys` omitted / null / undefined / empty array → drain everything.
//   - `keys` non-empty → drain only the listed module keys.
//   - Returns: count of callbacks executed.
void InitializeHotDisposeRunner(v8::Isolate* isolate, v8::Local<v8::Context> context);

// Drain `import.meta.hot.prune(cb)` callbacks for the given module keys (or
// every registered module if `keys` is empty). Same snapshot/swap semantics as
// `RunHotDisposeCallbacks` — callbacks fire exactly once per drain, the
// registry is cleared atomically per key, and per-callback failures are logged
// but never propagate.
//
// Returns the number of callbacks successfully executed.
int RunHotPruneCallbacks(v8::Isolate* isolate, v8::Local<v8::Context> context,
                         const std::vector<std::string>& keys);

// Initialize the global `__nsRunHmrPrune([keys?])` function. Symmetric with
// `__nsRunHmrDispose` but for `prune` callbacks.
//
// JS signature: `__nsRunHmrPrune(keys?: string[]) => number`
void InitializeHotPruneRunner(v8::Isolate* isolate, v8::Local<v8::Context> context);

// `decline()` support. When user code calls `import.meta.hot.decline()`, the
// module's canonical key is added to a process-wide declined set. The HMR
// client checks `IsAnyModuleDeclined(updatedKeys)` before applying an update —
// if any updated key is declined, the update is converted into a full reload
// (matches Vite spec: "If the module triggers HMR, full reload occurs").
void MarkHotDeclined(const std::string& key);

// Returns true if the given key is in the declined set. Used by the
// `__nsHasDeclinedModule` JS helper below.
bool IsHotDeclined(const std::string& key);

// Returns true if ANY of the supplied keys are in the declined set, OR if
// the declined set is non-empty AND `keys` is empty (caller is asking
// "is anything declined at all?"). The runtime canonicalizes its registry
// keys via `canonicalHotKey` (strips fragments, normalizes script extensions,
// rewrites NS HMR virtual prefixes); the HMR client should pass canonical
// URLs straight from `evictPaths` for accurate matching.
bool IsAnyModuleDeclined(const std::vector<std::string>& keys);

// Initialize the global `__nsHasDeclinedModule([keys?])` function. Returns
// `true` if any of the listed keys is declined (or if the declined set is
// non-empty AND no keys were passed). The Angular HMR client calls this with
// `evictPaths` before reboot; on `true` it falls back to `__nsReloadDevApp()`.
//
// JS signature: `__nsHasDeclinedModule(keys?: string[]) => boolean`
void InitializeHotDeclinedHelper(v8::Isolate* isolate, v8::Local<v8::Context> context);

// ─────────────────────────────────────────────────────────────
// Small v8 utility helpers (shared between Runtime.cpp and HMRSupport.cpp).
// Declared here once so both translation units share a single definition.

// Read an optional string property from `object` into `*out`. Returns false
// if the property is missing, null, undefined, or non-convertible.
bool GetOptionalStringProperty(v8::Isolate* isolate, v8::Local<v8::Context> context,
                               v8::Local<v8::Object> object, const char* key,
                               std::string* out);

// Construct an already-resolved Promise<undefined>.
v8::Local<v8::Promise> CreateResolvedPromise(v8::Isolate* isolate,
                                             v8::Local<v8::Context> context);

// Construct an already-rejected Promise with the given reason.
v8::Local<v8::Promise> CreateRejectedPromise(v8::Local<v8::Context> context,
                                             v8::Local<v8::Value> reason);

// Mirror a globally-installed function onto `globalThis.<name>` so legacy
// `globalThis.__nsXxx(...)` callers keep working when the runtime installs
// the canonical function on the realm's global object via FunctionTemplate.
void MirrorFunctionOnGlobalThis(v8::Isolate* isolate, v8::Local<v8::Context> context,
                                const char* name);

// ─────────────────────────────────────────────────────────────
// HMR + dev-session global installer
//
// Installs every JS-callable global the @nativescript/vite HMR client and the
// dev-session bootstrap depend on. Idempotent per realm; safe to call from any
// place that has a fresh context + isolate scope.
//
// JS globals installed (all on the realm's global object AND mirrored on
// globalThis):
//   - __nsConfigureDevRuntime / __nsConfigureRuntime  (import map + volatile patterns)
//   - __nsSupportsRuntimeConfigUrl                    (data property, true)
//   - __nsStartDevSession                             (async session bootstrap)
//   - __nsInvalidateModules                           (registry eviction)
//   - __nsKickstartHmrPrefetch                        (parallel HTTP prewarm)
//   - __nsReloadDevApp                                (re-import session entry)
//   - __nsApplyStyleUpdate                            (CSS HMR apply)
//   - __nsGetLoadedModuleUrls                         (registry introspection)
//   - (debug only) __NS_DISPATCH_HOT_EVENT__,
//     __nsRunHmrDispose, __nsRunHmrPrune,
//     __nsHasDeclinedModule
void InitializeHmrDevGlobals(v8::Isolate* isolate, v8::Local<v8::Context> context);

} // namespace tns

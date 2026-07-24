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
}

namespace tns {

// HMRSupport: the native half of the NativeScript dev-loader contract.
//
// The runtime deliberately exposes *mechanism* only:
//   - the synchronous HTTP text fetch backing the HTTP ESM loader
//     (V8's ResolveModuleCallback is synchronous, so the fetch must be
//     native),
//   - a body prewarm cache + list-mode kickstart so a server-computed
//     module closure can be fetched in one parallel wave before V8's
//     serial synchronous walk,
//   - eviction plumbing (prefetch-cache evict + an eviction-driven
//     fetch nonce that defeats any HTTP cache layer between the
//     runtime and the dev server),
//   - the dev-boot-complete signal that disarms cold-boot-only
//     behaviors (host yield pump, kickstart pump-wait).
//
// Everything else — boot orchestration, `import.meta.hot`, hot-callback
// registries, full reload, CSS apply, WebSocket protocol — is HMR
// *policy* and lives in the JS dev client (`@nativescript/vite`).

// ─────────────────────────────────────────────────────────────
// HTTP loader helpers (used by dev/HMR and general-purpose HTTP module loading)
//
// Normalize an HTTP(S) URL into a stable module registry/cache key.
// - Always strips URL fragments.
// - For NativeScript dev endpoints, drops known cache busters (t/v/import)
//   and sorts remaining query params for stability.
// - For non-dev/public URLs, preserves the full query string as part of the
//   cache key.
// Module identity IS the (canonical) URL — the dev server serves every
// module under exactly one URL and never varies it for freshness.
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
// On a fast path, returns from the in-memory kickstart-prewarm cache
// without touching the network (destructive one-shot read). On the slow
// path, performs a synchronous JNI fetch with a bounded retry on
// transient socket-pool failures.
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

// Drop all entries in the prewarm cache. Safe to call from any thread.
// Used by Runtime teardown and by HMR cache-poison scenarios where the
// dev server has indicated a graph version bump.
void ClearHttpModulePrefetchCache();

// Register a "yield" callback that `HttpFetchText` should invoke around its
// synchronous network turn so the caller can pump its own runloop (e.g. the
// JS-thread runloop so a placeholder UI can repaint during cold-boot).
//
// Default: a no-op. Android's main NativeScript isolate runs JS on the UI
// thread, so there is no separate JS-thread runloop to pump here; a host
// that drives its own loop can install a real pump (e.g. one calling
// ALooper_pollOnce(0)) via this hook.
//
// Pass `nullptr` to disable any yielding (used by hosts that drive their own
// run loop or by tests that want bit-for-bit deterministic fetch timing).
// Safe to call from any thread; reads use acquire/release ordering.
void RegisterHttpFetchYield(void (*callback)());

// Drop a specific URL set from the prewarm cache. Safe to call from any
// thread; missing keys are silently ignored. Used by `InvalidateModules`
// so that an HMR eviction also purges any stale HTTP body a previous
// kickstart wave left behind. Without this, the kickstart's cache plus
// `HttpFetchText`'s destructive-read fast path would happily serve V8 a
// stale body from the prior save — visible to the user as a 1-cycle lag
// between save and visual update.
void EvictHttpModulePrefetchCacheUrls(const std::vector<std::string>& urls);

// Mark a URL set (canonicalized internally) so that the NEXT network
// fetch of each URL carries a unique `__ns_dev_nonce` query parameter,
// guaranteeing no HTTP cache layer between the runtime and the dev
// server (OS URL caches, proxies, a host-installed HttpResponseCache)
// can satisfy the request. Called by `InvalidateModules` for the
// eviction set; marks are consumed when a fresh body arrives.
// The nonce is transport-only and never affects module identity.
void MarkUrlsForCacheBust(const std::vector<std::string>& urls);

// List-mode kickstart prewarm. Fetches ONLY the explicit URL list it
// was given (no body scanning, no graph recursion — the dev server owns
// the module graph and supplies closures: `evictPaths` for HMR, an
// entry-graph crawl for cold boot). Fetches run in parallel (up to
// `maxConcurrent`), each body landing in the prewarm cache that
// `HttpFetchText` reads. Blocks the calling thread until the wave
// drains or `timeoutSeconds` elapses.
//
// By feeding the precomputed list we turn N sequential
// `LoadHttpModuleForUrl` calls (the importer chain during V8's
// ResolveModuleCallback walk) into a single parallel wave that
// completes before V8 starts walking.
//
// Cleared/blocked URLs are filtered up front; partial success is
// reported as success (the V8 walk falls back to per-module
// HttpFetchText for anything we couldn't pre-fill).
//
// `outFetchedCount` (optional) receives the number of distinct URLs
// fetched. `outElapsedMs` (optional) receives wall-clock time.
bool KickstartHmrPrefetchUrlsSync(const std::vector<std::string>& urls,
                                  int maxConcurrent,
                                  double timeoutSeconds,
                                  size_t* outFetchedCount,
                                  uint64_t* outElapsedMs);

// Flip the dev-boot-complete signal: sets the JS-visible
// `__NS_HMR_BOOT_COMPLETE__` global and the native atomic that gates the
// cold-boot-only behaviors (kickstart pump-wait yield cadence). Exposed
// to JS as `__NS_DEV__.setDevBootComplete(value?: boolean)`.
void SetDevBootComplete(v8::Isolate* isolate, v8::Local<v8::Context> context,
                        bool value);

// Clear process-wide dev-loader state (prewarm cache, cache-bust marks,
// boot-complete flag). MUST be called during Runtime teardown before
// isolate disposal — and only for the MAIN isolate (worker teardown must
// not wipe shared state the main isolate still uses).
void CleanupHMRGlobals();

// Mirror a globally-installed value onto `globalThis.<name>` so
// `globalThis.<name>` lookups resolve when the runtime installs the
// canonical value on the realm's global object.
void MirrorGlobalOnGlobalThis(v8::Isolate* isolate, v8::Local<v8::Context> context,
                              const char* name);

// ─────────────────────────────────────────────────────────────
// Dev host namespace installer
//
// Installs the single `__NS_DEV__` namespace object that carries every
// JS-callable dev primitive that any tooling can depend on.
// Idempotent per realm; safe to call from any place that has a fresh
// context + isolate scope. Installed on the realm's global object AND
// mirrored on globalThis.
//
// `__NS_DEV__` members:
//   - configureRuntime(config)        (import map + volatile patterns)
//   - invalidateModules(urls)         (registry + cache eviction)
//   - kickstartPrefetch(urls, opts?)  (parallel HTTP prewarm, list mode)
//   - seedModuleBodies(entries)       (batch prewarm seeding from the boot archive)
//   - getLoadedModuleUrls()           (registry introspection)
//   - setDevBootComplete(value?)      (boot-complete signal)
//   - terminateAllWorkers()           (main isolate only; see CallbackHandlers.h)
//   - canonicalizeHttpUrlKey(url)     (debug builds only; test diagnostic)
void InitializeHmrDevGlobals(v8::Isolate* isolate, v8::Local<v8::Context> context,
                             bool isWorker);

} // namespace tns

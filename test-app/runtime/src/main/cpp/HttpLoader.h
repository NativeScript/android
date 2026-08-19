#pragma once

#include <functional>
#include <string>
#include <vector>

// Forward declare v8 types to keep this header lightweight and avoid
// requiring V8 headers at include sites.
namespace v8 {
class Isolate;
template <class T>
class Local;
class Object;
class Function;
class Context;
class Value;
}  // namespace v8

namespace tns {

// HttpLoader: the native half of the NativeScript HTTP module-loader
// contract.
//
// The runtime deliberately exposes *mechanism* only:
//   - the synchronous HTTP text fetch backing the HTTP ESM loader's
//     fallback path (V8's ResolveModuleCallback is synchronous — still
//     true as of 14.9.207.39 — so the fallback must be native),
//   - the async background-thread fetch behind the phase-1 module-graph
//     walk (StartModuleGraphLoad), which is how module bodies
//     normally arrive,
//   - eviction plumbing (an eviction-driven fetch nonce that defeats
//     any HTTP cache layer between the runtime and the origin),
//   - the boot-evaluation flag that arms the cold-boot looper pump only
//     while an entry module is evaluating (derived by the runtime itself),
//   - the remote-module security gate, seeded once from nativescript.config
//     at boot and never exposed on ns:runtime getConfig/setConfig.

// ─────────────────────────────────────────────────────────────
// HTTP loader helpers (used by dev/HMR and general-purpose HTTP module loading)
//
// The canonical-key *mechanism* (fragment strip, cache-buster param drop,
// param sort) must be native because it keys the module registry inside V8's
// synchronous resolve walk. The *vocabulary* — which query params are pure
// cache busters, which path prefixes identify dev endpoints whose queries may
// be normalized, and which paths must keep their query verbatim because the
// query IS the identity — is server/framework policy, supplied by the dev
// client via ns:module `configureLoader({ canonicalization: {...} })`. It is
// per-isolate loader vocabulary — installed through SetCanonicalizationConfig
// in ModuleInternalCallbacks.h — so CanonicalizeHttpUrlKey runs on the
// isolate's own thread only. The transport never canonicalizes; it carries
// keys computed for it.
//
// When unconfigured, canonicalization is purely mechanical (fragment strip).
struct CanonicalizationConfig {
    std::vector<std::string> stripParams;            // query param names to drop
    std::vector<std::string> devPathPrefixes;        // StartsWith → normalize query
    std::vector<std::string> preserveQueryPrefixes;  // contains → keep query
};

// Normalize an HTTP(S) URL into a stable module registry/cache key.
// - Always strips URL fragments.
// - For NativeScript dev endpoints, drops known cache busters (t/v/import)
//   and sorts remaining query params for stability.
// - For non-dev/public URLs, preserves the full query string as part of the
//   cache key.
// Module identity IS the (canonical) URL — the dev server serves every
// module under exactly one URL and never varies it for freshness.
std::string CanonicalizeHttpUrlKey(const std::string& url);

// What a module response turned out to be. Decided once, by the shared
// classifier, for whichever transport produced the response.
enum class ModuleResponseKind {
    kJavaScript,
    kJson,
};

// The outcome of fetching one module over HTTP. Both transports produce this
// same verdict, so the synchronous fallback and the async graph walk cannot
// drift apart on what counts as a usable module.
struct ModuleFetchResult {
    bool ok = false;
    int status = 0;
    ModuleResponseKind kind = ModuleResponseKind::kJavaScript;
    // Normalized: an empty 2xx JavaScript body becomes the canonical empty
    // module. Meaningful only when `ok`.
    std::string body;
    std::string contentType;  // as received, parameters included
    // Reader-facing explanation, non-empty exactly when `!ok`. This is the text
    // that reaches the importer's rejection, so it names the URL and the cause.
    std::string failureReason;
};

// Synchronous module fetch with one retry on transport error — the fallback
// path for anything the module-graph walk missed. Blocks the calling thread.
// Returns `result.ok`.
bool HttpFetchModule(const std::string& url, ModuleFetchResult& result);

// Asynchronous single-URL module fetch — the I/O primitive behind the
// module-graph walk (see StartModuleGraphLoad in ModuleInternalCallbacks.h).
// Same response policy as HttpFetchModule, minus the JS-thread block:
//   - security gate (IsRemoteUrlAllowed) checked up front,
//   - a JNI HttpURLConnection GET on a background thread with the same
//     request shape as the sync path (cache-bust nonce, zero-cache headers,
//     no cookies) and one retry on transport error.
// `completion(result)` is invoked exactly once, on an arbitrary thread —
// callers must hop to their JS thread before touching V8.
void FetchModuleBodyAsync(
    const std::string& url,
    std::function<void(ModuleFetchResult result)> completion);

// Return the most recent low-level fetch error reason for the calling
// thread, or an empty string if the last fetch succeeded (or no fetch
// has run on this thread yet). Take semantics — the slot is cleared on
// read. Android-only diagnostic for splicing JNI exceptions into JS
// errors when the transport never reached an HTTP status.
std::string TakeLastHttpFetchErrorReason();

// Register a "yield" callback that `HttpFetchModule` should invoke around its
// synchronous network turn so the caller can pump its own runloop (e.g. the
// JS-thread looper so a placeholder UI can repaint during cold-boot).
//
// Default: a built-in pump that no-ops outside the JS thread / after the
// dev boot completes (see `MaybePumpJSThreadDuringBoot` in HttpLoader.cpp).
//
// Pass `nullptr` to disable any yielding (used by hosts that drive their own
// run loop or by tests that want bit-for-bit deterministic fetch timing).
// Safe to call from any thread; reads use acquire/release ordering.
void RegisterHttpFetchYield(void (*callback)());

// Mark a URL set (canonicalized internally) so that the NEXT network
// fetch of each URL carries a unique `__ns_dev_nonce` query parameter,
// guaranteeing no HTTP cache layer between the runtime and the origin
// can satisfy the request. Called by `InvalidateModules` for the
// eviction set; marks are consumed when a fresh body arrives.
// The nonce is transport-only and never affects module identity.
void MarkUrlsForCacheBust(const std::vector<std::string>& urls);

// Arm/disarm this thread's boot-evaluation window: while nonzero, the yield
// inside synchronous HTTP fetches may pump the JS thread's looper (safe only
// while the entry module is evaluating — nothing else owns the looper yet).
// Balanced RAII-style by ModuleInternal::Load.
void SetBootEvaluationActive(bool active);

// Clear the transport's process-wide state (cache-bust marks). MUST be
// called inside Runtime::DestroyRuntime() before isolate
// disposal — and only for the MAIN isolate (worker teardown must not wipe
// shared state the main isolate still uses).
void CleanupHttpLoaderGlobals();

// ─────────────────────────────────────────────────────────────
// Remote-module security gate
//
// Seeded once from nativescript.config / package.json (`security.allowRemoteModules`,
// `security.remoteModuleAllowlist`) the first time a fetch is gated. Debug
// apps always allow. These values are not readable or writable through
// ns:runtime getConfig/setConfig — only nativescript.config at boot.

// In debug mode (Runtime.isDebuggable()): always returns true.
// Otherwise returns the boot-time `security.allowRemoteModules` value.
bool IsRemoteModulesAllowed();

// Whether `url` may be fetched as a remote ES module. Debug apps always
// allow. Production requires allowRemoteModules, then an allowlist match
// (or all URLs if the allowlist is empty).
bool IsRemoteUrlAllowed(const std::string& url);

// Mirrors com.tns.Runtime.isDebuggable(), cached once via the security
// config init. Fail-safe false until initialized.
bool IsDebuggable();

// ─────────────────────────────────────────────────────────────
// The `ns:module` builtin binding
//
// Populates the native half of the `ns:module` builtin module — the one
// namespace carrying every JS-callable dev primitive that any tooling can
// depend on. Called from NsBuiltinModules::BuildBinding the first time a
// realm resolves `ns:module` (via require, static import, or import());
// ns-module.js shapes and freezes the exports.
//
// `ns:module` members:
//   - configureLoader(config)         (import map + volatile patterns +
//                                      canonicalization vocabulary)
//   - invalidateModules(urls)         (registry + cache eviction)
//   - getLoadedModuleUrls()           (registry introspection)
//   - canonicalizeHttpUrlKey(url)     (debug builds only; test diagnostic)
//
// Worker teardown across HMR cycles is userland: the dev client intercepts
// the global `Worker` constructor and terminates tracked instances
// (worker.terminate() cascades to nested workers via Runtime::DestroyRuntime).
//
// Returns false (with an exception pending or a failed Set) when the
// binding could not be populated.
bool BuildNsModuleBinding(v8::Local<v8::Context> context,
                          v8::Local<v8::Object> binding);

}  // namespace tns

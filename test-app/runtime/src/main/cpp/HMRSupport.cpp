// HMRSupport.cpp
//
// The native half of the NativeScript dev-loader contract. The runtime
// exposes *mechanism* only (sync HTTP module fetch, prewarm cache +
// list-mode kickstart, eviction plumbing, dev-boot-complete signal),
// consolidated under the single `__NS_DEV__` namespace object. All HMR
// *policy* — boot orchestration, `import.meta.hot`, hot-callback
// registries, full reload, CSS apply, WebSocket protocol — lives in the
// JS dev client (`@nativescript/vite`).
#include "HMRSupport.h"

#include "ArgConverter.h"
#include "CallbackHandlers.h"
#include "DevFlags.h"
#include "JEnv.h"
#include "ModuleInternalCallbacks.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <jni.h>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tns {

// ──────────────────────────────────────────────────────────────────────────
// Local v8 string helper: thin convenience wrapper so call sites can read
// more compactly.
static inline v8::Local<v8::String> ToV8String(v8::Isolate* isolate, const char* str) {
  if (str == nullptr) {
    return v8::String::Empty(isolate);
  }
  return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal).ToLocalChecked();
}

static inline bool StartsWith(const std::string& s, const char* prefix) {
  size_t n = strlen(prefix);
  return s.size() >= n && s.compare(0, n, prefix) == 0;
}

static inline bool EndsWith(const std::string& s, const char* suffix) {
  size_t n = strlen(suffix);
  return s.size() >= n && s.compare(s.size() - n, n, suffix) == 0;
}

void MirrorGlobalOnGlobalThis(v8::Isolate* isolate, v8::Local<v8::Context> context,
                              const char* name) {
  std::string src =
      "if (typeof globalThis !== 'undefined' && typeof globalThis." +
      std::string(name) +
      " === 'undefined') {"
      "  Object.defineProperty(globalThis, '" + std::string(name) +
      "', { value: this." + std::string(name) +
      ", writable: true, configurable: true, enumerable: false });"
      "}";

  v8::Local<v8::Script> script;
  if (v8::Script::Compile(context, ToV8String(isolate, src.c_str()))
          .ToLocal(&script)) {
    script->Run(context).FromMaybe(v8::Local<v8::Value>());
  }
}

static void SetBooleanGlobal(v8::Isolate* isolate, v8::Local<v8::Context> context,
                             const char* key, bool value) {
  context->Global()
      ->Set(context, ToV8String(isolate, key), v8::Boolean::New(isolate, value))
      .FromMaybe(false);
}

// ─────────────────────────────────────────────────────────────
// Dev-boot completion flag
//
// Native-side mirror of `__NS_HMR_BOOT_COMPLETE__`. Read by the kickstart
// pump-wait so its gate is a single relaxed atomic load on the HMR-time
// hot path. The JS dev client flips this via
// `__NS_DEV__.setDevBootComplete(bool)` once the real app root view
// commits; boot orchestration itself is entirely userland.
static std::atomic<bool> g_devSessionBootComplete{false};

static inline bool IsDevSessionBootComplete() {
  return g_devSessionBootComplete.load(std::memory_order_relaxed);
}

void SetDevBootComplete(v8::Isolate* isolate, v8::Local<v8::Context> context,
                        bool value) {
  SetBooleanGlobal(isolate, context, "__NS_HMR_BOOT_COMPLETE__", value);
  g_devSessionBootComplete.store(value, std::memory_order_relaxed);
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dev-boot] __NS_HMR_BOOT_COMPLETE__=%s", value ? "true" : "false");
  }
}

// ─────────────────────────────────────────────────────────────
// HTTP loader helpers

std::string CanonicalizeHttpUrlKey(const std::string& url) {
  // Some loaders wrap HTTP module URLs as file://http(s)://...
  std::string normalizedUrl = url;
  if (StartsWith(normalizedUrl, "file://http://") || StartsWith(normalizedUrl, "file://https://")) {
    normalizedUrl = normalizedUrl.substr(strlen("file://"));
  }
  if (!(StartsWith(normalizedUrl, "http://") || StartsWith(normalizedUrl, "https://"))) {
    return normalizedUrl;
  }
  // Drop fragment entirely
  size_t hashPos = normalizedUrl.find('#');
  std::string noHash = (hashPos == std::string::npos) ? normalizedUrl : normalizedUrl.substr(0, hashPos);

  // Locate path start and query start
  size_t schemePos = noHash.find("://");
  if (schemePos == std::string::npos) {
    // Unexpected shape; fall back to removing whole query
    size_t q = noHash.find('?');
    return (q == std::string::npos) ? noHash : noHash.substr(0, q);
  }
  size_t pathStart = noHash.find('/', schemePos + 3);
  if (pathStart == std::string::npos) {
    // No path; nothing to normalize
    return noHash;
  }
  size_t qPos = noHash.find('?', pathStart);
  std::string originAndPath = (qPos == std::string::npos) ? noHash : noHash.substr(0, qPos);
  std::string query = (qPos == std::string::npos) ? std::string() : noHash.substr(qPos + 1);

  // IMPORTANT: This function is used as an HTTP module registry/cache key.
  // For general-purpose HTTP module loading (public internet), the query string
  // can be part of the module's identity (auth, content versioning, routing, etc).
  // Therefore we only apply query normalization (sorting/dropping) for known
  // NativeScript dev endpoints where `t`/`v`/`import` are purely cache busters.
  //
  // The dev server serves every module under ONE canonical URL — module
  // identity IS the URL string. Freshness after an HMR edit is handled by
  // `__NS_DEV__.invalidateModules` (registry + prefetch-cache evict) plus the
  // eviction-driven fetch nonce in `PerformHttpFetchOnceSync`, never by URL
  // variation. There is deliberately no path-tag vocabulary to collapse here.
  //
  // Special cases that LOOK like dev endpoints but aren't normalized:
  //
  //   `/@ng/component` (Angular HMR component-update endpoint)
  //     The `t` (timestamp) parameter is the WHOLE POINT of the URL — it
  //     identifies a specific recompile of the component's metadata after
  //     a `.html`/style edit. Stripping it would collapse every HMR fetch
  //     to the same cache key (the boot-time call uses `Date.now()` and
  //     each subsequent save uses a new `Date.now()`), and the second
  //     `__ns_import(...)` would hit V8's module cache, resolve the
  //     boot-time `_UpdateMetadata` default export, and call
  //     `ɵɵreplaceMetadata` with stale instructions. Result: server logs
  //     `(client) hmr update`, the listener fires, but the visual never
  //     changes because the runtime swapped the live view's metadata
  //     with the same metadata it already had. Treat the path as a
  //     non-dev endpoint and preserve the query verbatim so each
  //     timestamped fetch is a distinct registry entry.
  //
  // Apply the special-case check BEFORE the dev-endpoint short-circuit so
  // it covers paths under `/ns/m/<componentDir>/@ng/component` (the
  // resolved URL Angular's compiler produces relative to the component's
  // `import.meta.url`).
  {
    std::string pathOnly = originAndPath.substr(pathStart);
    if (pathOnly.find("/@ng/component") != std::string::npos) {
      // Preserve query as-is — `t` is the version discriminator.
      return noHash;
    }
    const bool isDevEndpoint =
      StartsWith(pathOnly, "/ns/") ||
      StartsWith(pathOnly, "/node_modules/.vite/") ||
      StartsWith(pathOnly, "/@id/") ||
      StartsWith(pathOnly, "/@fs/");
    if (!isDevEndpoint) {
      // Preserve query as-is (fragment already removed).
      return noHash;
    }
  }

  if (query.empty()) return originAndPath;

  // Keep all params except typical import markers or t/v cache busters; sort for stability.
  std::vector<std::string> kept;
  size_t start = 0;
  while (start <= query.size()) {
    size_t amp = query.find('&', start);
    std::string pair = (amp == std::string::npos) ? query.substr(start) : query.substr(start, amp - start);
    if (!pair.empty()) {
      size_t eq = pair.find('=');
      std::string name = (eq == std::string::npos) ? pair : pair.substr(0, eq);
      // Drop import marker and common cache-busting stamps.
      if (!(name == "import" || name == "t" || name == "v")) kept.push_back(pair);
    }
    if (amp == std::string::npos) break;
    start = amp + 1;
  }
  if (kept.empty()) return originAndPath;
  std::sort(kept.begin(), kept.end());
  std::string rebuilt = originAndPath + "?";
  for (size_t i = 0; i < kept.size(); i++) {
    if (i > 0) rebuilt += "&";
    rebuilt += kept[i];
  }
  return rebuilt;
}

// Resolve a relative/root-absolute import specifier against a parent URL
// using plain string manipulation. Only relative (`./`, `../`) and
// root-absolute (`/`) specifiers are resolved here; bare specifiers and
// already-absolute URLs fall through unchanged. Exposed via
// `HMRSupport.h` so `ModuleInternalCallbacks.cpp` can share this single
// resolver instead of carrying its own copy.
std::string ResolveImportSpecifierAgainstUrl(const std::string& specifier,
                                              const std::string& parentUrl) {
  if (specifier.empty()) return "";
  // Already absolute.
  if (StartsWith(specifier, "http://") || StartsWith(specifier, "https://")) {
    return specifier;
  }
  bool isRelative = StartsWith(specifier, "./") || StartsWith(specifier, "../");
  bool isRootAbs = !specifier.empty() && specifier[0] == '/';
  if (!isRelative && !isRootAbs) return "";

  if (!(StartsWith(parentUrl, "http://") || StartsWith(parentUrl, "https://"))) {
    return "";
  }
  // Drop fragment + query from parent.
  std::string base = parentUrl;
  size_t hp = base.find('#'); if (hp != std::string::npos) base = base.substr(0, hp);
  size_t qp = base.find('?'); if (qp != std::string::npos) base = base.substr(0, qp);

  size_t schemePos = base.find("://");
  if (schemePos == std::string::npos) return "";
  size_t pathStart = base.find('/', schemePos + 3);
  std::string origin = (pathStart == std::string::npos) ? base : base.substr(0, pathStart);
  std::string path = (pathStart == std::string::npos) ? std::string("/") : base.substr(pathStart);

  std::string specPath = specifier;
  std::string suffix;
  size_t specQ = specPath.find('?');
  size_t specH = specPath.find('#');
  size_t cut = std::string::npos;
  if (specQ != std::string::npos && specH != std::string::npos) cut = std::min(specQ, specH);
  else if (specQ != std::string::npos) cut = specQ;
  else if (specH != std::string::npos) cut = specH;
  if (cut != std::string::npos) { suffix = specPath.substr(cut); specPath = specPath.substr(0, cut); }

  std::string newPath;
  if (isRootAbs) {
    newPath = specPath;
  } else {
    size_t lastSlash = path.find_last_of('/');
    std::string baseDir = (lastSlash == std::string::npos) ? std::string("/") : path.substr(0, lastSlash + 1);
    newPath = baseDir + specPath;
  }
  // Normalize `.` and `..` segments.
  std::vector<std::string> stack;
  bool absolute = !newPath.empty() && newPath[0] == '/';
  size_t i = 0;
  while (i <= newPath.size()) {
    size_t j = newPath.find('/', i);
    std::string seg = (j == std::string::npos) ? newPath.substr(i) : newPath.substr(i, j - i);
    if (seg.empty() || seg == ".") {
      // skip
    } else if (seg == "..") {
      if (!stack.empty()) stack.pop_back();
    } else {
      stack.push_back(seg);
    }
    if (j == std::string::npos) break;
    i = j + 1;
  }
  std::string norm = absolute ? "/" : std::string();
  for (size_t k = 0; k < stack.size(); ++k) {
    if (k > 0) norm += "/";
    norm += stack[k];
  }
  return origin + norm + suffix;
}

// ─────────────────────────────────────────────────────────────
// Eviction-driven fetch cache-bust
//
// When the HMR client invalidates a module, the NEXT network fetch of
// that module must not be satisfiable by any HTTP cache layer between
// the runtime and the dev server (OS URL caches, proxies, a
// host-installed HttpResponseCache). `InvalidateModules` marks the
// canonical keys of the eviction set here; `PerformHttpFetchOnceSync`
// then appends a unique `__ns_dev_nonce` query parameter to the
// wire-level request for any marked URL, guaranteeing the cache sees
// a URL it has never stored. The nonce is transport-only — it never
// enters the module registry key (identity stays the canonical URL),
// and the server and the registry never see a varied URL.
static std::mutex g_bustNextFetchMutex;
static std::unordered_set<std::string> g_bustNextFetchKeys;

void MarkUrlsForCacheBust(const std::vector<std::string>& urls) {
  if (urls.empty()) return;
  std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
  for (const auto& url : urls) {
    if (url.empty()) continue;
    if (!(StartsWith(url, "http://") || StartsWith(url, "https://"))) continue;
    g_bustNextFetchKeys.insert(CanonicalizeHttpUrlKey(url));
  }
}

// Peek (do not consume) — the fetch may be retried on transient failure
// and the retry must still carry a nonce. Cleared on fetch success.
static bool IsUrlMarkedForCacheBust(const std::string& url) {
  std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
  if (g_bustNextFetchKeys.empty()) return false;
  return g_bustNextFetchKeys.find(CanonicalizeHttpUrlKey(url)) != g_bustNextFetchKeys.end();
}

static void ClearCacheBustForUrl(const std::string& url) {
  std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
  if (g_bustNextFetchKeys.empty()) return;
  g_bustNextFetchKeys.erase(CanonicalizeHttpUrlKey(url));
}

static void ClearAllCacheBustMarks() {
  std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
  g_bustNextFetchKeys.clear();
}

// ============================================================================
// HTTP body cache + parallel kickstart prewarm
// ============================================================================
//
// V8 only exposes a synchronous ResolveModuleCallback for static imports.
// Each call into HttpFetchText() blocks the JS thread on a synchronous
// network turn, which forces serial fetching from the JS thread's
// perspective.
//
// `__NS_DEV__.kickstartPrefetch(urls)` lets the JS dev client hand the
// runtime a server-computed module closure (cold-boot graph or HMR
// eviction set) to fetch in one parallel wave BEFORE V8 walks the import
// graph. Bodies land in `g_prefetchCache` keyed by full URL; the
// always-on cache read in `HttpFetchText` then serves V8's synchronous
// walk at memory speed.
//
// The runtime performs NO import scanning and NO speculative graph
// discovery of its own — the server owns the module graph and supplies
// explicit URL lists.
//
// Correctness invariants:
//   1. Cache reads consume (one-shot). A second HttpFetchText for the
//      same URL after a cache hit triggers a fresh network fetch — this
//      is the right behavior for HMR where re-fetching means we got a
//      newer version of the module.
//   2. Every kickstart fetch goes through IsRemoteUrlAllowed() exactly
//      the same way HttpFetchText does. The security gate is preserved.
//   3. Kickstart overwrites cache entries unconditionally — a body the
//      client explicitly asked to re-fetch is authoritative by
//      construction (the previous entry is stale).

namespace {

std::mutex g_prefetchMutex;
// Heap-allocated (leaky singleton) to prevent V8 crash during
// __cxa_finalize_ranges. See g_moduleRegistry comment in
// ModuleInternalCallbacks.cpp for full rationale.
auto* _g_prefetchCache = new std::unordered_map<std::string, std::string>();
auto& g_prefetchCache = *_g_prefetchCache;

bool LooksLikeJsSourceUrl(const std::string& url) {
  size_t qpos = url.find('?');
  std::string path = (qpos == std::string::npos) ? url : url.substr(0, qpos);
  // Block clearly non-JS content; on cache hit V8 would attempt to compile
  // CSS/images/etc. as ES modules and fail in confusing ways.
  if (tns::EndsWith(path, ".css") || tns::EndsWith(path, ".scss") ||
      tns::EndsWith(path, ".sass") || tns::EndsWith(path, ".less")) return false;
  if (tns::EndsWith(path, ".png") || tns::EndsWith(path, ".jpg") ||
      tns::EndsWith(path, ".jpeg") || tns::EndsWith(path, ".gif") ||
      tns::EndsWith(path, ".svg") || tns::EndsWith(path, ".webp") ||
      tns::EndsWith(path, ".ico")) return false;
  if (tns::EndsWith(path, ".json")) return false;
  if (tns::EndsWith(path, ".html") || tns::EndsWith(path, ".htm")) return false;
  if (tns::EndsWith(path, ".woff") || tns::EndsWith(path, ".woff2") ||
      tns::EndsWith(path, ".ttf") || tns::EndsWith(path, ".otf") ||
      tns::EndsWith(path, ".eot")) return false;
  if (tns::EndsWith(path, ".mp4") || tns::EndsWith(path, ".webm") ||
      tns::EndsWith(path, ".mp3") || tns::EndsWith(path, ".wav")) return false;
  return true;
}

// Pluggable host yield. Default: no-op. Embedders that want a JS-thread
// runloop pump during cold-boot fetches can install one via
// `RegisterHttpFetchYield` (e.g. ALooper_pollOnce(0)).
void NoopHttpFetchYield() {}
std::atomic<void (*)()> g_httpFetchYield{&NoopHttpFetchYield};

inline void InvokeHttpFetchYield() {
  auto cb = g_httpFetchYield.load(std::memory_order_acquire);
  if (cb != nullptr) cb();
}

}  // anonymous namespace

void RegisterHttpFetchYield(void (*callback)()) {
  g_httpFetchYield.store(callback, std::memory_order_release);
}

void ClearHttpModulePrefetchCache() {
  std::lock_guard<std::mutex> lock(g_prefetchMutex);
  g_prefetchCache.clear();
}

// Drop a specific URL set from `g_prefetchCache`. Used by
// `InvalidateModules` so an HMR eviction purges any stale HTTP body
// the previous kickstart wave left behind. See the doc comment in
// HMRSupport.h for the cache-poisoning case this fixes.
void EvictHttpModulePrefetchCacheUrls(const std::vector<std::string>& urls) {
  if (urls.empty()) return;
  std::lock_guard<std::mutex> lock(g_prefetchMutex);
  size_t hits = 0;
  for (const std::string& u : urls) {
    auto it = g_prefetchCache.find(u);
    if (it != g_prefetchCache.end()) { g_prefetchCache.erase(it); ++hits; }
  }
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[prefetch][evict] urls=%lu hits=%lu remaining=%lu",
                (unsigned long)urls.size(), (unsigned long)hits,
                (unsigned long)g_prefetchCache.size());
  }
}

// Thread-local capture of the most recent JNI-level fetch failure
// (e.g. `ConnectException: failed to connect to /10.0.2.2 (port 5173)
// after 15000ms: connect failed: ECONNREFUSED`). Callers that just
// got back `status=0` from `HttpFetchText` can pull this string and
// splice it into the JS error to give users actionable detail rather
// than a generic "Failed to fetch" line.
//
// Thread-local because each V8 isolate / worker has its own JS
// thread, and concurrent fetches would otherwise clobber each
// other's diagnostic.
static thread_local std::string g_lastHttpFetchErrorReason;

static void RecordLastHttpFetchError(const char* stage,
                                     const std::string& excClass,
                                     const std::string& excMsg) {
  // Format is grep-friendly and short enough to splice into a JS
  // Error message without exploding the line length:
  //   stage=get-response-code class=java.net.ConnectException msg=...
  g_lastHttpFetchErrorReason.assign("stage=");
  g_lastHttpFetchErrorReason.append(stage ? stage : "?");
  g_lastHttpFetchErrorReason.append(" class=");
  g_lastHttpFetchErrorReason.append(excClass);
  g_lastHttpFetchErrorReason.append(" msg=");
  g_lastHttpFetchErrorReason.append(excMsg);
}

void ClearLastHttpFetchErrorReason() {
  g_lastHttpFetchErrorReason.clear();
}

std::string TakeLastHttpFetchErrorReason() {
  std::string out = std::move(g_lastHttpFetchErrorReason);
  g_lastHttpFetchErrorReason.clear();
  return out;
}

// Decide whether a captured fetch failure reason looks like the
// transient okhttp / socket-pool class of bug that one retry on a
// fresh connection reliably clears.
//
// The list is deliberately narrow. Hard failures like `ConnectException`,
// `UnknownHostException`, `MalformedURLException`, or the runtime's own
// security gate (`status=403`) are NOT retryable — retrying would only mask
// config bugs (wrong host/port, missing allowlist entry) behind extra latency.
//
// Patterns covered:
//   * `unexpected end of stream` — server closed the socket mid-handshake
//     (Http1xStream.readResponseHeaders).
//   * `Connection reset` / `SocketException` / `EOFException` — same root
//     cause, different surfacing depending on when the RST/FIN landed.
//   * `Software caused connection abort` — Android/Linux variant, seen on
//     emulators under load.
//   * `Stream closed` / `StreamResetException` — HTTP/2 codepath (some reverse
//     proxies upgrade the tunnel to h2).
static bool IsRetryableFetchReason(const std::string& reason) {
  if (reason.find("unexpected end of stream") != std::string::npos) return true;
  if (reason.find("Connection reset") != std::string::npos) return true;
  if (reason.find("Software caused connection abort") != std::string::npos) return true;
  if (reason.find("EOFException") != std::string::npos) return true;
  if (reason.find("SocketException") != std::string::npos) return true;
  if (reason.find("StreamResetException") != std::string::npos) return true;
  if (reason.find("Stream closed") != std::string::npos) return true;
  return false;
}

// Raw JNI fetch — no cache lookup, no allowlist gate. Used by the
// kickstart threads (which already pre-filtered URLs) so the public
// `HttpFetchText` can keep its allowlist-and-cache logic in one place
// without recursing into itself. Returns true on success (2xx,
// non-empty body).
static bool PerformHttpFetchOnceSync(const std::string& url,
                                     std::string& out,
                                     std::string& contentType,
                                     int& status);

// If a Java exception is pending, drain it into `outClassName` /
// `outMessage` and clear it so subsequent JNI calls don't ABORT the
// process. Returns true when an exception was actually present.
//
// We grab both the simple class name (e.g. `ConnectException`) and
// the `toString()` payload because the latter often includes the
// underlying OS errno (`Connection refused`, `Network unreachable`,
// `failed to connect to /10.0.2.2 (port 5173)` etc.) — exactly the
// diagnostic the silent `status=0` symptom hides.
static bool DrainPendingJniException(JEnv& env, std::string& outClassName, std::string& outMessage) {
  outClassName.clear();
  outMessage.clear();
  jthrowable th = env.ExceptionOccurred();
  if (!th) return false;
  env.ExceptionClear();

  jclass clsThrowable = env.GetObjectClass(th);
  if (clsThrowable) {
    jclass clsClass = env.FindClass("java/lang/Class");
    if (clsClass) {
      jmethodID getName = env.GetMethodID(clsClass, "getName", "()Ljava/lang/String;");
      if (getName) {
        jstring jName = static_cast<jstring>(env.CallObjectMethod(clsThrowable, getName));
        env.ExceptionClear();
        if (jName) {
          outClassName = ArgConverter::jstringToString(jName);
        }
      }
    }
    jmethodID toString = env.GetMethodID(clsThrowable, "toString", "()Ljava/lang/String;");
    if (toString) {
      jstring jMsg = static_cast<jstring>(env.CallObjectMethod(th, toString));
      env.ExceptionClear();
      if (jMsg) {
        outMessage = ArgConverter::jstringToString(jMsg);
      }
    }
  }
  env.ExceptionClear();
  return true;
}

// Minimal HTTP fetch using java.net.* via JNI. Returns true on success (2xx) and non-empty body.
// Security: This is the single point of enforcement for remote module loading.
// In debug mode, all URLs are allowed. In production, checks security.allowRemoteModules
// and security.remoteModuleAllowlist from the app config.
bool HttpFetchText(const std::string& url, std::string& out, std::string& contentType, int& status) {
  out.clear();
  contentType.clear();
  status = 0;
  // Start each fetch with a clean diagnostic slot so a successful
  // fetch can't leave a stale reason from a previous failure.
  ClearLastHttpFetchErrorReason();

  // Security gate: check if remote module loading is allowed before any HTTP fetch.
  if (!IsRemoteUrlAllowed(url)) {
    status = 403;
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][security][blocked] %s", url.c_str());
    }
    return false;
  }

  // Cache-read fast path. The JS dev client populates `g_prefetchCache`
  // via `__NS_DEV__.kickstartPrefetch(urls)` right before importing (cold
  // boot) or re-importing (HMR); by the time V8's synchronous walk asks
  // for a module, the body is already here and the walk runs at memory
  // speed instead of network speed.
  //
  // Cache reads are one-shot; consuming the entry guarantees that a
  // re-fetch (e.g. after HMR) goes back to the network for fresh source.
  {
    std::string cached;
    bool cacheHit = false;
    {
      std::lock_guard<std::mutex> lock(g_prefetchMutex);
      auto it = g_prefetchCache.find(url);
      if (it != g_prefetchCache.end()) {
        cached = std::move(it->second);
        g_prefetchCache.erase(it);
        cacheHit = true;
      }
    }
    if (cacheHit) {
      out = std::move(cached);
      contentType = "application/javascript";
      status = 200;
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[http-loader][prefetch][hit] %s (%lu bytes)", url.c_str(), (unsigned long)out.size());
      }
      // Yield to the host between back-to-back cache hits so any
      // installed heartbeat/runloop pump gets a turn.
      InvokeHttpFetchYield();
      return true;
    }
  }

  // Slow path: synchronous fetch with bounded retry on transient okhttp-class
  // failures. Android's stock HttpURLConnection (backed by okhttp) periodically
  // half-recycles sockets when the server's keep-alive timeout fires before the
  // next request lands; the next use of that socket throws
  // `IOException: unexpected end of stream` from
  // `Http1xStream.readResponseHeaders`. It manifests as random per-request
  // failures across modules on cold boot, and a fresh connection on the next
  // attempt succeeds.
  //
  // `Connection: close` prevents okhttp from pooling our own connection but
  // doesn't help when the server already poisoned the pool from an earlier
  // in-flight fetch. The retry covers both cases, and the system-wide
  // `http.keepAlive=false` set inside `PerformHttpFetchOnceSync` keeps okhttp
  // from pooling in the first place.
  constexpr int kMaxAttempts = 3;
  for (int attempt = 1; attempt <= kMaxAttempts; ++attempt) {
    // Clear the slot at the start of each attempt so a previous
    // attempt's reason can't leak into a later one.
    ClearLastHttpFetchErrorReason();
    if (PerformHttpFetchOnceSync(url, out, contentType, status)) {
      if (attempt > 1 && IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[http-esm][fetch][retry-ok] url=%s attempt=%d", url.c_str(), attempt);
      }
      // Yield to the host after the sync fetch block so any installed
      // pump can repaint before V8 calls us again.
      InvokeHttpFetchYield();
      return true;
    }
    std::string reason = TakeLastHttpFetchErrorReason();
    if (attempt >= kMaxAttempts || !IsRetryableFetchReason(reason)) {
      // Re-stash so the caller sees the same reason we just consumed.
      if (!reason.empty()) {
        RecordLastHttpFetchError("final-attempt", "captured", reason);
      }
      return false;
    }
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][fetch][retry] url=%s attempt=%d/%d reason=%s",
                  url.c_str(), attempt, kMaxAttempts, reason.c_str());
    }
    // Short linear backoff. A stale pooled socket only needs one
    // tick to clear; longer waits would just add cold-boot latency
    // on what's typically dozens of static imports.
    std::this_thread::sleep_for(std::chrono::milliseconds(25 * attempt));
  }
  return false;
}

// True raw HTTP fetch path. Kept separate from HttpFetchText so the
// kickstart (which already filtered URLs and intends to populate the
// cache) doesn't re-check the cache itself.
static bool PerformHttpFetchOnceSync(const std::string& url,
                                     std::string& out,
                                     std::string& contentType,
                                     int& status) {
  out.clear();
  contentType.clear();
  status = 0;
  // Entry trace gated behind `logScriptLoading`. Cold boot fires this dozens of
  // times per session, so it stays off by default; enable `logScriptLoading`
  // when triaging the HTTP-ESM path.
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[http-esm][fetch][enter] url=%s", url.c_str());
  }

  // Eviction-driven cache-bust: if this URL's canonical key was marked
  // by `InvalidateModules` (via `MarkUrlsForCacheBust`), append a
  // unique nonce query parameter so any HTTP cache layer sees a URL it
  // has never stored and must go to origin. The dev server ignores
  // unknown query params on module routes, so the response body is
  // unchanged. First-touch fetches don't need busting — nothing has
  // cached them yet — so unmarked URLs go out verbatim (some Vite
  // virtual routes require exact-match URLs and 404 on unknown query
  // params).
  std::string fetchUrl = url;
  const bool bustRequested = IsUrlMarkedForCacheBust(url);
  if (bustRequested) {
    static std::atomic<uint64_t> s_fetchSeq{0};
    const uint64_t seq = s_fetchSeq.fetch_add(1, std::memory_order_relaxed);
    const uint64_t nowMs = (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    fetchUrl += (url.find('?') == std::string::npos) ? '?' : '&';
    fetchUrl += "__ns_dev_nonce=";
    fetchUrl += std::to_string(nowMs);
    fetchUrl += "-";
    fetchUrl += std::to_string(seq);
  }

  try {
    JEnv env;

    // One-time process-wide kill switch for okhttp's connection
    // pool. Android's stock HttpURLConnection (which okhttp backs)
    // pools sockets across requests, and when Vite's keep-alive
    // timeout fires before our next request we end up reusing a
    // dead socket and hitting
    // `IOException: unexpected end of stream`. Setting
    // `http.keepAlive=false` forces a fresh TCP connection per
    // fetch, which sidesteps the pool entirely.
    //
    // We also set `Connection: close` on the per-request headers
    // below as a belt-and-suspenders signal — the property covers
    // the pool, the header covers the wire. Doing this once via
    // an atomic guard keeps the cost out of the hot path on
    // repeat fetches (this is a synchronous V8 module-loader
    // hot path that can fire dozens of times per cold boot).
    static std::atomic<bool> sKeepAliveDisabled{false};
    if (!sKeepAliveDisabled.exchange(true)) {
      jclass clsSystem = env.FindClass("java/lang/System");
      if (clsSystem) {
        jmethodID setProperty = env.GetStaticMethodID(
            clsSystem, "setProperty",
            "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        if (setProperty) {
          jstring jKey = env.NewStringUTF("http.keepAlive");
          jstring jVal = env.NewStringUTF("false");
          env.CallStaticObjectMethod(clsSystem, setProperty, jKey, jVal);
          // Don't care about the previous value or about exceptions
          // here — `System.setProperty` only throws SecurityException
          // under a SecurityManager and Android apps don't install one.
          env.ExceptionClear();
        }
      }
    }

    // Allow network operations on the current thread (dev-only HMR path)
    // Some Android environments enforce StrictMode which throws NetworkOnMainThreadException
    // when performing network I/O on the main thread. Since this fetch runs on the JS/V8 thread
    // during development, explicitly relax the policy here.
    {
      jclass clsStrict = env.FindClass("android/os/StrictMode");
      jclass clsPolicyBuilder = env.FindClass("android/os/StrictMode$ThreadPolicy$Builder");
      if (clsStrict && clsPolicyBuilder) {
        jmethodID builderCtor = env.GetMethodID(clsPolicyBuilder, "<init>", "()V");
        jobject builder = env.NewObject(clsPolicyBuilder, builderCtor);
        if (builder) {
          jmethodID permitAll = env.GetMethodID(clsPolicyBuilder, "permitAll", "()Landroid/os/StrictMode$ThreadPolicy$Builder;");
          jobject builder2 = permitAll ? env.CallObjectMethod(builder, permitAll) : builder;
          jmethodID build = env.GetMethodID(clsPolicyBuilder, "build", "()Landroid/os/StrictMode$ThreadPolicy;");
          jobject policy = build ? env.CallObjectMethod(builder2 ? builder2 : builder, build) : nullptr;
          if (policy) {
            jmethodID setThreadPolicy = env.GetStaticMethodID(clsStrict, "setThreadPolicy", "(Landroid/os/StrictMode$ThreadPolicy;)V");
            if (setThreadPolicy) {
              env.CallStaticVoidMethod(clsStrict, setThreadPolicy, policy);
            }
          }
        }
      }
    }

    jclass clsURL = env.FindClass("java/net/URL");
    if (!clsURL) return false;
    jmethodID urlCtor = env.GetMethodID(clsURL, "<init>", "(Ljava/lang/String;)V");
    jmethodID openConnection = env.GetMethodID(clsURL, "openConnection", "()Ljava/net/URLConnection;");
    jstring jUrlStr = env.NewStringUTF(fetchUrl.c_str());
    jobject urlObj = env.NewObject(clsURL, urlCtor, jUrlStr);

    // `URL` ctor throws MalformedURLException on bad input. Drain it
    // so we can blame the right thing in logs rather than the silent
    // path below.
    {
      std::string excClass, excMsg;
      if (DrainPendingJniException(env, excClass, excMsg)) {
        RecordLastHttpFetchError("url-ctor", excClass, excMsg);
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[http-esm][fetch][exception] stage=url-ctor url=%s class=%s msg=%s",
                      url.c_str(), excClass.c_str(), excMsg.c_str());
        }
        return false;
      }
    }

    jobject conn = env.CallObjectMethod(urlObj, openConnection);
    // `URL.openConnection()` can throw IOException for unsupported
    // protocols or proxy lookup failures. Capture the message before
    // it gets eaten by the bare `return false`.
    {
      std::string excClass, excMsg;
      if (DrainPendingJniException(env, excClass, excMsg)) {
        RecordLastHttpFetchError("open-connection", excClass, excMsg);
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[http-esm][fetch][exception] stage=open-connection url=%s class=%s msg=%s",
                      url.c_str(), excClass.c_str(), excMsg.c_str());
        }
        return false;
      }
    }
    if (!conn) return false;

    jclass clsConn = env.GetObjectClass(conn);
    jmethodID setConnectTimeout = env.GetMethodID(clsConn, "setConnectTimeout", "(I)V");
    jmethodID setReadTimeout = env.GetMethodID(clsConn, "setReadTimeout", "(I)V");
    jmethodID setDoInput = env.GetMethodID(clsConn, "setDoInput", "(Z)V");
    jmethodID setUseCaches = env.GetMethodID(clsConn, "setUseCaches", "(Z)V");
    jmethodID setReqProp = env.GetMethodID(clsConn, "setRequestProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
  env.CallVoidMethod(conn, setConnectTimeout, 15000);
  env.CallVoidMethod(conn, setReadTimeout, 15000);
  if (setDoInput) { env.CallVoidMethod(conn, setDoInput, JNI_TRUE); }
  if (setUseCaches) { env.CallVoidMethod(conn, setUseCaches, JNI_FALSE); }
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Accept"), env.NewStringUTF("application/javascript, text/javascript, */*;q=0.1"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Accept-Encoding"), env.NewStringUTF("identity"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Cache-Control"), env.NewStringUTF("no-cache"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Connection"), env.NewStringUTF("close"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("User-Agent"), env.NewStringUTF("NativeScript-HTTP-ESM"));

    // Try to get status via HttpURLConnection if possible
    jclass clsHttp = env.FindClass("java/net/HttpURLConnection");
    bool isHttp = clsHttp && env.IsInstanceOf(conn, clsHttp);
    jmethodID getResponseCode = isHttp ? env.GetMethodID(clsHttp, "getResponseCode", "()I") : nullptr;
    jmethodID getErrorStream = isHttp ? env.GetMethodID(clsHttp, "getErrorStream", "()Ljava/io/InputStream;") : nullptr;
    if (isHttp && getResponseCode) {
      status = env.CallIntMethod(conn, getResponseCode);
      // `getResponseCode()` is the call that actually performs the TCP
      // connect — so this is where ConnectException / SocketTimeout /
      // UnknownHost surface. Drain the exception here so it doesn't return
      // as a bare `status=0`.
      std::string excClass, excMsg;
      if (DrainPendingJniException(env, excClass, excMsg)) {
        RecordLastHttpFetchError("get-response-code", excClass, excMsg);
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[http-esm][fetch][exception] stage=get-response-code url=%s class=%s msg=%s",
                      url.c_str(), excClass.c_str(), excMsg.c_str());
        }
        return false;
      }
    }

    // Read InputStream (prefer error stream on HTTP error codes)
    jmethodID getInputStream = env.GetMethodID(clsConn, "getInputStream", "()Ljava/io/InputStream;");
    jobject inStream = nullptr;
    if (isHttp && status >= 400 && getErrorStream) {
      inStream = env.CallObjectMethod(conn, getErrorStream);
    }
    if (!inStream) {
      inStream = env.CallObjectMethod(conn, getInputStream);
    }
    // `getInputStream()` is the second place a connect failure surfaces
    // (when the previous `getResponseCode` path didn't trigger it,
    // e.g. for non-HTTP URLConnection subclasses).
    {
      std::string excClass, excMsg;
      if (DrainPendingJniException(env, excClass, excMsg)) {
        RecordLastHttpFetchError("get-input-stream", excClass, excMsg);
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[http-esm][fetch][exception] stage=get-input-stream url=%s class=%s msg=%s",
                      url.c_str(), excClass.c_str(), excMsg.c_str());
        }
        return false;
      }
    }
    if (!inStream) return false;

    jclass clsIS = env.GetObjectClass(inStream);
    jmethodID readMethod = env.GetMethodID(clsIS, "read", "([B)I");
    jmethodID closeIS = env.GetMethodID(clsIS, "close", "()V");

    jclass clsBAOS = env.FindClass("java/io/ByteArrayOutputStream");
    jmethodID baosCtor = env.GetMethodID(clsBAOS, "<init>", "()V");
    jmethodID baosWrite = env.GetMethodID(clsBAOS, "write", "([BII)V");
    jmethodID baosToByteArray = env.GetMethodID(clsBAOS, "toByteArray", "()[B");
    jmethodID baosClose = env.GetMethodID(clsBAOS, "close", "()V");
    jobject baos = env.NewObject(clsBAOS, baosCtor);

    jbyteArray buffer = env.NewByteArray(8192);
    while (true) {
      jint n = env.CallIntMethod(inStream, readMethod, buffer);
      if (n < 0) break; // -1 indicates EOF
      if (n == 0) {
        // Defensive: continue reading if zero bytes returned
        continue;
      }
      env.CallVoidMethod(baos, baosWrite, buffer, 0, n);
    }

    env.CallVoidMethod(inStream, closeIS);
    jbyteArray bytes = (jbyteArray) env.CallObjectMethod(baos, baosToByteArray);
    env.CallVoidMethod(baos, baosClose);

    if (!bytes) return false;
    jsize len = env.GetArrayLength(bytes);
    out.resize(static_cast<size_t>(len));
    if (len > 0) {
      env.GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte*>(&out[0]));
    }

    // Content-Type if available
    jmethodID getContentType = env.GetMethodID(clsConn, "getContentType", "()Ljava/lang/String;");
    jstring jct = (jstring) env.CallObjectMethod(conn, getContentType);
    if (jct) {
      contentType = ArgConverter::jstringToString(jct);
    }

    if (status == 0) status = 200; // assume OK if not HTTP
    bool ok = status >= 200 && status < 300 && !out.empty();
    // A fresh body arrived from origin — the bust request (if any) has
    // been satisfied. Clear the mark so steady-state re-fetches of the
    // same URL don't keep paying the nonce (and stay exact-match for
    // routes that require it).
    if (ok && bustRequested) {
      ClearCacheBustForUrl(url);
    }
    return ok;
  } catch (NativeScriptException& nse) {
    // `JEnv::CheckForJavaException()` converts any pending Java
    // exception into a `NativeScriptException` and rethrows on the
    // C++ side. Because JEnv has already called `ExceptionClear`,
    // `DrainPendingJniException` at the JNI call sites above sees
    // nothing — the only place the original Java message survives
    // is on this exception object. So we record it here too,
    // covering both the wrapped JNI calls (`env.GetMethodID`,
    // `env.CallVoidMethod`, etc.) and the raw `m_env->` calls in
    // the same try block.
    std::string what = nse.what() ? nse.what() : "";
    if (what.empty()) {
      what = nse.GetErrorMessage();
    }
    RecordLastHttpFetchError("native-script-exception", "tns::NativeScriptException", what);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][fetch][exception] stage=native-script-exception url=%s msg=%s",
                  url.c_str(), what.c_str());
    }
    return false;
  } catch (std::exception& ex) {
    std::string what = ex.what() ? ex.what() : "<unknown>";
    RecordLastHttpFetchError("std-exception", "std::exception", what);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][fetch][exception] stage=std-exception url=%s msg=%s",
                  url.c_str(), what.c_str());
    }
    return false;
  } catch (...) {
    RecordLastHttpFetchError("unknown-cpp-exception", "<unknown>", "<no message available>");
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][fetch][exception] stage=unknown-cpp-exception url=%s",
                  url.c_str());
    }
    return false;
  }
}

// ─────────────────────────────────────────────────────────────
// List-mode kickstart prewarm.
//
// The dev server owns the module graph: it computes the inverse-dep
// closure for HMR updates (`evictPaths`) and can crawl the entry graph
// for cold boot. The client hands that explicit URL list to
// `__NS_DEV__.kickstartPrefetch(urls)`, which fetches every entry in one
// parallel wave into `g_prefetchCache` before V8 starts its serial
// synchronous walk. Concurrency is bounded by a counting semaphore
// implemented with mutex + condition variable (NDKs do not yet ship the
// C++20 `std::counting_semaphore`).

namespace {

class CountingSemaphore {
 public:
  explicit CountingSemaphore(int initial) : count_(initial) {}
  void Acquire() {
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [this]{ return count_ > 0; });
    --count_;
  }
  void Release() {
    {
      std::lock_guard<std::mutex> lk(m_);
      ++count_;
    }
    cv_.notify_one();
  }

 private:
  std::mutex m_;
  std::condition_variable cv_;
  int count_;
};

struct KickstartContext {
  std::mutex mutex;
  std::unordered_set<std::string> visited;
  std::atomic<size_t> fetchedCount{0};
  std::atomic<size_t> bytes{0};
  std::unique_ptr<CountingSemaphore> concurrency;

  // Outstanding-work counter: each scheduled fetch increments + decrements a
  // counter under a mutex, and the wait loop blocks on `cv.wait_for` for
  // transitions to zero.
  std::mutex pendingMutex;
  std::condition_variable pendingCv;
  int pending = 0;

  void EnterPending() {
    std::lock_guard<std::mutex> lk(pendingMutex);
    ++pending;
  }
  void LeavePending() {
    {
      std::lock_guard<std::mutex> lk(pendingMutex);
      if (pending > 0) --pending;
    }
    pendingCv.notify_all();
  }
  // Wait up to `sliceMs` for `pending == 0`. Returns true if drained.
  bool WaitDrainSlice(int sliceMs) {
    std::unique_lock<std::mutex> lk(pendingMutex);
    return pendingCv.wait_for(lk, std::chrono::milliseconds(sliceMs),
                              [this]{ return pending == 0; });
  }
};

void KickstartScheduleUrls(std::shared_ptr<KickstartContext> ctx,
                           std::vector<std::string> urls) {
  for (const std::string& urlRef : urls) {
    if (urlRef.empty()) continue;
    if (!StartsWith(urlRef, "http://") && !StartsWith(urlRef, "https://")) continue;
    if (!LooksLikeJsSourceUrl(urlRef)) continue;
    if (!IsRemoteUrlAllowed(urlRef)) continue;

    bool fresh;
    {
      std::lock_guard<std::mutex> lock(ctx->mutex);
      fresh = ctx->visited.insert(urlRef).second;
    }
    if (!fresh) continue;

    // No "already cached" short-circuit here — the caller has explicitly
    // told us "fetch these URLs fresh". Any body sitting in
    // `g_prefetchCache` for one of them is a leftover from a previous
    // wave that V8 didn't consume; honoring it would feed V8 a stale
    // body on the next walk — the "1 cycle behind" symptom for `.ts`
    // edits with many transitive importers. (`InvalidateModules`
    // pre-clears the cache for the eviction set, so this is
    // defense-in-depth — but the kickstart may also be invoked
    // manually for diagnostics, and we want it to be correct in
    // isolation.)

    ctx->EnterPending();
    std::string urlCopy = urlRef;
    auto ctxCopy = ctx;
    std::thread([ctxCopy, urlCopy]() {
      ctxCopy->concurrency->Acquire();
      std::string body, contentType;
      int status = 0;
      bool ok = PerformHttpFetchOnceSync(urlCopy, body, contentType, status);
      if (ok && status >= 200 && status < 300 && !body.empty()) {
        const size_t bodySize = body.size();
        // Overwrite unconditionally — the fresh body we just fetched is
        // by definition the authoritative copy; any older cache entry is
        // stale by construction (the caller has just told us so).
        {
          std::lock_guard<std::mutex> lock(g_prefetchMutex);
          g_prefetchCache[urlCopy] = std::move(body);
        }
        ctxCopy->fetchedCount.fetch_add(1, std::memory_order_relaxed);
        ctxCopy->bytes.fetch_add(bodySize, std::memory_order_relaxed);
      }
      ctxCopy->concurrency->Release();
      ctxCopy->LeavePending();
    }).detach();
  }
}

}  // anonymous namespace

bool KickstartHmrPrefetchUrlsSync(const std::vector<std::string>& urls,
                                  int maxConcurrent,
                                  double timeoutSeconds,
                                  size_t* outFetchedCount,
                                  uint64_t* outElapsedMs) {
  if (urls.empty()) return false;
  // Drop empty / non-allowlisted URLs up front. We still want a
  // truthy result even if some entries get filtered, because partial
  // success is strictly better than the no-kickstart baseline.
  std::vector<std::string> filtered;
  filtered.reserve(urls.size());
  for (const auto& u : urls) {
    if (u.empty()) continue;
    if (!IsRemoteUrlAllowed(u)) continue;
    filtered.push_back(u);
  }
  if (filtered.empty()) return false;

  if (maxConcurrent <= 0) maxConcurrent = 16;
  if (timeoutSeconds <= 0.0) timeoutSeconds = 10.0;

  const auto start = std::chrono::steady_clock::now();

  // Diagnostic seed — we record the first URL purely so the log line
  // has a recognizable anchor when the user is correlating with their
  // server-side `[hmr-ws][update] file=...` line.
  const std::string diagSeed = filtered.front();
  const size_t requestedCount = filtered.size();

  auto ctx = std::make_shared<KickstartContext>();
  ctx->concurrency = std::make_unique<CountingSemaphore>(maxConcurrent);

  KickstartScheduleUrls(ctx, std::move(filtered));

  // Wait loop. Uses a slice-based timeout so the host runloop (via the
  // pluggable yield hook) gets a chance to drain between slices — this
  // matters most during cold boot, before the dev client has called
  // `__NS_DEV__.setDevBootComplete(true)`. 50ms is short enough to feel
  // responsive and long enough to avoid spinning.
  const int sliceMs = 50;
  const auto deadline = start + std::chrono::milliseconds(static_cast<long>(timeoutSeconds * 1000.0));
  bool drained = false;
  while (true) {
    drained = ctx->WaitDrainSlice(sliceMs);
    if (drained) break;
    if (std::chrono::steady_clock::now() >= deadline) break;
    if (!IsDevSessionBootComplete()) {
      InvokeHttpFetchYield();
    }
  }

  const auto end = std::chrono::steady_clock::now();
  const uint64_t elapsedMs =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  const size_t fetched = ctx->fetchedCount.load(std::memory_order_relaxed);
  const size_t bytes = ctx->bytes.load(std::memory_order_relaxed);

  if (outFetchedCount) *outFetchedCount = fetched;
  if (outElapsedMs) *outElapsedMs = elapsedMs;

  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[hmr-kickstart][list] first=%s urls=%lu fetched=%lu bytes=%lu ms=%llu status=%s concurrency=%d",
                diagSeed.c_str(),
                (unsigned long)requestedCount,
                (unsigned long)fetched,
                (unsigned long)bytes,
                (unsigned long long)elapsedMs,
                drained ? "drained" : "timeout",
                maxConcurrent);
  }

  return drained;
}

void CleanupHMRGlobals() {
  // Drop any kickstart-prewarmed module sources. These are plain
  // std::string buffers (no v8::Global), but flushing them on teardown
  // prevents stale source from leaking into a re-launched runtime in
  // the same process.
  ClearHttpModulePrefetchCache();
  ClearAllCacheBustMarks();
  // Reset the boot-complete flag so a re-launched runtime in the same
  // process starts in "cold boot" mode again (yield pump armed).
  g_devSessionBootComplete.store(false, std::memory_order_relaxed);
}

// ─────────────────────────────────────────────────────────────
// Dev-loader JS-callable globals
//
// The runtime's dev surface is deliberately small: it exposes
// *mechanism* only (resolution config, registry eviction, parallel
// prewarm, registry introspection, boot-complete signal). All HMR
// *policy* — boot orchestration, `import.meta.hot`, full reload, CSS
// apply, WebSocket protocol — lives in the JS dev client
// (`@nativescript/vite`).

namespace {

// Sets the function name on the v8 Function for nicer stack traces and
// attaches it as a method of the `__NS_DEV__` namespace object.
void InstallDevFunction(v8::Isolate* isolate, v8::Local<v8::Context> context,
                        v8::Local<v8::Object> target, const char* name,
                        v8::FunctionCallback callback) {
  v8::Local<v8::FunctionTemplate> fnTpl =
      v8::FunctionTemplate::New(isolate, callback);
  v8::Local<v8::Function> fn = fnTpl->GetFunction(context).ToLocalChecked();
  fn->SetName(ToV8String(isolate, name));
  target->CreateDataProperty(context, ToV8String(isolate, name), fn)
      .Check();
}

// Parse an import-map value (a JSON string OR a JS object of shape
// `{ imports: { "<key>": "<url>", ... } }`) into flat (key → URL) entries using
// V8's own JSON/object model. Returns true if it found an `imports` object
// (even if empty); false if the value is unusable. Only flat string key→URL
// mappings are honored; non-string import values are skipped.
bool ReadImportMapEntries(v8::Isolate* isolate,
                          v8::Local<v8::Context> context,
                          v8::Local<v8::Value> importMapValue,
                          std::vector<std::pair<std::string, std::string>>* out) {
  v8::Local<v8::Value> mapVal = importMapValue;
  if (mapVal->IsString()) {
    v8::Local<v8::Value> parsed;
    if (!v8::JSON::Parse(context, mapVal.As<v8::String>()).ToLocal(&parsed)) {
      return false;
    }
    mapVal = parsed;
  }
  if (!mapVal->IsObject()) return false;
  v8::Local<v8::Object> mapObj = mapVal.As<v8::Object>();

  v8::Local<v8::Value> importsVal;
  if (!mapObj->Get(context, ToV8String(isolate, "imports")).ToLocal(&importsVal) ||
      !importsVal->IsObject()) {
    return false;
  }
  v8::Local<v8::Object> imports = importsVal.As<v8::Object>();
  v8::Local<v8::Array> keys;
  if (!imports->GetOwnPropertyNames(context).ToLocal(&keys)) return false;

  for (uint32_t i = 0; i < keys->Length(); ++i) {
    v8::Local<v8::Value> keyVal;
    if (!keys->Get(context, i).ToLocal(&keyVal)) continue;
    v8::Local<v8::Value> valVal;
    if (!imports->Get(context, keyVal).ToLocal(&valVal) || !valVal->IsString()) continue;
    v8::String::Utf8Value keyUtf8(isolate, keyVal);
    v8::String::Utf8Value valUtf8(isolate, valVal);
    if (*keyUtf8 && *valUtf8) {
      out->emplace_back(std::string(*keyUtf8), std::string(*valUtf8));
    }
  }
  return true;
}

// `__NS_DEV__.configureRuntime(config)` — apply the dev client's resolver
// configuration: the bare-specifier import map and the volatile URL
// patterns. Bare-specifier resolution happens inside V8's synchronous
// `ResolveModuleCallback` — an embedder host callback JS cannot install
// or intercept — which is why this must be native.
void ConfigureDevRuntimeCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
  bool logScriptLoading = tns::IsScriptLoadingLogEnabled();

  if (info.Length() < 1 || !info[0]->IsObject()) {
    if (logScriptLoading) {
      DEBUG_WRITE("[__NS_DEV__.configureRuntime] expected config object argument");
    }
    return;
  }
  v8::Local<v8::Object> config = info[0].As<v8::Object>();

  // importMap: accept either a JSON string or an object with `{imports:{}}`.
  // The dev server's runtime-config endpoint serializes as an object;
  // older entry paths pass a serialized JSON string. Accept both shapes.
  v8::Local<v8::Value> importMapVal;
  if (config->Get(ctx, ToV8String(isolate, "importMap")).ToLocal(&importMapVal) &&
      !importMapVal->IsUndefined() && !importMapVal->IsNull()) {
    std::vector<std::pair<std::string, std::string>> importEntries;
    if (ReadImportMapEntries(isolate, ctx, importMapVal, &importEntries) &&
        !importEntries.empty()) {
      SetImportMapEntries(importEntries);
      if (logScriptLoading) {
        DEBUG_WRITE("[__NS_DEV__.configureRuntime] import map set (%zu entries)", importEntries.size());
      }
    }
  }

  // volatilePatterns: list of URL substrings that should always re-fetch.
  v8::Local<v8::Value> vpVal;
  if (config->Get(ctx, ToV8String(isolate, "volatilePatterns")).ToLocal(&vpVal) && vpVal->IsArray()) {
    v8::Local<v8::Array> arr = vpVal.As<v8::Array>();
    std::vector<std::string> patterns;
    patterns.reserve(arr->Length());
    for (uint32_t i = 0; i < arr->Length(); i++) {
      v8::Local<v8::Value> elem;
      if (arr->Get(ctx, i).ToLocal(&elem) && elem->IsString()) {
        v8::String::Utf8Value utf8(isolate, elem);
        if (*utf8) patterns.emplace_back(*utf8);
      }
    }
    if (!patterns.empty()) {
      SetVolatilePatterns(patterns);
      if (logScriptLoading) {
        DEBUG_WRITE("[__NS_DEV__.configureRuntime] %zu volatile patterns set", patterns.size());
      }
    }
  }
}

void InvalidateModulesCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

  if (info.Length() < 1 || !info[0]->IsArray()) {
    if (tns::IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[__NS_DEV__.invalidateModules] expected array of URL strings");
    }
    return;
  }
  v8::Local<v8::Array> urlsArray = info[0].As<v8::Array>();
  std::vector<std::string> urls;
  urls.reserve(urlsArray->Length());
  for (uint32_t i = 0; i < urlsArray->Length(); i++) {
    v8::Local<v8::Value> v;
    if (!urlsArray->Get(ctx, i).ToLocal(&v) || !v->IsString()) continue;
    v8::String::Utf8Value utf8(isolate, v);
    if (*utf8) urls.emplace_back(*utf8);
  }
  // Observability: surface every URL the runtime is asked to drop so we
  // can correlate "asked to evict X" against "actually had X loaded as
  // Y" when canonicalization differs. Verbose-gated since per-event
  // chatter is only useful while debugging an eviction mismatch.
  if (tns::IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[ns-hmr][android-invalidate] called urls.count=%zu", urls.size());
    size_t shown = 0;
    for (const auto& u : urls) {
      if (shown >= 32) break;
      DEBUG_WRITE("[ns-hmr][android-invalidate] url[%zu]=%s", shown, u.c_str());
      ++shown;
    }
    if (urls.size() > shown) {
      DEBUG_WRITE("[ns-hmr][android-invalidate] (hidden %zu more URL(s))", urls.size() - shown);
    }
  }
  InvalidateModules(urls);
}

// `__NS_DEV__.kickstartPrefetch(urls, options?)` lets the HMR client tell
// the runtime "the next (re-)import will walk this module set — please
// pre-fill the loader cache with every listed body before V8 starts
// walking". The list is always server-computed (the dev server owns the
// module graph: eviction closures for HMR, entry-graph crawls for cold
// boot); the runtime performs no graph discovery of its own. A single
// string argument is accepted as a one-element list.
//
// Returns `{ ok, fetched, ms }` so JS can log the result. On failure
// callers should fall back to V8's normal synchronous walk.
void KickstartHmrPrefetchCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

  auto buildResult = [&](bool ok, size_t fetched, uint64_t elapsedMs) {
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    (void)result->Set(ctx, ToV8String(isolate, "ok"), v8::Boolean::New(isolate, ok));
    (void)result->Set(ctx, ToV8String(isolate, "fetched"),
                      v8::Integer::NewFromUnsigned(isolate, (uint32_t)fetched));
    (void)result->Set(ctx, ToV8String(isolate, "ms"),
                      v8::Number::New(isolate, (double)elapsedMs));
    info.GetReturnValue().Set(result);
  };

  if (info.Length() < 1 || (!info[0]->IsString() && !info[0]->IsArray())) {
    if (tns::IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[__NS_DEV__.kickstartPrefetch] expected (urls: string[], options?) or (url: string, options?)");
    }
    buildResult(false, 0, 0);
    return;
  }

  int maxConcurrent = 16;
  double timeoutSeconds = 10.0;
  if (info.Length() >= 2 && info[1]->IsObject()) {
    v8::Local<v8::Object> options = info[1].As<v8::Object>();
    v8::Local<v8::Value> mcVal;
    if (options->Get(ctx, ToV8String(isolate, "maxConcurrent")).ToLocal(&mcVal) &&
        mcVal->IsNumber()) {
      double mc = mcVal->NumberValue(ctx).FromMaybe(16.0);
      if (mc >= 1.0 && mc <= 64.0) maxConcurrent = (int)mc;
    }
    v8::Local<v8::Value> toVal;
    if (options->Get(ctx, ToV8String(isolate, "timeoutMs")).ToLocal(&toVal) &&
        toVal->IsNumber()) {
      double ms = toVal->NumberValue(ctx).FromMaybe(10000.0);
      if (ms >= 100.0 && ms <= 60000.0) timeoutSeconds = ms / 1000.0;
    }
  }

  std::vector<std::string> urls;
  if (info[0]->IsArray()) {
    v8::Local<v8::Array> arr = info[0].As<v8::Array>();
    const uint32_t len = arr->Length();
    urls.reserve(len);
    for (uint32_t i = 0; i < len; i++) {
      v8::Local<v8::Value> elem;
      if (!arr->Get(ctx, i).ToLocal(&elem)) continue;
      if (!elem->IsString()) continue;
      v8::String::Utf8Value u8(isolate, elem);
      if (!*u8) continue;
      std::string s(*u8);
      if (s.empty()) continue;
      urls.push_back(std::move(s));
    }
  } else {
    v8::String::Utf8Value u8(isolate, info[0]);
    if (*u8) {
      std::string s(*u8);
      if (!s.empty()) urls.push_back(std::move(s));
    }
  }

  if (urls.empty()) {
    buildResult(false, 0, 0);
    return;
  }

  size_t fetched = 0;
  uint64_t elapsedMs = 0;
  bool ok = KickstartHmrPrefetchUrlsSync(urls, maxConcurrent, timeoutSeconds,
                                         &fetched, &elapsedMs);
  buildResult(ok, fetched, elapsedMs);
}

void GetLoadedModuleUrlsCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
  std::vector<std::string> urls = GetLoadedModuleUrls();
  v8::Local<v8::Array> result = v8::Array::New(isolate, static_cast<int>(urls.size()));
  for (uint32_t i = 0; i < urls.size(); i++) {
    (void)result->Set(ctx, i, ToV8String(isolate, urls[i].c_str()));
  }
  info.GetReturnValue().Set(result);
}

// `__NS_DEV__.setDevBootComplete(value?: boolean)` — the JS dev client calls
// this (with `true`, or no argument) once the real app root view has
// committed. It flips both the JS-visible `__NS_HMR_BOOT_COMPLETE__`
// global and the native atomic that disarms the cold-boot yield cadence
// in the kickstart pump-wait. The client may also pass `false` before
// a full JS-realm reload to re-arm the boot-time behaviors.
void SetDevBootCompleteCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

  bool value = true;
  if (info.Length() >= 1 && !info[0]->IsUndefined() && !info[0]->IsNull()) {
    value = info[0]->BooleanValue(isolate);
  }

  tns::SetDevBootComplete(isolate, ctx, value);
}

// Debug-only diagnostic: expose CanonicalizeHttpUrlKey to JS so the test
// harness can pin its identity behavior. Not part of the @nativescript/vite
// client API; release builds omit it.
void CanonicalizeHttpUrlKeyCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  if (info.Length() < 1 || !info[0]->IsString()) {
    info.GetReturnValue().SetEmptyString();
    return;
  }
  v8::String::Utf8Value u(isolate, info[0]);
  std::string key = CanonicalizeHttpUrlKey(*u ? std::string(*u) : std::string());
  info.GetReturnValue().Set(ToV8String(isolate, key.c_str()));
}

}  // anonymous namespace

void InitializeHmrDevGlobals(v8::Isolate* isolate, v8::Local<v8::Context> context,
                             bool isWorker) {
  // The dev host API lives here: `__NS_DEV__`.
  //
  // Installed in EVERY build, release included. That is deliberate: the
  // security boundary sits at the network layer, not the namespace —
  // every HTTP fetch the runtime can make passes through
  // `IsRemoteUrlAllowed()` (DevFlags.cpp), which in release builds denies
  // everything unless the app config explicitly opts in via
  // `security.allowRemoteModules`. Remote module loading is a supported
  // release feature behind that opt-in, and an opted-in app needs
  // `configureRuntime` / `invalidateModules` / `kickstartPrefetch` to
  // operate it. For default-config release apps the members are inert.
  v8::Local<v8::Object> dev = v8::Object::New(isolate);

  InstallDevFunction(isolate, context, dev, "configureRuntime", ConfigureDevRuntimeCallback);
  InstallDevFunction(isolate, context, dev, "invalidateModules", InvalidateModulesCallback);
  InstallDevFunction(isolate, context, dev, "kickstartPrefetch", KickstartHmrPrefetchCallback);
  InstallDevFunction(isolate, context, dev, "getLoadedModuleUrls", GetLoadedModuleUrlsCallback);
  InstallDevFunction(isolate, context, dev, "setDevBootComplete", SetDevBootCompleteCallback);

  // Main-isolate only: terminating workers from inside a worker would let
  // a stuck worker take down its peers (see CallbackHandlers.h).
  if (!isWorker) {
    InstallDevFunction(isolate, context, dev, "terminateAllWorkers",
                       CallbackHandlers::TerminateAllWorkersCallback);
  }

  if (IsDebuggable()) {
    // Debug-only diagnostic: expose the HTTP canonical-key function to JS so
    // the test harness can pin its identity behavior across cache-busters
    // and dev-endpoint query normalization.
    InstallDevFunction(isolate, context, dev, "canonicalizeHttpUrlKey",
                       CanonicalizeHttpUrlKeyCallback);
  }

  context->Global()
      ->Set(context, ToV8String(isolate, "__NS_DEV__"), dev)
      .FromMaybe(false);
  MirrorGlobalOnGlobalThis(isolate, context, "__NS_DEV__");
}

} // namespace tns

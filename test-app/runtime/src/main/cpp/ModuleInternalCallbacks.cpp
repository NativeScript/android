#include "ModuleInternal.h"
#include "ModuleInternalCallbacks.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include "Runtime.h"
#include "Util.h"
#include <sys/stat.h>
#include <string>
#include <vector>
#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <unordered_set>
#include "HMRSupport.h"
#include "DevFlags.h"
#include "JEnv.h"

using namespace v8;
using namespace std;
using namespace tns;

// ────────────────────────────────────────────────────────────────────────────
// Forward declarations for helpers defined below their first use. Every helper
// called from ResolveModuleCallback / ImportModuleDynamicallyCallback is
// declared here so definition order within the translation unit doesn't matter.
namespace tns {
static inline bool StartsWith(const std::string& s, const char* prefix);
static inline bool EndsWith(const std::string& s, const char* suffix);
static std::string CanonicalizeRegistryKey(const std::string& key);
static std::string NormalizeViteSpecifier(const std::string& specifier);
static std::string LookupImportMap(const std::string& specifier);
static bool HasUrlScheme(const std::string& spec);
static bool IsSyntheticNamespaceKey(const std::string& key);
static bool IsVolatileUrl(const std::string& url);
static v8::MaybeLocal<v8::Module> CompileModuleForResolveRegisterOnly(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& source, const std::string& registryKey);
static v8::MaybeLocal<v8::Module> ResolveFromVendorRegistry(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& vendorId);
}  // namespace tns

// ────────────────────────────────────────────────────────────────────────────
// Per-isolate ES module registry.
//
// `g_moduleRegistry` is `thread_local`: each NS isolate (the main JS thread
// plus each Worker thread) gets its own per-thread map. `v8::Global<Module>`
// handles are isolate-bound; sharing one map across isolates lets thread A
// fetch a handle thread B created, and V8 will fail the identity check during
// `InstantiateModule` (the dependency module belongs to a different isolate
// than the module it's being linked into). We use the reference + leaky
// singleton pattern so that every call site continues to write
// `g_moduleRegistry[key] = …` without churning ~100+ call sites to use
// accessor functions, AND so that we don't run a destructor on the underlying
// map when the worker thread tears down (which would call
// `v8::Global<Module>::Reset()` on handles whose isolate may already be gone).
//
// On each thread's first use of `g_moduleRegistry`, the initializer below
// runs once per thread to bind the reference to that thread's map.
namespace {
using ModuleHandleMap = std::unordered_map<std::string, v8::Global<v8::Module>>;

ModuleHandleMap& MakePerIsolateModuleRegistry() {
  thread_local auto* p = new ModuleHandleMap();
  return *p;
}
}  // namespace

namespace tns {
thread_local std::unordered_map<std::string, v8::Global<v8::Module>>& g_moduleRegistry =
    MakePerIsolateModuleRegistry();
}  // namespace tns

// ────────────────────────────────────────────────────────────────────────────
// Per-process module-resolution state (import map + vendor cache + fallbacks).
//
// These are leaky singletons for the same reason as the HMR registries in
// HMRSupport.cpp: their destructors would call `v8::Global<Module>::Reset()`
// during `__cxa_finalize_ranges`, after the owning isolate is already gone,
// crashing the process on app exit. Heap-allocate + leak (`new` once, never
// `delete`) so the OS reclaims memory on process exit and we never run a
// destructor that touches V8.
//
// Vendor cache is `thread_local` because vendor SyntheticModules are
// isolate-bound. Reusing one across isolates breaks the linker's
// export-table check. The other registries are process-wide configuration
// (no v8::Global) and intentionally shared across isolates.

namespace tns {

static std::mutex g_importMapMutex;
static auto* _g_importMap = new std::unordered_map<std::string, std::string>();
static auto& g_importMap = *_g_importMap;

static std::mutex g_volatilePatternsMutex;
static auto* _g_volatilePatterns = new std::vector<std::string>();
static auto& g_volatilePatterns = *_g_volatilePatterns;

namespace {
using ModuleHandleMap = std::unordered_map<std::string, v8::Global<v8::Module>>;

ModuleHandleMap& MakePerIsolateVendorModuleCache() {
  thread_local auto* p = new ModuleHandleMap();
  return *p;
}
}  // namespace

static thread_local std::unordered_map<std::string, v8::Global<v8::Module>>& g_vendorModuleCache =
    MakePerIsolateVendorModuleCache();

// Set of canonical keys currently being resolved (loop-breaker for cyclic
// HTTP imports). thread_local because dynamic-import waits are isolate-bound.
namespace {
std::unordered_set<std::string>& MakePerIsolateInFlightSet() {
  thread_local auto* p = new std::unordered_set<std::string>();
  return *p;
}
}  // namespace
static thread_local std::unordered_set<std::string>& g_modulesInFlight =
    MakePerIsolateInFlightSet();

}  // namespace tns

// Forward declaration used by logging helper
std::string GetApplicationPath();


// Diagnostic helper: emit detailed V8 compile error info for HTTP ESM sources.
static void LogHttpCompileDiagnostics(v8::Isolate* isolate,
                                      v8::Local<v8::Context> context,
                                      const std::string& url,
                                      const std::string& code,
                                      v8::TryCatch& tc) {
    if (!IsScriptLoadingLogEnabled()) {
        return;
    }
    using namespace v8;

    const char* classification = "unknown";
    std::string msgStr;
    std::string srcLineStr;
    int lineNum = 0;
    int startCol = 0;
    int endCol = 0;

    Local<Message> message = tc.Message();
    if (!message.IsEmpty()) {
        String::Utf8Value m8(isolate, message->Get());
        if (*m8) msgStr = *m8;
        lineNum = message->GetLineNumber(context).FromMaybe(0);
        startCol = message->GetStartColumn();
        endCol = message->GetEndColumn();
        MaybeLocal<String> maybeLine = message->GetSourceLine(context);
        if (!maybeLine.IsEmpty()) {
            String::Utf8Value l8(isolate, maybeLine.ToLocalChecked());
            if (*l8) srcLineStr = *l8;
        }
        // Classify the failure for quick triage.
        if (msgStr.find("Unexpected identifier") != std::string::npos ||
            msgStr.find("Unexpected token") != std::string::npos) {
            if (msgStr.find("export") != std::string::npos &&
                code.find("export default") == std::string::npos &&
                code.find("__sfc__") != std::string::npos) {
                classification = "missing-export-default";
            } else {
                classification = "syntax";
            }
        } else if (msgStr.find("Cannot use import statement") != std::string::npos) {
            classification = "wrap-error";
        }
    }
    if (strcmp(classification, "unknown") == 0) {
        if (code.find("export default") == std::string::npos && code.find("__sfc__") != std::string::npos) classification = "missing-export-default";
        else if (code.find("__sfc__") != std::string::npos && code.find("export {") == std::string::npos && code.find("export ") == std::string::npos) classification = "no-exports";
        else if (code.find("import ") == std::string::npos && code.find("export ") == std::string::npos) classification = "not-module";
        else if (code.find("_openBlock") != std::string::npos && code.find("openBlock") == std::string::npos) classification = "underscore-helper-unmapped";
    }

    // FNV-1a 64-bit hash of source for correlation
    unsigned long long h = 1469598103934665603ull; // offset basis
    for (unsigned char c : code) { h ^= c; h *= 1099511628211ull; }

    // Trim the snippet for readability
    std::string snippet = code.substr(0, 600);
    for (char& ch : snippet) { if (ch == '\n' || ch == '\r') ch = ' '; }
    if (srcLineStr.size() > 240) srcLineStr = srcLineStr.substr(0, 240);

    DEBUG_WRITE("[http-esm][compile][v8-error][%s] %s line=%d col=%d..%d hash=%llx bytes=%lu msg=%s srcLine=%s snippet=%s",
                classification,
                url.c_str(),
                lineNum,
                startCol,
                endCol,
                (unsigned long long)h,
                (unsigned long)code.size(),
                msgStr.c_str(),
                srcLineStr.c_str(),
                snippet.c_str());
}

// Resolution of relative / root-absolute import specifiers against an http(s)
// referrer lives in `HMRSupport.cpp` (`ResolveImportSpecifierAgainstUrl`); call
// sites below invoke `tns::ResolveImportSpecifierAgainstUrl(spec, referrer)`.

// ────────────────────────────────────────────────────────────────────────────
// Module-resolution helpers (ESM resolver hardening). Helpers referenced before
// their definition are forward-declared at the top of this file.
namespace tns {

static inline bool StartsWith(const std::string& s, const char* prefix) {
  size_t n = strlen(prefix);
  return s.size() >= n && s.compare(0, n, prefix) == 0;
}

static inline bool EndsWith(const std::string& s, const char* suffix) {
  size_t n = strlen(suffix);
  return s.size() >= n && s.compare(s.size() - n, n, suffix) == 0;
}

// Synthetic-namespace keys (ns-vendor://, optional:, node:, blob:) are NOT
// filesystem paths. Treat them as opaque identifiers — never collapse,
// percent-decode, or path-normalize them — so they keep their exact registry
// identity through invalidation and reload.
static bool IsSyntheticNamespaceKey(const std::string& key) {
  return StartsWith(key, "ns-vendor://") || StartsWith(key, "optional:") ||
         StartsWith(key, "node:") || StartsWith(key, "blob:");
}

// Returns true for any specifier with a leading URL scheme of the form
// `<scheme>:` where the scheme contains no `/` (filesystem paths and bare
// specifiers stay false). Used by InitializeImportMetaObject to decide
// whether `import.meta.url` should preserve the specifier verbatim vs.
// wrap it in `file://`.
static bool HasUrlScheme(const std::string& spec) {
  size_t schemePos = spec.find(':');
  if (schemePos == std::string::npos || schemePos == 0) return false;
  size_t slashPos = spec.find('/');
  if (slashPos != std::string::npos && slashPos < schemePos) return false;
  // Scheme chars must be alphanumeric / `+` / `-` / `.` per RFC 3986.
  for (size_t i = 0; i < schemePos; ++i) {
    char c = spec[i];
    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '+' && c != '-' && c != '.') {
      return false;
    }
  }
  return true;
}

// Matches `url` against the `volatilePatterns` configured by Vite via
// `__nsConfigureRuntime({ volatilePatterns: [...] })` (substring match).
// Android's HTTP loader does not consult this: it enforces volatility
// structurally — a consume-once prefetch read plus eviction on HMR
// invalidation (see HttpFetchText in HMRSupport.cpp). It is available for a
// read-gated cache policy, which the iOS loader uses.
static bool IsVolatileUrl(const std::string& url) {
  std::lock_guard<std::mutex> lock(g_volatilePatternsMutex);
  for (const auto& pat : g_volatilePatterns) {
    if (url.find(pat) != std::string::npos) return true;
  }
  return false;
}

// Canonicalize a raw module key into a stable registry key.
//
// Rules:
//   - HTTP/HTTPS keys go through `CanonicalizeHttpUrlKey` (drop fragment,
//     normalize bridge endpoints, sort query, strip `?import`).
//   - `file://http(s)://...` keys unwrap the outer scheme, then HTTP-canon.
//   - `blob:` keys are preserved verbatim (they're opaque random IDs).
//   - `ns-vendor://`, `optional:`, `node:` (and any other custom scheme
//     where the scheme appears before the first slash) are preserved
//     verbatim — these are NOT filesystem paths.
//   - Plain filesystem paths fall through unchanged (the Android resolver
//     resolves them lazily via candidate scans).
static std::string CanonicalizeRegistryKey(const std::string& key) {
  if (key.empty()) {
    return key;
  }

  if (StartsWith(key, "http://") || StartsWith(key, "https://") ||
      StartsWith(key, "file://http://") || StartsWith(key, "file://https://")) {
    return CanonicalizeHttpUrlKey(key);
  }
  if (StartsWith(key, "blob:")) {
    return key;
  }
  if (IsSyntheticNamespaceKey(key)) {
    return key;
  }
  // Any other custom scheme, or a plain filesystem path: preserve verbatim.
  // The Android resolver resolves filesystem paths lazily via candidate scans.
  return key;
}

// Compile a module body for "register only" mode used by the HTTP loader
// and vendor wrapper. Caller owns instantiation + evaluation; we just
// produce the v8::Module and seat it in `g_moduleRegistry` under
// `registryKey` so resolver callbacks can find it during link.
static v8::MaybeLocal<v8::Module> CompileModuleForResolveRegisterOnly(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& source, const std::string& registryKey) {
  v8::EscapableHandleScope scope(isolate);
  v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, source);
  v8::Local<v8::String> urlString = ArgConverter::ConvertToV8String(isolate, registryKey);
  v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false, false,
                          true /* is_module */);
  v8::ScriptCompiler::Source src(sourceText, origin);

  v8::Local<v8::Module> mod;
  {
    v8::TryCatch tc(isolate);
    if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
      if (IsScriptLoadingLogEnabled()) {
        v8::Local<v8::Value> ex = tc.Exception();
        v8::String::Utf8Value m(isolate, ex);
        DEBUG_WRITE("[http-esm][compile-register][fail] key=%s msg=%s",
                    registryKey.c_str(), *m ? *m : "(unknown)");
      }
      return v8::MaybeLocal<v8::Module>();
    }
  }
  g_moduleRegistry[registryKey].Reset(isolate, mod);
  return scope.Escape(mod);
}

// Normalize a Vite-rewritten specifier into the canonical import-map key.
// Handles two common Vite dev-server rewrite patterns:
//   1. Prebundled deps:  "/node_modules/.vite/deps/solid-js.js?v=abc"   → "solid-js"
//                        "/node_modules/.vite/deps/@tanstack_solid-router.js" → "@tanstack/solid-router"
//   2. Explicit node_modules paths:
//        "/node_modules/@angular/core/fesm2022/core.mjs" → "@angular/core/fesm2022/core.mjs"
//        "/node_modules/tslib/tslib.es6.mjs"             → "tslib"
//
// For explicit node_modules paths we preserve non-main-entry subpaths so the
// import map's trailing-slash HTTP prefixes can keep complex package build
// outputs on HTTP. Only bare package roots and simple root-level main entries
// collapse back to the package id for vendor/exact import-map resolution.
static std::string NormalizeViteSpecifier(const std::string& specifier) {
  // Pattern 1: Vite prebundled deps — /node_modules/.vite/deps/<flattened-id>.js
  {
    const std::string viteDepsPrefix = "/node_modules/.vite/deps/";
    const std::string viteDepsPrefix2 = "node_modules/.vite/deps/";
    std::string prefix;
    if (specifier.compare(0, viteDepsPrefix.size(), viteDepsPrefix) == 0)
      prefix = viteDepsPrefix;
    else if (specifier.compare(0, viteDepsPrefix2.size(), viteDepsPrefix2) == 0)
      prefix = viteDepsPrefix2;

    if (!prefix.empty()) {
      std::string id = specifier.substr(prefix.size());
      auto qpos = id.find('?');
      if (qpos != std::string::npos) id = id.substr(0, qpos);
      auto dotpos = id.rfind('.');
      if (dotpos != std::string::npos) id = id.substr(0, dotpos);
      if (!id.empty() && id[0] == '@') {
        auto upos = id.find('_');
        if (upos != std::string::npos) {
          id = id.substr(0, upos) + "/" + id.substr(upos + 1);
          auto upos2 = id.find('_', upos + 1);
          if (upos2 != std::string::npos) {
            id = id.substr(0, upos2);
          }
        }
      }
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[import-map][normalize] vite-deps: %s -> %s", specifier.c_str(), id.c_str());
      }
      return id;
    }
  }

  // Pattern 2: Resolved node_modules path — /node_modules/<pkg>/...
  {
    const std::string nmPrefix = "/node_modules/";
    const std::string nmPrefix2 = "node_modules/";
    std::string sub;
    if (specifier.compare(0, nmPrefix.size(), nmPrefix) == 0)
      sub = specifier.substr(nmPrefix.size());
    else if (specifier.compare(0, nmPrefix2.size(), nmPrefix2) == 0)
      sub = specifier.substr(nmPrefix2.size());

    if (!sub.empty() && sub[0] != '.') {
      if (sub.compare(0, 6, ".vite/") == 0) return "";

      std::string subNoQuery = sub;
      std::string querySuffix;
      auto subQueryPos = sub.find('?');
      if (subQueryPos != std::string::npos) {
        subNoQuery = sub.substr(0, subQueryPos);
        querySuffix = sub.substr(subQueryPos);
      }

      std::string pkgName;
      if (subNoQuery[0] == '@') {
        auto slash1 = subNoQuery.find('/');
        if (slash1 != std::string::npos) {
          auto slash2 = subNoQuery.find('/', slash1 + 1);
          pkgName = (slash2 != std::string::npos) ? subNoQuery.substr(0, slash2) : subNoQuery;
        }
      } else {
        auto slash = subNoQuery.find('/');
        pkgName = (slash != std::string::npos) ? subNoQuery.substr(0, slash) : subNoQuery;
      }
      if (!pkgName.empty()) {
        std::string normalized = pkgName;
        std::string remainder;
        if (subNoQuery.size() > pkgName.size()) {
          remainder = subNoQuery.substr(pkgName.size());
          if (!remainder.empty() && remainder[0] == '/') {
            remainder.erase(0, 1);
          }
        }

        if (!remainder.empty()) {
          bool preserveSubpath = remainder.find('/') != std::string::npos;

          if (!preserveSubpath) {
            const std::string pkgBaseName = pkgName.substr(pkgName.find_last_of('/') + 1);
            std::string withoutExt = remainder;
            auto dot = withoutExt.rfind('.');
            if (dot != std::string::npos) {
              withoutExt = withoutExt.substr(0, dot);
            }
            std::string withoutPlatform = withoutExt;
            for (const char* suffix : {".ios", ".android", ".visionos"}) {
              if (EndsWith(withoutPlatform, suffix)) {
                withoutPlatform = withoutPlatform.substr(0, withoutPlatform.size() - strlen(suffix));
                break;
              }
            }
            const bool isRootLevelMainEntry = withoutPlatform == "index" ||
                                              withoutPlatform == pkgBaseName ||
                                              withoutPlatform.rfind(pkgBaseName + ".", 0) == 0;
            preserveSubpath = !isRootLevelMainEntry;
          }

          if (preserveSubpath) {
            normalized = pkgName + "/" + remainder + querySuffix;
          }
        }

        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[import-map][normalize] node_modules: %s -> %s", specifier.c_str(), normalized.c_str());
        }
        return normalized;
      }
    }
  }

  return "";
}

// Look up a specifier in the import map. Supports both exact matches and
// prefix matches (trailing-slash entries like "solid-js/" that map subpaths).
// Returns the mapped URL or empty string if no match.
static std::string LookupImportMap(const std::string& specifier) {
  std::lock_guard<std::mutex> lock(g_importMapMutex);
  auto it = g_importMap.find(specifier);
  if (it != g_importMap.end()) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[import-map] exact: %s -> %s", specifier.c_str(), it->second.c_str());
    }
    return it->second;
  }
  std::string bestKey;
  std::string bestValue;
  for (const auto& kv : g_importMap) {
    const std::string& key = kv.first;
    if (key.empty() || key.back() != '/') continue;
    if (specifier.size() > key.size() && specifier.compare(0, key.size(), key) == 0) {
      if (key.size() > bestKey.size()) {
        bestKey = key;
        bestValue = kv.second;
      }
    }
  }
  if (!bestKey.empty()) {
    std::string remainder = specifier.substr(bestKey.size());
    std::string resolved = bestValue + remainder;
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[import-map] prefix: %s -> %s (via %s)", specifier.c_str(), resolved.c_str(), bestKey.c_str());
    }
    return resolved;
  }
  return "";
}

// Escape `s` as a single-quoted JS string literal. Returns the literal
// including the surrounding quotes so call sites can splice it directly
// into a generated source string (e.g. `"foo(" + JsStringLiteral(id) + ")"`).
// Handles backslash, single quote, the JS line terminators (\n, \r,
// U+2028, U+2029), and other ASCII control characters via `\xNN`.
static std::string JsStringLiteral(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    out.push_back('\'');
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c == '\\') { out += "\\\\"; ++i; continue; }
        if (c == '\'') { out += "\\'"; ++i; continue; }
        if (c == '\n') { out += "\\n"; ++i; continue; }
        if (c == '\r') { out += "\\r"; ++i; continue; }
        if (c == 0xE2 && i + 2 < s.size() &&
            static_cast<unsigned char>(s[i + 1]) == 0x80 &&
            (static_cast<unsigned char>(s[i + 2]) == 0xA8 ||
             static_cast<unsigned char>(s[i + 2]) == 0xA9)) {
            out += (static_cast<unsigned char>(s[i + 2]) == 0xA8) ? "\\u2028" : "\\u2029";
            i += 3;
            continue;
        }
        if (c < 0x20) {
            char buf[7];
            std::snprintf(buf, sizeof(buf), "\\x%02X", c);
            out += buf;
            ++i;
            continue;
        }
        out.push_back(static_cast<char>(c));
        ++i;
    }
    out.push_back('\'');
    return out;
}

// Helper: returns true if `name` is a valid JS identifier that can appear in
// `export const <name> = ...` without quoting. Conservative check — rejects
// anything that could cause a parse error in the generated ESM wrapper.
static bool IsValidJSIdentifier(const std::string& name) {
  if (name.empty()) return false;
  char first = name[0];
  if (!((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z') ||
        first == '_' || first == '$'))
    return false;
  for (size_t i = 1; i < name.size(); i++) {
    char c = name[i];
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') || c == '_' || c == '$'))
      return false;
  }
  return true;
}

// Create an ESM wrapper that re-exports all named exports from the vendor
// registry. The vendor bootstrap (JS side) populates
// globalThis.__nsVendorRegistry with pre-bundled module namespace objects
// (via `import * as`). This function enumerates the actual property names
// of the vendor module and generates explicit `export const X = __mod['X'];`
// statements so V8's ESM resolution finds every named export.
static v8::MaybeLocal<v8::Module> ResolveFromVendorRegistry(v8::Isolate* isolate,
                                                            v8::Local<v8::Context> context,
                                                            const std::string& vendorId) {
  auto cached = g_vendorModuleCache.find(vendorId);
  if (cached != g_vendorModuleCache.end()) {
    v8::Local<v8::Module> mod = cached->second.Get(isolate);
    if (!mod.IsEmpty() && mod->GetStatus() != v8::Module::kErrored) {
      return mod;
    }
    cached->second.Reset();
    g_vendorModuleCache.erase(cached);
  }

  std::vector<std::string> exportNames;

  v8::TryCatch tc(isolate);
  do {
    v8::Local<v8::Object> global = context->Global();

    v8::Local<v8::Value> regVal;
    if (!global->Get(context, ArgConverter::ConvertToV8String(isolate, "__nsVendorRegistry")).ToLocal(&regVal) ||
        regVal->IsNullOrUndefined()) {
      break;
    }
    v8::Local<v8::Object> registry = regVal.As<v8::Object>();

    v8::Local<v8::Value> getFnVal;
    if (!registry->Get(context, ArgConverter::ConvertToV8String(isolate, "get")).ToLocal(&getFnVal) ||
        !getFnVal->IsFunction()) {
      break;
    }
    v8::Local<v8::Value> getArgs[] = { ArgConverter::ConvertToV8String(isolate, vendorId) };
    v8::Local<v8::Value> modVal;
    if (!getFnVal.As<v8::Function>()->Call(context, registry, 1, getArgs).ToLocal(&modVal) ||
        modVal->IsNullOrUndefined()) {
      break;
    }

    v8::Local<v8::Object> modObj = modVal.As<v8::Object>();
    v8::Local<v8::Array> keys;
    if (!modObj->GetOwnPropertyNames(context).ToLocal(&keys)) {
      break;
    }

    for (uint32_t i = 0; i < keys->Length(); i++) {
      v8::Local<v8::Value> key;
      if (!keys->Get(context, i).ToLocal(&key) || !key->IsString()) continue;
      v8::String::Utf8Value keyUtf8(isolate, key);
      if (!*keyUtf8) continue;
      std::string name(*keyUtf8);
      if (name != "default" && IsValidJSIdentifier(name)) {
        exportNames.push_back(name);
      }
    }
  } while (false);

  if (tc.HasCaught()) {
    tc.Reset();
  }

  std::string moduleKey = "ns-vendor://" + vendorId;
  // Two failure modes are distinguished so the runtime error names the
  // class of problem: registry not yet populated (wrapper evaluated
  // before `installVendorBootstrap()` ran) vs. specifier absent from a
  // populated registry (vendor bundle does not ship this entry).
  // `vendorId` is escaped through `JsStringLiteral` so any character is
  // safe to embed inside the generated JS source.
  const std::string idLiteral = JsStringLiteral(vendorId);
  std::string src =
    "const __reg = globalThis.__nsVendorRegistry;\n"
    "if (!__reg || __reg.size === 0) {\n"
    "  throw new Error('ns-vendor wrapper ' + " + idLiteral +
    " + ' evaluated before __nsVendorRegistry was populated');\n"
    "}\n"
    "const __mod = __reg.get(" + idLiteral + ");\n"
    "if (!__mod) {\n"
    "  throw new Error('ns-vendor specifier ' + " + idLiteral +
    " + ' not in __nsVendorRegistry (' + __reg.size + ' entries)');\n"
    "}\n"
    "export default __mod.default !== undefined ? __mod.default : __mod;\n";

  for (const auto& name : exportNames) {
    src += "export const " + name + " = __mod[" + JsStringLiteral(name) + "];\n";
  }

  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[import-map][vendor] generating wrapper for ns-vendor://%s with %lu named exports",
                vendorId.c_str(), (unsigned long)exportNames.size());
  }

  v8::MaybeLocal<v8::Module> m = CompileModuleForResolveRegisterOnly(isolate, context, src, moduleKey);
  if (!m.IsEmpty()) {
    v8::Local<v8::Module> mod;
    if (m.ToLocal(&mod)) {
      g_vendorModuleCache[vendorId].Reset(isolate, mod);
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[import-map][vendor] resolved ns-vendor://%s", vendorId.c_str());
      }
    }
  }
  return m;
}

// Public: replace the import map with parsed (key → URL) entries. The dev server's
// import-map JSON is parsed by V8 in the caller (HMRSupport.cpp); only the flat
// `imports` table reaches here.
void SetImportMapEntries(const std::vector<std::pair<std::string, std::string>>& entries) {
  std::lock_guard<std::mutex> lock(g_importMapMutex);
  g_importMap.clear();
  for (const auto& kv : entries) {
    if (!kv.first.empty()) {
      g_importMap[kv.first] = kv.second;
    }
  }
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[import-map] loaded %lu entries", (unsigned long)g_importMap.size());
  }
}

void SetVolatilePatterns(const std::vector<std::string>& patterns) {
  std::lock_guard<std::mutex> lock(g_volatilePatternsMutex);
  g_volatilePatterns = patterns;
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[import-map] volatile patterns: %lu", (unsigned long)g_volatilePatterns.size());
  }
}

void CleanupImportMapGlobals() {
  {
    std::lock_guard<std::mutex> lock(g_importMapMutex);
    g_importMap.clear();
  }
  {
    std::lock_guard<std::mutex> lock(g_volatilePatternsMutex);
    g_volatilePatterns.clear();
  }
  for (auto& kv : g_vendorModuleCache) { kv.second.Reset(); }
  g_vendorModuleCache.clear();
  g_modulesInFlight.clear();
}

std::vector<std::string> GetLoadedModuleUrls() {
  std::vector<std::string> urls;
  urls.reserve(g_moduleRegistry.size());
  for (const auto& kv : g_moduleRegistry) {
    if (!kv.first.empty()) urls.push_back(kv.first);
  }
  return urls;
}

void RemoveModuleFromRegistry(const std::string& canonicalKey) {
  const std::string registryKey = CanonicalizeRegistryKey(canonicalKey);
  // Defensive: never wipe a sentinel key.
  if (registryKey == "@" ||
      registryKey.find("__invalid_at__.mjs") != std::string::npos) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][guard] ignore remove for sentinel %s", registryKey.c_str());
    }
    return;
  }

  auto it = g_moduleRegistry.find(registryKey);
  if (it != g_moduleRegistry.end()) {
    bool isHttpKey = StartsWith(registryKey, "http://") || StartsWith(registryKey, "https://");
    if (IsScriptLoadingLogEnabled() && !isHttpKey) {
      DEBUG_WRITE("[resolver] removing stale module %s", registryKey.c_str());
    }
    it->second.Reset();
    g_moduleRegistry.erase(it);
  }
}

size_t InvalidateModules(const std::vector<std::string>& keys) {
  size_t removed = 0;
  std::vector<std::string> urlsToEvict;
  urlsToEvict.reserve(keys.size());
  for (const auto& raw : keys) {
    if (raw.empty()) continue;
    const std::string registryKey = CanonicalizeRegistryKey(raw);
    auto it = g_moduleRegistry.find(registryKey);
    if (it != g_moduleRegistry.end()) {
      it->second.Reset();
      g_moduleRegistry.erase(it);
      ++removed;
    }
    if (StartsWith(registryKey, "http://") || StartsWith(registryKey, "https://")) {
      urlsToEvict.push_back(registryKey);
    }
  }
  if (!urlsToEvict.empty()) {
    EvictHttpModulePrefetchCacheUrls(urlsToEvict);
  }
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[resolver][invalidate] requested=%lu removed=%lu",
                (unsigned long)keys.size(), (unsigned long)removed);
  }
  return removed;
}

v8::MaybeLocal<v8::Module> LoadHttpModuleForUrl(v8::Isolate* isolate,
                                                v8::Local<v8::Context> context,
                                                const std::string& url,
                                                std::string* errorMessage) {
  if (url.empty()) {
    if (errorMessage) *errorMessage = "[http-esm][load] empty URL";
    return v8::MaybeLocal<v8::Module>();
  }
  const std::string registryKey = CanonicalizeRegistryKey(url);

  auto it = g_moduleRegistry.find(registryKey);
  if (it != g_moduleRegistry.end()) {
    return it->second.Get(isolate);
  }

  // Loop-breaker: if we're already fetching this URL inside this isolate
  // (cyclic HTTP import), don't recurse. The caller's outer fetch will
  // populate the registry; on the second pass our cache lookup above will
  // succeed.
  if (g_modulesInFlight.count(registryKey) > 0) {
    if (errorMessage) *errorMessage = "[http-esm][load] cyclic-inflight " + registryKey;
    return v8::MaybeLocal<v8::Module>();
  }
  g_modulesInFlight.insert(registryKey);

  std::string body;
  std::string contentType;
  int status = 0;
  bool ok = HttpFetchText(url, body, contentType, status) && !body.empty();
  g_modulesInFlight.erase(registryKey);

  if (!ok) {
    if (errorMessage) {
      *errorMessage = std::string("[http-esm][load] fetch-fail ") + url +
                      " status=" + std::to_string(status);
    }
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][load][fetch-fail] request=%s key=%s status=%d",
                  url.c_str(), registryKey.c_str(), status);
    }
    return v8::MaybeLocal<v8::Module>();
  }

  v8::MaybeLocal<v8::Module> loaded =
      CompileModuleForResolveRegisterOnly(isolate, context, body, registryKey);
  if (loaded.IsEmpty()) {
    if (errorMessage) {
      *errorMessage = std::string("[http-esm][load] compile-fail ") + url;
    }
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][load][compile-fail] request=%s key=%s bytes=%zu",
                  url.c_str(), registryKey.c_str(), body.size());
    }
    return v8::MaybeLocal<v8::Module>();
  }
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[http-esm][load][ok] request=%s key=%s type=%s bytes=%zu",
                url.c_str(), registryKey.c_str(), contentType.c_str(), body.size());
  }
  return loaded;
}

}  // namespace tns

// Import meta callback to support import.meta.url and import.meta.dirname
void InitializeImportMetaObject(Local<Context> context, Local<Module> module, Local<Object> meta) {
    Isolate* isolate = context->GetIsolate();
    
    // Look up the module path in the global module registry (with safety checks)
    std::string modulePath;
    
    try {
        for (auto& kv : g_moduleRegistry) {
            // Check if Global handle is empty before accessing
            if (kv.second.IsEmpty()) {
                continue;
            }
            
            Local<Module> registered = kv.second.Get(isolate);
            if (!registered.IsEmpty() && registered == module) {
                modulePath = kv.first;
                break;
            }
        }
    } catch (...) {
        DEBUG_WRITE("InitializeImportMetaObject: Exception during module registry lookup, using fallback");
        modulePath = "";  // Will use fallback path
    }
    
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("InitializeImportMetaObject: Module lookup: found path = %s",
                    modulePath.empty() ? "(empty)" : modulePath.c_str());
        DEBUG_WRITE("InitializeImportMetaObject: Registry size: %zu", g_moduleRegistry.size());
    }
    
    // Convert to URL for import.meta.url:
    //   - http(s) keys keep the URL verbatim
    //   - Synthetic-namespace keys (`node:`, `blob:`, `ns-vendor://`,
    //     `optional:`) MUST preserve their identity — wrapping them in
    //     `file://` would make `import.meta.url` decode them as filesystem
    //     paths in user code, which is wrong (they're not files).
    //   - Any other URL-schemed specifier (`<scheme>:` before any `/`)
    //     also passes through verbatim (`tns::HasUrlScheme` check).
    //   - File-system paths get the standard `file://` wrap.
    std::string moduleUrl;
    if (!modulePath.empty()) {
        if (modulePath.rfind("http://", 0) == 0 || modulePath.rfind("https://", 0) == 0) {
            moduleUrl = modulePath;
        } else if (tns::HasUrlScheme(modulePath)) {
            moduleUrl = modulePath;
        } else {
            moduleUrl = "file://" + modulePath;
        }
    } else {
        // Fallback URL if module not found in registry
        moduleUrl = "file:///android_asset/app/";
    }
    
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("InitializeImportMetaObject: Final URL: %s", moduleUrl.c_str());
    }
    
    Local<String> url = ArgConverter::ConvertToV8String(isolate, moduleUrl);
    
    // Set import.meta.url property
    meta->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "url"), url).Check();
    
    // Add import.meta.dirname support (extract directory).
    //
    // For synthetic-namespace keys (node:, blob:, ns-vendor://, optional:)
    // the concept of a "directory" doesn't apply — the module isn't a file
    // on disk. dirname falls back to the full URL so user code that joins
    // paths still produces a recognizable key (and `node:url` /
    // `blob:abc-def` etc. don't accidentally read as filesystem prefixes).
    std::string dirname;
    if (!modulePath.empty()) {
        if (modulePath.rfind("http://", 0) == 0 || modulePath.rfind("https://", 0) == 0) {
            size_t q = modulePath.find('?');
            std::string noQuery = (q == std::string::npos) ? modulePath : modulePath.substr(0, q);
            size_t lastSlash = noQuery.find_last_of('/');
            dirname = (lastSlash == std::string::npos) ? modulePath : noQuery.substr(0, lastSlash);
        } else if (tns::HasUrlScheme(modulePath)) {
            dirname = modulePath;  // synthetic — no real directory
        } else {
            size_t lastSlash = modulePath.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                dirname = modulePath.substr(0, lastSlash);
            } else {
                dirname = "/android_asset/app";  // fallback
            }
        }
    } else {
        dirname = "/android_asset/app";  // fallback
    }
    
    Local<String> dirnameStr = ArgConverter::ConvertToV8String(isolate, dirname);
    
    // Set import.meta.dirname property
    meta->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "dirname"), dirnameStr).Check();

    // Attach import.meta.hot for HMR — debug/dev builds only. In a release
    // build the HMR client and dev-session globals are not installed (see the
    // isDebuggable gate in Runtime::PrepareV8Runtime), so this per-module hot
    // surface would be inert dead weight on every module. Gate it on
    // isDebuggable so production modules carry only import.meta.url/dirname.
    // Standard HMR code always guards with `if (import.meta.hot)`, so leaving
    // it undefined in release is the conventional, safe behavior.
    if (tns::IsDebuggable()) {
        tns::InitializeImportMetaHot(isolate, context, meta, modulePath);
    }
}

// Helper function to check if a file exists and is a regular file
bool IsFile(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return false;
    }
    return (st.st_mode & S_IFMT) == S_IFREG;
}

// Helper function to add extension if missing
std::string WithExtension(const std::string& path, const std::string& ext) {
    if (path.size() >= ext.size() && path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
        return path;
    }
    return path + ext;
}

// Helper function to check if a module is a Node.js built-in (e.g., node:url)
bool IsNodeBuiltinModule(const std::string& spec) {
    return spec.size() > 5 && spec.substr(0, 5) == "node:";
}

// Helper function to get application path (for Android, we'll use a simple approach)
std::string GetApplicationPath() {
    // For Android, use the actual file system path instead of asset path
    // This should match the ApplicationFilesPath + "/app" from Module.java
    JEnv env;
    jstring applicationFilesPath = (jstring) env.CallStaticObjectMethod(ModuleInternal::MODULE_CLASS, ModuleInternal::GET_APPLICATION_FILES_PATH_METHOD_ID);
    std::string path = ArgConverter::jstringToString(applicationFilesPath);
    return path + "/app";
}

// ResolveModuleCallback - Main callback invoked by V8 to resolve import statements
v8::MaybeLocal<v8::Module> ResolveModuleCallback(v8::Local<v8::Context> context,
                                                 v8::Local<v8::String> specifier,
                                                 v8::Local<v8::FixedArray> import_assertions,
                                                 v8::Local<v8::Module> referrer) {
    v8::Isolate* isolate = context->GetIsolate();

    // 1) Convert specifier to std::string
    v8::String::Utf8Value specUtf8(isolate, specifier);
    std::string spec = *specUtf8 ? *specUtf8 : "";
    if (spec.empty()) {
        return v8::MaybeLocal<v8::Module>();
    }

    // Debug logging
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("ResolveModuleCallback: Resolving '%s'", spec.c_str());
    }

    // ── ESM resolver hardening ─────────────────────────────────────────────
    // Heal common bundler-rewrite anomalies BEFORE any registry lookup so
    // identity-mismatched keys never enter `g_moduleRegistry`.
    //
    //   1. A lone "@" (bundler dropped the package id) → rewrite to a
    //      sentinel string that downstream resolvers ignore.
    //   2. "@/<rest>" (root-absolute alias the dev server didn't expand)
    //      → strip the prefix so the path lookup operates on "/<rest>".
    //   3. Malformed `http:/<path>` (one slash, often from Vite stripping
    //      the second slash through string ops) → re-insert.
    if (spec == "@") {
        // Sentinel — never matches a real module. Synthesize a clear
        // failure so the user's error trace anchors to the bad import.
        if (tns::IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[resolver][guard] bare-@ specifier; returning empty");
        }
        return v8::MaybeLocal<v8::Module>();
    }
    if (spec.size() >= 2 && spec[0] == '@' && spec[1] == '/') {
        std::string rewritten = spec.substr(1);  // "@/foo" → "/foo"
        if (tns::IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[resolver][guard] rewrite @/ -> / for spec=%s -> %s",
                        spec.c_str(), rewritten.c_str());
        }
        spec = rewritten;
    }
    if (spec.rfind("http:/", 0) == 0 && spec.rfind("http://", 0) != 0) {
        spec.insert(5, "/");
    } else if (spec.rfind("https:/", 0) == 0 && spec.rfind("https://", 0) != 0) {
        spec.insert(6, "/");
    }

    // ── Import-map lookup (bare specifiers only) ──────────────────────────
    // Bare specifiers (no `.`, `/`, scheme) are mapped through the dev
    // server's import map before anything else. The map can resolve to:
    //   - `ns-vendor://<id>` → SyntheticModule via vendor registry
    //   - `http(s)://...`    → HTTP loader path below
    //   - `<path>`           → falls through to filesystem candidate scan
    // Vite-rewritten `/node_modules/...` specifiers are normalized via
    // `NormalizeViteSpecifier` first so the import-map key matches.
    if (!spec.empty() && spec[0] != '.' && spec[0] != '/' &&
        spec.find("://") == std::string::npos && !tns::HasUrlScheme(spec)) {
        std::string lookupKey = spec;
        std::string vNorm = tns::NormalizeViteSpecifier(spec);
        if (!vNorm.empty()) lookupKey = vNorm;
        std::string mapped = tns::LookupImportMap(lookupKey);
        if (!mapped.empty()) {
            if (tns::IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[import-map][hit] %s -> %s", spec.c_str(), mapped.c_str());
            }
            spec = mapped;
        }
    } else if (!spec.empty() && spec[0] == '/' &&
               spec.find("://") == std::string::npos &&
               spec.find("/node_modules/") != std::string::npos) {
        // Root-absolute node_modules path — try import-map after normalization.
        std::string vNorm = tns::NormalizeViteSpecifier(spec);
        if (!vNorm.empty()) {
            std::string mapped = tns::LookupImportMap(vNorm);
            if (!mapped.empty()) {
                if (tns::IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("[import-map][hit-root-abs] %s -> %s (via %s)",
                                spec.c_str(), mapped.c_str(), vNorm.c_str());
                }
                spec = mapped;
            }
        }
    }

    // ── Synthetic-namespace identity preservation ─────────────────────────
    // ns-vendor:// / optional: / node: / blob: are NOT filesystem paths.
    // Resolve them via dedicated paths instead of falling through to the
    // candidate-scan logic below (which would try to stat them as files).
    if (spec.rfind("ns-vendor://", 0) == 0) {
        std::string vendorId = spec.substr(strlen("ns-vendor://"));
        v8::Local<v8::Module> vendorMod;
        if (tns::ResolveFromVendorRegistry(isolate, context, vendorId).ToLocal(&vendorMod)) {
            return v8::MaybeLocal<v8::Module>(vendorMod);
        }
        // Vendor registry doesn't have it — throw a clear "not found" so the
        // import rejects with a useful message instead of a stat-as-file miss.
        std::string msg = "Vendor module not found: " + spec;
        isolate->ThrowException(v8::Exception::Error(
            ArgConverter::ConvertToV8String(isolate, msg)));
        return v8::MaybeLocal<v8::Module>();
    }
    if (spec.rfind("blob:", 0) == 0) {
        // Blob URLs are issued by URL.createObjectURL — the corresponding
        // module should already be in g_moduleRegistry under the exact blob
        // key (loader wrote it on creation). Look up verbatim; no
        // normalization (the random ID is the identity).
        auto it = g_moduleRegistry.find(spec);
        if (it != g_moduleRegistry.end()) {
            return v8::MaybeLocal<v8::Module>(it->second.Get(isolate));
        }
        std::string msg = "Blob module not found: " + spec;
        isolate->ThrowException(v8::Exception::Error(
            ArgConverter::ConvertToV8String(isolate, msg)));
        return v8::MaybeLocal<v8::Module>();
    }
    if (spec.rfind("optional:", 0) == 0) {
        // Optional-module sentinel — return empty so V8 throws the standard
        // ESM resolve failure (the caller is responsible for swallowing).
        if (tns::IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[resolver][optional] not found: %s", spec.c_str());
        }
        return v8::MaybeLocal<v8::Module>();
    }

    // Attempt to resolve relative or root-absolute specifiers against an HTTP referrer URL
    std::string referrerPath;
    for (auto& kv : g_moduleRegistry) {
        v8::Local<v8::Module> registered = kv.second.Get(isolate);
        if (!registered.IsEmpty() && registered == referrer) {
            referrerPath = kv.first;
            break;
        }
    }
    bool specIsRelative = !spec.empty() && spec[0] == '.';
    bool specIsRootAbs = !spec.empty() && spec[0] == '/';
    auto startsWithHttp = [](const std::string& s) -> bool {
        return s.rfind("http://", 0) == 0 || s.rfind("https://", 0) == 0;
    };
    if (!startsWithHttp(spec) && (specIsRelative || specIsRootAbs)) {
        if (!referrerPath.empty() && startsWithHttp(referrerPath)) {
            std::string resolved = tns::ResolveImportSpecifierAgainstUrl(spec, referrerPath);
            if (!resolved.empty()) {
                if (IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("ResolveModuleCallback: HTTP-relative resolved '%s' + '%s' -> '%s'",
                                referrerPath.c_str(), spec.c_str(), resolved.c_str());
                }
                spec = resolved;
            }
        } else if (specIsRootAbs) {
            // Fallback: use global __NS_HTTP_ORIGIN__ if present to anchor root-absolute specs
            v8::Local<v8::String> key = ArgConverter::ConvertToV8String(isolate, "__NS_HTTP_ORIGIN__");
            v8::Local<v8::Object> global = context->Global();
            v8::MaybeLocal<v8::Value> maybeOriginVal = global->Get(context, key);
            v8::Local<v8::Value> originVal;
            if (!maybeOriginVal.IsEmpty() && maybeOriginVal.ToLocal(&originVal) && originVal->IsString()) {
                v8::String::Utf8Value o8(isolate, originVal);
                std::string origin = *o8 ? *o8 : "";
                if (!origin.empty() && (origin.rfind("http://", 0) == 0 || origin.rfind("https://", 0) == 0)) {
                    std::string refBase = origin;
                    if (refBase.back() != '/') refBase += '/';
                    std::string resolved = tns::ResolveImportSpecifierAgainstUrl(spec, refBase);
                    if (!resolved.empty()) {
                        if (IsScriptLoadingLogEnabled()) {
                            DEBUG_WRITE("[http-esm][http-origin][fallback] origin=%s spec=%s -> %s", refBase.c_str(), spec.c_str(), resolved.c_str());
                        }
                        spec = resolved;
                    }
                }
            }
        }
    }

    // HTTP(S) ESM support: resolve, fetch and compile from dev server
    // Security: HttpFetchText gates remote module access centrally.
    if (spec.rfind("http://", 0) == 0 || spec.rfind("https://", 0) == 0) {
        std::string canonical = tns::CanonicalizeHttpUrlKey(spec);
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][resolve] spec=%s canonical=%s", spec.c_str(), canonical.c_str());
        }
        auto it = g_moduleRegistry.find(canonical);
        if (it != g_moduleRegistry.end()) {
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][cache] hit %s", canonical.c_str());
            }
            return v8::MaybeLocal<v8::Module>(it->second.Get(isolate));
        }

        std::string body, ct;
        int status = 0;
        if (!tns::HttpFetchText(spec, body, ct, status)) {
            // Pull any JNI-captured reason BEFORE composing the error so
            // the user sees `connect failed: ECONNREFUSED` (or whatever)
            // alongside the bare `status=0` instead of having to dig
            // through logcat.
            std::string reason = tns::TakeLastHttpFetchErrorReason();
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][fetch][fail] url=%s status=%d reason=%s",
                            spec.c_str(), status, reason.c_str());
            }
            std::string msg = std::string("Failed to fetch ") + spec + ", status=" + std::to_string(status);
            if (!reason.empty()) {
                msg += ", " + reason;
            }
            isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
            return v8::MaybeLocal<v8::Module>();
        }
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][fetch][ok] url=%s status=%d bytes=%lu ct=%s", spec.c_str(), status, (unsigned long)body.size(), ct.c_str());
        }

        v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, body);
        v8::Local<v8::String> urlString = ArgConverter::ConvertToV8String(isolate, canonical);
        v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true);
        v8::ScriptCompiler::Source src(sourceText, origin);
        v8::Local<v8::Module> mod;
        {
            v8::TryCatch tc(isolate);
            if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
                LogHttpCompileDiagnostics(isolate, context, canonical, body, tc);
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "HTTP module compile failed")));
                return v8::MaybeLocal<v8::Module>();
            }
        }
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][compile][ok] %s bytes=%lu", canonical.c_str(), (unsigned long)body.size());
        }
        // Register before instantiation to allow cyclic imports to resolve to same instance
        g_moduleRegistry[canonical].Reset(isolate, mod);
        // Do not evaluate here; allow V8 to handle instantiation/evaluation in importer context.
        // Instantiate proactively if desired (safe), but not required.
        // if (mod->GetStatus() == v8::Module::kUninstantiated) {
        //     if (!mod->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
        //         g_moduleRegistry.erase(canonical);
        //         return v8::MaybeLocal<v8::Module>();
        //     }
        // }
        // Let V8 evaluate during importer evaluation. Returning compiled module is fine.
        return v8::MaybeLocal<v8::Module>(mod);
    }

    // 2) Find which filepath the referrer was compiled under (local filesystem case)
    // referrerPath may already be set above; leave as-is if found.
    if (referrerPath.empty()) {
        for (auto& kv : g_moduleRegistry) {
            v8::Local<v8::Module> registered = kv.second.Get(isolate);
            if (registered == referrer) {
                referrerPath = kv.first;
                break;
            }
        }
    }

    // If we couldn't identify the referrer and the specifier is relative, 
    // assume the base directory is the application root
    bool specIsRelativeFs = !spec.empty() && spec[0] == '.';
    if (referrerPath.empty() && specIsRelativeFs) {
        referrerPath = GetApplicationPath() + "/index.mjs";  // Default referrer
    }

    // 3) Compute base directory from referrer path
    size_t slash = referrerPath.find_last_of("/\\");
    std::string baseDir = slash == std::string::npos ? "" : referrerPath.substr(0, slash + 1);

    // 4) Build candidate paths for resolution
    std::vector<std::string> candidateBases;
    std::string appPath = GetApplicationPath();

    if (!spec.empty() && spec[0] == '.') {
        // Relative import (./ or ../)
        std::string cleanSpec = spec.substr(0, 2) == "./" ? spec.substr(2) : spec;
        std::string candidate = baseDir + cleanSpec;
        candidateBases.push_back(candidate);
    if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ResolveModuleCallback: Relative import: '%s' + '%s' -> '%s'",
                       baseDir.c_str(), cleanSpec.c_str(), candidate.c_str());
        }
    } else if (spec.size() > 7 && spec.substr(0, 7) == "file://") {
        // Absolute file URL
        std::string tail = spec.substr(7);  // strip file://
        if (tail.empty() || tail[0] != '/') {
            tail = "/" + tail;
        }

        // Map common virtual roots to the real appPath
        const std::string appVirtualRoot = "/app/";                      // e.g. file:///app/foo.mjs
        const std::string androidAssetAppRoot = "/android_asset/app/";   // e.g. file:///android_asset/app/foo.mjs

        std::string candidate;
        if (tail.rfind(appVirtualRoot, 0) == 0) {
            // Drop the leading "/app/" and prepend real appPath
            candidate = appPath + "/" + tail.substr(appVirtualRoot.size());
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// to appPath mapping: '%s' -> '%s'", tail.c_str(), candidate.c_str());
            }
        } else if (tail.rfind(androidAssetAppRoot, 0) == 0) {
            // Replace "/android_asset/app/" with the real appPath
            candidate = appPath + "/" + tail.substr(androidAssetAppRoot.size());
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// android_asset mapping: '%s' -> '%s'", tail.c_str(), candidate.c_str());
            }
        } else if (tail.rfind(appPath, 0) == 0) {
            // Already an absolute on-disk path to the app folder
            candidate = tail;
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// absolute path preserved: '%s'", candidate.c_str());
            }
        } else {
            // Fallback: treat as absolute on-disk path
            candidate = tail;
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// generic absolute: '%s'", candidate.c_str());
            }
        }

        candidateBases.push_back(candidate);
    } else if (!spec.empty() && spec[0] == '~') {
        // Alias to application root using ~/path
        std::string tail = spec.size() >= 2 && spec[1] == '/' ? spec.substr(2) : spec.substr(1);
        std::string candidate = appPath + "/" + tail;
        candidateBases.push_back(candidate);
    } else if (!spec.empty() && spec[0] == '/') {
        // Absolute path within the bundle
        candidateBases.push_back(appPath + spec);
    } else {
        // Bare specifier – resolve relative to the application root
        std::string candidate = appPath + "/" + spec;
        candidateBases.push_back(candidate);

        // Try converting underscores to slashes (bundler heuristic)
        std::string withSlashes = spec;
        std::replace(withSlashes.begin(), withSlashes.end(), '_', '/');
        std::string candidateSlashes = appPath + "/" + withSlashes;
        if (candidateSlashes != candidate) {
            candidateBases.push_back(candidateSlashes);
        }
    }

    // 5) Attempt to resolve to an actual file
    std::string absPath;
    bool found = false;

    for (const std::string& baseCandidate : candidateBases) {
        absPath = baseCandidate;

        // Check if file exists as-is
        if (IsFile(absPath)) {
            found = true;
            break;
        }

        // Try adding extensions
        const char* exts[] = {".mjs", ".js"};
        for (const char* ext : exts) {
            std::string candidate = WithExtension(absPath, ext);
            if (IsFile(candidate)) {
                absPath = candidate;
                found = true;
                break;
            }
        }
        if (found) break;

        // Try index files if path is a directory
        const char* indexExts[] = {"/index.mjs", "/index.js"};
        for (const char* idx : indexExts) {
            std::string candidate = absPath + idx;
            if (IsFile(candidate)) {
                absPath = candidate;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    // 6) Handle special cases if file not found
    if (!found) {
        // Check for Node.js built-in modules
        if (IsNodeBuiltinModule(spec)) {
            std::string builtinName = spec.substr(5);  // Remove "node:" prefix
            
            // Create polyfill content for Node.js built-in modules
            std::string polyfillContent;
            
            if (builtinName == "url") {
                // Create a polyfill for node:url with fileURLToPath
                polyfillContent = "// Polyfill for node:url\n"
                                "export function fileURLToPath(url) {\n"
                                "  if (typeof url === 'string') {\n"
                                "    if (url.startsWith('file://')) {\n"
                                "      return decodeURIComponent(url.slice(7));\n"
                                "    }\n"
                                "    return url;\n"
                                "  }\n"
                                "  if (url && typeof url.href === 'string') {\n"
                                "    return fileURLToPath(url.href);\n"
                                "  }\n"
                                "  throw new Error('Invalid URL');\n"
                                "}\n"
                                "\n"
                                "export function pathToFileURL(path) {\n"
                                "  const encoded = encodeURIComponent(path).replace(/%2F/g, '/');\n"
                                "  return new URL('file://' + encoded);\n"
                                "}\n";
            } else if (builtinName == "module") {
                // Create a polyfill for node:module with createRequire
                polyfillContent = "// Polyfill for node:module\n"
                                "export function createRequire(filename) {\n"
                                "  // Return the global require function\n"
                                "  // In NativeScript, require is globally available\n"
                                "  if (typeof require === 'function') {\n"
                                "    return require;\n"
                                "  }\n"
                                "  \n"
                                "  // Fallback: create a basic require function\n"
                                "  return function(id) {\n"
                                "    throw new Error('Module ' + id + ' not found. NativeScript require() not available.');\n"
                                "  };\n"
                                "}\n"
                                "\n"
                                "// Export as default as well for compatibility\n"
                                "export default { createRequire };\n";
            } else if (builtinName == "path") {
                // Create a polyfill for node:path
                polyfillContent = "// Polyfill for node:path\n"
                                "export const sep = '/';\n"
                                "export const delimiter = ':';\n"
                                "\n"
                                "export function basename(path, ext) {\n"
                                "  const name = path.split('/').pop() || '';\n"
                                "  return ext && name.endsWith(ext) ? name.slice(0, -ext.length) : name;\n"
                                "}\n"
                                "\n"
                                "export function dirname(path) {\n"
                                "  const parts = path.split('/');\n"
                                "  return parts.slice(0, -1).join('/') || '/';\n"
                                "}\n"
                                "\n"
                                "export function extname(path) {\n"
                                "  const name = basename(path);\n"
                                "  const dot = name.lastIndexOf('.');\n"
                                "  return dot > 0 ? name.slice(dot) : '';\n"
                                "}\n"
                                "\n"
                                "export function join(...paths) {\n"
                                "  return paths.filter(Boolean).join('/').replace(/\\/+/g, '/');\n"
                                "}\n"
                                "\n"
                                "export function resolve(...paths) {\n"
                                "  let resolved = '';\n"
                                "  for (let path of paths) {\n"
                                "    if (path.startsWith('/')) {\n"
                                "      resolved = path;\n"
                                "    } else {\n"
                                "      resolved = join(resolved, path);\n"
                                "    }\n"
                                "  }\n"
                                "  return resolved || '/';\n"
                                "}\n"
                                "\n"
                                "export function isAbsolute(path) {\n"
                                "  return path.startsWith('/');\n"
                                "}\n"
                                "\n"
                                "export default { basename, dirname, extname, join, resolve, isAbsolute, sep, delimiter };\n";
            } else if (builtinName == "fs") {
                // Create a basic polyfill for node:fs
                polyfillContent = "// Polyfill for node:fs\n"
                                "console.warn('Node.js fs module is not supported in NativeScript. Use @nativescript/core File APIs instead.');\n"
                                "\n"
                                "export function readFileSync() {\n"
                                "  throw new Error('fs.readFileSync is not supported in NativeScript. Use @nativescript/core File APIs.');\n"
                                "}\n"
                                "\n"
                                "export function writeFileSync() {\n"
                                "  throw new Error('fs.writeFileSync is not supported in NativeScript. Use @nativescript/core File APIs.');\n"
                                "}\n"
                                "\n"
                                "export function existsSync() {\n"
                                "  throw new Error('fs.existsSync is not supported in NativeScript. Use @nativescript/core File APIs.');\n"
                                "}\n"
                                "\n"
                                "export default { readFileSync, writeFileSync, existsSync };\n";
            } else {
                // Generic polyfill for other Node.js built-in modules
                polyfillContent = "// Polyfill for node:" + builtinName + "\n"
                                "console.warn('Node.js built-in module \\'node:" + builtinName + "\\' is not fully supported in NativeScript');\n"
                                "export default {};\n";
            }
            
            // Create module source and compile it in-memory
            v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, polyfillContent);
            
            // Build URL for stack traces
            std::string moduleUrl = "node:" + builtinName;
            v8::Local<v8::String> urlString = ArgConverter::ConvertToV8String(isolate, moduleUrl);
            
            v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true /* is_module */);
            v8::ScriptCompiler::Source src(sourceText, origin);
            
            v8::Local<v8::Module> polyfillModule;
            if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&polyfillModule)) {
                std::string msg = "Failed to compile polyfill for: " + spec;
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
                return v8::MaybeLocal<v8::Module>();
            }
            
            // Store in registry before instantiation
            g_moduleRegistry[spec].Reset(isolate, polyfillModule);
            
            // Instantiate the module
            if (!polyfillModule->InstantiateModule(context, ResolveModuleCallback).FromMaybe(false)) {
                g_moduleRegistry.erase(spec);
                std::string msg = "Failed to instantiate polyfill for: " + spec;
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
                return v8::MaybeLocal<v8::Module>();
            }
            
            // Evaluate the module
            v8::MaybeLocal<v8::Value> evalResult = polyfillModule->Evaluate(context);
            if (evalResult.IsEmpty()) {
                g_moduleRegistry.erase(spec);
                std::string msg = "Failed to evaluate polyfill for: " + spec;
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
                return v8::MaybeLocal<v8::Module>();
            }
            
            return v8::MaybeLocal<v8::Module>(polyfillModule);
            
        } else if (tns::ModuleInternal::IsLikelyOptionalModule(spec)) {
            // For optional modules, create a placeholder
            std::string msg = "Optional module not found: " + spec;
            DEBUG_WRITE("ResolveModuleCallback: %s", msg.c_str());
            // Return empty to indicate module not found gracefully
            return v8::MaybeLocal<v8::Module>();
        } else {
            // Regular module not found
            std::string msg = "Cannot find module " + spec + " (tried " + absPath + ")";
            isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
            return v8::MaybeLocal<v8::Module>();
        }
    }

    // 7) Handle JSON modules
    if (absPath.size() >= 5 && absPath.compare(absPath.size() - 5, 5, ".json") == 0) {
    if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ResolveModuleCallback: Handling JSON module '%s'", absPath.c_str());
        }

        // Read JSON file content
        std::string jsonText = Runtime::GetRuntime(isolate)->ReadFileText(absPath);

        // Create ES module that exports the JSON as default
        std::string moduleSource = "export default " + jsonText + ";";

        v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, moduleSource);
        std::string url = "file://" + absPath;

        v8::Local<v8::String> urlString;
        if (!v8::String::NewFromUtf8(isolate, url.c_str(), v8::NewStringType::kNormal).ToLocal(&urlString)) {
            isolate->ThrowException(v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, "Failed to create URL string for JSON module")));
            return v8::MaybeLocal<v8::Module>();
        }

        v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false,
                                false, true /* is_module */);

        v8::ScriptCompiler::Source src(sourceText, origin);

        v8::Local<v8::Module> jsonModule;
        if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&jsonModule)) {
            isolate->ThrowException(v8::Exception::SyntaxError(
                ArgConverter::ConvertToV8String(isolate, "Failed to compile JSON module")));
            return v8::MaybeLocal<v8::Module>();
        }

        // Instantiate and evaluate the JSON module
        if (!jsonModule->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
            return v8::MaybeLocal<v8::Module>();
        }

        v8::MaybeLocal<v8::Value> evalResult = jsonModule->Evaluate(context);
        if (evalResult.IsEmpty()) {
            return v8::MaybeLocal<v8::Module>();
        }

        // Store in registry with safe handle management
        auto it = g_moduleRegistry.find(absPath);
        if (it != g_moduleRegistry.end()) {
            it->second.Reset();
        }
        g_moduleRegistry[absPath].Reset(isolate, jsonModule);
        return v8::MaybeLocal<v8::Module>(jsonModule);
    }

    // 8) Check if we've already compiled this module
    auto it = g_moduleRegistry.find(absPath);
    if (it != g_moduleRegistry.end()) {
    if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ResolveModuleCallback: Found cached module '%s'", absPath.c_str());
        }
        return v8::MaybeLocal<v8::Module>(it->second.Get(isolate));
    }

    // 9) Compile and register the new module
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("ResolveModuleCallback: Compiling new module '%s'", absPath.c_str());
    }
    try {
        // Use our existing LoadESModule function to compile the module
        tns::ModuleInternal::LoadESModule(isolate, absPath);
    } catch (NativeScriptException& ex) {
        DEBUG_WRITE("ResolveModuleCallback: Failed to compile module '%s'", absPath.c_str());
        ex.ReThrowToV8();
        return v8::MaybeLocal<v8::Module>();
    }

    // LoadESModule should have added it to g_moduleRegistry
    auto it2 = g_moduleRegistry.find(absPath);
    if (it2 == g_moduleRegistry.end()) {
        // Something went wrong
        std::string msg = "Failed to register compiled module: " + absPath;
        isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
        return v8::MaybeLocal<v8::Module>();
    }

    return v8::MaybeLocal<v8::Module>(it2->second.Get(isolate));
}

// Dynamic import() host callback
v8::MaybeLocal<v8::Promise> ImportModuleDynamicallyCallback(
    v8::Local<v8::Context> context, v8::Local<v8::Data> host_defined_options,
    v8::Local<v8::Value> resource_name, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> import_assertions) {
    v8::Isolate* isolate = context->GetIsolate();
    
    // Convert specifier to std::string for logging
    v8::String::Utf8Value specUtf8(isolate, specifier);
    std::string spec = *specUtf8 ? *specUtf8 : "";
    
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("ImportModuleDynamicallyCallback: Dynamic import for '%s'", spec.c_str());
    }
    
    v8::EscapableHandleScope scope(isolate);

    // Create a Promise resolver we'll resolve/reject synchronously for now.
    v8::Local<v8::Promise::Resolver> resolver;
    if (!v8::Promise::Resolver::New(context).ToLocal(&resolver)) {
        // Failed to create resolver, return empty promise
        return v8::MaybeLocal<v8::Promise>();
    }

    // Resolve relative or root-absolute dynamic imports against the referrer's URL when provided
    auto isHttpLike = [](const std::string& s) -> bool {
        return s.rfind("http://", 0) == 0 || s.rfind("https://", 0) == 0;
    };
    bool specIsRelative = !spec.empty() && spec[0] == '.';
    bool specIsRootAbs = !spec.empty() && spec[0] == '/';
    std::string referrerUrl;
    if (!resource_name.IsEmpty() && resource_name->IsString()) {
        v8::String::Utf8Value r8(isolate, resource_name);
        referrerUrl = *r8 ? *r8 : "";
    }
    if ((specIsRelative || specIsRootAbs) && isHttpLike(referrerUrl)) {
        std::string resolved = tns::ResolveImportSpecifierAgainstUrl(spec, referrerUrl);
        if (!resolved.empty()) {
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][http-rel] base=%s spec=%s -> %s", referrerUrl.c_str(), spec.c_str(), resolved.c_str());
            }
            spec = resolved;
        } else if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][dyn][http-rel][skip] base=%s spec=%s", referrerUrl.c_str(), spec.c_str());
        }
    }

    // Blob URL dynamic import — synthetic, must preserve identity (no
    // canonicalization). The HTTP/file machinery below would mis-handle a
    // `blob:` key as a fetch target. Caller already registered the module
    // under the exact blob key when URL.createObjectURL was called.
    if (spec.rfind("blob:", 0) == 0) {
        auto it = g_moduleRegistry.find(spec);
        if (it == g_moduleRegistry.end()) {
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[dyn-import][blob][miss] %s", spec.c_str());
            }
            resolver->Reject(context,
                v8::Exception::Error(ArgConverter::ConvertToV8String(
                    isolate, std::string("Blob module not found: ") + spec))).Check();
            return scope.Escape(resolver->GetPromise());
        }
        v8::Local<v8::Module> blobMod = it->second.Get(isolate);
        if (blobMod->GetStatus() == v8::Module::kUninstantiated) {
            if (!blobMod->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
                resolver->Reject(context,
                    v8::Exception::Error(ArgConverter::ConvertToV8String(
                        isolate, "Blob module instantiate failed"))).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }
        if (blobMod->GetStatus() != v8::Module::kEvaluated) {
            if (blobMod->Evaluate(context).IsEmpty()) {
                resolver->Reject(context,
                    v8::Exception::Error(ArgConverter::ConvertToV8String(
                        isolate, "Blob module evaluation failed"))).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }
        resolver->Resolve(context, blobMod->GetModuleNamespace()).Check();
        return scope.Escape(resolver->GetPromise());
    }

    // Handle HTTP(S) dynamic import directly
    // Security: HttpFetchText gates remote module access centrally.
    if (!spec.empty() && isHttpLike(spec)) {
        std::string canonical = tns::CanonicalizeHttpUrlKey(spec);
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][dyn][resolve] spec=%s canonical=%s", spec.c_str(), canonical.c_str());
        }
        v8::Local<v8::Module> mod;
        auto it = g_moduleRegistry.find(canonical);
        if (it != g_moduleRegistry.end()) {
            mod = it->second.Get(isolate);
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][http-cache hit] %s", canonical.c_str());
            }
        } else {
            std::string body, ct; int status = 0;
            if (!tns::HttpFetchText(spec, body, ct, status)) {
                std::string reason = tns::TakeLastHttpFetchErrorReason();
                if (IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("[http-esm][dyn][fetch][fail] url=%s status=%d reason=%s",
                                spec.c_str(), status, reason.c_str());
                }
                std::string rejMsg = std::string("Failed to fetch ") + spec + ", status=" + std::to_string(status);
                if (!reason.empty()) {
                    rejMsg += ", " + reason;
                }
                resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, rejMsg))).Check();
                return scope.Escape(resolver->GetPromise());
            }
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][fetch][ok] url=%s status=%d bytes=%lu ct=%s", spec.c_str(), status, (unsigned long)body.size(), ct.c_str());
            }
            v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, body);
            v8::Local<v8::String> urlString = ArgConverter::ConvertToV8String(isolate, canonical);
            v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true);
            v8::ScriptCompiler::Source src(sourceText, origin);
            {
                v8::TryCatch tc(isolate);
                if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
                    LogHttpCompileDiagnostics(isolate, context, canonical, body, tc);
                    resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "HTTP module compile failed"))).Check();
                    return scope.Escape(resolver->GetPromise());
                }
            }
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][compile][ok] %s bytes=%lu", canonical.c_str(), (unsigned long)body.size());
            }
            g_moduleRegistry[canonical].Reset(isolate, mod);
        }
        if (mod->GetStatus() == v8::Module::kUninstantiated) {
            if (!mod->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
                resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Instantiate failed"))).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }
        if (mod->GetStatus() != v8::Module::kEvaluated) {
            if (mod->Evaluate(context).IsEmpty()) {
                resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Evaluation failed"))).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }
        resolver->Resolve(context, mod->GetModuleNamespace()).Check();
        return scope.Escape(resolver->GetPromise());
    }

    // Re-use the static resolver to locate / compile the module for non-HTTP cases.
    try {
        // Pass empty referrer since this V8 version doesn't expose GetModule() on
        // ScriptOrModule. The resolver will fall back to absolute-path heuristics.
        v8::Local<v8::Module> refMod;

        v8::MaybeLocal<v8::Module> maybeModule =
            ResolveModuleCallback(context, specifier, import_assertions, refMod);

        v8::Local<v8::Module> module;
        if (!maybeModule.ToLocal(&module)) {
            // Resolution failed; reject to avoid leaving a pending Promise (white screen)
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ImportModuleDynamicallyCallback: Resolution failed for '%s'", spec.c_str());
            }
            v8::Local<v8::Value> ex = v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, std::string("Failed to resolve module: ") + spec));
            resolver->Reject(context, ex).Check();
            return scope.Escape(resolver->GetPromise());
        }

        // If not yet instantiated/evaluated, do it now
        if (module->GetStatus() == v8::Module::kUninstantiated) {
            if (!module->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
                if (IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("ImportModuleDynamicallyCallback: Instantiate failed for '%s'", spec.c_str());
                }
                resolver
                    ->Reject(context,
                             v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Failed to instantiate module")))
                    .Check();
                return scope.Escape(resolver->GetPromise());
            }
        }

        if (module->GetStatus() != v8::Module::kEvaluated) {
            if (module->Evaluate(context).IsEmpty()) {
                if (IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("ImportModuleDynamicallyCallback: Evaluation failed for '%s'", spec.c_str());
                }
                v8::Local<v8::Value> ex =
                    v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Evaluation failed"));
                resolver->Reject(context, ex).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }

        resolver->Resolve(context, module->GetModuleNamespace()).Check();
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ImportModuleDynamicallyCallback: Successfully resolved '%s'", spec.c_str());
        }
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ImportModuleDynamicallyCallback: Native exception for '%s'", spec.c_str());
        }
        resolver
            ->Reject(context, v8::Exception::Error(
                                  ArgConverter::ConvertToV8String(isolate, "Native error during dynamic import")))
            .Check();
    }

    return scope.Escape(resolver->GetPromise());
}

// ModuleInternalCallbacks.cpp
#include "ModuleInternalCallbacks.h"

#include <android/looper.h>
#include <sys/stat.h>
#include <v8.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <time.h>
#include <vector>

#include "ArgConverter.h"
#include "Constants.h"
#include "HttpLoader.h"
#include "JEnv.h"
#include "ModuleInternal.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "NsBuiltinModules.h"
#include "Runtime.h"
#include "Util.h"
#include "robin_hood.h"

using namespace v8;
using namespace std;
using namespace tns;

namespace tns {

// ─────────────────────────────────────────────────────────────
// Small string helpers (kept file-local — used everywhere below).

static inline bool StartsWith(const std::string& s, const char* prefix) {
  size_t n = strlen(prefix);
  return s.size() >= n && s.compare(0, n, prefix) == 0;
}

static inline bool EndsWith(const std::string& value, const std::string& suffix) {
  if (suffix.size() > value.size()) return false;
  return std::equal(suffix.rbegin(), suffix.rend(), value.rbegin());
}

// Node.js built-in namespace check (node:url, node:module, node:path, ...).
static bool IsNodeBuiltinModule(const std::string& moduleName) {
  return moduleName.rfind("node:", 0) == 0;
}

// Filesystem: `path` names an existing regular file.
static bool IsFile(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) != 0) {
    return false;
  }
  return (st.st_mode & S_IFMT) == S_IFREG;
}

// Append `ext` if `path` doesn't already carry it.
static std::string WithExtension(const std::string& path, const std::string& ext) {
  if (path.size() >= ext.size() &&
      path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
    return path;
  }
  return path + ext;
}

// Application filesystem root for on-disk .mjs/.js resolution.
// Mirrors Module.java's getApplicationFilesPath + "/app". Cached after first
// JNI call — the value is process-stable, and re-entering JNI on every
// resolver hit would add avoidable overhead to hot module-graph walks.
static std::string GetApplicationPath() {
  static std::string cached;
  static std::once_flag flag;
  std::call_once(flag, []() {
    JEnv env;
    jstring applicationFilesPath = (jstring)env.CallStaticObjectMethod(
        ModuleInternal::MODULE_CLASS,
        ModuleInternal::GET_APPLICATION_FILES_PATH_METHOD_ID);
    if (applicationFilesPath != nullptr) {
      cached = ArgConverter::jstringToString(applicationFilesPath) + "/app";
    }
  });
  return cached;
}

// Collapse "." and ".." segments, preserving a leading "/".
static std::string NormalizeDotSegments(const std::string& path) {
  std::vector<std::string> stack;
  bool absolute = !path.empty() && path[0] == '/';
  size_t i = 0;
  while (i <= path.size()) {
    size_t j = path.find('/', i);
    std::string seg = (j == std::string::npos) ? path.substr(i) : path.substr(i, j - i);
    if (seg.empty() || seg == ".") {
      // skip
    } else if (seg == "..") {
      if (!stack.empty()) stack.pop_back();
    } else {
      stack.push_back(std::move(seg));
    }
    if (j == std::string::npos) break;
    i = j + 1;
  }
  std::string norm = absolute ? "/" : std::string();
  for (size_t k = 0; k < stack.size(); k++) {
    if (k > 0) norm += "/";
    norm += stack[k];
  }
  return norm;
}

// Normalize a filesystem path: collapse duplicate slashes, "./" and "../"
// segments. Same intent as iOS's `stringByStandardizingPath`, minus the
// Foundation dependency (no HOME expansion, which we never used anyway).
static std::string NormalizePath(const std::string& path) {
  if (path.empty()) return path;
  return NormalizeDotSegments(path);
}

// Convert a file:// URL to a filesystem path. Handles both file:///a/b and
// file:/a/b variants. Percent-decoding is deliberately omitted — the runtime
// only emits ASCII file:// URLs internally.
static std::string FileURLToPath(const std::string& url) {
  if (url.empty()) return url;
  if (!StartsWith(url, "file://")) return url;
  std::string tail = url.substr(7);
  // Strip host component when present (file://host/path → /path). NS never
  // emits a host, but be tolerant.
  if (!tail.empty() && tail[0] != '/') {
    size_t slash = tail.find('/');
    tail = (slash == std::string::npos) ? std::string() : tail.substr(slash);
  }
  // Drop query and fragment — these have no meaning for filesystem paths.
  size_t cut = tail.find_first_of("?#");
  if (cut != std::string::npos) tail = tail.substr(0, cut);
  return NormalizePath(tail);
}

// Resolve a relative or root-absolute spec against an HTTP(S) referrer URL.
// Returns empty string if resolution is not applicable.
static std::string ResolveHttpRelative(const std::string& referrerUrl,
                                       const std::string& spec) {
  if (referrerUrl.empty()) return std::string();
  if (!(StartsWith(referrerUrl, "http://") || StartsWith(referrerUrl, "https://"))) {
    return std::string();
  }
  // Normalize referrer: drop fragment and query.
  std::string base = referrerUrl;
  size_t hashPos = base.find('#');
  if (hashPos != std::string::npos) base = base.substr(0, hashPos);
  size_t qPos = base.find('?');
  if (qPos != std::string::npos) base = base.substr(0, qPos);

  size_t schemePos = base.find("://");
  if (schemePos == std::string::npos) return std::string();
  size_t pathStart = base.find('/', schemePos + 3);
  std::string origin = (pathStart == std::string::npos) ? base : base.substr(0, pathStart);
  std::string path = (pathStart == std::string::npos) ? std::string("/")
                                                      : base.substr(pathStart);

  std::string specPath = spec;
  std::string specSuffix;
  size_t specQ = specPath.find('?');
  size_t specH = specPath.find('#');
  size_t cut = std::string::npos;
  if (specQ != std::string::npos && specH != std::string::npos) {
    cut = std::min(specQ, specH);
  } else if (specQ != std::string::npos) {
    cut = specQ;
  } else if (specH != std::string::npos) {
    cut = specH;
  }
  if (cut != std::string::npos) {
    specSuffix = specPath.substr(cut);
    specPath = specPath.substr(0, cut);
  }

  std::string newPath;
  if (!specPath.empty() && specPath[0] == '/') {
    newPath = specPath;
  } else {
    size_t lastSlash = path.find_last_of('/');
    std::string baseDir = (lastSlash == std::string::npos)
                              ? std::string("/")
                              : path.substr(0, lastSlash + 1);
    newPath = baseDir + specPath;
  }
  return origin + NormalizeDotSegments(newPath) + specSuffix;
}

// Resolve a relative "./" or "../" specifier against a file:// referrer URL.
// Returns an absolute file:// URL, or empty when not applicable. Preserved
// for parity with the earlier Android loader; the current resolver builds
// filesystem candidates directly against GetApplicationPath() so this helper
// is unused for now.
[[maybe_unused]] static std::string ResolveFileRelative(
    const std::string& referrerUrl, const std::string& spec) {
  const std::string filePrefix = "file://";
  if (!StartsWith(referrerUrl, filePrefix.c_str())) return std::string();
  if (spec.empty() || spec[0] != '.') return std::string();
  std::string refPath = referrerUrl.substr(filePrefix.size());
  size_t hashPos = refPath.find('#');
  if (hashPos != std::string::npos) refPath = refPath.substr(0, hashPos);
  size_t qPos = refPath.find('?');
  if (qPos != std::string::npos) refPath = refPath.substr(0, qPos);
  size_t lastSlash = refPath.find_last_of('/');
  std::string baseDir = (lastSlash == std::string::npos)
                            ? std::string("/")
                            : refPath.substr(0, lastSlash + 1);
  return filePrefix + NormalizeDotSegments(baseDir + spec);
}

// Forward declarations for helpers referenced before their definitions.
static bool ShouldTraceRegistryKey(const std::string& rawKey,
                                   const std::string& registryKey);
static std::string CanonicalizeRegistryKey(const std::string& key);
static const char* ModuleStatusToString(v8::Module::Status status);
static void KillAsyncGraphLoadsForIsolate(v8::Isolate* isolate);
static bool IsCurrentIsolateWorker(v8::Isolate* isolate);
static std::string ExtractRelativePath(const std::string& path);
static void RejectAndClearInvalidatedModuleState(v8::Isolate* isolate,
                                                 v8::Local<v8::Context> context,
                                                 const std::string& registryKey);
static bool IsVolatileUrl(const std::string& url);

// ─────────────────────────────────────────────────────────────
// AdoptThenable
//
// Turn any thenable value into a real v8::Promise. Promises returned by
// V8 itself (Module::Evaluate) are genuine and take the fast path;
// user-space thenables (e.g. Proxy'd Promises) fail v8::Value::IsPromise
// but adopting them via Promise::Resolver::New + Resolve preserves their
// state.
static v8::MaybeLocal<v8::Promise> AdoptThenable(v8::Isolate* isolate,
                                                 v8::Local<v8::Context> context,
                                                 v8::Local<v8::Value> value) {
  if (value.IsEmpty()) return v8::MaybeLocal<v8::Promise>();
  if (value->IsPromise()) return value.As<v8::Promise>();
  if (!value->IsObject()) return v8::MaybeLocal<v8::Promise>();

  v8::Local<v8::Value> thenVal;
  if (!value.As<v8::Object>()
           ->Get(context, ArgConverter::ConvertToV8String(isolate, "then"))
           .ToLocal(&thenVal) ||
      !thenVal->IsFunction()) {
    return v8::MaybeLocal<v8::Promise>();
  }

  v8::Local<v8::Promise::Resolver> adopter;
  if (!v8::Promise::Resolver::New(context).ToLocal(&adopter) ||
      adopter->Resolve(context, value).IsNothing()) {
    return v8::MaybeLocal<v8::Promise>();
  }
  return adopter->GetPromise();
}

// ─────────────────────────────────────────────────────────────
// Compile helpers

static v8::MaybeLocal<v8::Module> CompileModuleFromSource(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& code, const std::string& urlStr) {
  v8::EscapableHandleScope hs(isolate);
  // NUL-preserving conversion: module source may contain embedded NUL bytes;
  // the char* path would truncate.
  v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, code);
  v8::Local<v8::String> urlV8;
  if (!v8::String::NewFromUtf8(isolate, urlStr.c_str(),
                               v8::NewStringType::kNormal)
           .ToLocal(&urlV8)) {
    return v8::MaybeLocal<v8::Module>();
  }
  v8::ScriptOrigin origin(urlV8, 0, 0, false, -1, v8::Local<v8::Value>(),
                          false, false, true /* is_module */);
  v8::ScriptCompiler::Source src(sourceText, origin);
  v8::Local<v8::Module> mod;
  if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
    return v8::MaybeLocal<v8::Module>();
  }
  if (mod->GetStatus() == v8::Module::kUninstantiated) {
    if (!mod->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
      return v8::MaybeLocal<v8::Module>();
    }
  }
  if (mod->GetStatus() != v8::Module::kEvaluated) {
    if (mod->Evaluate(context).IsEmpty()) {
      return v8::MaybeLocal<v8::Module>();
    }
  }
  return hs.Escape(mod);
}

// Compile-only variant used inside ResolveModuleCallback. Compiles a
// v8::Module and registers it under urlStr but does NOT instantiate or
// evaluate. V8 is currently instantiating the importer and will handle
// instantiation of this dependency.
static v8::MaybeLocal<v8::Module> CompileModuleForResolveRegisterOnly(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& code, const std::string& urlStr) {
  v8::EscapableHandleScope hs(isolate);
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  const std::string registryKey = CanonicalizeRegistryKey(urlStr);
  if (IsScriptLoadingLogEnabled() && ShouldTraceRegistryKey(urlStr, registryKey)) {
    DEBUG_WRITE("[resolver][register-resolve-only] raw=%s key=%s",
                urlStr.c_str(), registryKey.c_str());
  }

  v8::Local<v8::String> sourceText =
      ArgConverter::ConvertToV8String(isolate, code);
  v8::Local<v8::String> urlV8;
  if (!v8::String::NewFromUtf8(isolate, urlStr.c_str(),
                               v8::NewStringType::kNormal)
           .ToLocal(&urlV8)) {
    return v8::MaybeLocal<v8::Module>();
  }
  v8::ScriptOrigin origin(urlV8, 0, 0, false, -1, v8::Local<v8::Value>(),
                          false, false, true /* is_module */);
  v8::ScriptCompiler::Source src(sourceText, origin);
  v8::Local<v8::Module> mod;
  {
    v8::TryCatch tcCompile(isolate);
    if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
      if (IsDebuggable() && IsScriptLoadingLogEnabled()) {
        uint64_t h = 1469598103934665603ull;  // FNV-1a 64-bit
        for (unsigned char c : code) {
          h ^= c;
          h *= 1099511628211ull;
        }
        std::string snippet = code.substr(0, 600);
        for (char& ch : snippet) {
          if (ch == '\n' || ch == '\r') ch = ' ';
        }
        const char* classification = "unknown";
        v8::Local<v8::Message> message = tcCompile.Message();
        std::string msgStr;
        std::string srcLineStr;
        int lineNum = 0;
        int startCol = 0;
        int endCol = 0;
        if (!message.IsEmpty()) {
          v8::String::Utf8Value m8(isolate, message->Get());
          if (*m8) msgStr = *m8;
          lineNum = message->GetLineNumber(context).FromMaybe(0);
          startCol = message->GetStartColumn();
          endCol = message->GetEndColumn();
          v8::MaybeLocal<v8::String> maybeLine = message->GetSourceLine(context);
          if (!maybeLine.IsEmpty()) {
            v8::String::Utf8Value l8(isolate, maybeLine.ToLocalChecked());
            if (*l8) srcLineStr = *l8;
          }
          if (msgStr.find("Unexpected identifier") != std::string::npos ||
              msgStr.find("Unexpected token") != std::string::npos) {
            if (msgStr.find("export") != std::string::npos &&
                code.find("export default") == std::string::npos &&
                code.find("__sfc__") != std::string::npos)
              classification = "missing-export-default";
            else
              classification = "syntax";
          } else if (msgStr.find("Cannot use import statement") != std::string::npos) {
            classification = "wrap-error";
          }
        }
        if (classification == std::string("unknown")) {
          if (code.find("export default") == std::string::npos &&
              code.find("__sfc__") != std::string::npos)
            classification = "missing-export-default";
          else if (code.find("__sfc__") != std::string::npos &&
                   code.find("export {") == std::string::npos &&
                   code.find("export ") == std::string::npos)
            classification = "no-exports";
          else if (code.find("import ") == std::string::npos &&
                   code.find("export ") == std::string::npos)
            classification = "not-module";
          else if (code.find("_openBlock") != std::string::npos &&
                   code.find("openBlock") == std::string::npos)
            classification = "underscore-helper-unmapped";
        }
        if (srcLineStr.size() > 240) srcLineStr = srcLineStr.substr(0, 240);
        DEBUG_WRITE(
            "[http-esm][compile][v8-error][%s] %s line=%d col=%d..%d "
            "hash=%llx bytes=%lu msg=%s srcLine=%s snippet=%s",
            classification, urlStr.c_str(), lineNum, startCol, endCol,
            (unsigned long long)h, (unsigned long)code.size(),
            msgStr.c_str(), srcLineStr.c_str(), snippet.c_str());
      }
      return v8::MaybeLocal<v8::Module>();
    }
  }
  auto itExisting = g_moduleRegistry.find(registryKey);
  if (itExisting != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = itExisting->second.Get(isolate);
    if (!existing.IsEmpty()) {
      return hs.Escape(existing);
    }
  }
  g_moduleRegistry[registryKey].Reset(isolate, mod);
  return hs.Escape(mod);
}

// ─────────────────────────────────────────────────────────────
// Per-isolate module registries
//
// Why per-isolate (not process-global, not thread_local): v8::Global<T>
// handles are bound to the isolate that created them; reading their internal
// state from a different isolate is undefined behaviour. NS Workers each run
// a separate v8::Isolate on their own thread and, under HMR, may fetch the
// same URLs the main thread already loaded — a shared map would hand the
// worker isolate a Module the main isolate compiled, and V8's linker would
// read the cross-isolate export table and emit bogus errors like:
//   SyntaxError: The requested module 'X' does not provide an export named 'Y'
// Keying by v8::Isolate* stays correct even if an isolate is ever entered
// from another thread under v8::Locker.
//
// Lifetime: the per-isolate state is created lazily on first access and torn
// down by DestroyModuleStateForIsolate(), which the Runtime destructor
// should call while the isolate is still alive (before disposal) — so every
// v8::Global is Reset() at a safe time.

namespace {
struct PerIsolateModuleState {
  ModuleHandleMap registry;            // canonical key  -> compiled module
  ModuleHandleMap fallbackRegistry;    // canonical key  -> last good module
  ModuleHandleMap fallbackByRelative;  // relative path  -> last good module
};

std::mutex& ModuleStateTableMutex() {
  static std::mutex* mutex = new std::mutex();
  return *mutex;
}

robin_hood::unordered_map<v8::Isolate*, std::unique_ptr<PerIsolateModuleState>>&
ModuleStateTable() {
  static auto* table = new robin_hood::unordered_map<
      v8::Isolate*, std::unique_ptr<PerIsolateModuleState>>();
  return *table;
}

PerIsolateModuleState& ModuleStateFor(v8::Isolate* isolate) {
  std::lock_guard<std::mutex> lock(ModuleStateTableMutex());
  auto& table = ModuleStateTable();
  auto it = table.find(isolate);
  if (it == table.end()) {
    it = table.emplace(isolate, std::make_unique<PerIsolateModuleState>()).first;
  }
  return *it->second;
}
}  // namespace

ModuleHandleMap& ModuleRegistryFor(v8::Isolate* isolate) {
  return ModuleStateFor(isolate).registry;
}

static ModuleHandleMap& ModuleFallbackRegistryFor(v8::Isolate* isolate) {
  return ModuleStateFor(isolate).fallbackRegistry;
}

static ModuleHandleMap& ModuleFallbackByRelativeFor(v8::Isolate* isolate) {
  return ModuleStateFor(isolate).fallbackByRelative;
}

void DestroyModuleStateForIsolate(v8::Isolate* isolate) {
  // First: neutralize any in-flight async graph loads for this isolate. Their
  // fetch completions check the dead flag before touching V8, and their
  // context Globals are Reset here while the isolate is still alive.
  KillAsyncGraphLoadsForIsolate(isolate);

  std::unique_ptr<PerIsolateModuleState> state;
  {
    std::lock_guard<std::mutex> lock(ModuleStateTableMutex());
    auto& table = ModuleStateTable();
    auto it = table.find(isolate);
    if (it == table.end()) return;
    state = std::move(it->second);
    table.erase(it);
  }
  for (auto& kv : state->registry) kv.second.Reset();
  for (auto& kv : state->fallbackRegistry) kv.second.Reset();
  for (auto& kv : state->fallbackByRelative) kv.second.Reset();
}

// ─────────────────────────────────────────────────────────────
// Import map: bare specifier → resolved URL (populated by ns:module
// configureLoader). Instead of rewriting import statements on the bundler
// side, the runtime resolves bare specifiers through this map to HTTP module
// URLs. Source code is served as-is.
static robin_hood::unordered_map<std::string, std::string> g_importMap;

// Volatile URL patterns: URLs matching these substrings are always re-fetched
// (cache is evicted before loading). Configured at boot by the dev client —
// the vocabulary is server/framework policy, so the runtime carries no
// framework-specific URL strings here.
static std::vector<std::string> g_volatilePatterns;

static bool ShouldTraceRegistryKey(const std::string& rawKey,
                                   const std::string& registryKey) {
  if (rawKey != registryKey) return true;
  return StartsWith(registryKey, "optional:") ||
         StartsWith(registryKey, "node:") ||
         StartsWith(registryKey, "blob:");
}

static std::string CanonicalizeRegistryKey(const std::string& key) {
  if (key.empty()) return key;

  std::string registryKey;
  const char* classification = "path";
  bool traceEvenWithoutChange = false;

  if (StartsWith(key, "http://") || StartsWith(key, "https://") ||
      StartsWith(key, "file://http://") || StartsWith(key, "file://https://")) {
    registryKey = CanonicalizeHttpUrlKey(key);
    classification = "http";
  } else if (StartsWith(key, "file://")) {
    registryKey = NormalizePath(FileURLToPath(key));
    classification = "file-url";
  } else if (StartsWith(key, "blob:")) {
    registryKey = key;
    classification = "blob";
    traceEvenWithoutChange = true;
  } else {
    // Preserve non-filesystem module namespaces such as optional: and node:
    // so synthetic/in-memory modules keep their exact registry identity.
    size_t schemePos = key.find(':');
    size_t slashPos = key.find('/');
    if (schemePos != std::string::npos &&
        (slashPos == std::string::npos || schemePos < slashPos)) {
      registryKey = key;
      classification = "custom-scheme";
      traceEvenWithoutChange = true;
    } else {
      registryKey = NormalizePath(key);
    }
  }

  if (IsScriptLoadingLogEnabled() &&
      (traceEvenWithoutChange || registryKey != key)) {
    DEBUG_WRITE("[resolver][registry-key][%s] raw=%s key=%s", classification,
                key.c_str(), registryKey.c_str());
  }
  return registryKey;
}

v8::MaybeLocal<v8::Module> LoadHttpModuleForUrl(v8::Isolate* isolate,
                                                v8::Local<v8::Context> context,
                                                const std::string& requestedUrl) {
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  const std::string registryKey = CanonicalizeHttpUrlKey(requestedUrl);

  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[http-esm][load][begin] request=%s key=%s",
                requestedUrl.c_str(), registryKey.c_str());
  }

  auto itExisting = g_moduleRegistry.find(registryKey);
  if (itExisting != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = itExisting->second.Get(isolate);
    if (!existing.IsEmpty() && existing->GetStatus() != v8::Module::kErrored) {
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[http-esm][load][cache-hit] key=%s", registryKey.c_str());
      }
      return v8::MaybeLocal<v8::Module>(existing);
    }
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][load][drop-errored] key=%s", registryKey.c_str());
    }
    RemoveModuleFromRegistry(registryKey);
  }

  std::string body;
  std::string contentType;
  int status = 0;
  if (!HttpFetchText(requestedUrl, body, contentType, status) || body.empty()) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][load][fetch-fail] request=%s key=%s status=%d",
                  requestedUrl.c_str(), registryKey.c_str(), status);
    }
    if (IsDebuggable()) {
      std::string msg = "HTTP import failed: " + requestedUrl +
                        " (status=" + std::to_string(status) + ")";
      isolate->ThrowException(v8::Exception::Error(
          ArgConverter::ConvertToV8String(isolate, msg)));
    }
    return v8::MaybeLocal<v8::Module>();
  }

  v8::MaybeLocal<v8::Module> loaded =
      CompileModuleForResolveRegisterOnly(isolate, context, body, registryKey);
  if (loaded.IsEmpty()) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-esm][load][compile-fail] request=%s key=%s bytes=%zu",
                  requestedUrl.c_str(), registryKey.c_str(), body.size());
    }
    if (IsDebuggable()) {
      std::string msg = "HTTP import compile failed: " + requestedUrl;
      isolate->ThrowException(v8::Exception::Error(
          ArgConverter::ConvertToV8String(isolate, msg)));
    }
    return v8::MaybeLocal<v8::Module>();
  }

  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[http-esm][load][ok] request=%s key=%s type=%s bytes=%zu",
                requestedUrl.c_str(), registryKey.c_str(),
                contentType.c_str(), body.size());
  }
  return loaded;
}

// ─────────────────────────────────────────────────────────────
// Import map helpers

// Small hand-rolled JSON scanner for a flat {"imports": {"key": "value", ...}}
// shape. Only strings are accepted; anything malformed is silently skipped —
// same behaviour as the iOS Foundation-based parser for non-object roots.
namespace {
struct JsonScanner {
  const std::string& s;
  size_t i = 0;

  explicit JsonScanner(const std::string& src) : s(src) {}

  void SkipWs() {
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' ||
                             s[i] == '\r')) {
      ++i;
    }
  }

  bool Peek(char c) {
    SkipWs();
    return i < s.size() && s[i] == c;
  }

  bool Consume(char c) {
    if (Peek(c)) {
      ++i;
      return true;
    }
    return false;
  }

  // Parses a JSON string into `out`. Handles standard escape sequences
  // (\", \\, \/, \b, \f, \n, \r, \t) and \uXXXX (BMP only; surrogate pairs
  // are decoded to their two escapes as-is when not paired — good enough
  // for the small import-map vocabulary the dev server emits).
  bool ReadString(std::string& out) {
    SkipWs();
    if (i >= s.size() || s[i] != '"') return false;
    ++i;
    out.clear();
    while (i < s.size()) {
      char c = s[i++];
      if (c == '"') return true;
      if (c != '\\') {
        out.push_back(c);
        continue;
      }
      if (i >= s.size()) return false;
      char e = s[i++];
      switch (e) {
        case '"':
        case '\\':
        case '/':
          out.push_back(e);
          break;
        case 'b': out.push_back('\b'); break;
        case 'f': out.push_back('\f'); break;
        case 'n': out.push_back('\n'); break;
        case 'r': out.push_back('\r'); break;
        case 't': out.push_back('\t'); break;
        case 'u': {
          if (i + 4 > s.size()) return false;
          unsigned int cp = 0;
          for (int k = 0; k < 4; ++k) {
            char h = s[i++];
            cp <<= 4;
            if (h >= '0' && h <= '9') cp |= (unsigned)(h - '0');
            else if (h >= 'a' && h <= 'f') cp |= (unsigned)(h - 'a' + 10);
            else if (h >= 'A' && h <= 'F') cp |= (unsigned)(h - 'A' + 10);
            else return false;
          }
          if (cp < 0x80) {
            out.push_back((char)cp);
          } else if (cp < 0x800) {
            out.push_back((char)(0xC0 | (cp >> 6)));
            out.push_back((char)(0x80 | (cp & 0x3F)));
          } else {
            out.push_back((char)(0xE0 | (cp >> 12)));
            out.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back((char)(0x80 | (cp & 0x3F)));
          }
          break;
        }
        default:
          return false;
      }
    }
    return false;
  }

  // Skip an arbitrary JSON value (object/array/string/number/keyword) —
  // used to step over "imports" siblings we don't care about.
  bool SkipValue() {
    SkipWs();
    if (i >= s.size()) return false;
    char c = s[i];
    if (c == '"') {
      std::string tmp;
      return ReadString(tmp);
    }
    if (c == '{' || c == '[') {
      char open = c, close = (c == '{') ? '}' : ']';
      int depth = 0;
      bool inString = false;
      while (i < s.size()) {
        char ch = s[i++];
        if (inString) {
          if (ch == '\\' && i < s.size()) ++i;
          else if (ch == '"') inString = false;
        } else {
          if (ch == '"') inString = true;
          else if (ch == open) ++depth;
          else if (ch == close) {
            --depth;
            if (depth == 0) return true;
          }
        }
      }
      return false;
    }
    // Number / true / false / null — read until the next value terminator.
    while (i < s.size()) {
      char ch = s[i];
      if (ch == ',' || ch == '}' || ch == ']' || ch == ' ' || ch == '\t' ||
          ch == '\n' || ch == '\r') {
        return true;
      }
      ++i;
    }
    return true;
  }
};
}  // namespace

void SetImportMap(const std::string& json) {
  g_importMap.clear();
  if (json.empty()) return;

  JsonScanner sc(json);
  if (!sc.Consume('{')) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[import-map] parse failed: not an object");
    }
    return;
  }

  // Find and enter the "imports" object; skip any siblings.
  bool foundImports = false;
  while (!sc.Peek('}')) {
    std::string key;
    if (!sc.ReadString(key)) break;
    if (!sc.Consume(':')) break;
    if (key == "imports") {
      if (!sc.Consume('{')) break;
      foundImports = true;
      // Parse the flat {"k":"v", ...} body.
      while (!sc.Peek('}')) {
        std::string k, v;
        if (!sc.ReadString(k)) break;
        if (!sc.Consume(':')) break;
        if (sc.Peek('"')) {
          if (!sc.ReadString(v)) break;
          g_importMap[k] = v;
        } else {
          // Skip non-string values (arrays, objects, etc.) — mirrors iOS.
          if (!sc.SkipValue()) break;
        }
        if (!sc.Consume(',')) break;
      }
      sc.Consume('}');
    } else {
      if (!sc.SkipValue()) break;
    }
    if (!sc.Consume(',')) break;
  }

  if (IsScriptLoadingLogEnabled()) {
    if (!foundImports) {
      DEBUG_WRITE("[import-map] no 'imports' object found");
    }
    DEBUG_WRITE("[import-map] loaded %lu entries",
                (unsigned long)g_importMap.size());
  }
}

void SetVolatilePatterns(const std::vector<std::string>& patterns) {
  g_volatilePatterns = patterns;
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[import-map] volatile patterns: %lu",
                (unsigned long)g_volatilePatterns.size());
  }
}

static bool IsVolatileUrl(const std::string& url) {
  for (const auto& pat : g_volatilePatterns) {
    if (url.find(pat) != std::string::npos) return true;
  }
  return false;
}

// Normalize a Vite-rewritten specifier into the canonical import-map key.
// Handles two common patterns:
//   1. Prebundled deps:  "/node_modules/.vite/deps/solid-js.js?v=abc"   → "solid-js"
//                        "/node_modules/.vite/deps/@tanstack_solid-router.js" →
//                        "@tanstack/solid-router"
//   2. Explicit node_modules paths:
//        "/node_modules/@angular/core/fesm2022/core.mjs" → "@angular/core/fesm2022/core.mjs"
//        "/node_modules/tslib/tslib.es6.mjs"             → "tslib"
static std::string NormalizeViteSpecifier(const std::string& specifier) {
  // Pattern 1: Vite prebundled deps.
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
        DEBUG_WRITE("[import-map][normalize] vite-deps: %s -> %s",
                    specifier.c_str(), id.c_str());
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
          pkgName = (slash2 != std::string::npos) ? subNoQuery.substr(0, slash2)
                                                  : subNoQuery;
        }
      } else {
        auto slash = subNoQuery.find('/');
        pkgName = (slash != std::string::npos) ? subNoQuery.substr(0, slash)
                                               : subNoQuery;
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
            const std::string pkgBaseName =
                pkgName.substr(pkgName.find_last_of('/') + 1);
            std::string withoutExt = remainder;
            auto dot = withoutExt.rfind('.');
            if (dot != std::string::npos) {
              withoutExt = withoutExt.substr(0, dot);
            }
            std::string withoutPlatform = withoutExt;
            for (const auto& suffix : {std::string(".ios"), std::string(".android"),
                                       std::string(".visionos")}) {
              if (EndsWith(withoutPlatform, suffix)) {
                withoutPlatform =
                    withoutPlatform.substr(0, withoutPlatform.size() - suffix.size());
                break;
              }
            }
            const bool isRootLevelMainEntry =
                withoutPlatform == "index" ||
                withoutPlatform == pkgBaseName ||
                withoutPlatform.rfind(pkgBaseName + ".", 0) == 0;
            preserveSubpath = !isRootLevelMainEntry;
          }

          if (preserveSubpath) {
            normalized = pkgName + "/" + remainder + querySuffix;
          }
        }

        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[import-map][normalize] node_modules: %s -> %s",
                      specifier.c_str(), normalized.c_str());
        }
        return normalized;
      }
    }
  }
  return "";
}

// Look up a specifier in the import map. Supports exact and prefix matches
// (trailing-slash entries like "solid-js/" that map subpaths).
static std::string LookupImportMap(const std::string& specifier) {
  auto it = g_importMap.find(specifier);
  if (it != g_importMap.end()) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[import-map] exact: %s -> %s", specifier.c_str(),
                  it->second.c_str());
    }
    return it->second;
  }
  std::string bestKey;
  std::string bestValue;
  for (const auto& kv : g_importMap) {
    const std::string& key = kv.first;
    if (key.back() != '/') continue;
    if (specifier.size() > key.size() &&
        specifier.compare(0, key.size(), key) == 0) {
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
      DEBUG_WRITE("[import-map] prefix: %s -> %s (via %s)", specifier.c_str(),
                  resolved.c_str(), bestKey.c_str());
    }
    return resolved;
  }
  return "";
}

void CleanupImportMapGlobals() {
  // Process-global import-map state (not isolate-bound). The per-isolate
  // module handle maps (registry / fallback / fallbackByRelative) are torn
  // down separately by DestroyModuleStateForIsolate(), which the Runtime
  // destructor invokes for every isolate before disposal.
  g_importMap.clear();
  g_volatilePatterns.clear();
}

// ─────────────────────────────────────────────────────────────
// Worker isolate detection: iOS keys off Caches::Get(isolate)->isWorker.
// Android encodes the same signal by installing a WORKER_WRAPPER pointer in
// the isolate's data slot on worker isolates only (see Runtime.h).
static bool IsCurrentIsolateWorker(v8::Isolate* isolate) {
  if (isolate == nullptr) return false;
  return isolate->GetData((uint32_t)Runtime::IsolateData::WORKER_WRAPPER) !=
         nullptr;
}

// Monotonic microseconds since some fixed epoch — matches iOS's
// CFAbsoluteTimeGetCurrent() semantic (used for internal timing only, never
// exposed to JS).
static uint64_t MonotonicUs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)(ts.tv_nsec / 1000);
}

// ─────────────────────────────────────────────────────────────
// Async HTTP module-graph pipeline
//
// See the contract comment in ModuleInternalCallbacks.h. Mechanically:
//
//   EnqueueUrl(root)
//     → FetchModuleBodyAsync (background thread — see HttpLoader.cpp)
//     → hop to the isolate's JS thread via LooperTasks::Post
//     → CompileModuleForResolveRegisterOnly (registers under the canonical
//       URL key — the exact entry ResolveModuleCallback will look up)
//     → GetModuleRequests() → ResolveModuleRequestForWalk → EnqueueUrl(…)
//     → when pendingFetches drains, onComplete fires on the JS thread.
//
// Thread discipline: `visited`, `pendingFetches`, `failed`, `completed` are
// touched ONLY on the isolate's JS thread (every fetch completion hops there
// first). Only raw I/O runs off-thread. The one crossing signal is `dead`,
// an atomic set by isolate teardown so in-flight completions become no-ops
// instead of touching a disposed isolate.

namespace {
struct AsyncGraphLoad {
  v8::Isolate* isolate = nullptr;
  v8::Global<v8::Context> context;
  std::shared_ptr<LooperTasks> jsTasks;             // isolate's JS thread queue
  std::string rootKey;                              // canonical registry key of the root URL
  robin_hood::unordered_set<std::string> visited;   // canonical keys (JS thread only)
  int pendingFetches = 0;                           // JS thread only
  bool failed = false;                              // JS thread only (root failure)
  bool completed = false;                           // JS thread only
  std::string failureMessage;
  size_t fetchedCount = 0;
  size_t compiledCount = 0;
  uint64_t startUs = 0;
  std::atomic<bool> dead{false};                    // set by isolate teardown (any thread)
  std::function<void(bool ok, const std::string& errorMessage,
                     v8::Local<v8::Context> context)>
      onComplete;

  ~AsyncGraphLoad() {
    g_asyncGraphLoadsInFlightCounter().fetch_sub(1, std::memory_order_acq_rel);
  }

  static std::atomic<int>& g_asyncGraphLoadsInFlightCounter() {
    static std::atomic<int> counter{0};
    return counter;
  }
};

std::mutex& AsyncGraphLoadsMutex() {
  static std::mutex* mutex = new std::mutex();
  return *mutex;
}

robin_hood::unordered_map<v8::Isolate*,
                          std::vector<std::weak_ptr<AsyncGraphLoad>>>&
AsyncGraphLoadsByIsolate() {
  static auto* table = new robin_hood::unordered_map<
      v8::Isolate*, std::vector<std::weak_ptr<AsyncGraphLoad>>>();
  return *table;
}

void RegisterAsyncGraphLoad(v8::Isolate* isolate,
                            const std::shared_ptr<AsyncGraphLoad>& load) {
  std::lock_guard<std::mutex> lock(AsyncGraphLoadsMutex());
  auto& loads = AsyncGraphLoadsByIsolate()[isolate];
  loads.erase(std::remove_if(loads.begin(), loads.end(),
                             [](const std::weak_ptr<AsyncGraphLoad>& w) {
                               return w.expired();
                             }),
              loads.end());
  loads.push_back(load);
}
}  // namespace

bool HasPendingAsyncModuleGraphWork() {
  return AsyncGraphLoad::g_asyncGraphLoadsInFlightCounter().load(
             std::memory_order_acquire) > 0;
}

// Isolate-teardown hook: mark every in-flight load owned by `isolate` dead
// (pending fetch completions become no-ops) and Reset their context Globals
// NOW, while the isolate is still alive.
static void KillAsyncGraphLoadsForIsolate(v8::Isolate* isolate) {
  std::vector<std::shared_ptr<AsyncGraphLoad>> doomed;
  {
    std::lock_guard<std::mutex> lock(AsyncGraphLoadsMutex());
    auto& table = AsyncGraphLoadsByIsolate();
    auto it = table.find(isolate);
    if (it == table.end()) return;
    for (auto& weak : it->second) {
      if (auto load = weak.lock()) {
        doomed.push_back(std::move(load));
      }
    }
    table.erase(it);
  }
  for (auto& load : doomed) {
    load->dead.store(true, std::memory_order_release);
    load->context.Reset();
  }
}

// Resolve one static module request to an absolute HTTP(S) URL using the
// SAME logic ResolveModuleCallback applies, in the same order: malformed
// scheme repair → import map (direct, then Vite-normalized) → absolute
// HTTP passthrough → relative/root-absolute resolution against an HTTP
// referrer. Returns empty for everything the walk should NOT touch.
static std::string ResolveModuleRequestForWalk(const std::string& rawSpec,
                                               const std::string& referrerUrl) {
  if (rawSpec.empty() || rawSpec == "@") return "";
  std::string spec = rawSpec;
  if (spec.rfind("http:/", 0) == 0 && spec.rfind("http://", 0) != 0) {
    spec.insert(5, "/");
  } else if (spec.rfind("https:/", 0) == 0 && spec.rfind("https://", 0) != 0) {
    spec.insert(6, "/");
  }

  if (!g_importMap.empty()) {
    std::string mapped = LookupImportMap(spec);
    if (mapped.empty()) {
      std::string normalized = NormalizeViteSpecifier(spec);
      if (!normalized.empty()) {
        mapped = LookupImportMap(normalized);
      }
    }
    if (!mapped.empty()) spec = mapped;
  }

  if (StartsWith(spec, "http://") || StartsWith(spec, "https://")) {
    return spec;
  }

  const bool specIsRelative = !spec.empty() && spec[0] == '.';
  const bool specIsRootAbs = !spec.empty() && spec[0] == '/';
  const bool referrerIsHttp = StartsWith(referrerUrl, "http://") ||
                              StartsWith(referrerUrl, "https://");
  if ((specIsRelative || specIsRootAbs) && referrerIsHttp) {
    std::string resolved = ResolveHttpRelative(referrerUrl, spec);
    if (StartsWith(resolved, "http://") || StartsWith(resolved, "https://")) {
      return resolved;
    }
  }
  return "";
}

static void AsyncGraphEnqueueUrl(const std::shared_ptr<AsyncGraphLoad>& load,
                                 const std::string& url);

// Walk `mod`'s static module requests and enqueue every HTTP-resolvable
// dependency. JS thread only; `moduleUrl` is the canonical URL the module
// was registered under (the referrer for relative resolution).
static void AsyncGraphWalkModuleRequests(
    const std::shared_ptr<AsyncGraphLoad>& load,
    v8::Local<v8::Context> /*context*/, v8::Local<v8::Module> mod,
    const std::string& moduleUrl) {
  v8::Isolate* isolate = load->isolate;
  v8::Local<v8::FixedArray> requests = mod->GetModuleRequests();
  const int length = requests->Length();
  for (int i = 0; i < length; i++) {
    v8::Local<v8::ModuleRequest> request =
        requests->Get(i).As<v8::ModuleRequest>();
    if (request.IsEmpty()) continue;
    v8::Local<v8::String> specV8 = request->GetSpecifier();
    v8::String::Utf8Value specUtf8(isolate, specV8);
    if (!*specUtf8) continue;
    std::string resolved = ResolveModuleRequestForWalk(*specUtf8, moduleUrl);
    if (resolved.empty()) continue;
    AsyncGraphEnqueueUrl(load, resolved);
  }
}

// Fire onComplete exactly once, when the frontier has drained. JS thread only.
static void AsyncGraphMaybeComplete(const std::shared_ptr<AsyncGraphLoad>& load,
                                    v8::Local<v8::Context> context) {
  if (load->completed || load->pendingFetches > 0) return;
  load->completed = true;
  if (IsScriptLoadingLogEnabled()) {
    const uint64_t endUs = MonotonicUs();
    const uint64_t ms = endUs > load->startUs ? (endUs - load->startUs) / 1000ull : 0ull;
    DEBUG_WRITE(
        "[async-graph][done] root=%s urls=%lu fetched=%lu compiled=%lu ms=%llu ok=%d",
        load->rootKey.c_str(), (unsigned long)load->visited.size(),
        (unsigned long)load->fetchedCount, (unsigned long)load->compiledCount,
        (unsigned long long)ms, load->failed ? 0 : 1);
  }
  auto onComplete = std::move(load->onComplete);
  load->onComplete = nullptr;
  if (onComplete) {
    v8::TryCatch tc(load->isolate);
    onComplete(!load->failed, load->failureMessage, context);
    (void)tc;  // swallow any pending exception; failures already surface as rejections
  }
}

// A fetched body arrived on the isolate's JS thread: compile + register it,
// then walk its requests. Runs outside any V8 scope, so it enters the isolate
// the same way other cross-thread callbacks do.
static void AsyncGraphOnFetchCompleted(
    const std::shared_ptr<AsyncGraphLoad>& load, const std::string& url,
    bool ok, int status, const std::shared_ptr<std::string>& body) {
  if (load->dead.load(std::memory_order_acquire)) return;
  v8::Isolate* isolate = load->isolate;
  if (Runtime::GetRuntime(isolate) == nullptr) return;

  v8::Locker locker(isolate);
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = load->context.Get(isolate);
  if (context.IsEmpty()) return;
  v8::Context::Scope context_scope(context);

  load->pendingFetches--;

  const std::string key = CanonicalizeHttpUrlKey(url);
  const bool isRoot = (key == load->rootKey);

  if (!load->failed) {
    if (!ok) {
      if (isRoot) {
        load->failed = true;
        load->failureMessage = "HTTP import failed: " + url +
                               " (status=" + std::to_string(status) + ")";
      } else if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[async-graph][dep-fetch-fail] %s status=%d (left to sync resolver)",
                    url.c_str(), status);
      }
    } else {
      load->fetchedCount++;
      v8::MaybeLocal<v8::Module> maybeMod =
          CompileModuleForResolveRegisterOnly(isolate, context, *body, key);
      v8::Local<v8::Module> mod;
      if (!maybeMod.ToLocal(&mod)) {
        if (isRoot) {
          load->failed = true;
          load->failureMessage = "HTTP import compile failed: " + url;
        } else if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[async-graph][dep-compile-fail] %s (left to sync resolver)",
                      url.c_str());
        }
      } else {
        load->compiledCount++;
        AsyncGraphWalkModuleRequests(load, context, mod, key);
      }
    }
  }

  AsyncGraphMaybeComplete(load, context);
  isolate->PerformMicrotaskCheckpoint();
}

// Enqueue one URL into the walk frontier. JS thread only.
static void AsyncGraphEnqueueUrl(const std::shared_ptr<AsyncGraphLoad>& load,
                                 const std::string& url) {
  const std::string key = CanonicalizeHttpUrlKey(url);
  if (!load->visited.insert(key).second) return;

  v8::Isolate* isolate = load->isolate;
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  auto it = g_moduleRegistry.find(key);
  if (it != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = it->second.Get(isolate);
    if (!existing.IsEmpty() && existing->GetStatus() != v8::Module::kErrored) {
      if (existing->GetStatus() == v8::Module::kUninstantiated) {
        v8::Local<v8::Context> context = load->context.Get(isolate);
        if (!context.IsEmpty()) {
          AsyncGraphWalkModuleRequests(load, context, existing, key);
        }
      }
      return;
    }
    RemoveModuleFromRegistry(key);
  }

  load->pendingFetches++;
  std::shared_ptr<LooperTasks> jsTasks = load->jsTasks;
  std::shared_ptr<AsyncGraphLoad> loadRef = load;
  FetchModuleBodyAsync(url, [loadRef, url, jsTasks](bool ok, int status,
                                                    std::string body) {
    // Arbitrary thread. Hop to the isolate's JS thread before touching any
    // walk state or V8. If the isolate died in between, drop everything —
    // the context Global was already Reset by the teardown hook.
    if (loadRef->dead.load(std::memory_order_acquire) || jsTasks == nullptr) {
      return;
    }
    auto bodyPtr = std::make_shared<std::string>(std::move(body));
    jsTasks->Post([loadRef, url, ok, status, bodyPtr]() {
      AsyncGraphOnFetchCompleted(loadRef, url, ok, status, bodyPtr);
    });
  });
}

void StartAsyncHttpModuleGraphLoad(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& rootUrl,
    std::function<void(bool ok, const std::string& errorMessage,
                       v8::Local<v8::Context> context)>
        onComplete) {
  auto load = std::make_shared<AsyncGraphLoad>();
  load->isolate = isolate;
  load->context.Reset(isolate, context);
  load->rootKey = CanonicalizeHttpUrlKey(rootUrl);
  load->startUs = MonotonicUs();
  load->onComplete = std::move(onComplete);

  Runtime* runtime = Runtime::GetRuntime(isolate);
  load->jsTasks = runtime != nullptr ? runtime->GetLooperTasks() : nullptr;

  AsyncGraphLoad::g_asyncGraphLoadsInFlightCounter().fetch_add(
      1, std::memory_order_acq_rel);
  RegisterAsyncGraphLoad(isolate, load);

  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[async-graph][start] root=%s key=%s", rootUrl.c_str(),
                load->rootKey.c_str());
  }

  AsyncGraphEnqueueUrl(load, rootUrl);
  // Root already registered (or nothing fetchable): complete inline.
  AsyncGraphMaybeComplete(load, context);
}

bool RunAsyncHttpModuleGraphLoadPumped(v8::Isolate* isolate,
                                       v8::Local<v8::Context> context,
                                       const std::string& rootUrl,
                                       double timeoutSeconds) {
  if (timeoutSeconds <= 0.0) timeoutSeconds = 60.0;
  auto done = std::make_shared<bool>(false);
  StartAsyncHttpModuleGraphLoad(
      isolate, context, rootUrl,
      [done](bool /*ok*/, const std::string& /*errorMessage*/,
             v8::Local<v8::Context>) { *done = true; });

  // Manual looper pump ("until either all is settled or the app takes
  // over"): the walk's completion tasks are posted to this thread's
  // LooperTasks queue and dispatched via ALooper — polling the looper here
  // services them. ALooper_pollOnce with a small timeout keeps the pump
  // responsive without spinning.
  const auto deadline =
      std::chrono::steady_clock::now() +
      std::chrono::milliseconds(static_cast<int64_t>(timeoutSeconds * 1000.0));
  while (!*done && std::chrono::steady_clock::now() < deadline) {
    ALooper_pollOnce(10 /* ms */, nullptr, nullptr, nullptr);
  }
  if (!*done && IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE(
        "[async-graph][pumped][timeout] root=%s after %.1fs (sync loader takes over)",
        rootUrl.c_str(), timeoutSeconds);
  }
  return *done;
}

// ─────────────────────────────────────────────────────────────
// Registry mutation + diagnostics

// Compute a relative path key for fallback lookup (mirrors iOS's helper).
// On Android there is no separate Documents directory — everything lives
// under the application path.
static std::string ExtractRelativePath(const std::string& path) {
  std::string appPrefix = NormalizePath(GetApplicationPath());
  if (!appPrefix.empty()) {
    std::string directPrefix = appPrefix + "/";
    if (path.rfind(directPrefix, 0) == 0) {
      return path.substr(directPrefix.size());
    }
    // Some code paths carry "…/app/…" twice (bundled app folder).
    std::string appFolderPrefix = appPrefix + "/app/";
    if (path.rfind(appFolderPrefix, 0) == 0) {
      return path.substr(appFolderPrefix.size());
    }
  }
  return "";
}

static const char* ModuleStatusToString(v8::Module::Status status) {
  switch (status) {
    case v8::Module::kUninstantiated:
      return "Uninstantiated";
    case v8::Module::kInstantiating:
      return "Instantiating";
    case v8::Module::kInstantiated:
      return "Instantiated";
    case v8::Module::kEvaluating:
      return "Evaluating";
    case v8::Module::kEvaluated:
      return "Evaluated";
    case v8::Module::kErrored:
      return "Errored";
  }
  return "Unknown";
}

void RemoveModuleFromRegistry(const std::string& canonicalPath) {
  // Only ever called on an isolate's own JS thread during module
  // resolution/loading, so the entered isolate owns the maps to mutate.
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  if (isolate == nullptr) return;
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  auto& g_moduleFallbackRegistry = ModuleFallbackRegistryFor(isolate);
  auto& g_moduleFallbackByRelative = ModuleFallbackByRelativeFor(isolate);
  const std::string registryKey = CanonicalizeRegistryKey(canonicalPath);

  // Defensive: never operate on an anomalous/sentinel key.
  auto isSentinel = [](const std::string& s) -> bool {
    if (s == "@") return true;
    return s.find("__invalid_at__.mjs") != std::string::npos;
  };
  if (isSentinel(registryKey)) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][guard-v3] ignore remove for sentinel %s",
                  registryKey.c_str());
    }
    return;
  }

  auto classify = [](const std::string& s) -> const char* {
    if (s == "@") return "sentinel:@";
    if (s.find("__invalid_at__.mjs") != std::string::npos)
      return "sentinel:invalid_at";
    bool http = StartsWith(s, "http://") || StartsWith(s, "https://");
    if (http) {
      if (IsVolatileUrl(s)) return "http:volatile";
      if (s.find("/@ns/sfc/") != std::string::npos) return "http:sfc";
      if (s.find("/@ns/m/") != std::string::npos) return "http:m";
      return "http:other";
    }
    if (StartsWith(s, "file://")) return "file-url";
    return "path";
  };

  if (IsScriptLoadingLogEnabled()) {
    if (registryKey != canonicalPath) {
      DEBUG_WRITE("[resolver][remove:pre] raw=%s key=%s class=%s",
                  canonicalPath.c_str(), registryKey.c_str(),
                  classify(registryKey));
    } else {
      DEBUG_WRITE("[resolver][remove:pre] key=%s class=%s", registryKey.c_str(),
                  classify(registryKey));
    }
  }

  size_t regPre = g_moduleRegistry.size();
  size_t fbPre = g_moduleFallbackRegistry.size();
  size_t relPre = g_moduleFallbackByRelative.size();

  auto it = g_moduleRegistry.find(registryKey);
  if (it != g_moduleRegistry.end()) {
    bool isHttpKey =
        StartsWith(registryKey, "http://") || StartsWith(registryKey, "https://");
    if (IsScriptLoadingLogEnabled() && !isHttpKey) {
      DEBUG_WRITE("[resolver] removing stale module %s", registryKey.c_str());
    }
    it->second.Reset();
    g_moduleRegistry.erase(it);
  } else if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE(
        "[resolver][remove:miss] key not found, proceed to clear fallbacks (%s)",
        registryKey.c_str());
  }
  auto fb = g_moduleFallbackRegistry.find(registryKey);
  if (fb != g_moduleFallbackRegistry.end()) {
    fb->second.Reset();
    g_moduleFallbackRegistry.erase(fb);
  }
  std::string rel = ExtractRelativePath(registryKey);
  if (!rel.empty()) {
    auto fbr = g_moduleFallbackByRelative.find(rel);
    if (fbr != g_moduleFallbackByRelative.end()) {
      fbr->second.Reset();
      g_moduleFallbackByRelative.erase(fbr);
    }
  }

  if (IsScriptLoadingLogEnabled()) {
    size_t regPost = g_moduleRegistry.size();
    size_t fbPost = g_moduleFallbackRegistry.size();
    size_t relPost = g_moduleFallbackByRelative.size();
    DEBUG_WRITE(
        "[resolver][remove:post] reg %lu->%lu fb %lu->%lu rel %lu->%lu",
        (unsigned long)regPre, (unsigned long)regPost, (unsigned long)fbPre,
        (unsigned long)fbPost, (unsigned long)relPre, (unsigned long)relPost);
  }
}

std::vector<std::string> GetLoadedModuleUrls() {
  std::vector<std::string> urls;
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  if (isolate == nullptr) return urls;
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  urls.reserve(g_moduleRegistry.size());

  for (const auto& entry : g_moduleRegistry) {
    const std::string& key = entry.first;
    if (key.empty()) continue;
    if (StartsWith(key, "blob:") || key.find("://") != std::string::npos) {
      urls.push_back(key);
    }
  }
  std::sort(urls.begin(), urls.end());
  urls.erase(std::unique(urls.begin(), urls.end()), urls.end());
  return urls;
}

void InvalidateModules(v8::Isolate* isolate, v8::Local<v8::Context> context,
                       const std::vector<std::string>& urls) {
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  if (urls.empty()) return;

  robin_hood::unordered_set<std::string> seen;
  std::vector<std::string> uniqueUrls;
  uniqueUrls.reserve(urls.size());

  for (const auto& url : urls) {
    if (url.empty()) continue;
    std::string registryKey = CanonicalizeRegistryKey(url);
    if (registryKey.empty()) continue;
    if (!seen.insert(registryKey).second) continue;
    uniqueUrls.push_back(registryKey);
  }

  const bool logScriptLoading = IsScriptLoadingLogEnabled();
  size_t hits = 0, misses = 0;
  for (const auto& url : uniqueUrls) {
    bool present = g_moduleRegistry.find(url) != g_moduleRegistry.end();
    if (present) hits++;
    else misses++;
    if (logScriptLoading) {
      DEBUG_WRITE("[ns-hmr][android-invalidate] %s key=%s",
                  present ? "HIT " : "MISS", url.c_str());
    }
    RejectAndClearInvalidatedModuleState(isolate, context, url);
    RemoveModuleFromRegistry(url);
  }

  // Second layer: the OS HTTP cache is outside our control and may serve
  // a previous save's body even with no-store headers. Mark every
  // invalidated key so the NEXT network fetch carries a unique
  // `__ns_dev_nonce` query param — the network sees a URL it has never
  // cached and must go to origin. The nonce is transport-only; module
  // identity stays the canonical URL.
  MarkUrlsForCacheBust(uniqueUrls);

  if (logScriptLoading) {
    DEBUG_WRITE(
        "[ns-hmr][android-invalidate] summary unique=%lu hits=%lu misses=%lu "
        "(registry now=%lu)",
        (unsigned long)uniqueUrls.size(), (unsigned long)hits,
        (unsigned long)misses, (unsigned long)g_moduleRegistry.size());
  }
}

void UpdateModuleFallback(v8::Isolate* isolate,
                          const std::string& canonicalPath,
                          v8::Local<v8::Module> module) {
  auto& g_moduleFallbackRegistry = ModuleFallbackRegistryFor(isolate);
  auto& g_moduleFallbackByRelative = ModuleFallbackByRelativeFor(isolate);
  auto fallbackIt = g_moduleFallbackRegistry.find(canonicalPath);
  if (fallbackIt != g_moduleFallbackRegistry.end()) {
    fallbackIt->second.Reset();
  }
  if (!module.IsEmpty()) {
    g_moduleFallbackRegistry[canonicalPath].Reset(isolate, module);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver] fallback updated for %s from evaluated module",
                  canonicalPath.c_str());
    }
    std::string relative = ExtractRelativePath(canonicalPath);
    if (!relative.empty()) {
      auto relativeIt = g_moduleFallbackByRelative.find(relative);
      if (relativeIt != g_moduleFallbackByRelative.end()) {
        relativeIt->second.Reset();
      }
      g_moduleFallbackByRelative[relative].Reset(isolate, module);
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[resolver] fallback relative updated for %s",
                    relative.c_str());
      }
    }
  }
}

// ─────────────────────────────────────────────────────────────
// Thread-local resolver state
//
// Recursion detection + module in-flight/waiter tracking. Everything here is
// touched only from the isolate's own JS thread, so thread_local is safe.
static thread_local std::vector<std::string> g_moduleResolutionStack;
static thread_local robin_hood::unordered_map<std::string, size_t> g_moduleReentryCounts;
static thread_local robin_hood::unordered_map<std::string,
                                              robin_hood::unordered_set<std::string>>
    g_moduleReentryParents;
static thread_local robin_hood::unordered_map<std::string, std::string> g_modulePrimaryImporters;
static thread_local robin_hood::unordered_set<std::string> g_modulesInFlight;
static thread_local robin_hood::unordered_set<std::string> g_modulesPendingReset;
static constexpr size_t kMaxModuleReentryCount = 256;
// Waiters: module registry key -> list of Promise resolvers waiting for
// completion (instantiated/evaluated or errored).
static robin_hood::unordered_map<std::string,
                                 std::vector<v8::Global<v8::Promise::Resolver>>>
    g_moduleWaiters;
// Dynamic HTTP import waiters: resolve to module namespace when available.
static thread_local robin_hood::unordered_map<
    std::string, std::vector<v8::Global<v8::Promise::Resolver>>>
    g_httpDynamicWaiters;

static bool IsModuleEvaluationInProgress(v8::Module::Status status) {
  return status == v8::Module::kInstantiating ||
         status == v8::Module::kEvaluating;
}

static void ResolveResolversWithModuleNamespace(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    std::vector<v8::Global<v8::Promise::Resolver>>& resolvers,
    v8::Local<v8::Module> module, const std::string& registryKey) {
  if (resolvers.empty()) return;
  if (module.IsEmpty() || module->GetStatus() != v8::Module::kEvaluated) {
    std::string msg = "Module did not finish evaluation: " + registryKey;
    v8::Local<v8::Value> errObj =
        v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg));
    for (auto& resGlobal : resolvers) {
      v8::Local<v8::Promise::Resolver> resolver = resGlobal.Get(isolate);
      if (!resolver.IsEmpty()) {
        resolver->Reject(context, errObj).FromMaybe(false);
      }
      resGlobal.Reset();
    }
    return;
  }
  v8::Local<v8::Value> moduleNamespace = module->GetModuleNamespace();
  for (auto& resGlobal : resolvers) {
    v8::Local<v8::Promise::Resolver> resolver = resGlobal.Get(isolate);
    if (!resolver.IsEmpty()) {
      resolver->Resolve(context, moduleNamespace).FromMaybe(false);
    }
    resGlobal.Reset();
  }
}

static void RejectResolversWithReason(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    std::vector<v8::Global<v8::Promise::Resolver>>& resolvers,
    v8::Local<v8::Value> reason) {
  if (resolvers.empty()) return;
  for (auto& resGlobal : resolvers) {
    v8::Local<v8::Promise::Resolver> resolver = resGlobal.Get(isolate);
    if (!resolver.IsEmpty()) {
      resolver->Reject(context, reason).FromMaybe(false);
    }
    resGlobal.Reset();
  }
}

static bool QueueModuleWaiterIfInFlight(v8::Isolate* isolate,
                                        const std::string& registryKey,
                                        v8::Local<v8::Module> module,
                                        v8::Local<v8::Promise::Resolver> resolver) {
  if (registryKey.empty() || module.IsEmpty() ||
      !IsModuleEvaluationInProgress(module->GetStatus()) ||
      g_modulesInFlight.find(registryKey) == g_modulesInFlight.end()) {
    return false;
  }
  g_moduleWaiters[registryKey].emplace_back(isolate, resolver);
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dyn-import][await] queued module waiter for %s status=%s",
                registryKey.c_str(),
                ModuleStatusToString(module->GetStatus()));
  }
  return true;
}

static bool QueueHttpDynamicWaiterIfInFlight(
    v8::Isolate* isolate, const std::string& registryKey,
    v8::Local<v8::Module> module, v8::Local<v8::Promise::Resolver> resolver) {
  if (registryKey.empty() || module.IsEmpty() ||
      !IsModuleEvaluationInProgress(module->GetStatus()) ||
      g_modulesInFlight.find(registryKey) == g_modulesInFlight.end()) {
    return false;
  }
  g_httpDynamicWaiters[registryKey].emplace_back(isolate, resolver);
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dyn-import][http-await] queued waiter for %s status=%s",
                registryKey.c_str(),
                ModuleStatusToString(module->GetStatus()));
  }
  return true;
}

// Build a rejection reason that PRESERVES the underlying V8 exception text.
static v8::Local<v8::Value> BuildModuleFailureReason(v8::Isolate* isolate,
                                                     v8::TryCatch& tc,
                                                     const char* stage,
                                                     const std::string& urlOrKey) {
  std::string message = std::string(stage) + ": " + urlOrKey;
  if (tc.HasCaught()) {
    v8::Local<v8::Message> excMessage = tc.Message();
    if (!excMessage.IsEmpty()) {
      v8::String::Utf8Value text(isolate, excMessage->Get());
      if (*text != nullptr && strlen(*text) > 0) {
        message += std::string(" — ") + *text;
      }
    } else {
      v8::Local<v8::Value> exception = tc.Exception();
      if (!exception.IsEmpty()) {
        v8::String::Utf8Value text(isolate, exception);
        if (*text != nullptr && strlen(*text) > 0) {
          message += std::string(" — ") + *text;
        }
      }
    }
  }
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dyn-import][failure] %s", message.c_str());
  }
  return v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, message));
}

static void ResolveModuleWaiters(v8::Isolate* isolate,
                                 v8::Local<v8::Context> context,
                                 const std::string& registryKey,
                                 v8::Local<v8::Module> module) {
  auto waitIt = g_moduleWaiters.find(registryKey);
  if (waitIt == g_moduleWaiters.end()) return;
  std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
  resolvers.swap(waitIt->second);
  g_moduleWaiters.erase(waitIt);
  ResolveResolversWithModuleNamespace(isolate, context, resolvers, module,
                                      registryKey);
}

static void RejectModuleWaiters(v8::Isolate* isolate,
                                v8::Local<v8::Context> context,
                                const std::string& registryKey,
                                v8::Local<v8::Value> reason) {
  auto waitIt = g_moduleWaiters.find(registryKey);
  if (waitIt == g_moduleWaiters.end()) return;
  std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
  resolvers.swap(waitIt->second);
  g_moduleWaiters.erase(waitIt);
  RejectResolversWithReason(isolate, context, resolvers, reason);
}

static void ResolveHttpDynamicWaiters(v8::Isolate* isolate,
                                      v8::Local<v8::Context> context,
                                      const std::string& registryKey,
                                      v8::Local<v8::Module> module) {
  auto waitIt = g_httpDynamicWaiters.find(registryKey);
  if (waitIt != g_httpDynamicWaiters.end()) {
    std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
    resolvers.swap(waitIt->second);
    g_httpDynamicWaiters.erase(waitIt);
    ResolveResolversWithModuleNamespace(isolate, context, resolvers, module,
                                        registryKey);
  }
  g_modulesInFlight.erase(registryKey);
}

static void RejectHttpDynamicWaiters(v8::Isolate* isolate,
                                     v8::Local<v8::Context> context,
                                     const std::string& registryKey,
                                     v8::Local<v8::Value> reason) {
  auto waitIt = g_httpDynamicWaiters.find(registryKey);
  if (waitIt != g_httpDynamicWaiters.end()) {
    std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
    resolvers.swap(waitIt->second);
    g_httpDynamicWaiters.erase(waitIt);
    RejectResolversWithReason(isolate, context, resolvers, reason);
  }
  g_modulesInFlight.erase(registryKey);
}

static void RejectResolversForInvalidation(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    std::vector<v8::Global<v8::Promise::Resolver>>& resolvers,
    const std::string& registryKey) {
  if (resolvers.empty()) return;
  std::string message = "Module invalidated during dev reload: " + registryKey;
  v8::Local<v8::Value> error =
      v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, message));
  for (auto& resolverGlobal : resolvers) {
    v8::Local<v8::Promise::Resolver> resolver = resolverGlobal.Get(isolate);
    if (!resolver.IsEmpty()) {
      resolver->Reject(context, error).FromMaybe(false);
    }
    resolverGlobal.Reset();
  }
}

static void RejectAndClearInvalidatedModuleState(v8::Isolate* isolate,
                                                 v8::Local<v8::Context> context,
                                                 const std::string& registryKey) {
  g_moduleReentryCounts.erase(registryKey);
  g_moduleReentryParents.erase(registryKey);
  g_modulePrimaryImporters.erase(registryKey);
  g_modulesInFlight.erase(registryKey);
  g_modulesPendingReset.erase(registryKey);

  auto waitIt = g_moduleWaiters.find(registryKey);
  if (waitIt != g_moduleWaiters.end()) {
    std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
    resolvers.swap(waitIt->second);
    g_moduleWaiters.erase(waitIt);
    RejectResolversForInvalidation(isolate, context, resolvers, registryKey);
  }

  auto dynamicWaitIt = g_httpDynamicWaiters.find(registryKey);
  if (dynamicWaitIt != g_httpDynamicWaiters.end()) {
    std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
    resolvers.swap(dynamicWaitIt->second);
    g_httpDynamicWaiters.erase(dynamicWaitIt);
    RejectResolversForInvalidation(isolate, context, resolvers, registryKey);
  }
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[resolver][invalidate-state] cleared in-flight state for %s",
                registryKey.c_str());
  }
}

namespace {
struct ResolutionStackGuard {
  ResolutionStackGuard(v8::Isolate* isolate, std::vector<std::string>& stack,
                       const std::string& entry)
      : isolate_(isolate), stack_(stack), entry_(entry), active_(true) {
    stack_.push_back(entry_);
    g_moduleReentryCounts[entry_] = 0;
    g_moduleReentryParents.erase(entry_);
    if (stack_.size() > 1) {
      g_modulePrimaryImporters[entry_] = stack_[stack_.size() - 2];
    } else {
      g_modulePrimaryImporters.erase(entry_);
    }
    g_modulesInFlight.insert(entry_);
    g_modulesPendingReset.erase(entry_);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][stack] push (%lu) %s",
                  static_cast<unsigned long>(stack_.size()), entry_.c_str());
    }
  }

  ~ResolutionStackGuard() {
    if (!active_ || stack_.empty()) return;
    auto& g_moduleRegistry = ModuleRegistryFor(isolate_);
    auto& g_moduleFallbackRegistry = ModuleFallbackRegistryFor(isolate_);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][stack] pop (%lu) %s",
                  static_cast<unsigned long>(stack_.size()), entry_.c_str());
    }
    g_moduleReentryCounts.erase(entry_);
    g_moduleReentryParents.erase(entry_);
    g_modulePrimaryImporters.erase(entry_);
    g_modulesInFlight.erase(entry_);

    v8::Module::Status finalStatus = v8::Module::kErrored;
    auto regIt = g_moduleRegistry.find(entry_);
    if (regIt != g_moduleRegistry.end()) {
      v8::Local<v8::Module> m = regIt->second.Get(isolate_);
      if (!m.IsEmpty()) finalStatus = m->GetStatus();
    }
    bool isError = finalStatus == v8::Module::kErrored;
    auto waitIt = g_moduleWaiters.find(entry_);
    if (waitIt != g_moduleWaiters.end()) {
      v8::Local<v8::Context> currentContext = isolate_->GetCurrentContext();
      if (isError || regIt == g_moduleRegistry.end()) {
        std::string msg = "Module evaluation failed: " + entry_;
        RejectModuleWaiters(
            isolate_, currentContext, entry_,
            v8::Exception::Error(ArgConverter::ConvertToV8String(isolate_, msg)));
      } else {
        v8::Local<v8::Module> resolvedModule = regIt->second.Get(isolate_);
        ResolveModuleWaiters(isolate_, currentContext, entry_, resolvedModule);
      }
    }
    stack_.pop_back();
    auto pendingIt = g_modulesPendingReset.find(entry_);
    if (pendingIt != g_modulesPendingReset.end()) {
      auto it = g_moduleRegistry.find(entry_);
      if (it != g_moduleRegistry.end()) {
        v8::Local<v8::Module> module = it->second.Get(isolate_);
        v8::Module::Status status =
            module.IsEmpty() ? v8::Module::kErrored : module->GetStatus();
        if (status != v8::Module::kEvaluated && status != v8::Module::kErrored) {
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE(
                "[resolver] dropping incomplete module after unwind %s (status=%s)",
                entry_.c_str(), ModuleStatusToString(status));
          }
          RemoveModuleFromRegistry(entry_);
        }
      }
      g_modulesPendingReset.erase(pendingIt);
    }

    auto activeIt = g_moduleRegistry.find(entry_);
    if (activeIt != g_moduleRegistry.end()) {
      v8::Local<v8::Module> activeModule = activeIt->second.Get(isolate_);
      if (!activeModule.IsEmpty() &&
          activeModule->GetStatus() == v8::Module::kEvaluated) {
        g_moduleFallbackRegistry[entry_].Reset(isolate_, activeModule);
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE(
              "[resolver] updated fallback module for %s after successful evaluation",
              entry_.c_str());
        }
      }
    }
  }

  void Release() { active_ = false; }

 private:
  v8::Isolate* isolate_;
  std::vector<std::string>& stack_;
  std::string entry_;
  bool active_;
};
}  // namespace

// ─────────────────────────────────────────────────────────────
// JSON module → synthetic ES module

// Compile a `.json` file as an ES module whose default export is the parsed
// JSON value. Handles registry insertion and eager evaluation.
static v8::MaybeLocal<v8::Module> CompileJsonAsEsModule(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& absPath, const std::string& registryAbsPath,
    bool isWorker) {
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  if (isWorker && IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[resolver] Worker handling JSON module '%s'", absPath.c_str());
  }

  std::string jsonText = Runtime::GetRuntime(isolate)->ReadFileText(absPath);
  std::string moduleSource = "export default " + jsonText + ";";
  v8::Local<v8::String> sourceText =
      ArgConverter::ConvertToV8String(isolate, moduleSource);
  std::string url = "file://" + absPath;

  v8::Local<v8::String> urlString;
  if (!v8::String::NewFromUtf8(isolate, url.c_str(),
                               v8::NewStringType::kNormal)
           .ToLocal(&urlString)) {
    isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(
        isolate, "Failed to create URL string for JSON module")));
    return v8::MaybeLocal<v8::Module>();
  }

  v8::ScriptOrigin origin(urlString, 0, 0, false, -1, v8::Local<v8::Value>(),
                          false, false, true /* is_module */);
  v8::ScriptCompiler::Source src(sourceText, origin);

  v8::Local<v8::Module> jsonModule;
  if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&jsonModule)) {
    isolate->ThrowException(v8::Exception::SyntaxError(
        ArgConverter::ConvertToV8String(isolate, "Failed to compile JSON module")));
    return v8::MaybeLocal<v8::Module>();
  }

  if (!jsonModule->InstantiateModule(context, &ResolveModuleCallback)
           .FromMaybe(false)) {
    return v8::MaybeLocal<v8::Module>();
  }
  v8::MaybeLocal<v8::Value> evalResult = jsonModule->Evaluate(context);
  if (evalResult.IsEmpty()) return v8::MaybeLocal<v8::Module>();

  auto it = g_moduleRegistry.find(registryAbsPath);
  if (it != g_moduleRegistry.end()) it->second.Reset();
  g_moduleRegistry[registryAbsPath].Reset(isolate, jsonModule);
  return v8::MaybeLocal<v8::Module>(jsonModule);
}

// ─────────────────────────────────────────────────────────────
// node: builtin polyfills (Android). iOS ships node:url only; Android has
// carried node:url / node:module / node:path shims for longer. Kept here to
// avoid a behavior regression relative to current Android main.
static const char* NodeUrlPolyfill() {
  return "// In-memory polyfill for node:url\n"
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
}

static const char* NodeModulePolyfill() {
  return "// In-memory polyfill for node:module\n"
         "export function createRequire(filename) {\n"
         "  if (typeof require === 'function') {\n"
         "    return require;\n"
         "  }\n"
         "  return function(id) {\n"
         "    throw new Error('Module ' + id + ' not found. NativeScript require() not available.');\n"
         "  };\n"
         "}\n"
         "export default { createRequire };\n";
}

static const char* NodePathPolyfill() {
  return "// In-memory polyfill for node:path\n"
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
}

// Compile + register a node: builtin polyfill under `key`. Returns the
// compiled (but not instantiated) module on success.
static v8::MaybeLocal<v8::Module> CompileNodeBuiltinPolyfill(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& spec, const std::string& key) {
  const std::string builtinName = spec.substr(5);  // drop "node:"
  const char* polyfill = nullptr;
  if (builtinName == "url") polyfill = NodeUrlPolyfill();
  else if (builtinName == "module") polyfill = NodeModulePolyfill();
  else if (builtinName == "path") polyfill = NodePathPolyfill();
  else {
    isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(
        isolate, NsBuiltinModules::NotFoundMessage(spec))));
    return v8::MaybeLocal<v8::Module>();
  }
  return CompileModuleForResolveRegisterOnly(isolate, context, polyfill, key);
}

// ─────────────────────────────────────────────────────────────
// ResolveModuleCallback — invoked by V8 to resolve `import X from '<spec>'`.
//
// Structure mirrors iOS: import-map first, then HTTP fast path, then
// filesystem resolution against the application root using the Android
// virtual-root mappings (file:///app/ and file:///android_asset/app/).

v8::MaybeLocal<v8::Module> ResolveModuleCallback(
    v8::Local<v8::Context> context, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> /*import_assertions*/,
    v8::Local<v8::Module> referrer) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);
  auto& g_moduleFallbackRegistry = ModuleFallbackRegistryFor(isolate);

  v8::String::Utf8Value specUtf8(isolate, specifier);
  const std::string rawSpec = *specUtf8 ? *specUtf8 : "";
  if (rawSpec.empty()) return v8::MaybeLocal<v8::Module>();

  // Builtins resolve before any path handling.
  if (NsBuiltinModules::IsRegistered(rawSpec) ||
      NsBuiltinModules::IsNsScheme(rawSpec)) {
    v8::Local<v8::Module> builtin;
    if (NsBuiltinModules::GetModule(context, rawSpec).ToLocal(&builtin)) {
      return v8::MaybeLocal<v8::Module>(builtin);
    }
    if (!NsBuiltinModules::IsRegistered(rawSpec)) {
      isolate->ThrowException(
          v8::Exception::Error(ArgConverter::ConvertToV8String(
              isolate, NsBuiltinModules::NotFoundMessage(rawSpec))));
    }
    return v8::MaybeLocal<v8::Module>();
  }

  std::string normalizedSpec = rawSpec;
  // Repair malformed http:/ or https:/ prefixes so the HTTP fast path fires.
  if (normalizedSpec.rfind("http:/", 0) == 0 &&
      normalizedSpec.rfind("http://", 0) != 0) {
    normalizedSpec.insert(5, "/");
  } else if (normalizedSpec.rfind("https:/", 0) == 0 &&
             normalizedSpec.rfind("https://", 0) != 0) {
    normalizedSpec.insert(6, "/");
  }

  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[resolver][spec] %s", normalizedSpec.c_str());
  }

  // Guard against a bare '@' spec — invalid; refuse to poison the registry.
  if (normalizedSpec == "@") {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][normalize] ignoring invalid '@' static spec");
    }
    return v8::MaybeLocal<v8::Module>();
  }

  // Import map resolution (bare specifiers → resolved URLs).
  if (!g_importMap.empty()) {
    std::string mapped = LookupImportMap(normalizedSpec);
    if (mapped.empty()) {
      std::string normalized = NormalizeViteSpecifier(normalizedSpec);
      if (!normalized.empty()) {
        mapped = LookupImportMap(normalized);
        if (!mapped.empty() && IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[resolver][import-map] normalized: %s -> %s -> %s",
                      normalizedSpec.c_str(), normalized.c_str(),
                      mapped.c_str());
        }
      }
    }
    if (!mapped.empty()) {
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[resolver][import-map] rewrite: %s -> %s",
                    normalizedSpec.c_str(), mapped.c_str());
      }
      normalizedSpec = mapped;
    } else {
      bool looksBare = !normalizedSpec.empty() && normalizedSpec[0] != '/' &&
                       normalizedSpec[0] != '.' &&
                       normalizedSpec.find("://") == std::string::npos &&
                       normalizedSpec.find('\\') == std::string::npos;
      if (looksBare && IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE(
            "[resolver][import-map][miss] bare='%s' importMap.size=%lu",
            normalizedSpec.c_str(), (unsigned long)g_importMap.size());
      }
    }
  }

  const std::string& spec = normalizedSpec;

  // Early absolute-HTTP fast path.
  if (StartsWith(spec, "http://") || StartsWith(spec, "https://")) {
    return LoadHttpModuleForUrl(isolate, context, spec);
  }

  const bool isWorker = IsCurrentIsolateWorker(isolate);
  if (isWorker && IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[resolver] Worker trying to resolve '%s'", spec.c_str());
  }

  // Find the referrer's registered path so we can resolve relative specs
  // against its directory.
  std::string referrerPath;
  for (auto& kv : g_moduleRegistry) {
    v8::Local<v8::Module> registered = kv.second.Get(isolate);
    if (!registered.IsEmpty() && registered == referrer) {
      referrerPath = kv.first;
      break;
    }
  }
  bool specIsRelative = !spec.empty() && spec[0] == '.';
  if (referrerPath.empty() && specIsRelative) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver] No referrer for relative '%s' - assuming app root",
                  spec.c_str());
    }
    referrerPath = GetApplicationPath() + "/index.mjs";
  }

  size_t slash = referrerPath.find_last_of("/\\");
  std::string baseDir =
      slash == std::string::npos ? "" : referrerPath.substr(0, slash + 1);

  // Relative or root-absolute against an HTTP referrer resolves via HTTP.
  bool referrerIsHttp = !referrerPath.empty() &&
                        (StartsWith(referrerPath, "http://") ||
                         StartsWith(referrerPath, "https://"));
  bool specIsRootAbs = !spec.empty() && spec[0] == '/';
  if (referrerIsHttp && (specIsRelative || specIsRootAbs)) {
    std::string resolvedHttp = ResolveHttpRelative(referrerPath, spec);
    if (!resolvedHttp.empty() &&
        (StartsWith(resolvedHttp, "http://") ||
         StartsWith(resolvedHttp, "https://"))) {
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[resolver][http-rel] base=%s spec=%s -> %s",
                    referrerPath.c_str(), spec.c_str(), resolvedHttp.c_str());
      }
      return LoadHttpModuleForUrl(isolate, context, resolvedHttp);
    }
  } else if (!referrerIsHttp && specIsRootAbs) {
    // Fallback: use __NS_HTTP_ORIGIN__ if present to anchor bare root-absolute
    // specs (matches historical Android behavior).
    v8::Local<v8::String> key =
        ArgConverter::ConvertToV8String(isolate, "__NS_HTTP_ORIGIN__");
    v8::Local<v8::Object> global = context->Global();
    v8::MaybeLocal<v8::Value> maybeOriginVal = global->Get(context, key);
    v8::Local<v8::Value> originVal;
    if (!maybeOriginVal.IsEmpty() && maybeOriginVal.ToLocal(&originVal) &&
        originVal->IsString()) {
      v8::String::Utf8Value o8(isolate, originVal);
      std::string origin = *o8 ? *o8 : "";
      if (!origin.empty() && (StartsWith(origin, "http://") ||
                              StartsWith(origin, "https://"))) {
        std::string refBase = origin;
        if (refBase.back() != '/') refBase += '/';
        std::string resolved = ResolveHttpRelative(refBase, spec);
        if (StartsWith(resolved, "http://") ||
            StartsWith(resolved, "https://")) {
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[resolver][http-origin][fallback] origin=%s spec=%s -> %s",
                        refBase.c_str(), spec.c_str(), resolved.c_str());
          }
          return LoadHttpModuleForUrl(isolate, context, resolved);
        }
      }
    }
  }

  // ── Build filesystem candidate paths ──
  const std::string appPath = GetApplicationPath();
  std::vector<std::string> candidateBases;

  if (!spec.empty() && spec[0] == '.') {
    std::string cleanSpec = spec.rfind("./", 0) == 0 ? spec.substr(2) : spec;
    std::string candidate = NormalizePath(baseDir + cleanSpec);
    candidateBases.push_back(candidate);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][normalize-rel] %s + %s -> %s", baseDir.c_str(),
                  cleanSpec.c_str(), candidate.c_str());
    }
  } else if (StartsWith(spec, "file://")) {
    // Absolute file URL. Handle the two virtual roots the runtime emits.
    std::string tail = spec.substr(7);
    if (tail.empty() || tail[0] != '/') tail = "/" + tail;

    const std::string appVirtualRoot = "/app/";
    const std::string androidAssetAppRoot = "/android_asset/app/";
    std::string candidate;
    if (tail.rfind(appVirtualRoot, 0) == 0) {
      candidate = appPath + "/" + tail.substr(appVirtualRoot.size());
    } else if (tail.rfind(androidAssetAppRoot, 0) == 0) {
      candidate = appPath + "/" + tail.substr(androidAssetAppRoot.size());
    } else if (tail.rfind(appPath, 0) == 0) {
      candidate = tail;
    } else {
      candidate = tail;
    }
    candidateBases.push_back(NormalizePath(candidate));
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][file-url] tail=%s -> %s", tail.c_str(),
                  candidateBases.back().c_str());
    }
  } else if (!spec.empty() && spec[0] == '~') {
    std::string tail = spec.size() >= 2 && spec[1] == '/' ? spec.substr(2)
                                                          : spec.substr(1);
    std::string base = NormalizePath(appPath + "/" + tail);
    candidateBases.push_back(base);
    // Also try appPath/app for projects that bundle JS under an app folder.
    std::string baseApp = NormalizePath(appPath + "/app/" + tail);
    if (baseApp != base) candidateBases.push_back(baseApp);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][tilde] spec=%s base=%s appBase=%s", spec.c_str(),
                  base.c_str(), baseApp.c_str());
    }
  } else if (!spec.empty() && spec[0] == '/') {
    // Absolute path. Dynamic import may already have resolved a relative
    // specifier to a real filesystem path under the application root; use
    // that as-is so we don't prefix ApplicationPath twice. Bundle-relative
    // paths like /app/... or /src/... still resolve against appPath.
    if (!appPath.empty() && spec.rfind(appPath, 0) == 0) {
      candidateBases.push_back(NormalizePath(spec));
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[resolver][abs-fs] spec=%s", spec.c_str());
      }
    } else {
      std::string base = NormalizePath(appPath + spec);
      candidateBases.push_back(base);
      const std::string appPrefix = "/app/";
      if (spec.rfind(appPrefix, 0) == 0) {
        std::string tailNoApp = spec.substr(appPrefix.size() - 1);
        std::string baseNoApp = NormalizePath(appPath + tailNoApp);
        if (baseNoApp != base) candidateBases.push_back(baseNoApp);
      }
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[resolver][abs] spec=%s base=%s", spec.c_str(),
                    base.c_str());
      }
    }
  } else {
    // Bare specifier — resolve relative to the application root.
    std::string base = NormalizePath(appPath + "/" + spec);
    candidateBases.push_back(base);
    // Underscore-separated bundler chunk heuristic.
    std::string withSlashes = spec;
    std::replace(withSlashes.begin(), withSlashes.end(), '_', '/');
    std::string baseSlashes = NormalizePath(appPath + "/" + withSlashes);
    if (baseSlashes != base) candidateBases.push_back(baseSlashes);
  }

  // Reroute a candidate that accidentally embeds a collapsed HTTP URL.
  auto rerouteHttpIfEmbedded = [&](const std::string& p,
                                   v8::MaybeLocal<v8::Module>* moduleOut) -> bool {
    size_t pos1 = p.find("/http:/");
    size_t pos2 = p.find("/https:/");
    size_t pos = std::min(pos1 == std::string::npos ? SIZE_MAX : pos1,
                          pos2 == std::string::npos ? SIZE_MAX : pos2);
    if (pos == SIZE_MAX) return false;
    std::string tail = p.substr(pos + 1);
    if (StartsWith(tail, "http:/") && !StartsWith(tail, "http://")) {
      tail.insert(5, "/");
    } else if (StartsWith(tail, "https:/") && !StartsWith(tail, "https://")) {
      tail.insert(6, "/");
    }
    if (!(StartsWith(tail, "http://") || StartsWith(tail, "https://")))
      return false;
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[resolver][http-embedded] %s -> %s", p.c_str(), tail.c_str());
    }
    if (moduleOut != nullptr) {
      *moduleOut = LoadHttpModuleForUrl(isolate, context, tail);
    }
    return true;
  };

  // ── Resolve on disk ──
  std::string absPath;
  bool found = false;

  for (const std::string& baseCandidate : candidateBases) {
    absPath = baseCandidate;

    v8::MaybeLocal<v8::Module> embeddedHttpModule;
    if (rerouteHttpIfEmbedded(absPath, &embeddedHttpModule)) {
      return embeddedHttpModule;
    }

    if (IsFile(absPath)) {
      found = true;
      break;
    }
    const char* exts[] = {".mjs", ".js"};
    for (const char* e : exts) {
      std::string cand = NormalizePath(WithExtension(absPath, e));
      if (IsFile(cand)) {
        absPath = cand;
        found = true;
        break;
      }
    }
    if (found) break;
    const char* idxExts[] = {"/index.mjs", "/index.js"};
    for (const char* idx : idxExts) {
      std::string cand = NormalizePath(absPath + idx);
      if (IsFile(cand)) {
        absPath = cand;
        found = true;
        break;
      }
    }
    if (found) break;
  }

  if (found) absPath = NormalizePath(absPath);
  const std::string registryAbsPath = CanonicalizeRegistryKey(absPath);

  if (!found) {
    // node: builtins that don't exist on disk get an in-memory polyfill
    // module. Anything else throws Cannot find module (matches iOS HEAD;
    // no optional-module empty-return placeholder).
    if (IsNodeBuiltinModule(spec)) {
      std::string key = spec;  // e.g. "node:url"
      auto itExisting = g_moduleRegistry.find(key);
      if (itExisting != g_moduleRegistry.end()) {
        v8::Local<v8::Module> existing = itExisting->second.Get(isolate);
        if (!existing.IsEmpty() && existing->GetStatus() != v8::Module::kErrored) {
          return v8::MaybeLocal<v8::Module>(existing);
        }
        RemoveModuleFromRegistry(key);
      }
      v8::MaybeLocal<v8::Module> m =
          CompileNodeBuiltinPolyfill(isolate, context, spec, key);
      v8::Local<v8::Module> mod;
      if (m.ToLocal(&mod)) return m;
      // CompileNodeBuiltinPolyfill already threw (unknown builtin, or
      // compile failure). Do not overwrite that exception.
      return v8::MaybeLocal<v8::Module>();
    }
    std::string msg = "Cannot find module '" + spec + "' (tried " + absPath + ")";
    isolate->ThrowException(
        v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
    return v8::MaybeLocal<v8::Module>();
  }

  // JSON module: compile a synthetic ESM.
  if (EndsWith(absPath, ".json")) {
    return CompileJsonAsEsModule(isolate, context, absPath, registryAbsPath,
                                 isWorker);
  }

  // Cache lookup.
  auto it = g_moduleRegistry.find(registryAbsPath);
  if (it != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = it->second.Get(isolate);
    v8::Module::Status status =
        existing.IsEmpty() ? v8::Module::kErrored : existing->GetStatus();
    bool inCurrentStack =
        std::find(g_moduleResolutionStack.begin(),
                  g_moduleResolutionStack.end(),
                  registryAbsPath) != g_moduleResolutionStack.end();
    bool shouldReuse = !existing.IsEmpty() && status != v8::Module::kErrored;
    if (shouldReuse &&
        (status == v8::Module::kUninstantiated ||
         status == v8::Module::kInstantiating ||
         status == v8::Module::kEvaluating)) {
      if (!inCurrentStack) shouldReuse = false;
    }
    if (shouldReuse) {
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[resolver] cache hit %s (status=%s)", absPath.c_str(),
                    ModuleStatusToString(status));
      }
      return v8::MaybeLocal<v8::Module>(existing);
    }
    if (!existing.IsEmpty() && status == v8::Module::kEvaluated) {
      auto fallbackIt = g_moduleFallbackRegistry.find(registryAbsPath);
      if (fallbackIt != g_moduleFallbackRegistry.end()) {
        fallbackIt->second.Reset();
      }
      g_moduleFallbackRegistry[registryAbsPath].Reset(isolate, existing);
    }
    RemoveModuleFromRegistry(absPath);
  }

  // Detect recursive load prior to LoadESModule.
  auto cycleIt = std::find(g_moduleResolutionStack.begin(),
                           g_moduleResolutionStack.end(), registryAbsPath);
  if (cycleIt != g_moduleResolutionStack.end()) {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE(
          "[resolver] Detected recursive load for %s (stack len %lu)",
          absPath.c_str(), (unsigned long)g_moduleResolutionStack.size());
    }
    auto existing = g_moduleRegistry.find(registryAbsPath);
    if (existing != g_moduleRegistry.end()) {
      return v8::MaybeLocal<v8::Module>(existing->second.Get(isolate));
    }
    if (IsDebuggable()) {
      DEBUG_WRITE("[resolver] Debug mode - empty return for recursive load: %s",
                  absPath.c_str());
      return v8::MaybeLocal<v8::Module>();
    }
    std::string msg = "Recursive module resolution detected for " + absPath;
    isolate->ThrowException(
        v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
    return v8::MaybeLocal<v8::Module>();
  }

  ResolutionStackGuard stackGuard(isolate, g_moduleResolutionStack,
                                  registryAbsPath);
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[resolver] -> LoadESModule %s", absPath.c_str());
  }
  try {
    tns::ModuleInternal::LoadESModule(isolate, absPath);
  } catch (NativeScriptException& ex) {
    if (isWorker) {
      DEBUG_WRITE("[resolver] Worker failed to compile '%s' -> '%s'",
                  spec.c_str(), absPath.c_str());
    }
    ex.ReThrowToV8();
    return v8::MaybeLocal<v8::Module>();
  }
  auto it2 = g_moduleRegistry.find(registryAbsPath);
  if (it2 == g_moduleRegistry.end()) {
    return v8::MaybeLocal<v8::Module>();
  }
  return v8::MaybeLocal<v8::Module>(it2->second.Get(isolate));
}

// ─────────────────────────────────────────────────────────────
// FinishHttpDynamicImport
//
// Called on the JS thread once the async graph walk has fetched (and
// registered as uninstantiated) the transitive closure for an HTTP dynamic
// import. Instantiates + evaluates the root and settles all queued
// dynamic-import waiters. Top-level await is fanned out to a Then handler so
// waiters only settle after the returned promise settles.
static void FinishHttpDynamicImport(v8::Isolate* isolate,
                                    v8::Local<v8::Context> context,
                                    const std::string& key,
                                    const std::string& requestUrl) {
  if (IsScriptLoadingLogEnabled()) {
    auto& g_moduleRegistry = ModuleRegistryFor(isolate);
    if (g_moduleRegistry.find(key) == g_moduleRegistry.end()) {
      DEBUG_WRITE("[async-graph][fallback-sync-load] root missed walk: %s",
                  key.c_str());
    }
  }
  v8::MaybeLocal<v8::Module> modMaybe =
      LoadHttpModuleForUrl(isolate, context, requestUrl);
  if (!modMaybe.IsEmpty()) {
    v8::Local<v8::Module> mod;
    if (modMaybe.ToLocal(&mod)) {
      if (mod->GetStatus() == v8::Module::kUninstantiated) {
        v8::TryCatch tcInstantiate(isolate);
        if (!mod->InstantiateModule(context, &ResolveModuleCallback)
                 .FromMaybe(false)) {
          RemoveModuleFromRegistry(key);
          RejectHttpDynamicWaiters(
              isolate, context, key,
              BuildModuleFailureReason(isolate, tcInstantiate,
                                       "Instantiation failed (http-loader)",
                                       requestUrl));
          return;
        }
      }

      if (IsModuleEvaluationInProgress(mod->GetStatus())) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE(
              "[dyn-import][http-loader] waiting on existing evaluation for %s status=%s",
              key.c_str(), ModuleStatusToString(mod->GetStatus()));
        }
        return;
      }

      if (mod->GetStatus() != v8::Module::kEvaluated) {
        v8::Local<v8::Value> evalResult;
        {
          v8::TryCatch tcEvaluate(isolate);
          if (!mod->Evaluate(context).ToLocal(&evalResult)) {
            RemoveModuleFromRegistry(key);
            RejectHttpDynamicWaiters(
                isolate, context, key,
                BuildModuleFailureReason(isolate, tcEvaluate,
                                         "Evaluation failed (http-loader)",
                                         requestUrl));
            return;
          }
        }
        if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
          v8::Local<v8::Promise> p = evalResult.As<v8::Promise>();
          struct EvalWaitData2 {
            std::string key;
            v8::Global<v8::Context> ctx;
            v8::Global<v8::Module> mod;
          };
          auto* data2 = new EvalWaitData2{
              key, v8::Global<v8::Context>(isolate, context),
              v8::Global<v8::Module>(isolate, mod)};
          auto onFulfilled2 = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            v8::Isolate* iso = info.GetIsolate();
            v8::HandleScope hs(iso);
            if (!info.Data()->IsExternal()) return;
            auto* d = static_cast<EvalWaitData2*>(
                info.Data().As<v8::External>()->Value(
                    v8::kExternalPointerTypeTagDefault));
            v8::Local<v8::Context> ctx = d->ctx.Get(iso);
            std::string keyLocal = d->key;
            v8::Local<v8::Module> modLocal = d->mod.Get(iso);
            ResolveHttpDynamicWaiters(iso, ctx, keyLocal, modLocal);
            delete d;
          };
          auto onRejected2 = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            v8::Isolate* iso = info.GetIsolate();
            v8::HandleScope hs(iso);
            if (!info.Data()->IsExternal()) return;
            auto* d = static_cast<EvalWaitData2*>(
                info.Data().As<v8::External>()->Value(
                    v8::kExternalPointerTypeTagDefault));
            v8::Local<v8::Context> ctx = d->ctx.Get(iso);
            std::string keyLocal = d->key;
            v8::Local<v8::Value> reason =
                (info.Length() > 0)
                    ? info[0]
                    : v8::Exception::Error(ArgConverter::ConvertToV8String(
                          iso, "Evaluation failed (http-loader TLA)"));
            if (IsScriptLoadingLogEnabled()) {
              v8::String::Utf8Value r(iso, reason);
              if (*r) {
                DEBUG_WRITE("[dyn-import][http-loader][tla] rejected: %s", *r);
              }
            }
            RejectHttpDynamicWaiters(iso, ctx, keyLocal, reason);
            delete d;
          };
          v8::Local<v8::FunctionTemplate> thenFulfillTpl2 =
              v8::FunctionTemplate::New(
                  isolate, onFulfilled2,
                  v8::External::New(isolate, data2,
                                    v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Function> thenFulfill2 =
              thenFulfillTpl2->GetFunction(context).ToLocalChecked();
          v8::Local<v8::FunctionTemplate> thenRejectTpl2 =
              v8::FunctionTemplate::New(
                  isolate, onRejected2,
                  v8::External::New(isolate, data2,
                                    v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Function> thenReject2 =
              thenRejectTpl2->GetFunction(context).ToLocalChecked();
          p->Then(context, thenFulfill2, thenReject2).ToLocalChecked();
          return;
        }
      }
      ResolveHttpDynamicWaiters(isolate, context, key, mod);
      return;
    }
  }
  RejectHttpDynamicWaiters(
      isolate, context, key,
      v8::Exception::Error(
          ArgConverter::ConvertToV8String(isolate, "HTTP fetch/compile failed")));
}

// ─────────────────────────────────────────────────────────────
// ImportModuleDynamicallyCallback — host callback for `import()` expressions.
//
// Structure mirrors iOS: builtins → import-map → invalid-'@' guard → blob URL
// path → HTTP fast path (with coalescing + cache) → filesystem resolution via
// ResolveModuleCallback → instantiate/evaluate/TLA settle.
v8::MaybeLocal<v8::Promise> ImportModuleDynamicallyCallback(
    v8::Local<v8::Context> context, v8::Local<v8::Data> /*host_defined_options*/,
    v8::Local<v8::Value> resource_name, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> import_assertions) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);

  v8::String::Utf8Value specUtf8(isolate, specifier);
  const char* cSpec = (*specUtf8) ? *specUtf8 : "<invalid>";
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dyn-import] -> %s", cSpec);
    v8::Local<v8::Value> resName = resource_name;
    if (!resName.IsEmpty() && resName->IsString()) {
      v8::String::Utf8Value rn(isolate, resName);
      if (*rn) {
        DEBUG_WRITE("[dyn-import][referrer] %s", *rn);
      }
    }
  }

  std::string rawSpec = cSpec ? std::string(cSpec) : std::string();

  // Builtin modules never touch the loader below; the namespace comes straight
  // from the realm's synthetic module.
  if (NsBuiltinModules::IsRegistered(rawSpec) ||
      NsBuiltinModules::IsNsScheme(rawSpec)) {
    v8::EscapableHandleScope builtinScope(isolate);
    v8::Local<v8::Promise::Resolver> builtinResolver;
    if (!v8::Promise::Resolver::New(context).ToLocal(&builtinResolver)) {
      return v8::MaybeLocal<v8::Promise>();
    }
    v8::TryCatch tc(isolate);
    v8::Local<v8::Module> builtin;
    if (NsBuiltinModules::GetModule(context, rawSpec).ToLocal(&builtin)) {
      builtinResolver->Resolve(context, builtin->GetModuleNamespace())
          .FromMaybe(false);
    } else {
      v8::Local<v8::Value> error =
          tc.HasCaught()
              ? tc.Exception()
              : v8::Exception::Error(ArgConverter::ConvertToV8String(
                    isolate, NsBuiltinModules::NotFoundMessage(rawSpec)));
      // Reject must not run with a pending exception on the isolate.
      tc.Reset();
      builtinResolver->Reject(context, error).FromMaybe(false);
    }
    return builtinScope.Escape(builtinResolver->GetPromise());
  }

  std::string normalizedSpec = rawSpec;
  // remove query/hash ONLY for non-HTTP specs
  bool isHttpLike =
      (!normalizedSpec.empty() && (StartsWith(normalizedSpec, "http://") ||
                                    StartsWith(normalizedSpec, "https://")));
  if (!isHttpLike) {
    size_t qpos = normalizedSpec.find_first_of("?#");
    if (qpos != std::string::npos) {
      normalizedSpec = normalizedSpec.substr(0, qpos);
    }
  }
  if (normalizedSpec != rawSpec) {
    specifier = ArgConverter::ConvertToV8String(isolate, normalizedSpec);
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[dyn-import][normalize] %s -> %s", rawSpec.c_str(),
                  normalizedSpec.c_str());
    }
  }

  v8::EscapableHandleScope scope(isolate);

  v8::Local<v8::Promise::Resolver> resolver;
  if (!v8::Promise::Resolver::New(context).ToLocal(&resolver)) {
    return v8::MaybeLocal<v8::Promise>();
  }

  // ── Import map resolution for dynamic import() ──
  if (!g_importMap.empty() && !normalizedSpec.empty() && normalizedSpec != "@") {
    std::string mapped = LookupImportMap(normalizedSpec);
    if (mapped.empty()) {
      std::string normalized = NormalizeViteSpecifier(normalizedSpec);
      if (!normalized.empty()) {
        mapped = LookupImportMap(normalized);
        if (!mapped.empty() && IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][import-map] normalized: %s -> %s -> %s",
                      normalizedSpec.c_str(), normalized.c_str(),
                      mapped.c_str());
        }
      }
    }
    if (!mapped.empty()) {
      normalizedSpec = mapped;
      specifier = ArgConverter::ConvertToV8String(isolate, normalizedSpec);
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[dyn-import][import-map] rewrite: %s -> %s",
                    rawSpec.c_str(), normalizedSpec.c_str());
      }
    }
  }

  try {
    // Defensive guard: some dev-time toolchains emit a stray import('@') during
    // bootstrap. Treat it as a no-op module to avoid a hard failure.
    if (!normalizedSpec.empty() && normalizedSpec == "@") {
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE(
            "[dyn-import] ignoring invalid '@' spec (returning empty module)");
      }
      const char* kEmptySrc = "export {}\n";
      std::string url = "file:///app/__invalid_at__.mjs";
      v8::MaybeLocal<v8::Module> modMaybe =
          CompileModuleFromSource(isolate, context, kEmptySrc, url);
      v8::Local<v8::Module> mod;
      if (modMaybe.ToLocal(&mod)) {
        g_moduleRegistry[CanonicalizeRegistryKey(url)].Reset(isolate, mod);
        if (mod->GetStatus() != v8::Module::kEvaluated) {
          if (mod->Evaluate(context).IsEmpty()) {
            resolver
                ->Reject(context,
                         v8::Exception::Error(ArgConverter::ConvertToV8String(
                             isolate, "Evaluation failed for empty module")))
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          }
        }
        resolver->Resolve(context, mod->GetModuleNamespace()).FromMaybe(false);
        return scope.Escape(resolver->GetPromise());
      }
    }

    // ── Blob URL support (e.g. blob:nativescript/<uuid>) ──
    // Retrieve the blob content from the global BLOB_STORE via
    // URL.InternalAccessor.getData() (installed by Android's blob-url.js) and
    // compile it as an ES module.
    if (!normalizedSpec.empty() &&
        StartsWith(normalizedSpec, "blob:nativescript/")) {
      const std::string blobRegistryKey = CanonicalizeRegistryKey(normalizedSpec);
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[dyn-import][blob] trying blob URL %s key=%s",
                    normalizedSpec.c_str(), blobRegistryKey.c_str());
      }

      auto existingIt = g_moduleRegistry.find(blobRegistryKey);
      if (existingIt != g_moduleRegistry.end()) {
        v8::Local<v8::Module> existing = existingIt->second.Get(isolate);
        if (!existing.IsEmpty()) {
          v8::Module::Status existingStatus = existing->GetStatus();
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[dyn-import][blob-cache] hit %s status=%s",
                        blobRegistryKey.c_str(),
                        ModuleStatusToString(existingStatus));
          }
          if (existingStatus == v8::Module::kErrored) {
            RemoveModuleFromRegistry(blobRegistryKey);
          } else if (IsModuleEvaluationInProgress(existingStatus)) {
            g_modulesInFlight.insert(blobRegistryKey);
            g_httpDynamicWaiters[blobRegistryKey].emplace_back(isolate, resolver);
            if (IsScriptLoadingLogEnabled()) {
              DEBUG_WRITE(
                  "[dyn-import][blob-await] queued waiter for %s status=%s",
                  blobRegistryKey.c_str(), ModuleStatusToString(existingStatus));
            }
            return scope.Escape(resolver->GetPromise());
          } else {
            resolver->Resolve(context, existing->GetModuleNamespace())
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          }
        } else {
          RemoveModuleFromRegistry(blobRegistryKey);
        }
      }

      if (g_modulesInFlight.find(blobRegistryKey) != g_modulesInFlight.end()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] coalesce in-flight %s",
                      blobRegistryKey.c_str());
        }
        g_httpDynamicWaiters[blobRegistryKey].emplace_back(isolate, resolver);
        return scope.Escape(resolver->GetPromise());
      }

      g_modulesInFlight.insert(blobRegistryKey);
      g_httpDynamicWaiters[blobRegistryKey].emplace_back(isolate, resolver);

      v8::TryCatch tc(isolate);
      v8::Local<v8::Object> globalObj = context->Global();

      v8::Local<v8::Value> urlCtorVal;
      if (!globalObj
               ->Get(context, ArgConverter::ConvertToV8String(isolate, "URL"))
               .ToLocal(&urlCtorVal) ||
          !urlCtorVal->IsFunction()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] URL constructor not found");
        }
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "URL constructor not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> urlCtor = urlCtorVal.As<v8::Object>();

      v8::Local<v8::Value> internalAccessorVal;
      if (!urlCtor
               ->Get(context, ArgConverter::ConvertToV8String(isolate,
                                                              "InternalAccessor"))
               .ToLocal(&internalAccessorVal) ||
          !internalAccessorVal->IsObject()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] URL.InternalAccessor not found");
        }
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "URL.InternalAccessor not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> internalAccessor =
          internalAccessorVal.As<v8::Object>();

      v8::Local<v8::Value> getDataVal;
      if (!internalAccessor
               ->Get(context,
                     ArgConverter::ConvertToV8String(isolate, "getData"))
               .ToLocal(&getDataVal) ||
          !getDataVal->IsFunction()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] URL.InternalAccessor.getData not found");
        }
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "URL.InternalAccessor.getData not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Function> getDataFn = getDataVal.As<v8::Function>();

      v8::Local<v8::Value> urlArg =
          ArgConverter::ConvertToV8String(isolate, normalizedSpec);
      v8::Local<v8::Value> blobDataVal;
      if (!getDataFn->Call(context, internalAccessor, 1, &urlArg)
               .ToLocal(&blobDataVal) ||
          blobDataVal->IsNullOrUndefined()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] blob not found in BLOB_STORE: %s",
                      normalizedSpec.c_str());
        }
        std::string msg = "Blob not found: " + normalizedSpec;
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
        return scope.Escape(resolver->GetPromise());
      }

      if (!blobDataVal->IsObject()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] blob data is not an object");
        }
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, "Invalid blob data")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> blobData = blobDataVal.As<v8::Object>();

      v8::Local<v8::Value> blobVal;
      if (!blobData
               ->Get(context, ArgConverter::ConvertToV8String(isolate, "blob"))
               .ToLocal(&blobVal) ||
          !blobVal->IsObject()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] blob property not found");
        }
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "Blob object not found")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> blobObj = blobVal.As<v8::Object>();

      v8::Local<v8::Value> textFnVal;
      if (!blobObj
               ->Get(context, ArgConverter::ConvertToV8String(isolate, "text"))
               .ToLocal(&textFnVal) ||
          !textFnVal->IsFunction()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] Blob.text() not available");
        }
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "Blob.text() not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Function> textFn = textFnVal.As<v8::Function>();

      // Keep the two failure modes distinct — a throw out of text() and a
      // non-thenable return — and carry the thrown value's text into the
      // rejection to preserve diagnostics.
      v8::Local<v8::Value> textResultVal;
      std::string textFailure;
      {
        v8::TryCatch textTc(isolate);
        if (!textFn->Call(context, blobObj, 0, nullptr)
                 .ToLocal(&textResultVal)) {
          textFailure = "Blob.text() threw";
          if (textTc.HasCaught()) {
            v8::String::Utf8Value thrown(isolate, textTc.Exception());
            if (*thrown) {
              textFailure += std::string(": ") + *thrown;
            }
          }
        }
      }

      v8::Local<v8::Promise> textPromise;
      if (textFailure.empty() &&
          !AdoptThenable(isolate, context, textResultVal).ToLocal(&textPromise)) {
        textFailure = "Blob.text() did not return a thenable";
      }
      if (!textFailure.empty()) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] %s", textFailure.c_str());
        }
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, textFailure)));
        return scope.Escape(resolver->GetPromise());
      }

      struct BlobImportData {
        v8::Global<v8::Context> ctx;
        std::string blobUrl;
        std::string registryKey;
      };
      auto* data = new BlobImportData{v8::Global<v8::Context>(isolate, context),
                                       normalizedSpec, blobRegistryKey};

      auto onFulfilled = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        v8::Isolate* iso = info.GetIsolate();
        v8::HandleScope hs(iso);
        if (!info.Data()->IsExternal()) return;
        auto* d = static_cast<BlobImportData*>(
            info.Data().As<v8::External>()->Value(
                v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Context> ctx = d->ctx.Get(iso);

        if (info.Length() < 1 || !info[0]->IsString()) {
          RejectHttpDynamicWaiters(iso, ctx, d->registryKey,
                                   v8::Exception::Error(
                                       ArgConverter::ConvertToV8String(
                                           iso, "Blob text is not a string")));
          delete d;
          return;
        }

        v8::String::Utf8Value codeUtf8(iso, info[0]);
        std::string code = *codeUtf8 ? *codeUtf8 : "";

        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][blob] compiling blob module, code length=%zu",
                      code.size());
        }

        v8::MaybeLocal<v8::Module> modMaybe =
            CompileModuleForResolveRegisterOnly(iso, ctx, code, d->blobUrl);
        v8::Local<v8::Module> mod;
        if (!modMaybe.ToLocal(&mod)) {
          RejectHttpDynamicWaiters(iso, ctx, d->registryKey,
                                   v8::Exception::Error(
                                       ArgConverter::ConvertToV8String(
                                           iso, "Failed to compile blob module")));
          delete d;
          return;
        }

        if (mod->GetStatus() == v8::Module::kUninstantiated &&
            !mod->InstantiateModule(ctx, &ResolveModuleCallback)
                 .FromMaybe(false)) {
          RemoveModuleFromRegistry(d->registryKey);
          RejectHttpDynamicWaiters(
              iso, ctx, d->registryKey,
              v8::Exception::Error(ArgConverter::ConvertToV8String(
                  iso, "Failed to instantiate blob module")));
          delete d;
          return;
        }

        if (IsModuleEvaluationInProgress(mod->GetStatus())) {
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE(
                "[dyn-import][blob] waiting on existing evaluation for %s status=%s",
                d->registryKey.c_str(), ModuleStatusToString(mod->GetStatus()));
          }
          delete d;
          return;
        }

        if (mod->GetStatus() != v8::Module::kEvaluated) {
          v8::Local<v8::Value> evalResult;
          if (!mod->Evaluate(ctx).ToLocal(&evalResult)) {
            RemoveModuleFromRegistry(d->registryKey);
            RejectHttpDynamicWaiters(
                iso, ctx, d->registryKey,
                v8::Exception::Error(ArgConverter::ConvertToV8String(
                    iso, "Failed to evaluate blob module")));
            delete d;
            return;
          }

          if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
            struct BlobEvalData {
              std::string registryKey;
              v8::Global<v8::Context> ctx;
              v8::Global<v8::Module> mod;
            };
            auto* evalData = new BlobEvalData{
                d->registryKey, v8::Global<v8::Context>(iso, ctx),
                v8::Global<v8::Module>(iso, mod)};

            auto onEvalFulfilled =
                [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                  v8::Isolate* iso = info.GetIsolate();
                  v8::HandleScope hs(iso);
                  if (!info.Data()->IsExternal()) return;
                  auto* d = static_cast<BlobEvalData*>(
                      info.Data().As<v8::External>()->Value(
                          v8::kExternalPointerTypeTagDefault));
                  v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                  v8::Local<v8::Module> mod = d->mod.Get(iso);
                  ResolveHttpDynamicWaiters(iso, ctx, d->registryKey, mod);
                  delete d;
                };

            auto onEvalRejected =
                [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                  v8::Isolate* iso = info.GetIsolate();
                  v8::HandleScope hs(iso);
                  if (!info.Data()->IsExternal()) return;
                  auto* d = static_cast<BlobEvalData*>(
                      info.Data().As<v8::External>()->Value(
                          v8::kExternalPointerTypeTagDefault));
                  v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                  v8::Local<v8::Value> reason =
                      info.Length() > 0
                          ? info[0]
                          : v8::Exception::Error(
                                ArgConverter::ConvertToV8String(
                                    iso, "Blob module evaluation failed"));
                  RemoveModuleFromRegistry(d->registryKey);
                  RejectHttpDynamicWaiters(iso, ctx, d->registryKey, reason);
                  delete d;
                };

            v8::Local<v8::Promise> evalPromise = evalResult.As<v8::Promise>();
            v8::Local<v8::Function> onEvalFulfilledFn =
                v8::Function::New(
                    ctx, onEvalFulfilled,
                    v8::External::New(iso, evalData,
                                      v8::kExternalPointerTypeTagDefault))
                    .ToLocalChecked();
            v8::Local<v8::Function> onEvalRejectedFn =
                v8::Function::New(
                    ctx, onEvalRejected,
                    v8::External::New(iso, evalData,
                                      v8::kExternalPointerTypeTagDefault))
                    .ToLocalChecked();
            evalPromise->Then(ctx, onEvalFulfilledFn, onEvalRejectedFn)
                .FromMaybe(v8::Local<v8::Promise>());
            delete d;
            return;
          }
        }

        ResolveHttpDynamicWaiters(iso, ctx, d->registryKey, mod);
        delete d;
      };

      auto onRejected = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        v8::Isolate* iso = info.GetIsolate();
        v8::HandleScope hs(iso);
        if (!info.Data()->IsExternal()) return;
        auto* d = static_cast<BlobImportData*>(
            info.Data().As<v8::External>()->Value(
                v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Context> ctx = d->ctx.Get(iso);
        v8::Local<v8::Value> reason =
            info.Length() > 0
                ? info[0]
                : v8::Exception::Error(ArgConverter::ConvertToV8String(
                      iso, "Blob text() failed"));
        RejectHttpDynamicWaiters(iso, ctx, d->registryKey, reason);
        delete d;
      };

      v8::Local<v8::Function> onFulfilledFn =
          v8::Function::New(
              context, onFulfilled,
              v8::External::New(isolate, data,
                                v8::kExternalPointerTypeTagDefault))
              .ToLocalChecked();
      v8::Local<v8::Function> onRejectedFn =
          v8::Function::New(
              context, onRejected,
              v8::External::New(isolate, data,
                                v8::kExternalPointerTypeTagDefault))
              .ToLocalChecked();

      textPromise->Then(context, onFulfilledFn, onRejectedFn)
          .FromMaybe(v8::Local<v8::Promise>());

      return scope.Escape(resolver->GetPromise());
    }

    // ── HTTP(S) fast path ──
    // Security: HttpFetchText gates remote module access centrally.
    if (!normalizedSpec.empty() &&
        (StartsWith(normalizedSpec, "http://") ||
         StartsWith(normalizedSpec, "https://"))) {
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[dyn-import][http-loader] trying URL %s",
                    normalizedSpec.c_str());
      }
      std::string key = CanonicalizeHttpUrlKey(normalizedSpec);

      // Volatile-pattern eviction: if the URL matches any configured volatile
      // pattern, evict the cached module so we always re-fetch. Policy is
      // supplied exclusively by JS via ns:module `configureLoader({
      // volatilePatterns })` — the runtime carries no framework or server URL
      // vocabulary of its own.
      bool isVolatile = IsVolatileUrl(normalizedSpec);
      if (isVolatile) {
        auto ex = g_moduleRegistry.find(key);
        if (ex != g_moduleRegistry.end()) {
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[dyn-import][http-cache] drop volatile %s", key.c_str());
          }
          RemoveModuleFromRegistry(key);
        }
      }
      // Coalesce concurrent dynamic imports for the same HTTP key.
      auto inflight = g_modulesInFlight.find(key) != g_modulesInFlight.end();
      if (inflight) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import][http] coalesce in-flight %s", key.c_str());
        }
        g_httpDynamicWaiters[key].emplace_back(isolate, resolver);
        return scope.Escape(resolver->GetPromise());
      }
      // If module was already compiled, resolve immediately.
      auto itExisting = g_moduleRegistry.find(key);
      if (itExisting != g_moduleRegistry.end()) {
        v8::Local<v8::Module> existing = itExisting->second.Get(isolate);
        if (!existing.IsEmpty()) {
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[dyn-import][http-cache] hit %s status=%s", key.c_str(),
                        ModuleStatusToString(existing->GetStatus()));
          }
          v8::Module::Status st = existing->GetStatus();
          if (st == v8::Module::kErrored) {
            if (IsScriptLoadingLogEnabled()) {
              DEBUG_WRITE("[dyn-import][http-cache] dropping errored module for %s",
                          key.c_str());
            }
            RemoveModuleFromRegistry(key);
          } else if (IsModuleEvaluationInProgress(st)) {
            if (QueueHttpDynamicWaiterIfInFlight(isolate, key, existing,
                                                 resolver)) {
              return scope.Escape(resolver->GetPromise());
            }
            if (IsScriptLoadingLogEnabled()) {
              DEBUG_WRITE(
                  "[dyn-import][http-cache] avoiding re-entrant Evaluate for %s status=%s",
                  key.c_str(), ModuleStatusToString(st));
            }
            resolver->Resolve(context, existing->GetModuleNamespace())
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          } else {
            if (st != v8::Module::kEvaluated) {
              g_modulesInFlight.insert(key);
              if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[dyn-import][http-cache] awaiting evaluation %s",
                            key.c_str());
              }
              g_httpDynamicWaiters[key].emplace_back(isolate, resolver);
              if (st == v8::Module::kUninstantiated) {
                v8::TryCatch tcInstantiate(isolate);
                if (!existing->InstantiateModule(context, &ResolveModuleCallback)
                         .FromMaybe(false)) {
                  RemoveModuleFromRegistry(key);
                  RejectHttpDynamicWaiters(
                      isolate, context, key,
                      BuildModuleFailureReason(
                          isolate, tcInstantiate,
                          "Instantiation failed (http-cache hit)", key));
                  return scope.Escape(resolver->GetPromise());
                }
              }

              if (IsModuleEvaluationInProgress(existing->GetStatus())) {
                return scope.Escape(resolver->GetPromise());
              }

              v8::Local<v8::Value> evalResult;
              {
                v8::TryCatch tcEvaluate(isolate);
                if (!existing->Evaluate(context).ToLocal(&evalResult)) {
                  RemoveModuleFromRegistry(key);
                  RejectHttpDynamicWaiters(
                      isolate, context, key,
                      BuildModuleFailureReason(
                          isolate, tcEvaluate,
                          "Evaluation failed (http-cache hit)", key));
                  return scope.Escape(resolver->GetPromise());
                }
              }
              if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
                v8::Local<v8::Promise> p = evalResult.As<v8::Promise>();
                struct EvalWaitData {
                  std::string key;
                  v8::Global<v8::Context> ctx;
                  v8::Global<v8::Module> mod;
                };
                auto* data = new EvalWaitData{
                    key, v8::Global<v8::Context>(isolate, context),
                    v8::Global<v8::Module>(isolate, existing)};
                auto onFulfilled =
                    [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                      v8::Isolate* iso = info.GetIsolate();
                      v8::HandleScope hs(iso);
                      if (!info.Data()->IsExternal()) return;
                      auto* d = static_cast<EvalWaitData*>(
                          info.Data().As<v8::External>()->Value(
                              v8::kExternalPointerTypeTagDefault));
                      v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                      std::string keyLocal = d->key;
                      v8::Local<v8::Module> modLocal = d->mod.Get(iso);
                      ResolveHttpDynamicWaiters(iso, ctx, keyLocal, modLocal);
                      delete d;
                    };
                auto onRejected =
                    [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                      v8::Isolate* iso = info.GetIsolate();
                      v8::HandleScope hs(iso);
                      if (!info.Data()->IsExternal()) return;
                      auto* d = static_cast<EvalWaitData*>(
                          info.Data().As<v8::External>()->Value(
                              v8::kExternalPointerTypeTagDefault));
                      v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                      std::string keyLocal = d->key;
                      v8::Local<v8::Value> reason =
                          (info.Length() > 0)
                              ? info[0]
                              : v8::Exception::Error(
                                    ArgConverter::ConvertToV8String(
                                        iso, "Evaluation failed (http-cache TLA)"));
                      if (IsScriptLoadingLogEnabled()) {
                        v8::String::Utf8Value r(iso, reason);
                        if (*r) {
                          DEBUG_WRITE("[dyn-import][http-cache][tla] rejected: %s",
                                      *r);
                        }
                      }
                      RejectHttpDynamicWaiters(iso, ctx, keyLocal, reason);
                      delete d;
                    };
                v8::Local<v8::FunctionTemplate> thenFulfillTpl =
                    v8::FunctionTemplate::New(
                        isolate, onFulfilled,
                        v8::External::New(isolate, data,
                                          v8::kExternalPointerTypeTagDefault));
                v8::Local<v8::Function> thenFulfill =
                    thenFulfillTpl->GetFunction(context).ToLocalChecked();
                v8::Local<v8::FunctionTemplate> thenRejectTpl =
                    v8::FunctionTemplate::New(
                        isolate, onRejected,
                        v8::External::New(isolate, data,
                                          v8::kExternalPointerTypeTagDefault));
                v8::Local<v8::Function> thenReject =
                    thenRejectTpl->GetFunction(context).ToLocalChecked();
                p->Then(context, thenFulfill, thenReject).ToLocalChecked();
                return scope.Escape(resolver->GetPromise());
              }
              ResolveHttpDynamicWaiters(isolate, context, key, existing);
              return scope.Escape(resolver->GetPromise());
            }
            resolver->Resolve(context, existing->GetModuleNamespace())
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          }
        }
      }
      // Mark in-flight and start the async graph load.
      g_modulesInFlight.insert(key);
      g_httpDynamicWaiters[key].emplace_back(isolate, resolver);
      const std::string requestUrl = normalizedSpec;
      StartAsyncHttpModuleGraphLoad(
          isolate, context, requestUrl,
          [key, requestUrl, isolate](bool ok, const std::string& errorMessage,
                                      v8::Local<v8::Context> completionContext) {
            v8::Isolate* iso = isolate;
            if (!ok) {
              RejectHttpDynamicWaiters(
                  iso, completionContext, key,
                  v8::Exception::Error(
                      ArgConverter::ConvertToV8String(iso, errorMessage)));
              return;
            }
            FinishHttpDynamicImport(iso, completionContext, key, requestUrl);
          });
      return scope.Escape(resolver->GetPromise());
    }

    // ── Filesystem path ──
    // For relative specs, adjust against the referrer's resource URL so
    // ../-segments collapse and the resolver can find the target on disk.
    v8::Local<v8::Module> refMod;
    v8::Local<v8::String> adjustedSpecifier = specifier;
    if (!normalizedSpec.empty() &&
        (normalizedSpec.rfind("./", 0) == 0 ||
         normalizedSpec.rfind("../", 0) == 0)) {
      v8::Local<v8::Value> resName = resource_name;
      if (!resName.IsEmpty() && resName->IsString()) {
        v8::String::Utf8Value rn(isolate, resName);
        std::string refUrl = *rn ? *rn : std::string();
        if (!refUrl.empty()) {
          std::string refPath = FileURLToPath(refUrl);
          size_t slash = refPath.find_last_of("/\\");
          std::string baseDir = slash == std::string::npos
                                    ? std::string()
                                    : refPath.substr(0, slash + 1);
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[dyn-import][ref] url=%s base=%s spec=%s", refUrl.c_str(),
                        baseDir.c_str(), normalizedSpec.c_str());
          }
          std::string fsPath = NormalizePath(baseDir + normalizedSpec);
          if (!fsPath.empty()) {
            adjustedSpecifier =
                ArgConverter::ConvertToV8String(isolate, fsPath);
            if (IsScriptLoadingLogEnabled()) {
              DEBUG_WRITE("[dyn-import][normalize-rel] %s + %s -> %s",
                          baseDir.c_str(), normalizedSpec.c_str(),
                          fsPath.c_str());
            }
          }
        }
      } else if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE(
            "[dyn-import][ref] missing resource name; cannot normalize relative "
            "spec against referrer");
      }
    }

    v8::TryCatch resolveTc(isolate);
    v8::MaybeLocal<v8::Module> maybeModule = ResolveModuleCallback(
        context, adjustedSpecifier, import_assertions, refMod);
    if (IsScriptLoadingLogEnabled()) {
      v8::String::Utf8Value adj(isolate, adjustedSpecifier);
      const char* cAdj = (*adj) ? *adj : "<invalid>";
      DEBUG_WRITE("[dyn-import][resolver-call] raw=%s normalized=%s adjusted=%s",
                  rawSpec.c_str(), normalizedSpec.c_str(), cAdj);
    }
    v8::String::Utf8Value adjustedSpecUtf8(isolate, adjustedSpecifier);
    std::string adjustedRegistryKey =
        *adjustedSpecUtf8 ? CanonicalizeRegistryKey(*adjustedSpecUtf8)
                          : std::string();
    if (maybeModule.IsEmpty()) {
      if (resolveTc.HasCaught()) {
        resolver->Reject(context, resolveTc.Exception()).FromMaybe(false);
        return scope.Escape(resolver->GetPromise());
      } else {
        std::string msg = "Module resolution failed for dynamic import: ";
        msg += normalizedSpec.empty() ? "<empty>" : normalizedSpec;
        resolver
            ->Reject(context, v8::Exception::Error(
                                  ArgConverter::ConvertToV8String(isolate, msg)))
            .FromMaybe(false);
        return scope.Escape(resolver->GetPromise());
      }
    }

    v8::Local<v8::Module> module = maybeModule.ToLocalChecked();

    if (module->GetStatus() == v8::Module::kUninstantiated) {
      v8::TryCatch ictc(isolate);
      if (!module->InstantiateModule(context, &ResolveModuleCallback)
               .FromMaybe(false)) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import] instantiate failed %s",
                      normalizedSpec.c_str());
        }
        std::string msg =
            std::string("Failed to instantiate module: ") + normalizedSpec;
        if (ictc.HasCaught()) {
          std::string exStr = ArgConverter::ToString(isolate, ictc.Exception());
          if (!exStr.empty()) {
            msg.append(" - ");
            msg.append(exStr);
          }
        }
        resolver
            ->Reject(context, v8::Exception::Error(
                                  ArgConverter::ConvertToV8String(isolate, msg)))
            .Check();
        return scope.Escape(resolver->GetPromise());
      }
    }

    if (IsModuleEvaluationInProgress(module->GetStatus())) {
      if (QueueModuleWaiterIfInFlight(isolate, adjustedRegistryKey, module,
                                      resolver)) {
        return scope.Escape(resolver->GetPromise());
      }
      if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE(
            "[dyn-import] avoiding re-entrant Evaluate for %s status=%s",
            adjustedRegistryKey.empty() ? rawSpec.c_str()
                                        : adjustedRegistryKey.c_str(),
            ModuleStatusToString(module->GetStatus()));
      }
      resolver->Resolve(context, module->GetModuleNamespace()).Check();
      return scope.Escape(resolver->GetPromise());
    }

    if (module->GetStatus() != v8::Module::kEvaluated) {
      v8::Local<v8::Value> evalResult;
      if (!module->Evaluate(context).ToLocal(&evalResult)) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE("[dyn-import] evaluation failed %s",
                      normalizedSpec.c_str());
        }
        std::string msg =
            std::string("Evaluation failed for module: ") + normalizedSpec;
        v8::Local<v8::Value> ex = v8::Exception::Error(
            ArgConverter::ConvertToV8String(isolate, msg));
        resolver->Reject(context, ex).Check();
        return scope.Escape(resolver->GetPromise());
      }
      if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
        v8::Local<v8::Promise> p = evalResult.As<v8::Promise>();
        struct DynEvalData {
          v8::Global<v8::Context> ctx;
          v8::Global<v8::Module> mod;
          v8::Global<v8::Promise::Resolver> res;
        };
        auto* d = new DynEvalData{
            v8::Global<v8::Context>(isolate, context),
            v8::Global<v8::Module>(isolate, module),
            v8::Global<v8::Promise::Resolver>(isolate, resolver)};
        auto onFulfilled = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
          v8::Isolate* iso = info.GetIsolate();
          v8::HandleScope hs(iso);
          if (!info.Data()->IsExternal()) return;
          auto* d = static_cast<DynEvalData*>(
              info.Data().As<v8::External>()->Value(
                  v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Context> ctx = d->ctx.Get(iso);
          v8::Local<v8::Module> modLocal = d->mod.Get(iso);
          v8::Local<v8::Promise::Resolver> res = d->res.Get(iso);
          if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[dyn-import][tla] fulfilled, resolving namespace");
          }
          if (!res.IsEmpty())
            res->Resolve(ctx, modLocal->GetModuleNamespace()).FromMaybe(false);
          delete d;
        };
        auto onRejected = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
          v8::Isolate* iso = info.GetIsolate();
          v8::HandleScope hs(iso);
          if (!info.Data()->IsExternal()) return;
          auto* d = static_cast<DynEvalData*>(
              info.Data().As<v8::External>()->Value(
                  v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Context> ctx = d->ctx.Get(iso);
          v8::Local<v8::Promise::Resolver> res = d->res.Get(iso);
          v8::Local<v8::Value> reason =
              (info.Length() > 0)
                  ? info[0]
                  : v8::Exception::Error(ArgConverter::ConvertToV8String(
                        iso, "Evaluation failed (TLA)"));
          if (IsScriptLoadingLogEnabled()) {
            v8::String::Utf8Value r(iso, reason);
            if (*r) {
              DEBUG_WRITE("[dyn-import][tla] rejected: %s", *r);
            }
          }
          if (!res.IsEmpty()) res->Reject(ctx, reason).FromMaybe(false);
          delete d;
        };
        v8::Local<v8::FunctionTemplate> fulfillTpl = v8::FunctionTemplate::New(
            isolate, onFulfilled,
            v8::External::New(isolate, d, v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Function> fulfill =
            fulfillTpl->GetFunction(context).ToLocalChecked();
        v8::Local<v8::FunctionTemplate> rejectTpl = v8::FunctionTemplate::New(
            isolate, onRejected,
            v8::External::New(isolate, d, v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Function> reject =
            rejectTpl->GetFunction(context).ToLocalChecked();
        p->Then(context, fulfill, reject).ToLocalChecked();
        return scope.Escape(resolver->GetPromise());
      }
    }

    // Final verify before resolving for non-HTTP paths.
    v8::Local<v8::Value> nsFinal = module->GetModuleNamespace();
    if (nsFinal->IsObject()) {
      v8::Local<v8::Object> o = nsFinal.As<v8::Object>();
      v8::TryCatch tc3(isolate);
      v8::Local<v8::Value> defVal;
      if (!o->Get(context, ArgConverter::ConvertToV8String(isolate, "default"))
               .ToLocal(&defVal)) {
        if (IsScriptLoadingLogEnabled()) {
          DEBUG_WRITE(
              "[dyn-import][verify] ns.default threw after eval (generic) %s",
              normalizedSpec.c_str());
        }
        resolver
            ->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(
                                  isolate, "TDZ on default after eval (generic)")))
            .Check();
        return scope.Escape(resolver->GetPromise());
      }
    }
    resolver->Resolve(context, module->GetModuleNamespace()).Check();
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[dyn-import] resolved %s", normalizedSpec.c_str());
    }
  } catch (NativeScriptException& ex) {
    ex.ReThrowToV8();
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[dyn-import] native failed %s", normalizedSpec.c_str());
    }
    resolver
        ->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(
                              isolate, "Native error during dynamic import")))
        .Check();
  }

  return scope.Escape(resolver->GetPromise());
}

// ─────────────────────────────────────────────────────────────
// InitializeImportMetaObject — populates `import.meta.url` and
// `import.meta.dirname`. `import.meta.hot` is JS policy and is deliberately
// NOT set here (matches the port spec).
void InitializeImportMetaObject(v8::Local<v8::Context> context,
                                v8::Local<v8::Module> module,
                                v8::Local<v8::Object> meta) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto& g_moduleRegistry = ModuleRegistryFor(isolate);

  std::string modulePath;
  for (auto& kv : g_moduleRegistry) {
    v8::Local<v8::Module> registered = kv.second.Get(isolate);
    if (!registered.IsEmpty() && registered == module) {
      modulePath = kv.first;
      break;
    }
  }
  if (modulePath.empty()) return;

  std::string moduleUrl;
  std::string moduleDirname;
  if (StartsWith(modulePath, "http://") || StartsWith(modulePath, "https://")) {
    moduleUrl = modulePath;
    size_t slash = modulePath.find_last_of('/');
    moduleDirname = slash == std::string::npos ? modulePath
                                               : modulePath.substr(0, slash);
  } else if (StartsWith(modulePath, "blob:")) {
    moduleUrl = modulePath;
    moduleDirname = modulePath;
  } else {
    moduleUrl = StartsWith(modulePath, "file://") ? modulePath
                                                  : ("file://" + modulePath);
    std::string filesystemPath = FileURLToPath(moduleUrl);
    size_t slash = filesystemPath.find_last_of("/\\");
    moduleDirname = slash == std::string::npos ? filesystemPath
                                                : filesystemPath.substr(0, slash);
  }

  meta->CreateDataProperty(
          context, ArgConverter::ConvertToV8String(isolate, "url"),
          ArgConverter::ConvertToV8String(isolate, moduleUrl))
      .FromMaybe(false);
  meta->CreateDataProperty(
          context, ArgConverter::ConvertToV8String(isolate, "dirname"),
          ArgConverter::ConvertToV8String(isolate, moduleDirname))
      .FromMaybe(false);
}

}  // namespace tns

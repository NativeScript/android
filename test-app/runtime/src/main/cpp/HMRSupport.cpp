// HMRSupport.cpp
#include "HMRSupport.h"

#include "ArgConverter.h"
#include "DevFlags.h"
#include "JEnv.h"
#include "ModuleInternalCallbacks.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "Runtime.h"

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
// Resolver-side helpers used by the dev-session machinery below
// (`ApplyDevRuntimeConfigObject`, `CollectSessionModuleUrls`). The actual
// definitions live in ModuleInternalCallbacks.cpp; this header-style
// forward block lets HMRSupport.cpp call them without pulling the
// resolver header in (avoids a circular include).
void SetImportMap(const std::string& json);
void SetVolatilePatterns(const std::vector<std::string>& patterns);
std::vector<std::string> GetLoadedModuleUrls();

// ──────────────────────────────────────────────────────────────────────────
// Local v8 string helper: thin convenience wrapper around the existing
// `ArgConverter::ConvertToV8String` so call sites can read more compactly.
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

// Per-module hot data and callbacks. Keyed by canonical module path.
// Heap-allocated (leaky singleton) to prevent V8 crash during __cxa_finalize_ranges.
// See g_moduleRegistry comment in ModuleInternalCallbacks.cpp for full rationale.
static auto* _g_hotData = new std::unordered_map<std::string, v8::Global<v8::Object>>();
static auto& g_hotData = *_g_hotData;
static auto* _g_hotAccept = new std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>();
static auto& g_hotAccept = *_g_hotAccept;
static auto* _g_hotDispose = new std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>();
static auto& g_hotDispose = *_g_hotDispose;
// Per-module prune callbacks (`import.meta.hot.prune(cb)`). Symmetric with
// `g_hotDispose` — separate registry because Vite spec semantics differ:
// `dispose` fires on every replacement (every HMR cycle), `prune` fires
// only when the module is removed from the dependency graph entirely.
static auto* _g_hotPrune = new std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>();
static auto& g_hotPrune = *_g_hotPrune;

// Custom event listeners
// Keyed by event name (global, not per-module)
static auto* _g_hotEventListeners = new std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>();
static auto& g_hotEventListeners = *_g_hotEventListeners;

// Set of canonical module keys that called `import.meta.hot.decline()`.
// The HMR client checks this set before applying an update — if any update
// touches a declined key, the update converts to a full reload. No V8
// handles to clean up (just strings), so this lives in a plain set with
// its own mutex for thread safety.
static std::unordered_set<std::string> g_hotDeclined;
static std::mutex g_hotDeclinedMutex;

// Active deterministic dev-session state.
static DevSessionState g_activeDevSession;
static std::mutex g_activeDevSessionMutex;

bool GetOptionalStringProperty(v8::Isolate* isolate, v8::Local<v8::Context> context,
                               v8::Local<v8::Object> object, const char* key,
                               std::string* out) {
  if (out == nullptr) return false;

  v8::Local<v8::Value> value;
  if (!object->Get(context, ToV8String(isolate, key)).ToLocal(&value) ||
      value->IsUndefined() || value->IsNull()) {
    return false;
  }

  v8::Local<v8::String> stringValue;
  if (!value->ToString(context).ToLocal(&stringValue)) {
    return false;
  }

  v8::String::Utf8Value utf8(isolate, stringValue);
  *out = *utf8 ? *utf8 : "";
  return true;
}

v8::Local<v8::Promise> CreateResolvedPromise(v8::Isolate* isolate,
                                             v8::Local<v8::Context> context) {
  v8::Local<v8::Promise::Resolver> resolver =
      v8::Promise::Resolver::New(context).ToLocalChecked();
  resolver->Resolve(context, v8::Undefined(isolate)).FromMaybe(false);
  return resolver->GetPromise();
}

v8::Local<v8::Promise> CreateRejectedPromise(v8::Local<v8::Context> context,
                                             v8::Local<v8::Value> reason) {
  v8::Local<v8::Promise::Resolver> resolver =
      v8::Promise::Resolver::New(context).ToLocalChecked();
  resolver->Reject(context, reason).FromMaybe(false);
  return resolver->GetPromise();
}

void MirrorFunctionOnGlobalThis(v8::Isolate* isolate, v8::Local<v8::Context> context,
                                const char* name) {
  std::string src =
      "if (typeof globalThis !== 'undefined' && typeof globalThis." +
      std::string(name) +
      " !== 'function') {"
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

static bool GetOptionalBooleanProperty(v8::Isolate* isolate, v8::Local<v8::Context> context,
                                       v8::Local<v8::Object> object, const char* key,
                                       bool* out) {
  if (out == nullptr) return false;

  v8::Local<v8::Value> value;
  if (!object->Get(context, ToV8String(isolate, key)).ToLocal(&value) ||
      value->IsUndefined() || value->IsNull()) {
    return false;
  }

  *out = value->BooleanValue(isolate);
  return true;
}

static void SetBooleanGlobal(v8::Isolate* isolate, v8::Local<v8::Context> context,
                             const char* key, bool value) {
  context->Global()
      ->Set(context, ToV8String(isolate, key), v8::Boolean::New(isolate, value))
      .FromMaybe(false);
}

static void SetStringGlobal(v8::Isolate* isolate, v8::Local<v8::Context> context,
                            const char* key, const std::string& value) {
  context->Global()
      ->Set(context, ToV8String(isolate, key),
            ToV8String(isolate, value.c_str()))
      .FromMaybe(false);
}

static bool IsSupportedDevSessionPlatform(const std::string& platform) {
  // Dev sessions only support the "android" platform identifier.
  return platform == "android";
}

// Apply the v8::Object payload of `__nsConfigureDevRuntime`: re-validate the
// `importMap` shape and serialize it back to JSON for `SetImportMap`. Parsing
// runs entirely in V8 (via `ConfigureDevRuntimeCallback`), so this is a thin
// wrapper over that shared validation.
static bool ApplyDevRuntimeConfigObject(v8::Isolate* isolate,
                                        v8::Local<v8::Context> context,
                                        v8::Local<v8::Object> payload,
                                        std::string* errorMessage) {
  if (payload.IsEmpty()) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] runtime config payload must be an object";
    }
    return false;
  }

  v8::Local<v8::Value> importMapValue;
  if (!payload->Get(context, ToV8String(isolate, "importMap")).ToLocal(&importMapValue) ||
      !importMapValue->IsObject()) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] runtime config payload is missing importMap";
    }
    return false;
  }

  // Use JSON.stringify on the importMap object — keeps the on-disk format
  // identical to what `__nsConfigureRuntime` already accepts.
  v8::Local<v8::Object> jsonObj;
  v8::Local<v8::Value> globalJson;
  if (!context->Global()->Get(context, ToV8String(isolate, "JSON")).ToLocal(&globalJson) ||
      !globalJson->IsObject()) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] JSON global unavailable";
    }
    return false;
  }
  jsonObj = globalJson.As<v8::Object>();

  v8::Local<v8::Value> stringifyFnVal;
  if (!jsonObj->Get(context, ToV8String(isolate, "stringify")).ToLocal(&stringifyFnVal) ||
      !stringifyFnVal->IsFunction()) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] JSON.stringify unavailable";
    }
    return false;
  }

  v8::Local<v8::Function> stringifyFn = stringifyFnVal.As<v8::Function>();
  v8::Local<v8::Value> args[] = {importMapValue};
  v8::MaybeLocal<v8::Value> maybeJson = stringifyFn->Call(context, jsonObj, 1, args);
  v8::Local<v8::Value> jsonVal;
  if (!maybeJson.ToLocal(&jsonVal) || !jsonVal->IsString()) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] failed to serialize importMap";
    }
    return false;
  }

  v8::String::Utf8Value jsonUtf8(isolate, jsonVal);
  std::string importMapJson = *jsonUtf8 ? *jsonUtf8 : "";
  if (importMapJson.empty()) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] runtime config importMap was empty";
    }
    return false;
  }

  SetImportMap(importMapJson);

  std::vector<std::string> patterns;
  v8::Local<v8::Value> volatilePatternsValue;
  if (payload->Get(context, ToV8String(isolate, "volatilePatterns")).ToLocal(&volatilePatternsValue) &&
      volatilePatternsValue->IsArray()) {
    v8::Local<v8::Array> arr = volatilePatternsValue.As<v8::Array>();
    uint32_t length = arr->Length();
    for (uint32_t i = 0; i < length; ++i) {
      v8::Local<v8::Value> entry;
      if (!arr->Get(context, i).ToLocal(&entry)) continue;
      if (!entry->IsString()) continue;
      v8::String::Utf8Value utf8(isolate, entry);
      if (*utf8 && (*utf8)[0] != '\0') {
        patterns.emplace_back(*utf8);
      }
    }
  }

  if (!patterns.empty()) {
    SetVolatilePatterns(patterns);
  }

  return true;
}

v8::Local<v8::Object> GetOrCreateHotData(v8::Isolate* isolate, const std::string& key) {
  auto it = g_hotData.find(key);
  if (it != g_hotData.end()) {
    if (!it->second.IsEmpty()) {
      return it->second.Get(isolate);
    }
  }
  v8::Local<v8::Object> obj = v8::Object::New(isolate);
  g_hotData[key].Reset(isolate, obj);
  return obj;
}

bool ReadDevSessionConfig(v8::Isolate* isolate, v8::Local<v8::Context> context,
                          v8::Local<v8::Object> config, DevSessionState* out,
                          std::string* errorMessage) {
  if (out == nullptr) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] output session state is required";
    }
    return false;
  }

  DevSessionState next;
  next.active = true;
  GetOptionalStringProperty(isolate, context, config, "sessionId", &next.sessionId);
  GetOptionalStringProperty(isolate, context, config, "origin", &next.origin);
  GetOptionalStringProperty(isolate, context, config, "entryUrl", &next.entryUrl);
  GetOptionalStringProperty(isolate, context, config, "clientUrl", &next.clientUrl);
  GetOptionalStringProperty(isolate, context, config, "wsUrl", &next.wsUrl);
  GetOptionalStringProperty(isolate, context, config, "platform", &next.platform);
  GetOptionalStringProperty(isolate, context, config, "runtimeConfigUrl", &next.runtimeConfigUrl);

  v8::Local<v8::Value> featuresValue;
  if (config->Get(context, ToV8String(isolate, "features"))
          .ToLocal(&featuresValue) &&
      featuresValue->IsObject()) {
    v8::Local<v8::Object> features = featuresValue.As<v8::Object>();
    GetOptionalBooleanProperty(isolate, context, features, "fullReload",
                               &next.fullReload);
    GetOptionalBooleanProperty(isolate, context, features, "cssHmr",
                               &next.cssHmr);
  }

  if (next.sessionId.empty() || next.origin.empty() || next.entryUrl.empty() ||
      next.clientUrl.empty() || next.wsUrl.empty() || next.platform.empty()) {
    if (errorMessage != nullptr) {
      *errorMessage =
          "[__nsStartDevSession] sessionId, origin, clientUrl, wsUrl, entryUrl, and platform are required";
    }
    return false;
  }

  if (!IsSupportedDevSessionPlatform(next.platform)) {
    if (errorMessage != nullptr) {
      *errorMessage =
          "[__nsStartDevSession] platform must be android";
    }
    return false;
  }

  *out = next;
  return true;
}

void ResetActiveDevSession() {
  std::lock_guard<std::mutex> lock(g_activeDevSessionMutex);
  if (IsScriptLoadingLogEnabled() && g_activeDevSession.active) {
    DEBUG_WRITE("[dev-session] reset active session=%s started=%s",
                g_activeDevSession.sessionId.c_str(),
                g_activeDevSession.started ? "true" : "false");
  }
  g_activeDevSession = DevSessionState();
}

DevSessionState GetActiveDevSessionSnapshot() {
  std::lock_guard<std::mutex> lock(g_activeDevSessionMutex);
  return g_activeDevSession;
}

void StoreActiveDevSession(const DevSessionState& session) {
  std::lock_guard<std::mutex> lock(g_activeDevSessionMutex);
  g_activeDevSession = session;
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dev-session] stored session=%s started=%s origin=%s client=%s entry=%s",
                session.sessionId.c_str(), session.started ? "true" : "false",
                session.origin.c_str(), session.clientUrl.c_str(),
                session.entryUrl.c_str());
  }
}

bool HasDevSessionChanged(const DevSessionState& previous,
                          const DevSessionState& next) {
  return !previous.active || previous.sessionId != next.sessionId ||
         previous.origin != next.origin || previous.entryUrl != next.entryUrl ||
    previous.clientUrl != next.clientUrl || previous.wsUrl != next.wsUrl ||
    previous.runtimeConfigUrl != next.runtimeConfigUrl;
}

std::vector<std::string> CollectSessionModuleUrls(const DevSessionState& session) {
  std::vector<std::string> invalidate;
  if (!session.active || session.origin.empty()) {
    return invalidate;
  }

  for (const auto& url : tns::GetLoadedModuleUrls()) {
    if (!StartsWith(url, session.origin.c_str())) continue;
    if (!session.clientUrl.empty() && url == session.clientUrl) continue;
    invalidate.push_back(url);
  }

  return invalidate;
}

bool ApplyDevRuntimeConfigFromUrl(const std::string& url,
                                  std::string* errorMessage) {
  if (url.empty()) {
    return true;
  }

  std::string body;
  std::string contentType;
  int status = 0;
  if (!HttpFetchText(url, body, contentType, status) || body.empty()) {
    if (errorMessage != nullptr) {
      *errorMessage = std::string("[__nsStartDevSession] failed to fetch runtimeConfigUrl: ") + url;
    }
    return false;
  }

  // Parse the JSON response in V8: dev-session bootstrap runs on the JS thread,
  // so a live isolate is available.
  v8::Isolate* isolate = v8::Isolate::TryGetCurrent();
  if (isolate == nullptr) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] no current V8 isolate to parse runtime config";
    }
    return false;
  }

  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  if (context.IsEmpty()) {
    if (errorMessage != nullptr) {
      *errorMessage = "[__nsStartDevSession] no current V8 context to parse runtime config";
    }
    return false;
  }

  v8::TryCatch tc(isolate);
  v8::Local<v8::String> bodyStr = v8::String::NewFromUtf8(
      isolate, body.c_str(), v8::NewStringType::kNormal,
      static_cast<int>(body.size())).ToLocalChecked();
  v8::MaybeLocal<v8::Value> maybeParsed = v8::JSON::Parse(context, bodyStr);
  v8::Local<v8::Value> parsed;
  if (!maybeParsed.ToLocal(&parsed) || !parsed->IsObject()) {
    if (errorMessage != nullptr) {
      std::string detail = "unknown runtime config parse error";
      if (tc.HasCaught()) {
        v8::String::Utf8Value msg(isolate, tc.Exception());
        if (*msg) detail = *msg;
      }
      *errorMessage = std::string("[__nsStartDevSession] failed to parse runtime config: ") + detail;
    }
    return false;
  }

  if (!ApplyDevRuntimeConfigObject(isolate, context, parsed.As<v8::Object>(), errorMessage)) {
    return false;
  }

  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dev-session] runtime config applied url=%s", url.c_str());
  }

  return true;
}

// Native-side mirror of `__NS_HMR_BOOT_COMPLETE__`. Read by the
// runloop pump in `MaybePumpJSThreadDuringBoot` so its gate is a
// single relaxed atomic load on the HMR-time hot path.
static std::atomic<bool> g_devSessionBootComplete{false};

static inline bool IsDevSessionBootComplete() {
  return g_devSessionBootComplete.load(std::memory_order_relaxed);
}

void ApplyDevSessionGlobals(v8::Isolate* isolate,
                            v8::Local<v8::Context> context,
                            const DevSessionState& session) {
  SetStringGlobal(isolate, context, "__NS_HTTP_ORIGIN__", session.origin);
  SetStringGlobal(isolate, context, "__NS_HMR_WS_URL__", session.wsUrl);
  SetBooleanGlobal(isolate, context, "__NS_HMR_BOOT_COMPLETE__", false);
  SetBooleanGlobal(isolate, context, "__NS_HMR_CLIENT_ACTIVE__", false);
  SetBooleanGlobal(isolate, context, "__NS_HMR_BROWSER_RUNTIME_CLIENT_ACTIVE__", false);
  g_devSessionBootComplete.store(false, std::memory_order_relaxed);
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dev-session] globals applied session=%s origin=%s ws=%s bootComplete=false",
                session.sessionId.c_str(), session.origin.c_str(),
                session.wsUrl.c_str());
  }
}

void SetDevSessionBootComplete(v8::Isolate* isolate,
                               v8::Local<v8::Context> context,
                               bool value) {
  SetBooleanGlobal(isolate, context, "__NS_HMR_BOOT_COMPLETE__", value);
  g_devSessionBootComplete.store(value, std::memory_order_relaxed);
  if (IsScriptLoadingLogEnabled()) {
    DEBUG_WRITE("[dev-session] __NS_HMR_BOOT_COMPLETE__=%s",
                value ? "true" : "false");
  }
}

void RegisterHotAccept(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb) {
  if (cb.IsEmpty()) return;
  g_hotAccept[key].emplace_back(v8::Global<v8::Function>(isolate, cb));
}

void RegisterHotDispose(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb) {
  if (cb.IsEmpty()) return;
  g_hotDispose[key].emplace_back(v8::Global<v8::Function>(isolate, cb));
}

void RegisterHotPrune(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb) {
  if (cb.IsEmpty()) return;
  g_hotPrune[key].emplace_back(v8::Global<v8::Function>(isolate, cb));
}

std::vector<v8::Local<v8::Function>> GetHotAcceptCallbacks(v8::Isolate* isolate, const std::string& key) {
  std::vector<v8::Local<v8::Function>> out;
  auto it = g_hotAccept.find(key);
  if (it != g_hotAccept.end()) {
    for (auto& gfn : it->second) {
      if (!gfn.IsEmpty()) out.push_back(gfn.Get(isolate));
    }
  }
  return out;
}

std::vector<v8::Local<v8::Function>> GetHotDisposeCallbacks(v8::Isolate* isolate, const std::string& key) {
  std::vector<v8::Local<v8::Function>> out;
  auto it = g_hotDispose.find(key);
  if (it != g_hotDispose.end()) {
    for (auto& gfn : it->second) {
      if (!gfn.IsEmpty()) out.push_back(gfn.Get(isolate));
    }
  }
  return out;
}

std::vector<v8::Local<v8::Function>> GetHotPruneCallbacks(v8::Isolate* isolate, const std::string& key) {
  std::vector<v8::Local<v8::Function>> out;
  auto it = g_hotPrune.find(key);
  if (it != g_hotPrune.end()) {
    for (auto& gfn : it->second) {
      if (!gfn.IsEmpty()) out.push_back(gfn.Get(isolate));
    }
  }
  return out;
}

void RegisterHotEventListener(v8::Isolate* isolate, const std::string& event, v8::Local<v8::Function> cb) {
  if (cb.IsEmpty()) return;
  g_hotEventListeners[event].emplace_back(v8::Global<v8::Function>(isolate, cb));
}

void RemoveHotEventListener(v8::Isolate* isolate, const std::string& event, v8::Local<v8::Function> cb) {
  if (cb.IsEmpty()) return;
  auto it = g_hotEventListeners.find(event);
  if (it == g_hotEventListeners.end()) return;
  auto& listeners = it->second;
  // V8 strict equality — same Function reference. A user that registered
  // the same closure twice gets BOTH copies removed; matches
  // `EventTarget.removeEventListener` semantics for repeated registrations.
  for (auto i = listeners.begin(); i != listeners.end();) {
    if (!i->IsEmpty() && i->Get(isolate) == cb) {
      i->Reset();
      i = listeners.erase(i);
    } else {
      ++i;
    }
  }
  if (listeners.empty()) {
    g_hotEventListeners.erase(it);
  }
}

void MarkHotDeclined(const std::string& key) {
  if (key.empty()) return;
  std::lock_guard<std::mutex> lock(g_hotDeclinedMutex);
  g_hotDeclined.insert(key);
}

bool IsHotDeclined(const std::string& key) {
  if (key.empty()) return false;
  std::lock_guard<std::mutex> lock(g_hotDeclinedMutex);
  return g_hotDeclined.find(key) != g_hotDeclined.end();
}

bool IsAnyModuleDeclined(const std::vector<std::string>& keys) {
  std::lock_guard<std::mutex> lock(g_hotDeclinedMutex);
  if (g_hotDeclined.empty()) return false;
  if (keys.empty()) {
    // "Is anything declined?" — yes if the set is non-empty (already
    // checked above).
    return true;
  }
  for (const auto& k : keys) {
    if (g_hotDeclined.find(k) != g_hotDeclined.end()) return true;
  }
  return false;
}

std::vector<v8::Local<v8::Function>> GetHotEventListeners(v8::Isolate* isolate, const std::string& event) {
  std::vector<v8::Local<v8::Function>> out;
  auto it = g_hotEventListeners.find(event);
  if (it != g_hotEventListeners.end()) {
    for (auto& gfn : it->second) {
      if (!gfn.IsEmpty()) out.push_back(gfn.Get(isolate));
    }
  }
  return out;
}

void DispatchHotEvent(v8::Isolate* isolate, v8::Local<v8::Context> context, const std::string& event, v8::Local<v8::Value> data) {
  auto callbacks = GetHotEventListeners(isolate, event);
  const bool verbose = tns::IsScriptLoadingLogEnabled();

  // Single dispatch loop. Always observe `tryCatch.HasCaught()` and
  // `result.ToLocal(...)` for every listener (not just in verbose mode) so the
  // dispatcher's behavior never depends on whether logging is enabled.
  //
  // All `DEBUG_WRITE()` calls are gated behind `verbose`, so default dev
  // sessions stay quiet; the per-listener counters are cheap and feed a
  // verbose-only summary of whether any listener matched — the most useful
  // signal during HMR triage (enable with `logScriptLoading: true`).
  int matched = 0;   // returned undefined OR a truthy non-bool (Promise/object)
  int falsey = 0;    // returned literal `false`
  int threw = 0;     // listener threw synchronously
  int idx = 0;
  for (auto& cb : callbacks) {
    v8::TryCatch tryCatch(isolate);
    v8::Local<v8::Value> args[] = { data };
    v8::MaybeLocal<v8::Value> result = cb->Call(context, v8::Undefined(isolate), 1, args);
    if (tryCatch.HasCaught()) {
      threw++;
      if (verbose) {
        v8::Local<v8::Value> ex = tryCatch.Exception();
        v8::String::Utf8Value m(isolate, ex);
        DEBUG_WRITE("[import.meta.hot] Listener #%d for '%s' threw: %s", idx, event.c_str(), *m ? *m : "(unknown)");
      }
    } else {
      v8::Local<v8::Value> ret;
      if (result.ToLocal(&ret)) {
        if (ret->IsBoolean() && !ret->BooleanValue(isolate)) {
          falsey++;
        } else {
          matched++;
          if (verbose && !ret->IsUndefined()) {
            v8::String::Utf8Value rstr(isolate, ret);
            std::string s = *rstr ? *rstr : "(unknown)";
            DEBUG_WRITE("[import.meta.hot] Listener #%d for '%s' returned: %s", idx, event.c_str(), s.c_str());
          }
        }
      }
    }
    idx++;
  }
  if (verbose) {
    DEBUG_WRITE("[import.meta.hot] dispatch summary event='%s' total=%d matched=%d falsey=%d threw=%d",
                event.c_str(), (int)callbacks.size(), matched, falsey, threw);
  }
}

void InitializeHotEventDispatcher(v8::Isolate* isolate, v8::Local<v8::Context> context) {
  using v8::FunctionCallbackInfo;
  using v8::Local;
  using v8::Value;

  // Create a global function __NS_DISPATCH_HOT_EVENT__(event, data)
  // that the HMR client can call to dispatch events to registered listeners.
  // Returns the number of listeners that were invoked so callers can detect
  // "no-listener" scenarios (which would otherwise look identical to a
  // successful dispatch from the JS side).
  auto dispatchCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    v8::Local<v8::Context> ctx = iso->GetCurrentContext();

    if (info.Length() < 1 || !info[0]->IsString()) {
      info.GetReturnValue().Set(v8::Integer::New(iso, -1));
      return;
    }

    v8::String::Utf8Value eventName(iso, info[0]);
    std::string event = *eventName ? *eventName : "";
    if (event.empty()) {
      info.GetReturnValue().Set(v8::Integer::New(iso, -1));
      return;
    }

    v8::Local<Value> data = info.Length() > 1 ? info[1] : v8::Undefined(iso).As<Value>();

    auto callbacks = GetHotEventListeners(iso, event);

    if (tns::IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[import.meta.hot] Dispatching event '%s' to %d listener(s)", event.c_str(), (int)callbacks.size());
    }

    DispatchHotEvent(iso, ctx, event, data);
    info.GetReturnValue().Set(v8::Integer::New(iso, (int)callbacks.size()));
  };

  // __nsListHotEventListeners() — returns an object mapping every registered
  // event name to its current listener count. Diagnostic helper for HMR
  // dispatch issues so JS code can verify whether a given event has any
  // listeners attached at the time of dispatch (the typical failure mode is
  // a custom event being dispatched before the user's compiled component
  // module has executed its `import.meta.hot.on(...)` registration).
  auto listCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    v8::Local<v8::Context> ctx = iso->GetCurrentContext();
    v8::Local<v8::Object> result = v8::Object::New(iso);
    for (const auto& kv : g_hotEventListeners) {
      v8::Local<v8::String> name = ToV8String(iso, kv.first.c_str());
      v8::Local<v8::Integer> count = v8::Integer::New(iso, (int)kv.second.size());
      (void)result->CreateDataProperty(ctx, name, count);
    }
    info.GetReturnValue().Set(result);
  };

  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Function> dispatchFn = v8::Function::New(context, dispatchCb).ToLocalChecked();
  global->CreateDataProperty(context, ToV8String(isolate, "__NS_DISPATCH_HOT_EVENT__"), dispatchFn).Check();
  v8::Local<v8::Function> listFn = v8::Function::New(context, listCb).ToLocalChecked();
  global->CreateDataProperty(context, ToV8String(isolate, "__nsListHotEventListeners"), listFn).Check();
}

namespace {

// Shared drainer for the dispose/prune twin runners. Both have identical
// snapshot-and-swap semantics (re-entrancy safety, mid-drain
// re-registration, per-callback try/catch with a script-loading log); the
// only things that differ between them are the registry map they touch
// and the log tag. Extracting the common body keeps any future fix to
// the drain protocol from drifting between the two paths.
//
// `registry` is taken by reference so the caller's file-static map is
// mutated in place.
int DrainHotCallbacks(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::vector<std::string>& keys,
    std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>& registry,
    const char* logTag) {
  using v8::Function;
  using v8::Global;
  using v8::HandleScope;
  using v8::Local;
  using v8::Object;
  using v8::TryCatch;
  using v8::Value;

  // Snapshot the keys we'll drain so callers passing an empty list get
  // every registered module. We snapshot first (rather than iterating the
  // map directly) so the registry can be safely mutated mid-drain — both
  // when we erase entries below, and if a callback itself registers a
  // new dispose/prune for the same module (legal per Vite spec; lets
  // users implement hot-data persistence and re-arm side effects).
  std::vector<std::string> targetKeys;
  if (keys.empty()) {
    targetKeys.reserve(registry.size());
    for (const auto& kv : registry) {
      targetKeys.push_back(kv.first);
    }
  } else {
    targetKeys = keys;
  }

  if (targetKeys.empty()) return 0;

  HandleScope handleScope(isolate);
  int executed = 0;

  for (const auto& key : targetKeys) {
    auto it = registry.find(key);
    if (it == registry.end() || it->second.empty()) continue;

    // Move callbacks out of the registry BEFORE invoking. This prevents:
    //   * Re-entrant drain calls from re-firing the same callbacks.
    //   * Callbacks that re-register on the same module from racing with
    //     our iteration — their newly-registered cb lands in the
    //     now-empty bucket and survives until the next drain (the
    //     correct Vite-spec behaviour for a module that re-installs
    //     side-effects after running cleanup).
    std::vector<Global<Function>> callbacks;
    callbacks.swap(it->second);
    registry.erase(it);

    // The user-visible callback signature is `(data) => void`. Pass the
    // module's `hot.data` so users can stash state across the reload —
    // matches Vite's contract documented at:
    //   https://vite.dev/guide/api-hmr#hot-dispose-cb
    //   https://vite.dev/guide/api-hmr#hot-prune-cb
    Local<Object> data = GetOrCreateHotData(isolate, key);
    Local<Value> args[] = { data };

    for (auto& gfn : callbacks) {
      if (gfn.IsEmpty()) continue;
      Local<Function> cb = gfn.Get(isolate);
      if (cb.IsEmpty()) continue;

      TryCatch tryCatch(isolate);
      v8::MaybeLocal<Value> result = cb->Call(context, v8::Undefined(isolate), 1, args);
      (void)result;
      if (tryCatch.HasCaught()) {
        // One bad callback must NEVER take down the HMR cycle for
        // everyone else. Log under the existing script-loading flag so
        // the user has a way to enable diagnostic visibility without
        // recompiling, and continue.
        if (tns::IsScriptLoadingLogEnabled()) {
          Local<Value> ex = tryCatch.Exception();
          v8::String::Utf8Value msg(isolate, ex);
          DEBUG_WRITE("%s callback threw for key=%s: %s",
                      logTag, key.c_str(), *msg ? *msg : "(unknown)");
        }
        // Don't ReThrow — swallow per-callback failures so subsequent
        // drains (and the reboot itself) still run.
        continue;
      }
      ++executed;
    }
  }

  return executed;
}

}  // namespace

int RunHotDisposeCallbacks(v8::Isolate* isolate, v8::Local<v8::Context> context,
                           const std::vector<std::string>& keys) {
  return DrainHotCallbacks(isolate, context, keys, g_hotDispose,
                           "[import.meta.hot.dispose]");
}

void InitializeHotDisposeRunner(v8::Isolate* isolate, v8::Local<v8::Context> context) {
  using v8::FunctionCallbackInfo;
  using v8::Local;
  using v8::Value;

  // Global JS-callable: `__nsRunHmrDispose(keys?: string[]) => number`.
  // Drains `import.meta.hot.dispose` callbacks and returns how many ran. With
  // no argument (or a non-array) it drains every registered module; an array of
  // keys drains only those modules.
  auto runDisposeCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    v8::Local<v8::Context> ctx = iso->GetCurrentContext();

    std::vector<std::string> keys;
    if (info.Length() >= 1 && info[0]->IsArray()) {
      v8::Local<v8::Array> arr = info[0].As<v8::Array>();
      uint32_t length = arr->Length();
      keys.reserve(length);
      for (uint32_t i = 0; i < length; ++i) {
        v8::Local<Value> entry;
        if (!arr->Get(ctx, i).ToLocal(&entry)) continue;
        if (!entry->IsString()) continue;
        v8::String::Utf8Value s(iso, entry);
        if (*s) keys.emplace_back(*s);
      }
    }
    // info[0] is null/undefined/missing/non-array → empty `keys` → drain all.

    int executed = RunHotDisposeCallbacks(iso, ctx, keys);
    info.GetReturnValue().Set(static_cast<int32_t>(executed));
  };

  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Function> fn = v8::Function::New(context, runDisposeCb).ToLocalChecked();
  global->CreateDataProperty(context,
                             ToV8String(isolate, "__nsRunHmrDispose"),
                             fn).Check();
}

int RunHotPruneCallbacks(v8::Isolate* isolate, v8::Local<v8::Context> context,
                         const std::vector<std::string>& keys) {
  return DrainHotCallbacks(isolate, context, keys, g_hotPrune,
                           "[import.meta.hot.prune]");
}

void InitializeHotPruneRunner(v8::Isolate* isolate, v8::Local<v8::Context> context) {
  using v8::FunctionCallbackInfo;
  using v8::Local;
  using v8::Value;

  // Global JS-callable: `__nsRunHmrPrune(keys?: string[]) => number`.
  // Symmetric with `__nsRunHmrDispose`, draining `import.meta.hot.prune`
  // callbacks. No argument drains all registered modules; an array of keys
  // drains only those.
  auto runPruneCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    v8::Local<v8::Context> ctx = iso->GetCurrentContext();

    std::vector<std::string> keys;
    if (info.Length() >= 1 && info[0]->IsArray()) {
      v8::Local<v8::Array> arr = info[0].As<v8::Array>();
      uint32_t length = arr->Length();
      keys.reserve(length);
      for (uint32_t i = 0; i < length; ++i) {
        v8::Local<Value> entry;
        if (!arr->Get(ctx, i).ToLocal(&entry)) continue;
        if (!entry->IsString()) continue;
        v8::String::Utf8Value s(iso, entry);
        if (*s) keys.emplace_back(*s);
      }
    }

    int executed = RunHotPruneCallbacks(iso, ctx, keys);
    info.GetReturnValue().Set(static_cast<int32_t>(executed));
  };

  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Function> fn = v8::Function::New(context, runPruneCb).ToLocalChecked();
  global->CreateDataProperty(context,
                             ToV8String(isolate, "__nsRunHmrPrune"),
                             fn).Check();
}

void InitializeHotDeclinedHelper(v8::Isolate* isolate, v8::Local<v8::Context> context) {
  using v8::FunctionCallbackInfo;
  using v8::Local;
  using v8::Value;

  // Global JS-callable: `__nsHasDeclinedModule(keys?: string[]) => boolean`.
  // The Angular HMR client passes the eviction-set (`msg.evictPaths`) here
  // before applying an update; on `true` it falls back to a full reload via
  // `__nsReloadDevApp` instead of the per-cycle reboot.
  //
  // No-arg form ("is anything declined at all?") returns `true` if any
  // module ever called `import.meta.hot.decline()`. Useful as a coarse
  // pre-check: if the answer is `false` the client can skip the more
  // expensive per-key check below.
  auto hasDeclinedCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    v8::Local<v8::Context> ctx = iso->GetCurrentContext();

    std::vector<std::string> keys;
    if (info.Length() >= 1 && info[0]->IsArray()) {
      v8::Local<v8::Array> arr = info[0].As<v8::Array>();
      uint32_t length = arr->Length();
      keys.reserve(length);
      for (uint32_t i = 0; i < length; ++i) {
        v8::Local<Value> entry;
        if (!arr->Get(ctx, i).ToLocal(&entry)) continue;
        if (!entry->IsString()) continue;
        v8::String::Utf8Value s(iso, entry);
        if (*s) keys.emplace_back(*s);
      }
    }

    bool declined = IsAnyModuleDeclined(keys);
    info.GetReturnValue().Set(declined);
  };

  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Function> fn = v8::Function::New(context, hasDeclinedCb).ToLocalChecked();
  global->CreateDataProperty(context,
                             ToV8String(isolate, "__nsHasDeclinedModule"),
                             fn).Check();
}

void InitializeImportMetaHot(v8::Isolate* isolate,
                             v8::Local<v8::Context> context,
                             v8::Local<v8::Object> importMeta,
                             const std::string& modulePath) {
  using v8::Function;
  using v8::FunctionCallbackInfo;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  // Ensure context scope for property creation
  v8::HandleScope scope(isolate);

  // Canonicalize key to ensure per-module hot.data persists across HMR URLs.
  // Important: this must NOT affect the HTTP loader cache key; otherwise HMR fetches
  // can collapse onto an already-evaluated module and no update occurs.
  auto canonicalHotKey = [&](const std::string& in) -> std::string {
    // Unwrap file://http(s)://...
    std::string s = in;
    if (StartsWith(s, "file://http://") || StartsWith(s, "file://https://")) {
      s = s.substr(strlen("file://"));
    }

    const bool isHttpUrl = StartsWith(s, "http://") || StartsWith(s, "https://");
    if (isHttpUrl) {
      // Preserve meaningful dev-endpoint query identity (for example /ns/core?p=...)
      // while still dropping cache-busters and canonicalizing versioned bridge URLs.
      s = CanonicalizeHttpUrlKey(s);
    }

    // Drop fragment
    size_t hashPos = s.find('#');
    if (hashPos != std::string::npos) s = s.substr(0, hashPos);

    std::string noQuery = s;
    std::string suffix;
    if (!isHttpUrl) {
      size_t qPos = s.find('?');
      noQuery = (qPos == std::string::npos) ? s : s.substr(0, qPos);
    }

    // If it's an http(s) URL, normalize only the path portion below.
    size_t schemePos = noQuery.find("://");
    size_t pathStart = (schemePos == std::string::npos) ? 0 : noQuery.find('/', schemePos + 3);
    if (pathStart == std::string::npos) {
      // No path; return without query
      return noQuery;
    }

    std::string origin = noQuery.substr(0, pathStart);
    std::string pathAndSuffix = noQuery.substr(pathStart);
    if (isHttpUrl) {
      size_t qPos = pathAndSuffix.find('?');
      if (qPos != std::string::npos) {
        suffix = pathAndSuffix.substr(qPos);
        pathAndSuffix = pathAndSuffix.substr(0, qPos);
      }
    }
    std::string path = pathAndSuffix;

    // Normalize NS HMR virtual module paths:
    // /ns/m/__ns_hmr__/<token>/<rest> -> /ns/m/<rest>
    auto normalizeHmrVirtualPath = [&](const char* prefix) {
      size_t prefixLen = strlen(prefix);
      if (path.compare(0, prefixLen, prefix) != 0) {
        return false;
      }

      size_t nextSlash = path.find('/', prefixLen);
      if (nextSlash == std::string::npos) {
        return false;
      }

      path = std::string("/ns/m/") + path.substr(nextSlash + 1);
      return true;
    };

    // Keep import.meta.hot.data stable across both live-tagged and boot-tagged HMR URLs.
    if (!normalizeHmrVirtualPath("/ns/m/__ns_boot__/b1/__ns_hmr__/")) {
      normalizeHmrVirtualPath("/ns/m/__ns_hmr__/");
    }

    auto normalizeBridge = [&](const char* needle) {
      size_t nlen = strlen(needle);
      if (path.compare(0, nlen, needle) != 0) return;
      if (path.size() == nlen) return;
      if (path.size() <= nlen + 1 || path[nlen] != '/') return;

      size_t i = nlen + 1;
      size_t j = i;
      while (j < path.size() && std::isdigit(static_cast<unsigned char>(path[j]))) {
        j++;
      }
      if (j == i) return;
      if (j != path.size()) return;

      path = std::string(needle);
    };

    normalizeBridge("/ns/rt");
    normalizeBridge("/ns/core");

    // Normalize common script extensions so `/foo` and `/foo.ts` share hot.data.
    const char* exts[] = {".ts", ".tsx", ".js", ".jsx", ".mjs", ".cjs"};
    for (auto ext : exts) {
      if (EndsWith(path, ext)) {
        path = path.substr(0, path.size() - strlen(ext));
        break;
      }
    }

    // Also drop `.vue`? No — SFC endpoints should stay distinct.
    return origin + path + suffix;
  };

  const std::string key = canonicalHotKey(modulePath);
  if (tns::IsScriptLoadingLogEnabled()) {
    bool isReload = (g_hotData.find(key) != g_hotData.end());
    DEBUG_WRITE("[hmr][import.meta.hot] module=%s key=%s isReload=%d", modulePath.c_str(), key.c_str(), isReload);
  }

  // Helper to capture key in function data
  auto makeKeyData = [&](const std::string& k) -> Local<Value> {
    return ToV8String(isolate, k.c_str());
  };

  // accept([deps], cb?) — register cb if provided. The deps array is accepted
  // for Vite API compatibility but does not drive selective acceptance.
  auto acceptCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    Local<Value> data = info.Data();
    std::string key;
    if (!data.IsEmpty()) {
      v8::String::Utf8Value s(iso, data);
      key = *s ? *s : "";
    }
    v8::Local<v8::Function> cb;
    if (info.Length() >= 1 && info[0]->IsFunction()) {
      cb = info[0].As<v8::Function>();
    } else if (info.Length() >= 2 && info[1]->IsFunction()) {
      cb = info[1].As<v8::Function>();
    }
    if (!cb.IsEmpty()) {
      RegisterHotAccept(iso, key, cb);
    }
    // Return undefined
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  // dispose(cb) — register disposer
  auto disposeCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    Local<Value> data = info.Data();
    std::string key;
    if (!data.IsEmpty()) { v8::String::Utf8Value s(iso, data); key = *s ? *s : ""; }
    if (info.Length() >= 1 && info[0]->IsFunction()) {
      RegisterHotDispose(iso, key, info[0].As<v8::Function>());
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  // prune(cb) — register a callback that fires when this module is removed
  // from the dep graph (NOT on every replacement — that's `dispose`). Today
  // the NS HMR pipeline does wholesale reboots so prune callbacks rarely
  // fire, but the registry is plumbed end-to-end so a future per-module
  // HMR client can drain `g_hotPrune` via `__nsRunHmrPrune`.
  auto pruneCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    Local<Value> data = info.Data();
    std::string key;
    if (!data.IsEmpty()) { v8::String::Utf8Value s(iso, data); key = *s ? *s : ""; }
    if (info.Length() >= 1 && info[0]->IsFunction()) {
      RegisterHotPrune(iso, key, info[0].As<v8::Function>());
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  // decline() — mark this module as not hot-updateable (Vite spec). Adds the
  // canonical key to `g_hotDeclined`; the HMR client checks this set via
  // `__nsHasDeclinedModule(updatedKeys)` before applying an update and
  // converts the cycle into a full reload (`__nsReloadDevApp`) on a hit.
  auto declineCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    Local<Value> data = info.Data();
    std::string key;
    if (!data.IsEmpty()) { v8::String::Utf8Value s(iso, data); key = *s ? *s : ""; }
    if (!key.empty()) {
      MarkHotDeclined(key);
      if (tns::IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("[import.meta.hot.decline] key=%s", key.c_str());
      }
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  // invalidate(message?) — request a full app reload. Per Vite spec this
  // notifies the dev server; in NS we short-circuit to the runtime's
  // `__nsReloadDevApp` global (which already does the invalidate + re-import
  // dance). The optional `message` argument is logged.
  //
  // We invoke `__nsReloadDevApp` from a microtask so the user's current
  // execution stack (which contains the `invalidate()` call site) finishes
  // before the runtime tears down for reload — calling synchronously would
  // try to re-bootstrap from inside an in-flight callback.
  auto invalidateCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    Local<Value> data = info.Data();
    std::string key;
    if (!data.IsEmpty()) { v8::String::Utf8Value s(iso, data); key = *s ? *s : ""; }

    std::string message;
    if (info.Length() >= 1 && info[0]->IsString()) {
      v8::String::Utf8Value m(iso, info[0]);
      if (*m) message = *m;
    }
    if (tns::IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[import.meta.hot.invalidate] key=%s message=%s",
                  key.c_str(), message.empty() ? "(none)" : message.c_str());
    }

    v8::Local<v8::Context> ctx = iso->GetCurrentContext();
    v8::Local<v8::Object> global = ctx->Global();
    v8::Local<Value> reloadVal;
    if (!global->Get(ctx, ToV8String(iso, "__nsReloadDevApp")).ToLocal(&reloadVal)) {
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }
    if (!reloadVal->IsFunction()) {
      // Older runtime / non-dev mode — silently no-op. Nothing else
      // we can usefully do here.
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }

    // Defer the call via a resolved-promise microtask so we exit the
    // current call stack before the reload tears the runtime down. Using
    // microtasks rather than `setTimeout` keeps the deferral inside the
    // same V8 microtask checkpoint — no event-loop delay, no UI hitch.
    v8::Local<v8::Function> reloadFn = reloadVal.As<v8::Function>();
    v8::Local<v8::Promise::Resolver> resolver;
    if (v8::Promise::Resolver::New(ctx).ToLocal(&resolver)) {
      v8::Local<v8::Function> deferred =
          v8::Function::New(ctx, [](const FunctionCallbackInfo<Value>& innerInfo) {
            v8::Isolate* innerIso = innerInfo.GetIsolate();
            v8::Local<v8::Context> innerCtx = innerIso->GetCurrentContext();
            v8::Local<v8::Object> innerGlobal = innerCtx->Global();
            v8::Local<Value> reloadVal;
            if (!innerGlobal->Get(innerCtx, ToV8String(innerIso, "__nsReloadDevApp")).ToLocal(&reloadVal)) return;
            if (!reloadVal->IsFunction()) return;
            v8::Local<v8::Function> reloadFn = reloadVal.As<v8::Function>();
            v8::TryCatch tc(innerIso);
            (void)reloadFn->Call(innerCtx, v8::Undefined(innerIso), 0, nullptr);
            // Reload is a fire-and-forget Promise on its own. Per-call
            // failures aren't surfaced — they're not actionable from
            // user code.
          }).ToLocalChecked();
      v8::Local<v8::Promise> p = resolver->GetPromise();
      v8::MaybeLocal<v8::Promise> chained = p->Then(ctx, deferred);
      (void)chained;
      (void)resolver->Resolve(ctx, v8::Undefined(iso));
    } else {
      // Promise machinery unavailable — fall back to a synchronous call.
      // The user's current call stack will be torn down mid-execution
      // but the user already requested a full reload, so that's
      // acceptable.
      v8::TryCatch tc(iso);
      (void)reloadFn->Call(ctx, v8::Undefined(iso), 0, nullptr);
    }

    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  // on(event, cb) — register custom event listener
  auto onCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    if (info.Length() < 2) {
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }
    if (!info[0]->IsString() || !info[1]->IsFunction()) {
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }
    v8::String::Utf8Value eventName(iso, info[0]);
    std::string event = *eventName ? *eventName : "";
    if (!event.empty()) {
      RegisterHotEventListener(iso, event, info[1].As<v8::Function>());
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  // off(event, cb) — counterpart to `on`. Removes a previously-registered
  // listener (matched by V8 strict equality on the Function reference).
  auto offCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    if (info.Length() < 2) {
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }
    if (!info[0]->IsString() || !info[1]->IsFunction()) {
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }
    v8::String::Utf8Value eventName(iso, info[0]);
    std::string event = *eventName ? *eventName : "";
    if (!event.empty()) {
      RemoveHotEventListener(iso, event, info[1].As<v8::Function>());
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  // send(event, data) — send a custom message to the dev server. The runtime
  // intentionally does not own a WebSocket; it delegates to a JS-installed
  // `globalThis.__nsHmrSendToServer(event, data)` so the WebSocket-owning
  // JS layer (typically @nativescript/vite's HMR client) keeps sole
  // responsibility for transport. If no JS-side handler is installed (older
  // HMR clients, non-dev mode) this is a clean no-op.
  auto sendCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    v8::Local<v8::Context> ctx = iso->GetCurrentContext();
    v8::Local<v8::Object> global = ctx->Global();
    v8::Local<Value> handlerVal;
    if (!global->Get(ctx, ToV8String(iso, "__nsHmrSendToServer")).ToLocal(&handlerVal)) {
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }
    if (!handlerVal->IsFunction()) {
      info.GetReturnValue().Set(v8::Undefined(iso));
      return;
    }
    v8::Local<v8::Function> handler = handlerVal.As<v8::Function>();

    // Forward `(event, data)` exactly as called. We don't enforce types on
    // `event` (Vite spec only specifies the first arg as a string but
    // implementations let it be coerced) and we pass `data` through
    // verbatim — JS-side serialization is the transport's concern.
    int argc = info.Length();
    if (argc > 2) argc = 2;
    std::vector<v8::Local<Value>> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) args.push_back(info[i]);

    v8::TryCatch tc(iso);
    (void)handler->Call(ctx, v8::Undefined(iso), argc, args.data());
    if (tc.HasCaught() && tns::IsScriptLoadingLogEnabled()) {
      v8::Local<Value> ex = tc.Exception();
      v8::String::Utf8Value m(iso, ex);
      DEBUG_WRITE("[import.meta.hot.send] handler threw: %s", *m ? *m : "(unknown)");
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  Local<Object> hot = Object::New(isolate);
  // Stable flags
  hot->CreateDataProperty(context, ToV8String(isolate, "data"),
                          GetOrCreateHotData(isolate, key)).Check();
  // Methods
  hot->CreateDataProperty(
    context, ToV8String(isolate, "accept"),
      v8::Function::New(context, acceptCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
    context, ToV8String(isolate, "dispose"),
      v8::Function::New(context, disposeCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
    context, ToV8String(isolate, "prune"),
      v8::Function::New(context, pruneCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
    context, ToV8String(isolate, "decline"),
      v8::Function::New(context, declineCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
    context, ToV8String(isolate, "invalidate"),
      v8::Function::New(context, invalidateCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
    context, ToV8String(isolate, "on"),
      v8::Function::New(context, onCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
    context, ToV8String(isolate, "off"),
      v8::Function::New(context, offCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
    context, ToV8String(isolate, "send"),
      v8::Function::New(context, sendCb, makeKeyData(key)).ToLocalChecked()).Check();

  // Attach to import.meta
  importMeta->CreateDataProperty(
    context, ToV8String(isolate, "hot"),
    hot).Check();
}

// ─────────────────────────────────────────────────────────────
// HTTP loader helpers + speculative module prefetcher.
//
// The cache lives in `g_prefetchCache`, populated by background fetch
// threads (`std::thread` + `std::condition_variable` for concurrency
// gating). `HttpFetchText` checks the cache first (a "destructive read":
// consumed entries are erased) and only falls through to a fresh JNI
// HTTP fetch on a miss.
//
// Two flavours of kickstart drive the cache:
//   - `KickstartHmrPrefetchSync(seed)` — cold-boot BFS over static
//     imports, recursively widening from a seed URL until the wave
//     drains or `timeoutSeconds` elapses.
//   - `KickstartHmrPrefetchUrlsSync(urls)` — HMR-driven parallel
//     fetch for a precomputed inverse-dep closure (e.g. `evictPaths`
//     from a dev-server save message). No graph walk: server already
//     told us the exact set to refresh.
//
// `RegisterHttpFetchYield` exposes a pluggable "yield to host" hook
// called from inside `KickstartRunSync`'s wait loop. The default is a no-op;
// embedders can install their own pump to keep the UI responsive.

namespace {

// Cap how many import specifiers we honour per module on the BFS, and
// how large a body we'll scan at all. Pretty-printed bundler output
// can easily blow past both — at that point we're better off paying
// the network on demand than parsing the giant string twice.
constexpr size_t kPrefetchMaxImportsPerModule = 256;
constexpr size_t kPrefetchMaxScanBytes = 2 * 1024 * 1024;  // 2 MiB

std::mutex g_prefetchMutex;
auto* _g_prefetchCache = new std::unordered_map<std::string, std::string>();
auto& g_prefetchCache = *_g_prefetchCache;

inline bool IsHorizontalWs(char c) { return c == ' ' || c == '\t'; }
inline bool IsIdentifierChar(unsigned char c) {
  return std::isalnum(c) || c == '_' || c == '$';
}
inline char PreviousNonHwsChar(const std::string& s, size_t pos) {
  if (pos == 0) return 0;
  ssize_t i = static_cast<ssize_t>(pos) - 1;
  while (i >= 0 && IsHorizontalWs(s[i])) --i;
  if (i < 0) return 0;
  return s[i];
}

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

// Two-pass scan over a module body to extract its static-import URLs:
//   Pass 1: `... from "<spec>"` (covers all import-from forms, including
//           default, namespace, named, side-effect re-exports).
//   Pass 2: `import "<spec>"` (side-effect imports).
// Dynamic imports (`import(…)`) and `.from(…)` member access are
// explicitly rejected — accepting them would feed us too many false
// positives that dilute the BFS budget.
std::vector<std::string> ScanStaticImportSpecifiers(const std::string& source, size_t maxResults) {
  std::vector<std::string> result;
  if (source.size() > kPrefetchMaxScanBytes) return result;
  std::unordered_set<std::string> seen;
  result.reserve(16);

  auto captureSpecAfter = [&](size_t cursor) -> ssize_t {
    while (cursor < source.size()) {
      char c = source[cursor];
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { cursor++; continue; }
      break;
    }
    if (cursor >= source.size()) return -1;
    char quote = source[cursor];
    if (quote != '"' && quote != '\'' && quote != '`') return -1;
    size_t end = source.find(quote, cursor + 1);
    if (end == std::string::npos) return -1;
    std::string spec = source.substr(cursor + 1, end - cursor - 1);
    if (!spec.empty() && spec.find('\n') == std::string::npos && seen.insert(spec).second) {
      result.push_back(std::move(spec));
    }
    return static_cast<ssize_t>(end + 1);
  };

  {
    const char* needle = "from";
    const size_t needleLen = 4;
    size_t pos = 0;
    while (pos < source.size() && result.size() < maxResults) {
      size_t hit = source.find(needle, pos);
      if (hit == std::string::npos) break;
      if (hit > 0 && IsIdentifierChar(static_cast<unsigned char>(source[hit - 1]))) { pos = hit + 1; continue; }
      size_t after = hit + needleLen;
      if (after < source.size() && IsIdentifierChar(static_cast<unsigned char>(source[after]))) { pos = hit + 1; continue; }
      char prev = PreviousNonHwsChar(source, hit);
      bool ok = (prev == '}' || prev == '*' || prev == ',' ||
                 IsIdentifierChar(static_cast<unsigned char>(prev)));
      if (!ok) { pos = hit + 1; continue; }
      ssize_t adv = captureSpecAfter(after);
      if (adv < 0) { pos = hit + 1; continue; }
      pos = static_cast<size_t>(adv);
    }
  }
  {
    const char* needle = "import";
    const size_t needleLen = 6;
    size_t pos = 0;
    while (pos < source.size() && result.size() < maxResults) {
      size_t hit = source.find(needle, pos);
      if (hit == std::string::npos) break;
      if (hit > 0 && IsIdentifierChar(static_cast<unsigned char>(source[hit - 1]))) { pos = hit + 1; continue; }
      size_t after = hit + needleLen;
      if (after < source.size() && IsIdentifierChar(static_cast<unsigned char>(source[after]))) { pos = hit + 1; continue; }
      char prev = PreviousNonHwsChar(source, hit);
      bool atStmtStart = (prev == 0 || prev == '\n' || prev == '\r' || prev == ';' || prev == '}');
      if (!atStmtStart) { pos = hit + 1; continue; }
      size_t cursor = after;
      while (cursor < source.size() && IsHorizontalWs(source[cursor])) cursor++;
      if (cursor >= source.size()) break;
      char next = source[cursor];
      if (next == '(') { pos = hit + 1; continue; }
      if (next != '"' && next != '\'' && next != '`') { pos = hit + 1; continue; }
      ssize_t adv = captureSpecAfter(cursor);
      if (adv < 0) { pos = hit + 1; continue; }
      pos = static_cast<size_t>(adv);
    }
  }
  return result;
}

}  // anonymous namespace

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
  if (tns::StartsWith(specifier, "http://") || tns::StartsWith(specifier, "https://")) {
    return specifier;
  }
  bool isRelative = tns::StartsWith(specifier, "./") || tns::StartsWith(specifier, "../");
  bool isRootAbs = !specifier.empty() && specifier[0] == '/';
  if (!isRelative && !isRootAbs) return "";

  if (!(tns::StartsWith(parentUrl, "http://") || tns::StartsWith(parentUrl, "https://"))) {
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

namespace {

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

// ─────────────────────────────────────────────────────────────
// HTTP loader helpers (the speculative-prefetcher additions live above).

// Drop fragments and normalize parameters for consistent registry keys.
std::string CanonicalizeHttpUrlKey(const std::string& url) {
  // Some loaders wrap HTTP module URLs as file://http(s)://...
  std::string normalizedUrl = url;
  if (StartsWith(normalizedUrl, "file://http://") || StartsWith(normalizedUrl, "file://https://")) {
    normalizedUrl = normalizedUrl.substr(strlen("file://"));
  }
  if (!(StartsWith(normalizedUrl, "http://") || StartsWith(normalizedUrl, "https://"))) {
    return normalizedUrl;
  }
  // Remove fragment
  size_t hashPos = normalizedUrl.find('#');
  std::string noHash = (hashPos == std::string::npos) ? normalizedUrl : normalizedUrl.substr(0, hashPos);

  // Split into origin+path and query
  size_t qPos = noHash.find('?');
  std::string originAndPath = (qPos == std::string::npos) ? noHash : noHash.substr(0, qPos);
  std::string query = (qPos == std::string::npos) ? std::string() : noHash.substr(qPos + 1);

  // Normalize bridge endpoints to keep a single realm across HMR updates:
  // - /ns/rt/<ver>    -> /ns/rt
  // - /ns/core/<ver>  -> /ns/core
  size_t schemePos = originAndPath.find("://");
  if (schemePos != std::string::npos) {
    size_t pathStart = originAndPath.find('/', schemePos + 3);
    if (pathStart != std::string::npos) {
      std::string pathOnly = originAndPath.substr(pathStart);
      auto normalizeBridge = [&](const char* needle) {
        size_t nlen = strlen(needle);
        if (pathOnly.size() <= nlen) return false;
        if (pathOnly.compare(0, nlen, needle) != 0) return false;
        if (pathOnly.size() == nlen) return true;
        if (pathOnly[nlen] != '/') return false;
        size_t i = nlen + 1;
        size_t j = i;
        while (j < pathOnly.size() && isdigit((unsigned char)pathOnly[j])) j++;
        // Only normalize exact version segment: /ns/*/<digits> (no further segments)
        if (j == i) return false;
        if (j != pathOnly.size()) return false;
        originAndPath = originAndPath.substr(0, pathStart) + std::string(needle);
        return true;
      };
      if (!normalizeBridge("/ns/rt")) {
        normalizeBridge("/ns/core");
      }
    }
  }

  if (query.empty()) return originAndPath;

  // Strip ?import markers and sort remaining query params for stability
  std::vector<std::string> kept;
  size_t start = 0;
  while (start <= query.size()) {
    size_t amp = query.find('&', start);
    std::string pair = (amp == std::string::npos) ? query.substr(start) : query.substr(start, amp - start);
    if (!pair.empty()) {
      size_t eq = pair.find('=');
      std::string name = (eq == std::string::npos) ? pair : pair.substr(0, eq);
      if (!(name == "import")) kept.push_back(pair);
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
// background prefetch threads (which already pre-filtered URLs) so the
// public `HttpFetchText` can keep its allowlist-and-cache logic in one
// place without recursing into itself. Returns true on success (2xx,
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

  // Speculative-prefetch cache check (destructive read).
  //
  // Honoured only when the opt-in prefetcher is enabled (package.json
  // "httpModulePrefetch", default false). When disabled, the prefetch wave
  // never populates the cache (see KickstartHmrPrefetch*Sync) AND this read
  // is skipped, restoring the pre-prefetcher fetch behavior bit-for-bit.
  // Volatility is enforced upstream by `EvictHttpModulePrefetchCacheUrls` on
  // the eviction set rather than by gating reads here. Consuming the entry on
  // hit guarantees that a re-fetch after HMR goes back to the network for
  // fresh source.
  if (IsHttpModulePrefetchEnabled()) {
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

// True raw HTTP fetch path. See PerformHttpFetchOnceSync forward
// declaration above for purpose. We extracted this from HttpFetchText
// so the prefetcher (which already filtered URLs and intends to
// populate the cache) doesn't re-check the cache itself.
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
    jstring jUrlStr = env.NewStringUTF(url.c_str());
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
    return status >= 200 && status < 300 && !out.empty();
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
// Kickstart prefetcher.
//
// `KickstartHmrPrefetchSync` does seed-rooted BFS; `KickstartHmrPrefetchUrlsSync`
// runs a parallel fetch wave over a pre-computed URL list. Both funnel
// through `KickstartRunSync` so the wait loop / metrics / logging are
// shared. Concurrency is bounded by a counting semaphore implemented
// with mutex + condition variable (NDKs do not yet ship the C++20
// `std::counting_semaphore`).

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
  bool recursive = true;

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

    // In recursive (cold-boot BFS) mode, skip URLs already in the cache.
    // In HMR mode (recursive=false) the caller has *explicitly* listed
    // URLs to refresh — honoring an existing cache entry would feed V8
    // the stale body. So we skip this short-circuit when recursive=false.
    if (ctx->recursive) {
      std::lock_guard<std::mutex> lock(g_prefetchMutex);
      if (g_prefetchCache.find(urlRef) != g_prefetchCache.end()) continue;
    }

    ctx->EnterPending();
    std::string urlCopy = urlRef;
    const bool hmrMode = !ctx->recursive;
    auto ctxCopy = ctx;
    std::thread([ctxCopy, urlCopy, hmrMode]() {
      ctxCopy->concurrency->Acquire();
      std::string body, contentType;
      int status = 0;
      bool ok = PerformHttpFetchOnceSync(urlCopy, body, contentType, status);
      if (ok && status >= 200 && status < 300 && !body.empty()) {
        size_t bodySize = body.size();
        std::string scanSource;
        {
          std::lock_guard<std::mutex> lock(g_prefetchMutex);
          if (hmrMode) {
            // HMR: caller's URLs are by definition the authoritative copy.
            // Overwrite unconditionally; any older cache entry is stale.
            auto& slot = g_prefetchCache[urlCopy];
            slot = std::move(body);
            scanSource = slot;
            bodySize = slot.size();
          } else {
            // Cold boot: insert-without-overwrite. Another path may have
            // already landed this URL via opt-in speculative prefetch;
            // honour whichever copy got there first.
            auto inserted = g_prefetchCache.emplace(urlCopy, std::move(body));
            if (inserted.second) {
              scanSource = inserted.first->second;
            } else {
              scanSource = inserted.first->second;
              bodySize = inserted.first->second.size();
            }
          }
        }
        ctxCopy->fetchedCount.fetch_add(1, std::memory_order_relaxed);
        ctxCopy->bytes.fetch_add(bodySize, std::memory_order_relaxed);
        if (ctxCopy->recursive) {
          std::vector<std::string> specs =
              ScanStaticImportSpecifiers(scanSource, kPrefetchMaxImportsPerModule);
          if (!specs.empty()) {
            std::vector<std::string> nextUrls;
            nextUrls.reserve(specs.size());
            for (const std::string& spec : specs) {
              std::string absUrl = ResolveImportSpecifierAgainstUrl(spec, urlCopy);
              if (!absUrl.empty()) nextUrls.push_back(std::move(absUrl));
            }
            if (!nextUrls.empty()) {
              KickstartScheduleUrls(ctxCopy, std::move(nextUrls));
            }
          }
        }
      }
      ctxCopy->concurrency->Release();
      ctxCopy->LeavePending();
    }).detach();
  }
}

bool KickstartRunSync(std::vector<std::string> urls, int maxConcurrent,
                      double timeoutSeconds, bool recursive, const char* logLabel,
                      const std::string& diagSeed, size_t* outFetchedCount,
                      uint64_t* outElapsedMs) {
  if (urls.empty()) return false;

  std::vector<std::string> filtered;
  filtered.reserve(urls.size());
  for (auto& u : urls) {
    if (u.empty()) continue;
    if (!IsRemoteUrlAllowed(u)) continue;
    filtered.push_back(std::move(u));
  }
  if (filtered.empty()) return false;

  if (maxConcurrent <= 0) maxConcurrent = 16;
  if (timeoutSeconds <= 0.0) timeoutSeconds = 10.0;

  const auto start = std::chrono::steady_clock::now();

  auto ctx = std::make_shared<KickstartContext>();
  ctx->concurrency = std::make_unique<CountingSemaphore>(maxConcurrent);
  ctx->recursive = recursive;

  KickstartScheduleUrls(ctx, std::move(filtered));

  // Wait loop. Uses a slice-based timeout so the host runloop (e.g. the
  // JS-thread pump) gets a chance to drain between slices. 50ms is short
  // enough to feel responsive and long enough to avoid spinning.
  const int sliceMs = 50;
  const auto deadline = start + std::chrono::milliseconds(static_cast<long>(timeoutSeconds * 1000.0));
  bool drained = false;
  while (true) {
    drained = ctx->WaitDrainSlice(sliceMs);
    if (drained) break;
    if (std::chrono::steady_clock::now() >= deadline) break;
    InvokeHttpFetchYield();
  }

  const auto end = std::chrono::steady_clock::now();
  const uint64_t elapsedMs =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  const size_t fetched = ctx->fetchedCount.load(std::memory_order_relaxed);
  const size_t bytes = ctx->bytes.load(std::memory_order_relaxed);

  if (outFetchedCount) *outFetchedCount = fetched;
  if (outElapsedMs) *outElapsedMs = elapsedMs;

  if (IsScriptLoadingLogEnabled()) {
    if (recursive) {
      DEBUG_WRITE("[hmr-kickstart][%s] seed=%s fetched=%lu bytes=%lu ms=%llu status=%s concurrency=%d",
                  logLabel ? logLabel : "bfs", diagSeed.c_str(),
                  (unsigned long)fetched, (unsigned long)bytes,
                  (unsigned long long)elapsedMs,
                  drained ? "drained" : "timeout", maxConcurrent);
    } else {
      DEBUG_WRITE("[hmr-kickstart][%s] urls=%lu fetched=%lu bytes=%lu ms=%llu status=%s concurrency=%d",
                  logLabel ? logLabel : "list", (unsigned long)urls.size(),
                  (unsigned long)fetched, (unsigned long)bytes,
                  (unsigned long long)elapsedMs,
                  drained ? "drained" : "timeout", maxConcurrent);
    }
  }
  return drained;
}

}  // anonymous namespace

bool KickstartHmrPrefetchSync(const std::string& seedUrl,
                              int maxConcurrent,
                              double timeoutSeconds,
                              size_t* outFetchedCount,
                              uint64_t* outElapsedMs) {
  if (seedUrl.empty()) return false;
  // Opt-in gate (package.json "httpModulePrefetch", default false). Layered on
  // top of the IsRemoteUrlAllowed network gate; when disabled, the speculative
  // prefetch wave never runs.
  if (!IsHttpModulePrefetchEnabled()) return false;
  if (!IsRemoteUrlAllowed(seedUrl)) return false;
  std::vector<std::string> seeds{seedUrl};
  return KickstartRunSync(std::move(seeds), maxConcurrent, timeoutSeconds,
                          /*recursive=*/true, "bfs", seedUrl,
                          outFetchedCount, outElapsedMs);
}

bool KickstartHmrPrefetchUrlsSync(const std::vector<std::string>& urls,
                                  int maxConcurrent,
                                  double timeoutSeconds,
                                  size_t* outFetchedCount,
                                  uint64_t* outElapsedMs) {
  if (urls.empty()) return false;
  // Opt-in gate (package.json "httpModulePrefetch", default false). Per-URL
  // network access is still gated by IsRemoteUrlAllowed inside KickstartRunSync.
  if (!IsHttpModulePrefetchEnabled()) return false;
  std::string diagSeed;
  for (const auto& u : urls) {
    if (!u.empty()) { diagSeed = u; break; }
  }
  return KickstartRunSync(std::vector<std::string>(urls), maxConcurrent,
                          timeoutSeconds, /*recursive=*/false, "list",
                          diagSeed, outFetchedCount, outElapsedMs);
}

// ─────────────────────────────────────────────────────────────
// HMR + dev-session JS-callable globals.
//
// Installs the JS-callable globals the @nativescript/vite HMR client and
// deterministic dev-session bootstrap rely on. `Runtime::RunModule(const
// char*)` returns `void` on Android rather than `bool`, so failures are
// surfaced by
// catching `NativeScriptException`).

namespace {

// Helper used by both `__nsConfigureRuntime` and the `__nsConfigureDevRuntime`
// alias to apply the dev-session config payload, so both entry points behave
// identically.
void ConfigureDevRuntimeCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
  bool logScriptLoading = tns::IsScriptLoadingLogEnabled();

  // Defense in depth: never mutate the process-wide import map / volatile
  // patterns in a release build. The install site (Runtime::PrepareV8Runtime)
  // is already debug-gated, so this only fires if that gate is bypassed.
  if (!tns::IsDebuggable()) {
    return;
  }

  if (info.Length() < 1 || !info[0]->IsObject()) {
    if (logScriptLoading) {
      DEBUG_WRITE("[__nsConfigureRuntime] expected config object argument");
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
    std::string jsonStr;
    if (importMapVal->IsString()) {
      v8::String::Utf8Value utf8(isolate, importMapVal);
      if (*utf8) jsonStr = *utf8;
    } else if (importMapVal->IsObject()) {
      v8::Local<v8::Value> jsonGlobalVal;
      if (ctx->Global()->Get(ctx, ToV8String(isolate, "JSON")).ToLocal(&jsonGlobalVal) &&
          jsonGlobalVal->IsObject()) {
        v8::Local<v8::Object> jsonObj = jsonGlobalVal.As<v8::Object>();
        v8::Local<v8::Value> stringifyVal;
        if (jsonObj->Get(ctx, ToV8String(isolate, "stringify")).ToLocal(&stringifyVal) &&
            stringifyVal->IsFunction()) {
          v8::Local<v8::Function> stringify = stringifyVal.As<v8::Function>();
          v8::Local<v8::Value> args[] = {importMapVal};
          v8::Local<v8::Value> result;
          if (stringify->Call(ctx, jsonObj, 1, args).ToLocal(&result) && result->IsString()) {
            v8::String::Utf8Value utf8(isolate, result);
            if (*utf8) jsonStr = *utf8;
          }
        }
      }
    }
    if (!jsonStr.empty()) {
      SetImportMap(jsonStr);
      if (logScriptLoading) {
        DEBUG_WRITE("[__nsConfigureRuntime] import map set (%zu bytes)", jsonStr.size());
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
        DEBUG_WRITE("[__nsConfigureRuntime] %zu volatile patterns set", patterns.size());
      }
    }
  }
}

// Helper: wrap Runtime::RunModule (which is `void` on Android) in a try/catch
// so we can report success/failure to the dev-session callbacks by treating
// any NativeScriptException as failure.
//
// `outErrorMessage` captures `ex.what()` from the inner NativeScriptException
// so the caller can pass the real cause through to the JS-side rejection
// instead of losing it behind a generic "failed to import" message. (The
// `[dev-session] RunModule failed for %s: %s` log is gated behind
// `logScriptLoading` so users who haven't opted in still see at least the
// wrapped reason via the rejected `__nsStartDevSession` promise.)
bool RunModuleSafe(Runtime* runtime, const std::string& url,
                   std::string* outErrorMessage = nullptr) {
  try {
    runtime->RunModule(url.c_str());
    return true;
  } catch (NativeScriptException& ex) {
    if (tns::IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[dev-session] RunModule failed for %s: %s",
                  url.c_str(), ex.what());
    }
    if (outErrorMessage) {
      *outErrorMessage = ex.what() ? ex.what() : "<no-message>";
    }
    return false;
  } catch (...) {
    if (tns::IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[dev-session] RunModule unknown exception for %s", url.c_str());
    }
    if (outErrorMessage) {
      *outErrorMessage = "<unknown C++ exception>";
    }
    return false;
  }
}

void StartDevSessionCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

  // Defense in depth: dev sessions never start in a release build. The install
  // site is already debug-gated; reject here too in case that gate is bypassed.
  if (!tns::IsDebuggable()) {
    info.GetReturnValue().Set(CreateRejectedPromise(
        ctx, v8::Exception::Error(ToV8String(
                 isolate,
                 "[__nsStartDevSession] dev sessions are disabled in release builds"))));
    return;
  }

  if (info.Length() < 1 || !info[0]->IsObject()) {
    info.GetReturnValue().Set(CreateRejectedPromise(
        ctx, v8::Exception::TypeError(
                 ToV8String(isolate, "[__nsStartDevSession] expected config object"))));
    return;
  }

  v8::Local<v8::Object> config = info[0].As<v8::Object>();
  DevSessionState next;
  std::string sessionError;
  if (!ReadDevSessionConfig(isolate, ctx, config, &next, &sessionError)) {
    info.GetReturnValue().Set(CreateRejectedPromise(
        ctx, v8::Exception::TypeError(ToV8String(isolate, sessionError.c_str()))));
    return;
  }

  DevSessionState previous = GetActiveDevSessionSnapshot();
  bool sessionChanged = HasDevSessionChanged(previous, next);
  bool logScriptLoading = tns::IsScriptLoadingLogEnabled();

  if (sessionChanged && previous.active) {
    std::vector<std::string> staleUrls = CollectSessionModuleUrls(previous);
    if (logScriptLoading) {
      DEBUG_WRITE("[__nsStartDevSession] session changed old=%s new=%s invalidating=%lu",
                  previous.sessionId.c_str(), next.sessionId.c_str(),
                  (unsigned long)staleUrls.size());
    }
    if (!staleUrls.empty()) {
      InvalidateModules(staleUrls);
    }
  }

  if (!sessionChanged && previous.active && previous.started) {
    if (logScriptLoading) {
      DEBUG_WRITE("[__nsStartDevSession] session already active: %s", next.sessionId.c_str());
    }
    info.GetReturnValue().Set(CreateResolvedPromise(isolate, ctx));
    return;
  }

  // Optional native runtime-config delegation. Gated on a global flag the
  // JS side may set to opt in. When disabled, the JS dev session is
  // expected to call `__nsConfigureRuntime` itself.
  bool nativeDelegation = false;
  v8::Local<v8::Value> delegationFlag;
  if (ctx->Global()
          ->Get(ctx, ToV8String(isolate, "__NS_EXPERIMENTAL_NATIVE_RUNTIME_CONFIG_URL__"))
          .ToLocal(&delegationFlag) &&
      !delegationFlag->IsUndefined() && !delegationFlag->IsNull()) {
    nativeDelegation = delegationFlag->BooleanValue(isolate);
  }
  if (!next.runtimeConfigUrl.empty() && nativeDelegation) {
    if (logScriptLoading) {
      DEBUG_WRITE("[__nsStartDevSession] runtimeConfigUrl fetch start session=%s url=%s",
                  next.sessionId.c_str(), next.runtimeConfigUrl.c_str());
    }
    std::string runtimeConfigError;
    if (!ApplyDevRuntimeConfigFromUrl(next.runtimeConfigUrl, &runtimeConfigError)) {
      if (logScriptLoading) {
        DEBUG_WRITE("[__nsStartDevSession] runtimeConfigUrl fetch failed session=%s url=%s",
                    next.sessionId.c_str(), next.runtimeConfigUrl.c_str());
      }
      info.GetReturnValue().Set(CreateRejectedPromise(
          ctx, v8::Exception::Error(ToV8String(isolate, runtimeConfigError.c_str()))));
      return;
    }
    if (logScriptLoading) {
      DEBUG_WRITE("[__nsStartDevSession] runtimeConfigUrl fetch complete session=%s url=%s",
                  next.sessionId.c_str(), next.runtimeConfigUrl.c_str());
    }
  } else if (!next.runtimeConfigUrl.empty() && logScriptLoading) {
    DEBUG_WRITE(
        "[__nsStartDevSession] runtimeConfigUrl native delegation disabled; using JS-configured "
        "runtime session=%s url=%s",
        next.sessionId.c_str(), next.runtimeConfigUrl.c_str());
  }

  ApplyDevSessionGlobals(isolate, ctx, next);
  StoreActiveDevSession(next);

  Runtime* runtime = Runtime::GetRuntime(isolate);
  if (runtime == nullptr) {
    if (logScriptLoading) {
      DEBUG_WRITE("[__nsStartDevSession] runtime unavailable for session=%s",
                  next.sessionId.c_str());
    }
    info.GetReturnValue().Set(CreateRejectedPromise(
        ctx, v8::Exception::Error(ToV8String(isolate, "[__nsStartDevSession] runtime unavailable"))));
    return;
  }

  if (logScriptLoading) {
    DEBUG_WRITE("[__nsStartDevSession] clientUrl import start session=%s url=%s",
                next.sessionId.c_str(), next.clientUrl.c_str());
  }
  {
    std::string clientErr;
    if (!RunModuleSafe(runtime, next.clientUrl, &clientErr)) {
      std::string msg = std::string("[__nsStartDevSession] failed to import clientUrl: ") +
                        next.clientUrl + " — " + (clientErr.empty() ? "<no-message>" : clientErr);
      info.GetReturnValue().Set(CreateRejectedPromise(
          ctx, v8::Exception::Error(ToV8String(isolate, msg.c_str()))));
      return;
    }
  }
  if (logScriptLoading) {
    DEBUG_WRITE("[__nsStartDevSession] clientUrl import complete session=%s url=%s",
                next.sessionId.c_str(), next.clientUrl.c_str());
    DEBUG_WRITE("[__nsStartDevSession] entryUrl import start session=%s url=%s",
                next.sessionId.c_str(), next.entryUrl.c_str());
  }
  {
    std::string entryErr;
    if (!RunModuleSafe(runtime, next.entryUrl, &entryErr)) {
      std::string msg = std::string("[__nsStartDevSession] failed to import entryUrl: ") +
                        next.entryUrl + " — " + (entryErr.empty() ? "<no-message>" : entryErr);
      info.GetReturnValue().Set(CreateRejectedPromise(
          ctx, v8::Exception::Error(ToV8String(isolate, msg.c_str()))));
      return;
    }
  }

  next.started = true;
  StoreActiveDevSession(next);

  if (logScriptLoading) {
    DEBUG_WRITE("[__nsStartDevSession] entryUrl import complete session=%s url=%s",
                next.sessionId.c_str(), next.entryUrl.c_str());
    DEBUG_WRITE("[__nsStartDevSession] session=%s platform=%s origin=%s client=%s entry=%s changed=%s",
                next.sessionId.c_str(), next.platform.c_str(), next.origin.c_str(),
                next.clientUrl.c_str(), next.entryUrl.c_str(),
                sessionChanged ? "true" : "false");
  }
  info.GetReturnValue().Set(CreateResolvedPromise(isolate, ctx));
}

void InvalidateModulesCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

  if (info.Length() < 1 || !info[0]->IsArray()) {
    if (tns::IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[__nsInvalidateModules] expected array of URL strings");
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
      DEBUG_WRITE("[__nsKickstartHmrPrefetch] expected (seedUrl: string, options?) or (urls: string[], options?)");
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

  size_t fetched = 0;
  uint64_t elapsedMs = 0;
  if (info[0]->IsArray()) {
    v8::Local<v8::Array> arr = info[0].As<v8::Array>();
    const uint32_t len = arr->Length();
    std::vector<std::string> urls;
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
    if (urls.empty()) {
      buildResult(false, 0, 0);
      return;
    }
    bool ok = KickstartHmrPrefetchUrlsSync(urls, maxConcurrent, timeoutSeconds,
                                           &fetched, &elapsedMs);
    buildResult(ok, fetched, elapsedMs);
    return;
  }

  v8::String::Utf8Value seedUtf8(isolate, info[0]);
  if (!*seedUtf8) {
    buildResult(false, 0, 0);
    return;
  }
  std::string seedUrl(*seedUtf8);
  bool ok = KickstartHmrPrefetchSync(seedUrl, maxConcurrent, timeoutSeconds,
                                     &fetched, &elapsedMs);
  buildResult(ok, fetched, elapsedMs);
}

void ReloadDevAppCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
  bool logScriptLoading = tns::IsScriptLoadingLogEnabled();

  DevSessionState session = GetActiveDevSessionSnapshot();
  if (!session.active || session.entryUrl.empty()) {
    if (logScriptLoading) {
      DEBUG_WRITE("[__nsReloadDevApp] no active dev session");
    }
    info.GetReturnValue().Set(CreateRejectedPromise(
        ctx, v8::Exception::Error(
                 ToV8String(isolate, "[__nsReloadDevApp] no active dev session"))));
    return;
  }
  std::vector<std::string> sessionUrls = CollectSessionModuleUrls(session);
  if (logScriptLoading) {
    DEBUG_WRITE("[__nsReloadDevApp] invalidating session=%s urls=%lu",
                session.sessionId.c_str(), (unsigned long)sessionUrls.size());
  }
  if (!sessionUrls.empty()) {
    InvalidateModules(sessionUrls);
  }
  SetDevSessionBootComplete(isolate, ctx, false);
  Runtime* runtime = Runtime::GetRuntime(isolate);
  if (runtime == nullptr) {
    info.GetReturnValue().Set(CreateRejectedPromise(
        ctx, v8::Exception::Error(
                 ToV8String(isolate, "[__nsReloadDevApp] runtime unavailable"))));
    return;
  }
  if (logScriptLoading) {
    DEBUG_WRITE("[__nsReloadDevApp] entryUrl import start session=%s url=%s",
                session.sessionId.c_str(), session.entryUrl.c_str());
  }
  if (!RunModuleSafe(runtime, session.entryUrl)) {
    info.GetReturnValue().Set(CreateRejectedPromise(
        ctx, v8::Exception::Error(
                 ToV8String(isolate, "[__nsReloadDevApp] failed to import entryUrl"))));
    return;
  }
  if (logScriptLoading) {
    DEBUG_WRITE("[__nsReloadDevApp] entryUrl import complete session=%s url=%s",
                session.sessionId.c_str(), session.entryUrl.c_str());
    DEBUG_WRITE("[__nsReloadDevApp] session=%s reload complete (invalidated=%lu)",
                session.sessionId.c_str(), (unsigned long)sessionUrls.size());
  }
  info.GetReturnValue().Set(CreateResolvedPromise(isolate, ctx));
}

void ApplyStyleUpdateCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
  const bool logEnabled = tns::IsScriptLoadingLogEnabled();

  if (info.Length() < 1 || !info[0]->IsObject()) {
    if (logEnabled) DEBUG_WRITE("[__nsApplyStyleUpdate] expected payload object");
    return;
  }
  v8::Local<v8::Object> payload = info[0].As<v8::Object>();
  std::string cssText;
  std::string url;
  GetOptionalStringProperty(isolate, ctx, payload, "cssText", &cssText);
  GetOptionalStringProperty(isolate, ctx, payload, "url", &url);
  if (cssText.empty()) {
    if (logEnabled) DEBUG_WRITE("[__nsApplyStyleUpdate] missing cssText payload");
    return;
  }

  v8::Local<v8::Value> applicationValue;
  if (!ctx->Global()->Get(ctx, ToV8String(isolate, "Application")).ToLocal(&applicationValue) ||
      !applicationValue->IsObject()) {
    if (logEnabled) DEBUG_WRITE("[__nsApplyStyleUpdate] Application is unavailable for %s", url.c_str());
    return;
  }
  v8::Local<v8::Object> applicationObject = applicationValue.As<v8::Object>();
  v8::Local<v8::Value> addCssValue;
  if (!applicationObject->Get(ctx, ToV8String(isolate, "addCss")).ToLocal(&addCssValue) ||
      !addCssValue->IsFunction()) {
    if (logEnabled) DEBUG_WRITE("[__nsApplyStyleUpdate] Application.addCss is unavailable for %s", url.c_str());
    return;
  }
  v8::TryCatch tc(isolate);
  v8::Local<v8::Value> args[] = {ToV8String(isolate, cssText.c_str())};
  v8::Local<v8::Value> ignored;
  bool addCssCalled =
      addCssValue.As<v8::Function>()->Call(ctx, applicationObject, 1, args).ToLocal(&ignored);
  if (addCssCalled && !tc.HasCaught()) {
    v8::Local<v8::Value> getRootViewValue;
    if (applicationObject->Get(ctx, ToV8String(isolate, "getRootView")).ToLocal(&getRootViewValue) &&
        getRootViewValue->IsFunction()) {
      v8::Local<v8::Value> rootViewValue;
      if (getRootViewValue.As<v8::Function>()
              ->Call(ctx, applicationObject, 0, nullptr)
              .ToLocal(&rootViewValue) &&
          rootViewValue->IsObject()) {
        v8::Local<v8::Object> rootViewObject = rootViewValue.As<v8::Object>();
        v8::Local<v8::Value> cssStateChangeValue;
        if (rootViewObject->Get(ctx, ToV8String(isolate, "_onCssStateChange"))
                .ToLocal(&cssStateChangeValue) &&
            cssStateChangeValue->IsFunction()) {
          (void)cssStateChangeValue.As<v8::Function>()
              ->Call(ctx, rootViewObject, 0, nullptr)
              .ToLocal(&ignored);
        }
      }
    }
  }
  if (tc.HasCaught() && logEnabled) {
    DEBUG_WRITE("[__nsApplyStyleUpdate] failed for %s", url.c_str());
  }
  if (logEnabled) {
    DEBUG_WRITE("[__nsApplyStyleUpdate] applied %s", url.c_str());
  }
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

void InstallGlobalFunction(v8::Isolate* isolate, v8::Local<v8::Context> context,
                           const char* name, v8::FunctionCallback callback) {
  v8::Local<v8::FunctionTemplate> fnTpl = v8::FunctionTemplate::New(isolate, callback);
  v8::Local<v8::Function> fn = fnTpl->GetFunction(context).ToLocalChecked();
  fn->SetName(ToV8String(isolate, name));
  context->Global()->Set(context, ToV8String(isolate, name), fn).FromMaybe(false);
  MirrorFunctionOnGlobalThis(isolate, context, name);
}

}  // anonymous namespace

void InitializeHmrDevGlobals(v8::Isolate* isolate, v8::Local<v8::Context> context) {
  // Install the per-module HMR helpers and the dev-session global surface.
  // The main-thread AND debug-mode gating happens at the SINGLE call site in
  // `Runtime::PrepareV8Runtime` (`if (m_isMainThread && isDebuggable)`), so a
  // release build never reaches this function. The session-mutating callbacks
  // below additionally fail safe via `tns::IsDebuggable()` as defense in depth
  // in case a future caller forgets the call-site gate.
  try {
    InitializeHotEventDispatcher(isolate, context);
    InitializeHotDisposeRunner(isolate, context);
    InitializeHotPruneRunner(isolate, context);
    InitializeHotDeclinedHelper(isolate, context);
  } catch (...) {
    // Don't crash if HMR setup fails — the rest of init must still run.
  }

  // Install the dev-session bootstrap surface.
  InstallGlobalFunction(isolate, context, "__nsConfigureDevRuntime", ConfigureDevRuntimeCallback);
  InstallGlobalFunction(isolate, context, "__nsConfigureRuntime", ConfigureDevRuntimeCallback);
  (void)context->Global()
      ->CreateDataProperty(context, ToV8String(isolate, "__nsSupportsRuntimeConfigUrl"),
                           v8::Boolean::New(isolate, true))
      .FromMaybe(false);

  InstallGlobalFunction(isolate, context, "__nsStartDevSession", StartDevSessionCallback);
  InstallGlobalFunction(isolate, context, "__nsInvalidateModules", InvalidateModulesCallback);
  InstallGlobalFunction(isolate, context, "__nsKickstartHmrPrefetch", KickstartHmrPrefetchCallback);
  InstallGlobalFunction(isolate, context, "__nsReloadDevApp", ReloadDevAppCallback);
  InstallGlobalFunction(isolate, context, "__nsApplyStyleUpdate", ApplyStyleUpdateCallback);
  InstallGlobalFunction(isolate, context, "__nsGetLoadedModuleUrls", GetLoadedModuleUrlsCallback);
}

void CleanupHMRGlobals() {
  // Reset all v8::Global handles BEFORE the isolate is disposed.
  // These static maps survive past isolate teardown and their destructors
  // (__cxa_finalize_ranges) would call v8::Global::Reset() on an already-
  // destroyed isolate, causing a crash in v8::internal::GlobalHandles::Destroy().
  for (auto& kv : g_hotData) { kv.second.Reset(); }
  g_hotData.clear();

  for (auto& kv : g_hotAccept) {
    for (auto& fn : kv.second) { fn.Reset(); }
  }
  g_hotAccept.clear();

  for (auto& kv : g_hotDispose) {
    for (auto& fn : kv.second) { fn.Reset(); }
  }
  g_hotDispose.clear();

  for (auto& kv : g_hotPrune) {
    for (auto& fn : kv.second) { fn.Reset(); }
  }
  g_hotPrune.clear();

  for (auto& kv : g_hotEventListeners) {
    for (auto& fn : kv.second) { fn.Reset(); }
  }
  g_hotEventListeners.clear();

  {
    // `g_hotDeclined` holds plain strings — no v8::Global handles — but
    // we still clear it under its own mutex on teardown so a re-launched
    // runtime in the same process starts with a clean slate.
    std::lock_guard<std::mutex> lock(g_hotDeclinedMutex);
    g_hotDeclined.clear();
  }

  // Drop any speculatively-prefetched module sources. These are plain
  // std::string buffers (no v8::Global), but flushing them on teardown
  // prevents stale source from leaking into a re-launched runtime in
  // the same process.
  ClearHttpModulePrefetchCache();
}

} // namespace tns

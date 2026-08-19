#include "HttpLoader.h"

#include <android/looper.h>
#include <unistd.h>
#include <v8.h>

#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "ArgConverter.h"
#include "JEnv.h"
#include "ModuleInternal.h"
#include "ModuleInternalCallbacks.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "TraceLog.h"
#include "robin_hood.h"
#include "v8-json.h"

namespace tns {

static inline bool StartsWith(const std::string& s, const char* prefix) {
    size_t n = strlen(prefix);
    return s.size() >= n && s.compare(0, n, prefix) == 0;
}

static inline v8::Local<v8::String> ToV8String(v8::Isolate* isolate, const char* str) {
    return ArgConverter::ConvertToV8String(isolate, str ? std::string(str) : std::string());
}

static inline v8::Local<v8::String> ToV8String(v8::Isolate* isolate, const std::string& str) {
    return ArgConverter::ConvertToV8String(isolate, str);
}

// ─────────────────────────────────────────────────────────────
// Remote-module security gate

static std::once_flag s_securityConfigInitFlag;
static bool s_allowRemoteModules = false;
static std::vector<std::string> s_remoteModuleAllowlist;
static bool s_isDebuggable = false;

static bool RemoteUrlMatchesAllowlistEntry(const std::string& url, const std::string& entry) {
    if (entry.empty()) return false;
    if (url.size() < entry.size()) return false;
    if (url.compare(0, entry.size(), entry) != 0) return false;
    if (url.size() == entry.size()) return true;
    if (entry.back() == '/') return true;
    const char next = url[entry.size()];
    return next == '/' || next == '?' || next == '#';
}

static void InitializeSecurityConfig() {
    std::call_once(s_securityConfigInitFlag, []() {
        try {
            JEnv env;
            jclass runtimeClass = env.FindClass("com/tns/Runtime");
            if (runtimeClass == nullptr) {
                return;
            }

            jmethodID isDebuggableMid = env.GetStaticMethodID(runtimeClass, "isDebuggable", "()Z");
            if (isDebuggableMid != nullptr) {
                s_isDebuggable = env.CallStaticBooleanMethod(runtimeClass, isDebuggableMid) ==
                                 JNI_TRUE;
            }

            if (s_isDebuggable) {
                s_allowRemoteModules = true;
                return;
            }

            jmethodID allowRemoteMid =
                    env.GetStaticMethodID(runtimeClass, "isRemoteModulesAllowed", "()Z");
            if (allowRemoteMid != nullptr) {
                s_allowRemoteModules =
                        env.CallStaticBooleanMethod(runtimeClass, allowRemoteMid) == JNI_TRUE;
            }

            jmethodID getAllowlistMid = env.GetStaticMethodID(
                    runtimeClass, "getRemoteModuleAllowlist", "()[Ljava/lang/String;");
            if (getAllowlistMid != nullptr) {
                jobjectArray allowlistArray = static_cast<jobjectArray>(
                        env.CallStaticObjectMethod(runtimeClass, getAllowlistMid));
                if (allowlistArray != nullptr) {
                    jsize len = env.GetArrayLength(allowlistArray);
                    for (jsize i = 0; i < len; i++) {
                        jstring jstr =
                                static_cast<jstring>(env.GetObjectArrayElement(allowlistArray, i));
                        if (jstr != nullptr) {
                            const char* str = env.GetStringUTFChars(jstr, nullptr);
                            if (str != nullptr) {
                                s_remoteModuleAllowlist.emplace_back(str);
                                env.ReleaseStringUTFChars(jstr, str);
                            }
                            env.DeleteLocalRef(jstr);
                        }
                    }
                    env.DeleteLocalRef(allowlistArray);
                }
            }
        } catch (...) {
            // Keep defaults (remote modules disabled)
        }
    });
}

bool IsDebuggable() {
    InitializeSecurityConfig();
    return s_isDebuggable;
}

bool IsRemoteModulesAllowed() {
    if (IsDebuggable()) {
        return true;
    }
    InitializeSecurityConfig();
    return s_allowRemoteModules;
}

bool IsRemoteUrlAllowed(const std::string& url) {
    if (IsDebuggable()) {
        return true;
    }

    InitializeSecurityConfig();
    if (!s_allowRemoteModules) {
        return false;
    }

    if (s_remoteModuleAllowlist.empty()) {
        return true;
    }

    for (const std::string& entry : s_remoteModuleAllowlist) {
        if (RemoteUrlMatchesAllowlistEntry(url, entry)) {
            return true;
        }
    }

    return false;
}

// ─────────────────────────────────────────────────────────────
// Boot-evaluation flag

// Nonzero while this thread is evaluating an entry module (main or worker) —
// the only window in which the fetch yield may pump the looper: during entry
// evaluation nothing else owns it, while pumping mid-app would re-enter
// arbitrary user code under a synchronous fetch. Thread-local because a fetch
// and the entry evaluation that triggered it always share a thread, so a
// worker booting never arms the main thread's pump. The runtime derives this
// itself — there is no client signal to forget.
static thread_local int t_bootEvaluationDepth = 0;

void SetBootEvaluationActive(bool active) { t_bootEvaluationDepth += active ? 1 : -1; }

static inline bool IsBootEvaluationActive() { return t_bootEvaluationDepth > 0; }

// ─────────────────────────────────────────────────────────────
// Canonical module keys

std::string CanonicalizeHttpUrlKey(const std::string& url) {
    std::string normalizedUrl = url;
    if (StartsWith(normalizedUrl, "file://http://") || StartsWith(normalizedUrl, "file://https://")) {
        normalizedUrl = normalizedUrl.substr(strlen("file://"));
    }
    if (!(StartsWith(normalizedUrl, "http://") || StartsWith(normalizedUrl, "https://"))) {
        return normalizedUrl;
    }
    size_t hashPos = normalizedUrl.find('#');
    std::string noHash =
            (hashPos == std::string::npos) ? normalizedUrl : normalizedUrl.substr(0, hashPos);

    size_t schemePos = noHash.find("://");
    if (schemePos == std::string::npos) {
        size_t q = noHash.find('?');
        return (q == std::string::npos) ? noHash : noHash.substr(0, q);
    }
    size_t pathStart = noHash.find('/', schemePos + 3);
    if (pathStart == std::string::npos) {
        return noHash;
    }
    size_t qPos = noHash.find('?', pathStart);
    std::string originAndPath = (qPos == std::string::npos) ? noHash : noHash.substr(0, qPos);
    std::string query = (qPos == std::string::npos) ? std::string() : noHash.substr(qPos + 1);

    // This key is the module registry/cache key. For general-purpose HTTP
    // module loading the query can be part of a module's identity (auth,
    // content versioning, routing), so query normalization applies only to the
    // endpoints the client names, through the vocabulary it supplies.
    //
    // `preserveQueryFor` is checked BEFORE the dev-endpoint prefix test, so it
    // covers endpoints nested under a dev prefix: for some endpoints the query
    // IS the identity, and stripping it would collapse every refetch onto the
    // boot-time key.
    //
    // Until a client supplies that vocabulary, canonicalization is purely
    // mechanical: the fragment is gone and the query stays. Which params are
    // cache-busters and which paths are dev endpoints is knowledge only the
    // client has; guessing would silently collapse two distinct modules onto
    // one registry key.
    const CanonicalizationConfig* canon = CanonicalizationConfigForCurrentIsolate();
    if (canon == nullptr) {
        return noHash;
    }
    {
        std::string pathOnly = originAndPath.substr(pathStart);
        for (const auto& p : canon->preserveQueryPrefixes) {
            if (!p.empty() && pathOnly.find(p) != std::string::npos) {
                return noHash;
            }
        }
        bool isDevEndpoint = false;
        for (const auto& p : canon->devPathPrefixes) {
            if (!p.empty() && StartsWith(pathOnly, p.c_str())) {
                isDevEndpoint = true;
                break;
            }
        }
        if (!isDevEndpoint) {
            return noHash;
        }
    }

    if (query.empty()) return originAndPath;

    std::vector<std::string> kept;
    size_t start = 0;
    while (start <= query.size()) {
        size_t amp = query.find('&', start);
        std::string pair =
                (amp == std::string::npos) ? query.substr(start) : query.substr(start, amp - start);
        if (!pair.empty()) {
            size_t eq = pair.find('=');
            std::string name = (eq == std::string::npos) ? pair : pair.substr(0, eq);
            const bool drop = std::find(canon->stripParams.begin(), canon->stripParams.end(),
                                        name) != canon->stripParams.end();
            if (!drop) kept.push_back(pair);
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

// ─────────────────────────────────────────────────────────────
// Eviction-driven fetch cache-bust
//
// Process-global because it belongs to the transport, not to any isolate: the
// HTTP cache layers it defeats are shared by the whole process. The set is
// keyed by canonical keys the callers compute on their own isolate's thread
// and pass in by value, so nothing here canonicalizes.

static std::mutex g_bustNextFetchMutex;
static robin_hood::unordered_set<std::string> g_bustNextFetchKeys;

void MarkUrlsForCacheBust(const std::vector<std::string>& urls) {
    if (urls.empty()) return;
    std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
    for (const auto& url : urls) {
        if (url.empty()) continue;
        if (!(StartsWith(url, "http://") || StartsWith(url, "https://"))) continue;
        g_bustNextFetchKeys.insert(CanonicalizeHttpUrlKey(url));
    }
}

static bool IsUrlMarkedForCacheBust(const std::string& canonicalKey) {
    std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
    if (g_bustNextFetchKeys.empty()) return false;
    return g_bustNextFetchKeys.find(canonicalKey) != g_bustNextFetchKeys.end();
}

static void ClearCacheBustForUrl(const std::string& canonicalKey) {
    std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
    if (g_bustNextFetchKeys.empty()) return;
    g_bustNextFetchKeys.erase(canonicalKey);
}

static void ClearAllCacheBustMarks() {
    std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
    g_bustNextFetchKeys.clear();
}

// ─────────────────────────────────────────────────────────────
// JNI fetch diagnostics + request builder

static thread_local std::string t_lastHttpFetchErrorReason;

static void RecordLastHttpFetchError(const char* stage, const std::string& excClass,
                                     const std::string& excMsg) {
    t_lastHttpFetchErrorReason.assign("stage=");
    t_lastHttpFetchErrorReason.append(stage ? stage : "?");
    t_lastHttpFetchErrorReason.append(" class=");
    t_lastHttpFetchErrorReason.append(excClass);
    t_lastHttpFetchErrorReason.append(" msg=");
    t_lastHttpFetchErrorReason.append(excMsg);
}

static void ClearLastHttpFetchErrorReason() {
    t_lastHttpFetchErrorReason.clear();
}

std::string TakeLastHttpFetchErrorReason() {
    std::string out = std::move(t_lastHttpFetchErrorReason);
    t_lastHttpFetchErrorReason.clear();
    return out;
}

// Describes and clears a pending Java exception. The introspection calls go
// through the raw JNIEnv: JEnv's wrappers turn a pending Java exception into a
// thrown NativeScriptException, which here would replace the exception being
// described with the failure to describe it.
static bool DrainPendingJniException(JEnv& env, std::string& outClassName, std::string& outMessage) {
    outClassName.clear();
    outMessage.clear();
    JNIEnv* raw = env;
    jthrowable th = raw->ExceptionOccurred();
    if (!th) return false;
    raw->ExceptionClear();

    jclass clsThrowable = env.GetObjectClass(th);
    if (clsThrowable) {
        jclass clsClass = env.FindClass("java/lang/Class");
        if (clsClass) {
            jmethodID getName = env.GetMethodID(clsClass, "getName", "()Ljava/lang/String;");
            if (getName) {
                jstring jName = static_cast<jstring>(raw->CallObjectMethod(clsThrowable, getName));
                raw->ExceptionClear();
                if (jName) {
                    outClassName = ArgConverter::jstringToString(jName);
                }
            }
        }
        jmethodID toString = env.GetMethodID(clsThrowable, "toString", "()Ljava/lang/String;");
        if (toString) {
            jstring jMsg = static_cast<jstring>(raw->CallObjectMethod(th, toString));
            raw->ExceptionClear();
            if (jMsg) {
                outMessage = ArgConverter::jstringToString(jMsg);
            }
        }
    }
    raw->ExceptionClear();
    return true;
}

static bool PerformHttpFetchOnceSync(const std::string& url, const std::string& canonicalKey,
                                     std::string& out, std::string& contentType, int& status,
                                     bool& bustApplied);
static void MaybePumpJSThreadDuringBoot();
static inline void InvokeHttpFetchYield();

static std::string ApplyCacheBustNonce(const std::string& url, const std::string& canonicalKey,
                                       bool* outBustRequested) {
    std::string fetchUrl = url;
    const bool bustRequested = IsUrlMarkedForCacheBust(canonicalKey);
    if (outBustRequested) *outBustRequested = bustRequested;
    if (bustRequested) {
        static std::atomic<uint64_t> s_fetchSeq{0};
        const uint64_t seq = s_fetchSeq.fetch_add(1, std::memory_order_relaxed);
        const uint64_t nowMs = static_cast<uint64_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count());
        fetchUrl += (url.find('?') == std::string::npos) ? '?' : '&';
        fetchUrl += "__ns_dev_nonce=";
        fetchUrl += std::to_string(nowMs);
        fetchUrl += "-";
        fetchUrl += std::to_string(seq);
    }
    return fetchUrl;
}

static void DisableHttpKeepAliveOnce(JEnv& env) {
    static std::atomic<bool> sKeepAliveDisabled{false};
    if (sKeepAliveDisabled.exchange(true)) {
        return;
    }
    jclass clsSystem = env.FindClass("java/lang/System");
    if (clsSystem) {
        jmethodID setProperty = env.GetStaticMethodID(
                clsSystem, "setProperty",
                "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        if (setProperty) {
            jstring jKey = env.NewStringUTF("http.keepAlive");
            jstring jVal = env.NewStringUTF("false");
            env.CallStaticObjectMethod(clsSystem, setProperty, jKey, jVal);
            env.ExceptionClear();
        }
    }
}

static void PermitAllStrictMode(JEnv& env) {
    jclass clsStrict = env.FindClass("android/os/StrictMode");
    jclass clsPolicyBuilder = env.FindClass("android/os/StrictMode$ThreadPolicy$Builder");
    if (!clsStrict || !clsPolicyBuilder) {
        return;
    }
    jmethodID builderCtor = env.GetMethodID(clsPolicyBuilder, "<init>", "()V");
    jobject builder = env.NewObject(clsPolicyBuilder, builderCtor);
    if (!builder) {
        return;
    }
    jmethodID permitAll = env.GetMethodID(clsPolicyBuilder, "permitAll",
                                          "()Landroid/os/StrictMode$ThreadPolicy$Builder;");
    jobject builder2 = permitAll ? env.CallObjectMethod(builder, permitAll) : builder;
    jmethodID build = env.GetMethodID(clsPolicyBuilder, "build",
                                      "()Landroid/os/StrictMode$ThreadPolicy;");
    jobject policy = build ? env.CallObjectMethod(builder2 ? builder2 : builder, build) : nullptr;
    if (policy) {
        jmethodID setThreadPolicy = env.GetStaticMethodID(
                clsStrict, "setThreadPolicy", "(Landroid/os/StrictMode$ThreadPolicy;)V");
        if (setThreadPolicy) {
            env.CallStaticVoidMethod(clsStrict, setThreadPolicy, policy);
        }
    }
}

// ── The module response policy ───────────────────────────────
//
// Module scripts are strict about MIME: the HTML spec's "fetch a single module
// script" fails the fetch outright for anything that is not a JavaScript or
// JSON MIME type, where a classic script would sniff and run it anyway. That
// strictness is the whole point — an SPA dev server answering an unknown path
// with `200 text/html` should say so, not hand HTML to the parser and produce
// `Unexpected token '<'` from somewhere deep in the graph.
//
// Both transports classify here, so the synchronous fallback and the async
// walk cannot disagree about what a response means.

// "text/javascript; charset=utf-8" → "text/javascript": parameters stripped,
// trimmed, lowercased.
static std::string MimeEssence(const std::string& contentType) {
    size_t semi = contentType.find(';');
    std::string essence =
            semi == std::string::npos ? contentType : contentType.substr(0, semi);
    size_t begin = essence.find_first_not_of(" \t");
    if (begin == std::string::npos) {
        return "";
    }
    size_t end = essence.find_last_not_of(" \t");
    essence = essence.substr(begin, end - begin + 1);
    for (char& c : essence) {
        c = (char)tolower((unsigned char)c);
    }
    return essence;
}

// The HTML spec's JavaScript MIME type essence list, verbatim.
static bool IsJavaScriptMimeEssence(const std::string& essence) {
    static const char* const kJavaScriptEssences[] = {"application/ecmascript",
                                                      "application/javascript",
                                                      "application/x-ecmascript",
                                                      "application/x-javascript",
                                                      "text/ecmascript",
                                                      "text/javascript",
                                                      "text/javascript1.0",
                                                      "text/javascript1.1",
                                                      "text/javascript1.2",
                                                      "text/javascript1.3",
                                                      "text/javascript1.4",
                                                      "text/javascript1.5",
                                                      "text/jscript",
                                                      "text/livescript",
                                                      "text/x-ecmascript",
                                                      "text/x-javascript"};
    for (const char* candidate : kJavaScriptEssences) {
        if (essence == candidate) {
            return true;
        }
    }
    return false;
}

// A JSON MIME type is application/json, text/json, or any `+json` subtype.
static bool IsJsonMimeEssence(const std::string& essence) {
    if (essence == "application/json" || essence == "text/json") {
        return true;
    }
    const std::string suffix = "+json";
    return essence.size() > suffix.size() &&
           essence.compare(essence.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// `transportOk` means a response arrived at all; everything else about it —
// status, MIME, emptiness — is policy decided here. `body` is moved into the
// result on success.
static void ClassifyModuleResponse(const std::string& url, bool transportOk, int status,
                                   const std::string& contentType, std::string& body,
                                   ModuleFetchResult& result) {
    result.status = status;
    result.contentType = contentType;

    if (!transportOk) {
        result.failureReason = "HTTP import failed: " + url + " (network error)";
        return;
    }
    if (status == 204 || status == 205) {
        // "No content" carries no module, which the web treats as a network
        // error for a module script rather than as an empty module.
        result.failureReason =
                "HTTP import failed: " + url + " (status=" + std::to_string(status) +
                ", no content)";
        return;
    }
    if (status < 200 || status >= 300) {
        result.failureReason =
                "HTTP import failed: " + url + " (status=" + std::to_string(status) + ")";
        return;
    }

    const std::string essence = MimeEssence(contentType);
    if (essence.empty()) {
        result.failureReason =
                "Expected a JavaScript module but '" + url + "' responded with no MIME type";
        return;
    }

    if (IsJsonMimeEssence(essence)) {
        if (body.empty()) {
            result.failureReason =
                    "Expected a JSON module but '" + url + "' responded with an empty body";
            return;
        }
        result.kind = ModuleResponseKind::kJson;
    } else if (IsJavaScriptMimeEssence(essence)) {
        result.kind = ModuleResponseKind::kJavaScript;
        // An empty 2xx JavaScript body is a valid module: type-only TypeScript
        // modules transform to zero runtime code and dev servers serve them as
        // empty 200s. Failing here would kill the whole graph with a misleading
        // "status=200".
        if (body.empty()) {
            body = "export {};\n";
            TNS_DEBUG(Esm, "[http-loader] empty 2xx body for %s — serving canonical empty module",
                           url.c_str());
        }
    } else {
        result.failureReason = "Expected a JavaScript module but '" + url +
                               "' responded with MIME type '" + essence + "'";
        return;
    }

    result.ok = true;
    result.body = std::move(body);
}

bool HttpFetchModule(const std::string& url, ModuleFetchResult& result) {
    result = ModuleFetchResult();
    ClearLastHttpFetchErrorReason();

    // Security gate: the single point of enforcement for all HTTP module
    // loading, checked before any network turn.
    if (!IsRemoteUrlAllowed(url)) {
        result.status = 403;
        result.failureReason =
                "HTTP import blocked: remote module loading is not allowed for " + url;
        TNS_DEBUG(Esm, "[http-esm][security][blocked] %s", url.c_str());
        return false;
    }

    const bool urlLogEnabled = LogCategoryEnabled(LogCategory::Fetch);
    const auto netStart = urlLogEnabled ? std::chrono::steady_clock::now()
                                        : std::chrono::steady_clock::time_point{};

    // Canonicalize here, on the caller's isolate thread: the vocabulary the
    // key depends on belongs to that isolate, and the transport below must
    // never reach for it.
    const std::string canonicalKey = CanonicalizeHttpUrlKey(url);

    std::string body;
    std::string contentType;
    int status = 0;
    bool bustApplied = false;
    bool transportOk =
            PerformHttpFetchOnceSync(url, canonicalKey, body, contentType, status, bustApplied);
    if (!transportOk) {
        // One retry, and only for a transport error: an HTTP status is an
        // answer, not a failure to communicate, so asking again would just
        // repeat it.
        TNS_DEBUG(Esm, "[http-loader] retrying %s after initial fetch error", url.c_str());
        usleep(120 * 1000);
        transportOk =
                PerformHttpFetchOnceSync(url, canonicalKey, body, contentType, status, bustApplied);
    }

    ClassifyModuleResponse(url, transportOk, status, contentType, body, result);

    // A cache-bust mark is only satisfied by a response the loader can actually
    // use: a 404, or a 200 that classified as something other than a module,
    // leaves it armed for the next attempt.
    if (result.ok && bustApplied) {
        ClearCacheBustForUrl(canonicalKey);
    }

    if (!result.ok) {
        TNS_DEBUG(Esm, "[http-loader][fetch-sync][reject] %s", result.failureReason.c_str());
        return false;
    }

    TNS_DEBUG(Esm, "[http-loader] fetched status=%d content-type=%s bytes=%llu", result.status,
                   result.contentType.empty() ? "<none>" : result.contentType.c_str(),
                   (unsigned long long)result.body.size());
    if (urlLogEnabled) {
        const auto netMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                                   std::chrono::steady_clock::now() - netStart)
                                   .count();
        TNS_DEBUG(Fetch, "[http-loader][fetch][network] %s bytes=%lu ms=%lld", url.c_str(),
                         (unsigned long)result.body.size(), (long long)netMs);
    }

    InvokeHttpFetchYield();
    return true;
}

// Runs on whichever thread drives the fetch — the JS thread for the sync path,
// a detached background thread for the async one. `canonicalKey` is computed
// by the caller on its isolate's thread; nothing here may canonicalize.
//
// The network calls below go through the raw JNIEnv rather than JEnv's
// wrappers: a wrapper converts a pending Java exception into a thrown
// NativeScriptException, which would unwind past the per-stage handling that
// tells a status-bearing answer (an empty 404) apart from a transport failure,
// and past the InputStream close. The wrappers stay on the setup calls, whose
// failures have no per-stage verdict and are caught below.
static bool PerformHttpFetchOnceSync(const std::string& url, const std::string& canonicalKey,
                                     std::string& out, std::string& contentType, int& status,
                                     bool& bustApplied) {
    out.clear();
    contentType.clear();
    status = 0;
    TNS_DEBUG(Esm, "[http-esm][fetch][enter] url=%s", url.c_str());

    const std::string fetchUrl = ApplyCacheBustNonce(url, canonicalKey, &bustApplied);

    auto recordStageFailure = [&url](const char* stage, const std::string& excClass,
                                     const std::string& excMsg) {
        RecordLastHttpFetchError(stage, excClass, excMsg);
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=%s url=%s class=%s msg=%s", stage,
                       url.c_str(), excClass.c_str(), excMsg.c_str());
    };

    try {
        JEnv env;
        JNIEnv* raw = env;
        DisableHttpKeepAliveOnce(env);
        PermitAllStrictMode(env);

        jclass clsURL = env.FindClass("java/net/URL");
        if (!clsURL) return false;
        jmethodID urlCtor = env.GetMethodID(clsURL, "<init>", "(Ljava/lang/String;)V");
        jmethodID openConnection =
                env.GetMethodID(clsURL, "openConnection", "()Ljava/net/URLConnection;");
        jstring jUrlStr = env.NewStringUTF(fetchUrl.c_str());
        jobject urlObj = raw->NewObject(clsURL, urlCtor, jUrlStr);

        {
            std::string excClass, excMsg;
            if (DrainPendingJniException(env, excClass, excMsg)) {
                recordStageFailure("url-ctor", excClass, excMsg);
                return false;
            }
        }

        jobject conn = raw->CallObjectMethod(urlObj, openConnection);
        {
            std::string excClass, excMsg;
            if (DrainPendingJniException(env, excClass, excMsg)) {
                recordStageFailure("open-connection", excClass, excMsg);
                return false;
            }
        }
        if (!conn) return false;

        jclass clsConn = env.GetObjectClass(conn);
        jmethodID setConnectTimeout = env.GetMethodID(clsConn, "setConnectTimeout", "(I)V");
        jmethodID setReadTimeout = env.GetMethodID(clsConn, "setReadTimeout", "(I)V");
        jmethodID setDoInput = env.GetMethodID(clsConn, "setDoInput", "(Z)V");
        jmethodID setUseCaches = env.GetMethodID(clsConn, "setUseCaches", "(Z)V");
        jmethodID setReqProp =
                env.GetMethodID(clsConn, "setRequestProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
        env.CallVoidMethod(conn, setConnectTimeout, 15000);
        env.CallVoidMethod(conn, setReadTimeout, 15000);
        if (setDoInput) {
            env.CallVoidMethod(conn, setDoInput, JNI_TRUE);
        }
        if (setUseCaches) {
            env.CallVoidMethod(conn, setUseCaches, JNI_FALSE);
        }
        env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Accept"),
                           env.NewStringUTF("application/javascript, text/javascript, */*;q=0.1"));
        env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Accept-Encoding"),
                           env.NewStringUTF("identity"));
        env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Cache-Control"),
                           env.NewStringUTF("no-cache, no-store, max-age=0"));
        env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Pragma"),
                           env.NewStringUTF("no-cache"));
        env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Connection"),
                           env.NewStringUTF("close"));
        env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("User-Agent"),
                           env.NewStringUTF("NativeScript-HTTP-ESM"));

        jclass clsHttp = env.FindClass("java/net/HttpURLConnection");
        bool isHttp = clsHttp && env.IsInstanceOf(conn, clsHttp);
        jmethodID getResponseCode =
                isHttp ? env.GetMethodID(clsHttp, "getResponseCode", "()I") : nullptr;
        jmethodID getErrorStream =
                isHttp ? env.GetMethodID(clsHttp, "getErrorStream", "()Ljava/io/InputStream;")
                       : nullptr;
        // Once a status line has been read the server has answered, and every
        // body-side failure below stops being a transport error: an empty 404
        // is an answer, and reporting it as "network error" would both hide
        // the status and earn a pointless retry.
        bool haveStatus = false;
        if (isHttp && getResponseCode) {
            status = raw->CallIntMethod(conn, getResponseCode);
            std::string excClass, excMsg;
            if (DrainPendingJniException(env, excClass, excMsg)) {
                // The return value of a JNI call that threw is undefined.
                status = 0;
                recordStageFailure("get-response-code", excClass, excMsg);
                return false;
            }
            haveStatus = status > 0;
        }

        jmethodID getInputStream =
                env.GetMethodID(clsConn, "getInputStream", "()Ljava/io/InputStream;");
        jobject inStream = nullptr;
        if (isHttp && status >= 400 && getErrorStream) {
            inStream = raw->CallObjectMethod(conn, getErrorStream);
            raw->ExceptionClear();
        }
        if (!inStream) {
            // On an error status with no error body, getInputStream throws
            // FileNotFoundException rather than returning null.
            inStream = raw->CallObjectMethod(conn, getInputStream);
            std::string excClass, excMsg;
            if (DrainPendingJniException(env, excClass, excMsg)) {
                if (!haveStatus) {
                    recordStageFailure("get-input-stream", excClass, excMsg);
                    return false;
                }
                inStream = nullptr;
            }
        }
        if (!inStream && !haveStatus) return false;

        bool readFailed = false;
        if (inStream) {
            jclass clsIS = env.GetObjectClass(inStream);
            jmethodID readMethod = env.GetMethodID(clsIS, "read", "([B)I");
            jmethodID closeIS = env.GetMethodID(clsIS, "close", "()V");

            // The stream holds a socket fd, so nothing between here and the
            // end of this scope — a failed read, or a Java exception escaping
            // one of the checked wrappers — may leave it open.
            struct StreamCloser {
                JNIEnv* jni;
                jobject stream;
                jmethodID closeMethod;
                ~StreamCloser() {
                    if (closeMethod == nullptr) return;
                    jni->CallVoidMethod(stream, closeMethod);
                    jni->ExceptionClear();
                }
            } streamCloser{raw, inStream, closeIS};

            jclass clsBAOS = env.FindClass("java/io/ByteArrayOutputStream");
            jmethodID baosCtor = env.GetMethodID(clsBAOS, "<init>", "()V");
            jmethodID baosWrite = env.GetMethodID(clsBAOS, "write", "([BII)V");
            jmethodID baosToByteArray = env.GetMethodID(clsBAOS, "toByteArray", "()[B");
            jmethodID baosClose = env.GetMethodID(clsBAOS, "close", "()V");
            jobject baos = env.NewObject(clsBAOS, baosCtor);

            jbyteArray buffer = env.NewByteArray(8192);
            std::string excClass, excMsg;
            while (true) {
                jint n = raw->CallIntMethod(inStream, readMethod, buffer);
                if (DrainPendingJniException(env, excClass, excMsg)) {
                    recordStageFailure("read-body", excClass, excMsg);
                    readFailed = true;
                    break;
                }
                if (n < 0) break;
                if (n == 0) continue;
                raw->CallVoidMethod(baos, baosWrite, buffer, 0, n);
                if (DrainPendingJniException(env, excClass, excMsg)) {
                    recordStageFailure("read-body", excClass, excMsg);
                    readFailed = true;
                    break;
                }
            }

            if (!readFailed) {
                jbyteArray bytes =
                        static_cast<jbyteArray>(env.CallObjectMethod(baos, baosToByteArray));
                env.CallVoidMethod(baos, baosClose);
                if (bytes) {
                    jsize len = env.GetArrayLength(bytes);
                    out.resize(static_cast<size_t>(len));
                    if (len > 0) {
                        env.GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte*>(&out[0]));
                    }
                } else {
                    readFailed = true;
                }
            }
        }
        // A truncated read only matters when the body is what the caller
        // needs: on a non-2xx the status alone decides the outcome, so keep
        // the answer rather than turning it into a retryable network error.
        if (readFailed && (!haveStatus || (status >= 200 && status < 300))) {
            return false;
        }

        jmethodID getContentType =
                env.GetMethodID(clsConn, "getContentType", "()Ljava/lang/String;");
        jstring jct = static_cast<jstring>(env.CallObjectMethod(conn, getContentType));
        if (jct) {
            contentType = ArgConverter::jstringToString(jct);
        }

        if (status == 0) status = 200;
        // Keeps TakeLastHttpFetchErrorReason's contract across a recovered
        // retry: a reason belongs to the attempt that failed, not to the fetch.
        ClearLastHttpFetchErrorReason();
        // Pure transport: true means a response arrived. Whether that response
        // is a usable module — status, MIME, emptiness — is
        // ClassifyModuleResponse's call, so both fetch paths answer it the
        // same way.
        return true;
    } catch (NativeScriptException& nse) {
        std::string what = nse.what() ? nse.what() : "";
        if (what.empty()) {
            what = nse.GetErrorMessage();
        }
        RecordLastHttpFetchError("native-script-exception", "tns::NativeScriptException", what);
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=native-script-exception url=%s msg=%s",
                       url.c_str(), what.c_str());
        return false;
    } catch (std::exception& ex) {
        std::string what = ex.what() ? ex.what() : "<unknown>";
        RecordLastHttpFetchError("std-exception", "std::exception", what);
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=std-exception url=%s msg=%s",
                       url.c_str(), what.c_str());
        return false;
    } catch (...) {
        RecordLastHttpFetchError("unknown-cpp-exception", "<unknown>", "<no message available>");
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=unknown-cpp-exception url=%s",
                       url.c_str());
        return false;
    }
}

void FetchModuleBodyAsync(const std::string& url,
                          std::function<void(ModuleFetchResult result)> completion) {
    // Security gate: single point of enforcement, same as HttpFetchModule.
    if (!IsRemoteUrlAllowed(url)) {
        TNS_DEBUG(Esm, "[http-esm][security][blocked] %s", url.c_str());
        ModuleFetchResult blocked;
        blocked.status = 403;
        blocked.failureReason =
                "HTTP import blocked: remote module loading is not allowed for " + url;
        completion(std::move(blocked));
        return;
    }

    // Canonicalize before the hop: the vocabulary belongs to the calling
    // isolate, and the fetch thread below has no isolate to read it from.
    const std::string canonicalKey = CanonicalizeHttpUrlKey(url);

    std::thread([url, canonicalKey, completion = std::move(completion)]() mutable {
        JavaVM* jvm = Runtime::GetJVM();
        bool attachedHere = false;
        if (jvm != nullptr) {
            JNIEnv* raw = nullptr;
            if (jvm->GetEnv(reinterpret_cast<void**>(&raw), JNI_VERSION_1_6) != JNI_OK) {
                if (jvm->AttachCurrentThread(&raw, nullptr) == JNI_OK) {
                    attachedHere = true;
                }
            }
        }
        struct DetachIfAttached {
            JavaVM* jvm;
            bool attached;
            ~DetachIfAttached() {
                if (attached && jvm != nullptr) {
                    jvm->DetachCurrentThread();
                }
            }
        } detachGuard{jvm, attachedHere};

        std::string body;
        std::string contentType;
        int status = 0;
        const auto start = std::chrono::steady_clock::now();
        bool bustApplied = false;
        bool transportOk =
                PerformHttpFetchOnceSync(url, canonicalKey, body, contentType, status, bustApplied);
        if (!transportOk) {
            // Transport error → one retry, the same single-retry policy the
            // sync path applies.
            TNS_DEBUG(Esm, "[http-loader][fetch-async] retrying %s after transport error",
                           url.c_str());
            usleep(120 * 1000);
            transportOk = PerformHttpFetchOnceSync(url, canonicalKey, body, contentType, status,
                                                   bustApplied);
        }

        ModuleFetchResult result;
        ClassifyModuleResponse(url, transportOk, status, contentType, body, result);

        if (result.ok && bustApplied) {
            ClearCacheBustForUrl(canonicalKey);
        }

        if (!result.ok) {
            TNS_DEBUG(Esm, "[http-loader][fetch-async][reject] %s", result.failureReason.c_str());
        } else if (LogCategoryEnabled(LogCategory::Fetch)) {
            const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::steady_clock::now() - start)
                                    .count();
            TNS_DEBUG(Fetch, "[http-loader][fetch][async] %s bytes=%lu ms=%lld", url.c_str(),
                             (unsigned long)result.body.size(), (long long)ms);
        }
        completion(std::move(result));
    }).detach();
}

static void MaybePumpJSThreadDuringBoot() {
    v8::Isolate* isolate = v8::Isolate::TryGetCurrent();
    if (isolate == nullptr) return;
    if (!IsBootEvaluationActive()) return;
    if (isolate->GetData((uint32_t)Runtime::IsolateData::RUNTIME) == nullptr) return;

    isolate->PerformMicrotaskCheckpoint();
    ALooper_pollOnce(0, nullptr, nullptr, nullptr);
    isolate->PerformMicrotaskCheckpoint();
}

static std::atomic<void (*)()> g_httpFetchYield{&MaybePumpJSThreadDuringBoot};

void RegisterHttpFetchYield(void (*callback)()) {
    g_httpFetchYield.store(callback, std::memory_order_release);
}

static inline void InvokeHttpFetchYield() {
    auto cb = g_httpFetchYield.load(std::memory_order_acquire);
    if (cb != nullptr) cb();
}

void CleanupHttpLoaderGlobals() {
    // The boot-evaluation flag is thread-local and RAII-balanced by
    // ModuleInternal::Load, so it needs no reset here.
    ClearAllCacheBustMarks();
}

// ─────────────────────────────────────────────────────────────
// ns:module binding

namespace {

void InstallDevFunction(v8::Isolate* isolate, v8::Local<v8::Context> context,
                        v8::Local<v8::Object> target, const char* name,
                        v8::FunctionCallback callback) {
    v8::Local<v8::FunctionTemplate> fnTpl = v8::FunctionTemplate::New(isolate, callback);
    v8::Local<v8::Function> fn = fnTpl->GetFunction(context).ToLocalChecked();
    fn->SetName(ToV8String(isolate, name));
    target->CreateDataProperty(context, ToV8String(isolate, name), fn).Check();
}

// The only sections configureLoader understands. An unlisted key is a typo the
// caller hears about rather than a setting that silently does nothing.
constexpr const char* kLoaderConfigKeys[] = {"importMap", "volatilePatterns", "canonicalization"};

void ConfigureLoaderCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

    auto throwTypeError = [&](const std::string& message) {
        isolate->ThrowException(v8::Exception::TypeError(ToV8String(isolate, message)));
    };

    if (info.Length() < 1 || !info[0]->IsObject()) {
        throwTypeError("configureLoader expects a config object");
        return;
    }

    v8::Local<v8::Object> config = info[0].As<v8::Object>();

    // ── Validation phase ─────────────────────────────────────────────────
    // Nothing below mutates the vocabulary. The whole config is checked first
    // so a rejected call leaves every section exactly as it was — the
    // atomicity the import map alone used to have, now covering the entire
    // call.

    // Unknown top-level keys.
    v8::Local<v8::Array> configKeys;
    if (!config->GetOwnPropertyNames(ctx, v8::PropertyFilter::ONLY_ENUMERABLE,
                                     v8::KeyConversionMode::kConvertToString)
                 .ToLocal(&configKeys)) {
        return;  // pending exception
    }
    for (uint32_t i = 0; i < configKeys->Length(); i++) {
        v8::Local<v8::Value> keyVal;
        if (!configKeys->Get(ctx, i).ToLocal(&keyVal)) {
            return;
        }
        std::string key = ArgConverter::ToString(isolate, keyVal);
        bool known = false;
        for (const char* candidate : kLoaderConfigKeys) {
            if (key == candidate) {
                known = true;
                break;
            }
        }
        if (!known) {
            throwTypeError("configureLoader: unknown option '" + key + "'");
            return;
        }
    }

    // Reads `obj[key]` as an array of strings into `out`. `label` names the
    // section in any error. Returns false with an exception pending on a type
    // failure; `present` distinguishes "absent" from "present and valid".
    auto readStringArray = [&](v8::Local<v8::Object> obj, const char* key,
                               const std::string& label, std::vector<std::string>& out,
                               bool* present) -> bool {
        *present = false;
        v8::Local<v8::Value> val;
        if (!obj->Get(ctx, ToV8String(isolate, key)).ToLocal(&val)) {
            return false;
        }
        if (val->IsUndefined()) {
            return true;
        }
        if (!val->IsArray()) {
            throwTypeError("configureLoader: " + label + " must be an array of strings");
            return false;
        }
        v8::Local<v8::Array> arr = val.As<v8::Array>();
        for (uint32_t i = 0; i < arr->Length(); i++) {
            v8::Local<v8::Value> elem;
            if (!arr->Get(ctx, i).ToLocal(&elem)) {
                return false;
            }
            if (!elem->IsString()) {
                throwTypeError("configureLoader: " + label + "[" + std::to_string(i) +
                               "] must be a string");
                return false;
            }
            out.push_back(ArgConverter::ToString(isolate, elem));
        }
        *present = true;
        return true;
    };

    // importMap: an object or a JSON string. Validated here, installed below.
    std::string importMapJson;
    bool haveImportMap = false;
    v8::Local<v8::Value> importMapVal;
    if (!config->Get(ctx, ToV8String(isolate, "importMap")).ToLocal(&importMapVal)) {
        return;
    }
    if (!importMapVal->IsUndefined()) {
        std::string jsonStr;
        if (importMapVal->IsString()) {
            jsonStr = ArgConverter::ToString(isolate, importMapVal);
        } else if (importMapVal->IsObject()) {
            v8::Local<v8::String> stringified;
            if (!v8::JSON::Stringify(ctx, importMapVal).ToLocal(&stringified)) {
                return;  // a throwing toJSON / getter propagates unchanged
            }
            // JSON.stringify answers `undefined` for a function or a
            // symbol-valued object, which is not a JSON string.
            if (stringified->IsString()) {
                jsonStr = ArgConverter::ToString(isolate, stringified);
            }
        }
        if (jsonStr.empty()) {
            throwTypeError("configureLoader: importMap must be an object or a JSON string");
            return;
        }
        std::string importMapError;
        if (!ValidateImportMapJson(jsonStr, &importMapError)) {
            // The previous map is still installed: a rejected update changes
            // nothing, so a typo cannot empty a live session's vocabulary.
            throwTypeError("configureLoader: " + importMapError);
            return;
        }
        importMapJson = std::move(jsonStr);
        haveImportMap = true;
    }

    // volatilePatterns: array of strings. Presence of the array decides, not
    // its contents — an empty one is explicit policy meaning "nothing is
    // volatile any more", the same rule canonicalization follows, and the only
    // reading under which a present section replaces its state wholesale.
    std::vector<std::string> patterns;
    bool havePatterns = false;
    if (!readStringArray(config, "volatilePatterns", "volatilePatterns", patterns, &havePatterns)) {
        return;
    }

    // canonicalization: { stripParams, forPathPrefixes, preserveQueryFor } —
    // the URL vocabulary CanonicalizeHttpUrlKey applies (see its doc block).
    // Presence of the object marks the vocabulary as configured, replacing the
    // built-in fallback entirely (empty arrays are honored as explicit policy).
    CanonicalizationConfig canon;
    bool haveCanon = false;
    v8::Local<v8::Value> canonVal;
    if (!config->Get(ctx, ToV8String(isolate, "canonicalization")).ToLocal(&canonVal)) {
        return;
    }
    if (!canonVal->IsUndefined()) {
        if (!canonVal->IsObject()) {
            throwTypeError("configureLoader: canonicalization must be an object");
            return;
        }
        v8::Local<v8::Object> canonObj = canonVal.As<v8::Object>();
        bool ignored = false;
        if (!readStringArray(canonObj, "stripParams", "canonicalization.stripParams",
                             canon.stripParams, &ignored) ||
            !readStringArray(canonObj, "forPathPrefixes", "canonicalization.forPathPrefixes",
                             canon.devPathPrefixes, &ignored) ||
            !readStringArray(canonObj, "preserveQueryFor", "canonicalization.preserveQueryFor",
                             canon.preserveQueryPrefixes, &ignored)) {
            return;
        }
        haveCanon = true;
    }

    // ── Apply phase ──────────────────────────────────────────────────────
    // Everything validated; from here nothing can fail on the caller's input.

    if (haveImportMap) {
        // The re-parse inside SetImportMap is deterministic and already
        // succeeded above, so the only failure left is the isolate shutting
        // down.
        std::string installError;
        if (!SetImportMap(importMapJson, &installError)) {
            throwTypeError("configureLoader: " + installError);
            return;
        }
        TNS_DEBUG(Esm, "[ns:module configureLoader] import map set (%zu bytes)",
                       importMapJson.size());
    }

    if (havePatterns) {
        SetVolatilePatterns(patterns);
        TNS_DEBUG(Esm, "[ns:module configureLoader] %zu volatile patterns set", patterns.size());
    }

    if (haveCanon) {
        SetCanonicalizationConfig(std::move(canon));
    }
}

void InvalidateModulesCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

    if (info.Length() < 1 || !info[0]->IsArray()) {
        isolate->ThrowException(v8::Exception::TypeError(
                ToV8String(isolate, "invalidateModules expects an array of URL strings")));
        return;
    }

    v8::Local<v8::Array> urlsArray = info[0].As<v8::Array>();
    std::vector<std::string> urls;
    urls.reserve(urlsArray->Length());
    for (uint32_t index = 0; index < urlsArray->Length(); index++) {
        v8::Local<v8::Value> value;
        if (!urlsArray->Get(ctx, index).ToLocal(&value)) {
            return;
        }
        if (!value->IsString()) {
            isolate->ThrowException(v8::Exception::TypeError(ToV8String(
                    isolate,
                    "invalidateModules: urls[" + std::to_string(index) + "] must be a string")));
            return;
        }
        urls.push_back(ArgConverter::ToString(isolate, value));
    }

    if (tns::LogCategoryEnabled(tns::LogCategory::Registry)) {
        TNS_DEBUG(Registry, "invalidate called urls.count=%zu", urls.size());
        size_t shown = 0;
        for (const auto& u : urls) {
            if (shown >= 32) break;
            TNS_DEBUG(Registry, "invalidate url[%zu]=%s", shown, u.c_str());
            shown++;
        }
        if (urls.size() > shown) {
            TNS_DEBUG(Registry, "invalidate (hidden %zu more URL(s))", urls.size() - shown);
        }
    }

    tns::InvalidateModules(isolate, ctx, urls);
}

void GetLoadedModuleUrlsCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

    std::vector<std::string> urls = tns::GetLoadedModuleUrls();
    v8::Local<v8::Array> result = v8::Array::New(isolate, static_cast<int>(urls.size()));

    for (uint32_t index = 0; index < urls.size(); index++) {
        result->Set(ctx, index, ToV8String(isolate, urls[index])).FromMaybe(false);
    }

    info.GetReturnValue().Set(result);
}

}  // namespace

bool BuildNsModuleBinding(v8::Local<v8::Context> context, v8::Local<v8::Object> binding) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    InstallDevFunction(isolate, context, binding, "configureLoader", ConfigureLoaderCallback);
    InstallDevFunction(isolate, context, binding, "invalidateModules", InvalidateModulesCallback);
    InstallDevFunction(isolate, context, binding, "getLoadedModuleUrls",
                       GetLoadedModuleUrlsCallback);

    if (!ModuleInternal::InstallCreateRequireBinding(context, binding)) {
        return false;
    }

    if (IsDebuggable()) {
        auto canonicalizeCb = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            v8::Isolate* iso = info.GetIsolate();
            if (info.Length() < 1 || !info[0]->IsString()) {
                iso->ThrowException(v8::Exception::TypeError(
                        ToV8String(iso, "canonicalizeHttpUrlKey expects a URL string")));
                return;
            }
            std::string key = CanonicalizeHttpUrlKey(ArgConverter::ToString(iso, info[0]));
            info.GetReturnValue().Set(ToV8String(iso, key));
        };
        v8::Local<v8::Function> fn;
        if (v8::Function::New(context, canonicalizeCb).ToLocal(&fn)) {
            fn->SetName(ToV8String(isolate, "canonicalizeHttpUrlKey"));
            if (!binding
                         ->CreateDataProperty(context, ToV8String(isolate, "canonicalizeHttpUrlKey"),
                                              fn)
                         .FromMaybe(false)) {
                return false;
            }
        }
    }

    return true;
}

}  // namespace tns

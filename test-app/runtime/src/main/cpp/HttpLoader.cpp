#include "HttpLoader.h"

#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstring>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "ArgConverter.h"
#include "JEnv.h"
// The transport carries canonical keys rather than computing them, but
// CanonicalizeHttpUrlKey itself reads the calling isolate's canonicalization
// vocabulary — the one loader dependency left here.
#include "ModuleInternalCallbacks.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "TraceLog.h"
#include "robin_hood.h"

namespace tns {

static inline bool StartsWith(const std::string& s, const char* prefix) {
    size_t n = strlen(prefix);
    return s.size() >= n && s.compare(0, n, prefix) == 0;
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
// Canonical module keys

std::string NormalizeHttpModuleUrl(const std::string& path) {
    if (path.empty()) {
        return path;
    }

    std::string normalized = path;
    if (StartsWith(normalized, "file://http://") || StartsWith(normalized, "file://https://")) {
        normalized = normalized.substr(strlen("file://"));
    }

    // A path normalizer that collapses `//` into `/` (Java's, or a URL that
    // travelled through one) leaves the scheme separator one slash short.
    if (normalized.rfind("http:/", 0) == 0 && normalized.rfind("http://", 0) != 0) {
        normalized.insert(5, "/");
    } else if (normalized.rfind("https:/", 0) == 0 && normalized.rfind("https://", 0) != 0) {
        normalized.insert(6, "/");
    }

    return normalized;
}

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

void MarkKeysForCacheBust(const std::vector<std::string>& canonicalKeys) {
    if (canonicalKeys.empty()) return;
    std::lock_guard<std::mutex> lock(g_bustNextFetchMutex);
    for (const auto& key : canonicalKeys) {
        if (key.empty()) continue;
        if (!(StartsWith(key, "http://") || StartsWith(key, "https://"))) continue;
        g_bustNextFetchKeys.insert(key);
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

// A blocking network call on the JS thread is exactly what StrictMode is meant
// to flag, so the fetch relaxes the thread policy — but the policy belongs to
// the app, not to this request: whatever runs next on this thread must get its
// own policy back, including when the fetch leaves through an exception.
// Restoring uses the raw JNIEnv (JEnv's wrappers throw, and a destructor may
// run mid-unwind).
struct StrictModeScope {
    JNIEnv* jni = nullptr;
    jclass clsStrict = nullptr;
    jmethodID setThreadPolicy = nullptr;
    jobject savedPolicy = nullptr;

    explicit StrictModeScope(JEnv& env) {
        // Every bail must clear: FindClass/GetMethodID return null WITH an
        // exception pending, and leaving one armed makes the caller's next
        // JNI call illegal.
        JNIEnv* raw = env;
        clsStrict = env.FindClass("android/os/StrictMode");
        jclass clsPolicyBuilder = env.FindClass("android/os/StrictMode$ThreadPolicy$Builder");
        if (!clsStrict || !clsPolicyBuilder) {
            raw->ExceptionClear();
            return;
        }
        jmethodID getThreadPolicy = env.GetStaticMethodID(
                clsStrict, "getThreadPolicy", "()Landroid/os/StrictMode$ThreadPolicy;");
        jmethodID setter = env.GetStaticMethodID(
                clsStrict, "setThreadPolicy", "(Landroid/os/StrictMode$ThreadPolicy;)V");
        if (!getThreadPolicy || !setter) {
            raw->ExceptionClear();
            return;
        }
        // No captured policy means no way back, so leave the thread alone
        // rather than relaxing it permanently.
        jobject captured = env.CallStaticObjectMethod(clsStrict, getThreadPolicy);
        if (!captured) {
            raw->ExceptionClear();
            return;
        }

        jmethodID builderCtor = env.GetMethodID(clsPolicyBuilder, "<init>", "()V");
        jobject builder = builderCtor ? env.NewObject(clsPolicyBuilder, builderCtor) : nullptr;
        if (!builder) {
            raw->ExceptionClear();
            return;
        }
        jmethodID permitAll = env.GetMethodID(clsPolicyBuilder, "permitAll",
                                              "()Landroid/os/StrictMode$ThreadPolicy$Builder;");
        jobject builder2 = permitAll ? env.CallObjectMethod(builder, permitAll) : builder;
        jmethodID build = env.GetMethodID(clsPolicyBuilder, "build",
                                          "()Landroid/os/StrictMode$ThreadPolicy;");
        jobject policy = build ? env.CallObjectMethod(builder2 ? builder2 : builder, build)
                               : nullptr;
        if (!policy) {
            raw->ExceptionClear();
            return;
        }
        env.CallStaticVoidMethod(clsStrict, setter, policy);
        // Armed only once the permissive policy is actually in force. The
        // saved policy is a global ref so the restore does not depend on any
        // JNI local frame the caller pushed around this scope.
        setThreadPolicy = setter;
        savedPolicy = env.NewGlobalRef(captured);
        jni = env;
    }

    StrictModeScope(const StrictModeScope&) = delete;
    StrictModeScope& operator=(const StrictModeScope&) = delete;

    ~StrictModeScope() {
        if (jni == nullptr) return;
        jni->CallStaticVoidMethod(clsStrict, setThreadPolicy, savedPolicy);
        jni->DeleteGlobalRef(savedPolicy);
        jni->ExceptionClear();
    }
};

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

bool HttpFetchModule(const std::string& url, const std::string& canonicalKey,
                     ModuleFetchResult& result) {
    result = ModuleFetchResult();

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
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=%s url=%s class=%s msg=%s", stage,
                       url.c_str(), excClass.c_str(), excMsg.c_str());
    };

    JEnv env;
    JNIEnv* raw = env;

    // Request setup alone burns a couple of dozen local refs (a jstring per
    // header, one per drained exception), and the sync path runs inside a
    // caller's frame — V8's resolve walk — that must not be left holding
    // them. Declared OUTSIDE the try: a caught NativeScriptException may hold
    // the Java throwable as a local ref in this frame, which the handlers
    // below read — the pop must come after them, not during the unwind.
    const bool framePushed = raw->PushLocalFrame(64) == JNI_OK;
    if (!framePushed) {
        raw->ExceptionClear();
    }
    struct LocalFrame {
        JNIEnv* jni;
        bool pushed;
        ~LocalFrame() {
            if (pushed) jni->PopLocalFrame(nullptr);
        }
    } localFrame{raw, framePushed};

    try {
        StrictModeScope strictMode(env);

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
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=native-script-exception url=%s msg=%s",
                       url.c_str(), what.c_str());
        return false;
    } catch (std::exception& ex) {
        std::string what = ex.what() ? ex.what() : "<unknown>";
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=std-exception url=%s msg=%s",
                       url.c_str(), what.c_str());
        return false;
    } catch (...) {
        TNS_DEBUG(Esm, "[http-esm][fetch][exception] stage=unknown-cpp-exception url=%s",
                       url.c_str());
        return false;
    }
}

// ── Bounded async fetch ──────────────────────────────────────
//
// One module fetch, with everything the fetch thread needs; nothing here
// touches V8 or an isolate.
namespace {

struct ModuleFetchJob {
    std::string url;
    std::string canonicalKey;
    std::function<void(ModuleFetchResult)> completion;
};

// See the cap's rationale in HttpLoader.h. 16 matches iOS's
// HTTPMaximumConnectionsPerHost.
constexpr size_t kMaxConcurrentModuleFetches = 16;

std::mutex g_fetchQueueMutex;
std::deque<ModuleFetchJob> g_fetchQueue;
size_t g_fetchThreadCount = 0;

void RunModuleFetchJob(const ModuleFetchJob& job) {
    std::string body;
    std::string contentType;
    int status = 0;
    const auto start = std::chrono::steady_clock::now();
    bool bustApplied = false;
    bool transportOk = PerformHttpFetchOnceSync(job.url, job.canonicalKey, body, contentType,
                                                status, bustApplied);
    if (!transportOk) {
        // Transport error → one retry, the same single-retry policy the
        // sync path applies.
        TNS_DEBUG(Esm, "[http-loader][fetch-async] retrying %s after transport error",
                       job.url.c_str());
        usleep(120 * 1000);
        transportOk = PerformHttpFetchOnceSync(job.url, job.canonicalKey, body, contentType, status,
                                               bustApplied);
    }

    ModuleFetchResult result;
    ClassifyModuleResponse(job.url, transportOk, status, contentType, body, result);

    if (result.ok && bustApplied) {
        ClearCacheBustForUrl(job.canonicalKey);
    }

    if (!result.ok) {
        TNS_DEBUG(Esm, "[http-loader][fetch-async][reject] %s", result.failureReason.c_str());
    } else if (LogCategoryEnabled(LogCategory::Fetch)) {
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - start)
                                .count();
        TNS_DEBUG(Fetch, "[http-loader][fetch][async] %s bytes=%lu ms=%lld", job.url.c_str(),
                         (unsigned long)result.body.size(), (long long)ms);
    }
    job.completion(std::move(result));
}

// A fetch thread serves its own job and then drains the queue, so the JVM
// attach is paid once per thread rather than once per module and the cap
// bounds threads, not just sockets. Exiting checks the queue and decrements
// the count in ONE critical section: a producer that queues therefore always
// observes — and is observed by — a thread that has not yet exited, so a
// queued job cannot be stranded behind a thread on its way out.
void ModuleFetchThreadMain(ModuleFetchJob job) {
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

    for (;;) {
        RunModuleFetchJob(job);

        std::lock_guard<std::mutex> lock(g_fetchQueueMutex);
        if (g_fetchQueue.empty()) {
            g_fetchThreadCount--;
            return;
        }
        job = std::move(g_fetchQueue.front());
        g_fetchQueue.pop_front();
    }
}

}  // namespace

void FetchModuleBodyAsync(const std::string& url, const std::string& canonicalKey,
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

    ModuleFetchJob job{url, canonicalKey, std::move(completion)};
    {
        std::lock_guard<std::mutex> lock(g_fetchQueueMutex);
        if (g_fetchThreadCount >= kMaxConcurrentModuleFetches) {
            g_fetchQueue.push_back(std::move(job));
            return;
        }
        g_fetchThreadCount++;
    }

    // Copied, not moved: a throwing std::thread constructor leaves its
    // decay-copies in an unspecified state, and the failure path below still
    // has to deliver this job's completion.
    try {
        std::thread(ModuleFetchThreadMain, job).detach();
        return;
    } catch (...) {
    }

    // A counted slot with no thread behind it: anything queued against it has
    // nothing left to drain it if this was the last one, so those jobs are
    // failed here rather than left waiting on a thread that will never exist.
    std::deque<ModuleFetchJob> orphaned;
    {
        std::lock_guard<std::mutex> lock(g_fetchQueueMutex);
        g_fetchThreadCount--;
        if (g_fetchThreadCount == 0) {
            orphaned.swap(g_fetchQueue);
        }
    }

    // The completion contract is exactly-once, and the caller's bookkeeping
    // (pendingFetches) is already incremented — a swallowed spawn failure
    // wedges the graph walk short of settling forever.
    auto failSpawn = [](const ModuleFetchJob& failedJob) {
        ModuleFetchResult failed;
        failed.failureReason = "Could not fetch " + failedJob.url +
                               ": the runtime could not start a fetch thread";
        TNS_DEBUG(Esm, "[http-loader][fetch-async][spawn-fail] %s", failedJob.url.c_str());
        failedJob.completion(std::move(failed));
    };

    failSpawn(job);
    for (const ModuleFetchJob& orphan : orphaned) {
        failSpawn(orphan);
    }
}

void CleanupHttpLoaderGlobals() {
    ClearAllCacheBustMarks();
}

}  // namespace tns

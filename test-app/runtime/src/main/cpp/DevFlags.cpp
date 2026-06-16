// DevFlags.cpp
#include "DevFlags.h"
#include "JEnv.h"
#include "NativeScriptAssert.h"
#include <atomic>
#include <mutex>
#include <vector>
#include <string>

namespace tns {

// Cache the result of a parameterless `static boolean` method on
// `com.tns.Runtime`. `cached` and `initFlag` are caller-owned statics so
// every flag stays independently memoized; the helper does the JNI dance
// once and pins the result. Returns false (the safe default) if the
// class or method cannot be resolved.
static bool CachedBoolFlagFromJava(std::atomic<int>& cached,
                                   std::once_flag& initFlag,
                                   const char* javaStaticBoolMethod) {
  int v = cached.load(std::memory_order_acquire);
  if (v != -1) {
    return v == 1;
  }
  std::call_once(initFlag, [&]() {
    bool enabled = false;
    try {
      JEnv env;
      jclass runtimeClass = env.FindClass("com/tns/Runtime");
      if (runtimeClass != nullptr) {
        jmethodID mid = env.GetStaticMethodID(runtimeClass, javaStaticBoolMethod, "()Z");
        if (mid != nullptr) {
          jboolean res = env.CallStaticBooleanMethod(runtimeClass, mid);
          enabled = (res == JNI_TRUE);
        }
      }
    } catch (...) {
      // keep default false
    }
    cached.store(enabled ? 1 : 0, std::memory_order_release);
  });
  return cached.load(std::memory_order_acquire) == 1;
}

bool IsScriptLoadingLogEnabled() {
  static std::atomic<int> cached{-1};
  static std::once_flag initFlag;
  return CachedBoolFlagFromJava(cached, initFlag, "getLogScriptLoadingEnabled");
}

// HTTP module loader flags
//
// Reads `httpModulePrefetch` from app config (default: DISABLED).
//
// Apps that want to opt in for testing can set in package.json:
//
//   {
//     "httpModulePrefetch": true
//   }
//
// Returning false here short-circuits both the speculative-prefetch cache
// lookup (in HttpFetchText) and the prefetch wave (in KickstartHmrPrefetchSync /
// KickstartHmrPrefetchUrlsSync), restoring the pre-prefetcher behavior
// bit-for-bit. This is layered on top of the IsRemoteUrlAllowed network gate.
bool IsHttpModulePrefetchEnabled() {
  static std::atomic<int> cached{-1};
  static std::once_flag initFlag;
  bool enabled = CachedBoolFlagFromJava(cached, initFlag, "getHttpModulePrefetchEnabled");

  // Startup banner. Gated on the logScriptLoading flag so it stays silent
  // by default — flip the flag in package.json when diagnosing why
  // prefetch is or isn't engaging.
  //   [http-loader] prefetch=disabled   ← expected default
  //   [http-loader] prefetch=enabled    ← only if config opt-in
  static std::once_flag bannerFlag;
  std::call_once(bannerFlag, [enabled]() {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-loader] prefetch=%s shared-session=on hmr-kickstart=on",
                  enabled ? "enabled" : "disabled");
    }
  });
  return enabled;
}

// Default OFF because the volume is high (one line per fetch, hundreds per
// cold boot, hundreds per HMR refresh). Opt in via package.json:
//   { "httpFetchUrlLog": true }
bool IsHttpFetchUrlLogEnabled() {
  static std::atomic<int> cached{-1};
  static std::once_flag initFlag;
  bool enabled = CachedBoolFlagFromJava(cached, initFlag, "getHttpFetchUrlLogEnabled");

  static std::once_flag bannerFlag;
  std::call_once(bannerFlag, [enabled]() {
    if (IsScriptLoadingLogEnabled()) {
      DEBUG_WRITE("[http-loader] fetch-url-log=%s",
                  enabled ? "enabled" : "disabled");
    }
  });
  return enabled;
}

// Security config

static std::once_flag s_securityConfigInitFlag;
static bool s_allowRemoteModules = false;
static std::vector<std::string> s_remoteModuleAllowlist;
static bool s_isDebuggable = false;

// Returns true when `url` is covered by allowlist `entry`, matching only on
// URL component boundaries so lookalike-host and lookalike-port values are
// refused: an entry of "https://cdn.example.com" does NOT authorize
// "https://cdn.example.com.attacker.com/x.js" or
// "https://cdn.example.com:9999/x.js". The character after the matched entry
// text must be a path/query/fragment boundary ('/', '?', '#'), or the URL must
// end exactly at the entry, or the entry must already end in '/'.
//
// Deny-by-default: an entry without an explicit port does not match a URL
// that adds one. To allow a specific port, include it in the entry.
static bool RemoteUrlMatchesAllowlistEntry(const std::string& url,
                                           const std::string& entry) {
  if (entry.empty()) return false;
  if (url.size() < entry.size()) return false;
  if (url.compare(0, entry.size(), entry) != 0) return false;
  if (url.size() == entry.size()) return true;   // exact match
  if (entry.back() == '/') return true;           // entry ended at a boundary
  const char next = url[entry.size()];
  return next == '/' || next == '?' || next == '#';
}

void InitializeSecurityConfig() {
  std::call_once(s_securityConfigInitFlag, []() {
    try {
      JEnv env;
      jclass runtimeClass = env.FindClass("com/tns/Runtime");
      if (runtimeClass == nullptr) {
        return;
      }
      
      // Check isDebuggable first
      jmethodID isDebuggableMid = env.GetStaticMethodID(runtimeClass, "isDebuggable", "()Z");
      if (isDebuggableMid != nullptr) {
        jboolean res = env.CallStaticBooleanMethod(runtimeClass, isDebuggableMid);
        s_isDebuggable = (res == JNI_TRUE);
      }
      
      // If debuggable, we don't need to check further - always allow
      if (s_isDebuggable) {
        s_allowRemoteModules = true;
        return;
      }
      
      // Check isRemoteModulesAllowed
      jmethodID allowRemoteMid = env.GetStaticMethodID(runtimeClass, "isRemoteModulesAllowed", "()Z");
      if (allowRemoteMid != nullptr) {
        jboolean res = env.CallStaticBooleanMethod(runtimeClass, allowRemoteMid);
        s_allowRemoteModules = (res == JNI_TRUE);
      }
      
      // Get the allowlist
      jmethodID getAllowlistMid = env.GetStaticMethodID(runtimeClass, "getRemoteModuleAllowlist", "()[Ljava/lang/String;");
      if (getAllowlistMid != nullptr) {
        jobjectArray allowlistArray = (jobjectArray)env.CallStaticObjectMethod(runtimeClass, getAllowlistMid);
        if (allowlistArray != nullptr) {
          jsize len = env.GetArrayLength(allowlistArray);
          for (jsize i = 0; i < len; i++) {
            jstring jstr = (jstring)env.GetObjectArrayElement(allowlistArray, i);
            if (jstr != nullptr) {
              const char* str = env.GetStringUTFChars(jstr, nullptr);
              if (str != nullptr) {
                s_remoteModuleAllowlist.push_back(std::string(str));
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

bool IsRemoteModulesAllowed() {
  InitializeSecurityConfig();
  return s_allowRemoteModules || s_isDebuggable;
}

bool IsDebuggable() {
  InitializeSecurityConfig();
  return s_isDebuggable;
}

bool IsRemoteUrlAllowed(const std::string& url) {
  InitializeSecurityConfig();
  
  // Debug mode always allows all URLs
  if (s_isDebuggable) {
    return true;
  }
  
  // Production: first check if remote modules are allowed at all
  if (!s_allowRemoteModules) {
    return false;
  }
  
  // If no allowlist is configured, allow all URLs (user explicitly enabled remote modules)
  if (s_remoteModuleAllowlist.empty()) {
    return true;
  }
  
  // Check if URL matches any allowlist entry on a component boundary
  for (const std::string& entry : s_remoteModuleAllowlist) {
    if (RemoteUrlMatchesAllowlistEntry(url, entry)) {
      return true;
    }
  }
  
  return false;
}

} // namespace tns

// DevFlags.cpp
#include "DevFlags.h"
#include "JEnv.h"
#include <atomic>
#include <mutex>
#include <vector>
#include <string>

namespace tns {

bool IsScriptLoadingLogEnabled() {
  static std::atomic<int> cached{-1}; // -1 unknown, 0 false, 1 true
  int v = cached.load(std::memory_order_acquire);
  if (v != -1) {
    return v == 1;
  }

  static std::once_flag initFlag;
  std::call_once(initFlag, []() {
    bool enabled = false;
    try {
      JEnv env;
      jclass runtimeClass = env.FindClass("com/tns/Runtime");
      if (runtimeClass != nullptr) {
        jmethodID mid = env.GetStaticMethodID(runtimeClass, "getLogScriptLoadingEnabled", "()Z");
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

// Security config

static std::once_flag s_securityConfigInitFlag;
static bool s_allowRemoteModules = false;
static std::vector<std::string> s_remoteModuleAllowlist;
static bool s_isDebuggable = false;

// Helper to check if a URL starts with a given prefix
static bool UrlStartsWith(const std::string& url, const std::string& prefix) {
  if (prefix.size() > url.size()) return false;
  return url.compare(0, prefix.size(), prefix) == 0;
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
  
  // Check if URL matches any allowlist prefix
  for (const std::string& prefix : s_remoteModuleAllowlist) {
    if (UrlStartsWith(url, prefix)) {
      return true;
    }
  }
  
  return false;
}

} // namespace tns

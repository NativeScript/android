// DevFlags.cpp
#include "DevFlags.h"
#include "JEnv.h"
#include <atomic>
#include <mutex>

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

} // namespace tns

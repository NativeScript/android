#ifndef WRAPPERLIFETIME_H_
#define WRAPPERLIFETIME_H_

#include <cstdint>
#include <string>

namespace tns {

// How the JS wrappers of Java objects live. Process-wide: read from the app
// config "wrapperLifetime": { "mode": ..., "scope": ... } by the first runtime
// that starts, before any wrapper exists, and fixed for the process.
enum class WrapperLifetimeMode {
  // kFinalizer callbacks keep a collected wrapper while Java may still hold
  // its object: the Java side is made weak, and the wrapper is released only
  // once a later collection finds the Java object gone (ObjectManager).
  kResurrecting,
  // Phantom callbacks, no resurrection: every wrapper is a canary that dies
  // with its JS references. The JS properties of one in scope are mirrored
  // into a state object that lives until Java collects the object, and a
  // canary built later for the same object gets them back
  // (WrapperLifetimeCanary).
  kCanary,
};

enum class WrapperLifetimeScope {
  // The JS properties kept are those of instances Java can call into JS
  // through: JS-extended classes and interface implementations. Under
  // kCanary, other wrappers are canaries without state, and a wrapper built
  // after one died starts empty.
  kExtended,
  kAll,
};

namespace detail {
inline WrapperLifetimeMode wrapperLifetimeMode = WrapperLifetimeMode::kResurrecting;
inline WrapperLifetimeScope wrapperLifetimeScope = WrapperLifetimeScope::kExtended;
}  // namespace detail

inline WrapperLifetimeMode GetWrapperLifetimeMode() {
  return detail::wrapperLifetimeMode;
}
inline WrapperLifetimeScope GetWrapperLifetimeScope() {
  return detail::wrapperLifetimeScope;
}

const char* WrapperLifetimeModeName(WrapperLifetimeMode mode);
const char* WrapperLifetimeScopeName(WrapperLifetimeScope scope);

// com.tns.AppConfig validated both names. Only the first call takes effect.
void ConfigureWrapperLifetime(const std::string& mode, const std::string& scope);

// Per-runtime counters (ObjectManager), returned by ns:runtime's
// getWrapperLifetimeStats(). Touched only under the isolate's Locker.
struct WrapperLifetimeStats {
  // Finalizer callbacks that kept a collected wrapper until the Java side
  // says whether its object is still alive.
  uint64_t resurrections = 0;
  // Java objects held weakly because their wrapper was collected.
  uint64_t weakened = 0;
  // Wrappers released because Java had collected their object.
  uint64_t collected = 0;
  // Links released through __releaseNativeCounterpart.
  uint64_t releasedExplicitly = 0;
  // Times JS used a wrapper whose Java object was already collected, or
  // called a method on one whose link was released.
  uint64_t releasedAccesses = 0;
  // "canary" only: state objects created to hold JS properties.
  uint64_t canaryStates = 0;
  // "canary" only: state objects let go of because Java collected the object.
  uint64_t canaryNativeDeaths = 0;
  // "canary" only: writes to canaries the write handler saw.
  uint64_t canaryInterceptedWrites = 0;
  // "canary" only: of those, writes mirrored through the state name cache.
  uint64_t canaryCachedWrites = 0;
  // "canary" only: canaries built anew for a JS-extended instance whose
  // previous canary died while Java kept the object.
  uint64_t canaryRebuilds = 0;
};

}  // namespace tns

#endif /* WRAPPERLIFETIME_H_ */

#include "WrapperLifetime.h"

#include <mutex>

namespace tns {

const char* WrapperLifetimeModeName(WrapperLifetimeMode mode) {
  switch (mode) {
    case WrapperLifetimeMode::kResurrecting:
      return "resurrecting";
  }
  return "resurrecting";
}

const char* WrapperLifetimeScopeName(WrapperLifetimeScope scope) {
  return scope == WrapperLifetimeScope::kAll ? "all" : "extended";
}

void ConfigureWrapperLifetime(const std::string& mode, const std::string& scope) {
  static std::once_flag once;
  std::call_once(once, [&]() {
    if (mode == WrapperLifetimeModeName(WrapperLifetimeMode::kResurrecting)) {
      detail::wrapperLifetimeMode = WrapperLifetimeMode::kResurrecting;
    }
    detail::wrapperLifetimeScope =
        scope == "all" ? WrapperLifetimeScope::kAll : WrapperLifetimeScope::kExtended;
  });
}

}  // namespace tns

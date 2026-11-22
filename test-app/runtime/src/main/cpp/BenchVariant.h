#ifndef BENCHVARIANT_H_
#define BENCHVARIANT_H_

// Run-time variant switch for benchmark builds (tools/perf/README.md).
//
// Configured with -PbenchVariants (CMake NS_BENCH_VARIANTS=ON), the runtime
// reads the system property debug.ns.benchvariant once, when the library
// loads, so one APK can carry every label of an A/B as
// `if (BenchVariant() == N)` branches. Without the option BenchVariant() is a
// constant 0: the branches fold away and the property is never read.

#ifdef NS_BENCH_VARIANTS
#include <sys/system_properties.h>

#include <cstdlib>
#endif

namespace tns {

#ifdef NS_BENCH_VARIANTS

inline int g_benchVariant = 0;

inline int BenchVariant() { return g_benchVariant; }

// Called once from JNI_OnLoad, before any runtime exists.
inline void InitBenchVariant() {
  char value[PROP_VALUE_MAX] = {0};
  __system_property_get("debug.ns.benchvariant", value);
  g_benchVariant = static_cast<int>(std::strtol(value, nullptr, 10));
}

#else

constexpr int BenchVariant() { return 0; }

inline void InitBenchVariant() {}

#endif

}  // namespace tns

#endif  // BENCHVARIANT_H_

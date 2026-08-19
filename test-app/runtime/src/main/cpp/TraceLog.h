#ifndef TEST_APP_TRACELOG_H
#define TEST_APP_TRACELOG_H

#include <atomic>
#include <cstdint>
#include <string>

namespace tns {

/*
 * Category-scoped debug tracing.
 *
 * A process-wide bitmask of enabled categories, tested inline at every call
 * site, so a disabled category costs one relaxed load and a well-predicted
 * branch. Present in every build: these are traces, and a release build that
 * cannot be traced is a release build that cannot be diagnosed. Error and
 * lifecycle logs are unconditional and do not belong here.
 *
 * Turned on by the NS_DEBUG environment variable (read once at process init)
 * or by ns:runtime's `debug` config key.
 */
enum class LogCategory : uint8_t {
    Esm,       // module resolution, compilation, linking, evaluation
    Fetch,     // the HTTP module transport
    Registry,  // registry invalidation and dynamic-import cache bookkeeping
    kCount
};

/*
 * One bit per LogCategory. Written from process init and from main-isolate
 * setConfig; read from every thread. Relaxed suffices -- a trace line racing a
 * toggle changes nothing but that line.
 */
inline std::atomic<uint32_t> g_enabledLogCategories{0};

inline bool LogCategoryEnabled(LogCategory category) {
    return (g_enabledLogCategories.load(std::memory_order_relaxed) &
            (1u << static_cast<uint8_t>(category))) != 0;
}

/*
 * Writes one trace line under `category`, to that category's own logcat tag.
 * Out of line so nothing but the enabled test lands at the call site.
 */
void EmitDebugLog(LogCategory category, const char* format, ...)
        __attribute__((format(printf, 2, 3)));

const char* LogCategoryName(LogCategory category);
// Every category name, comma separated -- for the "valid categories are ..."
// diagnostic.
std::string AllLogCategoryNames();
// A comma-separated category list to a mask. Unknown names are skipped and
// reported through `hadUnknown` rather than failing the whole list.
uint32_t ParseLogCategories(const std::string& list, bool* hadUnknown);
// The canonical comma-separated list of the categories currently enabled.
std::string EnabledLogCategoryNames();
void SetEnabledLogCategories(uint32_t mask);
// Applies NS_DEBUG. Call once, before anything worth tracing runs.
void InitializeLogCategoriesFromEnvironment();

/*
 * A MACRO rather than a function or template on purpose: the arguments must
 * not be evaluated unless the category is on, and call sites routinely build
 * strings that cost far more than the line they would print.
 */
#define TNS_DEBUG(category, ...)                                                \
    do {                                                                        \
        if (tns::LogCategoryEnabled(tns::LogCategory::category)) [[unlikely]] { \
            tns::EmitDebugLog(tns::LogCategory::category, __VA_ARGS__);         \
        }                                                                       \
    } while (0)

}  // namespace tns

#endif  // TEST_APP_TRACELOG_H

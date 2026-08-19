#include "TraceLog.h"

#include <android/log.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "NativeScriptAssert.h"

namespace tns {

namespace {

// Index-aligned with tns::LogCategory; the only place a category name lives.
constexpr const char* kLogCategoryNames[] = {"esm", "fetch", "registry"};
// One logcat tag per category, so `adb logcat -s TNS.esm` filters without
// matching message text.
constexpr const char* kLogCategoryTags[] = {"TNS.esm", "TNS.fetch", "TNS.registry"};
constexpr size_t kLogCategoryCount = static_cast<size_t>(LogCategory::kCount);
static_assert(sizeof(kLogCategoryNames) / sizeof(kLogCategoryNames[0]) == kLogCategoryCount,
              "every LogCategory needs exactly one name");
static_assert(sizeof(kLogCategoryTags) / sizeof(kLogCategoryTags[0]) == kLogCategoryCount,
              "every LogCategory needs exactly one logcat tag");

void WriteDebugLine(LogCategory category, const char* message) {
    size_t index = static_cast<size_t>(category);
    const char* tag = index < kLogCategoryCount ? kLogCategoryTags[index] : "TNS.Native";
    __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", message);
}

std::string TrimAsciiSpace(const std::string& value) {
    size_t begin = value.find_first_not_of(" \t");
    if (begin == std::string::npos) {
        return std::string();
    }
    size_t end = value.find_last_not_of(" \t");
    return value.substr(begin, end - begin + 1);
}

}  // namespace

const char* LogCategoryName(LogCategory category) {
    size_t index = static_cast<size_t>(category);
    return index < kLogCategoryCount ? kLogCategoryNames[index] : "unknown";
}

std::string AllLogCategoryNames() {
    std::string names;
    for (size_t i = 0; i < kLogCategoryCount; ++i) {
        if (!names.empty()) {
            names += ",";
        }
        names += kLogCategoryNames[i];
    }
    return names;
}

uint32_t ParseLogCategories(const std::string& list, bool* hadUnknown) {
    if (hadUnknown != nullptr) {
        *hadUnknown = false;
    }

    uint32_t mask = 0;
    size_t start = 0;
    while (start <= list.size()) {
        size_t comma = list.find(',', start);
        size_t length = comma == std::string::npos ? std::string::npos : comma - start;
        std::string name = TrimAsciiSpace(list.substr(start, length));

        if (!name.empty()) {
            bool matched = false;
            for (size_t i = 0; i < kLogCategoryCount; ++i) {
                if (name == kLogCategoryNames[i]) {
                    mask |= 1u << i;
                    matched = true;
                    break;
                }
            }
            if (!matched && hadUnknown != nullptr) {
                *hadUnknown = true;
            }
        }

        if (comma == std::string::npos) {
            break;
        }
        start = comma + 1;
    }
    return mask;
}

std::string EnabledLogCategoryNames() {
    uint32_t mask = g_enabledLogCategories.load(std::memory_order_relaxed);
    std::string names;
    for (size_t i = 0; i < kLogCategoryCount; ++i) {
        if ((mask & (1u << i)) == 0) {
            continue;
        }
        if (!names.empty()) {
            names += ",";
        }
        names += kLogCategoryNames[i];
    }
    return names;
}

void SetEnabledLogCategories(uint32_t mask) {
    g_enabledLogCategories.store(mask, std::memory_order_relaxed);
}

void InitializeLogCategoriesFromEnvironment() {
    const char* value = getenv("NS_DEBUG");
    if (value == nullptr || *value == '\0') {
        return;
    }

    bool hadUnknown = false;
    SetEnabledLogCategories(ParseLogCategories(value, &hadUnknown));
    if (hadUnknown) {
        DEBUG_WRITE_FORCE("NS_DEBUG: ignoring unknown categories in '%s'; valid categories are %s",
                          value, AllLogCategoryNames().c_str());
    }
}

void EmitDebugLog(LogCategory category, const char* format, ...) {
    va_list ap;
    va_start(ap, format);

    char stackBuffer[1024];
    va_list apCopy;
    va_copy(apCopy, ap);
    int needed = vsnprintf(stackBuffer, sizeof(stackBuffer), format, apCopy);
    va_end(apCopy);

    if (needed < 0) {
        va_end(ap);
        return;
    }

    if (static_cast<size_t>(needed) < sizeof(stackBuffer)) {
        WriteDebugLine(category, stackBuffer);
    } else {
        std::vector<char> heapBuffer(static_cast<size_t>(needed) + 1);
        vsnprintf(heapBuffer.data(), heapBuffer.size(), format, ap);
        WriteDebugLine(category, heapBuffer.data());
    }

    va_end(ap);
}

}  // namespace tns

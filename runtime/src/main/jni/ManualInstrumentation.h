//
// Created by Panayot Cankov on 26/05/2017.
//

#ifndef TEST_APP_MANUALINSTRUMENTATION_H
#define TEST_APP_MANUALINSTRUMENTATION_H

#include "v8.h"
#import <chrono>
#import <NativeScriptAssert.h>

namespace tns {
    namespace instrumentation {
        class Frame {
        public:
            inline Frame() : Frame(nullptr) { }
            inline Frame(const char *name) : name(name), start(disabled ? disabled_time : std::chrono::steady_clock::now()) {}

            inline ~Frame() {
                if (name && check()) {
                    log(name);
                }
            }

            inline bool check() {
                if (disabled) {
                    return false;
                }
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::operator-(end, start)).count();
                return duration >= 16000;
            }

            inline void log(const char * message) {
                if (disabled) {
                    return;
                }
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::operator-(end, start)).count();
                auto startMilis = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count() / 1000.0;
                auto endMilis = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count() / 1000.0;
                __android_log_print(ANDROID_LOG_DEBUG, "JS", "Timeline: %.3fms: Runtime: %s  (%.3fms - %.3fms)", duration / 1000.0, message, startMilis, endMilis);
            }

            static inline void enable() { disabled = false; }
            static inline void disable() { disabled = true; }

        private:
            static bool disabled;
            static const std::chrono::steady_clock::time_point disabled_time; // Couldn't find reasonable constant

            const std::chrono::steady_clock::time_point start;
            const char *name;

            Frame(const Frame &) = delete;
            Frame &operator=(const Frame &) = delete;
        };
    };
};

/**
 * Place at the start of a method. Will log to android using the "JS" tag methods that execute relatively slow.
 */
#define TNSPERF() tns::instrumentation::Frame __tns_manual_instrumentation(__func__)

#endif //TEST_APP_MANUALINSTRUMENTATION_H

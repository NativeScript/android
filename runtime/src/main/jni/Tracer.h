#ifndef TRACER_H
#define TRACER_H

#include <string>
#include <jni.h>
#include <chrono>
#include "NativeScriptAssert.h"

namespace tns {

    class Tracer {
    public:
        static void Trace(const int descriptor, const char *format, ...);

        static void setEnabled(bool enable);

        static bool isEnabled();

        static void dumpToFileForDescriptor(const int descriptor);

        struct Descriptors {
        public:
            static const int CLASS = 0;
            static const int EXCEPTION = 1;
            static const int DIAGNOSTIC = 2;
            static const int WORKERS = 3;
            static const int APPLICATION = 4;
            static const int DEBUGGER = 5;
            static const int INFO = 6;
            static const int REQUIRE = 7;
            static const int REQUIRE_COMPILATION = 8;
            static const int REQUIRE_EVAL = 9;
            static const int BENCHMARK = 10;
            static const int Count = 11;
        };

    private:
        static bool enabled;
        static jclass TRACER_CLASS;
        static jmethodID TRACE_METHOD_ID;
        static jmethodID DUMP_TO_FILE_METHOD_ID;
        static jmethodID SET_ENABLED_METHOD_ID;
    };

}

#endif //TRACER_H



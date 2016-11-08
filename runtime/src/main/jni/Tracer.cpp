#include "Tracer.h"
#include "JEnv.h"
#include "JniLocalRef.h"

using namespace tns;

void Tracer::setEnabled(bool enable) {
    enabled = enable;

    if (enabled) {
        if (TRACER_CLASS == nullptr) {
            try {
                JEnv env;
                TRACER_CLASS = env.FindClass("com/tns/Tracer");
                TRACE_METHOD_ID = env.GetStaticMethodID(TRACER_CLASS, "trace",
                                                        "(ILjava/lang/String;)V");
                SET_ENABLED_METHOD_ID = env.GetStaticMethodID(TRACER_CLASS, "setEnabled", "(Z)V");
                DUMP_TO_FILE_METHOD_ID = env.GetStaticMethodID(TRACER_CLASS, "dumpToFile", "(I)V");
            } catch (std::exception ex) {
                return;
            }
        }
    }
}

void Tracer::Trace(const int descriptor, const char *format, ...) {
    if (!enabled) {
        return;
    }

    JEnv env;

    char buffer[1024];
    va_list args;
    va_start (args, format);
    vsprintf(buffer, format, args);
    JniLocalRef traceMessage(env.NewStringUTF(buffer));
    env.CallStaticVoidMethod(TRACER_CLASS, TRACE_METHOD_ID, (jint) descriptor,
                             (jstring) traceMessage);
    va_end(args);
}

void Tracer::dumpToFileForDescriptor(const int descriptor) {
    if (!enabled) {
        return;
    }

    JEnv env;
    env.CallStaticVoidMethod(TRACER_CLASS, DUMP_TO_FILE_METHOD_ID, descriptor);
}

bool Tracer::isEnabled() {
    return enabled;
}

bool Tracer::enabled = false;
jclass Tracer::TRACER_CLASS = nullptr;
jmethodID Tracer::TRACE_METHOD_ID;
jmethodID Tracer::DUMP_TO_FILE_METHOD_ID;
jmethodID Tracer::SET_ENABLED_METHOD_ID;
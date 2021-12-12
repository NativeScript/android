#ifndef PROFILER_H_
#define PROFILER_H_

#include "v8.h"
#include "v8-profiler.h"
#include <string>

namespace tns {
class Profiler {
    public:
        Profiler();

        void Init(v8::Isolate* isolate, const v8::Local<v8::Object>& globalObj, const std::string& appName, const std::string& outputDir);

    private:
        static void StartCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void StopCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void StartNDKProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void StopNDKProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void HeapSnapshotMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void StartCPUProfilerCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args);

        void StopCPUProfilerCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args);

        void HeapSnapshotMethodCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args);

        void StartCPUProfiler(v8::Isolate* isolate, const v8::Local<v8::String>& name);

        bool StopCPUProfiler(v8::Isolate* isolate, const v8::Local<v8::String>& name);

        void StartNDKProfiler();

        void StopNDKProfiler();

        bool Write(v8::CpuProfile* cpuProfile);

        std::string m_appName;

        std::string m_outputDir;
};
}

#endif /* PROFILER_H_ */

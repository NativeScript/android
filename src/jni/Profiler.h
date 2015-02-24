#ifndef PROFILER_H_
#define PROFILER_H_

#include "v8.h"
#include "v8-profiler.h"
#include <string>

namespace tns
{
	class Profiler
	{
	public:
		static void Init(const std::string& appName);

		static void StartCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void StopCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

	private:
		Profiler();

		static void StartCPUProfiler(v8::Isolate *isolate, const v8::Handle<v8::String>& name);

		static bool StopCPUProfiler(v8::Isolate *isolate, const v8::Handle<v8::String>& name);

		static bool Write(v8::CpuProfile *cpuProfile);

		static std::string s_appName;
	};
}


#endif /* PROFILER_H_ */

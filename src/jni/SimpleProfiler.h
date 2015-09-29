#ifndef SIMPLEPROFILER_H_
#define SIMPLEPROFILER_H_

#include "v8.h"
#include <cstdint>
#include <vector>

namespace tns
{
#ifndef SIMPLE_PROFILER
	#define SET_PROFILER_FRAME() ((void)0)
#else
	#define SET_PROFILER_FRAME() SimpleProfiler __frame(__FILE__, __LINE__)
#endif

	class SimpleProfiler
	{
	public:
		SimpleProfiler(char *fileName, int lineNumber);

		~SimpleProfiler();

		static void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& globalTemplate);

		static void PrintProfilerData();

	private:
		struct FrameEntry
		{
			FrameEntry(char *_fileName, int _lineNumer)
				: fileName(_fileName), lineNumber(_lineNumer), time(0), stackCount(0)
			{
			}
			bool operator<(const FrameEntry &rhs) const
			{
				return time < rhs.time;
			}
			char *fileName;
			int lineNumber;
			int64_t time;
			int stackCount;
		};

		static void PrintProfilerDataCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		FrameEntry *m_frame;
		int64_t m_time;
		static std::vector<FrameEntry> s_frames;
	};
}

#endif /* SIMPLEPROFILER_H_ */

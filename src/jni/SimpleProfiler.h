#ifndef SIMPLEPROFILER_H_
#define SIMPLEPROFILER_H_

#include "v8.h"
#include <cstdint>
#include <vector>

namespace tns
{
	class SimpleProfiler
	{
	public:
		SimpleProfiler(void *func);

		~SimpleProfiler();

		static void Init(v8::Isolate *isolate, v8::Handle<v8::ObjectTemplate>& globalTemplate);

		static void PrintProfilerData();

	private:
		struct FrameEntry
		{
			FrameEntry(void *_func)
				: func(_func), stackCount(0), time(0)
			{
			}
			void *func;
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

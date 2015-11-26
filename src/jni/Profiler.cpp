#include "Profiler.h"
#include "V8GlobalHelpers.h"
#include "prof.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include <stack>

using namespace v8;
using namespace std;
using namespace tns;


Profiler::Profiler()
{
}


void Profiler::Init(const string& appName)
{
	s_appName = appName;
}


void Profiler::StartCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();
	auto started = false;
	if ((args.Length() == 1) && (args[0]->IsString()))
	{
		auto name = args[0]->ToString();
		StartCPUProfiler(isolate, name);
		started = true;
	}

	args.GetReturnValue().Set(Boolean::New(isolate, started));

	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void Profiler::StopCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();
	auto stopped = false;
	if ((args.Length() == 1) && (args[0]->IsString()))
	{
		auto name = args[0]->ToString();
		stopped = StopCPUProfiler(isolate, name);
	}
	args.GetReturnValue().Set(Boolean::New(isolate, stopped));
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void Profiler::StartCPUProfiler(Isolate *isolate, const Local<String>& name)
{
	auto v8prof = isolate->GetCpuProfiler();
	v8prof->StartProfiling(name, true);
}


bool Profiler::StopCPUProfiler(Isolate *isolate, const Local<String>& name)
{
	auto v8prof = isolate->GetCpuProfiler();
	auto cpuProfile = v8prof->StopProfiling(name);

	auto success = false;

	if (nullptr != cpuProfile)
	{
		success = Write(cpuProfile);
		cpuProfile->Delete();
	}

	return success;
}

bool Profiler::Write(CpuProfile *cpuProfile)
{
	struct timespec nowt;
	clock_gettime(CLOCK_MONOTONIC, &nowt);
	uint64_t now = (int64_t) nowt.tv_sec*1000000000LL + nowt.tv_nsec;

	auto sec = static_cast<unsigned long>(now / 1000000);
	auto usec = static_cast<unsigned long>(now % 1000000);

	char filename[256];
	auto profileName = ConvertToString(cpuProfile->GetTitle());
	snprintf(filename, sizeof(filename), "/sdcard/%s-%s-%lu.%lu.cpuprofile", s_appName.c_str(), profileName.c_str(), sec, usec);

	auto fp = fopen(filename, "w");
	if (nullptr == fp)
	{
		return false;
	}

	fwrite("{\"head\":", sizeof(char), 8, fp);

	stack<const CpuProfileNode*> s;
	s.push(cpuProfile->GetTopDownRoot());

	char buff[1024];
	auto COMMA_NODE = reinterpret_cast<const CpuProfileNode*>(1);
	auto CLOSE_NODE = reinterpret_cast<const CpuProfileNode*>(2);

	while (!s.empty())
	{
		const CpuProfileNode *node = s.top();
		s.pop();
		if (node == CLOSE_NODE)
		{
			fwrite("]}", sizeof(char), 2, fp);
		}
		else if (node == COMMA_NODE)
		{
			fwrite(",", sizeof(char), 1, fp);
		}
		else
		{
			snprintf(buff, sizeof(buff), "{\"functionName\":\"%s\",\"scriptId\":%d,\"url\":\"%s\",\"lineNumber\":%d,\"columnNumber\":%d,\"hitCount\":%u,\"callUID\":%u,\"deoptReason\":\"%s\",\"id\":%u,\"children\":[",
				ConvertToString(node->GetFunctionName()).c_str(),
				node->GetScriptId(),
				ConvertToString(node->GetScriptResourceName()).c_str(),
				node->GetLineNumber(),
				node->GetColumnNumber(),
				node->GetHitCount(),
				node->GetCallUid(),
				node->GetBailoutReason(),
				node->GetNodeId());
			fwrite(buff, sizeof(char), strlen(buff), fp);

			s.push(CLOSE_NODE);

			int count = node->GetChildrenCount();
			for (int i=0; i<count; i++)
			{
				if (i > 0)
				{
					s.push(COMMA_NODE);
				}
				s.push(node->GetChild(i));
			}
		}
	}

	const double CONV_RATIO = 1000000.0;
	auto startTime = static_cast<double>(cpuProfile->GetStartTime()) / CONV_RATIO;
	auto endTime = static_cast<double>(cpuProfile->GetEndTime()) / CONV_RATIO;
	snprintf(buff, sizeof(buff), ",\"startTime\":%lf,\"endTime\":%lf,\"samples\":[", startTime, endTime);
	fwrite(buff, sizeof(char), strlen(buff), fp);
	int sampleCount = cpuProfile->GetSamplesCount();
	for (int i=0; i<sampleCount; i++)
	{
		auto format = (i > 0) ? ",%d" : "%d";
		snprintf(buff, sizeof(buff), format, cpuProfile->GetSample(i)->GetScriptId());
		fwrite(buff, sizeof(char), strlen(buff), fp);
	}

	// TODO: update V8, see https://codereview.chromium.org/259803002/
	/*
	snprintf(buff, sizeof(buff), "],\"timestamps\":[");
	fwrite(buff, sizeof(char), strlen(buff), fp);
	auto currTimeStamp = startTimeStamp;
	for (int i=0; i<sampleCount; i++)
	{
		auto format = (i > 0) ? ",%lld" : "%lld";
		snprintf(buff, sizeof(buff), format, cpuProfile->GetSampleTimestamp(i));
		fwrite(buff, sizeof(char), strlen(buff), fp);
	}
	*/

	fwrite("]}", sizeof(char), 2, fp);
	fclose(fp);

	return true;
}

void Profiler::StartNDKProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	StartNDKProfiler();
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void Profiler::StopNDKProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	StopNDKProfiler();
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void Profiler::StartNDKProfiler()
{
#ifdef NDK_PROFILER_ENABLED
	monstartup("libNativeScript.so");
#endif
}

void  Profiler::StopNDKProfiler()
{
#ifdef NDK_PROFILER_ENABLED
	moncleanup();
#endif
}

class FileOutputStream: public OutputStream
{
	public:
		FileOutputStream(FILE* stream) :
				stream_(stream)
		{
		}

		virtual int GetChunkSize()
		{
			return 65536; // big chunks == faster
		}

		virtual void EndOfStream()
		{
		}

		virtual WriteResult WriteAsciiChunk(char* data, int size)
		{
			const size_t len = static_cast<size_t>(size);
			size_t off = 0;

			while (off < len && !feof(stream_) && !ferror(stream_))
				off += fwrite(data + off, 1, len - off, stream_);

			return off == len ? kContinue : kAbort;
		}

	private:
		FILE* stream_;
};

void Profiler::HeapSnapshotMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	struct timespec nowt;
	clock_gettime(CLOCK_MONOTONIC, &nowt);
	uint64_t now = (int64_t) nowt.tv_sec*1000000000LL + nowt.tv_nsec;

	unsigned long sec = static_cast<unsigned long>(now / 1000000);
	unsigned long usec = static_cast<unsigned long>(now % 1000000);

	char filename[256];
	snprintf(filename, sizeof(filename), "/sdcard/%s-heapdump-%lu.%lu.heapsnapshot", s_appName.c_str(), sec, usec);

	FILE* fp = fopen(filename, "w");
	if (fp == nullptr)
	{
		return;
	}

	Isolate* isolate = Isolate::GetCurrent();

	const HeapSnapshot* snap = isolate->GetHeapProfiler()->TakeHeapSnapshot();

	FileOutputStream stream(fp);
	snap->Serialize(&stream, HeapSnapshot::kJSON);
	fclose(fp);
	const_cast<HeapSnapshot*>(snap)->Delete();
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

string Profiler::s_appName;

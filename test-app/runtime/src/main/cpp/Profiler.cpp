#include "Profiler.h"
#include "ArgConverter.h"
#include "prof.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

Profiler::Profiler() {
}

void Profiler::Init(Isolate* isolate, const Local<Object>& globalObj, const string& appName, const string& outputDir) {
    m_appName = appName;
    m_outputDir = outputDir;
    auto extData = External::New(isolate, this);
    globalObj->Set(ArgConverter::ConvertToV8String(isolate, "__startCPUProfiler"), FunctionTemplate::New(isolate, Profiler::StartCPUProfilerCallback, extData)->GetFunction());
    globalObj->Set(ArgConverter::ConvertToV8String(isolate, "__stopCPUProfiler"), FunctionTemplate::New(isolate, Profiler::StopCPUProfilerCallback, extData)->GetFunction());
    globalObj->Set(ArgConverter::ConvertToV8String(isolate, "__heapSnapshot"), FunctionTemplate::New(isolate, Profiler::HeapSnapshotMethodCallback, extData)->GetFunction());
    globalObj->Set(ArgConverter::ConvertToV8String(isolate, "__startNDKProfiler"), FunctionTemplate::New(isolate, Profiler::StartNDKProfilerCallback, extData)->GetFunction());
    globalObj->Set(ArgConverter::ConvertToV8String(isolate, "__stopNDKProfiler"), FunctionTemplate::New(isolate, Profiler::StopNDKProfilerCallback, extData)->GetFunction());
}

void Profiler::StartCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto extData = args.Data().As<External>();
        auto thiz = static_cast<Profiler*>(extData->Value());
        thiz->StartCPUProfilerCallbackImpl(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Profiler::StartCPUProfilerCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = args.GetIsolate();
    auto started = false;
    if ((args.Length() == 1) && (args[0]->IsString())) {
        auto name = args[0]->ToString();
        StartCPUProfiler(isolate, name);
        started = true;
    }
    args.GetReturnValue().Set(started);
}

void Profiler::StopCPUProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto extData = args.Data().As<External>();
        auto thiz = static_cast<Profiler*>(extData->Value());
        thiz->StopCPUProfilerCallbackImpl(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Profiler::StopCPUProfilerCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = args.GetIsolate();
    auto stopped = false;
    if ((args.Length() == 1) && (args[0]->IsString())) {
        auto name = args[0]->ToString();
        stopped = StopCPUProfiler(isolate, name);
    }
    args.GetReturnValue().Set(stopped);
}

void Profiler::StartCPUProfiler(Isolate* isolate, const Local<String>& name) {
    auto v8prof = isolate->GetCpuProfiler();
    v8prof->StartProfiling(name, true);
}

bool Profiler::StopCPUProfiler(Isolate* isolate, const Local<String>& name) {
    auto v8prof = isolate->GetCpuProfiler();
    auto cpuProfile = v8prof->StopProfiling(name);

    auto success = false;

    if (nullptr != cpuProfile) {
        success = Write(cpuProfile);
        cpuProfile->Delete();
    }

    return success;
}

bool Profiler::Write(CpuProfile* cpuProfile) {
    struct timespec nowt;
    clock_gettime(CLOCK_MONOTONIC, &nowt);
    uint64_t now = (int64_t) nowt.tv_sec * 1000000000LL + nowt.tv_nsec;

    auto sec = static_cast<unsigned long>(now / 1000000);
    auto usec = static_cast<unsigned long>(now % 1000000);

    char filename[256];
    auto profileName = ArgConverter::ConvertToString(cpuProfile->GetTitle());
    snprintf(filename, sizeof(filename), "%s/%s-%s-%lu.%lu.cpuprofile", m_outputDir.c_str(), m_appName.c_str(), profileName.c_str(), sec, usec);

    auto fp = fopen(filename, "w");
    if (nullptr == fp) {
        return false;
    }

    fwrite("{\"head\":", sizeof(char), 8, fp);

    stack<const CpuProfileNode*> s;
    s.push(cpuProfile->GetTopDownRoot());

    char buff[1024];
    auto COMMA_NODE = reinterpret_cast<const CpuProfileNode*>(1);
    auto CLOSE_NODE = reinterpret_cast<const CpuProfileNode*>(2);
    auto PREFIX = string("RegExp:");

    while (!s.empty()) {
        const CpuProfileNode* node = s.top();
        s.pop();
        if (node == CLOSE_NODE) {
            fwrite("]}", sizeof(char), 2, fp);
        } else if (node == COMMA_NODE) {
            fwrite(",", sizeof(char), 1, fp);
        } else {
            auto funcName = ArgConverter::ConvertToString(node->GetFunctionName());
            auto scriptName = ArgConverter::ConvertToString(node->GetScriptResourceName());
            auto lineNumber = node->GetLineNumber();
            auto columnNumber = node->GetColumnNumber();
            if (funcName.compare(0, PREFIX.size(), PREFIX) == 0) {
                stringstream ss;
                ss << "RegExp_" << scriptName << "_" << lineNumber << "_" << columnNumber;
                funcName = ss.str();
            }
            snprintf(buff, sizeof(buff), "{\"functionName\":\"%s\",\"scriptId\":%d,\"url\":\"%s\",\"lineNumber\":%d,\"columnNumber\":%d,\"hitCount\":%u,\"callUID\":%u,\"deoptReason\":\"%s\",\"id\":%u,\"children\":[",
                     funcName.c_str(),
                     node->GetScriptId(),
                     scriptName.c_str(),
                     lineNumber,
                     columnNumber,
                     node->GetHitCount(),
                     node->GetCallUid(),
                     node->GetBailoutReason(),
                     node->GetNodeId());
            fwrite(buff, sizeof(char), strlen(buff), fp);

            s.push(CLOSE_NODE);

            int count = node->GetChildrenCount();
            for (int i = 0; i < count; i++) {
                if (i > 0) {
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
    for (int i = 0; i < sampleCount; i++) {
        auto format = (i > 0) ? ",%d" : "%d";
        snprintf(buff, sizeof(buff), format, cpuProfile->GetSample(i)->GetNodeId());
        fwrite(buff, sizeof(char), strlen(buff), fp);
    }

    snprintf(buff, sizeof(buff), "],\"timestamps\":[");
    fwrite(buff, sizeof(char), strlen(buff), fp);
    for (int i=0; i<sampleCount; i++) {
        auto format = (i > 0) ? ",%lld" : "%lld";
        snprintf(buff, sizeof(buff), format, cpuProfile->GetSampleTimestamp(i));
        fwrite(buff, sizeof(char), strlen(buff), fp);
    }

    fwrite("]}", sizeof(char), 2, fp);
    fclose(fp);

    return true;
}

void Profiler::StartNDKProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto extData = args.Data().As<External>();
        auto thiz = static_cast<Profiler*>(extData->Value());
        thiz->StartNDKProfiler();
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Profiler::StopNDKProfilerCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto extData = args.Data().As<External>();
        auto thiz = static_cast<Profiler*>(extData->Value());
        thiz->StopNDKProfiler();
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Profiler::StartNDKProfiler() {
#ifdef NDK_PROFILER_ENABLED
    monstartup("libNativeScript.so");
#endif
}

void Profiler::StopNDKProfiler() {
#ifdef NDK_PROFILER_ENABLED
    moncleanup();
#endif
}

class FileOutputStream: public OutputStream {
    public:
        FileOutputStream(FILE* stream) :
            stream_(stream) {
        }

        virtual int GetChunkSize() {
            return 65536; // big chunks == faster
        }

        virtual void EndOfStream() {
        }

        virtual WriteResult WriteAsciiChunk(char* data, int size) {
            const size_t len = static_cast<size_t>(size);
            size_t off = 0;

            while (off < len && !feof(stream_) && !ferror(stream_)) {
                off += fwrite(data + off, 1, len - off, stream_);
            }

            return off == len ? kContinue : kAbort;
        }

    private:
        FILE* stream_;
};

void Profiler::HeapSnapshotMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto extData = args.Data().As<External>();
        auto thiz = static_cast<Profiler*>(extData->Value());
        thiz->HeapSnapshotMethodCallbackImpl(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Profiler::HeapSnapshotMethodCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args) {
    struct timespec nowt;
    clock_gettime(CLOCK_MONOTONIC, &nowt);
    uint64_t now = (int64_t) nowt.tv_sec * 1000000000LL + nowt.tv_nsec;

    unsigned long sec = static_cast<unsigned long>(now / 1000000);
    unsigned long usec = static_cast<unsigned long>(now % 1000000);

    char filename[256];
    snprintf(filename, sizeof(filename), "%s/%s-heapdump-%lu.%lu.heapsnapshot", m_outputDir.c_str(), m_appName.c_str(), sec, usec);

    FILE* fp = fopen(filename, "w");
    if (fp == nullptr) {
        return;
    }

    auto isolate = args.GetIsolate();

    const HeapSnapshot* snap = isolate->GetHeapProfiler()->TakeHeapSnapshot();

    FileOutputStream stream(fp);
    snap->Serialize(&stream, HeapSnapshot::kJSON);
    fclose(fp);
    const_cast<HeapSnapshot*>(snap)->Delete();
}


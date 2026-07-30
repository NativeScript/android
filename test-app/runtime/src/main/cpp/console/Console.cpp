//
// Created by pkanev on 12/8/2017.
//

#include <include/v8.h>
#include <assert.h>
#include <android/log.h>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include <V8GlobalHelpers.h>
#include <NativeScriptException.h>

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "Console.h"
#include "robin_hood.h"

namespace tns {

// internal/inspect.js, one compiled instance per isolate. Worker runtimes
// initialize on their own threads, so every access is under the mutex.
static std::mutex inspectMutex;
static robin_hood::unordered_map<v8::Isolate*, v8::Persistent<v8::Function>*> isolateToInspect;

static v8::Local<v8::Function> getInspectFunction(v8::Isolate* isolate) {
    std::lock_guard<std::mutex> lock(inspectMutex);
    auto it = isolateToInspect.find(isolate);
    if (it == isolateToInspect.end()) {
        return v8::Local<v8::Function>();
    }
    return it->second->Get(isolate);
}

v8::Local<v8::Object> Console::createConsole(v8::Local<v8::Context> context, ConsoleCallback callback, const int maxLogcatObjectSize, const bool forceLog) {
    m_callback = callback;
    m_maxLogcatObjectSize = maxLogcatObjectSize;
    v8::Context::Scope contextScope(context);
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::Object> console = v8::Object::New(isolate);
    bool success = console->SetPrototype(context, v8::Object::New(isolate)).FromMaybe(false);

    assert(success);

    std::map<std::string, double> timersMap;
    Console::s_isolateToConsoleTimersMap.insert(
        std::make_pair(v8::Isolate::GetCurrent(), timersMap));

    bindFunctionProperty(context, console, "assert", assertCallback);
    bindFunctionProperty(context, console, "error", errorCallback);
    bindFunctionProperty(context, console, "info", infoCallback);
    bindFunctionProperty(context, console, "log", logCallback);
    bindFunctionProperty(context, console, "warn", warnCallback);
    bindFunctionProperty(context, console, "dir", dirCallback);
    bindFunctionProperty(context, console, "trace", traceCallback);
    bindFunctionProperty(context, console, "time", timeCallback);
    bindFunctionProperty(context, console, "timeEnd", timeEndCallback);

    initInspect(context);

    return console;
}

static void getNativeWrapperHintCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    auto isolate = info.GetIsolate();
    if (info.Length() < 1) {
        return;
    }

    std::string hint = GetNativeWrapperHint(isolate, info[0]);
    if (!hint.empty()) {
        info.GetReturnValue().Set(ArgConverter::ConvertToV8String(isolate, hint));
    }
}

void Console::initInspect(v8::Local<v8::Context> context) {
    auto isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::Function> hintFunc;
    if (!v8::Function::New(context, getNativeWrapperHintCallback).ToLocal(&hintFunc)) {
        __android_log_write(ANDROID_LOG_WARN, LOG_TAG,
                            "Warning: Console failed to create the native-hint binding");
        return;
    }

    auto binding = v8::Object::New(isolate);
    if (!binding->Set(context, ArgConverter::ConvertToV8String(isolate, "getNativeWrapperHint"),
                      hintFunc).FromMaybe(false)) {
        __android_log_write(ANDROID_LOG_WARN, LOG_TAG,
                            "Warning: Console failed to populate the inspect binding");
        return;
    }

    v8::TryCatch tc(isolate);
    v8::Local<v8::Value> result;
    if (!BuiltinLoader::RunBuiltin(context, BuiltinId::kInspect, binding).ToLocal(&result) ||
            !result->IsFunction()) {
        __android_log_write(ANDROID_LOG_WARN, LOG_TAG,
                            "Warning: Console failed to initialize the inspect builtin");
        return;
    }

    std::lock_guard<std::mutex> lock(inspectMutex);
    isolateToInspect.emplace(isolate, new v8::Persistent<v8::Function>(isolate, result.As<v8::Function>()));
}

// depth < 0 leaves the builtin's own default (2); console.dir passes 4.
static std::string inspectValue(v8::Isolate* isolate, const v8::Local<v8::Value>& val, int depth = -1) {
    auto context = isolate->GetCurrentContext();

    auto inspect = getInspectFunction(isolate);
    if (!inspect.IsEmpty()) {
        v8::Local<v8::Value> args[2];
        int argc = 1;
        args[0] = val;
        if (depth >= 0) {
            auto options = v8::Object::New(isolate);
            if (options->Set(context, ArgConverter::ConvertToV8String(isolate, "depth"),
                             v8::Number::New(isolate, depth)).FromMaybe(false)) {
                args[1] = options;
                argc = 2;
            }
        }

        v8::TryCatch tc(isolate);
        v8::Local<v8::Value> result;
        if (inspect->Call(context, v8::Undefined(isolate), argc, args).ToLocal(&result) &&
                result->IsString()) {
            return ArgConverter::ConvertToString(result.As<v8::String>());
        }
    }

    // Init failed or the formatter threw: degrade to V8's own short description.
    v8::TryCatch tc(isolate);
    v8::Local<v8::String> fallback;
    if (val->ToDetailString(context).ToLocal(&fallback)) {
        return ArgConverter::ConvertToString(fallback);
    }
    return "";
}

void Console::sendToADBLogcat(const std::string& message, android_LogPriority logPriority) {
    // limit the size of the message that we send to logcat using the predefined value in package.json
    auto messageToLog = message;
    if (messageToLog.length() > m_maxLogcatObjectSize) {
        messageToLog = messageToLog.erase(m_maxLogcatObjectSize, std::string::npos);
        messageToLog = messageToLog + "...";
    }

    // split strings into chunks of 4000 characters
    // __android_log_write can't send more than 4000 to the stdout at a time
    auto messageLength = messageToLog.length();
    int maxStringLength = 4000;

    if (messageLength < maxStringLength) {
        __android_log_write(logPriority, Console::LOG_TAG, messageToLog.c_str());
    } else {
        for (int i = 0; i < messageLength; i += maxStringLength) {
            auto messagePart = messageToLog.substr(i, maxStringLength);

            __android_log_write(logPriority, Console::LOG_TAG, messagePart.c_str());
        }
    }
}

void Console::sendToDevToolsFrontEnd(v8::Isolate* isolate, ConsoleAPIType method, const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (!m_callback) {
        return;
    }

    std::vector<v8::Local<v8::Value>> arg_vector;
    unsigned nargs = args.Length();
    arg_vector.reserve(nargs);
    for (unsigned ix = 0; ix < nargs; ix++)
        arg_vector.push_back(args[ix]);

    m_callback(isolate, method, arg_vector);
}

void Console::sendToDevToolsFrontEnd(v8::Isolate* isolate, ConsoleAPIType method, const std::string& message) {
    if (!m_callback) {
        return;
    }

    std::vector<v8::Local<v8::Value>> args{ArgConverter::ConvertToV8String(isolate, message)};
    m_callback(isolate, method, args);
}

std::string buildStringFromArg(v8::Isolate* isolate, const v8::Local<v8::Value>& val) {
    // Top-level strings print raw (console.log("hi") -> hi); everything else
    // that can carry structure goes through the inspect builtin.
    if (val->IsString()) {
        return ArgConverter::ConvertToString(val.As<v8::String>());
    }
    if (val->IsObject()) {
        return inspectValue(isolate, val);
    }

    v8::TryCatch tc(isolate);
    v8::Local<v8::String> detail;
    if (val->ToDetailString(isolate->GetCurrentContext()).ToLocal(&detail)) {
        return ArgConverter::ConvertToString(detail);
    }
    return "";
}

std::string buildLogString(const v8::FunctionCallbackInfo<v8::Value>& info, int startingIndex = 0) {
    auto isolate = info.GetIsolate();
    v8::HandleScope scope(isolate);
    std::stringstream ss;

    auto argLen = info.Length();
    if (argLen) {
        for (int i = startingIndex; i < argLen; i++) {
            // separate args with a space
            if (i != 0) {
                ss << " ";
            }
            
            std::string argString = buildStringFromArg(isolate, info[i]);
            ss << argString;
        }
    } else {
        ss << std::endl;
    }

    return ss.str();
}

void Console::assertCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        auto isolate = info.GetIsolate();

        auto argLen = info.Length();
        auto expressionPasses = argLen && info[0]->BooleanValue(isolate);

        if (!expressionPasses) {
            std::stringstream assertionError;

            assertionError << "Assertion failed: ";

            if (argLen > 1) {
                assertionError << buildLogString(info, 1);
            } else {
                assertionError << "console.assert";
            }

            std::string log = assertionError.str();
            sendToADBLogcat(log, ANDROID_LOG_ERROR);
            sendToDevToolsFrontEnd(isolate, ConsoleAPIType::kAssert, info);
        }
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::errorCallback(const v8::FunctionCallbackInfo <v8::Value>& info) {
    try {
        std::string log = "CONSOLE ERROR: ";
        log += buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_ERROR);
        sendToDevToolsFrontEnd(info.GetIsolate(), ConsoleAPIType::kError, info);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::infoCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        std::string log = "CONSOLE INFO: ";
        log += buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_INFO);
        sendToDevToolsFrontEnd(info.GetIsolate(), ConsoleAPIType::kInfo, info);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::logCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        std::string log = "CONSOLE LOG: ";
        log += buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_INFO);
        sendToDevToolsFrontEnd(info.GetIsolate(), ConsoleAPIType::kLog, info);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::warnCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        std::string log = "CONSOLE WARN: ";
        log += buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_WARN);
        sendToDevToolsFrontEnd(info.GetIsolate(), ConsoleAPIType::kWarning, info);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::dirCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        auto isolate = info.GetIsolate();

        v8::HandleScope scope(isolate);

        std::stringstream ss;

        auto argLen = info.Length();
        if (argLen) {
            if (info[0]->IsObject()) {
                ss << "==== object dump start ====" << std::endl;
                ss << inspectValue(isolate, info[0], 4) << std::endl;
                ss << "==== object dump end ====" << std::endl;
            } else {
                std::string logString = buildLogString(info);

                ss << logString;
            }
        } else {
            ss << std::endl;
        }

        std::string log = ss.str();

        sendToADBLogcat(log, ANDROID_LOG_INFO);
        sendToDevToolsFrontEnd(isolate, ConsoleAPIType::kDir, info);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

const std::string buildStacktraceFrameLocationPart(v8::Local<v8::StackFrame> frame) {
    std::stringstream ss;

    auto scriptName = frame->GetScriptNameOrSourceURL();
    auto scriptNameConverted = ArgConverter::ConvertToString(scriptName);
    if (scriptNameConverted.length() < 1) {
        ss << "VM";
    } else {
        ss << scriptNameConverted << ":" << frame->GetLineNumber() << ":" << frame->GetColumn();
    }

    std::string stringResult = ss.str();

    return stringResult;
}

const std::string buildStacktraceFrameMessage(v8::Local<v8::StackFrame> frame) {
    std::stringstream ss;

    auto functionName = frame->GetFunctionName();
    auto functionNameConverted = ArgConverter::ConvertToString(functionName);
    if (functionNameConverted.length() < 1) {
        functionNameConverted = "<anonymous>";
    }

    if (frame->IsConstructor()) {
        ss << "at new " << functionNameConverted << " (" << buildStacktraceFrameLocationPart(frame) << ")";
    } else if (frame->IsEval()) {
        ss << "eval at " << buildStacktraceFrameLocationPart(frame) << std::endl;
    } else {
        ss << "at " << functionNameConverted << " (" << buildStacktraceFrameLocationPart(frame) << ")";
    }

    std::string stringResult = ss.str();

    return stringResult;
}

void Console::traceCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        auto isolate = info.GetIsolate();
        std::stringstream ss;

        std::string logString = buildLogString(info);

        if (logString.compare("\n") == 0) {
            ss << "Trace";
        } else {
            ss << "Trace: " << logString;
        }

        ss << std::endl;

        v8::HandleScope scope(isolate);

        auto stack = v8::StackTrace::CurrentStackTrace(isolate, 10, v8::StackTrace::StackTraceOptions::kDetailed);

        auto framesCount = stack->GetFrameCount();

        for (int i = 0; i < framesCount; i++) {
            auto frame = stack->GetFrame(isolate, i);

            ss << buildStacktraceFrameMessage(frame) << std::endl;
        }

        std::string log = ss.str();
        __android_log_write(ANDROID_LOG_ERROR, LOG_TAG, log.c_str());
        sendToDevToolsFrontEnd(isolate, ConsoleAPIType::kTrace, info);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::timeCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        auto isolate = info.GetIsolate();

        v8::HandleScope scope(isolate);

        auto argLen = info.Length();
        std::string label = "default";

        v8::Local<v8::String> argString;
        if (argLen && info[0]->ToString(isolate->GetCurrentContext()).ToLocal(&argString)) {
            label = ArgConverter::ConvertToString(argString);
        }

        auto it = Console::s_isolateToConsoleTimersMap.find(isolate);
        if (it == Console::s_isolateToConsoleTimersMap.end()) {
            // throw?
        }

        auto nano = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
        double timeStamp = nano.time_since_epoch().count();

        it->second.insert(std::make_pair(label, timeStamp));
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::timeEndCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        auto isolate = info.GetIsolate();

        auto argLen = info.Length();
        std::string label = "default";

        v8::Local<v8::String> argString;
        if (argLen && info[0]->ToString(isolate->GetCurrentContext()).ToLocal(&argString)) {
            label = ArgConverter::ConvertToString(argString);
        }

        auto it = Console::s_isolateToConsoleTimersMap.find(isolate);
        if (it == Console::s_isolateToConsoleTimersMap.end()) {
            // throw?
        }

        std::map<std::string, double> timersMap = it->second;

        auto itTimersMap = timersMap.find(label);
        if (itTimersMap == timersMap.end()) {
            std::string warning = std::string("No such label '" + label + "' for console.timeEnd()");

            __android_log_write(ANDROID_LOG_WARN, LOG_TAG, warning.c_str());
            sendToDevToolsFrontEnd(isolate, ConsoleAPIType::kWarning, warning);

            return;
        }

        auto nano = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
        double endTimeStamp = nano.time_since_epoch().count();
        double startTimeStamp = itTimersMap->second;

        it->second.erase(label);

        double diffMicroseconds = endTimeStamp - startTimeStamp;
        double diffMilliseconds = diffMicroseconds / 1000.0;

        std::stringstream ss;
        ss << "CONSOLE TIME: " << label << ": " << std::fixed << std::setprecision(3) << diffMilliseconds << "ms" ;
        std::string log = ss.str();

        __android_log_write(ANDROID_LOG_INFO, LOG_TAG, log.c_str());
        sendToDevToolsFrontEnd(isolate, ConsoleAPIType::kTimeEnd, log);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void Console::onDisposeIsolate(v8::Isolate* isolate) {
    s_isolateToConsoleTimersMap.erase(isolate);

    std::lock_guard<std::mutex> lock(inspectMutex);
    auto it = isolateToInspect.find(isolate);
    if (it != isolateToInspect.end()) {
        delete it->second;
        isolateToInspect.erase(it);
    }
}

const char* Console::LOG_TAG = "JS";
std::map<v8::Isolate*, std::map<std::string, double>> Console::s_isolateToConsoleTimersMap;
ConsoleCallback Console::m_callback = nullptr;
int Console::m_maxLogcatObjectSize;
}
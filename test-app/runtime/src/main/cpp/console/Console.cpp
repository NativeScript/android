//
// Created by pkanev on 12/8/2017.
//

#include <include/v8.h>
#include <assert.h>
#include <android/log.h>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <V8GlobalHelpers.h>
#include <v8_inspector/src/inspector/v8-log-agent-impl.h>
#include <JsV8InspectorClient.h>
#include "Console.h"

namespace tns {

v8::Local<v8::Object> Console::createConsole(v8::Local<v8::Context> context, const std::string& filesPath) {
    v8::Context::Scope contextScope(context);
    v8::Isolate* isolate = context->GetIsolate();

    v8::Local<v8::Object> console = v8::Object::New(isolate);
    bool success = console->SetPrototype(context, v8::Object::New(isolate)).FromMaybe(false);

    assert(success);

    std::map<std::string, double> timersMap;
    Console::s_isolateToConsoleTimersMap.insert(std::make_pair(context->GetIsolate(), timersMap));

    bindFunctionProperty(context, console, "assert", assertCallback);
    bindFunctionProperty(context, console, "error", errorCallback);
    bindFunctionProperty(context, console, "info", infoCallback);
    bindFunctionProperty(context, console, "log", logCallback);
    bindFunctionProperty(context, console, "warn", warnCallback);
    bindFunctionProperty(context, console, "dir", dirCallback);
    bindFunctionProperty(context, console, "trace", traceCallback);
    bindFunctionProperty(context, console, "time", timeCallback);
    bindFunctionProperty(context, console, "timeEnd", timeEndCallback);

    return console;
}

void Console::sendToADBLogcat(const std::string& message, android_LogPriority logPriority) {
    // split strings into chunks of 4000 characters
    // __android_log_write can't send more than 4000 to the stdout at a time

    auto messageLength = message.length();
    int maxStringLength = 4000;

    if (messageLength < maxStringLength) {
        __android_log_write(logPriority, Console::LOG_TAG, message.c_str());
    } else {
        for (int i = 0; i < messageLength; i += maxStringLength) {
            auto messagePart = message.substr(i, maxStringLength);

            __android_log_write(logPriority, Console::LOG_TAG, messagePart.c_str());
        }
    }
}

void Console::sendToDevToolsFrontEnd(v8::Isolate* isolate, const std::string& message, const std::string& logLevel) {
    if (!JsV8InspectorClient::inspectorIsConnected()) {
        return;
    }

    auto stack = v8::StackTrace::CurrentStackTrace(isolate, 1, v8::StackTrace::StackTraceOptions::kDetailed);

    auto frame = stack->GetFrame(0);

    // will be no-op in non-debuggable builds
    v8_inspector::V8LogAgentImpl::EntryAdded(message, logLevel, ArgConverter::ConvertToString(frame->GetScriptNameOrSourceURL()), frame->GetLineNumber());
}

const std::string buildLogString(const v8::FunctionCallbackInfo<v8::Value>& info, int startingIndex = 0) {
    auto isolate = info.GetIsolate();

    v8::HandleScope scope(isolate);

    std::stringstream ss;

    auto argLen = info.Length();
    if (argLen) {
        for (int i = startingIndex; i < argLen; i++) {
            v8::Local<v8::String> argString;
            if (info[i]->IsFunction()) {
                info[i]->ToDetailString(isolate->GetCurrentContext()).ToLocal(&argString);
            } else if (info[i]->IsObject()) {
                v8::Local<v8::Object> obj = info[i].As<v8::Object>();
                argString = JsonStringifyObject(isolate, obj);
            } else {
                info[i]->ToDetailString(isolate->GetCurrentContext()).ToLocal(&argString);
            }

            // separate args with a space
            if (i != 0) {
                ss << " ";
            }

            ss << ArgConverter::ConvertToString(argString);
        }
    } else {
        ss << std::endl;
    }

    std::string stringResult = ss.str();

    return stringResult;
}

void Console::assertCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    auto isolate = info.GetIsolate();

    auto argLen = info.Length();
    auto expressionPasses = argLen && info[0]->BooleanValue();

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
        sendToDevToolsFrontEnd(isolate, log, "error");
    }
}

void Console::errorCallback(const v8::FunctionCallbackInfo <v8::Value>& info) {
    std::string log = buildLogString(info);

    sendToADBLogcat(log, ANDROID_LOG_ERROR);
    sendToDevToolsFrontEnd(info.GetIsolate(), log, "error");
}

void Console::infoCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    std::string log = buildLogString(info);

    sendToADBLogcat(log, ANDROID_LOG_INFO);
    sendToDevToolsFrontEnd(info.GetIsolate(), log, "info");
}

void Console::logCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    std::string log = buildLogString(info);

    sendToADBLogcat(log, ANDROID_LOG_INFO);
    sendToDevToolsFrontEnd(info.GetIsolate(), log, "info");
}

void Console::warnCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    std::string log = buildLogString(info);

    sendToADBLogcat(log, ANDROID_LOG_WARN);
    sendToDevToolsFrontEnd(info.GetIsolate(), log, "warning");
}

void Console::dirCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();

    v8::HandleScope scope(isolate);

    std::stringstream ss;

    auto argLen = info.Length();
    if (argLen) {
        if (info[0]->IsObject()) {
            ss << "==== object dump start ====" << std::endl;
            v8::Local<v8::Object> argObject = info[0].As<v8::Object>();

            v8::Local<v8::Array> propNames;
            argObject->GetPropertyNames(context).ToLocal(&propNames);

            auto propertiesLen = propNames->Length();
            for (int i = 0; i < propertiesLen; i++) {
                auto propertyName = propNames->Get(context, i).ToLocalChecked();
                auto propertyValue = argObject->Get(context, propertyName).ToLocalChecked();

                auto propIsFunction = propertyValue->IsFunction();

                ss << ArgConverter::ConvertToString(propertyName->ToString(isolate));

                if (propIsFunction) {
                    ss << "()";
                } else if (propertyValue->IsObject()) {
                    std::string jsonStringifiedObject = ArgConverter::ConvertToString(JsonStringifyObject(isolate, propertyValue));
                    // if object prints out as the error string for circular references, replace with #CR instead for brevity
                    if (jsonStringifiedObject.find("circular structure") != std::string::npos) {
                        jsonStringifiedObject = "#CR";
                    }
                    ss << ": " << jsonStringifiedObject;
                } else {
                    ss << ": \"" << ArgConverter::ConvertToString(propertyValue->ToDetailString(isolate)) << "\"";
                }

                ss << std::endl;
            }

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
    sendToDevToolsFrontEnd(isolate, log, "info");
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
        auto frame = stack->GetFrame(i);

        ss << buildStacktraceFrameMessage(frame) << std::endl;
    }

    std::string log = ss.str();
    __android_log_write(ANDROID_LOG_ERROR, LOG_TAG, log.c_str());
    sendToDevToolsFrontEnd(isolate, log, "error");
}

void Console::timeCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
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

    auto nano = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    double timeStamp = nano.time_since_epoch().count();

    it->second.insert(std::make_pair(label, timeStamp));
}

void Console::timeEndCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
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
        sendToDevToolsFrontEnd(isolate, warning, "warning");

        return;
    }

    auto nano = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    double endTimeStamp = nano.time_since_epoch().count();
    double startTimeStamp = itTimersMap->second;

    it->second.erase(label);

    auto diff = endTimeStamp - startTimeStamp;

    std::stringstream ss;
    ss << label << ": " << std::fixed << std::setprecision(2) << diff << "ms" ;
    std::string log = ss.str();

    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, log.c_str());
    sendToDevToolsFrontEnd(isolate, log, "info");
}

const char* Console::LOG_TAG = "JS";
std::map<v8::Isolate*, std::map<std::string, double>> Console::s_isolateToConsoleTimersMap;
}
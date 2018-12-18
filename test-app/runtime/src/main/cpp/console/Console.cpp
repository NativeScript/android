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
#include <NativeScriptException.h>
#include "Console.h"

namespace tns {

v8::Local<v8::Object> Console::createConsole(v8::Local<v8::Context> context, ConsoleCallback callback, const int maxLogcatObjectSize, const bool forceLog) {
    m_callback = callback;
    m_maxLogcatObjectSize = maxLogcatObjectSize;
    m_forceLog = forceLog;
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

void Console::sendToDevToolsFrontEnd(v8::Isolate* isolate, const std::string& message, const std::string& logLevel) {
    if (m_callback != nullptr) {
        m_callback(message, logLevel);
    }
}

const v8::Local<v8::String> transformJSObject(v8::Isolate* isolate, v8::Local<v8::Object> object) {
    auto objToString = object->ToString(isolate);
    v8::Local<v8::String> resultString;

    auto hasCustomToStringImplementation = ArgConverter::ConvertToString(objToString).find("[object Object]") == std::string::npos;

    if (hasCustomToStringImplementation) {
        resultString = objToString;
    } else {
        v8::HandleScope scope(isolate);
        resultString = JsonStringifyObject(isolate, object);
    }

    return resultString;
}

const v8::Local<v8::String> buildStringFromArg(v8::Isolate* isolate, const v8::Local<v8::Value>& val) {
    v8::Local<v8::String> argString;
    if (val->IsFunction()) {
        val->ToDetailString(isolate->GetCurrentContext()).ToLocal(&argString);
    } else if (val->IsArray()) {
        auto cachedSelf = val;
        auto array = val->ToObject(isolate);
        auto arrayEntryKeys = array->GetPropertyNames(isolate->GetCurrentContext()).ToLocalChecked();
        auto context = isolate->GetCurrentContext();

        auto arrayLength = arrayEntryKeys->Length();

        argString = ArgConverter::ConvertToV8String(isolate, "[");

        for (int i = 0; i < arrayLength; i++) {
            auto propertyName = arrayEntryKeys->Get(context, i).ToLocalChecked();

            auto propertyValue = array->Get(context, propertyName).ToLocalChecked();

            // avoid bottomless recursion with cyclic reference to the same array
            if (propertyValue->StrictEquals(cachedSelf)) {
                argString = v8::String::Concat(isolate, argString, ArgConverter::ConvertToV8String(isolate, "[Circular]"));
                continue;
            }

            auto objectString = buildStringFromArg(isolate, propertyValue);

            argString = v8::String::Concat(isolate, argString, objectString);

            if (i != arrayLength - 1) {
                argString = v8::String::Concat(isolate, argString, ArgConverter::ConvertToV8String(isolate, ", "));
            }
        }

        argString = v8::String::Concat(isolate, argString, ArgConverter::ConvertToV8String(isolate, "]"));
    } else if (val->IsObject()) {
        v8::Local<v8::Object> obj = val.As<v8::Object>();

        argString = transformJSObject(isolate, obj);
    } else {
        val->ToDetailString(isolate->GetCurrentContext()).ToLocal(&argString);
    }

    return argString;
}

const std::string buildLogString(const v8::FunctionCallbackInfo<v8::Value>& info, int startingIndex = 0) {
    auto isolate = info.GetIsolate();

    v8::HandleScope scope(isolate);

    std::stringstream ss;

    auto argLen = info.Length();
    if (argLen) {
        for (int i = startingIndex; i < argLen; i++) {
            v8::Local<v8::String> argString;

            argString = buildStringFromArg(isolate, info[i]);

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
    if (!shouldLog()) {
        return;
    }
    try {
        auto isolate = info.GetIsolate();

        auto argLen = info.Length();
        auto context = isolate->GetCurrentContext();
        auto expressionPasses = argLen && info[0]->BooleanValue(context).ToChecked();

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
    if (!shouldLog()) {
        return;
    }
    try {
        std::string log = buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_ERROR);
        sendToDevToolsFrontEnd(info.GetIsolate(), log, "error");
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
    if (!shouldLog()) {
        return;
    }
    try {
        std::string log = buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_INFO);
        sendToDevToolsFrontEnd(info.GetIsolate(), log, "info");
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
    if (!shouldLog()) {
        return;
    }
    try {
        std::string log = buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_INFO);
        sendToDevToolsFrontEnd(info.GetIsolate(), log, "info");
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
    if (!shouldLog()) {
        return;
    }
    try {
        std::string log = buildLogString(info);

        sendToADBLogcat(log, ANDROID_LOG_WARN);
        sendToDevToolsFrontEnd(info.GetIsolate(), log, "warning");
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
    if (!shouldLog()) {
        return;
    }
    try {
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
                    } else if (propertyValue->IsArray()) {
                        auto stringResult = buildStringFromArg(isolate, propertyValue);
                        std::string jsonStringifiedArray = ArgConverter::ConvertToString(stringResult);
                        ss << ": " << jsonStringifiedArray;
                    } else if (propertyValue->IsObject()) {
                        auto obj = propertyValue->ToObject(isolate);
                        auto objString = transformJSObject(isolate, obj);
                        std::string jsonStringifiedObject = ArgConverter::ConvertToString(objString);
                        // if object prints out as the error string for circular references, replace with #CR instead for brevity
                        if (jsonStringifiedObject.find("circular structure") != std::string::npos) {
                            jsonStringifiedObject = "#CR";
                        }
                        ss << ": " << jsonStringifiedObject;
                    } else {
                        ss << ": \"" << ArgConverter::ConvertToString(propertyValue->ToDetailString(context).ToLocalChecked()) << "\"";
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
    if (!shouldLog()) {
        return;
    }
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
        sendToDevToolsFrontEnd(isolate, log, "error");
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
    if (!shouldLog()) {
        return;
    }
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

        auto nano = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
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
    if (!shouldLog()) {
        return;
    }
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

const char* Console::LOG_TAG = "JS";
std::map<v8::Isolate*, std::map<std::string, double>> Console::s_isolateToConsoleTimersMap;
ConsoleCallback Console::m_callback = nullptr;
int Console::m_maxLogcatObjectSize;
bool Console::m_forceLog;

#ifdef APPLICATION_IN_DEBUG
bool Console::isApplicationInDebug = true;
#else
bool Console::isApplicationInDebug = false;
#endif
}
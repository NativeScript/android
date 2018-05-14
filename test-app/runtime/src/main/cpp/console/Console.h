//
// Created by pkanev on 12/8/2017.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include <include/v8.h>
#include <string>
#include <ArgConverter.h>

namespace tns {

typedef void (*ConsoleCallback)(const std::string& message, const std::string& logLevel);

class Console {
    public:
        static v8::Local<v8::Object> createConsole(v8::Local<v8::Context> context, ConsoleCallback callback, const int maxLogcatObjectSize, const bool forceLog);

        static void assertCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void errorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void infoCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void logCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void warnCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void dirCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void traceCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void timeCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void timeEndCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

    private:
        static int m_maxLogcatObjectSize;
        static bool m_forceLog;
        static ConsoleCallback m_callback;
        static const char* LOG_TAG;
        static std::map<v8::Isolate*, std::map<std::string, double>> s_isolateToConsoleTimersMap;

        static bool isApplicationInDebug;
        static bool shouldLog() {
            return m_forceLog || isApplicationInDebug;
        }

        // heavily inspired by 'createBoundFunctionProperty' of V8's v8-console.h
        static void bindFunctionProperty(v8::Local<v8::Context> context,
                                         v8::Local<v8::Object> consoleInstance,
                                         const std::string& functionName,
                                         v8::FunctionCallback callback) {
            v8::Local<v8::String> funcName = ArgConverter::ConvertToV8String(context->GetIsolate(), functionName);

            v8::Local<v8::Function> func;
            if (!v8::Function::New(context, callback, consoleInstance, 0, v8::ConstructorBehavior::kThrow).ToLocal(&func)) {
                return;
            }

            func->SetName(funcName);

            consoleInstance->CreateDataProperty(context, funcName, func);
        }

        static void sendToADBLogcat(const std::string& log, android_LogPriority logPriority);
        static void sendToDevToolsFrontEnd(v8::Isolate* isolate, const std::string& message, const std::string& logLevel);
};

}

#endif //CONSOLE_H

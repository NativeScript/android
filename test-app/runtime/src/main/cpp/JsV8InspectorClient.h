#ifndef JSV8INSPECTORCLIENT_H_
#define JSV8INSPECTORCLIENT_H_

#include <string>
#include <vector>
#include <src/inspector/v8-console-message.h>
#include "v8.h"
#include "JEnv.h"
#include "v8-inspector.h"
#include "v8_inspector/ns-v8-tracing-agent-impl.h"

using namespace v8_inspector;

namespace tns {
class JsV8InspectorClient : V8InspectorClient, v8_inspector::V8Inspector::Channel {
    public:
        static JsV8InspectorClient* GetInstance();

        void init();
        void connect(jobject connection);
        void scheduleBreak();
        void disconnect();
        void dispatchMessage(const std::string& message);

        void registerModules();

        // Overrides of V8Inspector::Channel
        void sendResponse(int callId, std::unique_ptr<StringBuffer> message) override;
        void sendNotification(const std::unique_ptr<StringBuffer> message) override;
        void flushProtocolNotifications() override;

        static void sendToFrontEndCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void consoleLogCallback(v8::Isolate* isolate, ConsoleAPIType method, const std::vector<v8::Local<v8::Value>>& args);
        static void inspectorSendEventCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void registerDomainDispatcherCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void inspectorTimestampCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        // Overrides of V8InspectorClient
        void runMessageLoopOnPause(int context_group_id) override;
        void quitMessageLoopOnPause() override;

        static bool inspectorIsConnected() {
            return JsV8InspectorClient::GetInstance()->isConnected_;
        }

        static std::map<std::string, v8::Persistent<v8::Object>*> Domains;

    private:
        JsV8InspectorClient(v8::Isolate* isolate);

        // Override of V8InspectorClient
        v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

        void createInspectorSession();
        void doDispatchMessage(const std::string& message);

        static void InspectorIsConnectedGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static JsV8InspectorClient* instance;
        static constexpr int contextGroupId = 1;

        std::unique_ptr<tns::inspector::TracingAgentImpl> tracing_agent_;
        v8::Isolate* isolate_;
        std::unique_ptr<V8Inspector> inspector_;
        v8::Persistent<v8::Context> context_;
        std::unique_ptr<V8InspectorSession> session_;
        jclass inspectorClass_;
        jmethodID sendMethod_;
        jmethodID getInspectorMessageMethod_;
        jmethodID sendToDevToolsConsoleMethod_;
        jobject connection_;
        bool running_nested_loop_ : 1;
        bool terminated_ : 1;
        bool isConnected_ : 1;


    // {N} specific helpers
    bool CallDomainHandlerFunction(v8::Local<v8::Context> context,
                                   v8::Local<v8::Function> domainMethodFunc,
                                   const v8::Local<v8::Object>& arg,
                                   v8::Local<v8::Object>& domainDebugger,
                                   v8::Local<v8::Value>& result);
    std::string GetReturnMessageFromDomainHandlerResult(const v8::Local<v8::Value>& result, const v8::Local<v8::Value>& requestId);
};
}

#endif /* JSV8INSPECTORCLIENT_H_ */

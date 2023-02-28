#ifndef JSV8INSPECTORCLIENT_H_
#define JSV8INSPECTORCLIENT_H_

#include <string>
#include "v8.h"
#include "JEnv.h"
#include "src/inspector/v8-inspector-impl.h"
#include "src/inspector/v8-inspector-session-impl.h"
#include "v8-inspector.h"
#include "src/inspector/protocol/Forward.h"
#include "src/inspector/string-16.h"

using namespace v8_inspector;

namespace tns {
class JsV8InspectorClient : V8InspectorClient, v8_inspector::V8Inspector::Channel {
    public:
        static JsV8InspectorClient* GetInstance();

        template <class TypeName>
        static v8::Local<TypeName> PersistentToLocal(v8::Isolate* isolate, const v8::Persistent<TypeName>& persistent);

        void init();
        void connect(jobject connection);
        void scheduleBreak();
        void disconnect();
        void dispatchMessage(const std::string& message);

        // Overrides of V8Inspector::Channel
        void sendResponse(int callId, std::unique_ptr<StringBuffer> message) override;
        void sendNotification(const std::unique_ptr<StringBuffer> message) override;
        void flushProtocolNotifications() override;

        static void sendToFrontEndCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void consoleLogCallback(v8::Isolate* isolate, const std::string& message, const std::string& logLevel);

        // Overrides of V8InspectorClient
        void runMessageLoopOnPause(int context_group_id) override;
        void quitMessageLoopOnPause() override;

        static void attachInspectorCallbacks(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate>& globalObjectTemplate);
        static bool inspectorIsConnected() {
            return JsV8InspectorClient::GetInstance()->isConnected_;
        }

    private:
        JsV8InspectorClient(v8::Isolate* isolate);

        // Override of V8InspectorClient
        v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

        void createInspectorSession();
        void doDispatchMessage(const std::string& message);

        static void InspectorIsConnectedGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static JsV8InspectorClient* instance;
        static constexpr int contextGroupId = 1;

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
};
}

#endif /* JSV8INSPECTORCLIENT_H_ */

#ifndef JSV8INSPECTORCLIENT_H_
#define JSV8INSPECTORCLIENT_H_

#include <string>
#include "v8.h"
#include "v8-debug.h"
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
        void createInspectorSession(v8::Isolate* isolate, const v8::Local<v8::Context>& context);
        void disconnect();
        void dispatchMessage(const std::string& message);
        void doDispatchMessage(v8::Isolate* isolate, const std::string& message);

        void sendProtocolResponse(int callId, const v8_inspector::StringView& message) override;
        void sendProtocolNotification(const v8_inspector::StringView& message) override;
        void flushProtocolNotifications() override;

        static void sendToFrontEndCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void runMessageLoopOnPause(int context_group_id) override;
        void quitMessageLoopOnPause() override;
        v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

        static void attachInspectorCallbacks(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate>& globalObjectTemplate);
        static void InspectorIsConnectedGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        std::unique_ptr<V8Inspector> inspector_;
        v8::Isolate* isolate_;
        bool isConnected;

    private:
        JsV8InspectorClient(v8::Isolate* isolate);

        static JsV8InspectorClient* instance;
        static jclass inspectorClass;
        static jmethodID sendMethod;
        static jmethodID getInspectorMessageMethod;
        static jmethodID sendToDevToolsConsoleMethod;

        v8::Persistent<v8::Context> context_;
        std::unique_ptr<V8InspectorSession> session_;
        jobject connection;
        bool running_nested_loop_;
        bool terminated_;
};
}

#endif /* JSV8INSPECTORCLIENT_H_ */

#ifndef JSV8INSPECTORCLIENT_H_
#define JSV8INSPECTORCLIENT_H_

#include <string>
#include <v8_inspector/src/inspector/task-runner.h>
#include "v8.h"
#include "v8-debug.h"
#include "JEnv.h"
#include "src/inspector/v8-inspector-impl.h"
#include "src/inspector/v8-inspector-session-impl.h"
#include "v8-inspector.h"
#include "src/inspector/protocol/Forward.h"
#include "src/inspector/string-16.h"


using namespace v8_inspector;

namespace tns
{
	class JsV8InspectorClient : V8InspectorClient, v8_inspector::V8Inspector::Channel
	{
		public:
            static JsV8InspectorClient* GetInstance();

            void init();
			void connect(jobject connection);
			void doConnect(v8::Isolate *isolate, const v8::Local<v8::Context>& context);
			void disconnect();
			void dispatchMessage(const std::string& message);
			void doDispatchMessage(v8::Isolate *isolate, const std::string& message);

			//void sendProtocolResponse(int callId, const String16& message) override;
            //void sendProtocolNotification(const String16& message) override;
            //void flushProtocolNotifications() override;

			void sendProtocolResponse(int callId, const v8_inspector::StringView &message) override;
			void sendProtocolNotification(const v8_inspector::StringView &message) override;
			void flushProtocolNotifications() override;

		private:
            JsV8InspectorClient(v8::Isolate *isolate);
			~JsV8InspectorClient();

            static JsV8InspectorClient* instance;
            static jclass inspectorClass;
            static jmethodID sendMethod;

			TaskRunner* backend_runner;
            v8::Isolate* isolate_;
            std::unique_ptr<V8Inspector> inspector_;
            std::unique_ptr<V8InspectorSession> session_;
            jobject connection;
	};
}

#endif /* JSV8INSPECTORCLIENT_H_ */

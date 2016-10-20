#ifndef JSV8INSPECTORCLIENT_H_
#define JSV8INSPECTORCLIENT_H_

#include <string>
#include "v8.h"
#include "v8-debug.h"
#include "JEnv.h"
#include "v8_inspector/platform/v8_inspector/public/V8InspectorClient.h"
#include "v8_inspector/platform/v8_inspector/public/V8InspectorSession.h"
#include "v8_inspector/platform/v8_inspector/public/V8Inspector.h"

using namespace v8_inspector;


namespace tns
{
	class JsV8InspectorClient : V8InspectorClient, blink::protocol::FrontendChannel
	{
		public:
            JsV8InspectorClient(v8::Isolate *isolate);

			void connect();

			void disconnect();

            void dispatchMessage(const std::string& message);

            void sendProtocolResponse(int callId, const String16& message) override;
            void sendProtocolNotification(const String16& message) override;
            void flushProtocolNotifications() override;

		private:
            v8::Isolate* isolate_;
            std::unique_ptr<V8Inspector> inspector_;
            std::unique_ptr<V8InspectorSession> session_;
	};
}

#endif /* JSV8INSPECTORCLIENT_H_ */

#include "JsV8InspectorClient.h"
//#include "V8GlobalHelpers.h"
//#include "ArgConverter.h"
//#include "JniLocalRef.h"
//#include "NativeScriptException.h"
//#include "NativeScriptAssert.h"
#include <sstream>
#include <assert.h>
#include "v8_inspector/platform/v8_inspector/String16STL.h"

using namespace std;
using namespace tns;
using namespace v8;

using namespace v8_inspector;

JsV8InspectorClient::JsV8InspectorClient(v8::Isolate *isolate)
{
    isolate_ = isolate;
    inspector_ = V8Inspector::create(isolate, this);
    inspector_->contextCreated(v8_inspector::V8ContextInfo(isolate_->GetCurrentContext(), 1, "{N} Context"));

}

void JsV8InspectorClient::connect()
{
    session_ = inspector_->connect(1, this, nullptr);
}

void JsV8InspectorClient::disconnect()
{
    session_.reset();
}

void JsV8InspectorClient::dispatchMessage(const std::string& message)
{
    assert(session_ != nullptr);
    const String16 protocolMessage(message.c_str());
    session_->dispatchProtocolMessage(protocolMessage);
}

void JsV8InspectorClient::sendProtocolResponse(int callId, const String16& message)
{
    sendProtocolNotification(message);
}

void JsV8InspectorClient::sendProtocolNotification(const String16& message)
{
    //Java Interop. send through websocket connection
}

void JsV8InspectorClient::flushProtocolNotifications()
{
}
#include "JsV8InspectorClient.h"
//#include "V8GlobalHelpers.h"
//#include "ArgConverter.h"
//#include "JniLocalRef.h"
//#include "NativeScriptException.h"
//#include "NativeScriptAssert.h"
#include <sstream>
#include <assert.h>
#include "Runtime.h"
#include "src/inspector/string-16.h"

using namespace std;
using namespace tns;
using namespace v8;

using namespace v8_inspector;

JsV8InspectorClient::JsV8InspectorClient(v8::Isolate *isolate)
        : isolate_(nullptr),
          inspector_(nullptr),
          session_(nullptr),
          connection(nullptr)
{
    isolate_ = isolate;

    JEnv env;

    inspectorClass = env.FindClass("com/tns/AndroidJsV8Inspector");
    assert(inspectorClass != nullptr);

    sendMethod = env.GetStaticMethodID(inspectorClass, "send", "(Ljava/lang/Object;Ljava/lang/String;)V");
    assert(sendMethod != nullptr);
}

void JsV8InspectorClient::connect(jobject connection)
{
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    //session_ = inspector_->connect(1, this, nullptr);

    JEnv env;
    this->connection = env.NewGlobalRef(connection);
}

void JsV8InspectorClient::disconnect()
{
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    session_.reset();

    JEnv env;
    env.DeleteGlobalRef(this->connection);
    this->connection = nullptr;
}

void JsV8InspectorClient::dispatchMessage(const std::string &message)
{
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    assert(session_ != nullptr);
    //const String16 protocolMessage(message.c_str());
    //session_->dispatchProtocolMessage(protocolMessage);
}

//void JsV8InspectorClient::sendProtocolResponse(int callId, const String16 &message)
//{
//    sendProtocolNotification(message);
//}
//
//void JsV8InspectorClient::sendProtocolNotification(const String16 &message)
//{
//    if (inspectorClass == nullptr || this->connection == nullptr)
//    {
//        return;
//    }
//
//
//    JEnv env;
//    JniLocalRef string(env.NewStringUTF(message.utf8().c_str()));
//    env.CallStaticVoidMethod(inspectorClass, sendMethod,  this->connection, (jstring) string);
//}
//
//void JsV8InspectorClient::flushProtocolNotifications()
//{
//}

void JsV8InspectorClient::init()
{
    if (inspector_ != nullptr)
    {
        return;
    }

    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    inspector_ = V8Inspector::create(isolate_, this);
    //inspector_->contextCreated(v8_inspector::V8ContextInfo(isolate_->GetCurrentContext(), 1, "{N} Context"));
}

JsV8InspectorClient *JsV8InspectorClient::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new JsV8InspectorClient(Runtime::GetRuntime(0)->GetIsolate());
    }

    return instance;
}

JsV8InspectorClient* JsV8InspectorClient::instance = nullptr;
jclass JsV8InspectorClient::inspectorClass = nullptr;
jmethodID JsV8InspectorClient::sendMethod = nullptr;




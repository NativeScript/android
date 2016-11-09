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

#include "src/inspector/task-runner.h"

using namespace std;
using namespace tns;
using namespace v8;

using namespace v8_inspector;

class ConnectTask : public TaskRunner::Task
{
public:
    ConnectTask(JsV8InspectorClient* client, v8::base_copied::Semaphore  *ready_semaphore)
            : client_(client), ready_semaphore_(ready_semaphore)
    {

    }

    ~ConnectTask() = default;

    bool is_inspector_task() final
    {
        return true;
    }

    void Run(v8::Isolate *isolate, const v8::Global<v8::Context> &global_context)
    {
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = global_context.Get(isolate);
        client_->doConnect(isolate, context);
        if (ready_semaphore_ != nullptr)
        {
            ready_semaphore_->Signal();
        }
    }

private:
    JsV8InspectorClient* client_;
    v8::base_copied::Semaphore *ready_semaphore_;
};


class SendMessageToBackendTask : public TaskRunner::Task
{
public:
    explicit SendMessageToBackendTask(JsV8InspectorClient* client, const std::string &message)
            : client_(client), message_(message)
    {

    }

    bool is_inspector_task() final
    {
        return true;
    }

    void Run(v8::Isolate *isolate, const v8::Global<v8::Context> &global_context) override
    {
//        v8_inspector::V8InspectorSession *session = nullptr;
//        {
//            v8::HandleScope handle_scope(isolate);
//            v8::Local<v8::Context> context = global_context.Get(isolate);
//            session = InspectorClientImpl::SessionFromContext(context);
//            CHECK(session);
//        }
//
//        v8_inspector::StringView message_view(reinterpret_cast<const uint16_t *>(message_.characters16()), message_.length());
//        session->dispatchProtocolMessage(message_view);

        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = global_context.Get(isolate);
        client_->doDispatchMessage(isolate, message_);
    }

private:
    JsV8InspectorClient* client_;
    std::string message_;
};

JsV8InspectorClient::JsV8InspectorClient(v8::Isolate *isolate)
        : isolate_(isolate),
          inspector_(nullptr),
          session_(nullptr),
          connection(nullptr),
          backend_runner(nullptr)
{
    JEnv env;


    inspectorClass = env.FindClass("com/tns/AndroidJsV8Inspector");
    assert(inspectorClass != nullptr);

    sendMethod = env.GetStaticMethodID(inspectorClass, "send", "(Ljava/lang/Object;Ljava/lang/String;)V");
    assert(sendMethod != nullptr);
}

JsV8InspectorClient::~JsV8InspectorClient()
{
    if (backend_runner != nullptr)
    {
        delete backend_runner;
    }
}

void JsV8InspectorClient::connect(jobject connection)
{
    //Isolate::Scope isolate_scope(isolate_);
    //v8::HandleScope handleScope(isolate_);

    v8::base_copied::Semaphore ready_semaphore(0);

    JEnv env;
    this->connection = env.NewGlobalRef(connection);

    this->backend_runner->Append(new ConnectTask(this, &ready_semaphore));

    ready_semaphore.Wait();



    //inspector_ = V8Inspector::create(isolate_, this);

    //session_ = inspector_->connect(1, this, v8_inspector::StringView());

    //inspector_->contextCreated(v8_inspector::V8ContextInfo(isolate_->GetCurrentContext(), 1, v8_inspector::StringView()));


}

void JsV8InspectorClient::doConnect(v8::Isolate *isolate, const v8::Local<v8::Context>& context)
{
//    Isolate::Scope isolate_scope(isolate_);
//    v8::HandleScope handleScope(isolate_);


    inspector_ = V8Inspector::create(isolate, this);

    session_ = inspector_->connect(1, this, v8_inspector::StringView());

    inspector_->contextCreated(v8_inspector::V8ContextInfo(/*isolate->GetCurrentContext()*/ context, 1, v8_inspector::StringView()));
}

void JsV8InspectorClient::disconnect()
{
    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    session_.reset();

    JEnv env;
    env.DeleteGlobalRef(this->connection);
    this->connection = nullptr;
}


void JsV8InspectorClient::dispatchMessage(const std::string &message)
{
    //Isolate::Scope isolate_scope(isolate_);
    //v8::HandleScope handleScope(isolate_);

//    assert(session_ != nullptr);
//
//    const String16 msg(message.c_str());
//    v8_inspector::StringView message_view(reinterpret_cast<const uint16_t *>(msg.characters16()), msg.length());
//    session_->dispatchProtocolMessage(message_view);

    this->backend_runner->Append(new SendMessageToBackendTask(this, message));

}

void JsV8InspectorClient::doDispatchMessage(v8::Isolate *isolate, const std::string &message)
{
    //Isolate::Scope isolate_scope(isolate_);
    //v8::HandleScope handleScope(isolate_);

    assert(session_ != nullptr);

    const String16 msg(message.c_str());
    v8_inspector::StringView message_view(reinterpret_cast<const uint16_t *>(msg.characters16()), msg.length());
    session_->dispatchProtocolMessage(message_view);
}

void JsV8InspectorClient::sendProtocolResponse(int callId, const v8_inspector::StringView &message)
{
    //frontend_channel_->SendMessageToFrontend(message);
    sendProtocolNotification(message);
}

static v8_inspector::String16 ToString16(const v8_inspector::StringView &string)
{
    if (string.is8Bit())
    {
        return v8_inspector::String16(reinterpret_cast<const char *>(string.characters8()), string.length());
    }

    return v8_inspector::String16(reinterpret_cast<const uint16_t *>(string.characters16()), string.length());
}

void JsV8InspectorClient::sendProtocolNotification(const v8_inspector::StringView &message)
{
    //frontend_channel_->SendMessageToFrontend(message);

    if (inspectorClass == nullptr || this->connection == nullptr)
    {
        return;
    }

    v8_inspector::String16 msg = ToString16(message);

    JEnv env;
    JniLocalRef string(env.NewStringUTF(msg.utf8().c_str()));
    env.CallStaticVoidMethod(inspectorClass, sendMethod,  this->connection, (jstring) string);
}

void JsV8InspectorClient::flushProtocolNotifications()
{
}

void MessageHandler(v8::Local<v8::Message> message,
                    v8::Local<v8::Value> exception)
{
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = isolate->GetEnteredContext();
//    if (context.IsEmpty()) return;
//    v8_inspector::V8Inspector *inspector = InspectorClientImpl::InspectorFromContext(context);
//
//    v8::Local<v8::StackTrace> stack = message->GetStackTrace();
//    int script_id = message->GetScriptOrigin().ScriptID()->Value();
//    if (!stack.IsEmpty() && stack->GetFrameCount() > 0)
//    {
//        int top_script_id = stack->GetFrame(0)->GetScriptId();
//        if (top_script_id == script_id) script_id = 0;
//    }
//    int line_number = message->GetLineNumber(context).FromMaybe(0);
//    int column_number = 0;
//    if (message->GetStartColumn(context).IsJust())
//        column_number = message->GetStartColumn(context).FromJust() + 1;
//
//    v8_inspector::StringView detailed_message;
//    v8_inspector::String16 message_text_string = ToString16(message->Get());
//    v8_inspector::StringView message_text(message_text_string.characters16(),
//                                          message_text_string.length());
//    v8_inspector::String16 url_string;
//    if (message->GetScriptOrigin().ResourceName()->IsString())
//    {
//        url_string =
//                ToString16(message->GetScriptOrigin().ResourceName().As<v8::String>());
//    }
//    v8_inspector::StringView url(url_string.characters16(), url_string.length());
//
//    inspector->exceptionThrown(context, message_text, exception, detailed_message,
//                               url, line_number, column_number,
//                               inspector->createStackTrace(stack), script_id);
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

    //SendMessageToBackendExtension send_message_to_backend_extension;
    //v8::RegisterExtension(&send_message_to_backend_extension);

    v8::base_copied::Semaphore ready_semaphore(0);

    //const char *backend_extensions[0] = {"v8_inspector/setTimeout"};
    //v8::ExtensionConfiguration backend_configuration(arraysize(backend_extensions), backend_extensions);

    this->backend_runner = new TaskRunner(/*&backend_configuration*/ nullptr, false, &ready_semaphore);
    ready_semaphore.Wait();

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




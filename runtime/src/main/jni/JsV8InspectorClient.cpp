#include "JsV8InspectorClient.h"
//#include "V8GlobalHelpers.h"
//#include "ArgConverter.h"
//#include "JniLocalRef.h"
//#include "NativeScriptException.h"
//#include "NativeScriptAssert.h"
#include <sstream>
#include <assert.h>
#include <include/libplatform/libplatform.h>
#include "Runtime.h"
#include "NativeScriptException.h"

#include "ArgConverter.h"

using namespace std;
using namespace tns;
using namespace v8;

using namespace v8_inspector;

JsV8InspectorClient::JsV8InspectorClient(v8::Isolate *isolate)
        : isolate_(isolate),
          inspector_(nullptr),
          session_(nullptr),
          connection(nullptr),
          context_(),
          running_nested_loop_(false)
{
    JEnv env;

    inspectorClass = env.FindClass("com/tns/AndroidJsV8Inspector");
    assert(inspectorClass != nullptr);

    sendMethod = env.GetStaticMethodID(inspectorClass, "send", "(Ljava/lang/Object;Ljava/lang/String;)V");
    assert(sendMethod != nullptr);

    sendToDevToolsConsoleMethod = env.GetStaticMethodID(inspectorClass, "sendToDevToolsConsole", "(Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;)V");
    assert(sendToDevToolsConsoleMethod != nullptr);

    getInspectorMessageMethod = env.GetStaticMethodID(inspectorClass, "getInspectorMessage", "(Ljava/lang/Object;)Ljava/lang/String;");
    assert(getInspectorMessageMethod != nullptr);
}

void JsV8InspectorClient::connect(jobject connection)
{
    JEnv env;
    this->connection = env.NewGlobalRef(connection);
}

void JsV8InspectorClient::createInspectorSession(v8::Isolate *isolate, const v8::Local<v8::Context> &context)
{
    session_ = inspector_->connect(0, this, v8_inspector::StringView());
}

void JsV8InspectorClient::disconnect()
{
    if (this->connection == nullptr)
    {
        return;
    }

    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    session_->resume();
    session_.reset();

    JEnv env;
    env.DeleteGlobalRef(this->connection);
    this->connection = nullptr;

    this->createInspectorSession(isolate_, JsV8InspectorClient::PersistentToLocal(isolate_, context_));
}


void JsV8InspectorClient::dispatchMessage(const std::string &message)
{
    this->doDispatchMessage(isolate_, message);

}

void JsV8InspectorClient::runMessageLoopOnPause(int context_group_id)
{
    if (running_nested_loop_)
    {
        return;
    }

    JEnv env;

    terminated_ = false;
    running_nested_loop_ = true;
    while (!terminated_)
    {
        JniLocalRef msg(env.CallStaticObjectMethod(inspectorClass, getInspectorMessageMethod, this->connection));
        if (!msg.IsNull())
        {
            auto inspectorMessage = ArgConverter::jstringToString(msg);
            this->doDispatchMessage(this->isolate_, inspectorMessage);
        }

        while (v8::platform::PumpMessageLoop(Runtime::platform, isolate_))
        {
        }
    }
    terminated_ = false;
    running_nested_loop_ = false;
}

void JsV8InspectorClient::quitMessageLoopOnPause()
{
    terminated_ = true;
}

v8::Local<v8::Context> JsV8InspectorClient::ensureDefaultContextInGroup(int contextGroupId)
{
    v8::Local<v8::Context> context = PersistentToLocal(isolate_, context_);
    return context;
}

void JsV8InspectorClient::doDispatchMessage(v8::Isolate *isolate, const std::string &message)
{
    if (session_ == nullptr)
    {
        return;
    }

    const String16 msg(message.c_str());
    v8_inspector::StringView message_view(reinterpret_cast<const uint16_t *>(msg.characters16()), msg.length());
    session_->dispatchProtocolMessage(message_view);
}

void JsV8InspectorClient::sendProtocolResponse(int callId, const v8_inspector::StringView &message)
{
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
    if (inspectorClass == nullptr || this->connection == nullptr)
    {
        return;
    }

    v8_inspector::String16 msg = ToString16(message);

    JEnv env;
    const char *msss = msg.utf8().c_str();
    JniLocalRef str(env.NewStringUTF(msg.utf8().c_str()));
    env.CallStaticVoidMethod(inspectorClass, sendMethod, this->connection, (jstring) str);
}

void JsV8InspectorClient::flushProtocolNotifications()
{
}

template<class TypeName>
inline v8::Local<TypeName> StrongPersistentToLocal(const v8::Persistent<TypeName> &persistent)
{
    return *reinterpret_cast<v8::Local<TypeName> *>(const_cast<v8::Persistent<TypeName> *>(&persistent));
}

template<class TypeName>
inline v8::Local<TypeName> WeakPersistentToLocal(v8::Isolate *isolate, const v8::Persistent<TypeName> &persistent)
{
    return v8::Local<TypeName>::New(isolate, persistent);
}

template<class TypeName>
inline v8::Local<TypeName> JsV8InspectorClient::PersistentToLocal(v8::Isolate *isolate, const v8::Persistent<TypeName> &persistent)
{
    if (persistent.IsWeak())
    {
        return WeakPersistentToLocal(isolate, persistent);
    }
    else
    {
        return StrongPersistentToLocal(persistent);
    }
}

void JsV8InspectorClient::init()
{
    if (inspector_ != nullptr)
    {
        return;
    }

    v8::HandleScope handle_scope(isolate_);

    v8::Local<Context> context = Context::New(isolate_);
    v8::Context::Scope context_scope(context);

    inspector_ = V8Inspector::create(isolate_, this);

    inspector_->contextCreated(v8_inspector::V8ContextInfo(context, 0, v8_inspector::StringView()));

    v8::Persistent<v8::Context> persistentContext(context->GetIsolate(), context);
    context_.Reset(isolate_, persistentContext);

    this->createInspectorSession(isolate_, JsV8InspectorClient::PersistentToLocal(isolate_, context_));
}

JsV8InspectorClient *JsV8InspectorClient::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new JsV8InspectorClient(Runtime::GetRuntime(0)->GetIsolate());
    }

    return instance;
}


void JsV8InspectorClient::sendToFrontEndCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {

    if(instance->connection == nullptr) {
        return;
    }

    try
    {
        if ((args.Length() > 0) && args[0]->IsString())
        {
            std::string message = ArgConverter::ConvertToString(args[0]->ToString());

            std:string level = "log";
            if (args.Length() > 1  && args[1]->IsString())
            {
                level = ArgConverter::ConvertToString(args[1]->ToString());
            }

            JEnv env;
            JniLocalRef str(env.NewStringUTF(message.c_str()));
            JniLocalRef lev(env.NewStringUTF(level.c_str()));
            env.CallStaticVoidMethod(inspectorClass, sendToDevToolsConsoleMethod, instance->connection, (jstring) str, (jstring)lev);
        }
    }
    catch (NativeScriptException& e)
    {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MessageHandler(v8::Local<v8::Message> message, v8::Local<v8::Value> exception)
{
//    v8::Isolate *isolate = v8::Isolate::GetCurrent();
//    v8::Local<v8::Context> context = isolate->GetEnteredContext();
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

JsV8InspectorClient *JsV8InspectorClient::instance = nullptr;
jclass JsV8InspectorClient::inspectorClass = nullptr;
jmethodID JsV8InspectorClient::sendMethod = nullptr;
jmethodID JsV8InspectorClient::sendToDevToolsConsoleMethod = nullptr;
jmethodID JsV8InspectorClient::getInspectorMessageMethod = nullptr;




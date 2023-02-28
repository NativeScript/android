#include "JsV8InspectorClient.h"
#include <assert.h>
#include <include/libplatform/libplatform.h>
#include "Runtime.h"
#include "NativeScriptException.h"

#include "ArgConverter.h"
#include "DOMDomainCallbackHandlers.h"
#include "LogAgentImpl.h"
#include "NetworkDomainCallbackHandlers.h"

using namespace std;
using namespace tns;
using namespace v8;

using namespace v8_inspector;

// Utility functions for converting between inspector StringView and UTF8 string

static inline v8_inspector::StringView stringToStringView(const std::string &str) {
    auto* chars = reinterpret_cast<const uint8_t*>(str.c_str());
    return { chars, str.length() };
}

static inline std::string stringViewToString(v8::Isolate* isolate, const v8_inspector::StringView& stringView) {
    int length = static_cast<int>(stringView.length());
    if (!length) {
        return "";
    }
    v8::Local<v8::String> message = (
        stringView.is8Bit() ?
            v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t*>(stringView.characters8()), v8::NewStringType::kNormal, length) :
            v8::String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t*>(stringView.characters16()), v8::NewStringType::kNormal, length)
    ) .ToLocalChecked();
    v8::String::Utf8Value result(isolate, message);
    return *result;
}

JsV8InspectorClient::JsV8InspectorClient(v8::Isolate* isolate)
    : isolate_(isolate),
      inspector_(nullptr),
      session_(nullptr),
      connection_(nullptr),
      context_(),
      terminated_(true),
      running_nested_loop_(false),
      isConnected_(false) {
    JEnv env;

    inspectorClass_ = env.FindClass("com/tns/AndroidJsV8Inspector");
    assert(inspectorClass_ != nullptr);

    sendMethod_ = env.GetStaticMethodID(inspectorClass_, "send", "(Ljava/lang/Object;Ljava/lang/String;)V");
    assert(sendMethod_ != nullptr);

    sendToDevToolsConsoleMethod_ = env.GetStaticMethodID(inspectorClass_, "sendToDevToolsConsole", "(Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;)V");
    assert(sendToDevToolsConsoleMethod_ != nullptr);

    getInspectorMessageMethod_ = env.GetStaticMethodID(inspectorClass_, "getInspectorMessage", "(Ljava/lang/Object;)Ljava/lang/String;");
    assert(getInspectorMessageMethod_ != nullptr);
}

void JsV8InspectorClient::connect(jobject connection) {
    JEnv env;
    connection_ = env.NewGlobalRef(connection);
    isConnected_ = true;
}

void JsV8InspectorClient::scheduleBreak() {
    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);
    auto context = Runtime::GetRuntime(isolate_)->GetContext();
    Context::Scope context_scope(context);

    session_->schedulePauseOnNextStatement({}, {});
}

void JsV8InspectorClient::createInspectorSession() {
    session_ = inspector_->connect(JsV8InspectorClient::contextGroupId, this, {});
}

void JsV8InspectorClient::disconnect() {
    if (connection_ == nullptr) {
        return;
    }

    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    session_->resume();
    session_.reset();

    JEnv env;
    env.DeleteGlobalRef(connection_);
    connection_ = nullptr;
    isConnected_ = false;

    createInspectorSession();
}


void JsV8InspectorClient::dispatchMessage(const std::string& message) {
    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);
    auto context = Runtime::GetRuntime(isolate_)->GetContext();
    Context::Scope context_scope(context);

    doDispatchMessage(message);
}

void JsV8InspectorClient::runMessageLoopOnPause(int context_group_id) {
    if (running_nested_loop_) {
        return;
    }

    JEnv env;

    terminated_ = false;
    running_nested_loop_ = true;
    while (!terminated_) {
        JniLocalRef msg(env.CallStaticObjectMethod(inspectorClass_, getInspectorMessageMethod_, connection_));
        if (!msg.IsNull()) {
            auto inspectorMessage = ArgConverter::jstringToString(msg);
            doDispatchMessage(inspectorMessage);
        }

        while (v8::platform::PumpMessageLoop(Runtime::platform, isolate_)) {
        }
    }
    terminated_ = false;
    running_nested_loop_ = false;
}

void JsV8InspectorClient::quitMessageLoopOnPause() {
    terminated_ = true;
}

v8::Local<v8::Context> JsV8InspectorClient::ensureDefaultContextInGroup(int contextGroupId) {
    return context_.Get(isolate_);
}

void JsV8InspectorClient::doDispatchMessage(const std::string& message) {
    if (session_ == nullptr) {
        return;
    }

    v8_inspector::StringView message_view = stringToStringView(message);
    session_->dispatchProtocolMessage(message_view);
}

void JsV8InspectorClient::sendResponse(int callId, std::unique_ptr<StringBuffer> message) {
    sendNotification(std::move(message));
}

void JsV8InspectorClient::sendNotification(std::unique_ptr<StringBuffer> message) {
    if (connection_ == nullptr) {
        return;
    }

    const std::string msg = stringViewToString(isolate_, message->string());

    JEnv env;
    // TODO: Pete: Check if we can use a wide (utf 16) string here
    JniLocalRef str(env.NewStringUTF(msg.c_str()));
    env.CallStaticVoidMethod(inspectorClass_, sendMethod_, connection_, (jstring) str);
}

void JsV8InspectorClient::flushProtocolNotifications() {
}

void JsV8InspectorClient::init() {
    if (inspector_ != nullptr) {
        return;
    }

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);
    v8::Local<Context> context = Runtime::GetRuntime(isolate_)->GetContext();

    inspector_ = V8Inspector::create(isolate_, this);

    inspector_->contextCreated(v8_inspector::V8ContextInfo(context, JsV8InspectorClient::contextGroupId, {}));

    context_.Reset(isolate_, context);

    createInspectorSession();
}

JsV8InspectorClient* JsV8InspectorClient::GetInstance() {
    if (instance == nullptr) {
        instance = new JsV8InspectorClient(Runtime::GetRuntime(0)->GetIsolate());
    }

    return instance;
}

void JsV8InspectorClient::sendToFrontEndCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if ((instance == nullptr) || (instance->connection_ == nullptr)) {
        return;
    }

    try {
        auto isolate = args.GetIsolate();
        if ((args.Length() > 0) && args[0]->IsString()) {
            auto context = isolate->GetCurrentContext();
            std::string message = ArgConverter::ConvertToString(args[0]->ToString(context).ToLocalChecked());

            std::string level = "log";
            if (args.Length() > 1  && args[1]->IsString()) {
                level = ArgConverter::ConvertToString(args[1]->ToString(context).ToLocalChecked());
            }

            JEnv env;
            JniLocalRef str(env.NewStringUTF(message.c_str()));
            JniLocalRef lev(env.NewStringUTF(level.c_str()));
            env.CallStaticVoidMethod(instance->inspectorClass_, instance->sendToDevToolsConsoleMethod_, instance->connection_, (jstring) str, (jstring)lev);
        }
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void JsV8InspectorClient::consoleLogCallback(Isolate* isolate, const string& message, const string& logLevel) {
    if (!inspectorIsConnected()) {
        return;
    }

    auto stack = v8::StackTrace::CurrentStackTrace(isolate, 1, v8::StackTrace::StackTraceOptions::kDetailed);

    auto frame = stack->GetFrame(isolate, 0);

    // will be no-op in non-debuggable builds
    v8_inspector::V8LogAgentImpl::EntryAdded(message, logLevel, ArgConverter::ConvertToString(frame->GetScriptNameOrSourceURL()), frame->GetLineNumber());
}

void JsV8InspectorClient::attachInspectorCallbacks(Isolate* isolate,
        Local<ObjectTemplate>& globalObjectTemplate) {
    v8::HandleScope scope(isolate);

    auto inspectorJSObject = ObjectTemplate::New(isolate);

    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "responseReceived"), FunctionTemplate::New(isolate, NetworkDomainCallbackHandlers::ResponseReceivedCallback));
    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "requestWillBeSent"), FunctionTemplate::New(isolate, NetworkDomainCallbackHandlers::RequestWillBeSentCallback));
    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "dataForRequestId"), FunctionTemplate::New(isolate, NetworkDomainCallbackHandlers::DataForRequestIdCallback));
    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "loadingFinished"), FunctionTemplate::New(isolate, NetworkDomainCallbackHandlers::LoadingFinishedCallback));
    inspectorJSObject->SetAccessor(ArgConverter::ConvertToV8String(isolate, "isConnected"), JsV8InspectorClient::InspectorIsConnectedGetterCallback);

    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "documentUpdated"), FunctionTemplate::New(isolate, DOMDomainCallbackHandlers::DocumentUpdatedCallback));
    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "childNodeInserted"), FunctionTemplate::New(isolate, DOMDomainCallbackHandlers::ChildNodeInsertedCallback));
    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "childNodeRemoved"), FunctionTemplate::New(isolate, DOMDomainCallbackHandlers::ChildNodeRemovedCallback));
    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "attributeModified"), FunctionTemplate::New(isolate, DOMDomainCallbackHandlers::AttributeModifiedCallback));
    inspectorJSObject->Set(ArgConverter::ConvertToV8String(isolate, "attributeRemoved"), FunctionTemplate::New(isolate, DOMDomainCallbackHandlers::AttributeRemovedCallback));

    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__inspector"), inspectorJSObject);
}

void JsV8InspectorClient::InspectorIsConnectedGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    info.GetReturnValue().Set(JsV8InspectorClient::GetInstance()->isConnected_);
}

JsV8InspectorClient* JsV8InspectorClient::instance = nullptr;

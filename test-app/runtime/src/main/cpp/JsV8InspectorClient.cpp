#include "JsV8InspectorClient.h"
#include <assert.h>
#include <include/libplatform/libplatform.h>
#include <src/inspector/v8-console-message.h>
#include <src/inspector/v8-inspector-impl.h>
#include <src/inspector/v8-inspector-session-impl.h>
#include <src/inspector/v8-runtime-agent-impl.h>
#include <src/inspector/v8-stack-trace-impl.h>

#include "Runtime.h"
#include "NativeScriptException.h"

#include "ArgConverter.h"
#include "Util.h"

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

    std::vector<uint16_t> vector = tns::Util::ToVector(message);
    StringView messageView(vector.data(), vector.size());
    bool success;

    /*
    // livesync uses the inspector socket for HMR/LiveSync...
    if(message.find("Page.reload") != std::string::npos) {
        success = tns::LiveSync(this->isolate_);
        if (!success) {
            NSLog(@"LiveSync failed");
        }
        // todo: should we return here, or is it OK to pass onto a possible Page.reload domain handler?
    }
    */


    if(message.find("Tracing.start") != std::string::npos) {
        tracing_agent_->start();

        // echo back the request to notify frontend the action was a success
        // todo: send an empty response for the incoming message id instead.
        this->sendNotification(StringBuffer::create(messageView));
        return;
    }

    if(message.find("Tracing.end") != std::string::npos) {
        tracing_agent_->end();
        std::string res = tracing_agent_->getLastTrace();
        tracing_agent_->SendToDevtools(context, res);
        return;
    }


    // parse incoming message as JSON
    Local<Value> arg;
    success = v8::JSON::Parse(context, tns::ArgConverter::ConvertToV8String(isolate_, message)).ToLocal(&arg);

    // stop processing invalid messages
    if(!success) {

      //  NSLog(@"Inspector failed to parse incoming message: %s", message.c_str());
        // ignore failures to parse.
        return;
    }




    // Pass incoming message to a registerd domain handler if any
    if(!arg.IsEmpty() && arg->IsObject()) {
        Local<Object> domainDebugger;
        Local<Object> argObject = arg.As<Object>();
        Local<v8::Function> domainMethodFunc = tns::Util::GetDebuggerFunctionFromObject(context, argObject, domainDebugger);

        Local<Value> result;
        success = this->CallDomainHandlerFunction(context, domainMethodFunc, argObject, domainDebugger, result);

        if(success) {
            auto requestId = arg.As<Object>()->Get(context, tns::ArgConverter::ConvertToV8String(isolate_, "id")).ToLocalChecked();
            auto returnString = GetReturnMessageFromDomainHandlerResult(result, requestId);

            if(returnString.size() > 0) {
                std::vector<uint16_t> vector_ = tns::Util::ToVector(returnString);
                StringView messageView_(vector_.data(), vector_.size());
                auto msg = StringBuffer::create(messageView_);
                this->sendNotification(std::move(msg));
            }
            return;
        }
    }



    doDispatchMessage(message);
    // TODO: check why this is needed (it should trigger automatically when script depth is 0)
    isolate_->PerformMicrotaskCheckpoint();
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

    tracing_agent_.reset(new tns::inspector::TracingAgentImpl());
}

JsV8InspectorClient* JsV8InspectorClient::GetInstance() {
    if (instance == nullptr) {
        instance = new JsV8InspectorClient(Runtime::GetRuntime(0)->GetIsolate());
    }

    return instance;
}

void JsV8InspectorClient::inspectorSendEventCallback(const FunctionCallbackInfo<Value>& args) {
    if ((instance == nullptr) || (instance->connection_ == nullptr)) {
        return;
    }
    Isolate* isolate = args.GetIsolate();

    Local<v8::String> arg = args[0].As<v8::String>();
    std::string message = ArgConverter::ConvertToString(arg);

    /*
    JEnv env;
    // TODO: Pete: Check if we can use a wide (utf 16) string here
    JniLocalRef str(env.NewStringUTF(message.c_str()));
    env.CallStaticVoidMethod(instance->inspectorClass_, instance->sendMethod_, instance->connection_, (jstring) str);
    */

    // TODO: ios uses this method, but doesn't work on android
    // so I'm just sending directly to the socket (which seems to work)
    instance->dispatchMessage(message);
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

void JsV8InspectorClient::consoleLogCallback(Isolate* isolate, ConsoleAPIType method, const std::vector<v8::Local<v8::Value>>& args) {
    if (!inspectorIsConnected()) {
        return;
    }

    // Note, here we access private V8 API
    auto* impl = reinterpret_cast<v8_inspector::V8InspectorImpl*>(instance->inspector_.get());
    auto* session = reinterpret_cast<v8_inspector::V8InspectorSessionImpl*>(instance->session_.get());

    std::unique_ptr<V8StackTraceImpl> stack = impl->debugger()->captureStackTrace(false);

    v8::Local<v8::Context> context = instance->context_.Get(instance->isolate_);
    const int contextId = V8ContextInfo::executionContextId(context);

    std::unique_ptr<v8_inspector::V8ConsoleMessage> msg =
        v8_inspector::V8ConsoleMessage::createForConsoleAPI(
            context, contextId, contextGroupId, impl, instance->currentTimeMS(),
            method, args, String16{}, std::move(stack));

    session->runtimeAgent()->messageAdded(msg.get());
}

void JsV8InspectorClient::registerDomainDispatcherCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    std::string domain = tns::ArgConverter::ToString(isolate, args[0].As<v8::String>());
    auto it = Domains.find(domain);
    if (it == Domains.end()) {
        Local<v8::Function> domainCtorFunc = args[1].As<v8::Function>();
        Local<Context> context = isolate->GetCurrentContext();
        Local<Value> ctorArgs[0];
        Local<Value> domainInstance;
        bool success = domainCtorFunc->CallAsConstructor(context, 0, ctorArgs).ToLocal(&domainInstance);
        assert(success && domainInstance->IsObject());

        Local<Object> domainObj = domainInstance.As<Object>();
        Persistent<Object>* poDomainObj = new Persistent<Object>(isolate, domainObj);
        Domains.emplace(domain, poDomainObj);
    }
}

void JsV8InspectorClient::inspectorTimestampCallback(const FunctionCallbackInfo<Value>& args) {
    double timestamp = std::chrono::seconds(std::chrono::seconds(std::time(NULL))).count();
    args.GetReturnValue().Set(timestamp);
}

void JsV8InspectorClient::registerModules() {
    Isolate* isolate = isolate_;
    auto rt = Runtime::GetRuntime(isolate);
    v8::Locker l(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto ctx = rt->GetContext();
    v8::Context::Scope context_scope(ctx);

    Local<Context> context = isolate->GetEnteredOrMicrotaskContext();
    Local<Object> global = context->Global();
    Local<Object> inspectorObject = Object::New(isolate);

    assert(global->Set(context, ArgConverter::ConvertToV8String(isolate, "__inspector"), inspectorObject).FromMaybe(false));
    Local<v8::Function> func;
    bool success = v8::Function::New(context, registerDomainDispatcherCallback).ToLocal(&func);
    assert(success && global->Set(context, ArgConverter::ConvertToV8String(isolate, "__registerDomainDispatcher"), func).FromMaybe(false));

    Local<External> data = External::New(isolate, this);
    success = v8::Function::New(context, inspectorSendEventCallback, data).ToLocal(&func);
    assert(success && global->Set(context, ArgConverter::ConvertToV8String(isolate, "__inspectorSendEvent"), func).FromMaybe(false));

    success = v8::Function::New(context, inspectorTimestampCallback).ToLocal(&func);
    assert(success && global->Set(context, ArgConverter::ConvertToV8String(isolate, "__inspectorTimestamp"), func).FromMaybe(false));

    TryCatch tc(isolate);
    Runtime::GetRuntime(isolate)->RunModule("inspector_modules");

}


void JsV8InspectorClient::InspectorIsConnectedGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    info.GetReturnValue().Set(JsV8InspectorClient::GetInstance()->isConnected_);
}

JsV8InspectorClient* JsV8InspectorClient::instance = nullptr;




bool JsV8InspectorClient::CallDomainHandlerFunction(Local<Context> context, Local<Function> domainMethodFunc, const Local<Object>& arg, Local<Object>& domainDebugger, Local<Value>& result) {
    if(domainMethodFunc.IsEmpty() || !domainMethodFunc->IsFunction()) {
        return false;
    }

    bool success;
    Isolate* isolate = this->isolate_;
    TryCatch tc(isolate);

    Local<Value> params;
    success = arg.As<Object>()->Get(context, tns::ArgConverter::ConvertToV8String(isolate, "params")).ToLocal(&params);

    if(!success) {
        return false;
    }

    Local<Value> args[2] = { params, arg };
    success = domainMethodFunc->Call(context, domainDebugger, 2, args).ToLocal(&result);

    if (tc.HasCaught()) {
        std::string error = tns::ArgConverter::ToString(isolate_, tc.Message()->Get());

        // backwards compatibility
        if(error.find("may be enabled at a time") != std::string::npos) {
            // not returning false here because we are catching bogus errors from core...
            // Uncaught Error: One XXX may be enabled at a time...
            result = v8::Boolean::New(isolate, true);
            return true;
        }

        // log any other errors - they are caught, but still make them visible to the user.
       // tns::LogError(isolate, tc);

        return false;
    }

    return success;
}

std::string JsV8InspectorClient::GetReturnMessageFromDomainHandlerResult(const Local<Value>& result, const Local<Value>& requestId) {
    if(result.IsEmpty() || !(result->IsBoolean() || result->IsObject() || result->IsNullOrUndefined())) {
        return "";
    }

    Isolate* isolate = this->isolate_;

    if(!result->IsObject()) {
        // if there return value is a "true" boolean or undefined/null we send back an "ack" response with an empty result object
        if(result->IsNullOrUndefined() || result->BooleanValue(isolate_)) {
            return "{ \"id\":" + tns::ArgConverter::ToString(isolate_, requestId) + ", \"result\": {} }";
        }

        return "";
    }

    v8::Local<Context> context = Runtime::GetRuntime(isolate_)->GetContext();
    Local<Object> resObject = result.As<v8::Object>();
    Local<Value> stringified;

    bool success = true;
    // already a { result: ... } object
    if(resObject->Has(context, tns::ArgConverter::ToV8String(isolate, "result")).ToChecked()) {
        success = JSON::Stringify(context, result).ToLocal(&stringified);
    } else {
        // backwards compatibility - we wrap the response in a new object with the { id, result } keys
        // since the returned response only contained the result part.
        Context::Scope context_scope(context);

        Local<Object> newResObject = v8::Object::New(isolate);
        success = success && newResObject->Set(context, tns::ArgConverter::ToV8String(isolate, "id"), requestId).ToChecked();
        success = success && newResObject->Set(context, tns::ArgConverter::ToV8String(isolate, "result"), resObject).ToChecked();
        success = success && JSON::Stringify(context, newResObject).ToLocal(&stringified);
    }

    if(!success) {
        return "";
    }

    return tns::ArgConverter::ToString(isolate, stringified);
}

std::map<std::string, v8::Persistent<v8::Object>*> JsV8InspectorClient::Domains;

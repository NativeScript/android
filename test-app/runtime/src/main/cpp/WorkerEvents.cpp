#include "WorkerEvents.h"

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "RuntimeState.h"

using namespace v8;

namespace tns {

namespace {

/*
 * The worker-events builtin's delivery callouts for this isolate. Both message
 * directions share emitMessage; only the receiver differs. emitError is
 * parent-side only.
 */
struct WorkerEventsState {
    Global<v8::Function> emitMessage;
    Global<v8::Function> emitError;
};

Local<v8::Function> CalloutOf(Local<Object> exports, Isolate* isolate, const char* name) {
    Local<Value> callout;
    if (!exports->Get(isolate->GetCurrentContext(),
                      ArgConverter::ConvertToV8String(isolate, name))
                 .ToLocal(&callout) ||
        !callout->IsFunction()) {
        throw NativeScriptException(std::string("WorkerEvents::Init: the worker-events "
                                                "bootstrap did not return ") +
                                    name);
    }
    return callout.As<v8::Function>();
}

}  // namespace

void WorkerEvents::Init(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();

    Local<Object> exports;
    if (!BuiltinLoader::GetExports(context, BuiltinId::kWorkerEvents, nullptr)
                 .ToLocal(&exports)) {
        throw NativeScriptException("WorkerEvents::Init: the worker-events bootstrap failed");
    }

    Local<v8::Function> emitMessage = CalloutOf(exports, isolate, "emitMessage");
    Local<v8::Function> emitError = CalloutOf(exports, isolate, "emitError");

    auto* state = RuntimeState::For<WorkerEventsState>(isolate);
    if (state == nullptr) {
        throw NativeScriptException("WorkerEvents::Init: no runtime state for isolate");
    }
    state->emitMessage.Reset(isolate, emitMessage);
    state->emitError.Reset(isolate, emitError);
}

void WorkerEvents::EmitMessage(Isolate* isolate, Local<Object> receiver,
                               const std::shared_ptr<worker::Message>& message) {
    auto* state = RuntimeState::For<WorkerEventsState>(isolate);
    if (state == nullptr || state->emitMessage.IsEmpty()) {
        return;
    }
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return;
    }
    Local<Context> context = runtime->GetContext();

    Local<Value> data;
    Local<Value> ports;
    const char* type = "message";
    {
        TryCatch tc(isolate);
        if (!message->Deserialize(isolate, context, &ports).ToLocal(&data)) {
            if (tc.HasTerminated()) {
                return;
            }
            // HTML: a message that cannot be read still reaches its target, as
            // a `messageerror` event carrying nothing.
            tc.Reset();
            data = v8::Undefined(isolate);
            ports = Local<Value>();
            type = "messageerror";
        }
    }

    Local<Value> args[3]{data,
                         ports.IsEmpty() ? v8::Undefined(isolate).As<Value>() : ports,
                         ArgConverter::ConvertToV8String(isolate, type)};
    Local<Value> result;
    // A throw here is left pending on purpose: on the worker side the drain's
    // TryCatch turns it into the scope's error event, and on the parent side
    // the internal-lane entry runs with no TryCatch of its own, so V8's
    // uncaught-message listener reports it.
    (void)state->emitMessage.Get(isolate)->Call(context, receiver, 3, args).ToLocal(&result);
}

bool WorkerEvents::EmitError(Isolate* isolate, Local<Object> receiver,
                             const std::string& message, const std::string& source,
                             const std::string& stackTrace, int lineNumber) {
    auto* state = RuntimeState::For<WorkerEventsState>(isolate);
    if (state == nullptr || state->emitError.IsEmpty()) {
        return false;
    }
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return false;
    }
    Local<Context> context = runtime->GetContext();

    Local<Value> args[4]{ArgConverter::ConvertToV8String(isolate, message),
                         ArgConverter::ConvertToV8String(isolate, source),
                         Number::New(isolate, lineNumber),
                         ArgConverter::ConvertToV8String(isolate, stackTrace)};
    Local<Value> result;
    if (!state->emitError.Get(isolate)->Call(context, receiver, 4, args).ToLocal(&result)) {
        return false;
    }
    return result->BooleanValue(isolate);
}

}  // namespace tns

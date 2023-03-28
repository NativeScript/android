#include "MessageLoopTimer.h"
#include <android/looper.h>
#include <unistd.h>
#include <cerrno>
#include <thread>
#include "include/libplatform/libplatform.h"
#include "NativeScriptAssert.h"
#include "ArgConverter.h"
#include "Runtime.h"

using namespace tns;
using namespace v8;

static const int SLEEP_INTERVAL_MS = 100;

void MessageLoopTimer::Init(Local<Context> context) {
    this->RegisterStartStopFunctions(context);

    std::string proxyScript = R"(
        (function () {
            // We proxy the WebAssembly's compile, compileStreaming, instantiate and
            // instantiateStreaming methods so that they can start and stop a
            // MessageLoopTimer inside the promise callbacks. This timer will call
            // the v8::platform::PumpMessageLoop method at regular intervals.
            // https://github.com/NativeScript/android-runtime/issues/1558

            global.WebAssembly = new Proxy(WebAssembly, {
                get: (target, name) => {
                    let origMethod = target[name];
                    let proxyMethods = [
                        "compile",
                        "compileStreaming",
                        "instantiate",
                        "instantiateStreaming"
                    ];

                    if (proxyMethods.indexOf(name) < 0) {
                        return origMethod;
                    }

                    return function (...args) {
                        __messageLoopTimerStart();
                        let result = origMethod.apply(this, args);
                        return result.then(x => {
                            __messageLoopTimerStop();
                            return x;
                        }).catch(e => {
                            __messageLoopTimerStop();
                            throw e;
                        });
                    };
                }
            });
        })();
    )";

    Isolate* isolate = context->GetIsolate();

    auto source = ArgConverter::ConvertToV8String(isolate, proxyScript);
    Local<Script> script;
    bool success = Script::Compile(context, source).ToLocal(&script);
    assert(success && !script.IsEmpty());

    Local<Value> result;
    success = script->Run(context).ToLocal(&result);
    assert(success);
}

void MessageLoopTimer::RegisterStartStopFunctions(Local<Context> context) {
    Isolate* isolate = context->GetIsolate();
    Local<Object> global = context->Global();

    Local<External> ext = External::New(isolate, this);
    Local<Function> startFunc;
    Local<Function> stopFunc;
    bool success = Function::New(context, MessageLoopTimer::StartCallback, ext).ToLocal(&startFunc);
    assert(success);
    success = Function::New(context, MessageLoopTimer::StopCallback, ext).ToLocal(&stopFunc);
    assert(success);

    global->Set(context, ArgConverter::ConvertToV8String(isolate, "__messageLoopTimerStart"), startFunc);
    global->Set(context, ArgConverter::ConvertToV8String(isolate, "__messageLoopTimerStop"), stopFunc);
}

void MessageLoopTimer::StartCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    auto self = static_cast<MessageLoopTimer*>(info.Data().As<External>()->Value());
    if (self->m_isRunning) {
        return;
    }

    self->m_isRunning = true;

    auto looper = ALooper_forThread();
    if (looper == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "TNS.Native", "Unable to get looper for the current thread");
        return;
    }

    int status = pipe(self->m_fd);
    if (status != 0) {
        __android_log_print(ANDROID_LOG_ERROR, "TNS.Native", "Unable to create a pipe: %s", strerror(errno));
        return;
    }

    Isolate* isolate = info.GetIsolate();
    ALooper_addFd(looper, self->m_fd[0], 0, ALOOPER_EVENT_INPUT, MessageLoopTimer::PumpMessageLoopCallback, isolate);

    std::thread worker(MessageLoopTimer::WorkerThreadRun, self);

    worker.detach();
}

void MessageLoopTimer::StopCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    auto self = static_cast<MessageLoopTimer*>(info.Data().As<External>()->Value());
    if (!self->m_isRunning) {
        return;
    }

    self->m_isRunning = false;
}

int MessageLoopTimer::PumpMessageLoopCallback(int fd, int events, void* data) {
    uint8_t msg;
    read(fd, &msg, sizeof(uint8_t));

    auto isolate = static_cast<Isolate*>(data);
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    while (v8::platform::PumpMessageLoop(Runtime::platform, isolate)) {
//        isolate->RunMicrotasks();
        isolate->PerformMicrotaskCheckpoint();
    }

    return msg;
}

void MessageLoopTimer::WorkerThreadRun(MessageLoopTimer* timer) {
    while (timer->m_isRunning) {
        uint8_t msg = 1;
        write(timer->m_fd[1], &msg, sizeof(uint8_t));
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_INTERVAL_MS));
    }

    uint8_t msg = 0;
    write(timer->m_fd[1], &msg, sizeof(uint8_t));
}

//
// Created by Osei Fortune on 04/08/2022.
//

#include "SetTimeout.h"
#include "Runtime.h"
using namespace v8;
using namespace tns;

void SetTimeout::Init(Isolate* isolate, Local<ObjectTemplate> globalTemplate) {
    Local<FunctionTemplate> setTimeoutFuncTemplate = FunctionTemplate::New(isolate, SetTimeoutCallback);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__setTimeout"), setTimeoutFuncTemplate);

    Local<FunctionTemplate> clearTimeoutFuncTemplate = FunctionTemplate::New(isolate, ClearTimeoutCallback);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__clearTimeout"), clearTimeoutFuncTemplate);
}

void SetTimeout::SetTimeoutCallback(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    if (!args[0]->IsFunction()) {
        assert(false);
    }

    Local<Context> context = isolate->GetCurrentContext();

    double timeout = 0.0;
    if (args.Length() > 1 && args[1]->IsNumber()) {
        if (!args[1]->NumberValue(context).To(&timeout)) {
            assert(false);
        }
    }

    uint64_t key = ++count_;
    Local<v8::Function> callback = args[0].As<v8::Function>();
    CallbackHandlers::CacheEntry entry(isolate, callback, context);
    CallbackHandlers::setTimeoutCache_.emplace(key, std::move(entry));

    auto func = [](uint64_t key, uint64_t timeout) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        Elapsed(key);
    };

    std::thread thread(func, key, (uint64_t)timeout);
    thread.detach();
    args.GetReturnValue().Set((double) key);
}

void SetTimeout::ClearTimeoutCallback(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    if (!args[0]->IsNumber()) {
        assert(false);
    }

    Local<Context> context = isolate->GetCurrentContext();
    double value;
    if (!args[0]->NumberValue(context).To(&value)) {
        assert(false);
    }

    uint64_t key = value;
    auto it = CallbackHandlers::setTimeoutCache_.find(key);
    if (it == CallbackHandlers::setTimeoutCache_.end()) {
        return;
    }

    it->second.callback_.Reset();
    it->second.context_.Reset();
    CallbackHandlers::setTimeoutCache_.erase(it);
}

void SetTimeout::Elapsed(const uint64_t key) {
    auto it = CallbackHandlers::setTimeoutCache_.find(key);
    if (it == CallbackHandlers::setTimeoutCache_.end()) {
        return;
    }

    auto value = CallbackHandlers::Callback(key);
    value.type_ = CallbackHandlers::Callback::Type::SetTimeout;
    auto size = sizeof(CallbackHandlers::Callback);
    auto rt = Runtime::GetRuntime(it->second.isolate_);
    if (rt == nullptr) {
        return;
    }
    int writer;
    if(rt->isMainThread()){
        writer = Runtime::GetWriter();
    }else {
        writer = rt->GetCurrentWriter();
    }
    auto wrote = write(writer, &value, size);
}

std::atomic_int64_t SetTimeout::count_ = {0};


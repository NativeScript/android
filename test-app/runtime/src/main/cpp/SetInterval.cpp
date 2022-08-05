//
// Created by Osei Fortune on 04/08/2022.
//

#include "SetInterval.h"
#include "Runtime.h"

using namespace v8;
using namespace tns;

void SetInterval::Init(Isolate* isolate, Local<ObjectTemplate> globalTemplate) {
    Local<FunctionTemplate> setIntervalFuncTemplate = FunctionTemplate::New(isolate, SetIntervalCallback);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__setInterval"), setIntervalFuncTemplate);

    Local<FunctionTemplate> clearIntervalFuncTemplate = FunctionTemplate::New(isolate, ClearIntervalCallback);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__clearInterval"), clearIntervalFuncTemplate);
}

void SetInterval::SetIntervalCallback(const FunctionCallbackInfo<Value> &args) {
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
    CallbackHandlers::setIntervalCache_.emplace(key, std::move(entry));

    auto func = [](uint64_t key, uint64_t timeout) {
        bool isRunning = true;
        while (isRunning){
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
            isRunning = Elapsed(key);
        }
    };

    std::thread thread(func, key, (uint64_t)timeout);
    thread.detach();
    args.GetReturnValue().Set((double) key);
}

void SetInterval::ClearIntervalCallback(const FunctionCallbackInfo<Value> &args) {
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
    auto it = CallbackHandlers::setIntervalCache_.find(key);
    if (it == CallbackHandlers::setIntervalCache_.end()) {
        return;
    }


    it->second.callback_.Reset();
    it->second.context_.Reset();
    CallbackHandlers::setIntervalCache_.erase(it);
}

bool SetInterval::Elapsed(const uint64_t key) {
    auto it = CallbackHandlers::setIntervalCache_.find(key);
    if (it == CallbackHandlers::setIntervalCache_.end()) {
        return false;
    }

    auto value = CallbackHandlers::Callback(key);
    value.type_ = CallbackHandlers::Callback::Type::SetInterval;
    auto size = sizeof(CallbackHandlers::Callback);
    auto rt = Runtime::GetRuntime(it->second.isolate_);
    if (rt == nullptr) {
        return false;
    }
    int writer;
    if(rt->isMainThread()){
        writer = Runtime::GetWriter();
    }else {
        writer = rt->GetCurrentWriter();
    }
    auto wrote = write(writer, &value, size);

    return true;
}

std::atomic_int64_t SetInterval::count_ = {0};

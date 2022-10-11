//
// Created by Osei Fortune on 04/08/2022.
//

#include "SetInterval.h"
#include "Runtime.h"
#include "Constants.h"

using namespace v8;
using namespace tns;

void SetInterval::Init(Isolate *isolate, Local<ObjectTemplate> globalTemplate) {
    Local<FunctionTemplate> setIntervalFuncTemplate = FunctionTemplate::New(isolate,
                                                                            SetIntervalCallback);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__setInterval"),
                        setIntervalFuncTemplate);

    Local<FunctionTemplate> clearIntervalFuncTemplate = FunctionTemplate::New(isolate,
                                                                              ClearIntervalCallback);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__clearInterval"),
                        clearIntervalFuncTemplate);

    if (Constants::USE_EXPERIMENTAL_TIMERS) {
        auto attr = static_cast<v8::PropertyAttribute>(v8::PropertyAttribute::DontDelete |
                                                       v8::PropertyAttribute::ReadOnly |
                                                       v8::PropertyAttribute::DontEnum);
        globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "setInterval"),
                            setIntervalFuncTemplate, attr);

        globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "clearInterval"),
                            clearIntervalFuncTemplate, attr);
    }

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

    auto start = new std::atomic_bool(false);
    auto func = [](uint64_t key, uint64_t timeout, std::atomic_bool *start) {
        auto created = std::chrono::steady_clock::now();
        while (!start->load()) {}
        bool isRunning = true;
        while (isRunning) {
            if (!SetInterval::Has(key)) {
                break;
            }
            auto now = std::chrono::steady_clock::now();

            if (timeout < 0) {
                timeout = 0;
            }

            auto pre_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - created);
            auto new_timeout = timeout - pre_duration.count();
            if (new_timeout < 0) {
                isRunning = Elapsed(key);
            } else {
                auto duration = (now +
                                 std::chrono::milliseconds(timeout)) -
                                pre_duration;
                std::this_thread::sleep_until(duration);
                isRunning = Elapsed(key);
            }

            created = std::chrono::steady_clock::now();
        }
        delete start;
    };

    std::thread
            thread(func, key, (uint64_t)
            timeout, start);
    thread.detach();
    start->store(true);
    args.GetReturnValue().Set((double) key);
}

void SetInterval::ClearIntervalCallback(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    if (!args[0]->IsNumber()) {
        return;
    }

    Local<Context> context = isolate->GetCurrentContext();
    double value;
    if (!args[0]->NumberValue(context).To(&value)) {
        assert(false);
    }

    auto key = (uint64_t) value;
    auto it = CallbackHandlers::setIntervalCache_.find(key);
    if (it == CallbackHandlers::setIntervalCache_.end()) {
        return;
    }

    it->second.callback_.Reset();
    it->second.context_.Reset();
    it->second.SetRemoved(true);

    CallbackHandlers::setIntervalCache_.erase(it);
}

bool SetInterval::Elapsed(const uint64_t key) {
    auto it = CallbackHandlers::setIntervalCache_.find(key);
    if (it == CallbackHandlers::setIntervalCache_.end()) {
        return false;
    }

    if (it->second.GetRemoved()) {
        CallbackHandlers::setIntervalCache_.erase(key);
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
    if (rt->isMainThread()) {
        writer = Runtime::GetWriter();
    } else {
        writer = rt->GetCurrentWriter();
    }
    auto wrote = write(writer, &value, size);

    return true;
}

bool SetInterval::Has(const uint64_t key) {
    auto it = CallbackHandlers::setIntervalCache_.find(key);
    if (it == CallbackHandlers::setIntervalCache_.end()) {
        return false;
    }

    if (it->second.GetRemoved()) {
        CallbackHandlers::setIntervalCache_.erase(key);
        return false;
    }

    return true;
}

std::atomic_int64_t SetInterval::count_ = {0};

//
// Created by Osei Fortune on 04/08/2022.
//
#pragma
#include "include/v8.h"
#include "robin_hood.h"
#include <chrono>
#include <thread>
#include <unistd.h>
#include <dlfcn.h>
#include "ArgConverter.h"
#include "NativeScriptAssert.h"
#include "CallbackHandlers.h"

namespace tns {
    class SetInterval {
    public:
        static void Init(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> globalTemplate);
    private:
        static void SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void ClearIntervalCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static bool Elapsed(const uint64_t key);
        static std::atomic_int64_t count_;
    };
};



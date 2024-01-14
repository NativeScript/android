//
// Created by Osei Fortune on 15/011/2023.
//

#pragma once

#include <vector>
#include "ada/ada.h"
#include "v8.h"
#include "ArgConverter.h"

using namespace ada;
namespace tns {
    class URLImpl {
    public:
        URLImpl(url url);

        url *GetURL();

        static URLImpl *GetPointer(v8::Local<v8::Object> object);

        static v8::Local<v8::FunctionTemplate> GetCtor(v8::Isolate *isolate);

        static void Ctor(const v8::FunctionCallbackInfo<v8::Value> &args);


        static void
        GetHash(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetHash(v8::Local<v8::String> property,
                            v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info);


        static void
        GetHost(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetHost(v8::Local<v8::String> property,
                            v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info);


        static void
        GetHostName(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetHostName(v8::Local<v8::String> property,
                                v8::Local<v8::Value> value,
                                const v8::PropertyCallbackInfo<void> &info);


        static void
        GetHref(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetHref(v8::Local<v8::String> property,
                            v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info);


        static void
        GetOrigin(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);


        static void
        GetPassword(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetPassword(v8::Local<v8::String> property,
                                v8::Local<v8::Value> value,
                                const v8::PropertyCallbackInfo<void> &info);


        static void
        GetPathName(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetPathName(v8::Local<v8::String> property,
                                v8::Local<v8::Value> value,
                                const v8::PropertyCallbackInfo<void> &info);


        static void
        GetPort(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetPort(v8::Local<v8::String> property,
                            v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info);


        static void
        GetProtocol(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetProtocol(v8::Local<v8::String> property,
                                v8::Local<v8::Value> value,
                                const v8::PropertyCallbackInfo<void> &info);


        static void
        GetSearch(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetSearch(v8::Local<v8::String> property,
                              v8::Local<v8::Value> value,
                              const v8::PropertyCallbackInfo<void> &info);


        static void
        GetUserName(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void SetUserName(v8::Local<v8::String> property,
                                v8::Local<v8::Value> value,
                                const v8::PropertyCallbackInfo<void> &info);

        static void ToString(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void CanParse(const v8::FunctionCallbackInfo<v8::Value> &args);

        void BindFinalizer(v8::Isolate *isolate, const v8::Local<v8::Object> &object) {
            v8::HandleScope scopedHandle(isolate);
            weakHandle_.Reset(isolate, object);
            weakHandle_.SetWeak(this, Finalizer, v8::WeakCallbackType::kParameter);
        }

        static void Finalizer(const v8::WeakCallbackInfo<URLImpl> &data) {
            auto *pThis = data.GetParameter();
            pThis->weakHandle_.Reset();
            delete pThis;
        }

    private:
        url url_;
        v8::Global<v8::Object> weakHandle_;
    };
}

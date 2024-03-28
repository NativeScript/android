//
// Created by Osei Fortune on 14/01/2024.
//
#pragma once

#include "ada/ada.h"
#include "v8.h"
#include "ArgConverter.h"

namespace tns {

    class URLSearchParamsImpl {
    public:

        URLSearchParamsImpl(ada::url_search_params params);

        ada::url_search_params *GetURLSearchParams();

        static URLSearchParamsImpl *GetPointer(v8::Local<v8::Object> object);

        static v8::Local<v8::FunctionTemplate> GetCtor(v8::Isolate *isolate);

        static void Ctor(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Append(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Delete(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Entries(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void ForEach(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Get(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void GetAll(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Has(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Keys(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Set(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void
        GetSize(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void Sort(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void ToString(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Values(const v8::FunctionCallbackInfo<v8::Value> &args);

        void BindFinalizer(v8::Isolate *isolate, const v8::Local<v8::Object> &object) {
            v8::HandleScope scopedHandle(isolate);
            weakHandle_.Reset(isolate, object);
            weakHandle_.SetWeak(this, Finalizer, v8::WeakCallbackType::kParameter);
        }

        static void Finalizer(const v8::WeakCallbackInfo<URLSearchParamsImpl> &data) {
            auto *pThis = data.GetParameter();
            pThis->weakHandle_.Reset();
            delete pThis;
        }

    private:
        ada::url_search_params params_;
        v8::Global<v8::Object> weakHandle_;
    };

} // tns

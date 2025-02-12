//
// Created by Osei Fortune on 30/01/2025.
//

#ifndef TEST_APP_URLPATTERNIMPL_H
#define TEST_APP_URLPATTERNIMPL_H

#include "ada/ada.h"
#include "v8.h"
#include "ArgConverter.h"
#include "NativeScriptAssert.h"

using namespace ada;
namespace tns {

    class v8_regex_provider {
    public:

        using regex_type = v8::Global<v8::RegExp>;

        static std::optional<regex_type> create_instance(std::string_view pattern,
                                                         bool ignore_case);

        static std::optional<std::vector<std::optional<std::string>>> regex_search(
                std::string_view input, const regex_type &pattern);

        static bool regex_match(std::string_view input, const regex_type &pattern);
    };

    class URLPatternImpl {
    public:

        URLPatternImpl(url_pattern <v8_regex_provider> pattern);

        url_pattern <v8_regex_provider> *GetPattern();

        static URLPatternImpl *GetPointer(v8::Local<v8::Object> object);

        static v8::Local<v8::FunctionTemplate> GetCtor(v8::Isolate *isolate);

        static void Ctor(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void
        GetHash(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetHostName(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetPassword(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetPathName(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetPort(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetProtocol(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetSearch(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetUserName(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info);

        static void
        GetHasRegexpGroups(v8::Local<v8::Name> name,
                           const v8::PropertyCallbackInfo<v8::Value> &info);

        static void Test(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void Exec(const v8::FunctionCallbackInfo<v8::Value> &args);

        void BindFinalizer(v8::Isolate *isolate, const v8::Local<v8::Object> &object) {
            v8::HandleScope scopedHandle(isolate);
            weakHandle_.Reset(isolate, object);
            weakHandle_.SetWeak(this, Finalizer, v8::WeakCallbackType::kParameter);
        }

        static void Finalizer(const v8::WeakCallbackInfo<URLPatternImpl> &data) {
            auto *pThis = data.GetParameter();
            pThis->weakHandle_.Reset();
            delete pThis;
        }

    private:
        url_pattern <v8_regex_provider> pattern_;
        v8::Global<v8::Object> weakHandle_;

        static std::optional<ada::url_pattern_init>
        ParseInput(v8::Isolate *isolate, const v8::Local<v8::Value> &input);

    };
}


#endif //TEST_APP_URLPATTERNIMPL_H

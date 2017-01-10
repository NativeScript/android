#ifndef NUMERICCASTS_H_
#define NUMERICCASTS_H_

#include "v8.h"
#include <string>

namespace tns {
enum class CastType {
    None,
    Char,
    Byte,
    Short,
    Long,
    Float,
    Double
};

class NumericCasts {
    public:
        void CreateGlobalCastFunctions(v8::Isolate* isolate, const v8::Local<v8::ObjectTemplate>& globalTemplate);

        static CastType GetCastType(v8::Isolate* isolate, const v8::Local<v8::Object>& object);

        static v8::Local<v8::Value> GetCastValue(const v8::Local<v8::Object>& object);

        static void MarkAsLong(v8::Isolate* isolate, const v8::Local<v8::Object>& object, const v8::Local<v8::Value>& value);

    private:
        void MarkAsLongCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void MarkAsByteCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void MarkAsShortCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void MarkAsCharCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void MarkAsFloatCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void MarkAsDoubleCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void MarkJsObject(v8::Isolate* isolate, const v8::Local<v8::Object>& object, CastType castType, const v8::Local<v8::Value>& value);

        static void MarkAsLongCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void MarkAsByteCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void MarkAsShortCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void MarkAsCharCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void MarkAsFloatCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void MarkAsDoubleCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

        static NumericCasts* GetThis(const v8::FunctionCallbackInfo<v8::Value>& args);

        static std::string s_castMarker;
};
}

#endif /* NUMERICCASTS_H_ */

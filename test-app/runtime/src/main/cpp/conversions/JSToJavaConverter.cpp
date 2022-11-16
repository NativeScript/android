#include "JSToJavaConverter.h"
#include "ObjectManager.h"
#include "JniSignatureParser.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "V8StringConstants.h"
#include "NumericCasts.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "V8GlobalHelpers.h"
#include <cstdlib>
#include "CastTypes.h"
#include "./objects/JSToJavaObjectsConverter.h"
#include "./arrays/JSToJavaArraysConverter.h"
#include "./primitives/JSToJavaPrimitivesConverter.h"

using namespace v8;
using namespace std;
using namespace tns;

JSToJavaConverter::JSToJavaConverter(
        const FunctionCallbackInfo<v8::Value> &args,
        const string &methodSignature,
        MetadataEntry *entry)
        :
        m_isolate(args.GetIsolate()),
        m_methodSignature(methodSignature) {
    m_argsLen = args.Length();

    if (m_argsLen > 0) {
        if ((entry != nullptr) && (entry->isResolved)) {
            if (entry->parsedSig.empty()) {
                JniSignatureParser parser(m_methodSignature);
                entry->parsedSig = parser.Parse();
            }
            m_tokens = &entry->parsedSig;
        } else {
            JniSignatureParser parser(m_methodSignature);
            std::vector<std::string> parsed = parser.Parse();
            m_tokens = new std::vector<std::string>();
            m_tokens->reserve(parsed.size());
            for (std::string& p : parsed) {
                m_tokens->push_back(p);
            }
//            std::copy(parsed.begin(),  parsed.end(), m_tokens->begin());
        }

        for (int i = 0; i < m_argsLen; i++) {
            ConvertArg(args[i], i);
        }
    }
}

JSToJavaConverter::JSToJavaConverter(
        const v8::FunctionCallbackInfo<Value> &args,
        const string &methodSignature,
        MetadataEntry *entry,
        const v8::Local<v8::Object> kotlinExtensionFunctionThis)
        :
        m_isolate(args.GetIsolate()),
        m_methodSignature(methodSignature) {

    int v8ProvidedArgumentsLength = args.Length();
    m_argsLen = 1 + v8ProvidedArgumentsLength;

    if (m_argsLen > 0) {
        if ((entry != nullptr) && (entry->isResolved)) {
            if (entry->parsedSig.empty()) {
                JniSignatureParser parser(m_methodSignature);
                entry->parsedSig = parser.Parse();
            }
            m_tokens = &entry->parsedSig;
        } else {
            JniSignatureParser parser(m_methodSignature);
            auto parsed = parser.Parse();
            m_tokens = new std::vector<std::string>();
            m_tokens->reserve(parsed.size());
            for (std::string& p : parsed) {
                m_tokens->push_back(p);
            }
        }

        ConvertArg(kotlinExtensionFunctionThis, 0);

        for (int i = 0; i < v8ProvidedArgumentsLength; i++) {
            ConvertArg(args[i], i + 1);
        }
    }
}

JSToJavaConverter::JSToJavaConverter(
        const v8::FunctionCallbackInfo<Value> &args,
        const string &methodSignature)
        :
        m_isolate(args.GetIsolate()),
        m_methodSignature(methodSignature) {
    m_argsLen = args.Length();

    JniSignatureParser parser(m_methodSignature);
    auto parsed = parser.Parse();
    m_tokens = new std::vector<std::string>();
    m_tokens->reserve(parsed.size());
    for (std::string& p : parsed) {
        m_tokens->push_back(p);
    }

    for (int i = 0; i < m_argsLen; i++) {
        ConvertArg(args[i], i);
    }
}

bool JSToJavaConverter::ConvertArg(const Local<Value> &arg, int index) {
    const auto &typeSignature = m_tokens->at(index);

    if (arg->IsArray() && typeSignature[0] == '[') {
        return tns::ConvertJavaScriptArray(
                m_isolate,
                arg,
                index,
                m_tokens,
                m_jniArgRefsState
        );
    } else if (arg->IsNumber() || arg->IsNumberObject()) {
        return tns::ConvertJavaScriptNumber(
                m_isolate,
                arg,
                index,
                m_tokens,
                m_jniArgRefsState
        );
    } else if (arg->IsBoolean() || arg->IsBooleanObject()) {
        return tns::ConvertJavaScriptBoolean(
                m_isolate,
                arg,
                index,
                m_tokens,
                m_jniArgRefsState
        );
    } else if (arg->IsString() || arg->IsStringObject()) {
        return tns::ConvertJavaScriptString(
                arg,
                index,
                m_jniArgRefsState
        );
    } else if (arg->IsObject()) {
        return tns::ConvertJavaScriptObject(
                m_isolate,
                arg,
                index,
                m_methodSignature,
                m_tokens,

                m_jniArgRefsState
        );
    } else if (arg->IsUndefined() || arg->IsNull()) {
        m_jniArgRefsState.SetConvertedObject(index, nullptr);
        return true;
    } else {
        m_jniArgRefsState.SetConvertedObject(index, nullptr);
        return false;
    }
}

int JSToJavaConverter::Length() const {
    return m_argsLen;
}

jvalue *JSToJavaConverter::ToArgs() {
    return m_jniArgRefsState.m_args;
}

JSToJavaConverter::~JSToJavaConverter() {
    if (m_argsLen > 0) {
        JEnv env;
        auto refsSize = m_jniArgRefsState.m_args_refs_size;
        auto argsRefs = m_jniArgRefsState.m_args_refs;
        auto args = m_jniArgRefsState.m_args;

        for (int i = 0; i < refsSize; i++) {
            int index = argsRefs[i];
            if (index != -1) {
                env.DeleteLocalRef(args[index].l);
            }
        }
    }
}

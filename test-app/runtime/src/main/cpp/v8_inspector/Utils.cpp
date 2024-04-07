#include "Utils.h"
#include "JsV8InspectorClient.h"
#include "Util.h"

using namespace v8;
using namespace std;
namespace tns {

    Local <v8::Function>
    GetDebuggerFunction(Local <Context> context, std::string domain, std::string functionName,
                              Local <Object> &domainDebugger) {
        auto it = JsV8InspectorClient::Domains.find(domain);
        if (it == JsV8InspectorClient::Domains.end()) {
            return Local<v8::Function>();
        }

        Isolate *isolate = context->GetIsolate();
        domainDebugger = it->second->Get(isolate);

        Local <Value> value;
        auto funcName = v8::String::NewFromUtf8(isolate, functionName.c_str(),
                                                v8::NewStringType::kNormal,
                                                (int) functionName.length()).ToLocalChecked();
        bool success = domainDebugger->Get(context, funcName).ToLocal(&value);
        if (success && !value.IsEmpty() && value->IsFunction()) {
            return value.As<v8::Function>();
        }

        return Local<v8::Function>();
    }

    Local <v8::Function>
    GetDebuggerFunctionFromObject(Local <Context> context, const Local <Object> &object,
                                        Local <Object> &domainDebugger) {
        Isolate *isolate = context->GetIsolate();
        auto methodKey = v8::String::NewFromUtf8(isolate, "method",
                                                 v8::NewStringType::kNormal).ToLocalChecked();
        auto method = object->Get(context, methodKey).ToLocalChecked();
        auto methodString = Util::ToString(isolate, method);
        auto domainSeparatorIndex = methodString.find(".");
        auto domain = methodString.substr(0, domainSeparatorIndex);
        auto domainMethod = methodString.substr(domainSeparatorIndex + 1, methodString.size());

        if (domain.size() > 0) {
            return GetDebuggerFunction(context, domain, domainMethod, domainDebugger);
        }

        return Local<v8::Function>();
    }
}
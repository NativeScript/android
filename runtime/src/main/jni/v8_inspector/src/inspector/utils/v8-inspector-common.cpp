//
// Created by pkanev on 6/5/2017.
//

#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
#include <ArgConverter.h>
#include <NativeScriptAssert.h>

using tns::ArgConverter;

namespace v8_inspector {
namespace utils {
v8::Local<v8::Object> Common::getGlobalInspectorObject(v8::Isolate* isolate) {
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto inspectorObjectString = "__inspector";

    v8::Local<v8::Value> outInspector;

    auto maybeInspectorObj = global->Get(context, ArgConverter::ConvertToV8String(isolate, inspectorObjectString));

    if (maybeInspectorObj.ToLocal(&outInspector)) {
        return outInspector->ToObject();
    }

    return v8::Local<v8::Object>();
}

std::string Common::getJSCallErrorMessage(const std::string& functionName, v8::Local<v8::String> tcMessage) {
    auto errorMessage = "Error thrown while calling " + functionName + ": " + ArgConverter::ConvertToString(tcMessage);

    DEBUG_WRITE_FORCE("JS Error: %s", errorMessage.c_str());

    return errorMessage;
}
}
}

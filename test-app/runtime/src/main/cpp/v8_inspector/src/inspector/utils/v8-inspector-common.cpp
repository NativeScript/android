//
// Created by pkanev on 6/5/2017.
//

#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
#include <codecvt>
#include <locale>
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
        return outInspector->ToObject(context).ToLocalChecked();
    }

    return {};
}

std::string Common::getJSCallErrorMessage(const std::string& functionName, v8::Local<v8::String> tcMessage) {
    auto errorMessage = "Error thrown while calling " + functionName + ": " + ArgConverter::ConvertToString(tcMessage);

    DEBUG_WRITE_FORCE("JS Error: %s", errorMessage.c_str());

    return errorMessage;
}

protocol::DispatchResponse Common::protocolCommandNotSupportedDispatchResponse() {
    return protocol::DispatchResponse::ServerError(s_notImplemented);
}

std::vector<uint16_t> Common::toVector(const std::string &value) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::u16string valueu16 = convert.from_bytes(value);

    const auto *begin = reinterpret_cast<uint16_t const *>(valueu16.data());
    const auto *end = reinterpret_cast<uint16_t const *>(valueu16.data() + valueu16.size());
    std::vector<uint16_t> vector(begin, end);
    return vector;
}

const std::string Common::s_notImplemented = "Protocol command not supported.";
}
}

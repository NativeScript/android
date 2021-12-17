//
// Created by pkanev on 6/5/2017.
//

#ifndef V8_INSPECTOR_COMMON_H
#define V8_INSPECTOR_COMMON_H

#include <include/v8.h>
#include <v8_inspector/src/inspector/string-16.h>
#include <v8_inspector/src/inspector/protocol/Protocol.h>

namespace v8_inspector {
namespace utils {
class Common {
    public:
        static v8::Local<v8::Object> getGlobalInspectorObject(v8::Isolate* isolate);
        static std::string getJSCallErrorMessage(const std::string& functionName, v8::Local<v8::String> tcMessage);

        static protocol::DispatchResponse protocolCommandNotSupportedDispatchResponse();

        static std::vector<uint16_t> toVector(const std::string& value);

        template<typename T>
        static std::unique_ptr<protocol::Array<T>> fromValue(protocol::Value* value, protocol::ErrorSupport* errors) {
            protocol::ListValue* array = protocol::ListValue::cast(value);
            if (!array) {
                errors->AddError("array expected");
                return nullptr;
            }

            std::unique_ptr<protocol::Array<T>> result(new protocol::Array<T>());
            errors->Push();
            for (size_t i = 0; i < array->size(); ++i) {
                auto name = std::to_string(i);
                errors->SetName(name.c_str());
                std::unique_ptr<T> item = protocol::ValueConversions<T>::fromValue(array->at(i), errors);
                result->push_back(std::move(item));
            }

            errors->Pop();
            if (!errors->Errors().empty()) {
                return nullptr;
            }

            return result;
        }

    private:
        static const std::string s_notImplemented;
};

}
}

#endif //V8_INSPECTOR_COMMON_H

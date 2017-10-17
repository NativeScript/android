//
// Created by pkanev on 6/5/2017.
//

#ifndef V8_INSPECTOR_COMMON_H
#define V8_INSPECTOR_COMMON_H

#include <include/v8.h>

namespace v8_inspector {
namespace utils {
class Common {
    public:
        static v8::Local<v8::Object> getGlobalInspectorObject(v8::Isolate* isolate);
        static std::string getJSCallErrorMessage(const std::string& functionName, v8::Local<v8::String> tcMessage);
};
}
}

#endif //V8_INSPECTOR_COMMON_H

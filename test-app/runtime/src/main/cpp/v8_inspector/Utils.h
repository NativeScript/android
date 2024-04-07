#include <string>
#include <vector>

#ifndef UTILS_H_
#define UTILS_H_

#include "include/v8-inspector.h"

namespace tns {
    namespace inspector {
        static v8::Local<v8::Function> GetDebuggerFunctionFromObject(v8::Local<v8::Context> context,
                                                                     const v8::Local<v8::Object> &object,
                                                                     v8::Local<v8::Object> &domainDebugger);

        static v8::Local<v8::Function>
        GetDebuggerFunction(v8::Local<v8::Context> context, std::string domain,
                            std::string functionName, v8::Local<v8::Object> &domainDebugger);
    }
}

#endif /* UTILS_H_ */
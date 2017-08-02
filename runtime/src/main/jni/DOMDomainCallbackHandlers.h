//
// Created by pkanev on 5/10/2017.
//

#ifndef DOMDOMAINCALLBACKHANDLERS_H
#define DOMDOMAINCALLBACKHANDLERS_H

#include <include/v8.h>
#include <v8_inspector/src/inspector/v8-dom-agent-impl.h>
#include "JsV8InspectorClient.h"
#include "NativeScriptException.h"

namespace tns {
class DOMDomainCallbackHandlers {

    public:
        static void DocumentUpdatedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void ChildNodeInsertedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void ChildNodeRemovedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void AttributeModifiedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void AttributeRemovedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
};
}


#endif //DOMDOMAINCALLBACKHANDLERS_H

//
// Created by pkanev on 11/7/2017.
//

#ifndef CSSDOMAINCALLBACKHANDLERS_H
#define CSSDOMAINCALLBACKHANDLERS_H

#include <include/v8.h>
#include <v8_inspector/src/inspector/v8-css-agent-impl.h>
#include "JsV8InspectorClient.h"
#include "NativeScriptException.h"

namespace tns {
class CSSDomainCallbackHandlers {

    public:
        static void StyleSheetAddedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void StyleSheetRemovedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
};
}



#endif //CSSDOMAINCALLBACKHANDLERS_H

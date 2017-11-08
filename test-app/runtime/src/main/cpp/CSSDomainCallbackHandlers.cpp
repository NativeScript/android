//
// Created by pkanev on 11/7/2017.
//

#include <sstream>
#include <NativeScriptAssert.h>
#include "CSSDomainCallbackHandlers.h"
#include "V8GlobalHelpers.h"

using namespace tns;

void CSSDomainCallbackHandlers::StyleSheetAddedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto cssAgentInstance = V8CSSAgentImpl::Instance;

        if (!cssAgentInstance) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        if (args.Length() != 1 || !args[0]->IsObject()) {
            throw NativeScriptException("Calling StyleSheetAdded with invalid arguments. Required param: styleSheetHeader of type CSSStyleSheetHeader");
        }

        v8::Local<v8::Object> styleSheetHeaderParam = args[0]->ToObject(isolate);

        String errorString;
        auto styleSheetHeaderProtocolValue = toProtocolValue(&errorString, isolate->GetCurrentContext(), styleSheetHeaderParam);

        if (!errorString.isEmpty()) {
            auto errorMessage = "Error while parsing debug `CSSStyleSheetHeader` object. " + errorString.utf8();
            throw NativeScriptException(errorMessage);
        }

        protocol::ErrorSupport errorSupport;

        auto styleSheetHeader = protocol::CSS::CSSStyleSheetHeader::parse(styleSheetHeaderProtocolValue.get(), &errorSupport);

        auto errorSupportString = errorSupport.errors().utf8();
        if (!errorSupportString.empty()) {
            auto errorMessage = "Error while parsing debug `CSSStyleSheetHeader` object. " + errorSupportString;
            throw NativeScriptException(errorMessage);
        }

        cssAgentInstance->m_frontend.styleSheetAdded(std::move(styleSheetHeader));
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c exception!"));
        nsEx.ReThrowToV8();
    }
}

void CSSDomainCallbackHandlers::StyleSheetRemovedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {

}
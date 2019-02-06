//
// Created by pkanev on 5/10/2017.
//

#include <sstream>
#include <ArgConverter.h>
#include <NativeScriptAssert.h>
#include "DOMDomainCallbackHandlers.h"

using namespace tns;

void DOMDomainCallbackHandlers::DocumentUpdatedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto domAgentInstance = V8DOMAgentImpl::Instance;

    if (!domAgentInstance) {
        return;
    }

    domAgentInstance->m_frontend.documentUpdated();
}

void DOMDomainCallbackHandlers::ChildNodeInsertedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto domAgentInstance = V8DOMAgentImpl::Instance;

        if (!domAgentInstance) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        if (args.Length() != 3 || !(args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsString())) {
            throw NativeScriptException("Calling ChildNodeInserted with invalid arguments. Required params: parentId: number, lastId: number, node: JSON String");
        }

        auto parentId = args[0]->ToNumber(isolate);
        auto lastId = args[1]->ToNumber(isolate);
        auto node = args[2]->ToString(isolate);

        auto resultString = V8DOMAgentImpl::AddBackendNodeIdProperty(isolate, node);
        auto resultUtf16Data = resultString.data();

        auto nodeJson = protocol::StringUtil::parseJSON(String16((const uint16_t*) resultUtf16Data));

        protocol::ErrorSupport errorSupport;
        auto domNode = protocol::DOM::Node::fromValue(nodeJson.get(), &errorSupport);

        auto errorSupportString = errorSupport.errors().utf8();
        if (!errorSupportString.empty()) {
            auto errorMessage = "Error while parsing debug `DOM Node` object. ";
            DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorSupportString.c_str());
            return;
        }

        auto context = isolate->GetCurrentContext();
        domAgentInstance->m_frontend.childNodeInserted(parentId->Int32Value(context).ToChecked(), lastId->Int32Value(context).ToChecked(), std::move(domNode));
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

void DOMDomainCallbackHandlers::ChildNodeRemovedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto domAgentInstance = V8DOMAgentImpl::Instance;

        if (!domAgentInstance) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        if (args.Length() != 2 || !(args[0]->IsNumber() && args[1]->IsNumber())) {
            throw NativeScriptException("Calling ChildNodeRemoved with invalid arguments. Required params: parentId: number, nodeId: number");
        }

        auto parentId = args[0]->ToNumber(isolate);
        auto nodeId = args[1]->ToNumber(isolate);

        auto context = isolate->GetCurrentContext();
        domAgentInstance->m_frontend.childNodeRemoved(parentId->Int32Value(context).ToChecked(), nodeId->Int32Value(context).ToChecked());
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

void DOMDomainCallbackHandlers::AttributeModifiedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto domAgentInstance = V8DOMAgentImpl::Instance;

        if (!domAgentInstance) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        if (args.Length() != 3 || !(args[0]->IsNumber() && args[1]->IsString() && args[2]->IsString())) {
            throw NativeScriptException("Calling AttributeModified with invalid arguments. Required params: nodeId: number, name: string, value: string");
        }

        auto nodeId = args[0]->ToNumber(isolate);
        auto attributeName = args[1]->ToString(isolate);
        auto attributeValue = args[2]->ToString(isolate);

        auto context = isolate->GetCurrentContext();
        domAgentInstance->m_frontend.attributeModified(nodeId->Int32Value(context).ToChecked(),
                v8_inspector::toProtocolString(isolate, attributeName),
                v8_inspector::toProtocolString(isolate, attributeValue));
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

void DOMDomainCallbackHandlers::AttributeRemovedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto domAgentInstance = V8DOMAgentImpl::Instance;

        if (!domAgentInstance) {
            return;
        }
        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        if (args.Length() != 2 || !(args[0]->IsNumber() && args[1]->IsString())) {
            throw NativeScriptException("Calling AttributeRemoved with invalid arguments. Required params: nodeId: number, name: string");
        }

        auto nodeId = args[0]->ToNumber(isolate);
        auto attributeName = args[1]->ToString(isolate);

        auto context = isolate->GetCurrentContext();
        domAgentInstance->m_frontend.attributeRemoved(nodeId->Int32Value(context).ToChecked(),
                v8_inspector::toProtocolString(isolate, attributeName));
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

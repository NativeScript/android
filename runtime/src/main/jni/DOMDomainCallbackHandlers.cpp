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

        auto nodeJson = protocol::parseJSON(v8_inspector::toProtocolString(node));

        protocol::ErrorSupport errorSupport;
        auto domNode = protocol::DOM::Node::parse(nodeJson.get(), &errorSupport);

        auto errorSupportString = errorSupport.errors().utf8();
        if (!errorSupportString.empty()) {
            auto errorMessage = "Error while parsing debug `DOM Node` object. ";
            DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorSupportString.c_str());
        }

        domAgentInstance->m_frontend.childNodeInserted(parentId->Int32Value(), lastId->Int32Value(), std::move(domNode));
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

        domAgentInstance->m_frontend.childNodeRemoved(parentId->Int32Value(), nodeId->Int32Value());
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
        auto attributeName = args[1]->ToString();
        auto attributeValue = args[2]->ToString();

        domAgentInstance->m_frontend.attributeModified(nodeId->Int32Value(),
                v8_inspector::toProtocolString(attributeName),
                v8_inspector::toProtocolString(attributeValue));
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
        auto attributeName = args[1]->ToString();

        domAgentInstance->m_frontend.attributeRemoved(nodeId->Int32Value(),
                v8_inspector::toProtocolString(attributeName));
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

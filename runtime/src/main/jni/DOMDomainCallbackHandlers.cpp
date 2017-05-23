//
// Created by pkanev on 5/10/2017.
//

#include <ArgConverter.h>
#include <NativeScriptAssert.h>
#include "DOMDomainCallbackHandlers.h"

using namespace tns;

void DOMDomainCallbackHandlers::DocumentUpdatedCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto domAgentInstance = V8DOMAgentImpl::Instance;

    if (!domAgentInstance) {
        return;
    }

    domAgentInstance->m_frontend.documentUpdated();
}

void DOMDomainCallbackHandlers::ChildNodeInsertedCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto domAgentInstance = V8DOMAgentImpl::Instance;

    if (!domAgentInstance) {
        return;
    }

    if (args.Length() == 0) {
        return;
    }

    auto isolate = args.GetIsolate();

    v8::HandleScope scope(isolate);

    auto context = isolate->GetCurrentContext();

    // TODO: Pete: Validate!
    auto parentId = args[0]->ToNumber(isolate);
    auto lastId = args[1]->ToNumber(isolate);
    auto node = args[2]->ToString(isolate);

    auto nodeString = ArgConverter::ConvertToString(node);
    auto nodeCStr = nodeString.c_str();
    auto nodeJson = protocol::parseJSON(nodeCStr);

    protocol::ErrorSupport errorSupport;
    auto domNode = protocol::DOM::Node::parse(nodeJson.get(), &errorSupport);

    auto errorSupportString = errorSupport.errors().utf8();
    if (!errorSupportString.empty()) {
        auto errorMessage = "Error while parsing debug `DOM Node` object. ";
        DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorSupportString.c_str());
    }

    domAgentInstance->m_frontend.childNodeInserted(parentId->Int32Value(), lastId->Int32Value(), std::move(domNode));
}

void DOMDomainCallbackHandlers::ChildNodeRemovedCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto domAgentInstance = V8DOMAgentImpl::Instance;

    if (!domAgentInstance) {
        return;
    }

    if (args.Length() == 0) {
        return;
    }

    auto isolate = args.GetIsolate();

    v8::HandleScope scope(isolate);

    auto context = isolate->GetCurrentContext();

    // TODO: Pete: Validate!
    auto parentId = args[0]->ToNumber(isolate);
    auto nodeId = args[1]->ToNumber(isolate);

    domAgentInstance->m_frontend.childNodeRemoved(parentId->Int32Value(), nodeId->Int32Value());
}

void DOMDomainCallbackHandlers::AttributeModifiedCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto domAgentInstance = V8DOMAgentImpl::Instance;

    if (!domAgentInstance) {
        return;
    }


//    domAgentInstance->m_frontend.attributeModified();
}
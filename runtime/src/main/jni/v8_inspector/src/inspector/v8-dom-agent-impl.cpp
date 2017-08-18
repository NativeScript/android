//
// Created by pkanev on 4/24/2017.
//

#include <NativeScriptAssert.h>
#include "v8-dom-agent-impl.h"
#include <ArgConverter.h>
#include <Runtime.h>
#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>

namespace v8_inspector {

using tns::Runtime;
using tns::ArgConverter;

namespace DOMAgentState {
static const char domEnabled[] = "domEnabled";
}

V8DOMAgentImpl::V8DOMAgentImpl(V8InspectorSessionImpl* session,
                               protocol::FrontendChannel* frontendChannel,
                               protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

V8DOMAgentImpl::~V8DOMAgentImpl() { }

void V8DOMAgentImpl::enable(ErrorString*) {
    if (m_enabled) {
        return;
    }

    m_state->setBoolean(DOMAgentState::domEnabled, true);

    m_enabled = true;
}

void V8DOMAgentImpl::disable(ErrorString*) {
    if (!m_enabled) {
        return;
    }

    m_state->setBoolean(DOMAgentState::domEnabled, false);

    m_enabled = false;
}

void V8DOMAgentImpl::getDocument(ErrorString* errorString, std::unique_ptr<protocol::DOM::Node>* out_root) {
    std::unique_ptr<protocol::DOM::Node> defaultNode = protocol::DOM::Node::create()
            .setNodeId(0)
            .setNodeType(9)
            .setNodeName("Frame")
            .setLocalName("Frame")
            .setNodeValue("")
            .build();

    std::string getDocumentFunctionString = "getDocument";
    // TODO: Pete: Find a better way to get a hold of the isolate
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        auto getDocument = globalInspectorObject->Get(ArgConverter::ConvertToV8String(isolate, getDocumentFunctionString));

        if (!getDocument.IsEmpty() && getDocument->IsFunction()) {
            auto getDocumentFunc = getDocument.As<v8::Function>();
            v8::Local<v8::Value> args[] = {  };
            v8::TryCatch tc;

            auto maybeResult = getDocumentFunc->Call(context, global, 0, args);

            if (tc.HasCaught()) {
                *errorString = utils::Common::getJSCallErrorMessage(getDocumentFunctionString, tc.Message()->Get()).c_str();

                *out_root = std::move(defaultNode);
                return;
            }

            v8::Local<v8::Value> outResult;

            if (maybeResult.ToLocal(&outResult)) {
                auto resultString = ArgConverter::ConvertToUtf16String(outResult->ToString());

                auto resultUtf16Data = resultString.data();

                auto resultJson = protocol::parseJSON(String16((const uint16_t*) resultUtf16Data));

                protocol::ErrorSupport errorSupport;
                auto domNode = protocol::DOM::Node::parse(resultJson.get(), &errorSupport);

                auto errorSupportString = errorSupport.errors().utf8();
                *errorString = errorSupportString.c_str();
                if (!errorSupportString.empty()) {
                    auto errorMessage = "Error while parsing debug `DOM Node` object. ";
                    DEBUG_WRITE_FORCE("JS Error: %s", errorMessage, errorSupportString.c_str());
                } else {
                    *out_root = std::move(domNode);

                    return;
                }
            } else {
                *errorString = "Didn't get a proper result from __getDocument call. Returning empty visual tree.";
            }
        }
    }

    *out_root = std::move(defaultNode);
}

void V8DOMAgentImpl::removeNode(ErrorString* errorString, int in_nodeId) {
    std::string removeNodeFunctionString = "removeNode";

    // TODO: Pete: Find a better way to get a hold of the isolate
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        auto removeNode = globalInspectorObject->Get(ArgConverter::ConvertToV8String(isolate, removeNodeFunctionString));

        if (!removeNode.IsEmpty() && removeNode->IsFunction()) {
            auto removeNodeFunc = removeNode.As<v8::Function>();
            v8::Local<v8::Value> args[] = { v8::Number::New(isolate, in_nodeId) };
            v8::TryCatch tc;

            removeNodeFunc->Call(context, global, 1, args);

            if (tc.HasCaught()) {
                *errorString = utils::Common::getJSCallErrorMessage(removeNodeFunctionString, tc.Message()->Get()).c_str();
            }

            return;
        }
    }

    *errorString = "Couldn't remove the selected DOMNode from the visual tree.";
}

// Pete: return empty resolved object - prevents crashes when opening the 'properties', 'event listeners' tabs
// Not supported
void V8DOMAgentImpl::resolveNode(ErrorString*, int in_nodeId, const Maybe<String>& in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) {
    auto resolvedNode = protocol::Runtime::RemoteObject::create()
                        .setType("View")
                        .build();

    *out_object = std::move(resolvedNode);
}

void V8DOMAgentImpl::setAttributeValue(ErrorString* errorString, int in_nodeId, const String& in_name,
                                       const String& in_value) {
    // Irrelevant
}

void V8DOMAgentImpl::setAttributesAsText(ErrorString* errorString, int in_nodeId, const String& in_text,
        const Maybe<String>& in_name) {
    // call modules' View class methods to modify view's attribute
    // TODO: Pete: Find a better way to get a hold of the isolate
    std::string setAttributeAsTextFunctionString = "setAttributeAsText";
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        auto setAttributeAsText = globalInspectorObject->Get(ArgConverter::ConvertToV8String(isolate, setAttributeAsTextFunctionString));

        if (!setAttributeAsText.IsEmpty() && setAttributeAsText->IsFunction()) {
            auto setAttributeAsTextFunc = setAttributeAsText.As<v8::Function>();
            // TODO: Pete: Setting the content to contain utf-16 characters will still output garbage
            v8::Local<v8::Value> args[] = {
                v8::Number::New(isolate, in_nodeId),
                v8_inspector::toV8String(isolate, in_text),
                v8_inspector::toV8String(isolate, in_name.fromJust())
            };
            v8::TryCatch tc;

            setAttributeAsTextFunc->Call(context, global, 3, args);

            if (tc.HasCaught()) {
                *errorString = utils::Common::getJSCallErrorMessage(setAttributeAsTextFunctionString, tc.Message()->Get()).c_str();
            }

            return;
        }
    }
}

void V8DOMAgentImpl::removeAttribute(ErrorString* errorString, int in_nodeId, const String& in_name) {
    // Irrelevant
}

// Not supported
void V8DOMAgentImpl::performSearch(ErrorString*, const String& in_query,
                                   const Maybe<protocol::Array<int>>& in_nodeIds,
                                   String* out_searchId, int* out_resultCount) {

}

// Not supported
void V8DOMAgentImpl::getSearchResults(ErrorString*, const String& in_searchId, int in_fromIndex,
                                      int in_toIndex,
                                      std::unique_ptr<protocol::Array<int>>* out_nodeIds) {

}

// Not supported
void V8DOMAgentImpl::discardSearchResults(ErrorString*, const String& in_searchId) {

}

// Not supported
void V8DOMAgentImpl::highlightNode(ErrorString*,
                                   std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig,
                                   const Maybe<int>& in_nodeId,
                                   const Maybe<String>& in_objectId) {

}

void V8DOMAgentImpl::hideHighlight(ErrorString*) {

}

V8DOMAgentImpl* V8DOMAgentImpl::Instance = 0;
}

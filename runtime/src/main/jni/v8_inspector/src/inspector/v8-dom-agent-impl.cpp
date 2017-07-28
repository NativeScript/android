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

    DispatchResponse V8DOMAgentImpl::enable() {
        if (m_enabled) {
            return DispatchResponse::OK();
        }

        m_state->setBoolean(DOMAgentState::domEnabled, true);

        m_enabled = true;

        return DispatchResponse::OK();
    }

    DispatchResponse V8DOMAgentImpl::disable() {
        if (!m_enabled) {
            return DispatchResponse::OK();
        }

        m_state->setBoolean(DOMAgentState::domEnabled, false);

        m_enabled = false;

        return DispatchResponse::OK();
    }

    DispatchResponse V8DOMAgentImpl::getDocument(std::unique_ptr<protocol::DOM::Node>* out_root) {
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
                    auto error = utils::Common::getJSCallErrorMessage(getDocumentFunctionString, tc.Message()->Get()).c_str();

                    *out_root = std::move(defaultNode);
                    return DispatchResponse::Error(error);
                }

                v8::Local<v8::Value> outResult;

                if (maybeResult.ToLocal(&outResult)) {
                    auto resultString = ArgConverter::ConvertToUtf16String(outResult->ToString());

                    auto resultUtf16Data = resultString.data();

                    auto resultJson = protocol::StringUtil::parseJSON(String16((const uint16_t*) resultUtf16Data));

                    protocol::ErrorSupport errorSupport;
                    auto domNode = protocol::DOM::Node::fromValue(resultJson.get(), &errorSupport);

                    auto errorSupportString = errorSupport.errors().utf8();
                    if (!errorSupportString.empty()) {
                        auto errorMessage = "Error while parsing debug `DOM Node` object. ";
                        DEBUG_WRITE_FORCE("JS Error: %s", errorMessage, errorSupportString.c_str());
                        return DispatchResponse::Error(errorMessage);
                    } else {
                        *out_root = std::move(domNode);

                        return DispatchResponse::OK();
                    }
                } else {
                    return DispatchResponse::Error("Didn't get a proper result from __getDocument call. Returning empty visual tree.");
                }
            }
        }

        *out_root = std::move(defaultNode);
        return DispatchResponse::Error("Error getting DOM tree.");
    }

    DispatchResponse V8DOMAgentImpl::removeNode(int in_nodeId) {
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
                    auto error = utils::Common::getJSCallErrorMessage(removeNodeFunctionString, tc.Message()->Get()).c_str();
                    return DispatchResponse::Error(error);
                }

                return DispatchResponse::OK();
            }
        }

        return DispatchResponse::Error("Couldn't remove the selected DOMNode from the visual tree. Global Inspector object not found.");
    }

    DispatchResponse V8DOMAgentImpl::setAttributeValue(int in_nodeId, const String& in_name, const String& in_value) {
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

    DispatchResponse V8DOMAgentImpl::setAttributesAsText(int in_nodeId, const String& in_text, Maybe<String> in_name) {
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
                    auto error = utils::Common::getJSCallErrorMessage(setAttributeAsTextFunctionString, tc.Message()->Get()).c_str();
                    return DispatchResponse::Error(error);
                }

                return DispatchResponse::OK();
            }
        }

        return DispatchResponse::Error("Couldn't change selected DOM node's attribute. Global Inspector object not found.");
    }

    DispatchResponse V8DOMAgentImpl::removeAttribute(int in_nodeId, const String& in_name) {
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

    DispatchResponse V8DOMAgentImpl::performSearch(const String& in_query, Maybe<protocol::Array<int>> in_nodeIds, String* out_searchId, int* out_resultCount) {
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

    DispatchResponse V8DOMAgentImpl::getSearchResults(const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) {
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

    DispatchResponse V8DOMAgentImpl::discardSearchResults(const String& in_searchId) {
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

    DispatchResponse V8DOMAgentImpl::highlightNode(std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, Maybe<int> in_nodeId, Maybe<String> in_objectId) {
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

    DispatchResponse V8DOMAgentImpl::hideHighlight() {
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

    DispatchResponse V8DOMAgentImpl::resolveNode(int in_nodeId, Maybe<String> in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) {
        auto resolvedNode = protocol::Runtime::RemoteObject::create()
                .setType("View")
                .build();

        *out_object = std::move(resolvedNode);

        // TODO: Pete: See if it's best to return OK or Error
        return utils::Common::protocolCommandNotSupportedDispatchResponse();
    }

V8DOMAgentImpl* V8DOMAgentImpl::Instance = 0;
}

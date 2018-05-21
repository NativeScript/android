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

DispatchResponse V8DOMAgentImpl::getDocument(Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::DOM::Node>* out_root) {
    std::unique_ptr<protocol::DOM::Node> defaultNode = protocol::DOM::Node::create()
            .setNodeId(0)
            .setBackendNodeId(0)
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
            v8::TryCatch tc(isolate);

            auto maybeResult = getDocumentFunc->Call(context, global, 0, args);

            if (tc.HasCaught()) {
                auto error = utils::Common::getJSCallErrorMessage(getDocumentFunctionString, tc.Message()->Get()).c_str();

                *out_root = std::move(defaultNode);
                return DispatchResponse::Error(error);
            }

            v8::Local<v8::Value> outResult;

            if (maybeResult.ToLocal(&outResult)) {
                auto resultString = ArgConverter::ConvertToUtf16String(outResult->ToString());

                if (!outResult->ToObject()->Has(context, ArgConverter::ConvertToV8String(isolate, "backendNodeId")).FromMaybe(false)) {
                    // Using an older version of the modules which doesn't set the backendNodeId required property
                    resultString = AddBackendNodeIdProperty(isolate, outResult);
                }

                auto resultUtf16Data = resultString.data();

                auto resultJson = protocol::StringUtil::parseJSON(String16((const uint16_t*) resultUtf16Data));

                protocol::ErrorSupport errorSupport;
                auto domNode = protocol::DOM::Node::fromValue(resultJson.get(), &errorSupport);

                auto errorSupportString = errorSupport.errors().utf8();
                if (!errorSupportString.empty()) {
                    auto errorMessage = "Error while parsing debug `DOM Node` object. ";
                    DEBUG_WRITE_FORCE("JS Error: %s, Error support: %s", errorMessage, errorSupportString.c_str());
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
            v8::TryCatch tc(isolate);

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
            v8::TryCatch tc(isolate);

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

DispatchResponse V8DOMAgentImpl::performSearch(const String& in_query, Maybe<bool> in_includeUserAgentShadowDOM, String* out_searchId, int* out_resultCount) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getSearchResults(const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::discardSearchResults(const String& in_searchId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::resolveNode(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) {
    auto resolvedNode = protocol::Runtime::RemoteObject::create()
                        .setType("View")
                        .build();

    *out_object = std::move(resolvedNode);

    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::collectClassNamesFromSubtree(int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_classNames) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::copyTo(int in_nodeId, int in_targetNodeId, Maybe<int> in_insertBeforeNodeId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::describeNode(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::DOM::Node>* out_node) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::focus(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getAttributes(int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_attributes) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getBoxModel(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, std::unique_ptr<protocol::DOM::BoxModel>* out_model) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getFlattenedDocument(Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::Array<protocol::DOM::Node>>* out_nodes) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getNodeForLocation(int in_x, int in_y, Maybe<bool> in_includeUserAgentShadowDOM, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getOuterHTML(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, String* out_outerHTML) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getRelayoutBoundary(int in_nodeId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::markUndoableState() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::moveTo(int in_nodeId, int in_targetNodeId, Maybe<int> in_insertBeforeNodeId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::pushNodeByPathToFrontend(const String& in_path, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::pushNodesByBackendIdsToFrontend(std::unique_ptr<protocol::Array<int>> in_backendNodeIds, std::unique_ptr<protocol::Array<int>>* out_nodeIds) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::querySelector(int in_nodeId, const String& in_selector, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::querySelectorAll(int in_nodeId, const String& in_selector, std::unique_ptr<protocol::Array<int>>* out_nodeIds) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::redo() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::requestChildNodes(int in_nodeId, Maybe<int> in_depth, Maybe<bool> in_pierce) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::requestNode(const String& in_objectId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::setFileInputFiles(std::unique_ptr<protocol::Array<String>> in_files, Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::setInspectedNode(int in_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::setNodeName(int in_nodeId, const String& in_name, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::setNodeValue(int in_nodeId, const String& in_value) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::setOuterHTML(int in_nodeId, const String& in_outerHTML) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::undo() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8DOMAgentImpl::getFrameOwner(const String& in_frameId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

std::u16string V8DOMAgentImpl::AddBackendNodeIdProperty(v8::Isolate* isolate, v8::Local<v8::Value> jsonInput) {
    auto scriptSource =
        "(function () {"
        "   function addBackendNodeId(node) {"
        "       if (!node.backendNodeId) {"
        "           node.backendNodeId = 0;"
        "       }"
        "       if (node.children) {"
        "           for (var i = 0; i < node.children.length; i++) {"
        "               addBackendNodeId(node.children[i]);"
        "           }"
        "       }"
        "   }"
        "   return function(stringifiedNode) {"
        "       try {"
        "           const node = JSON.parse(stringifiedNode);"
        "           addBackendNodeId(node);"
        "           return JSON.stringify(node);"
        "       } catch (e) {"
        "           return stringifiedNode;"
        "       }"
        "   }"
        "})()";

    auto source = ArgConverter::ConvertToV8String(isolate, scriptSource);
    v8::Local<v8::Script> script;
    auto context = isolate->GetCurrentContext();
    v8::Script::Compile(context, source).ToLocal(&script);

    v8::Local<v8::Value> result;
    script->Run(context).ToLocal(&result);
    auto addBackendNodeIdFunction = result.As<v8::Function>();

    v8::Local<v8::Value> funcArguments[] = { jsonInput };
    v8::Local<v8::Value> scriptResult;
    addBackendNodeIdFunction->Call(context, context->Global(), 1, funcArguments).ToLocal(&scriptResult);

    auto resultString = ArgConverter::ConvertToUtf16String(scriptResult->ToString());
    return resultString;
}

V8DOMAgentImpl* V8DOMAgentImpl::Instance = 0;
}

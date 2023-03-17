//
// Created by pkanev on 4/24/2017.
//

#include <NativeScriptAssert.h>
#include <ArgConverter.h>
#include <v8_inspector/src/inspector/protocol/Runtime.h>
#include <v8_inspector/third_party/inspector_protocol/crdtp/json.h>

#include "DOMAgentImpl.h"
#include "utils/InspectorCommon.h"

namespace tns {

namespace DOMAgentState {
static const char domEnabled[] = "domEnabled";
}

DOMAgentImpl::DOMAgentImpl(V8InspectorSessionImpl* session,
                           protocol::FrontendChannel* frontendChannel,
                           protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

DOMAgentImpl::~DOMAgentImpl() { }

DispatchResponse DOMAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(DOMAgentState::domEnabled, true);

    m_enabled = true;

    return DispatchResponse::Success();
}

DispatchResponse DOMAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(DOMAgentState::domEnabled, false);

    m_enabled = false;

    return DispatchResponse::Success();
}

DispatchResponse DOMAgentImpl::getContentQuads(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, std::unique_ptr<protocol::Array<protocol::Array<double>>>* out_quads) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getDocument(Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::DOM::Node>* out_root) {
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
        v8::Local<v8::Value> getDocument;
        globalInspectorObject->Get(context, ArgConverter::ConvertToV8String(isolate, getDocumentFunctionString)).ToLocal(&getDocument);

        if (!getDocument.IsEmpty() && getDocument->IsFunction()) {
            auto getDocumentFunc = getDocument.As<v8::Function>();
            v8::Local<v8::Value> args[] = {  };
            v8::TryCatch tc(isolate);

            auto maybeResult = getDocumentFunc->Call(context, global, 0, args);

            if (tc.HasCaught()) {
                auto error = utils::Common::getJSCallErrorMessage(getDocumentFunctionString, tc.Message()->Get());

                *out_root = std::move(defaultNode);
                return DispatchResponse::ServerError(error);
            }

            v8::Local<v8::Value> outResult;

            if (maybeResult.ToLocal(&outResult)) {
                auto resultString = ArgConverter::ConvertToUtf16String(outResult->ToString(context).ToLocalChecked());

                if (!outResult->ToObject(context).ToLocalChecked()->Has(context, ArgConverter::ConvertToV8String(isolate, "backendNodeId")).FromMaybe(false)) {
                    // Using an older version of the modules which doesn't set the backendNodeId required property
                    resultString = AddBackendNodeIdProperty(isolate, outResult);
                }

                auto resultUtf16Data = resultString.data();
                v8_inspector::String16 resultProtocolString = v8_inspector::String16((const uint16_t*) resultUtf16Data);
                std::vector<uint8_t> cbor;
                v8_crdtp::json::ConvertJSONToCBOR(v8_crdtp::span<uint16_t>(resultProtocolString.characters16(), resultProtocolString.length()), &cbor);
                std::unique_ptr<protocol::Value> resultJson = protocol::Value::parseBinary(cbor.data(), cbor.size());
                protocol::ErrorSupport errorSupport;
                std::unique_ptr<protocol::DOM::Node> domNode = protocol::DOM::Node::fromValue(resultJson.get(), &errorSupport);

                std::vector<uint8_t> json;
                v8_crdtp::json::ConvertCBORToJSON(errorSupport.Errors(), &json);
                auto errorSupportString = v8_inspector::String16(reinterpret_cast<const char*>(json.data()), json.size()).utf8();
                if (!errorSupportString.empty()) {
                    auto errorMessage = "Error while parsing debug `DOM Node` object. ";
                    DEBUG_WRITE_FORCE("JS Error: %s, Error support: %s", errorMessage, errorSupportString.c_str());
                    return DispatchResponse::ServerError(errorMessage);
                } else {
                    *out_root = std::move(domNode);

                    return DispatchResponse::Success();
                }
            } else {
                return DispatchResponse::ServerError("Didn't get a proper result from __getDocument call. Returning empty visual tree.");
            }
        }
    }

    *out_root = std::move(defaultNode);
    return DispatchResponse::ServerError("Error getting DOM tree.");
}

DispatchResponse DOMAgentImpl::removeNode(int in_nodeId) {
    std::string removeNodeFunctionString = "removeNode";

    // TODO: Pete: Find a better way to get a hold of the isolate
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        v8::Local<v8::Value> removeNode;
        globalInspectorObject->Get(context, ArgConverter::ConvertToV8String(isolate, removeNodeFunctionString)).ToLocal(&removeNode);

        if (!removeNode.IsEmpty() && removeNode->IsFunction()) {
            auto removeNodeFunc = removeNode.As<v8::Function>();
            v8::Local<v8::Value> args[] = { v8::Number::New(isolate, in_nodeId) };
            v8::TryCatch tc(isolate);

            removeNodeFunc->Call(context, global, 1, args);

            if (tc.HasCaught()) {
                auto error = utils::Common::getJSCallErrorMessage(removeNodeFunctionString, tc.Message()->Get());
                return DispatchResponse::ServerError(error);
            }

            return DispatchResponse::Success();
        }
    }

    return DispatchResponse::ServerError("Couldn't remove the selected DOMNode from the visual tree. Global Inspector object not found.");
}

DispatchResponse DOMAgentImpl::setAttributeValue(int in_nodeId, const String& in_name, const String& in_value) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::setAttributesAsText(int in_nodeId, const String& in_text, Maybe<String> in_name) {
    // call modules' View class methods to modify view's attribute
    // TODO: Pete: Find a better way to get a hold of the isolate
    std::string setAttributeAsTextFunctionString = "setAttributeAsText";
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        v8::Local<v8::Value> setAttributeAsText;
        globalInspectorObject->Get(context, ArgConverter::ConvertToV8String(isolate, setAttributeAsTextFunctionString)).ToLocal(&setAttributeAsText);

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
                auto error = utils::Common::getJSCallErrorMessage(setAttributeAsTextFunctionString, tc.Message()->Get());
                return DispatchResponse::ServerError(error);
            }

            return DispatchResponse::Success();
        }
    }

    return DispatchResponse::ServerError("Couldn't change selected DOM node's attribute. Global Inspector object not found.");
}

DispatchResponse DOMAgentImpl::removeAttribute(int in_nodeId, const String& in_name) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::performSearch(const String& in_query, Maybe<bool> in_includeUserAgentShadowDOM, String* out_searchId, int* out_resultCount) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getSearchResults(const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::discardSearchResults(const String& in_searchId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::resolveNode(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectGroup, Maybe<int> in_executionContextId, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) {
    auto resolvedNode = protocol::Runtime::RemoteObject::create()
                        .setType("View")
                        .build();

    *out_object = std::move(resolvedNode);

    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::collectClassNamesFromSubtree(int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_classNames) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::copyTo(int in_nodeId, int in_targetNodeId, Maybe<int> in_insertBeforeNodeId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::describeNode(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::DOM::Node>* out_node) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::focus(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getAttributes(int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_attributes) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getBoxModel(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, std::unique_ptr<protocol::DOM::BoxModel>* out_model) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getFlattenedDocument(Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::Array<protocol::DOM::Node>>* out_nodes) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getNodeForLocation(int in_x, int in_y, Maybe<bool> in_includeUserAgentShadowDOM, int* out_backendNodeId, Maybe<int>* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getOuterHTML(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, String* out_outerHTML) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getRelayoutBoundary(int in_nodeId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::markUndoableState() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::moveTo(int in_nodeId, int in_targetNodeId, Maybe<int> in_insertBeforeNodeId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::pushNodeByPathToFrontend(const String& in_path, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::pushNodesByBackendIdsToFrontend(std::unique_ptr<protocol::Array<int>> in_backendNodeIds, std::unique_ptr<protocol::Array<int>>* out_nodeIds) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::querySelector(int in_nodeId, const String& in_selector, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::querySelectorAll(int in_nodeId, const String& in_selector, std::unique_ptr<protocol::Array<int>>* out_nodeIds) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::redo() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::requestChildNodes(int in_nodeId, Maybe<int> in_depth, Maybe<bool> in_pierce) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::requestNode(const String& in_objectId, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::setFileInputFiles(std::unique_ptr<protocol::Array<String>> in_files, Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getFileInfo(const String& in_objectId, String* out_path) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::setInspectedNode(int in_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::setNodeName(int in_nodeId, const String& in_name, int* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::setNodeValue(int in_nodeId, const String& in_value) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::setOuterHTML(int in_nodeId, const String& in_outerHTML) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::undo() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse DOMAgentImpl::getFrameOwner(const String& in_frameId, int* out_backendNodeId, Maybe<int>* out_nodeId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

std::u16string DOMAgentImpl::AddBackendNodeIdProperty(v8::Isolate* isolate, v8::Local<v8::Value> jsonInput) {
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

    auto resultString = ArgConverter::ConvertToUtf16String(scriptResult->ToString(context).ToLocalChecked());
    return resultString;
}

DOMAgentImpl* DOMAgentImpl::Instance = 0;
}  // namespace tns

//
// Created by pkanev on 4/24/2017.
//

#ifndef V8_DOM_AGENT_IMPL_H
#define V8_DOM_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/DOM.h>
#include <v8_inspector/src/inspector/protocol/Protocol.h>

namespace v8_inspector {

class V8InspectorSessionImpl;

using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;
using v8_inspector::protocol::DispatchResponse;

class V8DOMAgentImpl : public protocol::DOM::Backend {
    public:
        V8DOMAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                       protocol::DictionaryValue* state);

        ~V8DOMAgentImpl() override;

        virtual DispatchResponse enable() override;
        virtual DispatchResponse disable() override;
        virtual DispatchResponse getDocument(Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::DOM::Node>* out_root) override;
        virtual DispatchResponse removeNode(int in_nodeId) override;
        virtual DispatchResponse setAttributeValue(int in_nodeId, const String& in_name, const String& in_value) override;
        virtual DispatchResponse setAttributesAsText(int in_nodeId, const String& in_text, Maybe<String> in_name) override;
        virtual DispatchResponse removeAttribute(int in_nodeId, const String& in_name) override;
        virtual DispatchResponse performSearch(const String& in_query, Maybe<bool> in_includeUserAgentShadowDOM, String* out_searchId, int* out_resultCount) override;
        virtual DispatchResponse getSearchResults(const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) override;
        virtual DispatchResponse discardSearchResults(const String& in_searchId) override;
        virtual DispatchResponse resolveNode(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) override;

        DispatchResponse collectClassNamesFromSubtree(int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_classNames) override;
        DispatchResponse copyTo(int in_nodeId, int in_targetNodeId, Maybe<int> in_insertBeforeNodeId, int* out_nodeId) override;
        DispatchResponse describeNode(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::DOM::Node>* out_node) override;
        DispatchResponse focus(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId) override;
        DispatchResponse getAttributes(int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_attributes) override;
        DispatchResponse getBoxModel(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, std::unique_ptr<protocol::DOM::BoxModel>* out_model) override;
        DispatchResponse getFlattenedDocument(Maybe<int> in_depth, Maybe<bool> in_pierce, std::unique_ptr<protocol::Array<protocol::DOM::Node>>* out_nodes) override;
        DispatchResponse getNodeForLocation(int in_x, int in_y, Maybe<bool> in_includeUserAgentShadowDOM, int* out_nodeId) override;
        DispatchResponse getOuterHTML(Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId, String* out_outerHTML) override;
        DispatchResponse getRelayoutBoundary(int in_nodeId, int* out_nodeId) override;
        DispatchResponse markUndoableState() override;
        DispatchResponse moveTo(int in_nodeId, int in_targetNodeId, Maybe<int> in_insertBeforeNodeId, int* out_nodeId) override;
        DispatchResponse pushNodeByPathToFrontend(const String& in_path, int* out_nodeId) override;
        DispatchResponse pushNodesByBackendIdsToFrontend(std::unique_ptr<protocol::Array<int>> in_backendNodeIds, std::unique_ptr<protocol::Array<int>>* out_nodeIds) override;
        DispatchResponse querySelector(int in_nodeId, const String& in_selector, int* out_nodeId) override;
        DispatchResponse querySelectorAll(int in_nodeId, const String& in_selector, std::unique_ptr<protocol::Array<int>>* out_nodeIds) override;
        DispatchResponse redo() override;
        DispatchResponse requestChildNodes(int in_nodeId, Maybe<int> in_depth, Maybe<bool> in_pierce) override;
        DispatchResponse requestNode(const String& in_objectId, int* out_nodeId) override;
        DispatchResponse setFileInputFiles(std::unique_ptr<protocol::Array<String>> in_files, Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId) override;
        DispatchResponse setInspectedNode(int in_nodeId) override;
        DispatchResponse setNodeName(int in_nodeId, const String& in_name, int* out_nodeId) override;
        DispatchResponse setNodeValue(int in_nodeId, const String& in_value) override;
        DispatchResponse setOuterHTML(int in_nodeId, const String& in_outerHTML) override;
        DispatchResponse undo() override;
        DispatchResponse getFrameOwner(const String& in_frameId, int* out_nodeId) override;

        const bool enabled() {
            return m_enabled;
        };

        static V8DOMAgentImpl* Instance;
        protocol::DOM::Frontend m_frontend;

        static std::u16string AddBackendNodeIdProperty(v8::Isolate* isolate, v8::Local<v8::Value> jsonInput);
    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;

        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8DOMAgentImpl);
};
}

#endif //V8_DOM_AGENT_IMPL_H
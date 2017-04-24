//
// Created by pkanev on 4/24/2017.
//

#ifndef V8_DOM_AGENT_IMPL_H
#define V8_DOM_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/DOM.h>

namespace v8_inspector {

    class V8InspectorSessionImpl;

    using protocol::ErrorString;
    using v8_inspector::protocol::Maybe;
    using String = v8_inspector::String16;

    class V8DOMAgentImpl : public protocol::DOM::Backend {
    public:
        V8DOMAgentImpl(V8InspectorSessionImpl *, protocol::FrontendChannel *,
                           protocol::DictionaryValue *state);

        ~V8DOMAgentImpl() override;

        void enable(ErrorString*) override;
        void disable(ErrorString*) override;

        void getDocument(ErrorString*, std::unique_ptr<protocol::DOM::Node>* out_root) override;
        void requestChildNodes(ErrorString*, int in_nodeId, const Maybe<int>& in_depth) { };
        void querySelector(ErrorString*, int in_nodeId, const String& in_selector, int* out_nodeId) { };
        void querySelectorAll(ErrorString*, int in_nodeId, const String& in_selector, std::unique_ptr<protocol::Array<int>>* out_nodeIds) { };
        void setNodeName(ErrorString*, int in_nodeId, const String& in_name, int* out_nodeId) { };
        void setNodeValue(ErrorString*, int in_nodeId, const String& in_value) { };
        void removeNode(ErrorString*, int in_nodeId) { };
        void setAttributeValue(ErrorString*, int in_nodeId, const String& in_name, const String& in_value) { };
        void setAttributesAsText(ErrorString*, int in_nodeId, const String& in_text, const Maybe<String>& in_name) { };
        void removeAttribute(ErrorString*, int in_nodeId, const String& in_name) { };
        void getOuterHTML(ErrorString*, int in_nodeId, String* out_outerHTML) { };
        void setOuterHTML(ErrorString*, int in_nodeId, const String& in_outerHTML) { };
        void performSearch(ErrorString*, const String& in_query, const Maybe<protocol::Array<int>>& in_nodeIds, String* out_searchId, int* out_resultCount) { };
        void getSearchResults(ErrorString*, const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) { };
        void discardSearchResults(ErrorString*, const String& in_searchId) { };
        void requestNode(ErrorString*, const String& in_objectId, int* out_nodeId) { };
        void setInspectModeEnabled(ErrorString*, bool in_enabled, const Maybe<protocol::DOM::HighlightConfig>& in_highlightConfig) { };
        void highlightRect(ErrorString*, int in_x, int in_y, int in_width, int in_height, const Maybe<protocol::DOM::RGBAColor>& in_color, const Maybe<protocol::DOM::RGBAColor>& in_outlineColor, const Maybe<bool>& in_usePageCoordinates) { };
        void highlightQuad(ErrorString*, std::unique_ptr<protocol::Array<double>> in_quad, const Maybe<protocol::DOM::RGBAColor>& in_color, const Maybe<protocol::DOM::RGBAColor>& in_outlineColor, const Maybe<bool>& in_usePageCoordinates) { };
        void highlightSelector(ErrorString*, std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, const String& in_selectorString, const Maybe<String>& in_frameId) { };
        void highlightNode(ErrorString*, std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, const Maybe<int>& in_nodeId, const Maybe<String>& in_objectId) { };
        void hideHighlight(ErrorString*) { };
        void highlightFrame(ErrorString*, const String& in_frameId, const Maybe<protocol::DOM::RGBAColor>& in_contentColor, const Maybe<protocol::DOM::RGBAColor>& in_contentOutlineColor) { };
        void pushNodeByPathToFrontend(ErrorString*, const String& in_path, int* out_nodeId) { };
        void pushNodeByBackendIdToFrontend(ErrorString*, int in_backendNodeId, int* out_nodeId) { };
        void releaseBackendNodeIds(ErrorString*, const String& in_nodeGroup) { };
        void resolveNode(ErrorString*, int in_nodeId, const Maybe<String>& in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) { };
        void getAttributes(ErrorString*, int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_attributes) { };
        void moveTo(ErrorString*, int in_nodeId, int in_targetNodeId, const Maybe<int>& in_insertBeforeNodeId, int* out_nodeId) { };
        void undo(ErrorString*) { };
        void redo(ErrorString*) { };
        void markUndoableState(ErrorString*) { };
        void focus(ErrorString*, int in_nodeId) { };

        const bool enabled() {
            return m_enabled;
        };

        protocol::DOM::Frontend m_frontend;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8DOMAgentImpl);
    };
}

#endif //V8_DOM_AGENT_IMPL_H
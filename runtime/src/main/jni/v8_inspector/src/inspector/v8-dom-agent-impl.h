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
        V8DOMAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                       protocol::DictionaryValue* state);

        ~V8DOMAgentImpl() override;

        void enable(ErrorString*) override;
        void disable(ErrorString*) override;

        void getDocument(ErrorString*, std::unique_ptr<protocol::DOM::Node>* out_root) override;
        void removeNode(ErrorString*, int in_nodeId)override;
        void setAttributeValue(ErrorString*, int in_nodeId, const String& in_name, const String& in_value) override;
        void setAttributesAsText(ErrorString*, int in_nodeId, const String& in_text, const Maybe<String>& in_name) override;
        void removeAttribute(ErrorString*, int in_nodeId, const String& in_name) override;
        void performSearch(ErrorString*, const String& in_query, const Maybe<protocol::Array<int>>& in_nodeIds, String* out_searchId, int* out_resultCount) override;
        void getSearchResults(ErrorString*, const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) override;
        void discardSearchResults(ErrorString*, const String& in_searchId) override;
        void highlightNode(ErrorString*, std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, const Maybe<int>& in_nodeId, const Maybe<String>& in_objectId) override;
        void hideHighlight(ErrorString*) override;
        void resolveNode(ErrorString*, int in_nodeId, const Maybe<String>& in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) override;

        const bool enabled() {
            return m_enabled;
        };

        static V8DOMAgentImpl* Instance;
        protocol::DOM::Frontend m_frontend;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;

        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8DOMAgentImpl);
};
}

#endif //V8_DOM_AGENT_IMPL_H
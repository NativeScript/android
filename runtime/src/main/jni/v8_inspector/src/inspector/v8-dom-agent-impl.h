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
        V8DOMAgentImpl(V8InspectorSessionImpl *, protocol::FrontendChannel *,
                           protocol::DictionaryValue *state);

        ~V8DOMAgentImpl() override;

        virtual DispatchResponse enable() override;
        virtual DispatchResponse disable() override;
        virtual DispatchResponse getDocument(std::unique_ptr<protocol::DOM::Node>* out_root) override;
        virtual DispatchResponse removeNode(int in_nodeId) override;
        virtual DispatchResponse setAttributeValue(int in_nodeId, const String& in_name, const String& in_value) override;
        virtual DispatchResponse setAttributesAsText(int in_nodeId, const String& in_text, Maybe<String> in_name) override;
        virtual DispatchResponse removeAttribute(int in_nodeId, const String& in_name) override;
        virtual DispatchResponse performSearch(const String& in_query, Maybe<protocol::Array<int>> in_nodeIds, String* out_searchId, int* out_resultCount) override;
        virtual DispatchResponse getSearchResults(const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) override;
        virtual DispatchResponse discardSearchResults(const String& in_searchId) override;
        virtual DispatchResponse highlightNode(std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, Maybe<int> in_nodeId, Maybe<String> in_objectId) override;
        virtual DispatchResponse hideHighlight() override;
        virtual DispatchResponse resolveNode(int in_nodeId, Maybe<String> in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) override;

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
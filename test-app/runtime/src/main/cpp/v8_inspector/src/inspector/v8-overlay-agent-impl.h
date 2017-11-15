//
// Created by pkanev on 11/15/2017.
//

#ifndef V8_OVERLAY_AGENT_IMPL_H
#define V8_OVERLAY_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/Overlay.h>

namespace v8_inspector {

class V8InspectorSessionImpl;

using protocol::ErrorString;
using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;


class V8OverlayAgentImpl : public protocol::Overlay::Backend {
    public:
        V8OverlayAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                           protocol::DictionaryValue* state);

        ~V8OverlayAgentImpl() override;
        void enable(ErrorString*) override;
        void disable(ErrorString*) override;
        void setShowFPSCounter(ErrorString*, bool in_show) override;
        void setPausedInDebuggerMessage(ErrorString*, const Maybe<String>& in_message) override;
        void highlightNode(ErrorString*, std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig, const Maybe<int>& in_nodeId, const Maybe<int>& in_backendNodeId, const Maybe<String>& in_objectId) override;
        void highlightFrame(ErrorString*, const String& in_frameId, const Maybe<protocol::DOM::RGBA>& in_contentColor, const Maybe<protocol::DOM::RGBA>& in_contentOutlineColor) override;
        void hideHighlight(ErrorString*) override;
        void getHighlightObjectForTest(ErrorString*, int in_nodeId, std::unique_ptr<protocol::DictionaryValue>* out_highlight) override;

        protocol::Overlay::Frontend m_frontend;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8OverlayAgentImpl);
};

}

#endif //V8_OVERLAY_AGENT_IMPL_H

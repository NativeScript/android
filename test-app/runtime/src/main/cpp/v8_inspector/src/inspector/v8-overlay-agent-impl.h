//
// Created by pkanev on 11/15/2017.
//

#ifndef V8_OVERLAY_AGENT_IMPL_H
#define V8_OVERLAY_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/Overlay.h>
#include <Util.h>

namespace v8_inspector {

class V8InspectorSessionImpl;

using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;
using v8_inspector::protocol::DispatchResponse;


class V8OverlayAgentImpl : public protocol::Overlay::Backend {
    public:
        V8OverlayAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                           protocol::DictionaryValue* state);

        ~V8OverlayAgentImpl() override;
        DispatchResponse enable() override;
        DispatchResponse disable() override;
        DispatchResponse setShowFPSCounter(bool in_show) override;
        DispatchResponse setPausedInDebuggerMessage(const Maybe<String> in_message) override;
        DispatchResponse highlightNode(std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig, const Maybe<int> in_nodeId, const Maybe<int> in_backendNodeId, const Maybe<String> in_objectId) override;
        DispatchResponse highlightFrame(const String& in_frameId, Maybe<protocol::DOM::RGBA> in_contentColor, Maybe<protocol::DOM::RGBA> in_contentOutlineColor) override;
        DispatchResponse hideHighlight() override;
        DispatchResponse getHighlightObjectForTest(int in_nodeId, std::unique_ptr<protocol::DictionaryValue>* out_highlight) override;
        DispatchResponse highlightQuad(std::unique_ptr<protocol::Array<double>> in_quad, Maybe<protocol::DOM::RGBA> in_color, Maybe<protocol::DOM::RGBA> in_outlineColor) override;
        DispatchResponse highlightRect(int in_x, int in_y, int in_width, int in_height, Maybe<protocol::DOM::RGBA> in_color, Maybe<protocol::DOM::RGBA> in_outlineColor) override;
        DispatchResponse setInspectMode(const String& in_mode, Maybe<protocol::Overlay::HighlightConfig> in_highlightConfig) override;
        DispatchResponse setShowDebugBorders(bool in_show) override;
        DispatchResponse setShowPaintRects(bool in_result) override;
        DispatchResponse setShowScrollBottleneckRects(bool in_show) override;
        DispatchResponse setShowViewportSizeOnResize(bool in_show) override;
        DispatchResponse setSuspended(bool in_suspended) override;

        protocol::Overlay::Frontend m_frontend;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8OverlayAgentImpl);
};

}

#endif //V8_OVERLAY_AGENT_IMPL_H

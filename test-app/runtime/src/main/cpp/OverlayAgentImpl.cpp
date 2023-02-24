//
// Created by pkanev on 11/15/2017.
//

#include <ArgConverter.h>
#include "utils/InspectorCommon.h"
#include "OverlayAgentImpl.h"

namespace tns {

namespace OverlayAgentState {
static const char overlayEnabled[] = "overlayEnabled";
}

OverlayAgentImpl::OverlayAgentImpl(V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
                                   protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {}

OverlayAgentImpl::~OverlayAgentImpl() { }

DispatchResponse OverlayAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::ServerError("Overlay Agent already enabled!");
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, true);
    m_enabled = true;

    return DispatchResponse::Success();
}

DispatchResponse OverlayAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, false);

    m_enabled = false;

    return DispatchResponse::Success();
}

DispatchResponse OverlayAgentImpl::setShowFPSCounter(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setPausedInDebuggerMessage(const Maybe<String> in_message) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setShowAdHighlights(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::highlightNode(std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig,
        Maybe<int> in_nodeId,
        Maybe<int> in_backendNodeId,
        Maybe<String> in_objectId,
        Maybe<String> in_selector) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::highlightFrame(const String& in_frameId,
        Maybe<protocol::DOM::RGBA> in_contentColor,
        Maybe<protocol::DOM::RGBA> in_contentOutlineColor) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::hideHighlight() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::getHighlightObjectForTest(int in_nodeId,
        std::unique_ptr<protocol::DictionaryValue>* out_highlight) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::highlightQuad(std::unique_ptr<protocol::Array<double>> in_quad, Maybe<protocol::DOM::RGBA> in_color, Maybe<protocol::DOM::RGBA> in_outlineColor) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::highlightRect(int in_x, int in_y, int in_width, int in_height, Maybe<protocol::DOM::RGBA> in_color, Maybe<protocol::DOM::RGBA> in_outlineColor) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setInspectMode(const String& in_mode, Maybe<protocol::Overlay::HighlightConfig> in_highlightConfig) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setShowDebugBorders(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setShowPaintRects(bool in_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setShowScrollBottleneckRects(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setShowHitTestBorders(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setShowViewportSizeOnResize(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse OverlayAgentImpl::setSuspended(bool in_suspended) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

}  // namespace tns

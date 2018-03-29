//
// Created by pkanev on 11/15/2017.
//

#include <ArgConverter.h>
#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
#include "v8-overlay-agent-impl.h"

namespace v8_inspector {

using tns::ArgConverter;

namespace OverlayAgentState {
static const char overlayEnabled[] = "overlayEnabled";
}

V8OverlayAgentImpl::V8OverlayAgentImpl(V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
                                       protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {}

V8OverlayAgentImpl::~V8OverlayAgentImpl() { }

DispatchResponse V8OverlayAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::Error("Overlay Agent already enabled!");
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, true);
    m_enabled = true;

    return DispatchResponse::OK();
}

DispatchResponse V8OverlayAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::OK();
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, false);

    m_enabled = false;

    return DispatchResponse::OK();
}

DispatchResponse V8OverlayAgentImpl::setShowFPSCounter(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setPausedInDebuggerMessage(const Maybe<String> in_message) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::highlightNode(std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig,
        const Maybe<int> in_nodeId,
        const Maybe<int> in_backendNodeId,
        const Maybe<String> in_objectId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::highlightFrame(const String& in_frameId,
        const Maybe<protocol::DOM::RGBAColor> in_contentColor,
        const Maybe<protocol::DOM::RGBAColor> in_contentOutlineColor) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::hideHighlight() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::getHighlightObjectForTest(int in_nodeId,
        std::unique_ptr<protocol::DictionaryValue>* out_highlight) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}
}

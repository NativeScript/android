//
// Created by pkanev on 11/15/2017.
//

#include <NativeScriptAssert.h>
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
      m_enabled(false) {
}

V8OverlayAgentImpl::~V8OverlayAgentImpl() { }

void V8OverlayAgentImpl::enable(ErrorString* errorString) {
    if (m_enabled) {
        *errorString = "Overlay Agent already enabled!";
        return;
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, true);
    m_enabled = true;
}

void V8OverlayAgentImpl::disable(ErrorString*) {
    if (!m_enabled) {
        return;
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, false);

    m_enabled = false;
}

void V8OverlayAgentImpl::setShowFPSCounter(ErrorString* errorString, bool in_show) {
    *errorString = "setShowFPSCounter not implemented.";
}

void
V8OverlayAgentImpl::setPausedInDebuggerMessage(ErrorString* errorString, const Maybe<String>& in_message) {
    *errorString = "setPausedInDebuggerMessage not implemented.";
}

void V8OverlayAgentImpl::highlightNode(ErrorString* errorString,
                                       std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig,
                                       const Maybe<int>& in_nodeId,
                                       const Maybe<int>& in_backendNodeId,
                                       const Maybe<String>& in_objectId) {
    *errorString = "highlightNode not implemented.";
}

void V8OverlayAgentImpl::highlightFrame(ErrorString* errorString, const String& in_frameId,
                                        const Maybe<protocol::DOM::RGBA>& in_contentColor,
                                        const Maybe<protocol::DOM::RGBA>& in_contentOutlineColor) {
    *errorString = "highlightNode not implemented.";
}

void V8OverlayAgentImpl::hideHighlight(ErrorString* errorString) {
    *errorString = "hideHighlight not implemented.";
}

void V8OverlayAgentImpl::getHighlightObjectForTest(ErrorString* errorString, int in_nodeId,
        std::unique_ptr<protocol::DictionaryValue>* out_highlight) {
    *errorString = "getHighlightObjectForTest not implemented.";
}
}

//
// Created by pkanev on 4/24/2017.
//

#include "v8-dom-agent-impl.h"

namespace v8_inspector {

    namespace DOMAgentState {
        static const char domEnabled[] = "domEnabled";
    }

    V8DOMAgentImpl::V8DOMAgentImpl(V8InspectorSessionImpl *session,
                                                 protocol::FrontendChannel *frontendChannel,
                                                 protocol::DictionaryValue *state)
        : m_session(session),
          m_frontend(frontendChannel),
          m_state(state),
          m_enabled(false) { }

    V8DOMAgentImpl::~V8DOMAgentImpl() { }

    void V8DOMAgentImpl::enable(ErrorString*) {
        if (m_enabled) {
            return;
        }

        m_state->setBoolean(DOMAgentState::domEnabled, true);

        m_enabled = true;
    }

    void V8DOMAgentImpl::disable(ErrorString*) {
        if (!m_enabled) {
            return;
        }

        m_state->setBoolean(DOMAgentState::domEnabled, false);

        m_enabled = false;
    }

    void V8DOMAgentImpl::getDocument(ErrorString *errorString, std::unique_ptr<protocol::DOM::Node>* out_root) {

    };
}

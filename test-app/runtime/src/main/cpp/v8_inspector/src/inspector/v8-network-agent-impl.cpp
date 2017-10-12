//
// Created by pkanev on 2/22/2017.
//

#include <assert.h>
#include "v8-network-agent-impl.h"

namespace v8_inspector {

namespace NetworkAgentState {
static const char networkEnabled[] = "networkEnabled";
}

V8NetworkAgentImpl::V8NetworkAgentImpl(V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
                                       protocol::DictionaryValue* state)
    : m_responses(),
      m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

V8NetworkAgentImpl::~V8NetworkAgentImpl() {}

void V8NetworkAgentImpl::enable(ErrorString*) {
    if (m_enabled) {
        return;
    }

    m_state->setBoolean(NetworkAgentState::networkEnabled, true);

    m_enabled = true;
}

void V8NetworkAgentImpl::disable(ErrorString*) {
    if (!m_enabled) {
        return;
    }

    m_state->setBoolean(NetworkAgentState::networkEnabled, false);

    m_enabled = false;
}

void V8NetworkAgentImpl::setExtraHTTPHeaders(ErrorString* errorString, std::unique_ptr<protocol::Network::Headers> in_headers) {
    // not relevant in mobile context
    *errorString = "Cannot set extra http headers for mobile requests.";
}

void V8NetworkAgentImpl::getResponseBody(ErrorString* errorString, const String& in_requestId, String* out_body, bool* out_base64Encoded) {
    auto it = m_responses.find(in_requestId.utf8());

    if (it == m_responses.end()) {
        *errorString = "Response not found for requestId = " + in_requestId;
    } else {
        v8_inspector::utils::NetworkRequestData* response = it->second;
        *out_body = String16((const uint16_t*) response->getData());
        *out_base64Encoded = !response->hasTextContent();
    }
}

void V8NetworkAgentImpl::setCacheDisabled(ErrorString* errorString, bool in_cacheDisabled) {
    // not relevant in mobile context
    *errorString = "Requests are not cached.";
}

void V8NetworkAgentImpl::loadResource(const String& in_frameId, const String& in_url, std::unique_ptr<protocol::Network::Backend::LoadResourceCallback> callback) {
    // not relevant in mobile context
    assert("'loadResource' not called");
}

V8NetworkAgentImpl* V8NetworkAgentImpl::Instance = 0;
}
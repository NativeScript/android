//
// Created by pkanev on 2/22/2017.
//

#include <assert.h>
#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
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

///////

DispatchResponse V8NetworkAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::OK();
    }

    m_state->setBoolean(NetworkAgentState::networkEnabled, true);

    m_enabled = true;

    return DispatchResponse::OK();
}

DispatchResponse V8NetworkAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::OK();
    }

    m_state->setBoolean(NetworkAgentState::networkEnabled, false);

    m_enabled = false;

    return DispatchResponse::OK();
}

DispatchResponse V8NetworkAgentImpl::setExtraHTTPHeaders(std::unique_ptr<protocol::Network::Headers> in_headers) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::getResponseBody(const String& in_requestId, String* out_body, bool* out_base64Encoded) {
    auto it = m_responses.find(in_requestId.utf8());

    if (it == m_responses.end()) {
        auto error = "Response not found for requestId = " + in_requestId;
        return DispatchResponse::Error(error);
    } else {
        v8_inspector::utils::NetworkRequestData* response = it->second;
        *out_body = String16((const uint16_t*) response->getData());
        *out_base64Encoded = !response->hasTextContent();
    }

    return DispatchResponse::OK();
}

DispatchResponse V8NetworkAgentImpl::setCacheDisabled(bool in_cacheDisabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::loadResource(const String& in_frameId, const String& in_url, String* out_content, String* out_mimeType, double* out_status) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

//////

V8NetworkAgentImpl* V8NetworkAgentImpl::Instance = 0;
}
//
// Created by pkanev on 2/22/2017.
//

#include <assert.h>

#include "utils/InspectorCommon.h"
#include "NetworkAgentImpl.h"

namespace tns {

namespace NetworkAgentState {
static const char networkEnabled[] = "networkEnabled";
}

NetworkAgentImpl::NetworkAgentImpl(V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
                                   protocol::DictionaryValue* state)
    : m_responses(),
      m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

NetworkAgentImpl::~NetworkAgentImpl() {}

///////

DispatchResponse NetworkAgentImpl::enable(Maybe<int> in_maxTotalBufferSize, Maybe<int> in_maxResourceBufferSize, Maybe<int> in_maxPostDataSize) {
    if (m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(NetworkAgentState::networkEnabled, true);

    m_enabled = true;

    return DispatchResponse::Success();
}

DispatchResponse NetworkAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(NetworkAgentState::networkEnabled, false);

    m_enabled = false;

    return DispatchResponse::Success();
}

DispatchResponse NetworkAgentImpl::setExtraHTTPHeaders(std::unique_ptr<protocol::Network::Headers> in_headers) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

void NetworkAgentImpl::getResponseBody(const String& in_requestId, std::unique_ptr<GetResponseBodyCallback> callback) {
    auto it = m_responses.find(in_requestId.utf8());

    if (it == m_responses.end()) {
        auto error = "Response not found for requestId = " + in_requestId;
        callback->sendFailure(DispatchResponse::ServerError(error.utf8()));
        return;
    }

    utils::NetworkRequestData* response = it->second;
    auto body = v8_inspector::String16((const uint16_t*) response->getData());
    auto base64Encoded = !response->hasTextContent();
    callback->sendSuccess(body, base64Encoded);
}

DispatchResponse NetworkAgentImpl::setCacheDisabled(bool in_cacheDisabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::canClearBrowserCache(bool* out_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::canClearBrowserCookies(bool* out_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::emulateNetworkConditions(bool in_offline, double in_latency, double in_downloadThroughput, double in_uploadThroughput, Maybe<String> in_connectionType) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::getCertificate(const String& in_origin, std::unique_ptr<protocol::Array<String>>* out_tableNames) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

void NetworkAgentImpl::getRequestPostData(const String& in_requestId, std::unique_ptr<GetRequestPostDataCallback> callback) {
}

DispatchResponse NetworkAgentImpl::replayXHR(const String& in_requestId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::searchInResponseBody(const String& in_requestId, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>>* out_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::setBlockedURLs(std::unique_ptr<protocol::Array<String>> in_urls) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::setBypassServiceWorker(bool in_bypass) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse NetworkAgentImpl::setDataSizeLimitsForTest(int in_maxTotalSize, int in_maxResourceSize) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

//////

NetworkAgentImpl* NetworkAgentImpl::Instance = 0;
}  // namespace tns

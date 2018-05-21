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

DispatchResponse V8NetworkAgentImpl::enable(Maybe<int> in_maxTotalBufferSize, Maybe<int> in_maxResourceBufferSize, Maybe<int> in_maxPostDataSize) {
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

DispatchResponse V8NetworkAgentImpl::canClearBrowserCache(bool* out_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::canClearBrowserCookies(bool* out_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::canEmulateNetworkConditions(bool* out_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::clearBrowserCache() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::clearBrowserCookies() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::continueInterceptedRequest(const String& in_interceptionId, Maybe<String> in_errorReason, Maybe<String> in_rawResponse, Maybe<String> in_url, Maybe<String> in_method, Maybe<String> in_postData, Maybe<protocol::Network::Headers> in_headers, Maybe<protocol::Network::AuthChallengeResponse> in_authChallengeResponse) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::deleteCookies(const String& in_name, Maybe<String> in_url, Maybe<String> in_domain, Maybe<String> in_path) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::emulateNetworkConditions(bool in_offline, double in_latency, double in_downloadThroughput, double in_uploadThroughput, Maybe<String> in_connectionType) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::getAllCookies(std::unique_ptr<protocol::Array<protocol::Network::Cookie>>* out_cookies) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::getCertificate(const String& in_origin, std::unique_ptr<protocol::Array<String>>* out_tableNames) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::getCookies(Maybe<protocol::Array<String>> in_urls, std::unique_ptr<protocol::Array<protocol::Network::Cookie>>* out_cookies) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::getRequestPostData(const String& in_requestId, String* out_postData) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::getResponseBodyForInterception(const String& in_interceptionId, String* out_body, bool* out_base64Encoded) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::replayXHR(const String& in_requestId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::searchInResponseBody(const String& in_requestId, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>>* out_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::setBlockedURLs(std::unique_ptr<protocol::Array<String>> in_urls) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::setBypassServiceWorker(bool in_bypass) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::setCookie(const String& in_name, const String& in_value, Maybe<String> in_url, Maybe<String> in_domain, Maybe<String> in_path, Maybe<bool> in_secure, Maybe<bool> in_httpOnly, Maybe<String> in_sameSite, Maybe<double> in_expires, bool* out_success) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::setCookies(std::unique_ptr<protocol::Array<protocol::Network::CookieParam>> in_cookies) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::setDataSizeLimitsForTest(int in_maxTotalSize, int in_maxResourceSize) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::setRequestInterception(std::unique_ptr<protocol::Array<protocol::Network::RequestPattern>> in_patterns) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8NetworkAgentImpl::setUserAgentOverride(const String& in_userAgent) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

//////

V8NetworkAgentImpl* V8NetworkAgentImpl::Instance = 0;
}
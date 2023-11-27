//
// Created by pkanev on 2/22/2017.
//

#ifndef V8_NETWORK_AGENT_IMPL_H
#define V8_NETWORK_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/Network.h>
#include <map>
#include <v8_inspector/src/inspector/protocol/Protocol.h>

#include "utils/NetworkRequestData.h"

namespace v8_inspector {
class V8InspectorSessionImpl;
}

namespace tns {

using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;
using v8_inspector::protocol::DispatchResponse;
using v8_inspector::V8InspectorSessionImpl;
namespace protocol = v8_inspector::protocol;

class NetworkAgentImpl : public protocol::Network::Backend {
    public:
        NetworkAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                         protocol::DictionaryValue* state);
        ~NetworkAgentImpl() override;

        DispatchResponse enable(Maybe<int> in_maxTotalBufferSize, Maybe<int> in_maxResourceBufferSize, Maybe<int> in_maxPostDataSize) override;
        DispatchResponse disable() override;

        DispatchResponse setExtraHTTPHeaders(std::unique_ptr<protocol::Network::Headers> in_headers) override;

        /*
         * Returns content served for the given request.
         * The content of each response is stored in `m_responses` when a request completes.
         * @param in_requestId
         */
        void getResponseBody(const String& in_requestId, std::unique_ptr<GetResponseBodyCallback> callback) override;

        DispatchResponse setCacheDisabled(bool in_cacheDisabled) override;

        DispatchResponse canClearBrowserCache(bool* out_result) override;
        DispatchResponse canClearBrowserCookies(bool* out_result) override;
        DispatchResponse emulateNetworkConditions(bool in_offline, double in_latency, double in_downloadThroughput, double in_uploadThroughput, Maybe<String> in_connectionType) override;
        DispatchResponse getCertificate(const String& in_origin, std::unique_ptr<protocol::Array<String>>* out_tableNames) override;
        void getRequestPostData(const String& in_requestId, std::unique_ptr<GetRequestPostDataCallback> callback) override;
        DispatchResponse replayXHR(const String& in_requestId) override;
        DispatchResponse searchInResponseBody(const String& in_requestId, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>>* out_result) override;
        DispatchResponse setBlockedURLs(std::unique_ptr<protocol::Array<String>> in_urls) override;
        DispatchResponse setBypassServiceWorker(bool in_bypass) override;
        DispatchResponse setDataSizeLimitsForTest(int in_maxTotalSize, int in_maxResourceSize) override;

        const bool enabled() {
            return m_enabled;
        };

        static NetworkAgentImpl* Instance;
        protocol::Network::Frontend m_frontend;
        robin_hood::unordered_map<std::string, utils::NetworkRequestData*> m_responses;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        bool m_enabled;

        NetworkAgentImpl(const NetworkAgentImpl&) = delete;
        NetworkAgentImpl& operator=(const NetworkAgentImpl&) = delete;
};
}  // namespace tns

#endif //V8_NETWORK_AGENT_IMPL_H

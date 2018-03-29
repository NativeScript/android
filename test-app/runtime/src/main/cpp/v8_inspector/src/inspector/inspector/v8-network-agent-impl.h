//
// Created by pkanev on 2/22/2017.
//

#ifndef V8_NETWORK_AGENT_IMPL_H
#define V8_NETWORK_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/Network.h>
#include <v8_inspector/src/inspector/utils/v8-network-request-data.h>
#include <map>
#include <v8_inspector/src/inspector/protocol/Protocol.h>

namespace v8_inspector {

class V8InspectorSessionImpl;

using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;
using v8_inspector::protocol::DispatchResponse;

class V8NetworkAgentImpl : public protocol::Network::Backend {
    public:
        V8NetworkAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                           protocol::DictionaryValue* state);
        ~V8NetworkAgentImpl() override;

        DispatchResponse enable() override;
        DispatchResponse disable() override;

        DispatchResponse setExtraHTTPHeaders(std::unique_ptr<protocol::Network::Headers> in_headers) override;

        /*
         * Returns content served for the given request.
         * The content of each response is stored in `m_responses` when a request completes.
         * @param in_requestId
         */
        DispatchResponse getResponseBody(const String& in_requestId, String* out_body, bool* out_base64Encoded) override;

        DispatchResponse setCacheDisabled(bool in_cacheDisabled) override;

        DispatchResponse loadResource(const String& in_frameId, const String& in_url, String* out_content, String* out_mimeType, double* out_status) override;

        const bool enabled() {
            return m_enabled;
        };

        static V8NetworkAgentImpl* Instance;
        protocol::Network::Frontend m_frontend;
        std::map<std::string, v8_inspector::utils::NetworkRequestData*> m_responses;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8NetworkAgentImpl);
};
}

#endif //V8_NETWORK_AGENT_IMPL_H

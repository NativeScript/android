//
// Created by pkanev on 2/22/2017.
//

#ifndef V8_NETWORK_AGENT_IMPL_H
#define V8_NETWORK_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/Network.h>
#include <v8_inspector/src/inspector/utils/v8-network-request-data.h>
#include <map>

namespace v8_inspector {

class V8InspectorSessionImpl;

using protocol::ErrorString;
using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;

class V8NetworkAgentImpl : public protocol::Network::Backend {
    public:
        V8NetworkAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                           protocol::DictionaryValue* state);
        ~V8NetworkAgentImpl() override;

        void enable(ErrorString*) override;
        void disable(ErrorString*) override;

        void setExtraHTTPHeaders(ErrorString*, std::unique_ptr<protocol::Network::Headers> in_headers) override;
        void getResponseBody(ErrorString*, const String& in_requestId, String* out_body, bool* out_base64Encoded) override;
        void setCacheDisabled(ErrorString*, bool in_cacheDisabled) override;
        void loadResource(const String& in_frameId, const String& in_url, std::unique_ptr<LoadResourceCallback> callback) override;

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

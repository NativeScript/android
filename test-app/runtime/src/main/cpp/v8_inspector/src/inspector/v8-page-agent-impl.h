//
// Created by pkanev on 1/31/2017.
//

#ifndef V8_INSPECTOR_V8PAGEAGENTIMPL_H
#define V8_INSPECTOR_V8PAGEAGENTIMPL_H

#include <v8_inspector/src/inspector/protocol/Forward.h>
#include <v8_inspector/src/inspector/protocol/Page.h>
#include <v8_inspector/src/inspector/protocol/Protocol.h>
#include <v8_inspector/src/inspector/v8-inspector-session-impl.h>

namespace v8_inspector {

class V8InspectorSessionImpl;

using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;
using protocol::DispatchResponse;

class V8PageAgentImpl : public protocol::Page::Backend {
    public:
        V8PageAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                        protocol::DictionaryValue* state);
        ~V8PageAgentImpl() override;

        DispatchResponse enable() override;
        DispatchResponse disable() override;
        DispatchResponse addScriptToEvaluateOnLoad(const String& in_scriptSource, String* out_identifier) override;
        DispatchResponse removeScriptToEvaluateOnLoad(const String& in_identifier) override;
        DispatchResponse reload(Maybe<bool> in_ignoreCache, Maybe<String> in_scriptToEvaluateOnLoad) override;
        DispatchResponse getResourceTree(std::unique_ptr<protocol::Page::FrameResourceTree>* out_frameTree) override;
        DispatchResponse getResourceContent(const String& in_frameId, const String& in_url, String* out_content, bool* out_base64Encoded) override;
        DispatchResponse searchInResource(const String& in_frameId, const String& in_url, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, Maybe<String> in_requestId, std::unique_ptr<v8_inspector::protocol::Array<protocol::GenericTypes::SearchMatch>>* out_result) override;
        DispatchResponse searchInResources(const String& in_text, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<v8_inspector::protocol::Array<protocol::Page::SearchResult>>* out_result) override;
        DispatchResponse setDocumentContent(const String& in_frameId, const String& in_html) override;

        void restore();
        void reset();
        const bool enabled() {
            return m_enabled;
        };

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        protocol::Page::Frontend m_frontend;
        const std::string m_frameUrl;
        const std::string m_frameIdentifier;
        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8PageAgentImpl);
};

}

#endif //V8_INSPECTOR_V8PAGEAGENTIMPL_H

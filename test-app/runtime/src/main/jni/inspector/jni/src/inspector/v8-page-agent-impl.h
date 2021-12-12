//
// Created by pkanev on 1/31/2017.
//

#ifndef V8_INSPECTOR_V8PAGEAGENTIMPL_H
#define V8_INSPECTOR_V8PAGEAGENTIMPL_H

#include <v8_inspector/src/inspector/string-util.h>
#include <v8_inspector/src/inspector/protocol/Forward.h>
#include <v8_inspector/src/inspector/protocol/Page.h>
#include <v8_inspector/src/inspector/protocol/Protocol.h>
#include <v8_inspector/src/inspector/v8-inspector-session-impl.h>

namespace v8_inspector {

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete

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
        void getResourceContent(const String& in_frameId, const String& in_url, std::unique_ptr<GetResourceContentCallback> callback) override;
        void searchInResource(const String& in_frameId, const String& in_url, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<SearchInResourceCallback> callback) override;
        DispatchResponse setDocumentContent(const String& in_frameId, const String& in_html) override;
        DispatchResponse addScriptToEvaluateOnNewDocument(const String& in_source, Maybe<String> in_worldName, String* out_identifier) override;
        DispatchResponse createIsolatedWorld(const String& in_frameId, Maybe<String> in_worldName, Maybe<bool> in_grantUniveralAccess, int* out_executionContextId) override;
        DispatchResponse getFrameTree(std::unique_ptr<protocol::Page::FrameTree>* out_frameTree) override;
        DispatchResponse getLayoutMetrics(std::unique_ptr<protocol::Page::LayoutViewport>* out_layoutViewport, std::unique_ptr<protocol::Page::VisualViewport>* out_visualViewport, std::unique_ptr<protocol::DOM::Rect>* out_contentSize) override;
        DispatchResponse removeScriptToEvaluateOnNewDocument(const String& in_identifier) override;
        DispatchResponse setAdBlockingEnabled(bool in_enabled) override;
        DispatchResponse setBypassCSP(bool in_enabled) override;
        DispatchResponse setFontFamilies(std::unique_ptr<protocol::Page::FontFamilies> in_fontFamilies) override;
        DispatchResponse setFontSizes(std::unique_ptr<protocol::Page::FontSizes> in_fontSizes) override;
        DispatchResponse setLifecycleEventsEnabled(bool in_enabled) override;
        DispatchResponse startScreencast(Maybe<String> in_format, Maybe<int> in_quality, Maybe<int> in_maxWidth, Maybe<int> in_maxHeight, Maybe<int> in_everyNthFrame) override;
        DispatchResponse stopLoading() override;
        DispatchResponse stopScreencast() override;
        DispatchResponse setProduceCompilationCache(bool in_enabled) override;
        DispatchResponse addCompilationCache(const String& in_url, const protocol::Binary& in_data) override;
        DispatchResponse clearCompilationCache() override;
        DispatchResponse generateTestReport(const String& in_message, Maybe<String> in_group) override;
        DispatchResponse waitForDebugger() override;

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

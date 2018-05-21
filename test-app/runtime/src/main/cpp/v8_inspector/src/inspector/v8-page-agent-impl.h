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
        DispatchResponse searchInResource(const String& in_frameId, const String& in_url, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>>* out_result) override;
        DispatchResponse setDocumentContent(const String& in_frameId, const String& in_html) override;
        DispatchResponse addScriptToEvaluateOnNewDocument(const String& in_source, String* out_identifier) override;
        DispatchResponse bringToFront() override;
        DispatchResponse captureScreenshot(Maybe<String> in_format, Maybe<int> in_quality, Maybe<protocol::Page::Viewport> in_clip, Maybe<bool> in_fromSurface, String* out_data) override;
        DispatchResponse createIsolatedWorld(const String& in_frameId, Maybe<String> in_worldName, Maybe<bool> in_grantUniveralAccess, int* out_executionContextId) override;
        DispatchResponse getAppManifest(String* out_url, std::unique_ptr<protocol::Array<protocol::Page::AppManifestError>>* out_errors, Maybe<String>* out_data) override;
        DispatchResponse getFrameTree(std::unique_ptr<protocol::Page::FrameTree>* out_frameTree) override;
        DispatchResponse getLayoutMetrics(std::unique_ptr<protocol::Page::LayoutViewport>* out_layoutViewport, std::unique_ptr<protocol::Page::VisualViewport>* out_visualViewport, std::unique_ptr<protocol::DOM::Rect>* out_contentSize) override;
        DispatchResponse getNavigationHistory(int* out_currentIndex, std::unique_ptr<protocol::Array<protocol::Page::NavigationEntry>>* out_entries) override;
        DispatchResponse handleJavaScriptDialog(bool in_accept, Maybe<String> in_promptText) override;
        DispatchResponse navigate(const String& in_url, Maybe<String> in_referrer, Maybe<String> in_transitionType, Maybe<String> in_frameId, String* out_frameId, Maybe<String>* out_loaderId, Maybe<String>* out_errorText) override;
        DispatchResponse navigateToHistoryEntry(int in_entryId) override;
        DispatchResponse printToPDF(Maybe<bool> in_landscape, Maybe<bool> in_displayHeaderFooter, Maybe<bool> in_printBackground, Maybe<double> in_scale, Maybe<double> in_paperWidth, Maybe<double> in_paperHeight, Maybe<double> in_marginTop, Maybe<double> in_marginBottom, Maybe<double> in_marginLeft, Maybe<double> in_marginRight, Maybe<String> in_pageRanges, Maybe<bool> in_ignoreInvalidPageRanges, Maybe<String> in_headerTemplate, Maybe<String> in_footerTemplate, Maybe<bool> in_preferCSSPageSize, String* out_data) override;
        DispatchResponse removeScriptToEvaluateOnNewDocument(const String& in_identifier) override;
        DispatchResponse requestAppBanner() override;
        DispatchResponse screencastFrameAck(int in_sessionId) override;
        DispatchResponse setAdBlockingEnabled(bool in_enabled) override;
        DispatchResponse setBypassCSP(bool in_enabled) override;
        DispatchResponse setDownloadBehavior(const String& in_behavior, Maybe<String> in_downloadPath) override;
        DispatchResponse setLifecycleEventsEnabled(bool in_enabled) override;
        DispatchResponse startScreencast(Maybe<String> in_format, Maybe<int> in_quality, Maybe<int> in_maxWidth, Maybe<int> in_maxHeight, Maybe<int> in_everyNthFrame) override;
        DispatchResponse stopLoading() override;
        DispatchResponse crash() override;
        DispatchResponse stopScreencast() override;

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

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

using protocol::ErrorString;
using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;

class V8PageAgentImpl : public protocol::Page::Backend {
    public:
        V8PageAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                        protocol::DictionaryValue* state);
        ~V8PageAgentImpl() override;

        void enable(ErrorString*) override;
        void disable(ErrorString*) override;
        void getResourceTree(ErrorString*, std::unique_ptr<protocol::Page::FrameResourceTree>* out_frameTree) override;
        void getResourceContent(ErrorString*, const String& in_frameId, const String& in_url, String* out_content, bool* out_base64Encoded) override;
        void searchInResource(ErrorString*, const String& in_frameId, const String& in_url, const String& in_query, const Maybe<bool>& in_caseSensitive, const Maybe<bool>& in_isRegex, const Maybe<String>& in_requestId, std::unique_ptr<protocol::Array<protocol::GenericTypes::SearchMatch>>* out_result) override;
        void searchInResources(ErrorString*, const String& in_text, const Maybe<bool>& in_caseSensitive, const Maybe<bool>& in_isRegex, std::unique_ptr<protocol::Array<protocol::Page::SearchResult>>* out_result) override;
        void addScriptToEvaluateOnLoad(ErrorString*, const String& in_scriptSource, String* out_identifier) override;
        void removeScriptToEvaluateOnLoad(ErrorString*, const String& in_identifier) override;
        void reload(ErrorString*, const Maybe<bool>& in_ignoreCache, const Maybe<String>& in_scriptToEvaluateOnLoad) override;
        void navigate(ErrorString*, const String& in_url) override;
        void getCookies(ErrorString*, std::unique_ptr<protocol::Array<protocol::Page::Cookie>>* out_cookies) override;
        void deleteCookie(ErrorString*, const String& in_cookieName, const String& in_url) override;
        void setDocumentContent(ErrorString*, const String& in_frameId, const String& in_html) override;
        void setShowPaintRects(ErrorString*, bool in_result) override;
        void getScriptExecutionStatus(ErrorString*, String* out_result) override;
        void setScriptExecutionDisabled(ErrorString*, bool in_value) override;
        void setTouchEmulationEnabled(ErrorString*, bool in_enabled) override;
        void setEmulatedMedia(ErrorString*, const String& in_media) override;
        void getCompositingBordersVisible(ErrorString*, bool* out_result) override;
        void setCompositingBordersVisible(ErrorString*, bool in_visible) override;
        void snapshotNode(ErrorString*, int in_nodeId, String* out_dataURL) override;
        void snapshotRect(ErrorString*, int in_x, int in_y, int in_width, int in_height, const String& in_coordinateSystem, String* out_dataURL) override;
        void handleJavaScriptDialog(ErrorString*, bool in_accept, const Maybe<String>& in_promptText) override;
        void archive(ErrorString*, String* out_data) override;

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

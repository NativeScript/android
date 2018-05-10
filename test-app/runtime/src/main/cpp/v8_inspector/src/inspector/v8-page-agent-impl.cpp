//
// Created by pkanev on 1/31/2017.
//

#include <v8_inspector/src/inspector/utils/v8-search-utils-public.h>
#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
#include "v8-page-agent-impl.h"
#include "search-util.h"

namespace v8_inspector {

namespace PageAgentState {
static const char pageEnabled[] = "pageEnabled";
}

V8PageAgentImpl::V8PageAgentImpl(
    V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
    protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false),
      m_frameIdentifier(""),
      m_frameUrl("file://") {}

V8PageAgentImpl::~V8PageAgentImpl() {}

////////////////

DispatchResponse V8PageAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::OK();
    }

    m_state->setBoolean(PageAgentState::pageEnabled, true);

    m_enabled = true;

    return DispatchResponse::OK();
}

DispatchResponse V8PageAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::OK();
    }

    m_state->setBoolean(PageAgentState::pageEnabled, false);

    m_enabled = false;

    return DispatchResponse::OK();
}

DispatchResponse V8PageAgentImpl::addScriptToEvaluateOnLoad(const String& in_scriptSource, String* out_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::removeScriptToEvaluateOnLoad(const String& in_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::reload(Maybe<bool> in_ignoreCache, Maybe<String> in_scriptToEvaluateOnLoad) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::getResourceTree(std::unique_ptr<protocol::Page::FrameResourceTree>* out_frameTree) {
    std::unique_ptr<protocol::Page::Frame> frameObject = protocol::Page::Frame::create()
            .setId(m_frameIdentifier.c_str())
            .setLoaderId("NSLoaderIdentifier")
            .setMimeType("text/directory")
            .setSecurityOrigin("")
            .setUrl(m_frameUrl.c_str())
            .build();

    auto subresources = protocol::Array<protocol::Page::FrameResource>::create();

    auto resources = v8_inspector::utils::PageResource::getPageResources();

    for (auto const& resource : resources) {
        auto pageResource = resource.second;
        std::unique_ptr<protocol::Page::FrameResource> frameResource = protocol::Page::FrameResource::create()
                .setUrl(pageResource.getFilePath())
                .setType(pageResource.getType())
                .setMimeType(pageResource.getMimeType())
                .build();

        subresources->addItem(std::move(frameResource));
    }

    *out_frameTree = protocol::Page::FrameResourceTree::create()
                     .setFrame(std::move(frameObject))
                     .setResources(std::move(subresources))
                     .build();

    return DispatchResponse::OK();
}

DispatchResponse V8PageAgentImpl::getResourceContent(const String& in_frameId, const String& in_url, String* out_content, bool* out_base64Encoded) {
    if (in_url.utf8().compare(m_frameUrl) == 0) {
        *out_content = "";
        *out_base64Encoded = false;

        return DispatchResponse::OK();
    }

    std::map<std::string, v8_inspector::utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    auto it = cachedPageResources.find(in_url.utf8());
    if (it == cachedPageResources.end()) {
        *out_content = "";
        return DispatchResponse::Error("Resource not found.");
    }

    auto resource = it->second;

    *out_base64Encoded = !resource.hasTextContent();

    auto errorString = new String16();

    *out_content = resource.getContent(errorString);

    if (!errorString->isEmpty()) {
        return DispatchResponse::Error(*errorString);
    }

    return DispatchResponse::OK();
}

DispatchResponse V8PageAgentImpl::searchInResource(const String& in_frameId, const String& in_url, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>>* out_result) {
    bool isRegex = in_isRegex.fromMaybe(false);
    bool isCaseSensitive = in_caseSensitive.fromMaybe(false);

    std::map<std::string, v8_inspector::utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    auto result = protocol::Array<protocol::Debugger::SearchMatch>::create();

    auto it = cachedPageResources.find(in_url.utf8());
    if (it == cachedPageResources.end()) {
        *out_result = std::move(result);
        return DispatchResponse::OK();
    }

    auto resource = it->second;
    auto errorString = new String16();
    auto content = resource.getContent(errorString);
    if (errorString->isEmpty()) {
        auto matches = v8_inspector::utils::ResourceContentSearchUtils::searchInTextByLinesImpl(m_session, content, in_query, isCaseSensitive, isRegex);
        for (auto& match : matches) {
            result->addItem(std::move(match));
        }

        *out_result = std::move(result);
        return DispatchResponse::OK();
    }

    return DispatchResponse::Error(*errorString);
}

void V8PageAgentImpl::restore() {
    if (!m_state->booleanProperty(PageAgentState::pageEnabled, false)) {
        return;
    }

    enable();
}

void V8PageAgentImpl::reset() {

}

DispatchResponse V8PageAgentImpl::setDocumentContent(const String& in_frameId, const String& in_html) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::addScriptToEvaluateOnNewDocument(const String& in_source, String* out_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::bringToFront() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::captureScreenshot(Maybe<String> in_format, Maybe<int> in_quality, Maybe<protocol::Page::Viewport> in_clip, Maybe<bool> in_fromSurface, String* out_data) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::createIsolatedWorld(const String& in_frameId, Maybe<String> in_worldName, Maybe<bool> in_grantUniveralAccess, int* out_executionContextId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::getAppManifest(String* out_url, std::unique_ptr<protocol::Array<protocol::Page::AppManifestError>>* out_errors, Maybe<String>* out_data) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::getFrameTree(std::unique_ptr<protocol::Page::FrameTree>* out_frameTree) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::getLayoutMetrics(std::unique_ptr<protocol::Page::LayoutViewport>* out_layoutViewport, std::unique_ptr<protocol::Page::VisualViewport>* out_visualViewport, std::unique_ptr<protocol::DOM::Rect>* out_contentSize) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::getNavigationHistory(int* out_currentIndex, std::unique_ptr<protocol::Array<protocol::Page::NavigationEntry>>* out_entries) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::handleJavaScriptDialog(bool in_accept, Maybe<String> in_promptText) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::navigate(const String& in_url, Maybe<String> in_referrer, Maybe<String> in_transitionType, Maybe<String> in_frameId, String* out_frameId, Maybe<String>* out_loaderId, Maybe<String>* out_errorText) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::navigateToHistoryEntry(int in_entryId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::printToPDF(Maybe<bool> in_landscape, Maybe<bool> in_displayHeaderFooter, Maybe<bool> in_printBackground, Maybe<double> in_scale, Maybe<double> in_paperWidth, Maybe<double> in_paperHeight, Maybe<double> in_marginTop, Maybe<double> in_marginBottom, Maybe<double> in_marginLeft, Maybe<double> in_marginRight, Maybe<String> in_pageRanges, Maybe<bool> in_ignoreInvalidPageRanges, Maybe<String> in_headerTemplate, Maybe<String> in_footerTemplate, Maybe<bool> in_preferCSSPageSize, String* out_data) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::removeScriptToEvaluateOnNewDocument(const String& in_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::requestAppBanner() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::screencastFrameAck(int in_sessionId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setAdBlockingEnabled(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setBypassCSP(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setDownloadBehavior(const String& in_behavior, Maybe<String> in_downloadPath) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setLifecycleEventsEnabled(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::startScreencast(Maybe<String> in_format, Maybe<int> in_quality, Maybe<int> in_maxWidth, Maybe<int> in_maxHeight, Maybe<int> in_everyNthFrame) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::stopLoading() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::crash() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::stopScreencast() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

}
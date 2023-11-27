//
// Created by pkanev on 1/31/2017.
//

#include <src/inspector/search-util.h>

#include "PageAgentImpl.h"
#include "utils/SearchUtilsPublic.h"
#include "utils/InspectorCommon.h"

namespace tns {

namespace PageAgentState {
static const char pageEnabled[] = "pageEnabled";
}

PageAgentImpl::PageAgentImpl(
    V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
    protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false),
      m_frameIdentifier(""),
      m_frameUrl("file://") {}

PageAgentImpl::~PageAgentImpl() {}

////////////////

DispatchResponse PageAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(PageAgentState::pageEnabled, true);

    m_enabled = true;

    return DispatchResponse::Success();
}

DispatchResponse PageAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(PageAgentState::pageEnabled, false);

    m_enabled = false;

    return DispatchResponse::Success();
}

DispatchResponse PageAgentImpl::addScriptToEvaluateOnLoad(const String& in_scriptSource, String* out_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::removeScriptToEvaluateOnLoad(const String& in_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::reload(Maybe<bool> in_ignoreCache, Maybe<String> in_scriptToEvaluateOnLoad) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::getResourceTree(std::unique_ptr<protocol::Page::FrameResourceTree>* out_frameTree) {
    std::unique_ptr<protocol::Page::Frame> frameObject = protocol::Page::Frame::create()
            .setId(m_frameIdentifier.c_str())
            .setLoaderId("NSLoaderIdentifier")
            .setMimeType("text/directory")
            .setSecurityOrigin("")
            .setUrl(m_frameUrl.c_str())
            .build();

    auto subresources = std::make_unique<protocol::Array<protocol::Page::FrameResource>>();

    auto resources = utils::PageResource::getPageResources();

    for (auto const& resource : resources) {
        auto pageResource = resource.second;
        std::unique_ptr<protocol::Page::FrameResource> frameResource = protocol::Page::FrameResource::create()
                .setUrl(pageResource.getFilePath())
                .setType(pageResource.getType())
                .setMimeType(pageResource.getMimeType())
                .build();

        subresources->emplace_back(std::move(frameResource));
    }

    *out_frameTree = protocol::Page::FrameResourceTree::create()
                     .setFrame(std::move(frameObject))
                     .setResources(std::move(subresources))
                     .build();

    return DispatchResponse::Success();
}

void PageAgentImpl::getResourceContent(const String& in_frameId, const String& in_url, std::unique_ptr<GetResourceContentCallback> callback) {
    if (in_url.utf8().compare(m_frameUrl) == 0) {
        auto content = "";
        auto base64Encoded = false;

        callback->sendSuccess(content, base64Encoded);
        return;
    }

    robin_hood::unordered_map<std::string, utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    auto it = cachedPageResources.find(in_url.utf8());
    if (it == cachedPageResources.end()) {
        callback->sendFailure(DispatchResponse::ServerError("Resource not found."));
        return;
    }

    auto resource = it->second;

    auto base64Encoded = !resource.hasTextContent();

    auto errorString = new v8_inspector::String16();

    auto content = resource.getContent(errorString);

    if (!errorString->isEmpty()) {
        callback->sendFailure(DispatchResponse::ServerError(errorString->utf8()));
        return;
    }

    callback->sendSuccess(content, base64Encoded);
}

void PageAgentImpl::searchInResource(const String& in_frameId, const String& in_url, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<SearchInResourceCallback> callback) {
    bool isRegex = in_isRegex.fromMaybe(false);
    bool isCaseSensitive = in_caseSensitive.fromMaybe(false);

    robin_hood::unordered_map<std::string, utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    auto result = std::make_unique<protocol::Array<protocol::Debugger::SearchMatch>>();

    auto it = cachedPageResources.find(in_url.utf8());
    if (it == cachedPageResources.end()) {
        callback->sendSuccess(std::move(result));
        return;
    }

    auto resource = it->second;
    auto errorString = new v8_inspector::String16();
    auto content = resource.getContent(errorString);
    if (errorString->isEmpty()) {
        auto matches = utils::ResourceContentSearchUtils::searchInTextByLinesImpl(m_session, content, in_query, isCaseSensitive, isRegex);
        for (auto& match : matches) {
            result->emplace_back(std::move(match));
        }

        callback->sendSuccess(std::move(result));
        return;
    }

    callback->sendFailure(DispatchResponse::ServerError(errorString->utf8()));
}

void PageAgentImpl::restore() {
    if (!m_state->booleanProperty(PageAgentState::pageEnabled, false)) {
        return;
    }

    enable();
}

void PageAgentImpl::reset() {

}

DispatchResponse PageAgentImpl::setDocumentContent(const String& in_frameId, const String& in_html) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::addScriptToEvaluateOnNewDocument(const String& in_source, Maybe<String> in_worldName, String* out_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::createIsolatedWorld(const String& in_frameId, Maybe<String> in_worldName, Maybe<bool> in_grantUniveralAccess, int* out_executionContextId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::getFrameTree(std::unique_ptr<protocol::Page::FrameTree>* out_frameTree) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::getLayoutMetrics(std::unique_ptr<protocol::Page::LayoutViewport>* out_layoutViewport, std::unique_ptr<protocol::Page::VisualViewport>* out_visualViewport, std::unique_ptr<protocol::DOM::Rect>* out_contentSize) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::removeScriptToEvaluateOnNewDocument(const String& in_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::setAdBlockingEnabled(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::setBypassCSP(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::setFontFamilies(std::unique_ptr<protocol::Page::FontFamilies> in_fontFamilies) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::setFontSizes(std::unique_ptr<protocol::Page::FontSizes> in_fontSizes) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::setLifecycleEventsEnabled(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::startScreencast(Maybe<String> in_format, Maybe<int> in_quality, Maybe<int> in_maxWidth, Maybe<int> in_maxHeight, Maybe<int> in_everyNthFrame) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::stopLoading() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::stopScreencast() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::setProduceCompilationCache(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::addCompilationCache(const String& in_url, const protocol::Binary& in_data) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::clearCompilationCache() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::generateTestReport(const String& in_message, Maybe<String> in_group) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse PageAgentImpl::waitForDebugger() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

} // namespace tns

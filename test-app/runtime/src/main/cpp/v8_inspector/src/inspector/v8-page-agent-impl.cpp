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

    auto subresources = std::make_unique<protocol::Array<protocol::Page::FrameResource>>();

    auto resources = v8_inspector::utils::PageResource::getPageResources();

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

    return DispatchResponse::OK();
}

void V8PageAgentImpl::getResourceContent(const String& in_frameId, const String& in_url, std::unique_ptr<GetResourceContentCallback> callback) {
    if (in_url.utf8().compare(m_frameUrl) == 0) {
        auto content = "";
        auto base64Encoded = false;

        callback->sendSuccess(content, base64Encoded);
        return;
    }

    std::map<std::string, v8_inspector::utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    auto it = cachedPageResources.find(in_url.utf8());
    if (it == cachedPageResources.end()) {
        callback->sendFailure(DispatchResponse::Error("Resource not found."));
        return;
    }

    auto resource = it->second;

    auto base64Encoded = !resource.hasTextContent();

    auto errorString = new String16();

    auto content = resource.getContent(errorString);

    if (!errorString->isEmpty()) {
        callback->sendFailure(DispatchResponse::Error(*errorString));
        return;
    }

    callback->sendSuccess(content, base64Encoded);
}

void V8PageAgentImpl::searchInResource(const String& in_frameId, const String& in_url, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<SearchInResourceCallback> callback) {
    bool isRegex = in_isRegex.fromMaybe(false);
    bool isCaseSensitive = in_caseSensitive.fromMaybe(false);

    std::map<std::string, v8_inspector::utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
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
    auto errorString = new String16();
    auto content = resource.getContent(errorString);
    if (errorString->isEmpty()) {
        auto matches = v8_inspector::utils::ResourceContentSearchUtils::searchInTextByLinesImpl(m_session, content, in_query, isCaseSensitive, isRegex);
        for (auto& match : matches) {
            result->emplace_back(std::move(match));
        }

        callback->sendSuccess(std::move(result));
        return;
    }

    callback->sendFailure(DispatchResponse::Error(*errorString));
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

DispatchResponse V8PageAgentImpl::addScriptToEvaluateOnNewDocument(const String& in_source, Maybe<String> in_worldName, String* out_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::createIsolatedWorld(const String& in_frameId, Maybe<String> in_worldName, Maybe<bool> in_grantUniveralAccess, int* out_executionContextId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::getFrameTree(std::unique_ptr<protocol::Page::FrameTree>* out_frameTree) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::getLayoutMetrics(std::unique_ptr<protocol::Page::LayoutViewport>* out_layoutViewport, std::unique_ptr<protocol::Page::VisualViewport>* out_visualViewport, std::unique_ptr<protocol::DOM::Rect>* out_contentSize) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::removeScriptToEvaluateOnNewDocument(const String& in_identifier) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setAdBlockingEnabled(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setBypassCSP(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setFontFamilies(std::unique_ptr<protocol::Page::FontFamilies> in_fontFamilies) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setFontSizes(std::unique_ptr<protocol::Page::FontSizes> in_fontSizes) {
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

DispatchResponse V8PageAgentImpl::stopScreencast() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::setProduceCompilationCache(bool in_enabled) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::addCompilationCache(const String& in_url, const protocol::Binary& in_data) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::clearCompilationCache() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::generateTestReport(const String& in_message, Maybe<String> in_group) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8PageAgentImpl::waitForDebugger() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

}
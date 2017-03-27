//
// Created by pkanev on 1/31/2017.
//

#include <v8_inspector/src/inspector/utils/v8-search-utils-public.h>
#include "v8-page-agent-impl.h"
#include "search-util.h"

namespace v8_inspector {

namespace PageAgentState {
static const char pageEnabled[] = "pageEnabled";
}

V8PageAgentImpl::V8PageAgentImpl(
    V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
    protocol::DictionaryValue* state)
    : m_session(session), m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false),
      m_frameIdentifier("NSFrameIdentifier"),
      m_frameUrl("file://") {}

V8PageAgentImpl::~V8PageAgentImpl() {}

void V8PageAgentImpl::enable(ErrorString*) {
    if (m_enabled) {
        return;
    }

    m_state->setBoolean(PageAgentState::pageEnabled, true);

    m_enabled = true;
}

void V8PageAgentImpl::disable(ErrorString*) {
    if (!m_enabled) {
        return;
    }

    m_state->setBoolean(PageAgentState::pageEnabled, false);

    m_enabled = false;
}

void V8PageAgentImpl::restore() {
    if (!m_state->booleanProperty(PageAgentState::pageEnabled, false)) {
        return;
    }

    ErrorString ignored;
    enable(&ignored);
}

void V8PageAgentImpl::getResourceTree(
    ErrorString*, std::unique_ptr<protocol::Page::FrameResourceTree>* out_frameTree) {
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

}

void V8PageAgentImpl::getResourceContent(ErrorString* errorString, const String& in_frameId,
        const String& in_url, String* out_content,
        bool* out_base64Encoded) {
    if (in_url.utf8().compare(m_frameUrl) == 0) {
        *out_content = "";
        *out_base64Encoded = false;

        return;
    }

    std::map<std::string, v8_inspector::utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    auto it = cachedPageResources.find(in_url.utf8());
    if (it == cachedPageResources.end()) {
        *errorString = "Resource not found.";
        *out_content = "";
        return;
    }

    auto resource = it->second;

    *out_base64Encoded = !resource.hasTextContent();

    *out_content = resource.getContent(errorString);
}

void V8PageAgentImpl::searchInResource(ErrorString* errorString, const String& in_frameId,
                                       const String& in_url,
                                       const String& in_query,
                                       const Maybe<bool>& in_caseSensitive,
                                       const Maybe<bool>& in_isRegex,
                                       const Maybe<String>& in_requestId,
                                       std::unique_ptr<protocol::Array<protocol::GenericTypes::SearchMatch>>* out_result) {
    bool isRegex = in_isRegex.fromMaybe(false);
    bool isCaseSensitive = in_caseSensitive.fromMaybe(false);

    std::map<std::string, v8_inspector::utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    auto result = protocol::Array<protocol::GenericTypes::SearchMatch>::create();

    auto it = cachedPageResources.find(in_url.utf8());
    if (it == cachedPageResources.end()) {
        *out_result = std::move(result);
        return;
    }

    auto resource = it->second;
    auto content = resource.getContent(errorString);
    if (errorString->isEmpty()) {
        auto matches = v8_inspector::utils::ResourceContentSearchUtils::searchInTextByLinesImpl(m_session, content, in_query, isCaseSensitive, isRegex);
        for (auto& match : matches) {
            result->addItem(std::move(match));
        }

        *out_result = std::move(result);
    }
}

void V8PageAgentImpl::searchInResources(ErrorString*, const String& in_text,
                                        const Maybe<bool>& in_caseSensitive,
                                        const Maybe<bool>& in_isRegex,
                                        std::unique_ptr<protocol::Array<protocol::Page::SearchResult>>* out_result) {
    bool isRegex = in_isRegex.fromMaybe(false);
    bool isCaseSensitive = in_caseSensitive.fromMaybe(false);

    std::map<std::string, v8_inspector::utils::PageResource> cachedPageResources = utils::PageResource::s_cachedPageResources;
    if (utils::PageResource::s_cachedPageResources.size() == 0) {
        cachedPageResources = utils::PageResource::getPageResources();
    }

    std::vector<utils::PageResource> resourceObjects;

    for (auto const& mapKVP : cachedPageResources) {
        resourceObjects.push_back(mapKVP.second);
    }

    *out_result = utils::ResourceContentSearchUtils::getSearchMatches(m_session, resourceObjects, m_frameUrl.c_str(), in_text, isCaseSensitive, isRegex);
}

void V8PageAgentImpl::addScriptToEvaluateOnLoad(ErrorString*, const String& in_scriptSource,
        String* out_identifier) {

}

void V8PageAgentImpl::removeScriptToEvaluateOnLoad(ErrorString*, const String& in_identifier) {

}


void V8PageAgentImpl::reload(ErrorString*, const Maybe<bool>& in_ignoreCache,
                             const Maybe<String>& in_scriptToEvaluateOnLoad) {

}

void V8PageAgentImpl::setDocumentContent(ErrorString*, const String& in_frameId,
        const String& in_html) {

}


void V8PageAgentImpl::reset() {

}
}
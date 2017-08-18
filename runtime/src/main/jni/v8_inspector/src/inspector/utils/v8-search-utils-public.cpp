//
// Created by pkanev on 2/9/2017.
//

#include <v8_inspector/src/inspector/utils/v8-search-utils-public.h>
#include <v8_inspector/src/inspector/v8-page-agent-impl.h>

namespace v8_inspector {
namespace utils {
namespace ResourceContentSearchUtils {
std::unique_ptr<protocol::Array<protocol::Page::SearchResult>> getSearchMatches(V8InspectorSessionImpl* session, std::vector<utils::PageResource>& resources, const String16& frameIdentifier, const String16& text, bool isCaseSensitive, bool isRegex) {
    auto inspector = static_cast<V8InspectorSessionImpl*>(session)->inspector();

    auto regex = createSearchRegex(inspector, text, isCaseSensitive, isRegex);

    auto result = protocol::Array<protocol::Page::SearchResult>::create();

    for (PageResource& resource : resources) {
        ErrorString* errorString;
        String content = resource.getContent(errorString);
        if (errorString->isEmpty()) {
            int matchesCount = scriptRegexpMatchesByLines(*regex, content).size();
            if (matchesCount) {
                result->addItem(std::move(buildObjectForSearchResult(frameIdentifier, resource.getFilePath(), matchesCount)));
            }
        }
    }

    return result;
}

std::unique_ptr<protocol::Page::SearchResult> buildObjectForSearchResult(const String16& frameId, const String16& url, int matchesCount) {
    return protocol::Page::SearchResult::create()
           .setUrl(url)
           .setFrameId(frameId)
           .setMatchesCount(matchesCount)
           .build();
}
}
}
}

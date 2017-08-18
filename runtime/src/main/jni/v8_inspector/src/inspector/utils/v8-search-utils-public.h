//
// Created by pkanev on 2/9/2017.
//

#ifndef V8_SEARCH_UTILS_PUBLIC_H
#define V8_SEARCH_UTILS_PUBLIC_H

#include <v8_inspector/src/inspector/string-16.h>
#include <v8_inspector/src/inspector/v8-regex.h>
#include <v8_inspector/src/inspector/v8-inspector-session-impl.h>
#include <v8_inspector/src/inspector/protocol-platform.h>
#include <v8_inspector/src/inspector/protocol/Debugger.h>
#include "v8-page-resources.h"

namespace v8_inspector {
namespace utils {
namespace ResourceContentSearchUtils {
// Implementation taken from v8_inspector/src/inspector/search-util
String16 createSearchRegexSource(const String16& text) {
    String16Builder result;

    for (size_t i = 0; i < text.length(); i++) {
        UChar c = text[i];
        if (c == '[' || c == ']' || c == '(' || c == ')' || c == '{' || c == '}' ||
                c == '+' || c == '-' || c == '*' || c == '.' || c == ',' || c == '?' ||
                c == '\\' || c == '^' || c == '$' || c == '|') {
            result.append('\\');
        }
        result.append(c);
    }

    return result.toString();
}

// Implementation taken from v8_inspector/src/inspector/search-util
std::unique_ptr<std::vector<size_t>> lineEndings(const String16& text) {
    std::unique_ptr<std::vector<size_t>> result(new std::vector<size_t>());

    const String16 lineEndString = "\n";
    size_t start = 0;
    while (start < text.length()) {
        size_t lineEnd = text.find(lineEndString, start);
        if (lineEnd == String16::kNotFound) {
            break;
        }

        result->push_back(lineEnd);
        start = lineEnd + 1;
    }
    result->push_back(text.length());

    return result;
}

// Implementation taken from v8_inspector/src/inspector/search-util
std::vector<std::pair<int, String16>> scriptRegexpMatchesByLines(
const V8Regex& regex, const String16& text) {
    std::vector<std::pair<int, String16>> result;
    if (text.isEmpty()) {
        return result;
    }

    std::unique_ptr<std::vector<size_t>> endings(lineEndings(text));
    size_t size = endings->size();
    size_t start = 0;
    for (size_t lineNumber = 0; lineNumber < size; ++lineNumber) {
        size_t lineEnd = endings->at(lineNumber);
        String16 line = text.substring(start, lineEnd - start);
        if (line.length() && line[line.length() - 1] == '\r') {
            line = line.substring(0, line.length() - 1);
        }

        int matchLength;
        if (regex.match(line, 0, &matchLength) != -1) {
            result.push_back(std::pair<int, String16>(lineNumber, line));
        }

        start = lineEnd + 1;
    }
    return result;
}

// Implementation taken from v8_inspector/src/inspector/search-util
std::unique_ptr<V8Regex> createSearchRegex(V8InspectorImpl* inspector,
        const String16& query,
        bool caseSensitive, bool isRegex) {
    String16 regexSource = isRegex ? query : createSearchRegexSource(query);
    return wrapUnique(new V8Regex(inspector, regexSource, caseSensitive));
}

// Implementation taken from v8_inspector/src/inspector/search-util
// modified return type to protocol::GenericTypes::SearchMatch (originally protocol::Debugger::SearchMatch)
std::unique_ptr<protocol::GenericTypes::SearchMatch> buildObjectForSearchMatch(
    int lineNumber, const String16& lineContent) {
    return protocol::GenericTypes::SearchMatch::create()
           .setLineNumber(lineNumber)
           .setLineContent(lineContent)
           .build();
}

// Implementation taken from v8_inspector/src/inspector/search-util
std::vector<std::unique_ptr<protocol::GenericTypes::SearchMatch>>
        searchInTextByLinesImpl(V8InspectorSession* session, const String16& text,
                                const String16& query, const bool caseSensitive,
const bool isRegex) {
    std::unique_ptr<V8Regex> regex = createSearchRegex(
                                         static_cast<V8InspectorSessionImpl*>(session)->inspector(), query,
                                         caseSensitive, isRegex);
    std::vector<std::pair<int, String16>> matches =
                                           scriptRegexpMatchesByLines(*regex.get(), text);

    std::vector<std::unique_ptr<protocol::GenericTypes::SearchMatch>> result;
    for (const auto& match : matches) {
        result.push_back(buildObjectForSearchMatch(match.first, match.second));
    }
    return result;
}

std::unique_ptr<protocol::Array<protocol::Page::SearchResult>> getSearchMatches(
            V8InspectorSessionImpl* inspector, std::vector<utils::PageResource>& resources,
            const String16& frameIdentifier, const String16& text, bool isCaseSensitive,
            bool isRegex);

std::unique_ptr<protocol::Page::SearchResult> buildObjectForSearchResult(
    const String16& frameId, const String16& url, int matchesCount);
}
}
}

#endif //V8_SEARCH_UTILS_PUBLIC_H

//
// Created by pkanev on 2/2/2017.
//

#ifndef V8_PAGE_RESOURCES_H
#define V8_PAGE_RESOURCES_H

#include <map>
#include <string>

#include <v8_inspector/src/inspector/protocol/Network.h>
#include <v8_inspector/src/inspector/protocol/Page.h>
#include <v8_inspector/src/inspector/protocol/Protocol.h>

namespace tns {
namespace utils {

namespace protocol = v8_inspector::protocol;

class PageResource {
    public:
        PageResource(std::string filePath, std::string mimeType);
        bool hasTextContent() {
            return strcmp(m_type, protocol::Network::ResourceTypeEnum::Document) == 0 ||
                   strcmp(m_type, protocol::Network::ResourceTypeEnum::Stylesheet) == 0 ||
                   strcmp(m_type, protocol::Network::ResourceTypeEnum::Script) == 0;
        };
        const char* getMimeType() {
            return m_mimeType.c_str();
        };
        const char* getFilePath() {
            return m_filePath.c_str();
        };
        const char*  getType() {
            return m_type;
        };

        /*
         * Get string representation of the resource (file) contents
         * String is base64-encoded if the resource's MIME type isn't a Document | Stylesheet | Script
         */
        v8_inspector::String16 getContent(protocol::String*);

        /*
         * Gets all file paths available in the application's files/app directory
         * Java returns an array of pairs containing the file url and its MIME type
         */
        static std::map<std::string, PageResource> getPageResources();
        static std::map<std::string, PageResource> s_cachedPageResources;

    private:
        std::string m_filePath;
        std::string m_mimeType;
        std::u16string m_content;
        const char* m_type;

        static std::map<std::string, const char*> s_mimeTypeMap;
        /*
         * Maps MIME type strings to their v8 ResourceType counterparts
         * e.g. image/jpeg -> Image; text/javascript -> Script
         */
        static const char* resourceTypeByMimeType(std::string mimeType);
};
}
}

#endif //V8_PAGE_RESOURCES_H

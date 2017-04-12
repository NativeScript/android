//
// Created by pkanev on 3/7/2017.
//

#ifndef V8_NETWORK_REQUEST_DATA_H
#define V8_NETWORK_REQUEST_DATA_H

#include <v8_inspector/src/inspector/protocol/Network.h>

namespace v8_inspector {
namespace utils {

class NetworkRequestData {
    public:
        NetworkRequestData();
        NetworkRequestData(std::string data, bool hasTextContent);
        const char*  getData() {
            return m_data.c_str();
        };

        const bool hasTextContent() {
            return m_hasTextContent;
        }

        void setData(const std::string& data) {
            m_data = data;
        }

        void setHasTextContent(const bool hasTextContent) {
            m_hasTextContent = hasTextContent;
        }

    private:
        std::string m_data;
        bool m_hasTextContent;
};
}
}

#endif //V8_NETWORK_REQUEST_DATA_H

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
        NetworkRequestData(std::u16string data, bool hasTextContent);
        const char16_t*  getData() {
            return m_data.data();
        };

        const bool hasTextContent() {
            return m_hasTextContent;
        }

        void setData(const std::u16string& data) {
            m_data = data;
        }

        void setHasTextContent(const bool hasTextContent) {
            m_hasTextContent = hasTextContent;
        }

    private:
        std::u16string m_data;
        bool m_hasTextContent;
};
}
}

#endif //V8_NETWORK_REQUEST_DATA_H

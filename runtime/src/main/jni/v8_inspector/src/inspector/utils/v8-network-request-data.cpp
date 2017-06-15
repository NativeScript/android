//
// Created by pkanev on 3/10/2017.
//

#include "v8-network-request-data.h"

namespace v8_inspector {
namespace utils {
NetworkRequestData::NetworkRequestData()
    : m_data(),
      m_hasTextContent(true) { }
NetworkRequestData::NetworkRequestData(std::u16string data, bool hasTextContent)
    : m_data(data),
      m_hasTextContent(hasTextContent) { }
}
}
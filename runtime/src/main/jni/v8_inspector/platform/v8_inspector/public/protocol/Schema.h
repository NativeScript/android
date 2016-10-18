// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef protocol_Schema_api_h
#define protocol_Schema_api_h

#include "platform/PlatformExport.h"
#include "platform/inspector_protocol/InspectorProtocol.h"

namespace blink {
namespace protocol {
namespace Schema {
namespace API {

// ------------- Enums.

// ------------- Types.

class PLATFORM_EXPORT Domain {
public:
    virtual String16 toJSONString() const = 0;
    virtual ~Domain() { }
    static std::unique_ptr<protocol::Schema::API::Domain> fromJSONString(const String16& json);
};

} // namespace API
} // namespace Schema
} // namespace protocol
} // namespace blink

#endif // !defined(protocol_Schema_api_h)

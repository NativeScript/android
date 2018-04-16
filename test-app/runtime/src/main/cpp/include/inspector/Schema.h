// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Schema_api_h
#define v8_inspector_protocol_Schema_api_h

#include "v8-inspector.h"

namespace v8_inspector {
namespace protocol {
namespace Schema {
namespace API {

// ------------- Enums.

// ------------- Types.

class V8_EXPORT Domain {
public:
    virtual std::unique_ptr<StringBuffer> toJSONString() const = 0;
    virtual ~Domain() { }
    static std::unique_ptr<protocol::Schema::API::Domain> fromJSONString(const StringView& json);
};

} // namespace API
} // namespace Schema
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Schema_api_h)

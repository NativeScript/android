// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Runtime_api_h
#define v8_inspector_protocol_Runtime_api_h

#include "v8-inspector.h"

namespace v8_inspector {
namespace protocol {
namespace Runtime {
namespace API {

// ------------- Enums.

// ------------- Types.

class V8_EXPORT RemoteObject {
public:
    virtual std::unique_ptr<StringBuffer> toJSONString() const = 0;
    virtual ~RemoteObject() { }
    static std::unique_ptr<protocol::Runtime::API::RemoteObject> fromJSONString(const StringView& json);
};

class V8_EXPORT StackTrace {
public:
    virtual std::unique_ptr<StringBuffer> toJSONString() const = 0;
    virtual ~StackTrace() { }
    static std::unique_ptr<protocol::Runtime::API::StackTrace> fromJSONString(const StringView& json);
};

class V8_EXPORT StackTraceId {
public:
    virtual std::unique_ptr<StringBuffer> toJSONString() const = 0;
    virtual ~StackTraceId() { }
    static std::unique_ptr<protocol::Runtime::API::StackTraceId> fromJSONString(const StringView& json);
};

} // namespace API
} // namespace Runtime
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Runtime_api_h)

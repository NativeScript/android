// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Debugger_api_h
#define v8_inspector_protocol_Debugger_api_h

#include "v8-inspector.h"

namespace v8_inspector {
namespace protocol {
namespace Debugger {
namespace API {

// ------------- Enums.

namespace Paused {
namespace ReasonEnum {
V8_EXPORT extern const char* XHR;
V8_EXPORT extern const char* DOM;
V8_EXPORT extern const char* EventListener;
V8_EXPORT extern const char* Exception;
V8_EXPORT extern const char* Assert;
V8_EXPORT extern const char* DebugCommand;
V8_EXPORT extern const char* PromiseRejection;
V8_EXPORT extern const char* Other;
} // ReasonEnum
} // Paused

// ------------- Types.

class V8_EXPORT SearchMatch {
public:
    virtual std::unique_ptr<StringBuffer> toJSONString() const = 0;
    virtual ~SearchMatch() { }
    static std::unique_ptr<protocol::Debugger::API::SearchMatch> fromJSONString(const StringView& json);
};

} // namespace API
} // namespace Debugger
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Debugger_api_h)

// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/GenericTypes.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace GenericTypes {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "GenericTypes";
const char Metainfo::commandPrefix[] = "GenericTypes.";
const char Metainfo::version[] = "1.2";

std::unique_ptr<SearchMatch> SearchMatch::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SearchMatch> result(new SearchMatch());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* lineNumberValue = object->get("lineNumber");
    errors->setName("lineNumber");
    result->m_lineNumber = ValueConversions<double>::fromValue(lineNumberValue, errors);
    protocol::Value* lineContentValue = object->get("lineContent");
    errors->setName("lineContent");
    result->m_lineContent = ValueConversions<String>::fromValue(lineContentValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SearchMatch::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("lineNumber", ValueConversions<double>::toValue(m_lineNumber));
    result->setValue("lineContent", ValueConversions<String>::toValue(m_lineContent));
    return result;
}

std::unique_ptr<SearchMatch> SearchMatch::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::flush() {
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification) {
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
    public:
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend, bool fallThroughForNotFound)
            : DispatcherBase(frontendChannel)
            , m_backend(backend)
            , m_fallThroughForNotFound(fallThroughForNotFound) {
        }
        ~DispatcherImpl() override { }
        DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

    protected:
        using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;


        Backend* m_backend;
        bool m_fallThroughForNotFound;
};

DispatchResponse::Status DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) {
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        if (m_fallThroughForNotFound) {
            return DispatchResponse::kFallThrough;
        }
        reportProtocolError(callId, DispatchResponse::kMethodNotFound, "'" + method + "' wasn't found", nullptr);
        return DispatchResponse::kError;
    }

    protocol::ErrorSupport errors;
    return (this->*(it->second))(callId, std::move(messageObject), &errors);
}


// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("GenericTypes", std::unique_ptr<protocol::DispatcherBase>(new DispatcherImpl(dispatcher->channel(), backend, dispatcher->fallThroughForNotFound())));
}

} // GenericTypes
} // namespace v8_inspector
} // namespace protocol

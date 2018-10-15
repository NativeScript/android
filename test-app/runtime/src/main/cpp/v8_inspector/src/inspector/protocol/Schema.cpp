// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Schema.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Schema {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Schema";
const char Metainfo::commandPrefix[] = "Schema.";
const char Metainfo::version[] = "1.3";

std::unique_ptr<Domain> Domain::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Domain> result(new Domain());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* versionValue = object->get("version");
    errors->setName("version");
    result->m_version = ValueConversions<String>::fromValue(versionValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Domain::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("version", ValueConversions<String>::toValue(m_version));
    return result;
}

std::unique_ptr<Domain> Domain::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StringBuffer> Domain::toJSONString() const
{
    String json = toValue()->serialize();
    return StringBufferImpl::adopt(json);
}

// static
std::unique_ptr<API::Domain> API::Domain::fromJSONString(const StringView& json)
{
    ErrorSupport errors;
    std::unique_ptr<Value> value = StringUtil::parseJSON(json);
    if (!value)
        return nullptr;
    return protocol::Schema::Domain::fromValue(value.get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::flush()
{
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification)
{
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
public:
    DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend, bool fallThroughForNotFound)
        : DispatcherBase(frontendChannel)
        , m_backend(backend)
        , m_fallThroughForNotFound(fallThroughForNotFound) {
        m_dispatchMap["Schema.getDomains"] = &DispatcherImpl::getDomains;
    }
    ~DispatcherImpl() override { }
    DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
    HashMap<String, String>& redirects() { return m_redirects; }

protected:
    using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String, CallHandler>;
    DispatchMap m_dispatchMap;
    HashMap<String, String> m_redirects;

    DispatchResponse::Status getDomains(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

    Backend* m_backend;
    bool m_fallThroughForNotFound;
};

DispatchResponse::Status DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject)
{
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        if (m_fallThroughForNotFound)
            return DispatchResponse::kFallThrough;
        reportProtocolError(callId, DispatchResponse::kMethodNotFound, "'" + method + "' wasn't found", nullptr);
        return DispatchResponse::kError;
    }

    protocol::ErrorSupport errors;
    return (this->*(it->second))(callId, std::move(messageObject), &errors);
}


DispatchResponse::Status DispatcherImpl::getDomains(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Schema::Domain>> out_domains;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getDomains(&out_domains);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("domains", ValueConversions<protocol::Array<protocol::Schema::Domain>>::toValue(out_domains.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
{
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Schema", std::move(dispatcher));
}

} // Schema
} // namespace v8_inspector
} // namespace protocol

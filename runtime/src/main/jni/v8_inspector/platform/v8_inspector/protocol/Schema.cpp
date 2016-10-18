// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "platform/v8_inspector/protocol/Schema.h"

namespace blink {
namespace protocol {
namespace Schema {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Schema";
const char Metainfo::commandPrefix[] = "Schema.";
const char Metainfo::version[] = "1.1";

std::unique_ptr<Domain> Domain::parse(protocol::Value* value, ErrorSupport* errors)
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
    result->m_name = ValueConversions<String16>::parse(nameValue, errors);
    protocol::Value* versionValue = object->get("version");
    errors->setName("version");
    result->m_version = ValueConversions<String16>::parse(versionValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Domain::serialize() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String16>::serialize(m_name));
    result->setValue("version", ValueConversions<String16>::serialize(m_version));
    return result;
}

std::unique_ptr<Domain> Domain::clone() const
{
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

String16 Domain::toJSONString() const
{
    return serialize()->toJSONString();
}

// static
std::unique_ptr<API::Domain> API::Domain::fromJSONString(const String16& json)
{
    ErrorSupport errors;
    std::unique_ptr<Value> value = parseJSON(json);
    if (!value)
        return nullptr;
    return protocol::Schema::Domain::parse(value.get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
public:
    DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend)
        : DispatcherBase(frontendChannel)
        , m_backend(backend) {
        m_dispatchMap["Schema.getDomains"] = &DispatcherImpl::getDomains;
    }
    ~DispatcherImpl() override { }
    void dispatch(int callId, const String16& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

protected:
    using CallHandler = void (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String16, CallHandler>;
    DispatchMap m_dispatchMap;

    void getDomains(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

    Backend* m_backend;
};

void DispatcherImpl::dispatch(int callId, const String16& method, std::unique_ptr<protocol::DictionaryValue> messageObject)
{
    protocol::HashMap<String16, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        reportProtocolError(callId, MethodNotFound, "'" + method + "' wasn't found", nullptr);
        return;
    }

    protocol::ErrorSupport errors;
    (this->*(it->second))(callId, std::move(messageObject), &errors);
}


void DispatcherImpl::getDomains(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<protocol::Schema::Domain>> out_domains;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getDomains(&error, &out_domains);
    if (!error.length()) {
        result->setValue("domains", ValueConversions<protocol::Array<protocol::Schema::Domain>>::serialize(out_domains.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend)
{
    dispatcher->registerBackend("Schema", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // Schema
} // namespace protocol
} // namespace blink

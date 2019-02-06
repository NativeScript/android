// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Log.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Log {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Log";
const char Metainfo::commandPrefix[] = "Log.";
const char Metainfo::version[] = "1.3";

const char* LogEntry::SourceEnum::Xml = "xml";
const char* LogEntry::SourceEnum::Javascript = "javascript";
const char* LogEntry::SourceEnum::Network = "network";
const char* LogEntry::SourceEnum::Storage = "storage";
const char* LogEntry::SourceEnum::Appcache = "appcache";
const char* LogEntry::SourceEnum::Rendering = "rendering";
const char* LogEntry::SourceEnum::Security = "security";
const char* LogEntry::SourceEnum::Deprecation = "deprecation";
const char* LogEntry::SourceEnum::Worker = "worker";
const char* LogEntry::SourceEnum::Violation = "violation";
const char* LogEntry::SourceEnum::Intervention = "intervention";
const char* LogEntry::SourceEnum::Recommendation = "recommendation";
const char* LogEntry::SourceEnum::Other = "other";

const char* LogEntry::LevelEnum::Verbose = "verbose";
const char* LogEntry::LevelEnum::Info = "info";
const char* LogEntry::LevelEnum::Warning = "warning";
const char* LogEntry::LevelEnum::Error = "error";

std::unique_ptr<LogEntry> LogEntry::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<LogEntry> result(new LogEntry());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* sourceValue = object->get("source");
    errors->setName("source");
    result->m_source = ValueConversions<String>::fromValue(sourceValue, errors);
    protocol::Value* levelValue = object->get("level");
    errors->setName("level");
    result->m_level = ValueConversions<String>::fromValue(levelValue, errors);
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::fromValue(textValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* urlValue = object->get("url");
    if (urlValue) {
        errors->setName("url");
        result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    }
    protocol::Value* lineNumberValue = object->get("lineNumber");
    if (lineNumberValue) {
        errors->setName("lineNumber");
        result->m_lineNumber = ValueConversions<int>::fromValue(lineNumberValue, errors);
    }
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::fromValue(stackTraceValue, errors);
    }
    protocol::Value* networkRequestIdValue = object->get("networkRequestId");
    if (networkRequestIdValue) {
        errors->setName("networkRequestId");
        result->m_networkRequestId = ValueConversions<String>::fromValue(networkRequestIdValue, errors);
    }
    protocol::Value* workerIdValue = object->get("workerId");
    if (workerIdValue) {
        errors->setName("workerId");
        result->m_workerId = ValueConversions<String>::fromValue(workerIdValue, errors);
    }
    protocol::Value* argsValue = object->get("args");
    if (argsValue) {
        errors->setName("args");
        result->m_args = ValueConversions<protocol::Array<protocol::Runtime::RemoteObject>>::fromValue(argsValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LogEntry::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("source", ValueConversions<String>::toValue(m_source));
    result->setValue("level", ValueConversions<String>::toValue(m_level));
    result->setValue("text", ValueConversions<String>::toValue(m_text));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    if (m_url.isJust()) {
        result->setValue("url", ValueConversions<String>::toValue(m_url.fromJust()));
    }
    if (m_lineNumber.isJust()) {
        result->setValue("lineNumber", ValueConversions<int>::toValue(m_lineNumber.fromJust()));
    }
    if (m_stackTrace.isJust()) {
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_stackTrace.fromJust()));
    }
    if (m_networkRequestId.isJust()) {
        result->setValue("networkRequestId", ValueConversions<String>::toValue(m_networkRequestId.fromJust()));
    }
    if (m_workerId.isJust()) {
        result->setValue("workerId", ValueConversions<String>::toValue(m_workerId.fromJust()));
    }
    if (m_args.isJust()) {
        result->setValue("args", ValueConversions<protocol::Array<protocol::Runtime::RemoteObject>>::toValue(m_args.fromJust()));
    }
    return result;
}

std::unique_ptr<LogEntry> LogEntry::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* ViolationSetting::NameEnum::LongTask = "longTask";
const char* ViolationSetting::NameEnum::LongLayout = "longLayout";
const char* ViolationSetting::NameEnum::BlockedEvent = "blockedEvent";
const char* ViolationSetting::NameEnum::BlockedParser = "blockedParser";
const char* ViolationSetting::NameEnum::DiscouragedAPIUse = "discouragedAPIUse";
const char* ViolationSetting::NameEnum::Handler = "handler";
const char* ViolationSetting::NameEnum::RecurringHandler = "recurringHandler";

std::unique_ptr<ViolationSetting> ViolationSetting::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ViolationSetting> result(new ViolationSetting());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* thresholdValue = object->get("threshold");
    errors->setName("threshold");
    result->m_threshold = ValueConversions<double>::fromValue(thresholdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ViolationSetting::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("threshold", ValueConversions<double>::toValue(m_threshold));
    return result;
}

std::unique_ptr<ViolationSetting> ViolationSetting::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<EntryAddedNotification> EntryAddedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<EntryAddedNotification> result(new EntryAddedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* entryValue = object->get("entry");
    errors->setName("entry");
    result->m_entry = ValueConversions<protocol::Log::LogEntry>::fromValue(entryValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> EntryAddedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("entry", ValueConversions<protocol::Log::LogEntry>::toValue(m_entry.get()));
    return result;
}

std::unique_ptr<EntryAddedNotification> EntryAddedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::entryAdded(std::unique_ptr<protocol::Log::LogEntry> entry) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<EntryAddedNotification> messageData = EntryAddedNotification::create()
            .setEntry(std::move(entry))
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Log.entryAdded", std::move(messageData)));
}

void Frontend::flush() {
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification) {
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
    public:
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend)
            : DispatcherBase(frontendChannel)
            , m_backend(backend) {
            m_dispatchMap["Log.clear"] = &DispatcherImpl::clear;
            m_dispatchMap["Log.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["Log.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["Log.startViolationsReport"] = &DispatcherImpl::startViolationsReport;
            m_dispatchMap["Log.stopViolationsReport"] = &DispatcherImpl::stopViolationsReport;
        }
        ~DispatcherImpl() override { }
        bool canDispatch(const String& method) override;
        void dispatch(int callId, const String& method, const String& message, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
        std::unordered_map<String, String>& redirects() {
            return m_redirects;
        }

    protected:
        using CallHandler = void (DispatcherImpl::*)(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = std::unordered_map<String, CallHandler>;
        DispatchMap m_dispatchMap;
        std::unordered_map<String, String> m_redirects;

        void clear(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void disable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void enable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void startViolationsReport(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void stopViolationsReport(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

        Backend* m_backend;
};

bool DispatcherImpl::canDispatch(const String& method) {
    return m_dispatchMap.find(method) != m_dispatchMap.end();
}

void DispatcherImpl::dispatch(int callId, const String& method, const String& message, std::unique_ptr<protocol::DictionaryValue> messageObject) {
    std::unordered_map<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    DCHECK(it != m_dispatchMap.end());
    protocol::ErrorSupport errors;
    (this->*(it->second))(callId, method, message, std::move(messageObject), &errors);
}


void DispatcherImpl::clear(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->clear();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::disable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::enable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::startViolationsReport(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* configValue = object ? object->get("config") : nullptr;
    errors->setName("config");
    std::unique_ptr<protocol::Array<protocol::Log::ViolationSetting>> in_config = ValueConversions<protocol::Array<protocol::Log::ViolationSetting>>::fromValue(configValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->startViolationsReport(std::move(in_config));
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::stopViolationsReport(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stopViolationsReport();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend) {
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Log", std::move(dispatcher));
}

} // Log
} // namespace v8_inspector
} // namespace protocol

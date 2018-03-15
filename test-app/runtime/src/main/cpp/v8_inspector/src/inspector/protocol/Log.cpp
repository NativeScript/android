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
const char Metainfo::version[] = "1.2";

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

std::unique_ptr<LogEntry> LogEntry::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<LogEntry> result(new LogEntry());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* sourceValue = object->get("source");
    errors->setName("source");
    result->m_source = ValueConversions<String>::parse(sourceValue, errors);
    protocol::Value* levelValue = object->get("level");
    errors->setName("level");
    result->m_level = ValueConversions<String>::parse(levelValue, errors);
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::parse(textValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::parse(timestampValue, errors);
    protocol::Value* urlValue = object->get("url");
    if (urlValue) {
        errors->setName("url");
        result->m_url = ValueConversions<String>::parse(urlValue, errors);
    }
    protocol::Value* lineNumberValue = object->get("lineNumber");
    if (lineNumberValue) {
        errors->setName("lineNumber");
        result->m_lineNumber = ValueConversions<int>::parse(lineNumberValue, errors);
    }
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::parse(stackTraceValue, errors);
    }
    protocol::Value* networkRequestIdValue = object->get("networkRequestId");
    if (networkRequestIdValue) {
        errors->setName("networkRequestId");
        result->m_networkRequestId = ValueConversions<String>::parse(networkRequestIdValue, errors);
    }
    protocol::Value* workerIdValue = object->get("workerId");
    if (workerIdValue) {
        errors->setName("workerId");
        result->m_workerId = ValueConversions<String>::parse(workerIdValue, errors);
    }
    protocol::Value* argsValue = object->get("args");
    if (argsValue) {
        errors->setName("args");
        result->m_args = ValueConversions<protocol::Array<protocol::Runtime::RemoteObject>>::parse(argsValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LogEntry::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("source", ValueConversions<String>::serialize(m_source));
    result->setValue("level", ValueConversions<String>::serialize(m_level));
    result->setValue("text", ValueConversions<String>::serialize(m_text));
    result->setValue("timestamp", ValueConversions<double>::serialize(m_timestamp));
    if (m_url.isJust()) {
        result->setValue("url", ValueConversions<String>::serialize(m_url.fromJust()));
    }
    if (m_lineNumber.isJust()) {
        result->setValue("lineNumber", ValueConversions<int>::serialize(m_lineNumber.fromJust()));
    }
    if (m_stackTrace.isJust()) {
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::serialize(m_stackTrace.fromJust()));
    }
    if (m_networkRequestId.isJust()) {
        result->setValue("networkRequestId", ValueConversions<String>::serialize(m_networkRequestId.fromJust()));
    }
    if (m_workerId.isJust()) {
        result->setValue("workerId", ValueConversions<String>::serialize(m_workerId.fromJust()));
    }
    if (m_args.isJust()) {
        result->setValue("args", ValueConversions<protocol::Array<protocol::Runtime::RemoteObject>>::serialize(m_args.fromJust()));
    }
    return result;
}

std::unique_ptr<LogEntry> LogEntry::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

const char* ViolationSetting::NameEnum::LongTask = "longTask";
const char* ViolationSetting::NameEnum::LongLayout = "longLayout";
const char* ViolationSetting::NameEnum::BlockedEvent = "blockedEvent";
const char* ViolationSetting::NameEnum::BlockedParser = "blockedParser";
const char* ViolationSetting::NameEnum::DiscouragedAPIUse = "discouragedAPIUse";
const char* ViolationSetting::NameEnum::Handler = "handler";
const char* ViolationSetting::NameEnum::RecurringHandler = "recurringHandler";

std::unique_ptr<ViolationSetting> ViolationSetting::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ViolationSetting> result(new ViolationSetting());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::parse(nameValue, errors);
    protocol::Value* thresholdValue = object->get("threshold");
    errors->setName("threshold");
    result->m_threshold = ValueConversions<double>::parse(thresholdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ViolationSetting::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::serialize(m_name));
    result->setValue("threshold", ValueConversions<double>::serialize(m_threshold));
    return result;
}

std::unique_ptr<ViolationSetting> ViolationSetting::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::entryAdded(std::unique_ptr<protocol::Log::LogEntry> entry) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Log.entryAdded");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("entry", ValueConversions<protocol::Log::LogEntry>::serialize(entry.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::flush() {
    m_frontendChannel->flushProtocolNotifications();
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
    public:
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend)
            : DispatcherBase(frontendChannel)
            , m_backend(backend) {
            m_dispatchMap["Log.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["Log.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["Log.clear"] = &DispatcherImpl::clear;
            m_dispatchMap["Log.startViolationsReport"] = &DispatcherImpl::startViolationsReport;
            m_dispatchMap["Log.stopViolationsReport"] = &DispatcherImpl::stopViolationsReport;
        }
        ~DispatcherImpl() override { }
        void dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

    protected:
        using CallHandler = void (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;

        void enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void clear(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void startViolationsReport(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void stopViolationsReport(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

        Backend* m_backend;
};

void DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) {
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        reportProtocolError(callId, MethodNotFound, "'" + method + "' wasn't found", nullptr);
        return;
    }

    protocol::ErrorSupport errors;
    (this->*(it->second))(callId, std::move(messageObject), &errors);
}


void DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->enable(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->disable(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::clear(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->clear(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::startViolationsReport(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* configValue = object ? object->get("config") : nullptr;
    errors->setName("config");
    std::unique_ptr<protocol::Array<protocol::Log::ViolationSetting>> in_config = ValueConversions<protocol::Array<protocol::Log::ViolationSetting>>::parse(configValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->startViolationsReport(&error, std::move(in_config));
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::stopViolationsReport(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->stopViolationsReport(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("Log", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // Log
} // namespace v8_inspector
} // namespace protocol

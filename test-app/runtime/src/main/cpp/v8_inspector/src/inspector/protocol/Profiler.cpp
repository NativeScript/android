// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Profiler.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Profiler {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Profiler";
const char Metainfo::commandPrefix[] = "Profiler.";
const char Metainfo::version[] = "1.2";

std::unique_ptr<ProfileNode> ProfileNode::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ProfileNode> result(new ProfileNode());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<int>::fromValue(idValue, errors);
    protocol::Value* callFrameValue = object->get("callFrame");
    errors->setName("callFrame");
    result->m_callFrame = ValueConversions<protocol::Runtime::CallFrame>::fromValue(callFrameValue, errors);
    protocol::Value* hitCountValue = object->get("hitCount");
    if (hitCountValue) {
        errors->setName("hitCount");
        result->m_hitCount = ValueConversions<int>::fromValue(hitCountValue, errors);
    }
    protocol::Value* childrenValue = object->get("children");
    if (childrenValue) {
        errors->setName("children");
        result->m_children = ValueConversions<protocol::Array<int>>::fromValue(childrenValue, errors);
    }
    protocol::Value* deoptReasonValue = object->get("deoptReason");
    if (deoptReasonValue) {
        errors->setName("deoptReason");
        result->m_deoptReason = ValueConversions<String>::fromValue(deoptReasonValue, errors);
    }
    protocol::Value* positionTicksValue = object->get("positionTicks");
    if (positionTicksValue) {
        errors->setName("positionTicks");
        result->m_positionTicks = ValueConversions<protocol::Array<protocol::Profiler::PositionTickInfo>>::fromValue(positionTicksValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ProfileNode::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<int>::toValue(m_id));
    result->setValue("callFrame", ValueConversions<protocol::Runtime::CallFrame>::toValue(m_callFrame.get()));
    if (m_hitCount.isJust()) {
        result->setValue("hitCount", ValueConversions<int>::toValue(m_hitCount.fromJust()));
    }
    if (m_children.isJust()) {
        result->setValue("children", ValueConversions<protocol::Array<int>>::toValue(m_children.fromJust()));
    }
    if (m_deoptReason.isJust()) {
        result->setValue("deoptReason", ValueConversions<String>::toValue(m_deoptReason.fromJust()));
    }
    if (m_positionTicks.isJust()) {
        result->setValue("positionTicks", ValueConversions<protocol::Array<protocol::Profiler::PositionTickInfo>>::toValue(m_positionTicks.fromJust()));
    }
    return result;
}

std::unique_ptr<ProfileNode> ProfileNode::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<Profile> Profile::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Profile> result(new Profile());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nodesValue = object->get("nodes");
    errors->setName("nodes");
    result->m_nodes = ValueConversions<protocol::Array<protocol::Profiler::ProfileNode>>::fromValue(nodesValue, errors);
    protocol::Value* startTimeValue = object->get("startTime");
    errors->setName("startTime");
    result->m_startTime = ValueConversions<double>::fromValue(startTimeValue, errors);
    protocol::Value* endTimeValue = object->get("endTime");
    errors->setName("endTime");
    result->m_endTime = ValueConversions<double>::fromValue(endTimeValue, errors);
    protocol::Value* samplesValue = object->get("samples");
    if (samplesValue) {
        errors->setName("samples");
        result->m_samples = ValueConversions<protocol::Array<int>>::fromValue(samplesValue, errors);
    }
    protocol::Value* timeDeltasValue = object->get("timeDeltas");
    if (timeDeltasValue) {
        errors->setName("timeDeltas");
        result->m_timeDeltas = ValueConversions<protocol::Array<int>>::fromValue(timeDeltasValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Profile::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodes", ValueConversions<protocol::Array<protocol::Profiler::ProfileNode>>::toValue(m_nodes.get()));
    result->setValue("startTime", ValueConversions<double>::toValue(m_startTime));
    result->setValue("endTime", ValueConversions<double>::toValue(m_endTime));
    if (m_samples.isJust()) {
        result->setValue("samples", ValueConversions<protocol::Array<int>>::toValue(m_samples.fromJust()));
    }
    if (m_timeDeltas.isJust()) {
        result->setValue("timeDeltas", ValueConversions<protocol::Array<int>>::toValue(m_timeDeltas.fromJust()));
    }
    return result;
}

std::unique_ptr<Profile> Profile::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<PositionTickInfo> PositionTickInfo::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PositionTickInfo> result(new PositionTickInfo());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* lineValue = object->get("line");
    errors->setName("line");
    result->m_line = ValueConversions<int>::fromValue(lineValue, errors);
    protocol::Value* ticksValue = object->get("ticks");
    errors->setName("ticks");
    result->m_ticks = ValueConversions<int>::fromValue(ticksValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PositionTickInfo::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("line", ValueConversions<int>::toValue(m_line));
    result->setValue("ticks", ValueConversions<int>::toValue(m_ticks));
    return result;
}

std::unique_ptr<PositionTickInfo> PositionTickInfo::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CoverageRange> CoverageRange::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CoverageRange> result(new CoverageRange());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* startOffsetValue = object->get("startOffset");
    errors->setName("startOffset");
    result->m_startOffset = ValueConversions<int>::fromValue(startOffsetValue, errors);
    protocol::Value* endOffsetValue = object->get("endOffset");
    errors->setName("endOffset");
    result->m_endOffset = ValueConversions<int>::fromValue(endOffsetValue, errors);
    protocol::Value* countValue = object->get("count");
    errors->setName("count");
    result->m_count = ValueConversions<int>::fromValue(countValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CoverageRange::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("startOffset", ValueConversions<int>::toValue(m_startOffset));
    result->setValue("endOffset", ValueConversions<int>::toValue(m_endOffset));
    result->setValue("count", ValueConversions<int>::toValue(m_count));
    return result;
}

std::unique_ptr<CoverageRange> CoverageRange::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FunctionCoverage> FunctionCoverage::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FunctionCoverage> result(new FunctionCoverage());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* functionNameValue = object->get("functionName");
    errors->setName("functionName");
    result->m_functionName = ValueConversions<String>::fromValue(functionNameValue, errors);
    protocol::Value* rangesValue = object->get("ranges");
    errors->setName("ranges");
    result->m_ranges = ValueConversions<protocol::Array<protocol::Profiler::CoverageRange>>::fromValue(rangesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FunctionCoverage::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("functionName", ValueConversions<String>::toValue(m_functionName));
    result->setValue("ranges", ValueConversions<protocol::Array<protocol::Profiler::CoverageRange>>::toValue(m_ranges.get()));
    return result;
}

std::unique_ptr<FunctionCoverage> FunctionCoverage::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScriptCoverage> ScriptCoverage::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScriptCoverage> result(new ScriptCoverage());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* scriptIdValue = object->get("scriptId");
    errors->setName("scriptId");
    result->m_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* functionsValue = object->get("functions");
    errors->setName("functions");
    result->m_functions = ValueConversions<protocol::Array<protocol::Profiler::FunctionCoverage>>::fromValue(functionsValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScriptCoverage::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("scriptId", ValueConversions<String>::toValue(m_scriptId));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("functions", ValueConversions<protocol::Array<protocol::Profiler::FunctionCoverage>>::toValue(m_functions.get()));
    return result;
}

std::unique_ptr<ScriptCoverage> ScriptCoverage::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ConsoleProfileStartedNotification> ConsoleProfileStartedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ConsoleProfileStartedNotification> result(new ConsoleProfileStartedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<String>::fromValue(idValue, errors);
    protocol::Value* locationValue = object->get("location");
    errors->setName("location");
    result->m_location = ValueConversions<protocol::Debugger::Location>::fromValue(locationValue, errors);
    protocol::Value* titleValue = object->get("title");
    if (titleValue) {
        errors->setName("title");
        result->m_title = ValueConversions<String>::fromValue(titleValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ConsoleProfileStartedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<String>::toValue(m_id));
    result->setValue("location", ValueConversions<protocol::Debugger::Location>::toValue(m_location.get()));
    if (m_title.isJust()) {
        result->setValue("title", ValueConversions<String>::toValue(m_title.fromJust()));
    }
    return result;
}

std::unique_ptr<ConsoleProfileStartedNotification> ConsoleProfileStartedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ConsoleProfileFinishedNotification> ConsoleProfileFinishedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ConsoleProfileFinishedNotification> result(new ConsoleProfileFinishedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<String>::fromValue(idValue, errors);
    protocol::Value* locationValue = object->get("location");
    errors->setName("location");
    result->m_location = ValueConversions<protocol::Debugger::Location>::fromValue(locationValue, errors);
    protocol::Value* profileValue = object->get("profile");
    errors->setName("profile");
    result->m_profile = ValueConversions<protocol::Profiler::Profile>::fromValue(profileValue, errors);
    protocol::Value* titleValue = object->get("title");
    if (titleValue) {
        errors->setName("title");
        result->m_title = ValueConversions<String>::fromValue(titleValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ConsoleProfileFinishedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<String>::toValue(m_id));
    result->setValue("location", ValueConversions<protocol::Debugger::Location>::toValue(m_location.get()));
    result->setValue("profile", ValueConversions<protocol::Profiler::Profile>::toValue(m_profile.get()));
    if (m_title.isJust()) {
        result->setValue("title", ValueConversions<String>::toValue(m_title.fromJust()));
    }
    return result;
}

std::unique_ptr<ConsoleProfileFinishedNotification> ConsoleProfileFinishedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::consoleProfileStarted(const String& id, std::unique_ptr<protocol::Debugger::Location> location, Maybe<String> title) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<ConsoleProfileStartedNotification> messageData = ConsoleProfileStartedNotification::create()
            .setId(id)
            .setLocation(std::move(location))
            .build();
    if (title.isJust()) {
        messageData->setTitle(std::move(title).takeJust());
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Profiler.consoleProfileStarted", std::move(messageData)));
}

void Frontend::consoleProfileFinished(const String& id, std::unique_ptr<protocol::Debugger::Location> location, std::unique_ptr<protocol::Profiler::Profile> profile, Maybe<String> title) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<ConsoleProfileFinishedNotification> messageData = ConsoleProfileFinishedNotification::create()
            .setId(id)
            .setLocation(std::move(location))
            .setProfile(std::move(profile))
            .build();
    if (title.isJust()) {
        messageData->setTitle(std::move(title).takeJust());
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Profiler.consoleProfileFinished", std::move(messageData)));
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
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend, bool fallThroughForNotFound)
            : DispatcherBase(frontendChannel)
            , m_backend(backend)
            , m_fallThroughForNotFound(fallThroughForNotFound) {
            m_dispatchMap["Profiler.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["Profiler.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["Profiler.setSamplingInterval"] = &DispatcherImpl::setSamplingInterval;
            m_dispatchMap["Profiler.start"] = &DispatcherImpl::start;
            m_dispatchMap["Profiler.stop"] = &DispatcherImpl::stop;
            m_dispatchMap["Profiler.startPreciseCoverage"] = &DispatcherImpl::startPreciseCoverage;
            m_dispatchMap["Profiler.stopPreciseCoverage"] = &DispatcherImpl::stopPreciseCoverage;
            m_dispatchMap["Profiler.takePreciseCoverage"] = &DispatcherImpl::takePreciseCoverage;
            m_dispatchMap["Profiler.getBestEffortCoverage"] = &DispatcherImpl::getBestEffortCoverage;
        }
        ~DispatcherImpl() override { }
        DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

    protected:
        using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;

        DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status setSamplingInterval(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status start(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status stop(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status startPreciseCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status stopPreciseCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status takePreciseCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getBestEffortCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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


DispatchResponse::Status DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setSamplingInterval(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* intervalValue = object ? object->get("interval") : nullptr;
    errors->setName("interval");
    int in_interval = ValueConversions<int>::fromValue(intervalValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setSamplingInterval(in_interval);
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::start(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->start();
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::stop(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Profiler::Profile> out_profile;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stop(&out_profile);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("profile", ValueConversions<protocol::Profiler::Profile>::toValue(out_profile.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::startPreciseCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* callCountValue = object ? object->get("callCount") : nullptr;
    Maybe<bool> in_callCount;
    if (callCountValue) {
        errors->setName("callCount");
        in_callCount = ValueConversions<bool>::fromValue(callCountValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->startPreciseCoverage(std::move(in_callCount));
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::stopPreciseCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stopPreciseCoverage();
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::takePreciseCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Profiler::ScriptCoverage>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->takePreciseCoverage(&out_result);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::Profiler::ScriptCoverage>>::toValue(out_result.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getBestEffortCoverage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Profiler::ScriptCoverage>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getBestEffortCoverage(&out_result);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::Profiler::ScriptCoverage>>::toValue(out_result.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("Profiler", std::unique_ptr<protocol::DispatcherBase>(new DispatcherImpl(dispatcher->channel(), backend, dispatcher->fallThroughForNotFound())));
}

} // Profiler
} // namespace v8_inspector
} // namespace protocol

// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Debugger.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Debugger {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Debugger";
const char Metainfo::commandPrefix[] = "Debugger.";
const char Metainfo::version[] = "1.3";

std::unique_ptr<Location> Location::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Location> result(new Location());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* scriptIdValue = object->get("scriptId");
    errors->setName("scriptId");
    result->m_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* lineNumberValue = object->get("lineNumber");
    errors->setName("lineNumber");
    result->m_lineNumber = ValueConversions<int>::fromValue(lineNumberValue, errors);
    protocol::Value* columnNumberValue = object->get("columnNumber");
    if (columnNumberValue) {
        errors->setName("columnNumber");
        result->m_columnNumber = ValueConversions<int>::fromValue(columnNumberValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Location::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("scriptId", ValueConversions<String>::toValue(m_scriptId));
    result->setValue("lineNumber", ValueConversions<int>::toValue(m_lineNumber));
    if (m_columnNumber.isJust())
        result->setValue("columnNumber", ValueConversions<int>::toValue(m_columnNumber.fromJust()));
    return result;
}

std::unique_ptr<Location> Location::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScriptPosition> ScriptPosition::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScriptPosition> result(new ScriptPosition());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* lineNumberValue = object->get("lineNumber");
    errors->setName("lineNumber");
    result->m_lineNumber = ValueConversions<int>::fromValue(lineNumberValue, errors);
    protocol::Value* columnNumberValue = object->get("columnNumber");
    errors->setName("columnNumber");
    result->m_columnNumber = ValueConversions<int>::fromValue(columnNumberValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScriptPosition::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("lineNumber", ValueConversions<int>::toValue(m_lineNumber));
    result->setValue("columnNumber", ValueConversions<int>::toValue(m_columnNumber));
    return result;
}

std::unique_ptr<ScriptPosition> ScriptPosition::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CallFrame> CallFrame::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CallFrame> result(new CallFrame());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* callFrameIdValue = object->get("callFrameId");
    errors->setName("callFrameId");
    result->m_callFrameId = ValueConversions<String>::fromValue(callFrameIdValue, errors);
    protocol::Value* functionNameValue = object->get("functionName");
    errors->setName("functionName");
    result->m_functionName = ValueConversions<String>::fromValue(functionNameValue, errors);
    protocol::Value* functionLocationValue = object->get("functionLocation");
    if (functionLocationValue) {
        errors->setName("functionLocation");
        result->m_functionLocation = ValueConversions<protocol::Debugger::Location>::fromValue(functionLocationValue, errors);
    }
    protocol::Value* locationValue = object->get("location");
    errors->setName("location");
    result->m_location = ValueConversions<protocol::Debugger::Location>::fromValue(locationValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* scopeChainValue = object->get("scopeChain");
    errors->setName("scopeChain");
    result->m_scopeChain = ValueConversions<protocol::Array<protocol::Debugger::Scope>>::fromValue(scopeChainValue, errors);
    protocol::Value* thisValue = object->get("this");
    errors->setName("this");
    result->m_this = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(thisValue, errors);
    protocol::Value* returnValueValue = object->get("returnValue");
    if (returnValueValue) {
        errors->setName("returnValue");
        result->m_returnValue = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(returnValueValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CallFrame::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("callFrameId", ValueConversions<String>::toValue(m_callFrameId));
    result->setValue("functionName", ValueConversions<String>::toValue(m_functionName));
    if (m_functionLocation.isJust())
        result->setValue("functionLocation", ValueConversions<protocol::Debugger::Location>::toValue(m_functionLocation.fromJust()));
    result->setValue("location", ValueConversions<protocol::Debugger::Location>::toValue(m_location.get()));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("scopeChain", ValueConversions<protocol::Array<protocol::Debugger::Scope>>::toValue(m_scopeChain.get()));
    result->setValue("this", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_this.get()));
    if (m_returnValue.isJust())
        result->setValue("returnValue", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_returnValue.fromJust()));
    return result;
}

std::unique_ptr<CallFrame> CallFrame::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* Scope::TypeEnum::Global = "global";
const char* Scope::TypeEnum::Local = "local";
const char* Scope::TypeEnum::With = "with";
const char* Scope::TypeEnum::Closure = "closure";
const char* Scope::TypeEnum::Catch = "catch";
const char* Scope::TypeEnum::Block = "block";
const char* Scope::TypeEnum::Script = "script";
const char* Scope::TypeEnum::Eval = "eval";
const char* Scope::TypeEnum::Module = "module";

std::unique_ptr<Scope> Scope::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Scope> result(new Scope());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* objectValue = object->get("object");
    errors->setName("object");
    result->m_object = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(objectValue, errors);
    protocol::Value* nameValue = object->get("name");
    if (nameValue) {
        errors->setName("name");
        result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    }
    protocol::Value* startLocationValue = object->get("startLocation");
    if (startLocationValue) {
        errors->setName("startLocation");
        result->m_startLocation = ValueConversions<protocol::Debugger::Location>::fromValue(startLocationValue, errors);
    }
    protocol::Value* endLocationValue = object->get("endLocation");
    if (endLocationValue) {
        errors->setName("endLocation");
        result->m_endLocation = ValueConversions<protocol::Debugger::Location>::fromValue(endLocationValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Scope::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    result->setValue("object", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_object.get()));
    if (m_name.isJust())
        result->setValue("name", ValueConversions<String>::toValue(m_name.fromJust()));
    if (m_startLocation.isJust())
        result->setValue("startLocation", ValueConversions<protocol::Debugger::Location>::toValue(m_startLocation.fromJust()));
    if (m_endLocation.isJust())
        result->setValue("endLocation", ValueConversions<protocol::Debugger::Location>::toValue(m_endLocation.fromJust()));
    return result;
}

std::unique_ptr<Scope> Scope::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<SearchMatch> SearchMatch::fromValue(protocol::Value* value, ErrorSupport* errors)
{
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
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SearchMatch::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("lineNumber", ValueConversions<double>::toValue(m_lineNumber));
    result->setValue("lineContent", ValueConversions<String>::toValue(m_lineContent));
    return result;
}

std::unique_ptr<SearchMatch> SearchMatch::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StringBuffer> SearchMatch::toJSONString() const
{
    String json = toValue()->serialize();
    return StringBufferImpl::adopt(json);
}

// static
std::unique_ptr<API::SearchMatch> API::SearchMatch::fromJSONString(const StringView& json)
{
    ErrorSupport errors;
    std::unique_ptr<Value> value = StringUtil::parseJSON(json);
    if (!value)
        return nullptr;
    return protocol::Debugger::SearchMatch::fromValue(value.get(), &errors);
}

const char* BreakLocation::TypeEnum::DebuggerStatement = "debuggerStatement";
const char* BreakLocation::TypeEnum::Call = "call";
const char* BreakLocation::TypeEnum::Return = "return";

std::unique_ptr<BreakLocation> BreakLocation::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<BreakLocation> result(new BreakLocation());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* scriptIdValue = object->get("scriptId");
    errors->setName("scriptId");
    result->m_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* lineNumberValue = object->get("lineNumber");
    errors->setName("lineNumber");
    result->m_lineNumber = ValueConversions<int>::fromValue(lineNumberValue, errors);
    protocol::Value* columnNumberValue = object->get("columnNumber");
    if (columnNumberValue) {
        errors->setName("columnNumber");
        result->m_columnNumber = ValueConversions<int>::fromValue(columnNumberValue, errors);
    }
    protocol::Value* typeValue = object->get("type");
    if (typeValue) {
        errors->setName("type");
        result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> BreakLocation::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("scriptId", ValueConversions<String>::toValue(m_scriptId));
    result->setValue("lineNumber", ValueConversions<int>::toValue(m_lineNumber));
    if (m_columnNumber.isJust())
        result->setValue("columnNumber", ValueConversions<int>::toValue(m_columnNumber.fromJust()));
    if (m_type.isJust())
        result->setValue("type", ValueConversions<String>::toValue(m_type.fromJust()));
    return result;
}

std::unique_ptr<BreakLocation> BreakLocation::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<BreakpointResolvedNotification> BreakpointResolvedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<BreakpointResolvedNotification> result(new BreakpointResolvedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* breakpointIdValue = object->get("breakpointId");
    errors->setName("breakpointId");
    result->m_breakpointId = ValueConversions<String>::fromValue(breakpointIdValue, errors);
    protocol::Value* locationValue = object->get("location");
    errors->setName("location");
    result->m_location = ValueConversions<protocol::Debugger::Location>::fromValue(locationValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> BreakpointResolvedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("breakpointId", ValueConversions<String>::toValue(m_breakpointId));
    result->setValue("location", ValueConversions<protocol::Debugger::Location>::toValue(m_location.get()));
    return result;
}

std::unique_ptr<BreakpointResolvedNotification> BreakpointResolvedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* PausedNotification::ReasonEnum::XHR = "XHR";
const char* PausedNotification::ReasonEnum::DOM = "DOM";
const char* PausedNotification::ReasonEnum::EventListener = "EventListener";
const char* PausedNotification::ReasonEnum::Exception = "exception";
const char* PausedNotification::ReasonEnum::Assert = "assert";
const char* PausedNotification::ReasonEnum::DebugCommand = "debugCommand";
const char* PausedNotification::ReasonEnum::PromiseRejection = "promiseRejection";
const char* PausedNotification::ReasonEnum::OOM = "OOM";
const char* PausedNotification::ReasonEnum::Other = "other";
const char* PausedNotification::ReasonEnum::Ambiguous = "ambiguous";

std::unique_ptr<PausedNotification> PausedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PausedNotification> result(new PausedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* callFramesValue = object->get("callFrames");
    errors->setName("callFrames");
    result->m_callFrames = ValueConversions<protocol::Array<protocol::Debugger::CallFrame>>::fromValue(callFramesValue, errors);
    protocol::Value* reasonValue = object->get("reason");
    errors->setName("reason");
    result->m_reason = ValueConversions<String>::fromValue(reasonValue, errors);
    protocol::Value* dataValue = object->get("data");
    if (dataValue) {
        errors->setName("data");
        result->m_data = ValueConversions<protocol::DictionaryValue>::fromValue(dataValue, errors);
    }
    protocol::Value* hitBreakpointsValue = object->get("hitBreakpoints");
    if (hitBreakpointsValue) {
        errors->setName("hitBreakpoints");
        result->m_hitBreakpoints = ValueConversions<protocol::Array<String>>::fromValue(hitBreakpointsValue, errors);
    }
    protocol::Value* asyncStackTraceValue = object->get("asyncStackTrace");
    if (asyncStackTraceValue) {
        errors->setName("asyncStackTrace");
        result->m_asyncStackTrace = ValueConversions<protocol::Runtime::StackTrace>::fromValue(asyncStackTraceValue, errors);
    }
    protocol::Value* asyncStackTraceIdValue = object->get("asyncStackTraceId");
    if (asyncStackTraceIdValue) {
        errors->setName("asyncStackTraceId");
        result->m_asyncStackTraceId = ValueConversions<protocol::Runtime::StackTraceId>::fromValue(asyncStackTraceIdValue, errors);
    }
    protocol::Value* asyncCallStackTraceIdValue = object->get("asyncCallStackTraceId");
    if (asyncCallStackTraceIdValue) {
        errors->setName("asyncCallStackTraceId");
        result->m_asyncCallStackTraceId = ValueConversions<protocol::Runtime::StackTraceId>::fromValue(asyncCallStackTraceIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PausedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("callFrames", ValueConversions<protocol::Array<protocol::Debugger::CallFrame>>::toValue(m_callFrames.get()));
    result->setValue("reason", ValueConversions<String>::toValue(m_reason));
    if (m_data.isJust())
        result->setValue("data", ValueConversions<protocol::DictionaryValue>::toValue(m_data.fromJust()));
    if (m_hitBreakpoints.isJust())
        result->setValue("hitBreakpoints", ValueConversions<protocol::Array<String>>::toValue(m_hitBreakpoints.fromJust()));
    if (m_asyncStackTrace.isJust())
        result->setValue("asyncStackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_asyncStackTrace.fromJust()));
    if (m_asyncStackTraceId.isJust())
        result->setValue("asyncStackTraceId", ValueConversions<protocol::Runtime::StackTraceId>::toValue(m_asyncStackTraceId.fromJust()));
    if (m_asyncCallStackTraceId.isJust())
        result->setValue("asyncCallStackTraceId", ValueConversions<protocol::Runtime::StackTraceId>::toValue(m_asyncCallStackTraceId.fromJust()));
    return result;
}

std::unique_ptr<PausedNotification> PausedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScriptFailedToParseNotification> ScriptFailedToParseNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScriptFailedToParseNotification> result(new ScriptFailedToParseNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* scriptIdValue = object->get("scriptId");
    errors->setName("scriptId");
    result->m_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* startLineValue = object->get("startLine");
    errors->setName("startLine");
    result->m_startLine = ValueConversions<int>::fromValue(startLineValue, errors);
    protocol::Value* startColumnValue = object->get("startColumn");
    errors->setName("startColumn");
    result->m_startColumn = ValueConversions<int>::fromValue(startColumnValue, errors);
    protocol::Value* endLineValue = object->get("endLine");
    errors->setName("endLine");
    result->m_endLine = ValueConversions<int>::fromValue(endLineValue, errors);
    protocol::Value* endColumnValue = object->get("endColumn");
    errors->setName("endColumn");
    result->m_endColumn = ValueConversions<int>::fromValue(endColumnValue, errors);
    protocol::Value* executionContextIdValue = object->get("executionContextId");
    errors->setName("executionContextId");
    result->m_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    protocol::Value* hashValue = object->get("hash");
    errors->setName("hash");
    result->m_hash = ValueConversions<String>::fromValue(hashValue, errors);
    protocol::Value* executionContextAuxDataValue = object->get("executionContextAuxData");
    if (executionContextAuxDataValue) {
        errors->setName("executionContextAuxData");
        result->m_executionContextAuxData = ValueConversions<protocol::DictionaryValue>::fromValue(executionContextAuxDataValue, errors);
    }
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::fromValue(sourceMapURLValue, errors);
    }
    protocol::Value* hasSourceURLValue = object->get("hasSourceURL");
    if (hasSourceURLValue) {
        errors->setName("hasSourceURL");
        result->m_hasSourceURL = ValueConversions<bool>::fromValue(hasSourceURLValue, errors);
    }
    protocol::Value* isModuleValue = object->get("isModule");
    if (isModuleValue) {
        errors->setName("isModule");
        result->m_isModule = ValueConversions<bool>::fromValue(isModuleValue, errors);
    }
    protocol::Value* lengthValue = object->get("length");
    if (lengthValue) {
        errors->setName("length");
        result->m_length = ValueConversions<int>::fromValue(lengthValue, errors);
    }
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::fromValue(stackTraceValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScriptFailedToParseNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("scriptId", ValueConversions<String>::toValue(m_scriptId));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("startLine", ValueConversions<int>::toValue(m_startLine));
    result->setValue("startColumn", ValueConversions<int>::toValue(m_startColumn));
    result->setValue("endLine", ValueConversions<int>::toValue(m_endLine));
    result->setValue("endColumn", ValueConversions<int>::toValue(m_endColumn));
    result->setValue("executionContextId", ValueConversions<int>::toValue(m_executionContextId));
    result->setValue("hash", ValueConversions<String>::toValue(m_hash));
    if (m_executionContextAuxData.isJust())
        result->setValue("executionContextAuxData", ValueConversions<protocol::DictionaryValue>::toValue(m_executionContextAuxData.fromJust()));
    if (m_sourceMapURL.isJust())
        result->setValue("sourceMapURL", ValueConversions<String>::toValue(m_sourceMapURL.fromJust()));
    if (m_hasSourceURL.isJust())
        result->setValue("hasSourceURL", ValueConversions<bool>::toValue(m_hasSourceURL.fromJust()));
    if (m_isModule.isJust())
        result->setValue("isModule", ValueConversions<bool>::toValue(m_isModule.fromJust()));
    if (m_length.isJust())
        result->setValue("length", ValueConversions<int>::toValue(m_length.fromJust()));
    if (m_stackTrace.isJust())
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_stackTrace.fromJust()));
    return result;
}

std::unique_ptr<ScriptFailedToParseNotification> ScriptFailedToParseNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScriptParsedNotification> ScriptParsedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScriptParsedNotification> result(new ScriptParsedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* scriptIdValue = object->get("scriptId");
    errors->setName("scriptId");
    result->m_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* startLineValue = object->get("startLine");
    errors->setName("startLine");
    result->m_startLine = ValueConversions<int>::fromValue(startLineValue, errors);
    protocol::Value* startColumnValue = object->get("startColumn");
    errors->setName("startColumn");
    result->m_startColumn = ValueConversions<int>::fromValue(startColumnValue, errors);
    protocol::Value* endLineValue = object->get("endLine");
    errors->setName("endLine");
    result->m_endLine = ValueConversions<int>::fromValue(endLineValue, errors);
    protocol::Value* endColumnValue = object->get("endColumn");
    errors->setName("endColumn");
    result->m_endColumn = ValueConversions<int>::fromValue(endColumnValue, errors);
    protocol::Value* executionContextIdValue = object->get("executionContextId");
    errors->setName("executionContextId");
    result->m_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    protocol::Value* hashValue = object->get("hash");
    errors->setName("hash");
    result->m_hash = ValueConversions<String>::fromValue(hashValue, errors);
    protocol::Value* executionContextAuxDataValue = object->get("executionContextAuxData");
    if (executionContextAuxDataValue) {
        errors->setName("executionContextAuxData");
        result->m_executionContextAuxData = ValueConversions<protocol::DictionaryValue>::fromValue(executionContextAuxDataValue, errors);
    }
    protocol::Value* isLiveEditValue = object->get("isLiveEdit");
    if (isLiveEditValue) {
        errors->setName("isLiveEdit");
        result->m_isLiveEdit = ValueConversions<bool>::fromValue(isLiveEditValue, errors);
    }
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::fromValue(sourceMapURLValue, errors);
    }
    protocol::Value* hasSourceURLValue = object->get("hasSourceURL");
    if (hasSourceURLValue) {
        errors->setName("hasSourceURL");
        result->m_hasSourceURL = ValueConversions<bool>::fromValue(hasSourceURLValue, errors);
    }
    protocol::Value* isModuleValue = object->get("isModule");
    if (isModuleValue) {
        errors->setName("isModule");
        result->m_isModule = ValueConversions<bool>::fromValue(isModuleValue, errors);
    }
    protocol::Value* lengthValue = object->get("length");
    if (lengthValue) {
        errors->setName("length");
        result->m_length = ValueConversions<int>::fromValue(lengthValue, errors);
    }
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::fromValue(stackTraceValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScriptParsedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("scriptId", ValueConversions<String>::toValue(m_scriptId));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("startLine", ValueConversions<int>::toValue(m_startLine));
    result->setValue("startColumn", ValueConversions<int>::toValue(m_startColumn));
    result->setValue("endLine", ValueConversions<int>::toValue(m_endLine));
    result->setValue("endColumn", ValueConversions<int>::toValue(m_endColumn));
    result->setValue("executionContextId", ValueConversions<int>::toValue(m_executionContextId));
    result->setValue("hash", ValueConversions<String>::toValue(m_hash));
    if (m_executionContextAuxData.isJust())
        result->setValue("executionContextAuxData", ValueConversions<protocol::DictionaryValue>::toValue(m_executionContextAuxData.fromJust()));
    if (m_isLiveEdit.isJust())
        result->setValue("isLiveEdit", ValueConversions<bool>::toValue(m_isLiveEdit.fromJust()));
    if (m_sourceMapURL.isJust())
        result->setValue("sourceMapURL", ValueConversions<String>::toValue(m_sourceMapURL.fromJust()));
    if (m_hasSourceURL.isJust())
        result->setValue("hasSourceURL", ValueConversions<bool>::toValue(m_hasSourceURL.fromJust()));
    if (m_isModule.isJust())
        result->setValue("isModule", ValueConversions<bool>::toValue(m_isModule.fromJust()));
    if (m_length.isJust())
        result->setValue("length", ValueConversions<int>::toValue(m_length.fromJust()));
    if (m_stackTrace.isJust())
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_stackTrace.fromJust()));
    return result;
}

std::unique_ptr<ScriptParsedNotification> ScriptParsedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


namespace ContinueToLocation {
namespace TargetCallFramesEnum {
const char* Any = "any";
const char* Current = "current";
} // namespace TargetCallFramesEnum
} // namespace ContinueToLocation

namespace SetPauseOnExceptions {
namespace StateEnum {
const char* None = "none";
const char* Uncaught = "uncaught";
const char* All = "all";
} // namespace StateEnum
} // namespace SetPauseOnExceptions

namespace Paused {
namespace ReasonEnum {
const char* XHR = "XHR";
const char* DOM = "DOM";
const char* EventListener = "EventListener";
const char* Exception = "exception";
const char* Assert = "assert";
const char* DebugCommand = "debugCommand";
const char* PromiseRejection = "promiseRejection";
const char* OOM = "OOM";
const char* Other = "other";
const char* Ambiguous = "ambiguous";
} // namespace ReasonEnum
} // namespace Paused

namespace API {
namespace Paused {
namespace ReasonEnum {
const char* XHR = "XHR";
const char* DOM = "DOM";
const char* EventListener = "EventListener";
const char* Exception = "exception";
const char* Assert = "assert";
const char* DebugCommand = "debugCommand";
const char* PromiseRejection = "promiseRejection";
const char* OOM = "OOM";
const char* Other = "other";
const char* Ambiguous = "ambiguous";
} // namespace ReasonEnum
} // namespace Paused
} // namespace API

// ------------- Frontend notifications.

void Frontend::breakpointResolved(const String& breakpointId, std::unique_ptr<protocol::Debugger::Location> location)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<BreakpointResolvedNotification> messageData = BreakpointResolvedNotification::create()
        .setBreakpointId(breakpointId)
        .setLocation(std::move(location))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Debugger.breakpointResolved", std::move(messageData)));
}

void Frontend::paused(std::unique_ptr<protocol::Array<protocol::Debugger::CallFrame>> callFrames, const String& reason, Maybe<protocol::DictionaryValue> data, Maybe<protocol::Array<String>> hitBreakpoints, Maybe<protocol::Runtime::StackTrace> asyncStackTrace, Maybe<protocol::Runtime::StackTraceId> asyncStackTraceId, Maybe<protocol::Runtime::StackTraceId> asyncCallStackTraceId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<PausedNotification> messageData = PausedNotification::create()
        .setCallFrames(std::move(callFrames))
        .setReason(reason)
        .build();
    if (data.isJust())
        messageData->setData(std::move(data).takeJust());
    if (hitBreakpoints.isJust())
        messageData->setHitBreakpoints(std::move(hitBreakpoints).takeJust());
    if (asyncStackTrace.isJust())
        messageData->setAsyncStackTrace(std::move(asyncStackTrace).takeJust());
    if (asyncStackTraceId.isJust())
        messageData->setAsyncStackTraceId(std::move(asyncStackTraceId).takeJust());
    if (asyncCallStackTraceId.isJust())
        messageData->setAsyncCallStackTraceId(std::move(asyncCallStackTraceId).takeJust());
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Debugger.paused", std::move(messageData)));
}

void Frontend::resumed()
{
    if (!m_frontendChannel)
        return;
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Debugger.resumed"));
}

void Frontend::scriptFailedToParse(const String& scriptId, const String& url, int startLine, int startColumn, int endLine, int endColumn, int executionContextId, const String& hash, Maybe<protocol::DictionaryValue> executionContextAuxData, Maybe<String> sourceMapURL, Maybe<bool> hasSourceURL, Maybe<bool> isModule, Maybe<int> length, Maybe<protocol::Runtime::StackTrace> stackTrace)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ScriptFailedToParseNotification> messageData = ScriptFailedToParseNotification::create()
        .setScriptId(scriptId)
        .setUrl(url)
        .setStartLine(startLine)
        .setStartColumn(startColumn)
        .setEndLine(endLine)
        .setEndColumn(endColumn)
        .setExecutionContextId(executionContextId)
        .setHash(hash)
        .build();
    if (executionContextAuxData.isJust())
        messageData->setExecutionContextAuxData(std::move(executionContextAuxData).takeJust());
    if (sourceMapURL.isJust())
        messageData->setSourceMapURL(std::move(sourceMapURL).takeJust());
    if (hasSourceURL.isJust())
        messageData->setHasSourceURL(std::move(hasSourceURL).takeJust());
    if (isModule.isJust())
        messageData->setIsModule(std::move(isModule).takeJust());
    if (length.isJust())
        messageData->setLength(std::move(length).takeJust());
    if (stackTrace.isJust())
        messageData->setStackTrace(std::move(stackTrace).takeJust());
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Debugger.scriptFailedToParse", std::move(messageData)));
}

void Frontend::scriptParsed(const String& scriptId, const String& url, int startLine, int startColumn, int endLine, int endColumn, int executionContextId, const String& hash, Maybe<protocol::DictionaryValue> executionContextAuxData, Maybe<bool> isLiveEdit, Maybe<String> sourceMapURL, Maybe<bool> hasSourceURL, Maybe<bool> isModule, Maybe<int> length, Maybe<protocol::Runtime::StackTrace> stackTrace)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ScriptParsedNotification> messageData = ScriptParsedNotification::create()
        .setScriptId(scriptId)
        .setUrl(url)
        .setStartLine(startLine)
        .setStartColumn(startColumn)
        .setEndLine(endLine)
        .setEndColumn(endColumn)
        .setExecutionContextId(executionContextId)
        .setHash(hash)
        .build();
    if (executionContextAuxData.isJust())
        messageData->setExecutionContextAuxData(std::move(executionContextAuxData).takeJust());
    if (isLiveEdit.isJust())
        messageData->setIsLiveEdit(std::move(isLiveEdit).takeJust());
    if (sourceMapURL.isJust())
        messageData->setSourceMapURL(std::move(sourceMapURL).takeJust());
    if (hasSourceURL.isJust())
        messageData->setHasSourceURL(std::move(hasSourceURL).takeJust());
    if (isModule.isJust())
        messageData->setIsModule(std::move(isModule).takeJust());
    if (length.isJust())
        messageData->setLength(std::move(length).takeJust());
    if (stackTrace.isJust())
        messageData->setStackTrace(std::move(stackTrace).takeJust());
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Debugger.scriptParsed", std::move(messageData)));
}

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
        m_dispatchMap["Debugger.continueToLocation"] = &DispatcherImpl::continueToLocation;
        m_dispatchMap["Debugger.disable"] = &DispatcherImpl::disable;
        m_dispatchMap["Debugger.enable"] = &DispatcherImpl::enable;
        m_dispatchMap["Debugger.evaluateOnCallFrame"] = &DispatcherImpl::evaluateOnCallFrame;
        m_dispatchMap["Debugger.getPossibleBreakpoints"] = &DispatcherImpl::getPossibleBreakpoints;
        m_dispatchMap["Debugger.getScriptSource"] = &DispatcherImpl::getScriptSource;
        m_dispatchMap["Debugger.getStackTrace"] = &DispatcherImpl::getStackTrace;
        m_dispatchMap["Debugger.pause"] = &DispatcherImpl::pause;
        m_dispatchMap["Debugger.pauseOnAsyncCall"] = &DispatcherImpl::pauseOnAsyncCall;
        m_dispatchMap["Debugger.removeBreakpoint"] = &DispatcherImpl::removeBreakpoint;
        m_dispatchMap["Debugger.restartFrame"] = &DispatcherImpl::restartFrame;
        m_dispatchMap["Debugger.resume"] = &DispatcherImpl::resume;
        m_dispatchMap["Debugger.scheduleStepIntoAsync"] = &DispatcherImpl::scheduleStepIntoAsync;
        m_dispatchMap["Debugger.searchInContent"] = &DispatcherImpl::searchInContent;
        m_dispatchMap["Debugger.setAsyncCallStackDepth"] = &DispatcherImpl::setAsyncCallStackDepth;
        m_dispatchMap["Debugger.setBlackboxPatterns"] = &DispatcherImpl::setBlackboxPatterns;
        m_dispatchMap["Debugger.setBlackboxedRanges"] = &DispatcherImpl::setBlackboxedRanges;
        m_dispatchMap["Debugger.setBreakpoint"] = &DispatcherImpl::setBreakpoint;
        m_dispatchMap["Debugger.setBreakpointByUrl"] = &DispatcherImpl::setBreakpointByUrl;
        m_dispatchMap["Debugger.setBreakpointsActive"] = &DispatcherImpl::setBreakpointsActive;
        m_dispatchMap["Debugger.setPauseOnExceptions"] = &DispatcherImpl::setPauseOnExceptions;
        m_dispatchMap["Debugger.setReturnValue"] = &DispatcherImpl::setReturnValue;
        m_dispatchMap["Debugger.setScriptSource"] = &DispatcherImpl::setScriptSource;
        m_dispatchMap["Debugger.setSkipAllPauses"] = &DispatcherImpl::setSkipAllPauses;
        m_dispatchMap["Debugger.setVariableValue"] = &DispatcherImpl::setVariableValue;
        m_dispatchMap["Debugger.stepInto"] = &DispatcherImpl::stepInto;
        m_dispatchMap["Debugger.stepOut"] = &DispatcherImpl::stepOut;
        m_dispatchMap["Debugger.stepOver"] = &DispatcherImpl::stepOver;
    }
    ~DispatcherImpl() override { }
    DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
    HashMap<String, String>& redirects() { return m_redirects; }

protected:
    using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String, CallHandler>;
    DispatchMap m_dispatchMap;
    HashMap<String, String> m_redirects;

    DispatchResponse::Status continueToLocation(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status evaluateOnCallFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getPossibleBreakpoints(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getScriptSource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getStackTrace(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status pause(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status pauseOnAsyncCall(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status removeBreakpoint(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status restartFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status resume(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status scheduleStepIntoAsync(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status searchInContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setAsyncCallStackDepth(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setBlackboxPatterns(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setBlackboxedRanges(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setBreakpoint(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setBreakpointByUrl(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setBreakpointsActive(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setPauseOnExceptions(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setReturnValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setScriptSource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setSkipAllPauses(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setVariableValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status stepInto(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status stepOut(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status stepOver(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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


DispatchResponse::Status DispatcherImpl::continueToLocation(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* locationValue = object ? object->get("location") : nullptr;
    errors->setName("location");
    std::unique_ptr<protocol::Debugger::Location> in_location = ValueConversions<protocol::Debugger::Location>::fromValue(locationValue, errors);
    protocol::Value* targetCallFramesValue = object ? object->get("targetCallFrames") : nullptr;
    Maybe<String> in_targetCallFrames;
    if (targetCallFramesValue) {
        errors->setName("targetCallFrames");
        in_targetCallFrames = ValueConversions<String>::fromValue(targetCallFramesValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->continueToLocation(std::move(in_location), std::move(in_targetCallFrames));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Declare output parameters.
    String out_debuggerId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable(&out_debuggerId);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("debuggerId", ValueConversions<String>::toValue(out_debuggerId));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::evaluateOnCallFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* callFrameIdValue = object ? object->get("callFrameId") : nullptr;
    errors->setName("callFrameId");
    String in_callFrameId = ValueConversions<String>::fromValue(callFrameIdValue, errors);
    protocol::Value* expressionValue = object ? object->get("expression") : nullptr;
    errors->setName("expression");
    String in_expression = ValueConversions<String>::fromValue(expressionValue, errors);
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    Maybe<String> in_objectGroup;
    if (objectGroupValue) {
        errors->setName("objectGroup");
        in_objectGroup = ValueConversions<String>::fromValue(objectGroupValue, errors);
    }
    protocol::Value* includeCommandLineAPIValue = object ? object->get("includeCommandLineAPI") : nullptr;
    Maybe<bool> in_includeCommandLineAPI;
    if (includeCommandLineAPIValue) {
        errors->setName("includeCommandLineAPI");
        in_includeCommandLineAPI = ValueConversions<bool>::fromValue(includeCommandLineAPIValue, errors);
    }
    protocol::Value* silentValue = object ? object->get("silent") : nullptr;
    Maybe<bool> in_silent;
    if (silentValue) {
        errors->setName("silent");
        in_silent = ValueConversions<bool>::fromValue(silentValue, errors);
    }
    protocol::Value* returnByValueValue = object ? object->get("returnByValue") : nullptr;
    Maybe<bool> in_returnByValue;
    if (returnByValueValue) {
        errors->setName("returnByValue");
        in_returnByValue = ValueConversions<bool>::fromValue(returnByValueValue, errors);
    }
    protocol::Value* generatePreviewValue = object ? object->get("generatePreview") : nullptr;
    Maybe<bool> in_generatePreview;
    if (generatePreviewValue) {
        errors->setName("generatePreview");
        in_generatePreview = ValueConversions<bool>::fromValue(generatePreviewValue, errors);
    }
    protocol::Value* throwOnSideEffectValue = object ? object->get("throwOnSideEffect") : nullptr;
    Maybe<bool> in_throwOnSideEffect;
    if (throwOnSideEffectValue) {
        errors->setName("throwOnSideEffect");
        in_throwOnSideEffect = ValueConversions<bool>::fromValue(throwOnSideEffectValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Runtime::RemoteObject> out_result;
    Maybe<protocol::Runtime::ExceptionDetails> out_exceptionDetails;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->evaluateOnCallFrame(in_callFrameId, in_expression, std::move(in_objectGroup), std::move(in_includeCommandLineAPI), std::move(in_silent), std::move(in_returnByValue), std::move(in_generatePreview), std::move(in_throwOnSideEffect), &out_result, &out_exceptionDetails);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Runtime::RemoteObject>::toValue(out_result.get()));
        if (out_exceptionDetails.isJust())
            result->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(out_exceptionDetails.fromJust()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getPossibleBreakpoints(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* startValue = object ? object->get("start") : nullptr;
    errors->setName("start");
    std::unique_ptr<protocol::Debugger::Location> in_start = ValueConversions<protocol::Debugger::Location>::fromValue(startValue, errors);
    protocol::Value* endValue = object ? object->get("end") : nullptr;
    Maybe<protocol::Debugger::Location> in_end;
    if (endValue) {
        errors->setName("end");
        in_end = ValueConversions<protocol::Debugger::Location>::fromValue(endValue, errors);
    }
    protocol::Value* restrictToFunctionValue = object ? object->get("restrictToFunction") : nullptr;
    Maybe<bool> in_restrictToFunction;
    if (restrictToFunctionValue) {
        errors->setName("restrictToFunction");
        in_restrictToFunction = ValueConversions<bool>::fromValue(restrictToFunctionValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Debugger::BreakLocation>> out_locations;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getPossibleBreakpoints(std::move(in_start), std::move(in_end), std::move(in_restrictToFunction), &out_locations);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("locations", ValueConversions<protocol::Array<protocol::Debugger::BreakLocation>>::toValue(out_locations.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getScriptSource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptIdValue = object ? object->get("scriptId") : nullptr;
    errors->setName("scriptId");
    String in_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_scriptSource;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getScriptSource(in_scriptId, &out_scriptSource);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("scriptSource", ValueConversions<String>::toValue(out_scriptSource));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getStackTrace(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* stackTraceIdValue = object ? object->get("stackTraceId") : nullptr;
    errors->setName("stackTraceId");
    std::unique_ptr<protocol::Runtime::StackTraceId> in_stackTraceId = ValueConversions<protocol::Runtime::StackTraceId>::fromValue(stackTraceIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Runtime::StackTrace> out_stackTrace;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getStackTrace(std::move(in_stackTraceId), &out_stackTrace);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(out_stackTrace.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::pause(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->pause();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::pauseOnAsyncCall(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* parentStackTraceIdValue = object ? object->get("parentStackTraceId") : nullptr;
    errors->setName("parentStackTraceId");
    std::unique_ptr<protocol::Runtime::StackTraceId> in_parentStackTraceId = ValueConversions<protocol::Runtime::StackTraceId>::fromValue(parentStackTraceIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->pauseOnAsyncCall(std::move(in_parentStackTraceId));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::removeBreakpoint(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* breakpointIdValue = object ? object->get("breakpointId") : nullptr;
    errors->setName("breakpointId");
    String in_breakpointId = ValueConversions<String>::fromValue(breakpointIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->removeBreakpoint(in_breakpointId);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::restartFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* callFrameIdValue = object ? object->get("callFrameId") : nullptr;
    errors->setName("callFrameId");
    String in_callFrameId = ValueConversions<String>::fromValue(callFrameIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Debugger::CallFrame>> out_callFrames;
    Maybe<protocol::Runtime::StackTrace> out_asyncStackTrace;
    Maybe<protocol::Runtime::StackTraceId> out_asyncStackTraceId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->restartFrame(in_callFrameId, &out_callFrames, &out_asyncStackTrace, &out_asyncStackTraceId);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("callFrames", ValueConversions<protocol::Array<protocol::Debugger::CallFrame>>::toValue(out_callFrames.get()));
        if (out_asyncStackTrace.isJust())
            result->setValue("asyncStackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(out_asyncStackTrace.fromJust()));
        if (out_asyncStackTraceId.isJust())
            result->setValue("asyncStackTraceId", ValueConversions<protocol::Runtime::StackTraceId>::toValue(out_asyncStackTraceId.fromJust()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::resume(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->resume();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

class ScheduleStepIntoAsyncCallbackImpl : public Backend::ScheduleStepIntoAsyncCallback, public DispatcherBase::Callback {
public:
    ScheduleStepIntoAsyncCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, int callbackId)
        : DispatcherBase::Callback(std::move(backendImpl), callId, callbackId) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

DispatchResponse::Status DispatcherImpl::scheduleStepIntoAsync(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<ScheduleStepIntoAsyncCallbackImpl> callback(new ScheduleStepIntoAsyncCallbackImpl(weakPtr(), callId, nextCallbackId()));
    m_backend->scheduleStepIntoAsync(std::move(callback));
    return (weak->get() && weak->get()->lastCallbackFallThrough()) ? DispatchResponse::kFallThrough : DispatchResponse::kAsync;
}

DispatchResponse::Status DispatcherImpl::searchInContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptIdValue = object ? object->get("scriptId") : nullptr;
    errors->setName("scriptId");
    String in_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* queryValue = object ? object->get("query") : nullptr;
    errors->setName("query");
    String in_query = ValueConversions<String>::fromValue(queryValue, errors);
    protocol::Value* caseSensitiveValue = object ? object->get("caseSensitive") : nullptr;
    Maybe<bool> in_caseSensitive;
    if (caseSensitiveValue) {
        errors->setName("caseSensitive");
        in_caseSensitive = ValueConversions<bool>::fromValue(caseSensitiveValue, errors);
    }
    protocol::Value* isRegexValue = object ? object->get("isRegex") : nullptr;
    Maybe<bool> in_isRegex;
    if (isRegexValue) {
        errors->setName("isRegex");
        in_isRegex = ValueConversions<bool>::fromValue(isRegexValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->searchInContent(in_scriptId, in_query, std::move(in_caseSensitive), std::move(in_isRegex), &out_result);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::Debugger::SearchMatch>>::toValue(out_result.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setAsyncCallStackDepth(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* maxDepthValue = object ? object->get("maxDepth") : nullptr;
    errors->setName("maxDepth");
    int in_maxDepth = ValueConversions<int>::fromValue(maxDepthValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setAsyncCallStackDepth(in_maxDepth);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setBlackboxPatterns(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* patternsValue = object ? object->get("patterns") : nullptr;
    errors->setName("patterns");
    std::unique_ptr<protocol::Array<String>> in_patterns = ValueConversions<protocol::Array<String>>::fromValue(patternsValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setBlackboxPatterns(std::move(in_patterns));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setBlackboxedRanges(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptIdValue = object ? object->get("scriptId") : nullptr;
    errors->setName("scriptId");
    String in_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* positionsValue = object ? object->get("positions") : nullptr;
    errors->setName("positions");
    std::unique_ptr<protocol::Array<protocol::Debugger::ScriptPosition>> in_positions = ValueConversions<protocol::Array<protocol::Debugger::ScriptPosition>>::fromValue(positionsValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setBlackboxedRanges(in_scriptId, std::move(in_positions));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setBreakpoint(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* locationValue = object ? object->get("location") : nullptr;
    errors->setName("location");
    std::unique_ptr<protocol::Debugger::Location> in_location = ValueConversions<protocol::Debugger::Location>::fromValue(locationValue, errors);
    protocol::Value* conditionValue = object ? object->get("condition") : nullptr;
    Maybe<String> in_condition;
    if (conditionValue) {
        errors->setName("condition");
        in_condition = ValueConversions<String>::fromValue(conditionValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_breakpointId;
    std::unique_ptr<protocol::Debugger::Location> out_actualLocation;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setBreakpoint(std::move(in_location), std::move(in_condition), &out_breakpointId, &out_actualLocation);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("breakpointId", ValueConversions<String>::toValue(out_breakpointId));
        result->setValue("actualLocation", ValueConversions<protocol::Debugger::Location>::toValue(out_actualLocation.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setBreakpointByUrl(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* lineNumberValue = object ? object->get("lineNumber") : nullptr;
    errors->setName("lineNumber");
    int in_lineNumber = ValueConversions<int>::fromValue(lineNumberValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    Maybe<String> in_url;
    if (urlValue) {
        errors->setName("url");
        in_url = ValueConversions<String>::fromValue(urlValue, errors);
    }
    protocol::Value* urlRegexValue = object ? object->get("urlRegex") : nullptr;
    Maybe<String> in_urlRegex;
    if (urlRegexValue) {
        errors->setName("urlRegex");
        in_urlRegex = ValueConversions<String>::fromValue(urlRegexValue, errors);
    }
    protocol::Value* scriptHashValue = object ? object->get("scriptHash") : nullptr;
    Maybe<String> in_scriptHash;
    if (scriptHashValue) {
        errors->setName("scriptHash");
        in_scriptHash = ValueConversions<String>::fromValue(scriptHashValue, errors);
    }
    protocol::Value* columnNumberValue = object ? object->get("columnNumber") : nullptr;
    Maybe<int> in_columnNumber;
    if (columnNumberValue) {
        errors->setName("columnNumber");
        in_columnNumber = ValueConversions<int>::fromValue(columnNumberValue, errors);
    }
    protocol::Value* conditionValue = object ? object->get("condition") : nullptr;
    Maybe<String> in_condition;
    if (conditionValue) {
        errors->setName("condition");
        in_condition = ValueConversions<String>::fromValue(conditionValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_breakpointId;
    std::unique_ptr<protocol::Array<protocol::Debugger::Location>> out_locations;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setBreakpointByUrl(in_lineNumber, std::move(in_url), std::move(in_urlRegex), std::move(in_scriptHash), std::move(in_columnNumber), std::move(in_condition), &out_breakpointId, &out_locations);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("breakpointId", ValueConversions<String>::toValue(out_breakpointId));
        result->setValue("locations", ValueConversions<protocol::Array<protocol::Debugger::Location>>::toValue(out_locations.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setBreakpointsActive(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* activeValue = object ? object->get("active") : nullptr;
    errors->setName("active");
    bool in_active = ValueConversions<bool>::fromValue(activeValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setBreakpointsActive(in_active);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setPauseOnExceptions(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* stateValue = object ? object->get("state") : nullptr;
    errors->setName("state");
    String in_state = ValueConversions<String>::fromValue(stateValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setPauseOnExceptions(in_state);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setReturnValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* newValueValue = object ? object->get("newValue") : nullptr;
    errors->setName("newValue");
    std::unique_ptr<protocol::Runtime::CallArgument> in_newValue = ValueConversions<protocol::Runtime::CallArgument>::fromValue(newValueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setReturnValue(std::move(in_newValue));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setScriptSource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptIdValue = object ? object->get("scriptId") : nullptr;
    errors->setName("scriptId");
    String in_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* scriptSourceValue = object ? object->get("scriptSource") : nullptr;
    errors->setName("scriptSource");
    String in_scriptSource = ValueConversions<String>::fromValue(scriptSourceValue, errors);
    protocol::Value* dryRunValue = object ? object->get("dryRun") : nullptr;
    Maybe<bool> in_dryRun;
    if (dryRunValue) {
        errors->setName("dryRun");
        in_dryRun = ValueConversions<bool>::fromValue(dryRunValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    Maybe<protocol::Array<protocol::Debugger::CallFrame>> out_callFrames;
    Maybe<bool> out_stackChanged;
    Maybe<protocol::Runtime::StackTrace> out_asyncStackTrace;
    Maybe<protocol::Runtime::StackTraceId> out_asyncStackTraceId;
    Maybe<protocol::Runtime::ExceptionDetails> out_exceptionDetails;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setScriptSource(in_scriptId, in_scriptSource, std::move(in_dryRun), &out_callFrames, &out_stackChanged, &out_asyncStackTrace, &out_asyncStackTraceId, &out_exceptionDetails);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        if (out_callFrames.isJust())
            result->setValue("callFrames", ValueConversions<protocol::Array<protocol::Debugger::CallFrame>>::toValue(out_callFrames.fromJust()));
        if (out_stackChanged.isJust())
            result->setValue("stackChanged", ValueConversions<bool>::toValue(out_stackChanged.fromJust()));
        if (out_asyncStackTrace.isJust())
            result->setValue("asyncStackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(out_asyncStackTrace.fromJust()));
        if (out_asyncStackTraceId.isJust())
            result->setValue("asyncStackTraceId", ValueConversions<protocol::Runtime::StackTraceId>::toValue(out_asyncStackTraceId.fromJust()));
        if (out_exceptionDetails.isJust())
            result->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(out_exceptionDetails.fromJust()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setSkipAllPauses(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* skipValue = object ? object->get("skip") : nullptr;
    errors->setName("skip");
    bool in_skip = ValueConversions<bool>::fromValue(skipValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setSkipAllPauses(in_skip);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setVariableValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scopeNumberValue = object ? object->get("scopeNumber") : nullptr;
    errors->setName("scopeNumber");
    int in_scopeNumber = ValueConversions<int>::fromValue(scopeNumberValue, errors);
    protocol::Value* variableNameValue = object ? object->get("variableName") : nullptr;
    errors->setName("variableName");
    String in_variableName = ValueConversions<String>::fromValue(variableNameValue, errors);
    protocol::Value* newValueValue = object ? object->get("newValue") : nullptr;
    errors->setName("newValue");
    std::unique_ptr<protocol::Runtime::CallArgument> in_newValue = ValueConversions<protocol::Runtime::CallArgument>::fromValue(newValueValue, errors);
    protocol::Value* callFrameIdValue = object ? object->get("callFrameId") : nullptr;
    errors->setName("callFrameId");
    String in_callFrameId = ValueConversions<String>::fromValue(callFrameIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setVariableValue(in_scopeNumber, in_variableName, std::move(in_newValue), in_callFrameId);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::stepInto(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* breakOnAsyncCallValue = object ? object->get("breakOnAsyncCall") : nullptr;
    Maybe<bool> in_breakOnAsyncCall;
    if (breakOnAsyncCallValue) {
        errors->setName("breakOnAsyncCall");
        in_breakOnAsyncCall = ValueConversions<bool>::fromValue(breakOnAsyncCallValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stepInto(std::move(in_breakOnAsyncCall));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::stepOut(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stepOut();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::stepOver(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stepOver();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
{
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Debugger", std::move(dispatcher));
}

} // Debugger
} // namespace v8_inspector
} // namespace protocol

// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Network.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Network {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Network";
const char Metainfo::commandPrefix[] = "Network.";
const char Metainfo::version[] = "1.2";

std::unique_ptr<ResourceTiming> ResourceTiming::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ResourceTiming> result(new ResourceTiming());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* startTimeValue = object->get("startTime");
    errors->setName("startTime");
    result->m_startTime = ValueConversions<double>::parse(startTimeValue, errors);
    protocol::Value* domainLookupStartValue = object->get("domainLookupStart");
    errors->setName("domainLookupStart");
    result->m_domainLookupStart = ValueConversions<double>::parse(domainLookupStartValue, errors);
    protocol::Value* domainLookupEndValue = object->get("domainLookupEnd");
    errors->setName("domainLookupEnd");
    result->m_domainLookupEnd = ValueConversions<double>::parse(domainLookupEndValue, errors);
    protocol::Value* connectStartValue = object->get("connectStart");
    errors->setName("connectStart");
    result->m_connectStart = ValueConversions<double>::parse(connectStartValue, errors);
    protocol::Value* connectEndValue = object->get("connectEnd");
    errors->setName("connectEnd");
    result->m_connectEnd = ValueConversions<double>::parse(connectEndValue, errors);
    protocol::Value* secureConnectionStartValue = object->get("secureConnectionStart");
    errors->setName("secureConnectionStart");
    result->m_secureConnectionStart = ValueConversions<double>::parse(secureConnectionStartValue, errors);
    protocol::Value* requestStartValue = object->get("requestStart");
    errors->setName("requestStart");
    result->m_requestStart = ValueConversions<double>::parse(requestStartValue, errors);
    protocol::Value* responseStartValue = object->get("responseStart");
    errors->setName("responseStart");
    result->m_responseStart = ValueConversions<double>::parse(responseStartValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ResourceTiming::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("startTime", ValueConversions<double>::serialize(m_startTime));
    result->setValue("domainLookupStart", ValueConversions<double>::serialize(m_domainLookupStart));
    result->setValue("domainLookupEnd", ValueConversions<double>::serialize(m_domainLookupEnd));
    result->setValue("connectStart", ValueConversions<double>::serialize(m_connectStart));
    result->setValue("connectEnd", ValueConversions<double>::serialize(m_connectEnd));
    result->setValue("secureConnectionStart", ValueConversions<double>::serialize(m_secureConnectionStart));
    result->setValue("requestStart", ValueConversions<double>::serialize(m_requestStart));
    result->setValue("responseStart", ValueConversions<double>::serialize(m_responseStart));
    return result;
}

std::unique_ptr<ResourceTiming> ResourceTiming::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<Request> Request::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Request> result(new Request());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::parse(urlValue, errors);
    protocol::Value* methodValue = object->get("method");
    errors->setName("method");
    result->m_method = ValueConversions<String>::parse(methodValue, errors);
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::parse(headersValue, errors);
    protocol::Value* postDataValue = object->get("postData");
    if (postDataValue) {
        errors->setName("postData");
        result->m_postData = ValueConversions<String>::parse(postDataValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Request::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::serialize(m_url));
    result->setValue("method", ValueConversions<String>::serialize(m_method));
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::serialize(m_headers.get()));
    if (m_postData.isJust()) {
        result->setValue("postData", ValueConversions<String>::serialize(m_postData.fromJust()));
    }
    return result;
}

std::unique_ptr<Request> Request::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<Response> Response::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Response> result(new Response());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::parse(urlValue, errors);
    protocol::Value* statusValue = object->get("status");
    errors->setName("status");
    result->m_status = ValueConversions<double>::parse(statusValue, errors);
    protocol::Value* statusTextValue = object->get("statusText");
    errors->setName("statusText");
    result->m_statusText = ValueConversions<String>::parse(statusTextValue, errors);
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::parse(headersValue, errors);
    protocol::Value* headersTextValue = object->get("headersText");
    if (headersTextValue) {
        errors->setName("headersText");
        result->m_headersText = ValueConversions<String>::parse(headersTextValue, errors);
    }
    protocol::Value* mimeTypeValue = object->get("mimeType");
    errors->setName("mimeType");
    result->m_mimeType = ValueConversions<String>::parse(mimeTypeValue, errors);
    protocol::Value* requestHeadersValue = object->get("requestHeaders");
    if (requestHeadersValue) {
        errors->setName("requestHeaders");
        result->m_requestHeaders = ValueConversions<protocol::Network::Headers>::parse(requestHeadersValue, errors);
    }
    protocol::Value* requestHeadersTextValue = object->get("requestHeadersText");
    if (requestHeadersTextValue) {
        errors->setName("requestHeadersText");
        result->m_requestHeadersText = ValueConversions<String>::parse(requestHeadersTextValue, errors);
    }
    protocol::Value* fromDiskCacheValue = object->get("fromDiskCache");
    if (fromDiskCacheValue) {
        errors->setName("fromDiskCache");
        result->m_fromDiskCache = ValueConversions<bool>::parse(fromDiskCacheValue, errors);
    }
    protocol::Value* timingValue = object->get("timing");
    if (timingValue) {
        errors->setName("timing");
        result->m_timing = ValueConversions<protocol::Network::ResourceTiming>::parse(timingValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Response::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::serialize(m_url));
    result->setValue("status", ValueConversions<double>::serialize(m_status));
    result->setValue("statusText", ValueConversions<String>::serialize(m_statusText));
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::serialize(m_headers.get()));
    if (m_headersText.isJust()) {
        result->setValue("headersText", ValueConversions<String>::serialize(m_headersText.fromJust()));
    }
    result->setValue("mimeType", ValueConversions<String>::serialize(m_mimeType));
    if (m_requestHeaders.isJust()) {
        result->setValue("requestHeaders", ValueConversions<protocol::Network::Headers>::serialize(m_requestHeaders.fromJust()));
    }
    if (m_requestHeadersText.isJust()) {
        result->setValue("requestHeadersText", ValueConversions<String>::serialize(m_requestHeadersText.fromJust()));
    }
    if (m_fromDiskCache.isJust()) {
        result->setValue("fromDiskCache", ValueConversions<bool>::serialize(m_fromDiskCache.fromJust()));
    }
    if (m_timing.isJust()) {
        result->setValue("timing", ValueConversions<protocol::Network::ResourceTiming>::serialize(m_timing.fromJust()));
    }
    return result;
}

std::unique_ptr<Response> Response::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<WebSocketRequest> WebSocketRequest::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketRequest> result(new WebSocketRequest());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::parse(headersValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketRequest::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::serialize(m_headers.get()));
    return result;
}

std::unique_ptr<WebSocketRequest> WebSocketRequest::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<WebSocketResponse> WebSocketResponse::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketResponse> result(new WebSocketResponse());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* statusValue = object->get("status");
    errors->setName("status");
    result->m_status = ValueConversions<double>::parse(statusValue, errors);
    protocol::Value* statusTextValue = object->get("statusText");
    errors->setName("statusText");
    result->m_statusText = ValueConversions<String>::parse(statusTextValue, errors);
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::parse(headersValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketResponse::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("status", ValueConversions<double>::serialize(m_status));
    result->setValue("statusText", ValueConversions<String>::serialize(m_statusText));
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::serialize(m_headers.get()));
    return result;
}

std::unique_ptr<WebSocketResponse> WebSocketResponse::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<WebSocketFrame> WebSocketFrame::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketFrame> result(new WebSocketFrame());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* opcodeValue = object->get("opcode");
    errors->setName("opcode");
    result->m_opcode = ValueConversions<double>::parse(opcodeValue, errors);
    protocol::Value* maskValue = object->get("mask");
    errors->setName("mask");
    result->m_mask = ValueConversions<bool>::parse(maskValue, errors);
    protocol::Value* payloadDataValue = object->get("payloadData");
    errors->setName("payloadData");
    result->m_payloadData = ValueConversions<String>::parse(payloadDataValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketFrame::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("opcode", ValueConversions<double>::serialize(m_opcode));
    result->setValue("mask", ValueConversions<bool>::serialize(m_mask));
    result->setValue("payloadData", ValueConversions<String>::serialize(m_payloadData));
    return result;
}

std::unique_ptr<WebSocketFrame> WebSocketFrame::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CachedResource> CachedResource::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CachedResource> result(new CachedResource());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::parse(urlValue, errors);
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::parse(typeValue, errors);
    protocol::Value* responseValue = object->get("response");
    if (responseValue) {
        errors->setName("response");
        result->m_response = ValueConversions<protocol::Network::Response>::parse(responseValue, errors);
    }
    protocol::Value* bodySizeValue = object->get("bodySize");
    errors->setName("bodySize");
    result->m_bodySize = ValueConversions<double>::parse(bodySizeValue, errors);
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::parse(sourceMapURLValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CachedResource::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::serialize(m_url));
    result->setValue("type", ValueConversions<String>::serialize(m_type));
    if (m_response.isJust()) {
        result->setValue("response", ValueConversions<protocol::Network::Response>::serialize(m_response.fromJust()));
    }
    result->setValue("bodySize", ValueConversions<double>::serialize(m_bodySize));
    if (m_sourceMapURL.isJust()) {
        result->setValue("sourceMapURL", ValueConversions<String>::serialize(m_sourceMapURL.fromJust()));
    }
    return result;
}

std::unique_ptr<CachedResource> CachedResource::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

const char* Initiator::TypeEnum::Parser = "parser";
const char* Initiator::TypeEnum::Script = "script";
const char* Initiator::TypeEnum::Other = "other";

std::unique_ptr<Initiator> Initiator::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Initiator> result(new Initiator());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::parse(typeValue, errors);
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::parse(stackTraceValue, errors);
    }
    protocol::Value* urlValue = object->get("url");
    if (urlValue) {
        errors->setName("url");
        result->m_url = ValueConversions<String>::parse(urlValue, errors);
    }
    protocol::Value* lineNumberValue = object->get("lineNumber");
    if (lineNumberValue) {
        errors->setName("lineNumber");
        result->m_lineNumber = ValueConversions<double>::parse(lineNumberValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Initiator::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("type", ValueConversions<String>::serialize(m_type));
    if (m_stackTrace.isJust()) {
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::serialize(m_stackTrace.fromJust()));
    }
    if (m_url.isJust()) {
        result->setValue("url", ValueConversions<String>::serialize(m_url.fromJust()));
    }
    if (m_lineNumber.isJust()) {
        result->setValue("lineNumber", ValueConversions<double>::serialize(m_lineNumber.fromJust()));
    }
    return result;
}

std::unique_ptr<Initiator> Initiator::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::requestWillBeSent(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, std::unique_ptr<protocol::Network::Request> request, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, const Maybe<protocol::Network::Response>& redirectResponse, const Maybe<String>& type) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.requestWillBeSent");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    paramsObject->setValue("loaderId", ValueConversions<String>::serialize(loaderId));
    paramsObject->setValue("documentURL", ValueConversions<String>::serialize(documentURL));
    paramsObject->setValue("request", ValueConversions<protocol::Network::Request>::serialize(request.get()));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("initiator", ValueConversions<protocol::Network::Initiator>::serialize(initiator.get()));
    if (redirectResponse.isJust()) {
        paramsObject->setValue("redirectResponse", ValueConversions<protocol::Network::Response>::serialize(redirectResponse.fromJust()));
    }
    if (type.isJust()) {
        paramsObject->setValue("type", ValueConversions<String>::serialize(type.fromJust()));
    }
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::requestServedFromCache(const String& requestId) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.requestServedFromCache");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::responseReceived(const String& requestId, const String& frameId, const String& loaderId, double timestamp, const String& type, std::unique_ptr<protocol::Network::Response> response) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.responseReceived");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    paramsObject->setValue("loaderId", ValueConversions<String>::serialize(loaderId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("type", ValueConversions<String>::serialize(type));
    paramsObject->setValue("response", ValueConversions<protocol::Network::Response>::serialize(response.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::dataReceived(const String& requestId, double timestamp, int dataLength, int encodedDataLength) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.dataReceived");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("dataLength", ValueConversions<int>::serialize(dataLength));
    paramsObject->setValue("encodedDataLength", ValueConversions<int>::serialize(encodedDataLength));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::loadingFinished(const String& requestId, double timestamp, const Maybe<String>& sourceMapURL) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.loadingFinished");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    if (sourceMapURL.isJust()) {
        paramsObject->setValue("sourceMapURL", ValueConversions<String>::serialize(sourceMapURL.fromJust()));
    }
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::loadingFailed(const String& requestId, double timestamp, const String& errorText, const Maybe<bool>& canceled) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.loadingFailed");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("errorText", ValueConversions<String>::serialize(errorText));
    if (canceled.isJust()) {
        paramsObject->setValue("canceled", ValueConversions<bool>::serialize(canceled.fromJust()));
    }
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::requestServedFromMemoryCache(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, std::unique_ptr<protocol::Network::CachedResource> resource) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.requestServedFromMemoryCache");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    paramsObject->setValue("loaderId", ValueConversions<String>::serialize(loaderId));
    paramsObject->setValue("documentURL", ValueConversions<String>::serialize(documentURL));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("initiator", ValueConversions<protocol::Network::Initiator>::serialize(initiator.get()));
    paramsObject->setValue("resource", ValueConversions<protocol::Network::CachedResource>::serialize(resource.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::webSocketWillSendHandshakeRequest(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketRequest> request) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.webSocketWillSendHandshakeRequest");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("request", ValueConversions<protocol::Network::WebSocketRequest>::serialize(request.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::webSocketHandshakeResponseReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketResponse> response) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.webSocketHandshakeResponseReceived");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("response", ValueConversions<protocol::Network::WebSocketResponse>::serialize(response.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::webSocketCreated(const String& requestId, const String& url) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.webSocketCreated");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("url", ValueConversions<String>::serialize(url));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::webSocketClosed(const String& requestId, double timestamp) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.webSocketClosed");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::webSocketFrameReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.webSocketFrameReceived");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("response", ValueConversions<protocol::Network::WebSocketFrame>::serialize(response.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::webSocketFrameError(const String& requestId, double timestamp, const String& errorMessage) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.webSocketFrameError");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("errorMessage", ValueConversions<String>::serialize(errorMessage));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::webSocketFrameSent(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Network.webSocketFrameSent");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("requestId", ValueConversions<String>::serialize(requestId));
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    paramsObject->setValue("response", ValueConversions<protocol::Network::WebSocketFrame>::serialize(response.get()));
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
            m_dispatchMap["Network.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["Network.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["Network.setExtraHTTPHeaders"] = &DispatcherImpl::setExtraHTTPHeaders;
            m_dispatchMap["Network.getResponseBody"] = &DispatcherImpl::getResponseBody;
            m_dispatchMap["Network.setCacheDisabled"] = &DispatcherImpl::setCacheDisabled;
            m_dispatchMap["Network.loadResource"] = &DispatcherImpl::loadResource;
        }
        ~DispatcherImpl() override { }
        void dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

    protected:
        using CallHandler = void (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;

        void enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setExtraHTTPHeaders(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getResponseBody(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setCacheDisabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void loadResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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

void DispatcherImpl::setExtraHTTPHeaders(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* headersValue = object ? object->get("headers") : nullptr;
    errors->setName("headers");
    std::unique_ptr<protocol::Network::Headers> in_headers = ValueConversions<protocol::Network::Headers>::parse(headersValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setExtraHTTPHeaders(&error, std::move(in_headers));
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::getResponseBody(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* requestIdValue = object ? object->get("requestId") : nullptr;
    errors->setName("requestId");
    String in_requestId = ValueConversions<String>::parse(requestIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_body;
    bool out_base64Encoded;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getResponseBody(&error, in_requestId, &out_body, &out_base64Encoded);
    if (!error.length()) {
        result->setValue("body", ValueConversions<String>::serialize(out_body));
        result->setValue("base64Encoded", ValueConversions<bool>::serialize(out_base64Encoded));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::setCacheDisabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* cacheDisabledValue = object ? object->get("cacheDisabled") : nullptr;
    errors->setName("cacheDisabled");
    bool in_cacheDisabled = ValueConversions<bool>::parse(cacheDisabledValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setCacheDisabled(&error, in_cacheDisabled);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

class LoadResourceCallbackImpl : public Backend::LoadResourceCallback, public DispatcherBase::Callback {
    public:
        LoadResourceCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId)
            : DispatcherBase::Callback(std::move(backendImpl), callId) { }

        void sendSuccess(const String& content, const String& mimeType, double status) override {
            std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
            resultObject->setValue("content", ValueConversions<String>::serialize(content));
            resultObject->setValue("mimeType", ValueConversions<String>::serialize(mimeType));
            resultObject->setValue("status", ValueConversions<double>::serialize(status));
            sendIfActive(std::move(resultObject), ErrorString());
        }

        void sendFailure(const ErrorString& error) override {
            DCHECK(error.length());
            sendIfActive(nullptr, error);
        }

};

void DispatcherImpl::loadResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::parse(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    std::unique_ptr<LoadResourceCallbackImpl> callback(new LoadResourceCallbackImpl(weakPtr(), callId));

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    m_backend->loadResource(in_frameId,in_url,std::move(callback));
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("Network", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // Network
} // namespace v8_inspector
} // namespace protocol

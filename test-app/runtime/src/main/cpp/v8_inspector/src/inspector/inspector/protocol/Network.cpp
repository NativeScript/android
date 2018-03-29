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

std::unique_ptr<ResourceTiming> ResourceTiming::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ResourceTiming> result(new ResourceTiming());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* startTimeValue = object->get("startTime");
    errors->setName("startTime");
    result->m_startTime = ValueConversions<double>::fromValue(startTimeValue, errors);
    protocol::Value* domainLookupStartValue = object->get("domainLookupStart");
    errors->setName("domainLookupStart");
    result->m_domainLookupStart = ValueConversions<double>::fromValue(domainLookupStartValue, errors);
    protocol::Value* domainLookupEndValue = object->get("domainLookupEnd");
    errors->setName("domainLookupEnd");
    result->m_domainLookupEnd = ValueConversions<double>::fromValue(domainLookupEndValue, errors);
    protocol::Value* connectStartValue = object->get("connectStart");
    errors->setName("connectStart");
    result->m_connectStart = ValueConversions<double>::fromValue(connectStartValue, errors);
    protocol::Value* connectEndValue = object->get("connectEnd");
    errors->setName("connectEnd");
    result->m_connectEnd = ValueConversions<double>::fromValue(connectEndValue, errors);
    protocol::Value* secureConnectionStartValue = object->get("secureConnectionStart");
    errors->setName("secureConnectionStart");
    result->m_secureConnectionStart = ValueConversions<double>::fromValue(secureConnectionStartValue, errors);
    protocol::Value* requestStartValue = object->get("requestStart");
    errors->setName("requestStart");
    result->m_requestStart = ValueConversions<double>::fromValue(requestStartValue, errors);
    protocol::Value* responseStartValue = object->get("responseStart");
    errors->setName("responseStart");
    result->m_responseStart = ValueConversions<double>::fromValue(responseStartValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ResourceTiming::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("startTime", ValueConversions<double>::toValue(m_startTime));
    result->setValue("domainLookupStart", ValueConversions<double>::toValue(m_domainLookupStart));
    result->setValue("domainLookupEnd", ValueConversions<double>::toValue(m_domainLookupEnd));
    result->setValue("connectStart", ValueConversions<double>::toValue(m_connectStart));
    result->setValue("connectEnd", ValueConversions<double>::toValue(m_connectEnd));
    result->setValue("secureConnectionStart", ValueConversions<double>::toValue(m_secureConnectionStart));
    result->setValue("requestStart", ValueConversions<double>::toValue(m_requestStart));
    result->setValue("responseStart", ValueConversions<double>::toValue(m_responseStart));
    return result;
}

std::unique_ptr<ResourceTiming> ResourceTiming::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<Request> Request::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Request> result(new Request());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* methodValue = object->get("method");
    errors->setName("method");
    result->m_method = ValueConversions<String>::fromValue(methodValue, errors);
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::fromValue(headersValue, errors);
    protocol::Value* postDataValue = object->get("postData");
    if (postDataValue) {
        errors->setName("postData");
        result->m_postData = ValueConversions<String>::fromValue(postDataValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Request::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("method", ValueConversions<String>::toValue(m_method));
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::toValue(m_headers.get()));
    if (m_postData.isJust())
        result->setValue("postData", ValueConversions<String>::toValue(m_postData.fromJust()));
    return result;
}

std::unique_ptr<Request> Request::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<Response> Response::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Response> result(new Response());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* statusValue = object->get("status");
    errors->setName("status");
    result->m_status = ValueConversions<double>::fromValue(statusValue, errors);
    protocol::Value* statusTextValue = object->get("statusText");
    errors->setName("statusText");
    result->m_statusText = ValueConversions<String>::fromValue(statusTextValue, errors);
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::fromValue(headersValue, errors);
    protocol::Value* headersTextValue = object->get("headersText");
    if (headersTextValue) {
        errors->setName("headersText");
        result->m_headersText = ValueConversions<String>::fromValue(headersTextValue, errors);
    }
    protocol::Value* mimeTypeValue = object->get("mimeType");
    errors->setName("mimeType");
    result->m_mimeType = ValueConversions<String>::fromValue(mimeTypeValue, errors);
    protocol::Value* requestHeadersValue = object->get("requestHeaders");
    if (requestHeadersValue) {
        errors->setName("requestHeaders");
        result->m_requestHeaders = ValueConversions<protocol::Network::Headers>::fromValue(requestHeadersValue, errors);
    }
    protocol::Value* requestHeadersTextValue = object->get("requestHeadersText");
    if (requestHeadersTextValue) {
        errors->setName("requestHeadersText");
        result->m_requestHeadersText = ValueConversions<String>::fromValue(requestHeadersTextValue, errors);
    }
    protocol::Value* fromDiskCacheValue = object->get("fromDiskCache");
    if (fromDiskCacheValue) {
        errors->setName("fromDiskCache");
        result->m_fromDiskCache = ValueConversions<bool>::fromValue(fromDiskCacheValue, errors);
    }
    protocol::Value* timingValue = object->get("timing");
    if (timingValue) {
        errors->setName("timing");
        result->m_timing = ValueConversions<protocol::Network::ResourceTiming>::fromValue(timingValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Response::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("status", ValueConversions<double>::toValue(m_status));
    result->setValue("statusText", ValueConversions<String>::toValue(m_statusText));
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::toValue(m_headers.get()));
    if (m_headersText.isJust())
        result->setValue("headersText", ValueConversions<String>::toValue(m_headersText.fromJust()));
    result->setValue("mimeType", ValueConversions<String>::toValue(m_mimeType));
    if (m_requestHeaders.isJust())
        result->setValue("requestHeaders", ValueConversions<protocol::Network::Headers>::toValue(m_requestHeaders.fromJust()));
    if (m_requestHeadersText.isJust())
        result->setValue("requestHeadersText", ValueConversions<String>::toValue(m_requestHeadersText.fromJust()));
    if (m_fromDiskCache.isJust())
        result->setValue("fromDiskCache", ValueConversions<bool>::toValue(m_fromDiskCache.fromJust()));
    if (m_timing.isJust())
        result->setValue("timing", ValueConversions<protocol::Network::ResourceTiming>::toValue(m_timing.fromJust()));
    return result;
}

std::unique_ptr<Response> Response::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketRequest> WebSocketRequest::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketRequest> result(new WebSocketRequest());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::fromValue(headersValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketRequest::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::toValue(m_headers.get()));
    return result;
}

std::unique_ptr<WebSocketRequest> WebSocketRequest::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketResponse> WebSocketResponse::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketResponse> result(new WebSocketResponse());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* statusValue = object->get("status");
    errors->setName("status");
    result->m_status = ValueConversions<double>::fromValue(statusValue, errors);
    protocol::Value* statusTextValue = object->get("statusText");
    errors->setName("statusText");
    result->m_statusText = ValueConversions<String>::fromValue(statusTextValue, errors);
    protocol::Value* headersValue = object->get("headers");
    errors->setName("headers");
    result->m_headers = ValueConversions<protocol::Network::Headers>::fromValue(headersValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketResponse::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("status", ValueConversions<double>::toValue(m_status));
    result->setValue("statusText", ValueConversions<String>::toValue(m_statusText));
    result->setValue("headers", ValueConversions<protocol::Network::Headers>::toValue(m_headers.get()));
    return result;
}

std::unique_ptr<WebSocketResponse> WebSocketResponse::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketFrame> WebSocketFrame::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketFrame> result(new WebSocketFrame());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* opcodeValue = object->get("opcode");
    errors->setName("opcode");
    result->m_opcode = ValueConversions<double>::fromValue(opcodeValue, errors);
    protocol::Value* maskValue = object->get("mask");
    errors->setName("mask");
    result->m_mask = ValueConversions<bool>::fromValue(maskValue, errors);
    protocol::Value* payloadDataValue = object->get("payloadData");
    errors->setName("payloadData");
    result->m_payloadData = ValueConversions<String>::fromValue(payloadDataValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketFrame::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("opcode", ValueConversions<double>::toValue(m_opcode));
    result->setValue("mask", ValueConversions<bool>::toValue(m_mask));
    result->setValue("payloadData", ValueConversions<String>::toValue(m_payloadData));
    return result;
}

std::unique_ptr<WebSocketFrame> WebSocketFrame::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CachedResource> CachedResource::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CachedResource> result(new CachedResource());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* responseValue = object->get("response");
    if (responseValue) {
        errors->setName("response");
        result->m_response = ValueConversions<protocol::Network::Response>::fromValue(responseValue, errors);
    }
    protocol::Value* bodySizeValue = object->get("bodySize");
    errors->setName("bodySize");
    result->m_bodySize = ValueConversions<double>::fromValue(bodySizeValue, errors);
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::fromValue(sourceMapURLValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CachedResource::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    if (m_response.isJust())
        result->setValue("response", ValueConversions<protocol::Network::Response>::toValue(m_response.fromJust()));
    result->setValue("bodySize", ValueConversions<double>::toValue(m_bodySize));
    if (m_sourceMapURL.isJust())
        result->setValue("sourceMapURL", ValueConversions<String>::toValue(m_sourceMapURL.fromJust()));
    return result;
}

std::unique_ptr<CachedResource> CachedResource::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* Initiator::TypeEnum::Parser = "parser";
const char* Initiator::TypeEnum::Script = "script";
const char* Initiator::TypeEnum::Other = "other";

std::unique_ptr<Initiator> Initiator::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Initiator> result(new Initiator());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::fromValue(stackTraceValue, errors);
    }
    protocol::Value* urlValue = object->get("url");
    if (urlValue) {
        errors->setName("url");
        result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    }
    protocol::Value* lineNumberValue = object->get("lineNumber");
    if (lineNumberValue) {
        errors->setName("lineNumber");
        result->m_lineNumber = ValueConversions<double>::fromValue(lineNumberValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Initiator::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    if (m_stackTrace.isJust())
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_stackTrace.fromJust()));
    if (m_url.isJust())
        result->setValue("url", ValueConversions<String>::toValue(m_url.fromJust()));
    if (m_lineNumber.isJust())
        result->setValue("lineNumber", ValueConversions<double>::toValue(m_lineNumber.fromJust()));
    return result;
}

std::unique_ptr<Initiator> Initiator::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<RequestWillBeSentNotification> RequestWillBeSentNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RequestWillBeSentNotification> result(new RequestWillBeSentNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* loaderIdValue = object->get("loaderId");
    errors->setName("loaderId");
    result->m_loaderId = ValueConversions<String>::fromValue(loaderIdValue, errors);
    protocol::Value* documentURLValue = object->get("documentURL");
    errors->setName("documentURL");
    result->m_documentURL = ValueConversions<String>::fromValue(documentURLValue, errors);
    protocol::Value* requestValue = object->get("request");
    errors->setName("request");
    result->m_request = ValueConversions<protocol::Network::Request>::fromValue(requestValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* initiatorValue = object->get("initiator");
    errors->setName("initiator");
    result->m_initiator = ValueConversions<protocol::Network::Initiator>::fromValue(initiatorValue, errors);
    protocol::Value* redirectResponseValue = object->get("redirectResponse");
    if (redirectResponseValue) {
        errors->setName("redirectResponse");
        result->m_redirectResponse = ValueConversions<protocol::Network::Response>::fromValue(redirectResponseValue, errors);
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

std::unique_ptr<protocol::DictionaryValue> RequestWillBeSentNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("loaderId", ValueConversions<String>::toValue(m_loaderId));
    result->setValue("documentURL", ValueConversions<String>::toValue(m_documentURL));
    result->setValue("request", ValueConversions<protocol::Network::Request>::toValue(m_request.get()));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("initiator", ValueConversions<protocol::Network::Initiator>::toValue(m_initiator.get()));
    if (m_redirectResponse.isJust())
        result->setValue("redirectResponse", ValueConversions<protocol::Network::Response>::toValue(m_redirectResponse.fromJust()));
    if (m_type.isJust())
        result->setValue("type", ValueConversions<String>::toValue(m_type.fromJust()));
    return result;
}

std::unique_ptr<RequestWillBeSentNotification> RequestWillBeSentNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<RequestServedFromCacheNotification> RequestServedFromCacheNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RequestServedFromCacheNotification> result(new RequestServedFromCacheNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RequestServedFromCacheNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    return result;
}

std::unique_ptr<RequestServedFromCacheNotification> RequestServedFromCacheNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ResponseReceivedNotification> ResponseReceivedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ResponseReceivedNotification> result(new ResponseReceivedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* loaderIdValue = object->get("loaderId");
    errors->setName("loaderId");
    result->m_loaderId = ValueConversions<String>::fromValue(loaderIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* responseValue = object->get("response");
    errors->setName("response");
    result->m_response = ValueConversions<protocol::Network::Response>::fromValue(responseValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ResponseReceivedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("loaderId", ValueConversions<String>::toValue(m_loaderId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    result->setValue("response", ValueConversions<protocol::Network::Response>::toValue(m_response.get()));
    return result;
}

std::unique_ptr<ResponseReceivedNotification> ResponseReceivedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<DataReceivedNotification> DataReceivedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<DataReceivedNotification> result(new DataReceivedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* dataLengthValue = object->get("dataLength");
    errors->setName("dataLength");
    result->m_dataLength = ValueConversions<int>::fromValue(dataLengthValue, errors);
    protocol::Value* encodedDataLengthValue = object->get("encodedDataLength");
    errors->setName("encodedDataLength");
    result->m_encodedDataLength = ValueConversions<int>::fromValue(encodedDataLengthValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> DataReceivedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("dataLength", ValueConversions<int>::toValue(m_dataLength));
    result->setValue("encodedDataLength", ValueConversions<int>::toValue(m_encodedDataLength));
    return result;
}

std::unique_ptr<DataReceivedNotification> DataReceivedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<LoadingFinishedNotification> LoadingFinishedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<LoadingFinishedNotification> result(new LoadingFinishedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::fromValue(sourceMapURLValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LoadingFinishedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    if (m_sourceMapURL.isJust())
        result->setValue("sourceMapURL", ValueConversions<String>::toValue(m_sourceMapURL.fromJust()));
    return result;
}

std::unique_ptr<LoadingFinishedNotification> LoadingFinishedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<LoadingFailedNotification> LoadingFailedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<LoadingFailedNotification> result(new LoadingFailedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* errorTextValue = object->get("errorText");
    errors->setName("errorText");
    result->m_errorText = ValueConversions<String>::fromValue(errorTextValue, errors);
    protocol::Value* canceledValue = object->get("canceled");
    if (canceledValue) {
        errors->setName("canceled");
        result->m_canceled = ValueConversions<bool>::fromValue(canceledValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LoadingFailedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("errorText", ValueConversions<String>::toValue(m_errorText));
    if (m_canceled.isJust())
        result->setValue("canceled", ValueConversions<bool>::toValue(m_canceled.fromJust()));
    return result;
}

std::unique_ptr<LoadingFailedNotification> LoadingFailedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<RequestServedFromMemoryCacheNotification> RequestServedFromMemoryCacheNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RequestServedFromMemoryCacheNotification> result(new RequestServedFromMemoryCacheNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* loaderIdValue = object->get("loaderId");
    errors->setName("loaderId");
    result->m_loaderId = ValueConversions<String>::fromValue(loaderIdValue, errors);
    protocol::Value* documentURLValue = object->get("documentURL");
    errors->setName("documentURL");
    result->m_documentURL = ValueConversions<String>::fromValue(documentURLValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* initiatorValue = object->get("initiator");
    errors->setName("initiator");
    result->m_initiator = ValueConversions<protocol::Network::Initiator>::fromValue(initiatorValue, errors);
    protocol::Value* resourceValue = object->get("resource");
    errors->setName("resource");
    result->m_resource = ValueConversions<protocol::Network::CachedResource>::fromValue(resourceValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RequestServedFromMemoryCacheNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("loaderId", ValueConversions<String>::toValue(m_loaderId));
    result->setValue("documentURL", ValueConversions<String>::toValue(m_documentURL));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("initiator", ValueConversions<protocol::Network::Initiator>::toValue(m_initiator.get()));
    result->setValue("resource", ValueConversions<protocol::Network::CachedResource>::toValue(m_resource.get()));
    return result;
}

std::unique_ptr<RequestServedFromMemoryCacheNotification> RequestServedFromMemoryCacheNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketWillSendHandshakeRequestNotification> WebSocketWillSendHandshakeRequestNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketWillSendHandshakeRequestNotification> result(new WebSocketWillSendHandshakeRequestNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* requestValue = object->get("request");
    errors->setName("request");
    result->m_request = ValueConversions<protocol::Network::WebSocketRequest>::fromValue(requestValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketWillSendHandshakeRequestNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("request", ValueConversions<protocol::Network::WebSocketRequest>::toValue(m_request.get()));
    return result;
}

std::unique_ptr<WebSocketWillSendHandshakeRequestNotification> WebSocketWillSendHandshakeRequestNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketHandshakeResponseReceivedNotification> WebSocketHandshakeResponseReceivedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketHandshakeResponseReceivedNotification> result(new WebSocketHandshakeResponseReceivedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* responseValue = object->get("response");
    errors->setName("response");
    result->m_response = ValueConversions<protocol::Network::WebSocketResponse>::fromValue(responseValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketHandshakeResponseReceivedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("response", ValueConversions<protocol::Network::WebSocketResponse>::toValue(m_response.get()));
    return result;
}

std::unique_ptr<WebSocketHandshakeResponseReceivedNotification> WebSocketHandshakeResponseReceivedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketCreatedNotification> WebSocketCreatedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketCreatedNotification> result(new WebSocketCreatedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketCreatedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    return result;
}

std::unique_ptr<WebSocketCreatedNotification> WebSocketCreatedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketClosedNotification> WebSocketClosedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketClosedNotification> result(new WebSocketClosedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketClosedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    return result;
}

std::unique_ptr<WebSocketClosedNotification> WebSocketClosedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketFrameReceivedNotification> WebSocketFrameReceivedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketFrameReceivedNotification> result(new WebSocketFrameReceivedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* responseValue = object->get("response");
    errors->setName("response");
    result->m_response = ValueConversions<protocol::Network::WebSocketFrame>::fromValue(responseValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketFrameReceivedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("response", ValueConversions<protocol::Network::WebSocketFrame>::toValue(m_response.get()));
    return result;
}

std::unique_ptr<WebSocketFrameReceivedNotification> WebSocketFrameReceivedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketFrameErrorNotification> WebSocketFrameErrorNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketFrameErrorNotification> result(new WebSocketFrameErrorNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* errorMessageValue = object->get("errorMessage");
    errors->setName("errorMessage");
    result->m_errorMessage = ValueConversions<String>::fromValue(errorMessageValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketFrameErrorNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("errorMessage", ValueConversions<String>::toValue(m_errorMessage));
    return result;
}

std::unique_ptr<WebSocketFrameErrorNotification> WebSocketFrameErrorNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WebSocketFrameSentNotification> WebSocketFrameSentNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WebSocketFrameSentNotification> result(new WebSocketFrameSentNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* requestIdValue = object->get("requestId");
    errors->setName("requestId");
    result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* responseValue = object->get("response");
    errors->setName("response");
    result->m_response = ValueConversions<protocol::Network::WebSocketFrame>::fromValue(responseValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WebSocketFrameSentNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("requestId", ValueConversions<String>::toValue(m_requestId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("response", ValueConversions<protocol::Network::WebSocketFrame>::toValue(m_response.get()));
    return result;
}

std::unique_ptr<WebSocketFrameSentNotification> WebSocketFrameSentNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::requestWillBeSent(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, std::unique_ptr<protocol::Network::Request> request, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, Maybe<protocol::Network::Response> redirectResponse, Maybe<String> type)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<RequestWillBeSentNotification> messageData = RequestWillBeSentNotification::create()
        .setRequestId(requestId)
        .setFrameId(frameId)
        .setLoaderId(loaderId)
        .setDocumentURL(documentURL)
        .setRequest(std::move(request))
        .setTimestamp(timestamp)
        .setInitiator(std::move(initiator))
        .build();
    if (redirectResponse.isJust())
        messageData->setRedirectResponse(std::move(redirectResponse).takeJust());
    if (type.isJust())
        messageData->setType(std::move(type).takeJust());
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.requestWillBeSent", std::move(messageData)));
}

void Frontend::requestServedFromCache(const String& requestId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<RequestServedFromCacheNotification> messageData = RequestServedFromCacheNotification::create()
        .setRequestId(requestId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.requestServedFromCache", std::move(messageData)));
}

void Frontend::responseReceived(const String& requestId, const String& frameId, const String& loaderId, double timestamp, const String& type, std::unique_ptr<protocol::Network::Response> response)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ResponseReceivedNotification> messageData = ResponseReceivedNotification::create()
        .setRequestId(requestId)
        .setFrameId(frameId)
        .setLoaderId(loaderId)
        .setTimestamp(timestamp)
        .setType(type)
        .setResponse(std::move(response))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.responseReceived", std::move(messageData)));
}

void Frontend::dataReceived(const String& requestId, double timestamp, int dataLength, int encodedDataLength)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<DataReceivedNotification> messageData = DataReceivedNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .setDataLength(dataLength)
        .setEncodedDataLength(encodedDataLength)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.dataReceived", std::move(messageData)));
}

void Frontend::loadingFinished(const String& requestId, double timestamp, Maybe<String> sourceMapURL)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<LoadingFinishedNotification> messageData = LoadingFinishedNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .build();
    if (sourceMapURL.isJust())
        messageData->setSourceMapURL(std::move(sourceMapURL).takeJust());
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.loadingFinished", std::move(messageData)));
}

void Frontend::loadingFailed(const String& requestId, double timestamp, const String& errorText, Maybe<bool> canceled)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<LoadingFailedNotification> messageData = LoadingFailedNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .setErrorText(errorText)
        .build();
    if (canceled.isJust())
        messageData->setCanceled(std::move(canceled).takeJust());
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.loadingFailed", std::move(messageData)));
}

void Frontend::requestServedFromMemoryCache(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, std::unique_ptr<protocol::Network::CachedResource> resource)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<RequestServedFromMemoryCacheNotification> messageData = RequestServedFromMemoryCacheNotification::create()
        .setRequestId(requestId)
        .setFrameId(frameId)
        .setLoaderId(loaderId)
        .setDocumentURL(documentURL)
        .setTimestamp(timestamp)
        .setInitiator(std::move(initiator))
        .setResource(std::move(resource))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.requestServedFromMemoryCache", std::move(messageData)));
}

void Frontend::webSocketWillSendHandshakeRequest(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketRequest> request)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<WebSocketWillSendHandshakeRequestNotification> messageData = WebSocketWillSendHandshakeRequestNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .setRequest(std::move(request))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.webSocketWillSendHandshakeRequest", std::move(messageData)));
}

void Frontend::webSocketHandshakeResponseReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketResponse> response)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<WebSocketHandshakeResponseReceivedNotification> messageData = WebSocketHandshakeResponseReceivedNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .setResponse(std::move(response))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.webSocketHandshakeResponseReceived", std::move(messageData)));
}

void Frontend::webSocketCreated(const String& requestId, const String& url)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<WebSocketCreatedNotification> messageData = WebSocketCreatedNotification::create()
        .setRequestId(requestId)
        .setUrl(url)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.webSocketCreated", std::move(messageData)));
}

void Frontend::webSocketClosed(const String& requestId, double timestamp)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<WebSocketClosedNotification> messageData = WebSocketClosedNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.webSocketClosed", std::move(messageData)));
}

void Frontend::webSocketFrameReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<WebSocketFrameReceivedNotification> messageData = WebSocketFrameReceivedNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .setResponse(std::move(response))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.webSocketFrameReceived", std::move(messageData)));
}

void Frontend::webSocketFrameError(const String& requestId, double timestamp, const String& errorMessage)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<WebSocketFrameErrorNotification> messageData = WebSocketFrameErrorNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .setErrorMessage(errorMessage)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.webSocketFrameError", std::move(messageData)));
}

void Frontend::webSocketFrameSent(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<WebSocketFrameSentNotification> messageData = WebSocketFrameSentNotification::create()
        .setRequestId(requestId)
        .setTimestamp(timestamp)
        .setResponse(std::move(response))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Network.webSocketFrameSent", std::move(messageData)));
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
        m_dispatchMap["Network.enable"] = &DispatcherImpl::enable;
        m_dispatchMap["Network.disable"] = &DispatcherImpl::disable;
        m_dispatchMap["Network.setExtraHTTPHeaders"] = &DispatcherImpl::setExtraHTTPHeaders;
        m_dispatchMap["Network.getResponseBody"] = &DispatcherImpl::getResponseBody;
        m_dispatchMap["Network.setCacheDisabled"] = &DispatcherImpl::setCacheDisabled;
        m_dispatchMap["Network.loadResource"] = &DispatcherImpl::loadResource;
    }
    ~DispatcherImpl() override { }
    DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

protected:
    using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String, CallHandler>;
    DispatchMap m_dispatchMap;

    DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setExtraHTTPHeaders(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getResponseBody(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setCacheDisabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status loadResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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


DispatchResponse::Status DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setExtraHTTPHeaders(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* headersValue = object ? object->get("headers") : nullptr;
    errors->setName("headers");
    std::unique_ptr<protocol::Network::Headers> in_headers = ValueConversions<protocol::Network::Headers>::fromValue(headersValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setExtraHTTPHeaders(std::move(in_headers));
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getResponseBody(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* requestIdValue = object ? object->get("requestId") : nullptr;
    errors->setName("requestId");
    String in_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_body;
    bool out_base64Encoded;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getResponseBody(in_requestId, &out_body, &out_base64Encoded);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("body", ValueConversions<String>::toValue(out_body));
        result->setValue("base64Encoded", ValueConversions<bool>::toValue(out_base64Encoded));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setCacheDisabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* cacheDisabledValue = object ? object->get("cacheDisabled") : nullptr;
    errors->setName("cacheDisabled");
    bool in_cacheDisabled = ValueConversions<bool>::fromValue(cacheDisabledValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setCacheDisabled(in_cacheDisabled);
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::loadResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::fromValue(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_content;
    String out_mimeType;
    double out_status;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->loadResource(in_frameId, in_url, &out_content, &out_mimeType, &out_status);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("content", ValueConversions<String>::toValue(out_content));
        result->setValue("mimeType", ValueConversions<String>::toValue(out_mimeType));
        result->setValue("status", ValueConversions<double>::toValue(out_status));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend)
{
    dispatcher->registerBackend("Network", std::unique_ptr<protocol::DispatcherBase>(new DispatcherImpl(dispatcher->channel(), backend, dispatcher->fallThroughForNotFound())));
}

} // Network
} // namespace v8_inspector
} // namespace protocol

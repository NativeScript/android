// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Network_h
#define v8_inspector_protocol_Network_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.

namespace v8_inspector {
namespace protocol {
namespace Network {

// ------------- Forward and enum declarations.
// Unique loader identifier.
using LoaderId = String;
// Unique frame identifier.
using FrameId = String;
// Unique request identifier.
using RequestId = String;
// Number of seconds since epoch.
using Timestamp = double;
// Request / response headers as keys / values of JSON object.
using Headers = Object;
// Timing information for the request.
class ResourceTiming;
// HTTP request data.
class Request;
// HTTP response data.
class Response;
// WebSocket request data.
class WebSocketRequest;
// WebSocket response data.
class WebSocketResponse;
// WebSocket frame data.
class WebSocketFrame;
// Information about the cached resource.
class CachedResource;
// Information about the request initiator.
class Initiator;
// Wrapper for notification params
class RequestWillBeSentNotification;
// Wrapper for notification params
class RequestServedFromCacheNotification;
// Wrapper for notification params
class ResponseReceivedNotification;
// Wrapper for notification params
class DataReceivedNotification;
// Wrapper for notification params
class LoadingFinishedNotification;
// Wrapper for notification params
class LoadingFailedNotification;
// Wrapper for notification params
class RequestServedFromMemoryCacheNotification;
// Wrapper for notification params
class WebSocketWillSendHandshakeRequestNotification;
// Wrapper for notification params
class WebSocketHandshakeResponseReceivedNotification;
// Wrapper for notification params
class WebSocketCreatedNotification;
// Wrapper for notification params
class WebSocketClosedNotification;
// Wrapper for notification params
class WebSocketFrameReceivedNotification;
// Wrapper for notification params
class WebSocketFrameErrorNotification;
// Wrapper for notification params
class WebSocketFrameSentNotification;

// ------------- Type and builder declarations.

// Timing information for the request.
class  ResourceTiming : public Serializable{
    PROTOCOL_DISALLOW_COPY(ResourceTiming);
public:
    static std::unique_ptr<ResourceTiming> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ResourceTiming() override { }

    double getStartTime() { return m_startTime; }
    void setStartTime(double value) { m_startTime = value; }

    double getDomainLookupStart() { return m_domainLookupStart; }
    void setDomainLookupStart(double value) { m_domainLookupStart = value; }

    double getDomainLookupEnd() { return m_domainLookupEnd; }
    void setDomainLookupEnd(double value) { m_domainLookupEnd = value; }

    double getConnectStart() { return m_connectStart; }
    void setConnectStart(double value) { m_connectStart = value; }

    double getConnectEnd() { return m_connectEnd; }
    void setConnectEnd(double value) { m_connectEnd = value; }

    double getSecureConnectionStart() { return m_secureConnectionStart; }
    void setSecureConnectionStart(double value) { m_secureConnectionStart = value; }

    double getRequestStart() { return m_requestStart; }
    void setRequestStart(double value) { m_requestStart = value; }

    double getResponseStart() { return m_responseStart; }
    void setResponseStart(double value) { m_responseStart = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ResourceTiming> clone() const;

    template<int STATE>
    class ResourceTimingBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          StartTimeSet = 1 << 1,
          DomainLookupStartSet = 1 << 2,
          DomainLookupEndSet = 1 << 3,
          ConnectStartSet = 1 << 4,
          ConnectEndSet = 1 << 5,
          SecureConnectionStartSet = 1 << 6,
          RequestStartSet = 1 << 7,
          ResponseStartSet = 1 << 8,
            AllFieldsSet = (StartTimeSet | DomainLookupStartSet | DomainLookupEndSet | ConnectStartSet | ConnectEndSet | SecureConnectionStartSet | RequestStartSet | ResponseStartSet | 0)};


        ResourceTimingBuilder<STATE | StartTimeSet>& setStartTime(double value)
        {
            static_assert(!(STATE & StartTimeSet), "property startTime should not be set yet");
            m_result->setStartTime(value);
            return castState<StartTimeSet>();
        }

        ResourceTimingBuilder<STATE | DomainLookupStartSet>& setDomainLookupStart(double value)
        {
            static_assert(!(STATE & DomainLookupStartSet), "property domainLookupStart should not be set yet");
            m_result->setDomainLookupStart(value);
            return castState<DomainLookupStartSet>();
        }

        ResourceTimingBuilder<STATE | DomainLookupEndSet>& setDomainLookupEnd(double value)
        {
            static_assert(!(STATE & DomainLookupEndSet), "property domainLookupEnd should not be set yet");
            m_result->setDomainLookupEnd(value);
            return castState<DomainLookupEndSet>();
        }

        ResourceTimingBuilder<STATE | ConnectStartSet>& setConnectStart(double value)
        {
            static_assert(!(STATE & ConnectStartSet), "property connectStart should not be set yet");
            m_result->setConnectStart(value);
            return castState<ConnectStartSet>();
        }

        ResourceTimingBuilder<STATE | ConnectEndSet>& setConnectEnd(double value)
        {
            static_assert(!(STATE & ConnectEndSet), "property connectEnd should not be set yet");
            m_result->setConnectEnd(value);
            return castState<ConnectEndSet>();
        }

        ResourceTimingBuilder<STATE | SecureConnectionStartSet>& setSecureConnectionStart(double value)
        {
            static_assert(!(STATE & SecureConnectionStartSet), "property secureConnectionStart should not be set yet");
            m_result->setSecureConnectionStart(value);
            return castState<SecureConnectionStartSet>();
        }

        ResourceTimingBuilder<STATE | RequestStartSet>& setRequestStart(double value)
        {
            static_assert(!(STATE & RequestStartSet), "property requestStart should not be set yet");
            m_result->setRequestStart(value);
            return castState<RequestStartSet>();
        }

        ResourceTimingBuilder<STATE | ResponseStartSet>& setResponseStart(double value)
        {
            static_assert(!(STATE & ResponseStartSet), "property responseStart should not be set yet");
            m_result->setResponseStart(value);
            return castState<ResponseStartSet>();
        }

        std::unique_ptr<ResourceTiming> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ResourceTiming;
        ResourceTimingBuilder() : m_result(new ResourceTiming()) { }

        template<int STEP> ResourceTimingBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ResourceTimingBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::ResourceTiming> m_result;
    };

    static ResourceTimingBuilder<0> create()
    {
        return ResourceTimingBuilder<0>();
    }

private:
    ResourceTiming()
    {
          m_startTime = 0;
          m_domainLookupStart = 0;
          m_domainLookupEnd = 0;
          m_connectStart = 0;
          m_connectEnd = 0;
          m_secureConnectionStart = 0;
          m_requestStart = 0;
          m_responseStart = 0;
    }

    double m_startTime;
    double m_domainLookupStart;
    double m_domainLookupEnd;
    double m_connectStart;
    double m_connectEnd;
    double m_secureConnectionStart;
    double m_requestStart;
    double m_responseStart;
};


// HTTP request data.
class  Request : public Serializable{
    PROTOCOL_DISALLOW_COPY(Request);
public:
    static std::unique_ptr<Request> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~Request() override { }

    String getUrl() { return m_url; }
    void setUrl(const String& value) { m_url = value; }

    String getMethod() { return m_method; }
    void setMethod(const String& value) { m_method = value; }

    protocol::Network::Headers* getHeaders() { return m_headers.get(); }
    void setHeaders(std::unique_ptr<protocol::Network::Headers> value) { m_headers = std::move(value); }

    bool hasPostData() { return m_postData.isJust(); }
    String getPostData(const String& defaultValue) { return m_postData.isJust() ? m_postData.fromJust() : defaultValue; }
    void setPostData(const String& value) { m_postData = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<Request> clone() const;

    template<int STATE>
    class RequestBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          UrlSet = 1 << 1,
          MethodSet = 1 << 2,
          HeadersSet = 1 << 3,
            AllFieldsSet = (UrlSet | MethodSet | HeadersSet | 0)};


        RequestBuilder<STATE | UrlSet>& setUrl(const String& value)
        {
            static_assert(!(STATE & UrlSet), "property url should not be set yet");
            m_result->setUrl(value);
            return castState<UrlSet>();
        }

        RequestBuilder<STATE | MethodSet>& setMethod(const String& value)
        {
            static_assert(!(STATE & MethodSet), "property method should not be set yet");
            m_result->setMethod(value);
            return castState<MethodSet>();
        }

        RequestBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value)
        {
            static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
            m_result->setHeaders(std::move(value));
            return castState<HeadersSet>();
        }

        RequestBuilder<STATE>& setPostData(const String& value)
        {
            m_result->setPostData(value);
            return *this;
        }

        std::unique_ptr<Request> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class Request;
        RequestBuilder() : m_result(new Request()) { }

        template<int STEP> RequestBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<RequestBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::Request> m_result;
    };

    static RequestBuilder<0> create()
    {
        return RequestBuilder<0>();
    }

private:
    Request()
    {
    }

    String m_url;
    String m_method;
    std::unique_ptr<protocol::Network::Headers> m_headers;
    Maybe<String> m_postData;
};


// HTTP response data.
class  Response : public Serializable{
    PROTOCOL_DISALLOW_COPY(Response);
public:
    static std::unique_ptr<Response> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~Response() override { }

    String getUrl() { return m_url; }
    void setUrl(const String& value) { m_url = value; }

    double getStatus() { return m_status; }
    void setStatus(double value) { m_status = value; }

    String getStatusText() { return m_statusText; }
    void setStatusText(const String& value) { m_statusText = value; }

    protocol::Network::Headers* getHeaders() { return m_headers.get(); }
    void setHeaders(std::unique_ptr<protocol::Network::Headers> value) { m_headers = std::move(value); }

    bool hasHeadersText() { return m_headersText.isJust(); }
    String getHeadersText(const String& defaultValue) { return m_headersText.isJust() ? m_headersText.fromJust() : defaultValue; }
    void setHeadersText(const String& value) { m_headersText = value; }

    String getMimeType() { return m_mimeType; }
    void setMimeType(const String& value) { m_mimeType = value; }

    bool hasRequestHeaders() { return m_requestHeaders.isJust(); }
    protocol::Network::Headers* getRequestHeaders(protocol::Network::Headers* defaultValue) { return m_requestHeaders.isJust() ? m_requestHeaders.fromJust() : defaultValue; }
    void setRequestHeaders(std::unique_ptr<protocol::Network::Headers> value) { m_requestHeaders = std::move(value); }

    bool hasRequestHeadersText() { return m_requestHeadersText.isJust(); }
    String getRequestHeadersText(const String& defaultValue) { return m_requestHeadersText.isJust() ? m_requestHeadersText.fromJust() : defaultValue; }
    void setRequestHeadersText(const String& value) { m_requestHeadersText = value; }

    bool hasFromDiskCache() { return m_fromDiskCache.isJust(); }
    bool getFromDiskCache(bool defaultValue) { return m_fromDiskCache.isJust() ? m_fromDiskCache.fromJust() : defaultValue; }
    void setFromDiskCache(bool value) { m_fromDiskCache = value; }

    bool hasTiming() { return m_timing.isJust(); }
    protocol::Network::ResourceTiming* getTiming(protocol::Network::ResourceTiming* defaultValue) { return m_timing.isJust() ? m_timing.fromJust() : defaultValue; }
    void setTiming(std::unique_ptr<protocol::Network::ResourceTiming> value) { m_timing = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<Response> clone() const;

    template<int STATE>
    class ResponseBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          UrlSet = 1 << 1,
          StatusSet = 1 << 2,
          StatusTextSet = 1 << 3,
          HeadersSet = 1 << 4,
          MimeTypeSet = 1 << 5,
            AllFieldsSet = (UrlSet | StatusSet | StatusTextSet | HeadersSet | MimeTypeSet | 0)};


        ResponseBuilder<STATE | UrlSet>& setUrl(const String& value)
        {
            static_assert(!(STATE & UrlSet), "property url should not be set yet");
            m_result->setUrl(value);
            return castState<UrlSet>();
        }

        ResponseBuilder<STATE | StatusSet>& setStatus(double value)
        {
            static_assert(!(STATE & StatusSet), "property status should not be set yet");
            m_result->setStatus(value);
            return castState<StatusSet>();
        }

        ResponseBuilder<STATE | StatusTextSet>& setStatusText(const String& value)
        {
            static_assert(!(STATE & StatusTextSet), "property statusText should not be set yet");
            m_result->setStatusText(value);
            return castState<StatusTextSet>();
        }

        ResponseBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value)
        {
            static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
            m_result->setHeaders(std::move(value));
            return castState<HeadersSet>();
        }

        ResponseBuilder<STATE>& setHeadersText(const String& value)
        {
            m_result->setHeadersText(value);
            return *this;
        }

        ResponseBuilder<STATE | MimeTypeSet>& setMimeType(const String& value)
        {
            static_assert(!(STATE & MimeTypeSet), "property mimeType should not be set yet");
            m_result->setMimeType(value);
            return castState<MimeTypeSet>();
        }

        ResponseBuilder<STATE>& setRequestHeaders(std::unique_ptr<protocol::Network::Headers> value)
        {
            m_result->setRequestHeaders(std::move(value));
            return *this;
        }

        ResponseBuilder<STATE>& setRequestHeadersText(const String& value)
        {
            m_result->setRequestHeadersText(value);
            return *this;
        }

        ResponseBuilder<STATE>& setFromDiskCache(bool value)
        {
            m_result->setFromDiskCache(value);
            return *this;
        }

        ResponseBuilder<STATE>& setTiming(std::unique_ptr<protocol::Network::ResourceTiming> value)
        {
            m_result->setTiming(std::move(value));
            return *this;
        }

        std::unique_ptr<Response> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class Response;
        ResponseBuilder() : m_result(new Response()) { }

        template<int STEP> ResponseBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ResponseBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::Response> m_result;
    };

    static ResponseBuilder<0> create()
    {
        return ResponseBuilder<0>();
    }

private:
    Response()
    {
          m_status = 0;
    }

    String m_url;
    double m_status;
    String m_statusText;
    std::unique_ptr<protocol::Network::Headers> m_headers;
    Maybe<String> m_headersText;
    String m_mimeType;
    Maybe<protocol::Network::Headers> m_requestHeaders;
    Maybe<String> m_requestHeadersText;
    Maybe<bool> m_fromDiskCache;
    Maybe<protocol::Network::ResourceTiming> m_timing;
};


// WebSocket request data.
class  WebSocketRequest : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketRequest);
public:
    static std::unique_ptr<WebSocketRequest> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketRequest() override { }

    protocol::Network::Headers* getHeaders() { return m_headers.get(); }
    void setHeaders(std::unique_ptr<protocol::Network::Headers> value) { m_headers = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketRequest> clone() const;

    template<int STATE>
    class WebSocketRequestBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          HeadersSet = 1 << 1,
            AllFieldsSet = (HeadersSet | 0)};


        WebSocketRequestBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value)
        {
            static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
            m_result->setHeaders(std::move(value));
            return castState<HeadersSet>();
        }

        std::unique_ptr<WebSocketRequest> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketRequest;
        WebSocketRequestBuilder() : m_result(new WebSocketRequest()) { }

        template<int STEP> WebSocketRequestBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketRequestBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketRequest> m_result;
    };

    static WebSocketRequestBuilder<0> create()
    {
        return WebSocketRequestBuilder<0>();
    }

private:
    WebSocketRequest()
    {
    }

    std::unique_ptr<protocol::Network::Headers> m_headers;
};


// WebSocket response data.
class  WebSocketResponse : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketResponse);
public:
    static std::unique_ptr<WebSocketResponse> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketResponse() override { }

    double getStatus() { return m_status; }
    void setStatus(double value) { m_status = value; }

    String getStatusText() { return m_statusText; }
    void setStatusText(const String& value) { m_statusText = value; }

    protocol::Network::Headers* getHeaders() { return m_headers.get(); }
    void setHeaders(std::unique_ptr<protocol::Network::Headers> value) { m_headers = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketResponse> clone() const;

    template<int STATE>
    class WebSocketResponseBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          StatusSet = 1 << 1,
          StatusTextSet = 1 << 2,
          HeadersSet = 1 << 3,
            AllFieldsSet = (StatusSet | StatusTextSet | HeadersSet | 0)};


        WebSocketResponseBuilder<STATE | StatusSet>& setStatus(double value)
        {
            static_assert(!(STATE & StatusSet), "property status should not be set yet");
            m_result->setStatus(value);
            return castState<StatusSet>();
        }

        WebSocketResponseBuilder<STATE | StatusTextSet>& setStatusText(const String& value)
        {
            static_assert(!(STATE & StatusTextSet), "property statusText should not be set yet");
            m_result->setStatusText(value);
            return castState<StatusTextSet>();
        }

        WebSocketResponseBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value)
        {
            static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
            m_result->setHeaders(std::move(value));
            return castState<HeadersSet>();
        }

        std::unique_ptr<WebSocketResponse> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketResponse;
        WebSocketResponseBuilder() : m_result(new WebSocketResponse()) { }

        template<int STEP> WebSocketResponseBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketResponseBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketResponse> m_result;
    };

    static WebSocketResponseBuilder<0> create()
    {
        return WebSocketResponseBuilder<0>();
    }

private:
    WebSocketResponse()
    {
          m_status = 0;
    }

    double m_status;
    String m_statusText;
    std::unique_ptr<protocol::Network::Headers> m_headers;
};


// WebSocket frame data.
class  WebSocketFrame : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketFrame);
public:
    static std::unique_ptr<WebSocketFrame> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketFrame() override { }

    double getOpcode() { return m_opcode; }
    void setOpcode(double value) { m_opcode = value; }

    bool getMask() { return m_mask; }
    void setMask(bool value) { m_mask = value; }

    String getPayloadData() { return m_payloadData; }
    void setPayloadData(const String& value) { m_payloadData = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketFrame> clone() const;

    template<int STATE>
    class WebSocketFrameBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          OpcodeSet = 1 << 1,
          MaskSet = 1 << 2,
          PayloadDataSet = 1 << 3,
            AllFieldsSet = (OpcodeSet | MaskSet | PayloadDataSet | 0)};


        WebSocketFrameBuilder<STATE | OpcodeSet>& setOpcode(double value)
        {
            static_assert(!(STATE & OpcodeSet), "property opcode should not be set yet");
            m_result->setOpcode(value);
            return castState<OpcodeSet>();
        }

        WebSocketFrameBuilder<STATE | MaskSet>& setMask(bool value)
        {
            static_assert(!(STATE & MaskSet), "property mask should not be set yet");
            m_result->setMask(value);
            return castState<MaskSet>();
        }

        WebSocketFrameBuilder<STATE | PayloadDataSet>& setPayloadData(const String& value)
        {
            static_assert(!(STATE & PayloadDataSet), "property payloadData should not be set yet");
            m_result->setPayloadData(value);
            return castState<PayloadDataSet>();
        }

        std::unique_ptr<WebSocketFrame> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketFrame;
        WebSocketFrameBuilder() : m_result(new WebSocketFrame()) { }

        template<int STEP> WebSocketFrameBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketFrameBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketFrame> m_result;
    };

    static WebSocketFrameBuilder<0> create()
    {
        return WebSocketFrameBuilder<0>();
    }

private:
    WebSocketFrame()
    {
          m_opcode = 0;
          m_mask = false;
    }

    double m_opcode;
    bool m_mask;
    String m_payloadData;
};


// Information about the cached resource.
class  CachedResource : public Serializable{
    PROTOCOL_DISALLOW_COPY(CachedResource);
public:
    static std::unique_ptr<CachedResource> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~CachedResource() override { }

    String getUrl() { return m_url; }
    void setUrl(const String& value) { m_url = value; }

    String getType() { return m_type; }
    void setType(const String& value) { m_type = value; }

    bool hasResponse() { return m_response.isJust(); }
    protocol::Network::Response* getResponse(protocol::Network::Response* defaultValue) { return m_response.isJust() ? m_response.fromJust() : defaultValue; }
    void setResponse(std::unique_ptr<protocol::Network::Response> value) { m_response = std::move(value); }

    double getBodySize() { return m_bodySize; }
    void setBodySize(double value) { m_bodySize = value; }

    bool hasSourceMapURL() { return m_sourceMapURL.isJust(); }
    String getSourceMapURL(const String& defaultValue) { return m_sourceMapURL.isJust() ? m_sourceMapURL.fromJust() : defaultValue; }
    void setSourceMapURL(const String& value) { m_sourceMapURL = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<CachedResource> clone() const;

    template<int STATE>
    class CachedResourceBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          UrlSet = 1 << 1,
          TypeSet = 1 << 2,
          BodySizeSet = 1 << 3,
            AllFieldsSet = (UrlSet | TypeSet | BodySizeSet | 0)};


        CachedResourceBuilder<STATE | UrlSet>& setUrl(const String& value)
        {
            static_assert(!(STATE & UrlSet), "property url should not be set yet");
            m_result->setUrl(value);
            return castState<UrlSet>();
        }

        CachedResourceBuilder<STATE | TypeSet>& setType(const String& value)
        {
            static_assert(!(STATE & TypeSet), "property type should not be set yet");
            m_result->setType(value);
            return castState<TypeSet>();
        }

        CachedResourceBuilder<STATE>& setResponse(std::unique_ptr<protocol::Network::Response> value)
        {
            m_result->setResponse(std::move(value));
            return *this;
        }

        CachedResourceBuilder<STATE | BodySizeSet>& setBodySize(double value)
        {
            static_assert(!(STATE & BodySizeSet), "property bodySize should not be set yet");
            m_result->setBodySize(value);
            return castState<BodySizeSet>();
        }

        CachedResourceBuilder<STATE>& setSourceMapURL(const String& value)
        {
            m_result->setSourceMapURL(value);
            return *this;
        }

        std::unique_ptr<CachedResource> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class CachedResource;
        CachedResourceBuilder() : m_result(new CachedResource()) { }

        template<int STEP> CachedResourceBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<CachedResourceBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::CachedResource> m_result;
    };

    static CachedResourceBuilder<0> create()
    {
        return CachedResourceBuilder<0>();
    }

private:
    CachedResource()
    {
          m_bodySize = 0;
    }

    String m_url;
    String m_type;
    Maybe<protocol::Network::Response> m_response;
    double m_bodySize;
    Maybe<String> m_sourceMapURL;
};


// Information about the request initiator.
class  Initiator : public Serializable{
    PROTOCOL_DISALLOW_COPY(Initiator);
public:
    static std::unique_ptr<Initiator> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~Initiator() override { }

    struct  TypeEnum {
        static const char* Parser;
        static const char* Script;
        static const char* Other;
    }; // TypeEnum

    String getType() { return m_type; }
    void setType(const String& value) { m_type = value; }

    bool hasStackTrace() { return m_stackTrace.isJust(); }
    protocol::Runtime::StackTrace* getStackTrace(protocol::Runtime::StackTrace* defaultValue) { return m_stackTrace.isJust() ? m_stackTrace.fromJust() : defaultValue; }
    void setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) { m_stackTrace = std::move(value); }

    bool hasUrl() { return m_url.isJust(); }
    String getUrl(const String& defaultValue) { return m_url.isJust() ? m_url.fromJust() : defaultValue; }
    void setUrl(const String& value) { m_url = value; }

    bool hasLineNumber() { return m_lineNumber.isJust(); }
    double getLineNumber(double defaultValue) { return m_lineNumber.isJust() ? m_lineNumber.fromJust() : defaultValue; }
    void setLineNumber(double value) { m_lineNumber = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<Initiator> clone() const;

    template<int STATE>
    class InitiatorBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          TypeSet = 1 << 1,
            AllFieldsSet = (TypeSet | 0)};


        InitiatorBuilder<STATE | TypeSet>& setType(const String& value)
        {
            static_assert(!(STATE & TypeSet), "property type should not be set yet");
            m_result->setType(value);
            return castState<TypeSet>();
        }

        InitiatorBuilder<STATE>& setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value)
        {
            m_result->setStackTrace(std::move(value));
            return *this;
        }

        InitiatorBuilder<STATE>& setUrl(const String& value)
        {
            m_result->setUrl(value);
            return *this;
        }

        InitiatorBuilder<STATE>& setLineNumber(double value)
        {
            m_result->setLineNumber(value);
            return *this;
        }

        std::unique_ptr<Initiator> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class Initiator;
        InitiatorBuilder() : m_result(new Initiator()) { }

        template<int STEP> InitiatorBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<InitiatorBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::Initiator> m_result;
    };

    static InitiatorBuilder<0> create()
    {
        return InitiatorBuilder<0>();
    }

private:
    Initiator()
    {
    }

    String m_type;
    Maybe<protocol::Runtime::StackTrace> m_stackTrace;
    Maybe<String> m_url;
    Maybe<double> m_lineNumber;
};


// Wrapper for notification params
class  RequestWillBeSentNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(RequestWillBeSentNotification);
public:
    static std::unique_ptr<RequestWillBeSentNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~RequestWillBeSentNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    String getFrameId() { return m_frameId; }
    void setFrameId(const String& value) { m_frameId = value; }

    String getLoaderId() { return m_loaderId; }
    void setLoaderId(const String& value) { m_loaderId = value; }

    String getDocumentURL() { return m_documentURL; }
    void setDocumentURL(const String& value) { m_documentURL = value; }

    protocol::Network::Request* getRequest() { return m_request.get(); }
    void setRequest(std::unique_ptr<protocol::Network::Request> value) { m_request = std::move(value); }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    protocol::Network::Initiator* getInitiator() { return m_initiator.get(); }
    void setInitiator(std::unique_ptr<protocol::Network::Initiator> value) { m_initiator = std::move(value); }

    bool hasRedirectResponse() { return m_redirectResponse.isJust(); }
    protocol::Network::Response* getRedirectResponse(protocol::Network::Response* defaultValue) { return m_redirectResponse.isJust() ? m_redirectResponse.fromJust() : defaultValue; }
    void setRedirectResponse(std::unique_ptr<protocol::Network::Response> value) { m_redirectResponse = std::move(value); }

    bool hasType() { return m_type.isJust(); }
    String getType(const String& defaultValue) { return m_type.isJust() ? m_type.fromJust() : defaultValue; }
    void setType(const String& value) { m_type = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<RequestWillBeSentNotification> clone() const;

    template<int STATE>
    class RequestWillBeSentNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          FrameIdSet = 1 << 2,
          LoaderIdSet = 1 << 3,
          DocumentURLSet = 1 << 4,
          RequestSet = 1 << 5,
          TimestampSet = 1 << 6,
          InitiatorSet = 1 << 7,
            AllFieldsSet = (RequestIdSet | FrameIdSet | LoaderIdSet | DocumentURLSet | RequestSet | TimestampSet | InitiatorSet | 0)};


        RequestWillBeSentNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        RequestWillBeSentNotificationBuilder<STATE | FrameIdSet>& setFrameId(const String& value)
        {
            static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
            m_result->setFrameId(value);
            return castState<FrameIdSet>();
        }

        RequestWillBeSentNotificationBuilder<STATE | LoaderIdSet>& setLoaderId(const String& value)
        {
            static_assert(!(STATE & LoaderIdSet), "property loaderId should not be set yet");
            m_result->setLoaderId(value);
            return castState<LoaderIdSet>();
        }

        RequestWillBeSentNotificationBuilder<STATE | DocumentURLSet>& setDocumentURL(const String& value)
        {
            static_assert(!(STATE & DocumentURLSet), "property documentURL should not be set yet");
            m_result->setDocumentURL(value);
            return castState<DocumentURLSet>();
        }

        RequestWillBeSentNotificationBuilder<STATE | RequestSet>& setRequest(std::unique_ptr<protocol::Network::Request> value)
        {
            static_assert(!(STATE & RequestSet), "property request should not be set yet");
            m_result->setRequest(std::move(value));
            return castState<RequestSet>();
        }

        RequestWillBeSentNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        RequestWillBeSentNotificationBuilder<STATE | InitiatorSet>& setInitiator(std::unique_ptr<protocol::Network::Initiator> value)
        {
            static_assert(!(STATE & InitiatorSet), "property initiator should not be set yet");
            m_result->setInitiator(std::move(value));
            return castState<InitiatorSet>();
        }

        RequestWillBeSentNotificationBuilder<STATE>& setRedirectResponse(std::unique_ptr<protocol::Network::Response> value)
        {
            m_result->setRedirectResponse(std::move(value));
            return *this;
        }

        RequestWillBeSentNotificationBuilder<STATE>& setType(const String& value)
        {
            m_result->setType(value);
            return *this;
        }

        std::unique_ptr<RequestWillBeSentNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class RequestWillBeSentNotification;
        RequestWillBeSentNotificationBuilder() : m_result(new RequestWillBeSentNotification()) { }

        template<int STEP> RequestWillBeSentNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<RequestWillBeSentNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::RequestWillBeSentNotification> m_result;
    };

    static RequestWillBeSentNotificationBuilder<0> create()
    {
        return RequestWillBeSentNotificationBuilder<0>();
    }

private:
    RequestWillBeSentNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    String m_frameId;
    String m_loaderId;
    String m_documentURL;
    std::unique_ptr<protocol::Network::Request> m_request;
    double m_timestamp;
    std::unique_ptr<protocol::Network::Initiator> m_initiator;
    Maybe<protocol::Network::Response> m_redirectResponse;
    Maybe<String> m_type;
};


// Wrapper for notification params
class  RequestServedFromCacheNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(RequestServedFromCacheNotification);
public:
    static std::unique_ptr<RequestServedFromCacheNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~RequestServedFromCacheNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<RequestServedFromCacheNotification> clone() const;

    template<int STATE>
    class RequestServedFromCacheNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
            AllFieldsSet = (RequestIdSet | 0)};


        RequestServedFromCacheNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        std::unique_ptr<RequestServedFromCacheNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class RequestServedFromCacheNotification;
        RequestServedFromCacheNotificationBuilder() : m_result(new RequestServedFromCacheNotification()) { }

        template<int STEP> RequestServedFromCacheNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<RequestServedFromCacheNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::RequestServedFromCacheNotification> m_result;
    };

    static RequestServedFromCacheNotificationBuilder<0> create()
    {
        return RequestServedFromCacheNotificationBuilder<0>();
    }

private:
    RequestServedFromCacheNotification()
    {
    }

    String m_requestId;
};


// Wrapper for notification params
class  ResponseReceivedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ResponseReceivedNotification);
public:
    static std::unique_ptr<ResponseReceivedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ResponseReceivedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    String getFrameId() { return m_frameId; }
    void setFrameId(const String& value) { m_frameId = value; }

    String getLoaderId() { return m_loaderId; }
    void setLoaderId(const String& value) { m_loaderId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    String getType() { return m_type; }
    void setType(const String& value) { m_type = value; }

    protocol::Network::Response* getResponse() { return m_response.get(); }
    void setResponse(std::unique_ptr<protocol::Network::Response> value) { m_response = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ResponseReceivedNotification> clone() const;

    template<int STATE>
    class ResponseReceivedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          FrameIdSet = 1 << 2,
          LoaderIdSet = 1 << 3,
          TimestampSet = 1 << 4,
          TypeSet = 1 << 5,
          ResponseSet = 1 << 6,
            AllFieldsSet = (RequestIdSet | FrameIdSet | LoaderIdSet | TimestampSet | TypeSet | ResponseSet | 0)};


        ResponseReceivedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        ResponseReceivedNotificationBuilder<STATE | FrameIdSet>& setFrameId(const String& value)
        {
            static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
            m_result->setFrameId(value);
            return castState<FrameIdSet>();
        }

        ResponseReceivedNotificationBuilder<STATE | LoaderIdSet>& setLoaderId(const String& value)
        {
            static_assert(!(STATE & LoaderIdSet), "property loaderId should not be set yet");
            m_result->setLoaderId(value);
            return castState<LoaderIdSet>();
        }

        ResponseReceivedNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        ResponseReceivedNotificationBuilder<STATE | TypeSet>& setType(const String& value)
        {
            static_assert(!(STATE & TypeSet), "property type should not be set yet");
            m_result->setType(value);
            return castState<TypeSet>();
        }

        ResponseReceivedNotificationBuilder<STATE | ResponseSet>& setResponse(std::unique_ptr<protocol::Network::Response> value)
        {
            static_assert(!(STATE & ResponseSet), "property response should not be set yet");
            m_result->setResponse(std::move(value));
            return castState<ResponseSet>();
        }

        std::unique_ptr<ResponseReceivedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ResponseReceivedNotification;
        ResponseReceivedNotificationBuilder() : m_result(new ResponseReceivedNotification()) { }

        template<int STEP> ResponseReceivedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ResponseReceivedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::ResponseReceivedNotification> m_result;
    };

    static ResponseReceivedNotificationBuilder<0> create()
    {
        return ResponseReceivedNotificationBuilder<0>();
    }

private:
    ResponseReceivedNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    String m_frameId;
    String m_loaderId;
    double m_timestamp;
    String m_type;
    std::unique_ptr<protocol::Network::Response> m_response;
};


// Wrapper for notification params
class  DataReceivedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(DataReceivedNotification);
public:
    static std::unique_ptr<DataReceivedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~DataReceivedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    int getDataLength() { return m_dataLength; }
    void setDataLength(int value) { m_dataLength = value; }

    int getEncodedDataLength() { return m_encodedDataLength; }
    void setEncodedDataLength(int value) { m_encodedDataLength = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<DataReceivedNotification> clone() const;

    template<int STATE>
    class DataReceivedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
          DataLengthSet = 1 << 3,
          EncodedDataLengthSet = 1 << 4,
            AllFieldsSet = (RequestIdSet | TimestampSet | DataLengthSet | EncodedDataLengthSet | 0)};


        DataReceivedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        DataReceivedNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        DataReceivedNotificationBuilder<STATE | DataLengthSet>& setDataLength(int value)
        {
            static_assert(!(STATE & DataLengthSet), "property dataLength should not be set yet");
            m_result->setDataLength(value);
            return castState<DataLengthSet>();
        }

        DataReceivedNotificationBuilder<STATE | EncodedDataLengthSet>& setEncodedDataLength(int value)
        {
            static_assert(!(STATE & EncodedDataLengthSet), "property encodedDataLength should not be set yet");
            m_result->setEncodedDataLength(value);
            return castState<EncodedDataLengthSet>();
        }

        std::unique_ptr<DataReceivedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class DataReceivedNotification;
        DataReceivedNotificationBuilder() : m_result(new DataReceivedNotification()) { }

        template<int STEP> DataReceivedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<DataReceivedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::DataReceivedNotification> m_result;
    };

    static DataReceivedNotificationBuilder<0> create()
    {
        return DataReceivedNotificationBuilder<0>();
    }

private:
    DataReceivedNotification()
    {
          m_timestamp = 0;
          m_dataLength = 0;
          m_encodedDataLength = 0;
    }

    String m_requestId;
    double m_timestamp;
    int m_dataLength;
    int m_encodedDataLength;
};


// Wrapper for notification params
class  LoadingFinishedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(LoadingFinishedNotification);
public:
    static std::unique_ptr<LoadingFinishedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~LoadingFinishedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    bool hasSourceMapURL() { return m_sourceMapURL.isJust(); }
    String getSourceMapURL(const String& defaultValue) { return m_sourceMapURL.isJust() ? m_sourceMapURL.fromJust() : defaultValue; }
    void setSourceMapURL(const String& value) { m_sourceMapURL = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<LoadingFinishedNotification> clone() const;

    template<int STATE>
    class LoadingFinishedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
            AllFieldsSet = (RequestIdSet | TimestampSet | 0)};


        LoadingFinishedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        LoadingFinishedNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        LoadingFinishedNotificationBuilder<STATE>& setSourceMapURL(const String& value)
        {
            m_result->setSourceMapURL(value);
            return *this;
        }

        std::unique_ptr<LoadingFinishedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class LoadingFinishedNotification;
        LoadingFinishedNotificationBuilder() : m_result(new LoadingFinishedNotification()) { }

        template<int STEP> LoadingFinishedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<LoadingFinishedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::LoadingFinishedNotification> m_result;
    };

    static LoadingFinishedNotificationBuilder<0> create()
    {
        return LoadingFinishedNotificationBuilder<0>();
    }

private:
    LoadingFinishedNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
    Maybe<String> m_sourceMapURL;
};


// Wrapper for notification params
class  LoadingFailedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(LoadingFailedNotification);
public:
    static std::unique_ptr<LoadingFailedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~LoadingFailedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    String getErrorText() { return m_errorText; }
    void setErrorText(const String& value) { m_errorText = value; }

    bool hasCanceled() { return m_canceled.isJust(); }
    bool getCanceled(bool defaultValue) { return m_canceled.isJust() ? m_canceled.fromJust() : defaultValue; }
    void setCanceled(bool value) { m_canceled = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<LoadingFailedNotification> clone() const;

    template<int STATE>
    class LoadingFailedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
          ErrorTextSet = 1 << 3,
            AllFieldsSet = (RequestIdSet | TimestampSet | ErrorTextSet | 0)};


        LoadingFailedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        LoadingFailedNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        LoadingFailedNotificationBuilder<STATE | ErrorTextSet>& setErrorText(const String& value)
        {
            static_assert(!(STATE & ErrorTextSet), "property errorText should not be set yet");
            m_result->setErrorText(value);
            return castState<ErrorTextSet>();
        }

        LoadingFailedNotificationBuilder<STATE>& setCanceled(bool value)
        {
            m_result->setCanceled(value);
            return *this;
        }

        std::unique_ptr<LoadingFailedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class LoadingFailedNotification;
        LoadingFailedNotificationBuilder() : m_result(new LoadingFailedNotification()) { }

        template<int STEP> LoadingFailedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<LoadingFailedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::LoadingFailedNotification> m_result;
    };

    static LoadingFailedNotificationBuilder<0> create()
    {
        return LoadingFailedNotificationBuilder<0>();
    }

private:
    LoadingFailedNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
    String m_errorText;
    Maybe<bool> m_canceled;
};


// Wrapper for notification params
class  RequestServedFromMemoryCacheNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(RequestServedFromMemoryCacheNotification);
public:
    static std::unique_ptr<RequestServedFromMemoryCacheNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~RequestServedFromMemoryCacheNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    String getFrameId() { return m_frameId; }
    void setFrameId(const String& value) { m_frameId = value; }

    String getLoaderId() { return m_loaderId; }
    void setLoaderId(const String& value) { m_loaderId = value; }

    String getDocumentURL() { return m_documentURL; }
    void setDocumentURL(const String& value) { m_documentURL = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    protocol::Network::Initiator* getInitiator() { return m_initiator.get(); }
    void setInitiator(std::unique_ptr<protocol::Network::Initiator> value) { m_initiator = std::move(value); }

    protocol::Network::CachedResource* getResource() { return m_resource.get(); }
    void setResource(std::unique_ptr<protocol::Network::CachedResource> value) { m_resource = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<RequestServedFromMemoryCacheNotification> clone() const;

    template<int STATE>
    class RequestServedFromMemoryCacheNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          FrameIdSet = 1 << 2,
          LoaderIdSet = 1 << 3,
          DocumentURLSet = 1 << 4,
          TimestampSet = 1 << 5,
          InitiatorSet = 1 << 6,
          ResourceSet = 1 << 7,
            AllFieldsSet = (RequestIdSet | FrameIdSet | LoaderIdSet | DocumentURLSet | TimestampSet | InitiatorSet | ResourceSet | 0)};


        RequestServedFromMemoryCacheNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        RequestServedFromMemoryCacheNotificationBuilder<STATE | FrameIdSet>& setFrameId(const String& value)
        {
            static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
            m_result->setFrameId(value);
            return castState<FrameIdSet>();
        }

        RequestServedFromMemoryCacheNotificationBuilder<STATE | LoaderIdSet>& setLoaderId(const String& value)
        {
            static_assert(!(STATE & LoaderIdSet), "property loaderId should not be set yet");
            m_result->setLoaderId(value);
            return castState<LoaderIdSet>();
        }

        RequestServedFromMemoryCacheNotificationBuilder<STATE | DocumentURLSet>& setDocumentURL(const String& value)
        {
            static_assert(!(STATE & DocumentURLSet), "property documentURL should not be set yet");
            m_result->setDocumentURL(value);
            return castState<DocumentURLSet>();
        }

        RequestServedFromMemoryCacheNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        RequestServedFromMemoryCacheNotificationBuilder<STATE | InitiatorSet>& setInitiator(std::unique_ptr<protocol::Network::Initiator> value)
        {
            static_assert(!(STATE & InitiatorSet), "property initiator should not be set yet");
            m_result->setInitiator(std::move(value));
            return castState<InitiatorSet>();
        }

        RequestServedFromMemoryCacheNotificationBuilder<STATE | ResourceSet>& setResource(std::unique_ptr<protocol::Network::CachedResource> value)
        {
            static_assert(!(STATE & ResourceSet), "property resource should not be set yet");
            m_result->setResource(std::move(value));
            return castState<ResourceSet>();
        }

        std::unique_ptr<RequestServedFromMemoryCacheNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class RequestServedFromMemoryCacheNotification;
        RequestServedFromMemoryCacheNotificationBuilder() : m_result(new RequestServedFromMemoryCacheNotification()) { }

        template<int STEP> RequestServedFromMemoryCacheNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<RequestServedFromMemoryCacheNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::RequestServedFromMemoryCacheNotification> m_result;
    };

    static RequestServedFromMemoryCacheNotificationBuilder<0> create()
    {
        return RequestServedFromMemoryCacheNotificationBuilder<0>();
    }

private:
    RequestServedFromMemoryCacheNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    String m_frameId;
    String m_loaderId;
    String m_documentURL;
    double m_timestamp;
    std::unique_ptr<protocol::Network::Initiator> m_initiator;
    std::unique_ptr<protocol::Network::CachedResource> m_resource;
};


// Wrapper for notification params
class  WebSocketWillSendHandshakeRequestNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketWillSendHandshakeRequestNotification);
public:
    static std::unique_ptr<WebSocketWillSendHandshakeRequestNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketWillSendHandshakeRequestNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    protocol::Network::WebSocketRequest* getRequest() { return m_request.get(); }
    void setRequest(std::unique_ptr<protocol::Network::WebSocketRequest> value) { m_request = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketWillSendHandshakeRequestNotification> clone() const;

    template<int STATE>
    class WebSocketWillSendHandshakeRequestNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
          RequestSet = 1 << 3,
            AllFieldsSet = (RequestIdSet | TimestampSet | RequestSet | 0)};


        WebSocketWillSendHandshakeRequestNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        WebSocketWillSendHandshakeRequestNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        WebSocketWillSendHandshakeRequestNotificationBuilder<STATE | RequestSet>& setRequest(std::unique_ptr<protocol::Network::WebSocketRequest> value)
        {
            static_assert(!(STATE & RequestSet), "property request should not be set yet");
            m_result->setRequest(std::move(value));
            return castState<RequestSet>();
        }

        std::unique_ptr<WebSocketWillSendHandshakeRequestNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketWillSendHandshakeRequestNotification;
        WebSocketWillSendHandshakeRequestNotificationBuilder() : m_result(new WebSocketWillSendHandshakeRequestNotification()) { }

        template<int STEP> WebSocketWillSendHandshakeRequestNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketWillSendHandshakeRequestNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketWillSendHandshakeRequestNotification> m_result;
    };

    static WebSocketWillSendHandshakeRequestNotificationBuilder<0> create()
    {
        return WebSocketWillSendHandshakeRequestNotificationBuilder<0>();
    }

private:
    WebSocketWillSendHandshakeRequestNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
    std::unique_ptr<protocol::Network::WebSocketRequest> m_request;
};


// Wrapper for notification params
class  WebSocketHandshakeResponseReceivedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketHandshakeResponseReceivedNotification);
public:
    static std::unique_ptr<WebSocketHandshakeResponseReceivedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketHandshakeResponseReceivedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    protocol::Network::WebSocketResponse* getResponse() { return m_response.get(); }
    void setResponse(std::unique_ptr<protocol::Network::WebSocketResponse> value) { m_response = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketHandshakeResponseReceivedNotification> clone() const;

    template<int STATE>
    class WebSocketHandshakeResponseReceivedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
          ResponseSet = 1 << 3,
            AllFieldsSet = (RequestIdSet | TimestampSet | ResponseSet | 0)};


        WebSocketHandshakeResponseReceivedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        WebSocketHandshakeResponseReceivedNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        WebSocketHandshakeResponseReceivedNotificationBuilder<STATE | ResponseSet>& setResponse(std::unique_ptr<protocol::Network::WebSocketResponse> value)
        {
            static_assert(!(STATE & ResponseSet), "property response should not be set yet");
            m_result->setResponse(std::move(value));
            return castState<ResponseSet>();
        }

        std::unique_ptr<WebSocketHandshakeResponseReceivedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketHandshakeResponseReceivedNotification;
        WebSocketHandshakeResponseReceivedNotificationBuilder() : m_result(new WebSocketHandshakeResponseReceivedNotification()) { }

        template<int STEP> WebSocketHandshakeResponseReceivedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketHandshakeResponseReceivedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketHandshakeResponseReceivedNotification> m_result;
    };

    static WebSocketHandshakeResponseReceivedNotificationBuilder<0> create()
    {
        return WebSocketHandshakeResponseReceivedNotificationBuilder<0>();
    }

private:
    WebSocketHandshakeResponseReceivedNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
    std::unique_ptr<protocol::Network::WebSocketResponse> m_response;
};


// Wrapper for notification params
class  WebSocketCreatedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketCreatedNotification);
public:
    static std::unique_ptr<WebSocketCreatedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketCreatedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    String getUrl() { return m_url; }
    void setUrl(const String& value) { m_url = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketCreatedNotification> clone() const;

    template<int STATE>
    class WebSocketCreatedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          UrlSet = 1 << 2,
            AllFieldsSet = (RequestIdSet | UrlSet | 0)};


        WebSocketCreatedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        WebSocketCreatedNotificationBuilder<STATE | UrlSet>& setUrl(const String& value)
        {
            static_assert(!(STATE & UrlSet), "property url should not be set yet");
            m_result->setUrl(value);
            return castState<UrlSet>();
        }

        std::unique_ptr<WebSocketCreatedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketCreatedNotification;
        WebSocketCreatedNotificationBuilder() : m_result(new WebSocketCreatedNotification()) { }

        template<int STEP> WebSocketCreatedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketCreatedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketCreatedNotification> m_result;
    };

    static WebSocketCreatedNotificationBuilder<0> create()
    {
        return WebSocketCreatedNotificationBuilder<0>();
    }

private:
    WebSocketCreatedNotification()
    {
    }

    String m_requestId;
    String m_url;
};


// Wrapper for notification params
class  WebSocketClosedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketClosedNotification);
public:
    static std::unique_ptr<WebSocketClosedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketClosedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketClosedNotification> clone() const;

    template<int STATE>
    class WebSocketClosedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
            AllFieldsSet = (RequestIdSet | TimestampSet | 0)};


        WebSocketClosedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        WebSocketClosedNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        std::unique_ptr<WebSocketClosedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketClosedNotification;
        WebSocketClosedNotificationBuilder() : m_result(new WebSocketClosedNotification()) { }

        template<int STEP> WebSocketClosedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketClosedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketClosedNotification> m_result;
    };

    static WebSocketClosedNotificationBuilder<0> create()
    {
        return WebSocketClosedNotificationBuilder<0>();
    }

private:
    WebSocketClosedNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
};


// Wrapper for notification params
class  WebSocketFrameReceivedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketFrameReceivedNotification);
public:
    static std::unique_ptr<WebSocketFrameReceivedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketFrameReceivedNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    protocol::Network::WebSocketFrame* getResponse() { return m_response.get(); }
    void setResponse(std::unique_ptr<protocol::Network::WebSocketFrame> value) { m_response = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketFrameReceivedNotification> clone() const;

    template<int STATE>
    class WebSocketFrameReceivedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
          ResponseSet = 1 << 3,
            AllFieldsSet = (RequestIdSet | TimestampSet | ResponseSet | 0)};


        WebSocketFrameReceivedNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        WebSocketFrameReceivedNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        WebSocketFrameReceivedNotificationBuilder<STATE | ResponseSet>& setResponse(std::unique_ptr<protocol::Network::WebSocketFrame> value)
        {
            static_assert(!(STATE & ResponseSet), "property response should not be set yet");
            m_result->setResponse(std::move(value));
            return castState<ResponseSet>();
        }

        std::unique_ptr<WebSocketFrameReceivedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketFrameReceivedNotification;
        WebSocketFrameReceivedNotificationBuilder() : m_result(new WebSocketFrameReceivedNotification()) { }

        template<int STEP> WebSocketFrameReceivedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketFrameReceivedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketFrameReceivedNotification> m_result;
    };

    static WebSocketFrameReceivedNotificationBuilder<0> create()
    {
        return WebSocketFrameReceivedNotificationBuilder<0>();
    }

private:
    WebSocketFrameReceivedNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
    std::unique_ptr<protocol::Network::WebSocketFrame> m_response;
};


// Wrapper for notification params
class  WebSocketFrameErrorNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketFrameErrorNotification);
public:
    static std::unique_ptr<WebSocketFrameErrorNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketFrameErrorNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    String getErrorMessage() { return m_errorMessage; }
    void setErrorMessage(const String& value) { m_errorMessage = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketFrameErrorNotification> clone() const;

    template<int STATE>
    class WebSocketFrameErrorNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
          ErrorMessageSet = 1 << 3,
            AllFieldsSet = (RequestIdSet | TimestampSet | ErrorMessageSet | 0)};


        WebSocketFrameErrorNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        WebSocketFrameErrorNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        WebSocketFrameErrorNotificationBuilder<STATE | ErrorMessageSet>& setErrorMessage(const String& value)
        {
            static_assert(!(STATE & ErrorMessageSet), "property errorMessage should not be set yet");
            m_result->setErrorMessage(value);
            return castState<ErrorMessageSet>();
        }

        std::unique_ptr<WebSocketFrameErrorNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketFrameErrorNotification;
        WebSocketFrameErrorNotificationBuilder() : m_result(new WebSocketFrameErrorNotification()) { }

        template<int STEP> WebSocketFrameErrorNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketFrameErrorNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketFrameErrorNotification> m_result;
    };

    static WebSocketFrameErrorNotificationBuilder<0> create()
    {
        return WebSocketFrameErrorNotificationBuilder<0>();
    }

private:
    WebSocketFrameErrorNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
    String m_errorMessage;
};


// Wrapper for notification params
class  WebSocketFrameSentNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(WebSocketFrameSentNotification);
public:
    static std::unique_ptr<WebSocketFrameSentNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~WebSocketFrameSentNotification() override { }

    String getRequestId() { return m_requestId; }
    void setRequestId(const String& value) { m_requestId = value; }

    double getTimestamp() { return m_timestamp; }
    void setTimestamp(double value) { m_timestamp = value; }

    protocol::Network::WebSocketFrame* getResponse() { return m_response.get(); }
    void setResponse(std::unique_ptr<protocol::Network::WebSocketFrame> value) { m_response = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<WebSocketFrameSentNotification> clone() const;

    template<int STATE>
    class WebSocketFrameSentNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          RequestIdSet = 1 << 1,
          TimestampSet = 1 << 2,
          ResponseSet = 1 << 3,
            AllFieldsSet = (RequestIdSet | TimestampSet | ResponseSet | 0)};


        WebSocketFrameSentNotificationBuilder<STATE | RequestIdSet>& setRequestId(const String& value)
        {
            static_assert(!(STATE & RequestIdSet), "property requestId should not be set yet");
            m_result->setRequestId(value);
            return castState<RequestIdSet>();
        }

        WebSocketFrameSentNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value)
        {
            static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
            m_result->setTimestamp(value);
            return castState<TimestampSet>();
        }

        WebSocketFrameSentNotificationBuilder<STATE | ResponseSet>& setResponse(std::unique_ptr<protocol::Network::WebSocketFrame> value)
        {
            static_assert(!(STATE & ResponseSet), "property response should not be set yet");
            m_result->setResponse(std::move(value));
            return castState<ResponseSet>();
        }

        std::unique_ptr<WebSocketFrameSentNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class WebSocketFrameSentNotification;
        WebSocketFrameSentNotificationBuilder() : m_result(new WebSocketFrameSentNotification()) { }

        template<int STEP> WebSocketFrameSentNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<WebSocketFrameSentNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Network::WebSocketFrameSentNotification> m_result;
    };

    static WebSocketFrameSentNotificationBuilder<0> create()
    {
        return WebSocketFrameSentNotificationBuilder<0>();
    }

private:
    WebSocketFrameSentNotification()
    {
          m_timestamp = 0;
    }

    String m_requestId;
    double m_timestamp;
    std::unique_ptr<protocol::Network::WebSocketFrame> m_response;
};


// ------------- Backend interface.

class  Backend {
public:
    virtual ~Backend() { }

    virtual DispatchResponse enable() = 0;
    virtual DispatchResponse disable() = 0;
    virtual DispatchResponse setExtraHTTPHeaders(std::unique_ptr<protocol::Network::Headers> in_headers) = 0;
    virtual DispatchResponse getResponseBody(const String& in_requestId, String* out_body, bool* out_base64Encoded) = 0;
    virtual DispatchResponse setCacheDisabled(bool in_cacheDisabled) = 0;
    virtual DispatchResponse loadResource(const String& in_frameId, const String& in_url, String* out_content, String* out_mimeType, double* out_status) = 0;

};

// ------------- Frontend interface.

class  Frontend {
public:
    explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
    void requestWillBeSent(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, std::unique_ptr<protocol::Network::Request> request, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, Maybe<protocol::Network::Response> redirectResponse = Maybe<protocol::Network::Response>(), Maybe<String> type = Maybe<String>());
    void requestServedFromCache(const String& requestId);
    void responseReceived(const String& requestId, const String& frameId, const String& loaderId, double timestamp, const String& type, std::unique_ptr<protocol::Network::Response> response);
    void dataReceived(const String& requestId, double timestamp, int dataLength, int encodedDataLength);
    void loadingFinished(const String& requestId, double timestamp, Maybe<String> sourceMapURL = Maybe<String>());
    void loadingFailed(const String& requestId, double timestamp, const String& errorText, Maybe<bool> canceled = Maybe<bool>());
    void requestServedFromMemoryCache(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, std::unique_ptr<protocol::Network::CachedResource> resource);
    void webSocketWillSendHandshakeRequest(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketRequest> request);
    void webSocketHandshakeResponseReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketResponse> response);
    void webSocketCreated(const String& requestId, const String& url);
    void webSocketClosed(const String& requestId, double timestamp);
    void webSocketFrameReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response);
    void webSocketFrameError(const String& requestId, double timestamp, const String& errorMessage);
    void webSocketFrameSent(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response);

    void flush();
    void sendRawNotification(const String&);
private:
    FrontendChannel* m_frontendChannel;
};

// ------------- Dispatcher.

class  Dispatcher {
public:
    static void wire(UberDispatcher*, Backend*);

private:
    Dispatcher() { }
};

// ------------- Metainfo.

class  Metainfo {
public:
    using BackendClass = Backend;
    using FrontendClass = Frontend;
    using DispatcherClass = Dispatcher;
    static const char domainName[];
    static const char commandPrefix[];
    static const char version[];
};

} // namespace Network
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Network_h)

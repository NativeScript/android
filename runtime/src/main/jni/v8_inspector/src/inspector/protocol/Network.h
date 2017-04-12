// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Network_h
#define v8_inspector_protocol_Network_h

#include "src/inspector/protocol/Protocol.h"
#include "Runtime.h"
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

// ------------- Type and builder declarations.

// Timing information for the request.
class  ResourceTiming {
        PROTOCOL_DISALLOW_COPY(ResourceTiming);
    public:
        static std::unique_ptr<ResourceTiming> parse(protocol::Value* value, ErrorSupport* errors);

        ~ResourceTiming() { }

        double getStartTime() {
            return m_startTime;
        }
        void setStartTime(double value) {
            m_startTime = value;
        }

        double getDomainLookupStart() {
            return m_domainLookupStart;
        }
        void setDomainLookupStart(double value) {
            m_domainLookupStart = value;
        }

        double getDomainLookupEnd() {
            return m_domainLookupEnd;
        }
        void setDomainLookupEnd(double value) {
            m_domainLookupEnd = value;
        }

        double getConnectStart() {
            return m_connectStart;
        }
        void setConnectStart(double value) {
            m_connectStart = value;
        }

        double getConnectEnd() {
            return m_connectEnd;
        }
        void setConnectEnd(double value) {
            m_connectEnd = value;
        }

        double getSecureConnectionStart() {
            return m_secureConnectionStart;
        }
        void setSecureConnectionStart(double value) {
            m_secureConnectionStart = value;
        }

        double getRequestStart() {
            return m_requestStart;
        }
        void setRequestStart(double value) {
            m_requestStart = value;
        }

        double getResponseStart() {
            return m_responseStart;
        }
        void setResponseStart(double value) {
            m_responseStart = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
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
                    AllFieldsSet = (StartTimeSet | DomainLookupStartSet | DomainLookupEndSet | ConnectStartSet | ConnectEndSet | SecureConnectionStartSet | RequestStartSet | ResponseStartSet | 0)
                };


                ResourceTimingBuilder<STATE | StartTimeSet>& setStartTime(double value) {
                    static_assert(!(STATE & StartTimeSet), "property startTime should not be set yet");
                    m_result->setStartTime(value);
                    return castState<StartTimeSet>();
                }

                ResourceTimingBuilder<STATE | DomainLookupStartSet>& setDomainLookupStart(double value) {
                    static_assert(!(STATE & DomainLookupStartSet), "property domainLookupStart should not be set yet");
                    m_result->setDomainLookupStart(value);
                    return castState<DomainLookupStartSet>();
                }

                ResourceTimingBuilder<STATE | DomainLookupEndSet>& setDomainLookupEnd(double value) {
                    static_assert(!(STATE & DomainLookupEndSet), "property domainLookupEnd should not be set yet");
                    m_result->setDomainLookupEnd(value);
                    return castState<DomainLookupEndSet>();
                }

                ResourceTimingBuilder<STATE | ConnectStartSet>& setConnectStart(double value) {
                    static_assert(!(STATE & ConnectStartSet), "property connectStart should not be set yet");
                    m_result->setConnectStart(value);
                    return castState<ConnectStartSet>();
                }

                ResourceTimingBuilder<STATE | ConnectEndSet>& setConnectEnd(double value) {
                    static_assert(!(STATE & ConnectEndSet), "property connectEnd should not be set yet");
                    m_result->setConnectEnd(value);
                    return castState<ConnectEndSet>();
                }

                ResourceTimingBuilder<STATE | SecureConnectionStartSet>& setSecureConnectionStart(double value) {
                    static_assert(!(STATE & SecureConnectionStartSet), "property secureConnectionStart should not be set yet");
                    m_result->setSecureConnectionStart(value);
                    return castState<SecureConnectionStartSet>();
                }

                ResourceTimingBuilder<STATE | RequestStartSet>& setRequestStart(double value) {
                    static_assert(!(STATE & RequestStartSet), "property requestStart should not be set yet");
                    m_result->setRequestStart(value);
                    return castState<RequestStartSet>();
                }

                ResourceTimingBuilder<STATE | ResponseStartSet>& setResponseStart(double value) {
                    static_assert(!(STATE & ResponseStartSet), "property responseStart should not be set yet");
                    m_result->setResponseStart(value);
                    return castState<ResponseStartSet>();
                }

                std::unique_ptr<ResourceTiming> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ResourceTiming;
                ResourceTimingBuilder() : m_result(new ResourceTiming()) { }

                template<int STEP> ResourceTimingBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ResourceTimingBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::ResourceTiming> m_result;
        };

        static ResourceTimingBuilder<0> create() {
            return ResourceTimingBuilder<0>();
        }

    private:
        ResourceTiming() {
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
class  Request {
        PROTOCOL_DISALLOW_COPY(Request);
    public:
        static std::unique_ptr<Request> parse(protocol::Value* value, ErrorSupport* errors);

        ~Request() { }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        String getMethod() {
            return m_method;
        }
        void setMethod(const String& value) {
            m_method = value;
        }

        protocol::Network::Headers* getHeaders() {
            return m_headers.get();
        }
        void setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
            m_headers = std::move(value);
        }

        bool hasPostData() {
            return m_postData.isJust();
        }
        String getPostData(const String& defaultValue) {
            return m_postData.isJust() ? m_postData.fromJust() : defaultValue;
        }
        void setPostData(const String& value) {
            m_postData = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<Request> clone() const;

        template<int STATE>
        class RequestBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    UrlSet = 1 << 1,
                    MethodSet = 1 << 2,
                    HeadersSet = 1 << 3,
                    AllFieldsSet = (UrlSet | MethodSet | HeadersSet | 0)
                };


                RequestBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                RequestBuilder<STATE | MethodSet>& setMethod(const String& value) {
                    static_assert(!(STATE & MethodSet), "property method should not be set yet");
                    m_result->setMethod(value);
                    return castState<MethodSet>();
                }

                RequestBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
                    static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
                    m_result->setHeaders(std::move(value));
                    return castState<HeadersSet>();
                }

                RequestBuilder<STATE>& setPostData(const String& value) {
                    m_result->setPostData(value);
                    return *this;
                }

                std::unique_ptr<Request> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Request;
                RequestBuilder() : m_result(new Request()) { }

                template<int STEP> RequestBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<RequestBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::Request> m_result;
        };

        static RequestBuilder<0> create() {
            return RequestBuilder<0>();
        }

    private:
        Request() {
        }

        String m_url;
        String m_method;
        std::unique_ptr<protocol::Network::Headers> m_headers;
        Maybe<String> m_postData;
};


// HTTP response data.
class  Response {
        PROTOCOL_DISALLOW_COPY(Response);
    public:
        static std::unique_ptr<Response> parse(protocol::Value* value, ErrorSupport* errors);

        ~Response() { }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        double getStatus() {
            return m_status;
        }
        void setStatus(double value) {
            m_status = value;
        }

        String getStatusText() {
            return m_statusText;
        }
        void setStatusText(const String& value) {
            m_statusText = value;
        }

        protocol::Network::Headers* getHeaders() {
            return m_headers.get();
        }
        void setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
            m_headers = std::move(value);
        }

        bool hasHeadersText() {
            return m_headersText.isJust();
        }
        String getHeadersText(const String& defaultValue) {
            return m_headersText.isJust() ? m_headersText.fromJust() : defaultValue;
        }
        void setHeadersText(const String& value) {
            m_headersText = value;
        }

        String getMimeType() {
            return m_mimeType;
        }
        void setMimeType(const String& value) {
            m_mimeType = value;
        }

        bool hasRequestHeaders() {
            return m_requestHeaders.isJust();
        }
        protocol::Network::Headers* getRequestHeaders(protocol::Network::Headers* defaultValue) {
            return m_requestHeaders.isJust() ? m_requestHeaders.fromJust() : defaultValue;
        }
        void setRequestHeaders(std::unique_ptr<protocol::Network::Headers> value) {
            m_requestHeaders = std::move(value);
        }

        bool hasRequestHeadersText() {
            return m_requestHeadersText.isJust();
        }
        String getRequestHeadersText(const String& defaultValue) {
            return m_requestHeadersText.isJust() ? m_requestHeadersText.fromJust() : defaultValue;
        }
        void setRequestHeadersText(const String& value) {
            m_requestHeadersText = value;
        }

        bool hasFromDiskCache() {
            return m_fromDiskCache.isJust();
        }
        bool getFromDiskCache(bool defaultValue) {
            return m_fromDiskCache.isJust() ? m_fromDiskCache.fromJust() : defaultValue;
        }
        void setFromDiskCache(bool value) {
            m_fromDiskCache = value;
        }

        bool hasTiming() {
            return m_timing.isJust();
        }
        protocol::Network::ResourceTiming* getTiming(protocol::Network::ResourceTiming* defaultValue) {
            return m_timing.isJust() ? m_timing.fromJust() : defaultValue;
        }
        void setTiming(std::unique_ptr<protocol::Network::ResourceTiming> value) {
            m_timing = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
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
                    AllFieldsSet = (UrlSet | StatusSet | StatusTextSet | HeadersSet | MimeTypeSet | 0)
                };


                ResponseBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                ResponseBuilder<STATE | StatusSet>& setStatus(double value) {
                    static_assert(!(STATE & StatusSet), "property status should not be set yet");
                    m_result->setStatus(value);
                    return castState<StatusSet>();
                }

                ResponseBuilder<STATE | StatusTextSet>& setStatusText(const String& value) {
                    static_assert(!(STATE & StatusTextSet), "property statusText should not be set yet");
                    m_result->setStatusText(value);
                    return castState<StatusTextSet>();
                }

                ResponseBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
                    static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
                    m_result->setHeaders(std::move(value));
                    return castState<HeadersSet>();
                }

                ResponseBuilder<STATE>& setHeadersText(const String& value) {
                    m_result->setHeadersText(value);
                    return *this;
                }

                ResponseBuilder<STATE | MimeTypeSet>& setMimeType(const String& value) {
                    static_assert(!(STATE & MimeTypeSet), "property mimeType should not be set yet");
                    m_result->setMimeType(value);
                    return castState<MimeTypeSet>();
                }

                ResponseBuilder<STATE>& setRequestHeaders(std::unique_ptr<protocol::Network::Headers> value) {
                    m_result->setRequestHeaders(std::move(value));
                    return *this;
                }

                ResponseBuilder<STATE>& setRequestHeadersText(const String& value) {
                    m_result->setRequestHeadersText(value);
                    return *this;
                }

                ResponseBuilder<STATE>& setFromDiskCache(bool value) {
                    m_result->setFromDiskCache(value);
                    return *this;
                }

                ResponseBuilder<STATE>& setTiming(std::unique_ptr<protocol::Network::ResourceTiming> value) {
                    m_result->setTiming(std::move(value));
                    return *this;
                }

                std::unique_ptr<Response> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Response;
                ResponseBuilder() : m_result(new Response()) { }

                template<int STEP> ResponseBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ResponseBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::Response> m_result;
        };

        static ResponseBuilder<0> create() {
            return ResponseBuilder<0>();
        }

    private:
        Response() {
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
class  WebSocketRequest {
        PROTOCOL_DISALLOW_COPY(WebSocketRequest);
    public:
        static std::unique_ptr<WebSocketRequest> parse(protocol::Value* value, ErrorSupport* errors);

        ~WebSocketRequest() { }

        protocol::Network::Headers* getHeaders() {
            return m_headers.get();
        }
        void setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
            m_headers = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<WebSocketRequest> clone() const;

        template<int STATE>
        class WebSocketRequestBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    HeadersSet = 1 << 1,
                    AllFieldsSet = (HeadersSet | 0)
                };


                WebSocketRequestBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
                    static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
                    m_result->setHeaders(std::move(value));
                    return castState<HeadersSet>();
                }

                std::unique_ptr<WebSocketRequest> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class WebSocketRequest;
                WebSocketRequestBuilder() : m_result(new WebSocketRequest()) { }

                template<int STEP> WebSocketRequestBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<WebSocketRequestBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::WebSocketRequest> m_result;
        };

        static WebSocketRequestBuilder<0> create() {
            return WebSocketRequestBuilder<0>();
        }

    private:
        WebSocketRequest() {
        }

        std::unique_ptr<protocol::Network::Headers> m_headers;
};


// WebSocket response data.
class  WebSocketResponse {
        PROTOCOL_DISALLOW_COPY(WebSocketResponse);
    public:
        static std::unique_ptr<WebSocketResponse> parse(protocol::Value* value, ErrorSupport* errors);

        ~WebSocketResponse() { }

        double getStatus() {
            return m_status;
        }
        void setStatus(double value) {
            m_status = value;
        }

        String getStatusText() {
            return m_statusText;
        }
        void setStatusText(const String& value) {
            m_statusText = value;
        }

        protocol::Network::Headers* getHeaders() {
            return m_headers.get();
        }
        void setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
            m_headers = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<WebSocketResponse> clone() const;

        template<int STATE>
        class WebSocketResponseBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    StatusSet = 1 << 1,
                    StatusTextSet = 1 << 2,
                    HeadersSet = 1 << 3,
                    AllFieldsSet = (StatusSet | StatusTextSet | HeadersSet | 0)
                };


                WebSocketResponseBuilder<STATE | StatusSet>& setStatus(double value) {
                    static_assert(!(STATE & StatusSet), "property status should not be set yet");
                    m_result->setStatus(value);
                    return castState<StatusSet>();
                }

                WebSocketResponseBuilder<STATE | StatusTextSet>& setStatusText(const String& value) {
                    static_assert(!(STATE & StatusTextSet), "property statusText should not be set yet");
                    m_result->setStatusText(value);
                    return castState<StatusTextSet>();
                }

                WebSocketResponseBuilder<STATE | HeadersSet>& setHeaders(std::unique_ptr<protocol::Network::Headers> value) {
                    static_assert(!(STATE & HeadersSet), "property headers should not be set yet");
                    m_result->setHeaders(std::move(value));
                    return castState<HeadersSet>();
                }

                std::unique_ptr<WebSocketResponse> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class WebSocketResponse;
                WebSocketResponseBuilder() : m_result(new WebSocketResponse()) { }

                template<int STEP> WebSocketResponseBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<WebSocketResponseBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::WebSocketResponse> m_result;
        };

        static WebSocketResponseBuilder<0> create() {
            return WebSocketResponseBuilder<0>();
        }

    private:
        WebSocketResponse() {
            m_status = 0;
        }

        double m_status;
        String m_statusText;
        std::unique_ptr<protocol::Network::Headers> m_headers;
};


// WebSocket frame data.
class  WebSocketFrame {
        PROTOCOL_DISALLOW_COPY(WebSocketFrame);
    public:
        static std::unique_ptr<WebSocketFrame> parse(protocol::Value* value, ErrorSupport* errors);

        ~WebSocketFrame() { }

        double getOpcode() {
            return m_opcode;
        }
        void setOpcode(double value) {
            m_opcode = value;
        }

        bool getMask() {
            return m_mask;
        }
        void setMask(bool value) {
            m_mask = value;
        }

        String getPayloadData() {
            return m_payloadData;
        }
        void setPayloadData(const String& value) {
            m_payloadData = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<WebSocketFrame> clone() const;

        template<int STATE>
        class WebSocketFrameBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    OpcodeSet = 1 << 1,
                    MaskSet = 1 << 2,
                    PayloadDataSet = 1 << 3,
                    AllFieldsSet = (OpcodeSet | MaskSet | PayloadDataSet | 0)
                };


                WebSocketFrameBuilder<STATE | OpcodeSet>& setOpcode(double value) {
                    static_assert(!(STATE & OpcodeSet), "property opcode should not be set yet");
                    m_result->setOpcode(value);
                    return castState<OpcodeSet>();
                }

                WebSocketFrameBuilder<STATE | MaskSet>& setMask(bool value) {
                    static_assert(!(STATE & MaskSet), "property mask should not be set yet");
                    m_result->setMask(value);
                    return castState<MaskSet>();
                }

                WebSocketFrameBuilder<STATE | PayloadDataSet>& setPayloadData(const String& value) {
                    static_assert(!(STATE & PayloadDataSet), "property payloadData should not be set yet");
                    m_result->setPayloadData(value);
                    return castState<PayloadDataSet>();
                }

                std::unique_ptr<WebSocketFrame> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class WebSocketFrame;
                WebSocketFrameBuilder() : m_result(new WebSocketFrame()) { }

                template<int STEP> WebSocketFrameBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<WebSocketFrameBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::WebSocketFrame> m_result;
        };

        static WebSocketFrameBuilder<0> create() {
            return WebSocketFrameBuilder<0>();
        }

    private:
        WebSocketFrame() {
            m_opcode = 0;
            m_mask = false;
        }

        double m_opcode;
        bool m_mask;
        String m_payloadData;
};


// Information about the cached resource.
class  CachedResource {
        PROTOCOL_DISALLOW_COPY(CachedResource);
    public:
        static std::unique_ptr<CachedResource> parse(protocol::Value* value, ErrorSupport* errors);

        ~CachedResource() { }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        String getType() {
            return m_type;
        }
        void setType(const String& value) {
            m_type = value;
        }

        bool hasResponse() {
            return m_response.isJust();
        }
        protocol::Network::Response* getResponse(protocol::Network::Response* defaultValue) {
            return m_response.isJust() ? m_response.fromJust() : defaultValue;
        }
        void setResponse(std::unique_ptr<protocol::Network::Response> value) {
            m_response = std::move(value);
        }

        double getBodySize() {
            return m_bodySize;
        }
        void setBodySize(double value) {
            m_bodySize = value;
        }

        bool hasSourceMapURL() {
            return m_sourceMapURL.isJust();
        }
        String getSourceMapURL(const String& defaultValue) {
            return m_sourceMapURL.isJust() ? m_sourceMapURL.fromJust() : defaultValue;
        }
        void setSourceMapURL(const String& value) {
            m_sourceMapURL = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<CachedResource> clone() const;

        template<int STATE>
        class CachedResourceBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    UrlSet = 1 << 1,
                    TypeSet = 1 << 2,
                    BodySizeSet = 1 << 3,
                    AllFieldsSet = (UrlSet | TypeSet | BodySizeSet | 0)
                };


                CachedResourceBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                CachedResourceBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                CachedResourceBuilder<STATE>& setResponse(std::unique_ptr<protocol::Network::Response> value) {
                    m_result->setResponse(std::move(value));
                    return *this;
                }

                CachedResourceBuilder<STATE | BodySizeSet>& setBodySize(double value) {
                    static_assert(!(STATE & BodySizeSet), "property bodySize should not be set yet");
                    m_result->setBodySize(value);
                    return castState<BodySizeSet>();
                }

                CachedResourceBuilder<STATE>& setSourceMapURL(const String& value) {
                    m_result->setSourceMapURL(value);
                    return *this;
                }

                std::unique_ptr<CachedResource> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CachedResource;
                CachedResourceBuilder() : m_result(new CachedResource()) { }

                template<int STEP> CachedResourceBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CachedResourceBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::CachedResource> m_result;
        };

        static CachedResourceBuilder<0> create() {
            return CachedResourceBuilder<0>();
        }

    private:
        CachedResource() {
            m_bodySize = 0;
        }

        String m_url;
        String m_type;
        Maybe<protocol::Network::Response> m_response;
        double m_bodySize;
        Maybe<String> m_sourceMapURL;
};


// Information about the request initiator.
class  Initiator {
        PROTOCOL_DISALLOW_COPY(Initiator);
    public:
        static std::unique_ptr<Initiator> parse(protocol::Value* value, ErrorSupport* errors);

        ~Initiator() { }

        struct  TypeEnum {
            static const char* Parser;
            static const char* Script;
            static const char* Other;
        }; // TypeEnum

        String getType() {
            return m_type;
        }
        void setType(const String& value) {
            m_type = value;
        }

        bool hasStackTrace() {
            return m_stackTrace.isJust();
        }
        protocol::Runtime::StackTrace* getStackTrace(protocol::Runtime::StackTrace* defaultValue) {
            return m_stackTrace.isJust() ? m_stackTrace.fromJust() : defaultValue;
        }
        void setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
            m_stackTrace = std::move(value);
        }

        bool hasUrl() {
            return m_url.isJust();
        }
        String getUrl(const String& defaultValue) {
            return m_url.isJust() ? m_url.fromJust() : defaultValue;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        bool hasLineNumber() {
            return m_lineNumber.isJust();
        }
        double getLineNumber(double defaultValue) {
            return m_lineNumber.isJust() ? m_lineNumber.fromJust() : defaultValue;
        }
        void setLineNumber(double value) {
            m_lineNumber = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<Initiator> clone() const;

        template<int STATE>
        class InitiatorBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TypeSet = 1 << 1,
                    AllFieldsSet = (TypeSet | 0)
                };


                InitiatorBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                InitiatorBuilder<STATE>& setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setStackTrace(std::move(value));
                    return *this;
                }

                InitiatorBuilder<STATE>& setUrl(const String& value) {
                    m_result->setUrl(value);
                    return *this;
                }

                InitiatorBuilder<STATE>& setLineNumber(double value) {
                    m_result->setLineNumber(value);
                    return *this;
                }

                std::unique_ptr<Initiator> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Initiator;
                InitiatorBuilder() : m_result(new Initiator()) { }

                template<int STEP> InitiatorBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<InitiatorBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Network::Initiator> m_result;
        };

        static InitiatorBuilder<0> create() {
            return InitiatorBuilder<0>();
        }

    private:
        Initiator() {
        }

        String m_type;
        Maybe<protocol::Runtime::StackTrace> m_stackTrace;
        Maybe<String> m_url;
        Maybe<double> m_lineNumber;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual void enable(ErrorString*) = 0;
        virtual void disable(ErrorString*) = 0;
        virtual void setExtraHTTPHeaders(ErrorString*, std::unique_ptr<protocol::Network::Headers> in_headers) = 0;
        virtual void getResponseBody(ErrorString*, const String& in_requestId, String* out_body, bool* out_base64Encoded) = 0;
        virtual void setCacheDisabled(ErrorString*, bool in_cacheDisabled) = 0;
        class  LoadResourceCallback : public BackendCallback {
            public:
                virtual void sendSuccess(const String& content, const String& mimeType, double status) = 0;
        };
        virtual void loadResource(const String& in_frameId, const String& in_url, std::unique_ptr<LoadResourceCallback> callback) = 0;
};

// ------------- Frontend interface.

class  Frontend {
    public:
        Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void requestWillBeSent(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, std::unique_ptr<protocol::Network::Request> request, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, const Maybe<protocol::Network::Response>& redirectResponse = Maybe<protocol::Network::Response>(), const Maybe<String>& type = Maybe<String>());
        void requestServedFromCache(const String& requestId);
        void responseReceived(const String& requestId, const String& frameId, const String& loaderId, double timestamp, const String& type, std::unique_ptr<protocol::Network::Response> response);
        void dataReceived(const String& requestId, double timestamp, int dataLength, int encodedDataLength);
        void loadingFinished(const String& requestId, double timestamp, const Maybe<String>& sourceMapURL = Maybe<String>());
        void loadingFailed(const String& requestId, double timestamp, const String& errorText, const Maybe<bool>& canceled = Maybe<bool>());
        void requestServedFromMemoryCache(const String& requestId, const String& frameId, const String& loaderId, const String& documentURL, double timestamp, std::unique_ptr<protocol::Network::Initiator> initiator, std::unique_ptr<protocol::Network::CachedResource> resource);
        void webSocketWillSendHandshakeRequest(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketRequest> request);
        void webSocketHandshakeResponseReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketResponse> response);
        void webSocketCreated(const String& requestId, const String& url);
        void webSocketClosed(const String& requestId, double timestamp);
        void webSocketFrameReceived(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response);
        void webSocketFrameError(const String& requestId, double timestamp, const String& errorMessage);
        void webSocketFrameSent(const String& requestId, double timestamp, std::unique_ptr<protocol::Network::WebSocketFrame> response);

        void flush();
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

// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Page_h
#define v8_inspector_protocol_Page_h

#include "src/inspector/protocol/Protocol.h"
#include "GenericTypes.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.

namespace v8_inspector {
namespace protocol {
namespace Page {

// ------------- Forward and enum declarations.
// Resource type as it was perceived by the rendering engine.
using ResourceType = String;
// Coordinate system used by supplied coordinates.
using CoordinateSystem = String;
// Information about the Frame on the page.
class Frame;
//
class FrameResource;
// Information about the Frame hierarchy along with their cached resources.
class FrameResourceTree;
// Search result for resource.
class SearchResult;
// Unique script identifier.
using ScriptIdentifier = String;

namespace ResourceTypeEnum {
extern const char* Document;
extern const char* Stylesheet;
extern const char* Image;
extern const char* Font;
extern const char* Script;
extern const char* XHR;
extern const char* WebSocket;
extern const char* Other;
} // namespace ResourceTypeEnum

namespace CoordinateSystemEnum {
extern const char* Viewport;
extern const char* Page;
} // namespace CoordinateSystemEnum

// ------------- Type and builder declarations.

// Information about the Frame on the page.
class  Frame {
        PROTOCOL_DISALLOW_COPY(Frame);
    public:
        static std::unique_ptr<Frame> parse(protocol::Value* value, ErrorSupport* errors);

        ~Frame() { }

        String getId() {
            return m_id;
        }
        void setId(const String& value) {
            m_id = value;
        }

        bool hasParentId() {
            return m_parentId.isJust();
        }
        String getParentId(const String& defaultValue) {
            return m_parentId.isJust() ? m_parentId.fromJust() : defaultValue;
        }
        void setParentId(const String& value) {
            m_parentId = value;
        }

        String getLoaderId() {
            return m_loaderId;
        }
        void setLoaderId(const String& value) {
            m_loaderId = value;
        }

        bool hasName() {
            return m_name.isJust();
        }
        String getName(const String& defaultValue) {
            return m_name.isJust() ? m_name.fromJust() : defaultValue;
        }
        void setName(const String& value) {
            m_name = value;
        }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        String getSecurityOrigin() {
            return m_securityOrigin;
        }
        void setSecurityOrigin(const String& value) {
            m_securityOrigin = value;
        }

        String getMimeType() {
            return m_mimeType;
        }
        void setMimeType(const String& value) {
            m_mimeType = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<Frame> clone() const;

        template<int STATE>
        class FrameBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    IdSet = 1 << 1,
                    LoaderIdSet = 1 << 2,
                    UrlSet = 1 << 3,
                    SecurityOriginSet = 1 << 4,
                    MimeTypeSet = 1 << 5,
                    AllFieldsSet = (IdSet | LoaderIdSet | UrlSet | SecurityOriginSet | MimeTypeSet | 0)
                };


                FrameBuilder<STATE | IdSet>& setId(const String& value) {
                    static_assert(!(STATE & IdSet), "property id should not be set yet");
                    m_result->setId(value);
                    return castState<IdSet>();
                }

                FrameBuilder<STATE>& setParentId(const String& value) {
                    m_result->setParentId(value);
                    return *this;
                }

                FrameBuilder<STATE | LoaderIdSet>& setLoaderId(const String& value) {
                    static_assert(!(STATE & LoaderIdSet), "property loaderId should not be set yet");
                    m_result->setLoaderId(value);
                    return castState<LoaderIdSet>();
                }

                FrameBuilder<STATE>& setName(const String& value) {
                    m_result->setName(value);
                    return *this;
                }

                FrameBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                FrameBuilder<STATE | SecurityOriginSet>& setSecurityOrigin(const String& value) {
                    static_assert(!(STATE & SecurityOriginSet), "property securityOrigin should not be set yet");
                    m_result->setSecurityOrigin(value);
                    return castState<SecurityOriginSet>();
                }

                FrameBuilder<STATE | MimeTypeSet>& setMimeType(const String& value) {
                    static_assert(!(STATE & MimeTypeSet), "property mimeType should not be set yet");
                    m_result->setMimeType(value);
                    return castState<MimeTypeSet>();
                }

                std::unique_ptr<Frame> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Frame;
                FrameBuilder() : m_result(new Frame()) { }

                template<int STEP> FrameBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FrameBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::Frame> m_result;
        };

        static FrameBuilder<0> create() {
            return FrameBuilder<0>();
        }

    private:
        Frame() {
        }

        String m_id;
        Maybe<String> m_parentId;
        String m_loaderId;
        Maybe<String> m_name;
        String m_url;
        String m_securityOrigin;
        String m_mimeType;
};


//
class  FrameResource {
        PROTOCOL_DISALLOW_COPY(FrameResource);
    public:
        static std::unique_ptr<FrameResource> parse(protocol::Value* value, ErrorSupport* errors);

        ~FrameResource() { }

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

        String getMimeType() {
            return m_mimeType;
        }
        void setMimeType(const String& value) {
            m_mimeType = value;
        }

        bool hasFailed() {
            return m_failed.isJust();
        }
        bool getFailed(bool defaultValue) {
            return m_failed.isJust() ? m_failed.fromJust() : defaultValue;
        }
        void setFailed(bool value) {
            m_failed = value;
        }

        bool hasCanceled() {
            return m_canceled.isJust();
        }
        bool getCanceled(bool defaultValue) {
            return m_canceled.isJust() ? m_canceled.fromJust() : defaultValue;
        }
        void setCanceled(bool value) {
            m_canceled = value;
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
        std::unique_ptr<FrameResource> clone() const;

        template<int STATE>
        class FrameResourceBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    UrlSet = 1 << 1,
                    TypeSet = 1 << 2,
                    MimeTypeSet = 1 << 3,
                    AllFieldsSet = (UrlSet | TypeSet | MimeTypeSet | 0)
                };


                FrameResourceBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                FrameResourceBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                FrameResourceBuilder<STATE | MimeTypeSet>& setMimeType(const String& value) {
                    static_assert(!(STATE & MimeTypeSet), "property mimeType should not be set yet");
                    m_result->setMimeType(value);
                    return castState<MimeTypeSet>();
                }

                FrameResourceBuilder<STATE>& setFailed(bool value) {
                    m_result->setFailed(value);
                    return *this;
                }

                FrameResourceBuilder<STATE>& setCanceled(bool value) {
                    m_result->setCanceled(value);
                    return *this;
                }

                FrameResourceBuilder<STATE>& setSourceMapURL(const String& value) {
                    m_result->setSourceMapURL(value);
                    return *this;
                }

                std::unique_ptr<FrameResource> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class FrameResource;
                FrameResourceBuilder() : m_result(new FrameResource()) { }

                template<int STEP> FrameResourceBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FrameResourceBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::FrameResource> m_result;
        };

        static FrameResourceBuilder<0> create() {
            return FrameResourceBuilder<0>();
        }

    private:
        FrameResource() {
        }

        String m_url;
        String m_type;
        String m_mimeType;
        Maybe<bool> m_failed;
        Maybe<bool> m_canceled;
        Maybe<String> m_sourceMapURL;
};


// Information about the Frame hierarchy along with their cached resources.
class  FrameResourceTree {
        PROTOCOL_DISALLOW_COPY(FrameResourceTree);
    public:
        static std::unique_ptr<FrameResourceTree> parse(protocol::Value* value, ErrorSupport* errors);

        ~FrameResourceTree() { }

        protocol::Page::Frame* getFrame() {
            return m_frame.get();
        }
        void setFrame(std::unique_ptr<protocol::Page::Frame> value) {
            m_frame = std::move(value);
        }

        bool hasChildFrames() {
            return m_childFrames.isJust();
        }
        protocol::Array<protocol::Page::FrameResourceTree>* getChildFrames(protocol::Array<protocol::Page::FrameResourceTree>* defaultValue) {
            return m_childFrames.isJust() ? m_childFrames.fromJust() : defaultValue;
        }
        void setChildFrames(std::unique_ptr<protocol::Array<protocol::Page::FrameResourceTree>> value) {
            m_childFrames = std::move(value);
        }

        protocol::Array<protocol::Page::FrameResource>* getResources() {
            return m_resources.get();
        }
        void setResources(std::unique_ptr<protocol::Array<protocol::Page::FrameResource>> value) {
            m_resources = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<FrameResourceTree> clone() const;

        template<int STATE>
        class FrameResourceTreeBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    FrameSet = 1 << 1,
                    ResourcesSet = 1 << 2,
                    AllFieldsSet = (FrameSet | ResourcesSet | 0)
                };


                FrameResourceTreeBuilder<STATE | FrameSet>& setFrame(std::unique_ptr<protocol::Page::Frame> value) {
                    static_assert(!(STATE & FrameSet), "property frame should not be set yet");
                    m_result->setFrame(std::move(value));
                    return castState<FrameSet>();
                }

                FrameResourceTreeBuilder<STATE>& setChildFrames(std::unique_ptr<protocol::Array<protocol::Page::FrameResourceTree>> value) {
                    m_result->setChildFrames(std::move(value));
                    return *this;
                }

                FrameResourceTreeBuilder<STATE | ResourcesSet>& setResources(std::unique_ptr<protocol::Array<protocol::Page::FrameResource>> value) {
                    static_assert(!(STATE & ResourcesSet), "property resources should not be set yet");
                    m_result->setResources(std::move(value));
                    return castState<ResourcesSet>();
                }

                std::unique_ptr<FrameResourceTree> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class FrameResourceTree;
                FrameResourceTreeBuilder() : m_result(new FrameResourceTree()) { }

                template<int STEP> FrameResourceTreeBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FrameResourceTreeBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::FrameResourceTree> m_result;
        };

        static FrameResourceTreeBuilder<0> create() {
            return FrameResourceTreeBuilder<0>();
        }

    private:
        FrameResourceTree() {
        }

        std::unique_ptr<protocol::Page::Frame> m_frame;
        Maybe<protocol::Array<protocol::Page::FrameResourceTree>> m_childFrames;
        std::unique_ptr<protocol::Array<protocol::Page::FrameResource>> m_resources;
};


// Search result for resource.
class  SearchResult {
        PROTOCOL_DISALLOW_COPY(SearchResult);
    public:
        static std::unique_ptr<SearchResult> parse(protocol::Value* value, ErrorSupport* errors);

        ~SearchResult() { }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        String getFrameId() {
            return m_frameId;
        }
        void setFrameId(const String& value) {
            m_frameId = value;
        }

        double getMatchesCount() {
            return m_matchesCount;
        }
        void setMatchesCount(double value) {
            m_matchesCount = value;
        }

        bool hasRequestId() {
            return m_requestId.isJust();
        }
        String getRequestId(const String& defaultValue) {
            return m_requestId.isJust() ? m_requestId.fromJust() : defaultValue;
        }
        void setRequestId(const String& value) {
            m_requestId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<SearchResult> clone() const;

        template<int STATE>
        class SearchResultBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    UrlSet = 1 << 1,
                    FrameIdSet = 1 << 2,
                    MatchesCountSet = 1 << 3,
                    AllFieldsSet = (UrlSet | FrameIdSet | MatchesCountSet | 0)
                };


                SearchResultBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                SearchResultBuilder<STATE | FrameIdSet>& setFrameId(const String& value) {
                    static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
                    m_result->setFrameId(value);
                    return castState<FrameIdSet>();
                }

                SearchResultBuilder<STATE | MatchesCountSet>& setMatchesCount(double value) {
                    static_assert(!(STATE & MatchesCountSet), "property matchesCount should not be set yet");
                    m_result->setMatchesCount(value);
                    return castState<MatchesCountSet>();
                }

                SearchResultBuilder<STATE>& setRequestId(const String& value) {
                    m_result->setRequestId(value);
                    return *this;
                }

                std::unique_ptr<SearchResult> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class SearchResult;
                SearchResultBuilder() : m_result(new SearchResult()) { }

                template<int STEP> SearchResultBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<SearchResultBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::SearchResult> m_result;
        };

        static SearchResultBuilder<0> create() {
            return SearchResultBuilder<0>();
        }

    private:
        SearchResult() {
            m_matchesCount = 0;
        }

        String m_url;
        String m_frameId;
        double m_matchesCount;
        Maybe<String> m_requestId;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual void enable(ErrorString*) = 0;
        virtual void disable(ErrorString*) = 0;
        virtual void addScriptToEvaluateOnLoad(ErrorString*, const String& in_scriptSource, String* out_identifier) = 0;
        virtual void removeScriptToEvaluateOnLoad(ErrorString*, const String& in_identifier) = 0;
        virtual void reload(ErrorString*, const Maybe<bool>& in_ignoreCache, const Maybe<String>& in_scriptToEvaluateOnLoad) = 0;
        virtual void getResourceTree(ErrorString*, std::unique_ptr<protocol::Page::FrameResourceTree>* out_frameTree) = 0;
        virtual void getResourceContent(ErrorString*, const String& in_frameId, const String& in_url, String* out_content, bool* out_base64Encoded) = 0;
        virtual void searchInResource(ErrorString*, const String& in_frameId, const String& in_url, const String& in_query, const Maybe<bool>& in_caseSensitive, const Maybe<bool>& in_isRegex, const Maybe<String>& in_requestId, std::unique_ptr<protocol::Array<protocol::GenericTypes::SearchMatch>>* out_result) = 0;
        virtual void searchInResources(ErrorString*, const String& in_text, const Maybe<bool>& in_caseSensitive, const Maybe<bool>& in_isRegex, std::unique_ptr<protocol::Array<protocol::Page::SearchResult>>* out_result) = 0;
        virtual void setDocumentContent(ErrorString*, const String& in_frameId, const String& in_html) = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void loadEventFired(double timestamp);
        void frameDetached(const String& frameId);
        void frameStartedLoading(const String& frameId);
        void frameStoppedLoading(const String& frameId);

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

} // namespace Page
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Page_h)

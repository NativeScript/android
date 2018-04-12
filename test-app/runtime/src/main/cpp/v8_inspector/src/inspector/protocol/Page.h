// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Page_h
#define v8_inspector_protocol_Page_h

#include "src/inspector/protocol/Protocol.h"
#include "src/inspector/protocol/GenericTypes.h"
#include "src/inspector/protocol/Runtime.h"
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
// Unique frame identifier.
using FrameId = String;
// Viewport for capturing screenshot.
class Viewport;
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
// Wrapper for notification params
class LoadEventFiredNotification;
// Wrapper for notification params
class FrameDetachedNotification;
// Wrapper for notification params
class FrameStartedLoadingNotification;
// Wrapper for notification params
class FrameStoppedLoadingNotification;

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

// Viewport for capturing screenshot.
class  Viewport : public Serializable {
        PROTOCOL_DISALLOW_COPY(Viewport);
    public:
        static std::unique_ptr<Viewport> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~Viewport() override { }

        double getX() {
            return m_x;
        }
        void setX(double value) {
            m_x = value;
        }

        double getY() {
            return m_y;
        }
        void setY(double value) {
            m_y = value;
        }

        double getWidth() {
            return m_width;
        }
        void setWidth(double value) {
            m_width = value;
        }

        double getHeight() {
            return m_height;
        }
        void setHeight(double value) {
            m_height = value;
        }

        double getScale() {
            return m_scale;
        }
        void setScale(double value) {
            m_scale = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<Viewport> clone() const;

        template<int STATE>
        class ViewportBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    XSet = 1 << 1,
                    YSet = 1 << 2,
                    WidthSet = 1 << 3,
                    HeightSet = 1 << 4,
                    ScaleSet = 1 << 5,
                    AllFieldsSet = (XSet | YSet | WidthSet | HeightSet | ScaleSet | 0)
                };


                ViewportBuilder<STATE | XSet>& setX(double value) {
                    static_assert(!(STATE & XSet), "property x should not be set yet");
                    m_result->setX(value);
                    return castState<XSet>();
                }

                ViewportBuilder<STATE | YSet>& setY(double value) {
                    static_assert(!(STATE & YSet), "property y should not be set yet");
                    m_result->setY(value);
                    return castState<YSet>();
                }

                ViewportBuilder<STATE | WidthSet>& setWidth(double value) {
                    static_assert(!(STATE & WidthSet), "property width should not be set yet");
                    m_result->setWidth(value);
                    return castState<WidthSet>();
                }

                ViewportBuilder<STATE | HeightSet>& setHeight(double value) {
                    static_assert(!(STATE & HeightSet), "property height should not be set yet");
                    m_result->setHeight(value);
                    return castState<HeightSet>();
                }

                ViewportBuilder<STATE | ScaleSet>& setScale(double value) {
                    static_assert(!(STATE & ScaleSet), "property scale should not be set yet");
                    m_result->setScale(value);
                    return castState<ScaleSet>();
                }

                std::unique_ptr<Viewport> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Viewport;
                ViewportBuilder() : m_result(new Viewport()) { }

                template<int STEP> ViewportBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ViewportBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::Viewport> m_result;
        };

        static ViewportBuilder<0> create() {
            return ViewportBuilder<0>();
        }

    private:
        Viewport() {
            m_x = 0;
            m_y = 0;
            m_width = 0;
            m_height = 0;
            m_scale = 0;
        }

        double m_x;
        double m_y;
        double m_width;
        double m_height;
        double m_scale;
};


// Information about the Frame on the page.
class  Frame : public Serializable {
        PROTOCOL_DISALLOW_COPY(Frame);
    public:
        static std::unique_ptr<Frame> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~Frame() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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
class  FrameResource : public Serializable {
        PROTOCOL_DISALLOW_COPY(FrameResource);
    public:
        static std::unique_ptr<FrameResource> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~FrameResource() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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
class  FrameResourceTree : public Serializable {
        PROTOCOL_DISALLOW_COPY(FrameResourceTree);
    public:
        static std::unique_ptr<FrameResourceTree> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~FrameResourceTree() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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
class  SearchResult : public Serializable {
        PROTOCOL_DISALLOW_COPY(SearchResult);
    public:
        static std::unique_ptr<SearchResult> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~SearchResult() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


// Wrapper for notification params
class  LoadEventFiredNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(LoadEventFiredNotification);
    public:
        static std::unique_ptr<LoadEventFiredNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~LoadEventFiredNotification() override { }

        double getTimestamp() {
            return m_timestamp;
        }
        void setTimestamp(double value) {
            m_timestamp = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<LoadEventFiredNotification> clone() const;

        template<int STATE>
        class LoadEventFiredNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TimestampSet = 1 << 1,
                    AllFieldsSet = (TimestampSet | 0)
                };


                LoadEventFiredNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value) {
                    static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
                    m_result->setTimestamp(value);
                    return castState<TimestampSet>();
                }

                std::unique_ptr<LoadEventFiredNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class LoadEventFiredNotification;
                LoadEventFiredNotificationBuilder() : m_result(new LoadEventFiredNotification()) { }

                template<int STEP> LoadEventFiredNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<LoadEventFiredNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::LoadEventFiredNotification> m_result;
        };

        static LoadEventFiredNotificationBuilder<0> create() {
            return LoadEventFiredNotificationBuilder<0>();
        }

    private:
        LoadEventFiredNotification() {
            m_timestamp = 0;
        }

        double m_timestamp;
};


// Wrapper for notification params
class  FrameDetachedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(FrameDetachedNotification);
    public:
        static std::unique_ptr<FrameDetachedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~FrameDetachedNotification() override { }

        String getFrameId() {
            return m_frameId;
        }
        void setFrameId(const String& value) {
            m_frameId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<FrameDetachedNotification> clone() const;

        template<int STATE>
        class FrameDetachedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    FrameIdSet = 1 << 1,
                    AllFieldsSet = (FrameIdSet | 0)
                };


                FrameDetachedNotificationBuilder<STATE | FrameIdSet>& setFrameId(const String& value) {
                    static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
                    m_result->setFrameId(value);
                    return castState<FrameIdSet>();
                }

                std::unique_ptr<FrameDetachedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class FrameDetachedNotification;
                FrameDetachedNotificationBuilder() : m_result(new FrameDetachedNotification()) { }

                template<int STEP> FrameDetachedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FrameDetachedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::FrameDetachedNotification> m_result;
        };

        static FrameDetachedNotificationBuilder<0> create() {
            return FrameDetachedNotificationBuilder<0>();
        }

    private:
        FrameDetachedNotification() {
        }

        String m_frameId;
};


// Wrapper for notification params
class  FrameStartedLoadingNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(FrameStartedLoadingNotification);
    public:
        static std::unique_ptr<FrameStartedLoadingNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~FrameStartedLoadingNotification() override { }

        String getFrameId() {
            return m_frameId;
        }
        void setFrameId(const String& value) {
            m_frameId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<FrameStartedLoadingNotification> clone() const;

        template<int STATE>
        class FrameStartedLoadingNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    FrameIdSet = 1 << 1,
                    AllFieldsSet = (FrameIdSet | 0)
                };


                FrameStartedLoadingNotificationBuilder<STATE | FrameIdSet>& setFrameId(const String& value) {
                    static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
                    m_result->setFrameId(value);
                    return castState<FrameIdSet>();
                }

                std::unique_ptr<FrameStartedLoadingNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class FrameStartedLoadingNotification;
                FrameStartedLoadingNotificationBuilder() : m_result(new FrameStartedLoadingNotification()) { }

                template<int STEP> FrameStartedLoadingNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FrameStartedLoadingNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::FrameStartedLoadingNotification> m_result;
        };

        static FrameStartedLoadingNotificationBuilder<0> create() {
            return FrameStartedLoadingNotificationBuilder<0>();
        }

    private:
        FrameStartedLoadingNotification() {
        }

        String m_frameId;
};


// Wrapper for notification params
class  FrameStoppedLoadingNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(FrameStoppedLoadingNotification);
    public:
        static std::unique_ptr<FrameStoppedLoadingNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~FrameStoppedLoadingNotification() override { }

        String getFrameId() {
            return m_frameId;
        }
        void setFrameId(const String& value) {
            m_frameId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<FrameStoppedLoadingNotification> clone() const;

        template<int STATE>
        class FrameStoppedLoadingNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    FrameIdSet = 1 << 1,
                    AllFieldsSet = (FrameIdSet | 0)
                };


                FrameStoppedLoadingNotificationBuilder<STATE | FrameIdSet>& setFrameId(const String& value) {
                    static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
                    m_result->setFrameId(value);
                    return castState<FrameIdSet>();
                }

                std::unique_ptr<FrameStoppedLoadingNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class FrameStoppedLoadingNotification;
                FrameStoppedLoadingNotificationBuilder() : m_result(new FrameStoppedLoadingNotification()) { }

                template<int STEP> FrameStoppedLoadingNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FrameStoppedLoadingNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Page::FrameStoppedLoadingNotification> m_result;
        };

        static FrameStoppedLoadingNotificationBuilder<0> create() {
            return FrameStoppedLoadingNotificationBuilder<0>();
        }

    private:
        FrameStoppedLoadingNotification() {
        }

        String m_frameId;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual DispatchResponse enable() = 0;
        virtual DispatchResponse disable() = 0;
        virtual DispatchResponse addScriptToEvaluateOnLoad(const String& in_scriptSource, String* out_identifier) = 0;
        virtual DispatchResponse removeScriptToEvaluateOnLoad(const String& in_identifier) = 0;
        virtual DispatchResponse reload(Maybe<bool> in_ignoreCache, Maybe<String> in_scriptToEvaluateOnLoad) = 0;
        virtual DispatchResponse getResourceTree(std::unique_ptr<protocol::Page::FrameResourceTree>* out_frameTree) = 0;
        virtual DispatchResponse getResourceContent(const String& in_frameId, const String& in_url, String* out_content, bool* out_base64Encoded) = 0;
        virtual DispatchResponse searchInResource(const String& in_frameId, const String& in_url, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, Maybe<String> in_requestId, std::unique_ptr<protocol::Array<protocol::GenericTypes::SearchMatch>>* out_result) = 0;
        virtual DispatchResponse searchInResources(const String& in_text, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<protocol::Array<protocol::Page::SearchResult>>* out_result) = 0;
        virtual DispatchResponse setDocumentContent(const String& in_frameId, const String& in_html) = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void loadEventFired(double timestamp);
        void frameDetached(const String& frameId);
        void frameStartedLoading(const String& frameId);
        void frameStoppedLoading(const String& frameId);

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

} // namespace Page
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Page_h)

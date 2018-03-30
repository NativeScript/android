// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Overlay_h
#define v8_inspector_protocol_Overlay_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "src/inspector/protocol/DOM.h"
#include "src/inspector/protocol/Page.h"
#include "src/inspector/protocol/Runtime.h"

namespace v8_inspector {
namespace protocol {
namespace Overlay {

// ------------- Forward and enum declarations.
class HighlightConfig;
class ScreenshotRequestedNotification;

// ------------- Type and builder declarations.

class  HighlightConfig : public Serializable{
    PROTOCOL_DISALLOW_COPY(HighlightConfig);
public:
    static std::unique_ptr<HighlightConfig> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~HighlightConfig() override { }

    bool hasShowInfo() { return m_showInfo.isJust(); }
    bool getShowInfo(bool defaultValue) { return m_showInfo.isJust() ? m_showInfo.fromJust() : defaultValue; }
    void setShowInfo(bool value) { m_showInfo = value; }

    bool hasShowRulers() { return m_showRulers.isJust(); }
    bool getShowRulers(bool defaultValue) { return m_showRulers.isJust() ? m_showRulers.fromJust() : defaultValue; }
    void setShowRulers(bool value) { m_showRulers = value; }

    bool hasShowExtensionLines() { return m_showExtensionLines.isJust(); }
    bool getShowExtensionLines(bool defaultValue) { return m_showExtensionLines.isJust() ? m_showExtensionLines.fromJust() : defaultValue; }
    void setShowExtensionLines(bool value) { m_showExtensionLines = value; }

    bool hasDisplayAsMaterial() { return m_displayAsMaterial.isJust(); }
    bool getDisplayAsMaterial(bool defaultValue) { return m_displayAsMaterial.isJust() ? m_displayAsMaterial.fromJust() : defaultValue; }
    void setDisplayAsMaterial(bool value) { m_displayAsMaterial = value; }

    bool hasContentColor() { return m_contentColor.isJust(); }
    protocol::DOM::RGBAColor* getContentColor(protocol::DOM::RGBAColor* defaultValue) { return m_contentColor.isJust() ? m_contentColor.fromJust() : defaultValue; }
    void setContentColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_contentColor = std::move(value); }

    bool hasPaddingColor() { return m_paddingColor.isJust(); }
    protocol::DOM::RGBAColor* getPaddingColor(protocol::DOM::RGBAColor* defaultValue) { return m_paddingColor.isJust() ? m_paddingColor.fromJust() : defaultValue; }
    void setPaddingColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_paddingColor = std::move(value); }

    bool hasBorderColor() { return m_borderColor.isJust(); }
    protocol::DOM::RGBAColor* getBorderColor(protocol::DOM::RGBAColor* defaultValue) { return m_borderColor.isJust() ? m_borderColor.fromJust() : defaultValue; }
    void setBorderColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_borderColor = std::move(value); }

    bool hasMarginColor() { return m_marginColor.isJust(); }
    protocol::DOM::RGBAColor* getMarginColor(protocol::DOM::RGBAColor* defaultValue) { return m_marginColor.isJust() ? m_marginColor.fromJust() : defaultValue; }
    void setMarginColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_marginColor = std::move(value); }

    bool hasEventTargetColor() { return m_eventTargetColor.isJust(); }
    protocol::DOM::RGBAColor* getEventTargetColor(protocol::DOM::RGBAColor* defaultValue) { return m_eventTargetColor.isJust() ? m_eventTargetColor.fromJust() : defaultValue; }
    void setEventTargetColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_eventTargetColor = std::move(value); }

    bool hasShapeColor() { return m_shapeColor.isJust(); }
    protocol::DOM::RGBAColor* getShapeColor(protocol::DOM::RGBAColor* defaultValue) { return m_shapeColor.isJust() ? m_shapeColor.fromJust() : defaultValue; }
    void setShapeColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_shapeColor = std::move(value); }

    bool hasShapeMarginColor() { return m_shapeMarginColor.isJust(); }
    protocol::DOM::RGBAColor* getShapeMarginColor(protocol::DOM::RGBAColor* defaultValue) { return m_shapeMarginColor.isJust() ? m_shapeMarginColor.fromJust() : defaultValue; }
    void setShapeMarginColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_shapeMarginColor = std::move(value); }

    bool hasSelectorList() { return m_selectorList.isJust(); }
    String getSelectorList(const String& defaultValue) { return m_selectorList.isJust() ? m_selectorList.fromJust() : defaultValue; }
    void setSelectorList(const String& value) { m_selectorList = value; }

    bool hasCssGridColor() { return m_cssGridColor.isJust(); }
    protocol::DOM::RGBAColor* getCssGridColor(protocol::DOM::RGBAColor* defaultValue) { return m_cssGridColor.isJust() ? m_cssGridColor.fromJust() : defaultValue; }
    void setCssGridColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_cssGridColor = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<HighlightConfig> clone() const;

    template<int STATE>
    class HighlightConfigBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            AllFieldsSet = (0)};


        HighlightConfigBuilder<STATE>& setShowInfo(bool value)
        {
            m_result->setShowInfo(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShowRulers(bool value)
        {
            m_result->setShowRulers(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShowExtensionLines(bool value)
        {
            m_result->setShowExtensionLines(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setDisplayAsMaterial(bool value)
        {
            m_result->setDisplayAsMaterial(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setContentColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setContentColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setPaddingColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setPaddingColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setBorderColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setBorderColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setMarginColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setMarginColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setEventTargetColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setEventTargetColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShapeColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setShapeColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShapeMarginColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setShapeMarginColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setSelectorList(const String& value)
        {
            m_result->setSelectorList(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setCssGridColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setCssGridColor(std::move(value));
            return *this;
        }

        std::unique_ptr<HighlightConfig> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class HighlightConfig;
        HighlightConfigBuilder() : m_result(new HighlightConfig()) { }

        template<int STEP> HighlightConfigBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<HighlightConfigBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Overlay::HighlightConfig> m_result;
    };

    static HighlightConfigBuilder<0> create()
    {
        return HighlightConfigBuilder<0>();
    }

private:
    HighlightConfig()
    {
    }

    Maybe<bool> m_showInfo;
    Maybe<bool> m_showRulers;
    Maybe<bool> m_showExtensionLines;
    Maybe<bool> m_displayAsMaterial;
    Maybe<protocol::DOM::RGBAColor> m_contentColor;
    Maybe<protocol::DOM::RGBAColor> m_paddingColor;
    Maybe<protocol::DOM::RGBAColor> m_borderColor;
    Maybe<protocol::DOM::RGBAColor> m_marginColor;
    Maybe<protocol::DOM::RGBAColor> m_eventTargetColor;
    Maybe<protocol::DOM::RGBAColor> m_shapeColor;
    Maybe<protocol::DOM::RGBAColor> m_shapeMarginColor;
    Maybe<String> m_selectorList;
    Maybe<protocol::DOM::RGBAColor> m_cssGridColor;
};


class  ScreenshotRequestedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ScreenshotRequestedNotification);
public:
    static std::unique_ptr<ScreenshotRequestedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ScreenshotRequestedNotification() override { }

    protocol::Page::Viewport* getViewport() { return m_viewport.get(); }
    void setViewport(std::unique_ptr<protocol::Page::Viewport> value) { m_viewport = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ScreenshotRequestedNotification> clone() const;

    template<int STATE>
    class ScreenshotRequestedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ViewportSet = 1 << 1,
            AllFieldsSet = (ViewportSet | 0)};


        ScreenshotRequestedNotificationBuilder<STATE | ViewportSet>& setViewport(std::unique_ptr<protocol::Page::Viewport> value)
        {
            static_assert(!(STATE & ViewportSet), "property viewport should not be set yet");
            m_result->setViewport(std::move(value));
            return castState<ViewportSet>();
        }

        std::unique_ptr<ScreenshotRequestedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ScreenshotRequestedNotification;
        ScreenshotRequestedNotificationBuilder() : m_result(new ScreenshotRequestedNotification()) { }

        template<int STEP> ScreenshotRequestedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ScreenshotRequestedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Overlay::ScreenshotRequestedNotification> m_result;
    };

    static ScreenshotRequestedNotificationBuilder<0> create()
    {
        return ScreenshotRequestedNotificationBuilder<0>();
    }

private:
    ScreenshotRequestedNotification()
    {
    }

    std::unique_ptr<protocol::Page::Viewport> m_viewport;
};


// ------------- Backend interface.

class  Backend {
public:
    virtual ~Backend() { }

    virtual DispatchResponse enable() = 0;
    virtual DispatchResponse disable() = 0;
    virtual DispatchResponse setShowFPSCounter(bool in_show) = 0;
    virtual DispatchResponse setPausedInDebuggerMessage(Maybe<String> in_message) = 0;
    virtual DispatchResponse highlightNode(std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig, Maybe<int> in_nodeId, Maybe<int> in_backendNodeId, Maybe<String> in_objectId) = 0;
    virtual DispatchResponse highlightFrame(const String& in_frameId, Maybe<protocol::DOM::RGBAColor> in_contentColor, Maybe<protocol::DOM::RGBAColor> in_contentOutlineColor) = 0;
    virtual DispatchResponse hideHighlight() = 0;
    virtual DispatchResponse getHighlightObjectForTest(int in_nodeId, std::unique_ptr<protocol::DictionaryValue>* out_highlight) = 0;

};

// ------------- Frontend interface.

class  Frontend {
public:
    explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
    void screenshotRequested(std::unique_ptr<protocol::Page::Viewport> viewport);

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

} // namespace Overlay
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Overlay_h)

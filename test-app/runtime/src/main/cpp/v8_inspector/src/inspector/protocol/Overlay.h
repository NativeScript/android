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
// Configuration data for the highlighting of page elements.
class HighlightConfig;

// ------------- Type and builder declarations.

// Configuration data for the highlighting of page elements.
class  HighlightConfig {
        PROTOCOL_DISALLOW_COPY(HighlightConfig);
    public:
        static std::unique_ptr<HighlightConfig> parse(protocol::Value* value, ErrorSupport* errors);

        ~HighlightConfig() { }

        bool hasShowInfo() {
            return m_showInfo.isJust();
        }
        bool getShowInfo(bool defaultValue) {
            return m_showInfo.isJust() ? m_showInfo.fromJust() : defaultValue;
        }
        void setShowInfo(bool value) {
            m_showInfo = value;
        }

        bool hasShowRulers() {
            return m_showRulers.isJust();
        }
        bool getShowRulers(bool defaultValue) {
            return m_showRulers.isJust() ? m_showRulers.fromJust() : defaultValue;
        }
        void setShowRulers(bool value) {
            m_showRulers = value;
        }

        bool hasShowExtensionLines() {
            return m_showExtensionLines.isJust();
        }
        bool getShowExtensionLines(bool defaultValue) {
            return m_showExtensionLines.isJust() ? m_showExtensionLines.fromJust() : defaultValue;
        }
        void setShowExtensionLines(bool value) {
            m_showExtensionLines = value;
        }

        bool hasDisplayAsMaterial() {
            return m_displayAsMaterial.isJust();
        }
        bool getDisplayAsMaterial(bool defaultValue) {
            return m_displayAsMaterial.isJust() ? m_displayAsMaterial.fromJust() : defaultValue;
        }
        void setDisplayAsMaterial(bool value) {
            m_displayAsMaterial = value;
        }

        bool hasContentColor() {
            return m_contentColor.isJust();
        }
        protocol::DOM::RGBA* getContentColor(protocol::DOM::RGBA* defaultValue) {
            return m_contentColor.isJust() ? m_contentColor.fromJust() : defaultValue;
        }
        void setContentColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_contentColor = std::move(value);
        }

        bool hasPaddingColor() {
            return m_paddingColor.isJust();
        }
        protocol::DOM::RGBA* getPaddingColor(protocol::DOM::RGBA* defaultValue) {
            return m_paddingColor.isJust() ? m_paddingColor.fromJust() : defaultValue;
        }
        void setPaddingColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_paddingColor = std::move(value);
        }

        bool hasBorderColor() {
            return m_borderColor.isJust();
        }
        protocol::DOM::RGBA* getBorderColor(protocol::DOM::RGBA* defaultValue) {
            return m_borderColor.isJust() ? m_borderColor.fromJust() : defaultValue;
        }
        void setBorderColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_borderColor = std::move(value);
        }

        bool hasMarginColor() {
            return m_marginColor.isJust();
        }
        protocol::DOM::RGBA* getMarginColor(protocol::DOM::RGBA* defaultValue) {
            return m_marginColor.isJust() ? m_marginColor.fromJust() : defaultValue;
        }
        void setMarginColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_marginColor = std::move(value);
        }

        bool hasEventTargetColor() {
            return m_eventTargetColor.isJust();
        }
        protocol::DOM::RGBA* getEventTargetColor(protocol::DOM::RGBA* defaultValue) {
            return m_eventTargetColor.isJust() ? m_eventTargetColor.fromJust() : defaultValue;
        }
        void setEventTargetColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_eventTargetColor = std::move(value);
        }

        bool hasShapeColor() {
            return m_shapeColor.isJust();
        }
        protocol::DOM::RGBA* getShapeColor(protocol::DOM::RGBA* defaultValue) {
            return m_shapeColor.isJust() ? m_shapeColor.fromJust() : defaultValue;
        }
        void setShapeColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_shapeColor = std::move(value);
        }

        bool hasShapeMarginColor() {
            return m_shapeMarginColor.isJust();
        }
        protocol::DOM::RGBA* getShapeMarginColor(protocol::DOM::RGBA* defaultValue) {
            return m_shapeMarginColor.isJust() ? m_shapeMarginColor.fromJust() : defaultValue;
        }
        void setShapeMarginColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_shapeMarginColor = std::move(value);
        }

        bool hasSelectorList() {
            return m_selectorList.isJust();
        }
        String getSelectorList(const String& defaultValue) {
            return m_selectorList.isJust() ? m_selectorList.fromJust() : defaultValue;
        }
        void setSelectorList(const String& value) {
            m_selectorList = value;
        }

        bool hasCssGridColor() {
            return m_cssGridColor.isJust();
        }
        protocol::DOM::RGBA* getCssGridColor(protocol::DOM::RGBA* defaultValue) {
            return m_cssGridColor.isJust() ? m_cssGridColor.fromJust() : defaultValue;
        }
        void setCssGridColor(std::unique_ptr<protocol::DOM::RGBA> value) {
            m_cssGridColor = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<HighlightConfig> clone() const;

        template<int STATE>
        class HighlightConfigBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    AllFieldsSet = (0)
                };


                HighlightConfigBuilder<STATE>& setShowInfo(bool value) {
                    m_result->setShowInfo(value);
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setShowRulers(bool value) {
                    m_result->setShowRulers(value);
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setShowExtensionLines(bool value) {
                    m_result->setShowExtensionLines(value);
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setDisplayAsMaterial(bool value) {
                    m_result->setDisplayAsMaterial(value);
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setContentColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setContentColor(std::move(value));
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setPaddingColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setPaddingColor(std::move(value));
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setBorderColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setBorderColor(std::move(value));
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setMarginColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setMarginColor(std::move(value));
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setEventTargetColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setEventTargetColor(std::move(value));
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setShapeColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setShapeColor(std::move(value));
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setShapeMarginColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setShapeMarginColor(std::move(value));
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setSelectorList(const String& value) {
                    m_result->setSelectorList(value);
                    return *this;
                }

                HighlightConfigBuilder<STATE>& setCssGridColor(std::unique_ptr<protocol::DOM::RGBA> value) {
                    m_result->setCssGridColor(std::move(value));
                    return *this;
                }

                std::unique_ptr<HighlightConfig> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class HighlightConfig;
                HighlightConfigBuilder() : m_result(new HighlightConfig()) { }

                template<int STEP> HighlightConfigBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<HighlightConfigBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Overlay::HighlightConfig> m_result;
        };

        static HighlightConfigBuilder<0> create() {
            return HighlightConfigBuilder<0>();
        }

    private:
        HighlightConfig() {
        }

        Maybe<bool> m_showInfo;
        Maybe<bool> m_showRulers;
        Maybe<bool> m_showExtensionLines;
        Maybe<bool> m_displayAsMaterial;
        Maybe<protocol::DOM::RGBA> m_contentColor;
        Maybe<protocol::DOM::RGBA> m_paddingColor;
        Maybe<protocol::DOM::RGBA> m_borderColor;
        Maybe<protocol::DOM::RGBA> m_marginColor;
        Maybe<protocol::DOM::RGBA> m_eventTargetColor;
        Maybe<protocol::DOM::RGBA> m_shapeColor;
        Maybe<protocol::DOM::RGBA> m_shapeMarginColor;
        Maybe<String> m_selectorList;
        Maybe<protocol::DOM::RGBA> m_cssGridColor;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual void enable(ErrorString*) = 0;
        virtual void disable(ErrorString*) = 0;
        virtual void setShowFPSCounter(ErrorString*, bool in_show) = 0;
        virtual void setPausedInDebuggerMessage(ErrorString*, const Maybe<String>& in_message) = 0;
        virtual void highlightNode(ErrorString*, std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig, const Maybe<int>& in_nodeId, const Maybe<int>& in_backendNodeId, const Maybe<String>& in_objectId) = 0;
        virtual void highlightFrame(ErrorString*, const String& in_frameId, const Maybe<protocol::DOM::RGBA>& in_contentColor, const Maybe<protocol::DOM::RGBA>& in_contentOutlineColor) = 0;
        virtual void hideHighlight(ErrorString*) = 0;
        virtual void getHighlightObjectForTest(ErrorString*, int in_nodeId, std::unique_ptr<protocol::DictionaryValue>* out_highlight) = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void screenshotRequested(std::unique_ptr<protocol::Page::Viewport> viewport);

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

} // namespace Overlay
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Overlay_h)

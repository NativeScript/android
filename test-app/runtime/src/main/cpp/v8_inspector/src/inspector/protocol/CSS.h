// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_CSS_h
#define v8_inspector_protocol_CSS_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "src/inspector/protocol/DOM.h"

namespace v8_inspector {
namespace protocol {
namespace CSS {

// ------------- Forward and enum declarations.
using StyleSheetId = String;
using StyleSheetOrigin = String;
class PseudoElementMatches;
class InheritedStyleEntry;
class RuleMatch;
class Value;
class SelectorList;
class CSSStyleSheetHeader;
class CSSRule;
class RuleUsage;
class SourceRange;
class ShorthandEntry;
class CSSComputedStyleProperty;
class CSSStyle;
class CSSProperty;
class CSSMedia;
class MediaQuery;
class MediaQueryExpression;
class PlatformFontUsage;
class FontFace;
class CSSKeyframesRule;
class CSSKeyframeRule;
class StyleDeclarationEdit;
class FontsUpdatedNotification;
using MediaQueryResultChangedNotification = Object;
class StyleSheetAddedNotification;
class StyleSheetChangedNotification;
class StyleSheetRemovedNotification;

namespace StyleSheetOriginEnum {
extern const char* Injected;
extern const char* UserAgent;
extern const char* Inspector;
extern const char* Regular;
} // namespace StyleSheetOriginEnum

// ------------- Type and builder declarations.

class  PseudoElementMatches : public Serializable {
        PROTOCOL_DISALLOW_COPY(PseudoElementMatches);
    public:
        static std::unique_ptr<PseudoElementMatches> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~PseudoElementMatches() override { }

        String getPseudoType() {
            return m_pseudoType;
        }
        void setPseudoType(const String& value) {
            m_pseudoType = value;
        }

        protocol::Array<protocol::CSS::RuleMatch>* getMatches() {
            return m_matches.get();
        }
        void setMatches(std::unique_ptr<protocol::Array<protocol::CSS::RuleMatch>> value) {
            m_matches = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<PseudoElementMatches> clone() const;

        template<int STATE>
        class PseudoElementMatchesBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    PseudoTypeSet = 1 << 1,
                    MatchesSet = 1 << 2,
                    AllFieldsSet = (PseudoTypeSet | MatchesSet | 0)
                };


                PseudoElementMatchesBuilder<STATE | PseudoTypeSet>& setPseudoType(const String& value) {
                    static_assert(!(STATE & PseudoTypeSet), "property pseudoType should not be set yet");
                    m_result->setPseudoType(value);
                    return castState<PseudoTypeSet>();
                }

                PseudoElementMatchesBuilder<STATE | MatchesSet>& setMatches(std::unique_ptr<protocol::Array<protocol::CSS::RuleMatch>> value) {
                    static_assert(!(STATE & MatchesSet), "property matches should not be set yet");
                    m_result->setMatches(std::move(value));
                    return castState<MatchesSet>();
                }

                std::unique_ptr<PseudoElementMatches> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class PseudoElementMatches;
                PseudoElementMatchesBuilder() : m_result(new PseudoElementMatches()) { }

                template<int STEP> PseudoElementMatchesBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<PseudoElementMatchesBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::PseudoElementMatches> m_result;
        };

        static PseudoElementMatchesBuilder<0> create() {
            return PseudoElementMatchesBuilder<0>();
        }

    private:
        PseudoElementMatches() {
        }

        String m_pseudoType;
        std::unique_ptr<protocol::Array<protocol::CSS::RuleMatch>> m_matches;
};


class  InheritedStyleEntry : public Serializable {
        PROTOCOL_DISALLOW_COPY(InheritedStyleEntry);
    public:
        static std::unique_ptr<InheritedStyleEntry> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~InheritedStyleEntry() override { }

        bool hasInlineStyle() {
            return m_inlineStyle.isJust();
        }
        protocol::CSS::CSSStyle* getInlineStyle(protocol::CSS::CSSStyle* defaultValue) {
            return m_inlineStyle.isJust() ? m_inlineStyle.fromJust() : defaultValue;
        }
        void setInlineStyle(std::unique_ptr<protocol::CSS::CSSStyle> value) {
            m_inlineStyle = std::move(value);
        }

        protocol::Array<protocol::CSS::RuleMatch>* getMatchedCSSRules() {
            return m_matchedCSSRules.get();
        }
        void setMatchedCSSRules(std::unique_ptr<protocol::Array<protocol::CSS::RuleMatch>> value) {
            m_matchedCSSRules = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<InheritedStyleEntry> clone() const;

        template<int STATE>
        class InheritedStyleEntryBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    MatchedCSSRulesSet = 1 << 1,
                    AllFieldsSet = (MatchedCSSRulesSet | 0)
                };


                InheritedStyleEntryBuilder<STATE>& setInlineStyle(std::unique_ptr<protocol::CSS::CSSStyle> value) {
                    m_result->setInlineStyle(std::move(value));
                    return *this;
                }

                InheritedStyleEntryBuilder<STATE | MatchedCSSRulesSet>& setMatchedCSSRules(std::unique_ptr<protocol::Array<protocol::CSS::RuleMatch>> value) {
                    static_assert(!(STATE & MatchedCSSRulesSet), "property matchedCSSRules should not be set yet");
                    m_result->setMatchedCSSRules(std::move(value));
                    return castState<MatchedCSSRulesSet>();
                }

                std::unique_ptr<InheritedStyleEntry> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class InheritedStyleEntry;
                InheritedStyleEntryBuilder() : m_result(new InheritedStyleEntry()) { }

                template<int STEP> InheritedStyleEntryBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<InheritedStyleEntryBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::InheritedStyleEntry> m_result;
        };

        static InheritedStyleEntryBuilder<0> create() {
            return InheritedStyleEntryBuilder<0>();
        }

    private:
        InheritedStyleEntry() {
        }

        Maybe<protocol::CSS::CSSStyle> m_inlineStyle;
        std::unique_ptr<protocol::Array<protocol::CSS::RuleMatch>> m_matchedCSSRules;
};


class  RuleMatch : public Serializable {
        PROTOCOL_DISALLOW_COPY(RuleMatch);
    public:
        static std::unique_ptr<RuleMatch> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~RuleMatch() override { }

        protocol::CSS::CSSRule* getRule() {
            return m_rule.get();
        }
        void setRule(std::unique_ptr<protocol::CSS::CSSRule> value) {
            m_rule = std::move(value);
        }

        protocol::Array<int>* getMatchingSelectors() {
            return m_matchingSelectors.get();
        }
        void setMatchingSelectors(std::unique_ptr<protocol::Array<int>> value) {
            m_matchingSelectors = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<RuleMatch> clone() const;

        template<int STATE>
        class RuleMatchBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    RuleSet = 1 << 1,
                    MatchingSelectorsSet = 1 << 2,
                    AllFieldsSet = (RuleSet | MatchingSelectorsSet | 0)
                };


                RuleMatchBuilder<STATE | RuleSet>& setRule(std::unique_ptr<protocol::CSS::CSSRule> value) {
                    static_assert(!(STATE & RuleSet), "property rule should not be set yet");
                    m_result->setRule(std::move(value));
                    return castState<RuleSet>();
                }

                RuleMatchBuilder<STATE | MatchingSelectorsSet>& setMatchingSelectors(std::unique_ptr<protocol::Array<int>> value) {
                    static_assert(!(STATE & MatchingSelectorsSet), "property matchingSelectors should not be set yet");
                    m_result->setMatchingSelectors(std::move(value));
                    return castState<MatchingSelectorsSet>();
                }

                std::unique_ptr<RuleMatch> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class RuleMatch;
                RuleMatchBuilder() : m_result(new RuleMatch()) { }

                template<int STEP> RuleMatchBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<RuleMatchBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::RuleMatch> m_result;
        };

        static RuleMatchBuilder<0> create() {
            return RuleMatchBuilder<0>();
        }

    private:
        RuleMatch() {
        }

        std::unique_ptr<protocol::CSS::CSSRule> m_rule;
        std::unique_ptr<protocol::Array<int>> m_matchingSelectors;
};


class  Value : public Serializable {
        PROTOCOL_DISALLOW_COPY(Value);
    public:
        static std::unique_ptr<Value> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~Value() override { }

        String getText() {
            return m_text;
        }
        void setText(const String& value) {
            m_text = value;
        }

        bool hasRange() {
            return m_range.isJust();
        }
        protocol::CSS::SourceRange* getRange(protocol::CSS::SourceRange* defaultValue) {
            return m_range.isJust() ? m_range.fromJust() : defaultValue;
        }
        void setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
            m_range = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<Value> clone() const;

        template<int STATE>
        class ValueBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TextSet = 1 << 1,
                    AllFieldsSet = (TextSet | 0)
                };


                ValueBuilder<STATE | TextSet>& setText(const String& value) {
                    static_assert(!(STATE & TextSet), "property text should not be set yet");
                    m_result->setText(value);
                    return castState<TextSet>();
                }

                ValueBuilder<STATE>& setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
                    m_result->setRange(std::move(value));
                    return *this;
                }

                std::unique_ptr<Value> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Value;
                ValueBuilder() : m_result(new Value()) { }

                template<int STEP> ValueBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ValueBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::Value> m_result;
        };

        static ValueBuilder<0> create() {
            return ValueBuilder<0>();
        }

    private:
        Value() {
        }

        String m_text;
        Maybe<protocol::CSS::SourceRange> m_range;
};


class  SelectorList : public Serializable {
        PROTOCOL_DISALLOW_COPY(SelectorList);
    public:
        static std::unique_ptr<SelectorList> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~SelectorList() override { }

        protocol::Array<protocol::CSS::Value>* getSelectors() {
            return m_selectors.get();
        }
        void setSelectors(std::unique_ptr<protocol::Array<protocol::CSS::Value>> value) {
            m_selectors = std::move(value);
        }

        String getText() {
            return m_text;
        }
        void setText(const String& value) {
            m_text = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<SelectorList> clone() const;

        template<int STATE>
        class SelectorListBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    SelectorsSet = 1 << 1,
                    TextSet = 1 << 2,
                    AllFieldsSet = (SelectorsSet | TextSet | 0)
                };


                SelectorListBuilder<STATE | SelectorsSet>& setSelectors(std::unique_ptr<protocol::Array<protocol::CSS::Value>> value) {
                    static_assert(!(STATE & SelectorsSet), "property selectors should not be set yet");
                    m_result->setSelectors(std::move(value));
                    return castState<SelectorsSet>();
                }

                SelectorListBuilder<STATE | TextSet>& setText(const String& value) {
                    static_assert(!(STATE & TextSet), "property text should not be set yet");
                    m_result->setText(value);
                    return castState<TextSet>();
                }

                std::unique_ptr<SelectorList> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class SelectorList;
                SelectorListBuilder() : m_result(new SelectorList()) { }

                template<int STEP> SelectorListBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<SelectorListBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::SelectorList> m_result;
        };

        static SelectorListBuilder<0> create() {
            return SelectorListBuilder<0>();
        }

    private:
        SelectorList() {
        }

        std::unique_ptr<protocol::Array<protocol::CSS::Value>> m_selectors;
        String m_text;
};


class  CSSStyleSheetHeader : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSStyleSheetHeader);
    public:
        static std::unique_ptr<CSSStyleSheetHeader> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSStyleSheetHeader() override { }

        String getStyleSheetId() {
            return m_styleSheetId;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        String getFrameId() {
            return m_frameId;
        }
        void setFrameId(const String& value) {
            m_frameId = value;
        }

        String getSourceURL() {
            return m_sourceURL;
        }
        void setSourceURL(const String& value) {
            m_sourceURL = value;
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

        String getOrigin() {
            return m_origin;
        }
        void setOrigin(const String& value) {
            m_origin = value;
        }

        String getTitle() {
            return m_title;
        }
        void setTitle(const String& value) {
            m_title = value;
        }

        bool hasOwnerNode() {
            return m_ownerNode.isJust();
        }
        int getOwnerNode(int defaultValue) {
            return m_ownerNode.isJust() ? m_ownerNode.fromJust() : defaultValue;
        }
        void setOwnerNode(int value) {
            m_ownerNode = value;
        }

        bool getDisabled() {
            return m_disabled;
        }
        void setDisabled(bool value) {
            m_disabled = value;
        }

        bool hasHasSourceURL() {
            return m_hasSourceURL.isJust();
        }
        bool getHasSourceURL(bool defaultValue) {
            return m_hasSourceURL.isJust() ? m_hasSourceURL.fromJust() : defaultValue;
        }
        void setHasSourceURL(bool value) {
            m_hasSourceURL = value;
        }

        bool getIsInline() {
            return m_isInline;
        }
        void setIsInline(bool value) {
            m_isInline = value;
        }

        double getStartLine() {
            return m_startLine;
        }
        void setStartLine(double value) {
            m_startLine = value;
        }

        double getStartColumn() {
            return m_startColumn;
        }
        void setStartColumn(double value) {
            m_startColumn = value;
        }

        double getLength() {
            return m_length;
        }
        void setLength(double value) {
            m_length = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSStyleSheetHeader> clone() const;

        template<int STATE>
        class CSSStyleSheetHeaderBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    StyleSheetIdSet = 1 << 1,
                    FrameIdSet = 1 << 2,
                    SourceURLSet = 1 << 3,
                    OriginSet = 1 << 4,
                    TitleSet = 1 << 5,
                    DisabledSet = 1 << 6,
                    IsInlineSet = 1 << 7,
                    StartLineSet = 1 << 8,
                    StartColumnSet = 1 << 9,
                    LengthSet = 1 << 10,
                    AllFieldsSet = (StyleSheetIdSet | FrameIdSet | SourceURLSet | OriginSet | TitleSet | DisabledSet | IsInlineSet | StartLineSet | StartColumnSet | LengthSet | 0)
                };


                CSSStyleSheetHeaderBuilder<STATE | StyleSheetIdSet>& setStyleSheetId(const String& value) {
                    static_assert(!(STATE & StyleSheetIdSet), "property styleSheetId should not be set yet");
                    m_result->setStyleSheetId(value);
                    return castState<StyleSheetIdSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE | FrameIdSet>& setFrameId(const String& value) {
                    static_assert(!(STATE & FrameIdSet), "property frameId should not be set yet");
                    m_result->setFrameId(value);
                    return castState<FrameIdSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE | SourceURLSet>& setSourceURL(const String& value) {
                    static_assert(!(STATE & SourceURLSet), "property sourceURL should not be set yet");
                    m_result->setSourceURL(value);
                    return castState<SourceURLSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE>& setSourceMapURL(const String& value) {
                    m_result->setSourceMapURL(value);
                    return *this;
                }

                CSSStyleSheetHeaderBuilder<STATE | OriginSet>& setOrigin(const String& value) {
                    static_assert(!(STATE & OriginSet), "property origin should not be set yet");
                    m_result->setOrigin(value);
                    return castState<OriginSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE | TitleSet>& setTitle(const String& value) {
                    static_assert(!(STATE & TitleSet), "property title should not be set yet");
                    m_result->setTitle(value);
                    return castState<TitleSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE>& setOwnerNode(int value) {
                    m_result->setOwnerNode(value);
                    return *this;
                }

                CSSStyleSheetHeaderBuilder<STATE | DisabledSet>& setDisabled(bool value) {
                    static_assert(!(STATE & DisabledSet), "property disabled should not be set yet");
                    m_result->setDisabled(value);
                    return castState<DisabledSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE>& setHasSourceURL(bool value) {
                    m_result->setHasSourceURL(value);
                    return *this;
                }

                CSSStyleSheetHeaderBuilder<STATE | IsInlineSet>& setIsInline(bool value) {
                    static_assert(!(STATE & IsInlineSet), "property isInline should not be set yet");
                    m_result->setIsInline(value);
                    return castState<IsInlineSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE | StartLineSet>& setStartLine(double value) {
                    static_assert(!(STATE & StartLineSet), "property startLine should not be set yet");
                    m_result->setStartLine(value);
                    return castState<StartLineSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE | StartColumnSet>& setStartColumn(double value) {
                    static_assert(!(STATE & StartColumnSet), "property startColumn should not be set yet");
                    m_result->setStartColumn(value);
                    return castState<StartColumnSet>();
                }

                CSSStyleSheetHeaderBuilder<STATE | LengthSet>& setLength(double value) {
                    static_assert(!(STATE & LengthSet), "property length should not be set yet");
                    m_result->setLength(value);
                    return castState<LengthSet>();
                }

                std::unique_ptr<CSSStyleSheetHeader> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSStyleSheetHeader;
                CSSStyleSheetHeaderBuilder() : m_result(new CSSStyleSheetHeader()) { }

                template<int STEP> CSSStyleSheetHeaderBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSStyleSheetHeaderBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSStyleSheetHeader> m_result;
        };

        static CSSStyleSheetHeaderBuilder<0> create() {
            return CSSStyleSheetHeaderBuilder<0>();
        }

    private:
        CSSStyleSheetHeader() {
            m_disabled = false;
            m_isInline = false;
            m_startLine = 0;
            m_startColumn = 0;
            m_length = 0;
        }

        String m_styleSheetId;
        String m_frameId;
        String m_sourceURL;
        Maybe<String> m_sourceMapURL;
        String m_origin;
        String m_title;
        Maybe<int> m_ownerNode;
        bool m_disabled;
        Maybe<bool> m_hasSourceURL;
        bool m_isInline;
        double m_startLine;
        double m_startColumn;
        double m_length;
};


class  CSSRule : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSRule);
    public:
        static std::unique_ptr<CSSRule> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSRule() override { }

        bool hasStyleSheetId() {
            return m_styleSheetId.isJust();
        }
        String getStyleSheetId(const String& defaultValue) {
            return m_styleSheetId.isJust() ? m_styleSheetId.fromJust() : defaultValue;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        protocol::CSS::SelectorList* getSelectorList() {
            return m_selectorList.get();
        }
        void setSelectorList(std::unique_ptr<protocol::CSS::SelectorList> value) {
            m_selectorList = std::move(value);
        }

        String getOrigin() {
            return m_origin;
        }
        void setOrigin(const String& value) {
            m_origin = value;
        }

        protocol::CSS::CSSStyle* getStyle() {
            return m_style.get();
        }
        void setStyle(std::unique_ptr<protocol::CSS::CSSStyle> value) {
            m_style = std::move(value);
        }

        bool hasMedia() {
            return m_media.isJust();
        }
        protocol::Array<protocol::CSS::CSSMedia>* getMedia(protocol::Array<protocol::CSS::CSSMedia>* defaultValue) {
            return m_media.isJust() ? m_media.fromJust() : defaultValue;
        }
        void setMedia(std::unique_ptr<protocol::Array<protocol::CSS::CSSMedia>> value) {
            m_media = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSRule> clone() const;

        template<int STATE>
        class CSSRuleBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    SelectorListSet = 1 << 1,
                    OriginSet = 1 << 2,
                    StyleSet = 1 << 3,
                    AllFieldsSet = (SelectorListSet | OriginSet | StyleSet | 0)
                };


                CSSRuleBuilder<STATE>& setStyleSheetId(const String& value) {
                    m_result->setStyleSheetId(value);
                    return *this;
                }

                CSSRuleBuilder<STATE | SelectorListSet>& setSelectorList(std::unique_ptr<protocol::CSS::SelectorList> value) {
                    static_assert(!(STATE & SelectorListSet), "property selectorList should not be set yet");
                    m_result->setSelectorList(std::move(value));
                    return castState<SelectorListSet>();
                }

                CSSRuleBuilder<STATE | OriginSet>& setOrigin(const String& value) {
                    static_assert(!(STATE & OriginSet), "property origin should not be set yet");
                    m_result->setOrigin(value);
                    return castState<OriginSet>();
                }

                CSSRuleBuilder<STATE | StyleSet>& setStyle(std::unique_ptr<protocol::CSS::CSSStyle> value) {
                    static_assert(!(STATE & StyleSet), "property style should not be set yet");
                    m_result->setStyle(std::move(value));
                    return castState<StyleSet>();
                }

                CSSRuleBuilder<STATE>& setMedia(std::unique_ptr<protocol::Array<protocol::CSS::CSSMedia>> value) {
                    m_result->setMedia(std::move(value));
                    return *this;
                }

                std::unique_ptr<CSSRule> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSRule;
                CSSRuleBuilder() : m_result(new CSSRule()) { }

                template<int STEP> CSSRuleBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSRuleBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSRule> m_result;
        };

        static CSSRuleBuilder<0> create() {
            return CSSRuleBuilder<0>();
        }

    private:
        CSSRule() {
        }

        Maybe<String> m_styleSheetId;
        std::unique_ptr<protocol::CSS::SelectorList> m_selectorList;
        String m_origin;
        std::unique_ptr<protocol::CSS::CSSStyle> m_style;
        Maybe<protocol::Array<protocol::CSS::CSSMedia>> m_media;
};


class  RuleUsage : public Serializable {
        PROTOCOL_DISALLOW_COPY(RuleUsage);
    public:
        static std::unique_ptr<RuleUsage> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~RuleUsage() override { }

        String getStyleSheetId() {
            return m_styleSheetId;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        double getStartOffset() {
            return m_startOffset;
        }
        void setStartOffset(double value) {
            m_startOffset = value;
        }

        double getEndOffset() {
            return m_endOffset;
        }
        void setEndOffset(double value) {
            m_endOffset = value;
        }

        bool getUsed() {
            return m_used;
        }
        void setUsed(bool value) {
            m_used = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<RuleUsage> clone() const;

        template<int STATE>
        class RuleUsageBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    StyleSheetIdSet = 1 << 1,
                    StartOffsetSet = 1 << 2,
                    EndOffsetSet = 1 << 3,
                    UsedSet = 1 << 4,
                    AllFieldsSet = (StyleSheetIdSet | StartOffsetSet | EndOffsetSet | UsedSet | 0)
                };


                RuleUsageBuilder<STATE | StyleSheetIdSet>& setStyleSheetId(const String& value) {
                    static_assert(!(STATE & StyleSheetIdSet), "property styleSheetId should not be set yet");
                    m_result->setStyleSheetId(value);
                    return castState<StyleSheetIdSet>();
                }

                RuleUsageBuilder<STATE | StartOffsetSet>& setStartOffset(double value) {
                    static_assert(!(STATE & StartOffsetSet), "property startOffset should not be set yet");
                    m_result->setStartOffset(value);
                    return castState<StartOffsetSet>();
                }

                RuleUsageBuilder<STATE | EndOffsetSet>& setEndOffset(double value) {
                    static_assert(!(STATE & EndOffsetSet), "property endOffset should not be set yet");
                    m_result->setEndOffset(value);
                    return castState<EndOffsetSet>();
                }

                RuleUsageBuilder<STATE | UsedSet>& setUsed(bool value) {
                    static_assert(!(STATE & UsedSet), "property used should not be set yet");
                    m_result->setUsed(value);
                    return castState<UsedSet>();
                }

                std::unique_ptr<RuleUsage> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class RuleUsage;
                RuleUsageBuilder() : m_result(new RuleUsage()) { }

                template<int STEP> RuleUsageBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<RuleUsageBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::RuleUsage> m_result;
        };

        static RuleUsageBuilder<0> create() {
            return RuleUsageBuilder<0>();
        }

    private:
        RuleUsage() {
            m_startOffset = 0;
            m_endOffset = 0;
            m_used = false;
        }

        String m_styleSheetId;
        double m_startOffset;
        double m_endOffset;
        bool m_used;
};


class  SourceRange : public Serializable {
        PROTOCOL_DISALLOW_COPY(SourceRange);
    public:
        static std::unique_ptr<SourceRange> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~SourceRange() override { }

        int getStartLine() {
            return m_startLine;
        }
        void setStartLine(int value) {
            m_startLine = value;
        }

        int getStartColumn() {
            return m_startColumn;
        }
        void setStartColumn(int value) {
            m_startColumn = value;
        }

        int getEndLine() {
            return m_endLine;
        }
        void setEndLine(int value) {
            m_endLine = value;
        }

        int getEndColumn() {
            return m_endColumn;
        }
        void setEndColumn(int value) {
            m_endColumn = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<SourceRange> clone() const;

        template<int STATE>
        class SourceRangeBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    StartLineSet = 1 << 1,
                    StartColumnSet = 1 << 2,
                    EndLineSet = 1 << 3,
                    EndColumnSet = 1 << 4,
                    AllFieldsSet = (StartLineSet | StartColumnSet | EndLineSet | EndColumnSet | 0)
                };


                SourceRangeBuilder<STATE | StartLineSet>& setStartLine(int value) {
                    static_assert(!(STATE & StartLineSet), "property startLine should not be set yet");
                    m_result->setStartLine(value);
                    return castState<StartLineSet>();
                }

                SourceRangeBuilder<STATE | StartColumnSet>& setStartColumn(int value) {
                    static_assert(!(STATE & StartColumnSet), "property startColumn should not be set yet");
                    m_result->setStartColumn(value);
                    return castState<StartColumnSet>();
                }

                SourceRangeBuilder<STATE | EndLineSet>& setEndLine(int value) {
                    static_assert(!(STATE & EndLineSet), "property endLine should not be set yet");
                    m_result->setEndLine(value);
                    return castState<EndLineSet>();
                }

                SourceRangeBuilder<STATE | EndColumnSet>& setEndColumn(int value) {
                    static_assert(!(STATE & EndColumnSet), "property endColumn should not be set yet");
                    m_result->setEndColumn(value);
                    return castState<EndColumnSet>();
                }

                std::unique_ptr<SourceRange> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class SourceRange;
                SourceRangeBuilder() : m_result(new SourceRange()) { }

                template<int STEP> SourceRangeBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<SourceRangeBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::SourceRange> m_result;
        };

        static SourceRangeBuilder<0> create() {
            return SourceRangeBuilder<0>();
        }

    private:
        SourceRange() {
            m_startLine = 0;
            m_startColumn = 0;
            m_endLine = 0;
            m_endColumn = 0;
        }

        int m_startLine;
        int m_startColumn;
        int m_endLine;
        int m_endColumn;
};


class  ShorthandEntry : public Serializable {
        PROTOCOL_DISALLOW_COPY(ShorthandEntry);
    public:
        static std::unique_ptr<ShorthandEntry> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ShorthandEntry() override { }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        String getValue() {
            return m_value;
        }
        void setValue(const String& value) {
            m_value = value;
        }

        bool hasImportant() {
            return m_important.isJust();
        }
        bool getImportant(bool defaultValue) {
            return m_important.isJust() ? m_important.fromJust() : defaultValue;
        }
        void setImportant(bool value) {
            m_important = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ShorthandEntry> clone() const;

        template<int STATE>
        class ShorthandEntryBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    ValueSet = 1 << 2,
                    AllFieldsSet = (NameSet | ValueSet | 0)
                };


                ShorthandEntryBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                ShorthandEntryBuilder<STATE | ValueSet>& setValue(const String& value) {
                    static_assert(!(STATE & ValueSet), "property value should not be set yet");
                    m_result->setValue(value);
                    return castState<ValueSet>();
                }

                ShorthandEntryBuilder<STATE>& setImportant(bool value) {
                    m_result->setImportant(value);
                    return *this;
                }

                std::unique_ptr<ShorthandEntry> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ShorthandEntry;
                ShorthandEntryBuilder() : m_result(new ShorthandEntry()) { }

                template<int STEP> ShorthandEntryBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ShorthandEntryBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::ShorthandEntry> m_result;
        };

        static ShorthandEntryBuilder<0> create() {
            return ShorthandEntryBuilder<0>();
        }

    private:
        ShorthandEntry() {
        }

        String m_name;
        String m_value;
        Maybe<bool> m_important;
};


class  CSSComputedStyleProperty : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSComputedStyleProperty);
    public:
        static std::unique_ptr<CSSComputedStyleProperty> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSComputedStyleProperty() override { }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        String getValue() {
            return m_value;
        }
        void setValue(const String& value) {
            m_value = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSComputedStyleProperty> clone() const;

        template<int STATE>
        class CSSComputedStylePropertyBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    ValueSet = 1 << 2,
                    AllFieldsSet = (NameSet | ValueSet | 0)
                };


                CSSComputedStylePropertyBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                CSSComputedStylePropertyBuilder<STATE | ValueSet>& setValue(const String& value) {
                    static_assert(!(STATE & ValueSet), "property value should not be set yet");
                    m_result->setValue(value);
                    return castState<ValueSet>();
                }

                std::unique_ptr<CSSComputedStyleProperty> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSComputedStyleProperty;
                CSSComputedStylePropertyBuilder() : m_result(new CSSComputedStyleProperty()) { }

                template<int STEP> CSSComputedStylePropertyBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSComputedStylePropertyBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSComputedStyleProperty> m_result;
        };

        static CSSComputedStylePropertyBuilder<0> create() {
            return CSSComputedStylePropertyBuilder<0>();
        }

    private:
        CSSComputedStyleProperty() {
        }

        String m_name;
        String m_value;
};


class  CSSStyle : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSStyle);
    public:
        static std::unique_ptr<CSSStyle> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSStyle() override { }

        bool hasStyleSheetId() {
            return m_styleSheetId.isJust();
        }
        String getStyleSheetId(const String& defaultValue) {
            return m_styleSheetId.isJust() ? m_styleSheetId.fromJust() : defaultValue;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        protocol::Array<protocol::CSS::CSSProperty>* getCssProperties() {
            return m_cssProperties.get();
        }
        void setCssProperties(std::unique_ptr<protocol::Array<protocol::CSS::CSSProperty>> value) {
            m_cssProperties = std::move(value);
        }

        protocol::Array<protocol::CSS::ShorthandEntry>* getShorthandEntries() {
            return m_shorthandEntries.get();
        }
        void setShorthandEntries(std::unique_ptr<protocol::Array<protocol::CSS::ShorthandEntry>> value) {
            m_shorthandEntries = std::move(value);
        }

        bool hasCssText() {
            return m_cssText.isJust();
        }
        String getCssText(const String& defaultValue) {
            return m_cssText.isJust() ? m_cssText.fromJust() : defaultValue;
        }
        void setCssText(const String& value) {
            m_cssText = value;
        }

        bool hasRange() {
            return m_range.isJust();
        }
        protocol::CSS::SourceRange* getRange(protocol::CSS::SourceRange* defaultValue) {
            return m_range.isJust() ? m_range.fromJust() : defaultValue;
        }
        void setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
            m_range = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSStyle> clone() const;

        template<int STATE>
        class CSSStyleBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    CssPropertiesSet = 1 << 1,
                    ShorthandEntriesSet = 1 << 2,
                    AllFieldsSet = (CssPropertiesSet | ShorthandEntriesSet | 0)
                };


                CSSStyleBuilder<STATE>& setStyleSheetId(const String& value) {
                    m_result->setStyleSheetId(value);
                    return *this;
                }

                CSSStyleBuilder<STATE | CssPropertiesSet>& setCssProperties(std::unique_ptr<protocol::Array<protocol::CSS::CSSProperty>> value) {
                    static_assert(!(STATE & CssPropertiesSet), "property cssProperties should not be set yet");
                    m_result->setCssProperties(std::move(value));
                    return castState<CssPropertiesSet>();
                }

                CSSStyleBuilder<STATE | ShorthandEntriesSet>& setShorthandEntries(std::unique_ptr<protocol::Array<protocol::CSS::ShorthandEntry>> value) {
                    static_assert(!(STATE & ShorthandEntriesSet), "property shorthandEntries should not be set yet");
                    m_result->setShorthandEntries(std::move(value));
                    return castState<ShorthandEntriesSet>();
                }

                CSSStyleBuilder<STATE>& setCssText(const String& value) {
                    m_result->setCssText(value);
                    return *this;
                }

                CSSStyleBuilder<STATE>& setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
                    m_result->setRange(std::move(value));
                    return *this;
                }

                std::unique_ptr<CSSStyle> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSStyle;
                CSSStyleBuilder() : m_result(new CSSStyle()) { }

                template<int STEP> CSSStyleBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSStyleBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSStyle> m_result;
        };

        static CSSStyleBuilder<0> create() {
            return CSSStyleBuilder<0>();
        }

    private:
        CSSStyle() {
        }

        Maybe<String> m_styleSheetId;
        std::unique_ptr<protocol::Array<protocol::CSS::CSSProperty>> m_cssProperties;
        std::unique_ptr<protocol::Array<protocol::CSS::ShorthandEntry>> m_shorthandEntries;
        Maybe<String> m_cssText;
        Maybe<protocol::CSS::SourceRange> m_range;
};


class  CSSProperty : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSProperty);
    public:
        static std::unique_ptr<CSSProperty> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSProperty() override { }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        String getValue() {
            return m_value;
        }
        void setValue(const String& value) {
            m_value = value;
        }

        bool hasImportant() {
            return m_important.isJust();
        }
        bool getImportant(bool defaultValue) {
            return m_important.isJust() ? m_important.fromJust() : defaultValue;
        }
        void setImportant(bool value) {
            m_important = value;
        }

        bool hasImplicit() {
            return m_implicit.isJust();
        }
        bool getImplicit(bool defaultValue) {
            return m_implicit.isJust() ? m_implicit.fromJust() : defaultValue;
        }
        void setImplicit(bool value) {
            m_implicit = value;
        }

        bool hasText() {
            return m_text.isJust();
        }
        String getText(const String& defaultValue) {
            return m_text.isJust() ? m_text.fromJust() : defaultValue;
        }
        void setText(const String& value) {
            m_text = value;
        }

        bool hasParsedOk() {
            return m_parsedOk.isJust();
        }
        bool getParsedOk(bool defaultValue) {
            return m_parsedOk.isJust() ? m_parsedOk.fromJust() : defaultValue;
        }
        void setParsedOk(bool value) {
            m_parsedOk = value;
        }

        bool hasDisabled() {
            return m_disabled.isJust();
        }
        bool getDisabled(bool defaultValue) {
            return m_disabled.isJust() ? m_disabled.fromJust() : defaultValue;
        }
        void setDisabled(bool value) {
            m_disabled = value;
        }

        bool hasRange() {
            return m_range.isJust();
        }
        protocol::CSS::SourceRange* getRange(protocol::CSS::SourceRange* defaultValue) {
            return m_range.isJust() ? m_range.fromJust() : defaultValue;
        }
        void setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
            m_range = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSProperty> clone() const;

        template<int STATE>
        class CSSPropertyBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    ValueSet = 1 << 2,
                    AllFieldsSet = (NameSet | ValueSet | 0)
                };


                CSSPropertyBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                CSSPropertyBuilder<STATE | ValueSet>& setValue(const String& value) {
                    static_assert(!(STATE & ValueSet), "property value should not be set yet");
                    m_result->setValue(value);
                    return castState<ValueSet>();
                }

                CSSPropertyBuilder<STATE>& setImportant(bool value) {
                    m_result->setImportant(value);
                    return *this;
                }

                CSSPropertyBuilder<STATE>& setImplicit(bool value) {
                    m_result->setImplicit(value);
                    return *this;
                }

                CSSPropertyBuilder<STATE>& setText(const String& value) {
                    m_result->setText(value);
                    return *this;
                }

                CSSPropertyBuilder<STATE>& setParsedOk(bool value) {
                    m_result->setParsedOk(value);
                    return *this;
                }

                CSSPropertyBuilder<STATE>& setDisabled(bool value) {
                    m_result->setDisabled(value);
                    return *this;
                }

                CSSPropertyBuilder<STATE>& setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
                    m_result->setRange(std::move(value));
                    return *this;
                }

                std::unique_ptr<CSSProperty> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSProperty;
                CSSPropertyBuilder() : m_result(new CSSProperty()) { }

                template<int STEP> CSSPropertyBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSPropertyBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSProperty> m_result;
        };

        static CSSPropertyBuilder<0> create() {
            return CSSPropertyBuilder<0>();
        }

    private:
        CSSProperty() {
        }

        String m_name;
        String m_value;
        Maybe<bool> m_important;
        Maybe<bool> m_implicit;
        Maybe<String> m_text;
        Maybe<bool> m_parsedOk;
        Maybe<bool> m_disabled;
        Maybe<protocol::CSS::SourceRange> m_range;
};


class  CSSMedia : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSMedia);
    public:
        static std::unique_ptr<CSSMedia> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSMedia() override { }

        String getText() {
            return m_text;
        }
        void setText(const String& value) {
            m_text = value;
        }

        struct  SourceEnum {
            static const char* MediaRule;
            static const char* ImportRule;
            static const char* LinkedSheet;
            static const char* InlineSheet;
        }; // SourceEnum

        String getSource() {
            return m_source;
        }
        void setSource(const String& value) {
            m_source = value;
        }

        bool hasSourceURL() {
            return m_sourceURL.isJust();
        }
        String getSourceURL(const String& defaultValue) {
            return m_sourceURL.isJust() ? m_sourceURL.fromJust() : defaultValue;
        }
        void setSourceURL(const String& value) {
            m_sourceURL = value;
        }

        bool hasRange() {
            return m_range.isJust();
        }
        protocol::CSS::SourceRange* getRange(protocol::CSS::SourceRange* defaultValue) {
            return m_range.isJust() ? m_range.fromJust() : defaultValue;
        }
        void setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
            m_range = std::move(value);
        }

        bool hasStyleSheetId() {
            return m_styleSheetId.isJust();
        }
        String getStyleSheetId(const String& defaultValue) {
            return m_styleSheetId.isJust() ? m_styleSheetId.fromJust() : defaultValue;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        bool hasMediaList() {
            return m_mediaList.isJust();
        }
        protocol::Array<protocol::CSS::MediaQuery>* getMediaList(protocol::Array<protocol::CSS::MediaQuery>* defaultValue) {
            return m_mediaList.isJust() ? m_mediaList.fromJust() : defaultValue;
        }
        void setMediaList(std::unique_ptr<protocol::Array<protocol::CSS::MediaQuery>> value) {
            m_mediaList = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSMedia> clone() const;

        template<int STATE>
        class CSSMediaBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TextSet = 1 << 1,
                    SourceSet = 1 << 2,
                    AllFieldsSet = (TextSet | SourceSet | 0)
                };


                CSSMediaBuilder<STATE | TextSet>& setText(const String& value) {
                    static_assert(!(STATE & TextSet), "property text should not be set yet");
                    m_result->setText(value);
                    return castState<TextSet>();
                }

                CSSMediaBuilder<STATE | SourceSet>& setSource(const String& value) {
                    static_assert(!(STATE & SourceSet), "property source should not be set yet");
                    m_result->setSource(value);
                    return castState<SourceSet>();
                }

                CSSMediaBuilder<STATE>& setSourceURL(const String& value) {
                    m_result->setSourceURL(value);
                    return *this;
                }

                CSSMediaBuilder<STATE>& setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
                    m_result->setRange(std::move(value));
                    return *this;
                }

                CSSMediaBuilder<STATE>& setStyleSheetId(const String& value) {
                    m_result->setStyleSheetId(value);
                    return *this;
                }

                CSSMediaBuilder<STATE>& setMediaList(std::unique_ptr<protocol::Array<protocol::CSS::MediaQuery>> value) {
                    m_result->setMediaList(std::move(value));
                    return *this;
                }

                std::unique_ptr<CSSMedia> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSMedia;
                CSSMediaBuilder() : m_result(new CSSMedia()) { }

                template<int STEP> CSSMediaBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSMediaBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSMedia> m_result;
        };

        static CSSMediaBuilder<0> create() {
            return CSSMediaBuilder<0>();
        }

    private:
        CSSMedia() {
        }

        String m_text;
        String m_source;
        Maybe<String> m_sourceURL;
        Maybe<protocol::CSS::SourceRange> m_range;
        Maybe<String> m_styleSheetId;
        Maybe<protocol::Array<protocol::CSS::MediaQuery>> m_mediaList;
};


class  MediaQuery : public Serializable {
        PROTOCOL_DISALLOW_COPY(MediaQuery);
    public:
        static std::unique_ptr<MediaQuery> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~MediaQuery() override { }

        protocol::Array<protocol::CSS::MediaQueryExpression>* getExpressions() {
            return m_expressions.get();
        }
        void setExpressions(std::unique_ptr<protocol::Array<protocol::CSS::MediaQueryExpression>> value) {
            m_expressions = std::move(value);
        }

        bool getActive() {
            return m_active;
        }
        void setActive(bool value) {
            m_active = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<MediaQuery> clone() const;

        template<int STATE>
        class MediaQueryBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ExpressionsSet = 1 << 1,
                    ActiveSet = 1 << 2,
                    AllFieldsSet = (ExpressionsSet | ActiveSet | 0)
                };


                MediaQueryBuilder<STATE | ExpressionsSet>& setExpressions(std::unique_ptr<protocol::Array<protocol::CSS::MediaQueryExpression>> value) {
                    static_assert(!(STATE & ExpressionsSet), "property expressions should not be set yet");
                    m_result->setExpressions(std::move(value));
                    return castState<ExpressionsSet>();
                }

                MediaQueryBuilder<STATE | ActiveSet>& setActive(bool value) {
                    static_assert(!(STATE & ActiveSet), "property active should not be set yet");
                    m_result->setActive(value);
                    return castState<ActiveSet>();
                }

                std::unique_ptr<MediaQuery> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class MediaQuery;
                MediaQueryBuilder() : m_result(new MediaQuery()) { }

                template<int STEP> MediaQueryBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<MediaQueryBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::MediaQuery> m_result;
        };

        static MediaQueryBuilder<0> create() {
            return MediaQueryBuilder<0>();
        }

    private:
        MediaQuery() {
            m_active = false;
        }

        std::unique_ptr<protocol::Array<protocol::CSS::MediaQueryExpression>> m_expressions;
        bool m_active;
};


class  MediaQueryExpression : public Serializable {
        PROTOCOL_DISALLOW_COPY(MediaQueryExpression);
    public:
        static std::unique_ptr<MediaQueryExpression> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~MediaQueryExpression() override { }

        double getValue() {
            return m_value;
        }
        void setValue(double value) {
            m_value = value;
        }

        String getUnit() {
            return m_unit;
        }
        void setUnit(const String& value) {
            m_unit = value;
        }

        String getFeature() {
            return m_feature;
        }
        void setFeature(const String& value) {
            m_feature = value;
        }

        bool hasValueRange() {
            return m_valueRange.isJust();
        }
        protocol::CSS::SourceRange* getValueRange(protocol::CSS::SourceRange* defaultValue) {
            return m_valueRange.isJust() ? m_valueRange.fromJust() : defaultValue;
        }
        void setValueRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
            m_valueRange = std::move(value);
        }

        bool hasComputedLength() {
            return m_computedLength.isJust();
        }
        double getComputedLength(double defaultValue) {
            return m_computedLength.isJust() ? m_computedLength.fromJust() : defaultValue;
        }
        void setComputedLength(double value) {
            m_computedLength = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<MediaQueryExpression> clone() const;

        template<int STATE>
        class MediaQueryExpressionBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ValueSet = 1 << 1,
                    UnitSet = 1 << 2,
                    FeatureSet = 1 << 3,
                    AllFieldsSet = (ValueSet | UnitSet | FeatureSet | 0)
                };


                MediaQueryExpressionBuilder<STATE | ValueSet>& setValue(double value) {
                    static_assert(!(STATE & ValueSet), "property value should not be set yet");
                    m_result->setValue(value);
                    return castState<ValueSet>();
                }

                MediaQueryExpressionBuilder<STATE | UnitSet>& setUnit(const String& value) {
                    static_assert(!(STATE & UnitSet), "property unit should not be set yet");
                    m_result->setUnit(value);
                    return castState<UnitSet>();
                }

                MediaQueryExpressionBuilder<STATE | FeatureSet>& setFeature(const String& value) {
                    static_assert(!(STATE & FeatureSet), "property feature should not be set yet");
                    m_result->setFeature(value);
                    return castState<FeatureSet>();
                }

                MediaQueryExpressionBuilder<STATE>& setValueRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
                    m_result->setValueRange(std::move(value));
                    return *this;
                }

                MediaQueryExpressionBuilder<STATE>& setComputedLength(double value) {
                    m_result->setComputedLength(value);
                    return *this;
                }

                std::unique_ptr<MediaQueryExpression> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class MediaQueryExpression;
                MediaQueryExpressionBuilder() : m_result(new MediaQueryExpression()) { }

                template<int STEP> MediaQueryExpressionBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<MediaQueryExpressionBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::MediaQueryExpression> m_result;
        };

        static MediaQueryExpressionBuilder<0> create() {
            return MediaQueryExpressionBuilder<0>();
        }

    private:
        MediaQueryExpression() {
            m_value = 0;
        }

        double m_value;
        String m_unit;
        String m_feature;
        Maybe<protocol::CSS::SourceRange> m_valueRange;
        Maybe<double> m_computedLength;
};


class  PlatformFontUsage : public Serializable {
        PROTOCOL_DISALLOW_COPY(PlatformFontUsage);
    public:
        static std::unique_ptr<PlatformFontUsage> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~PlatformFontUsage() override { }

        String getFamilyName() {
            return m_familyName;
        }
        void setFamilyName(const String& value) {
            m_familyName = value;
        }

        bool getIsCustomFont() {
            return m_isCustomFont;
        }
        void setIsCustomFont(bool value) {
            m_isCustomFont = value;
        }

        double getGlyphCount() {
            return m_glyphCount;
        }
        void setGlyphCount(double value) {
            m_glyphCount = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<PlatformFontUsage> clone() const;

        template<int STATE>
        class PlatformFontUsageBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    FamilyNameSet = 1 << 1,
                    IsCustomFontSet = 1 << 2,
                    GlyphCountSet = 1 << 3,
                    AllFieldsSet = (FamilyNameSet | IsCustomFontSet | GlyphCountSet | 0)
                };


                PlatformFontUsageBuilder<STATE | FamilyNameSet>& setFamilyName(const String& value) {
                    static_assert(!(STATE & FamilyNameSet), "property familyName should not be set yet");
                    m_result->setFamilyName(value);
                    return castState<FamilyNameSet>();
                }

                PlatformFontUsageBuilder<STATE | IsCustomFontSet>& setIsCustomFont(bool value) {
                    static_assert(!(STATE & IsCustomFontSet), "property isCustomFont should not be set yet");
                    m_result->setIsCustomFont(value);
                    return castState<IsCustomFontSet>();
                }

                PlatformFontUsageBuilder<STATE | GlyphCountSet>& setGlyphCount(double value) {
                    static_assert(!(STATE & GlyphCountSet), "property glyphCount should not be set yet");
                    m_result->setGlyphCount(value);
                    return castState<GlyphCountSet>();
                }

                std::unique_ptr<PlatformFontUsage> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class PlatformFontUsage;
                PlatformFontUsageBuilder() : m_result(new PlatformFontUsage()) { }

                template<int STEP> PlatformFontUsageBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<PlatformFontUsageBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::PlatformFontUsage> m_result;
        };

        static PlatformFontUsageBuilder<0> create() {
            return PlatformFontUsageBuilder<0>();
        }

    private:
        PlatformFontUsage() {
            m_isCustomFont = false;
            m_glyphCount = 0;
        }

        String m_familyName;
        bool m_isCustomFont;
        double m_glyphCount;
};


class  FontFace : public Serializable {
        PROTOCOL_DISALLOW_COPY(FontFace);
    public:
        static std::unique_ptr<FontFace> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~FontFace() override { }

        String getFontFamily() {
            return m_fontFamily;
        }
        void setFontFamily(const String& value) {
            m_fontFamily = value;
        }

        String getFontStyle() {
            return m_fontStyle;
        }
        void setFontStyle(const String& value) {
            m_fontStyle = value;
        }

        String getFontVariant() {
            return m_fontVariant;
        }
        void setFontVariant(const String& value) {
            m_fontVariant = value;
        }

        String getFontWeight() {
            return m_fontWeight;
        }
        void setFontWeight(const String& value) {
            m_fontWeight = value;
        }

        String getFontStretch() {
            return m_fontStretch;
        }
        void setFontStretch(const String& value) {
            m_fontStretch = value;
        }

        String getUnicodeRange() {
            return m_unicodeRange;
        }
        void setUnicodeRange(const String& value) {
            m_unicodeRange = value;
        }

        String getSrc() {
            return m_src;
        }
        void setSrc(const String& value) {
            m_src = value;
        }

        String getPlatformFontFamily() {
            return m_platformFontFamily;
        }
        void setPlatformFontFamily(const String& value) {
            m_platformFontFamily = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<FontFace> clone() const;

        template<int STATE>
        class FontFaceBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    FontFamilySet = 1 << 1,
                    FontStyleSet = 1 << 2,
                    FontVariantSet = 1 << 3,
                    FontWeightSet = 1 << 4,
                    FontStretchSet = 1 << 5,
                    UnicodeRangeSet = 1 << 6,
                    SrcSet = 1 << 7,
                    PlatformFontFamilySet = 1 << 8,
                    AllFieldsSet = (FontFamilySet | FontStyleSet | FontVariantSet | FontWeightSet | FontStretchSet | UnicodeRangeSet | SrcSet | PlatformFontFamilySet | 0)
                };


                FontFaceBuilder<STATE | FontFamilySet>& setFontFamily(const String& value) {
                    static_assert(!(STATE & FontFamilySet), "property fontFamily should not be set yet");
                    m_result->setFontFamily(value);
                    return castState<FontFamilySet>();
                }

                FontFaceBuilder<STATE | FontStyleSet>& setFontStyle(const String& value) {
                    static_assert(!(STATE & FontStyleSet), "property fontStyle should not be set yet");
                    m_result->setFontStyle(value);
                    return castState<FontStyleSet>();
                }

                FontFaceBuilder<STATE | FontVariantSet>& setFontVariant(const String& value) {
                    static_assert(!(STATE & FontVariantSet), "property fontVariant should not be set yet");
                    m_result->setFontVariant(value);
                    return castState<FontVariantSet>();
                }

                FontFaceBuilder<STATE | FontWeightSet>& setFontWeight(const String& value) {
                    static_assert(!(STATE & FontWeightSet), "property fontWeight should not be set yet");
                    m_result->setFontWeight(value);
                    return castState<FontWeightSet>();
                }

                FontFaceBuilder<STATE | FontStretchSet>& setFontStretch(const String& value) {
                    static_assert(!(STATE & FontStretchSet), "property fontStretch should not be set yet");
                    m_result->setFontStretch(value);
                    return castState<FontStretchSet>();
                }

                FontFaceBuilder<STATE | UnicodeRangeSet>& setUnicodeRange(const String& value) {
                    static_assert(!(STATE & UnicodeRangeSet), "property unicodeRange should not be set yet");
                    m_result->setUnicodeRange(value);
                    return castState<UnicodeRangeSet>();
                }

                FontFaceBuilder<STATE | SrcSet>& setSrc(const String& value) {
                    static_assert(!(STATE & SrcSet), "property src should not be set yet");
                    m_result->setSrc(value);
                    return castState<SrcSet>();
                }

                FontFaceBuilder<STATE | PlatformFontFamilySet>& setPlatformFontFamily(const String& value) {
                    static_assert(!(STATE & PlatformFontFamilySet), "property platformFontFamily should not be set yet");
                    m_result->setPlatformFontFamily(value);
                    return castState<PlatformFontFamilySet>();
                }

                std::unique_ptr<FontFace> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class FontFace;
                FontFaceBuilder() : m_result(new FontFace()) { }

                template<int STEP> FontFaceBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FontFaceBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::FontFace> m_result;
        };

        static FontFaceBuilder<0> create() {
            return FontFaceBuilder<0>();
        }

    private:
        FontFace() {
        }

        String m_fontFamily;
        String m_fontStyle;
        String m_fontVariant;
        String m_fontWeight;
        String m_fontStretch;
        String m_unicodeRange;
        String m_src;
        String m_platformFontFamily;
};


class  CSSKeyframesRule : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSKeyframesRule);
    public:
        static std::unique_ptr<CSSKeyframesRule> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSKeyframesRule() override { }

        protocol::CSS::Value* getAnimationName() {
            return m_animationName.get();
        }
        void setAnimationName(std::unique_ptr<protocol::CSS::Value> value) {
            m_animationName = std::move(value);
        }

        protocol::Array<protocol::CSS::CSSKeyframeRule>* getKeyframes() {
            return m_keyframes.get();
        }
        void setKeyframes(std::unique_ptr<protocol::Array<protocol::CSS::CSSKeyframeRule>> value) {
            m_keyframes = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSKeyframesRule> clone() const;

        template<int STATE>
        class CSSKeyframesRuleBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    AnimationNameSet = 1 << 1,
                    KeyframesSet = 1 << 2,
                    AllFieldsSet = (AnimationNameSet | KeyframesSet | 0)
                };


                CSSKeyframesRuleBuilder<STATE | AnimationNameSet>& setAnimationName(std::unique_ptr<protocol::CSS::Value> value) {
                    static_assert(!(STATE & AnimationNameSet), "property animationName should not be set yet");
                    m_result->setAnimationName(std::move(value));
                    return castState<AnimationNameSet>();
                }

                CSSKeyframesRuleBuilder<STATE | KeyframesSet>& setKeyframes(std::unique_ptr<protocol::Array<protocol::CSS::CSSKeyframeRule>> value) {
                    static_assert(!(STATE & KeyframesSet), "property keyframes should not be set yet");
                    m_result->setKeyframes(std::move(value));
                    return castState<KeyframesSet>();
                }

                std::unique_ptr<CSSKeyframesRule> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSKeyframesRule;
                CSSKeyframesRuleBuilder() : m_result(new CSSKeyframesRule()) { }

                template<int STEP> CSSKeyframesRuleBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSKeyframesRuleBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSKeyframesRule> m_result;
        };

        static CSSKeyframesRuleBuilder<0> create() {
            return CSSKeyframesRuleBuilder<0>();
        }

    private:
        CSSKeyframesRule() {
        }

        std::unique_ptr<protocol::CSS::Value> m_animationName;
        std::unique_ptr<protocol::Array<protocol::CSS::CSSKeyframeRule>> m_keyframes;
};


class  CSSKeyframeRule : public Serializable {
        PROTOCOL_DISALLOW_COPY(CSSKeyframeRule);
    public:
        static std::unique_ptr<CSSKeyframeRule> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CSSKeyframeRule() override { }

        bool hasStyleSheetId() {
            return m_styleSheetId.isJust();
        }
        String getStyleSheetId(const String& defaultValue) {
            return m_styleSheetId.isJust() ? m_styleSheetId.fromJust() : defaultValue;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        String getOrigin() {
            return m_origin;
        }
        void setOrigin(const String& value) {
            m_origin = value;
        }

        protocol::CSS::Value* getKeyText() {
            return m_keyText.get();
        }
        void setKeyText(std::unique_ptr<protocol::CSS::Value> value) {
            m_keyText = std::move(value);
        }

        protocol::CSS::CSSStyle* getStyle() {
            return m_style.get();
        }
        void setStyle(std::unique_ptr<protocol::CSS::CSSStyle> value) {
            m_style = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CSSKeyframeRule> clone() const;

        template<int STATE>
        class CSSKeyframeRuleBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    OriginSet = 1 << 1,
                    KeyTextSet = 1 << 2,
                    StyleSet = 1 << 3,
                    AllFieldsSet = (OriginSet | KeyTextSet | StyleSet | 0)
                };


                CSSKeyframeRuleBuilder<STATE>& setStyleSheetId(const String& value) {
                    m_result->setStyleSheetId(value);
                    return *this;
                }

                CSSKeyframeRuleBuilder<STATE | OriginSet>& setOrigin(const String& value) {
                    static_assert(!(STATE & OriginSet), "property origin should not be set yet");
                    m_result->setOrigin(value);
                    return castState<OriginSet>();
                }

                CSSKeyframeRuleBuilder<STATE | KeyTextSet>& setKeyText(std::unique_ptr<protocol::CSS::Value> value) {
                    static_assert(!(STATE & KeyTextSet), "property keyText should not be set yet");
                    m_result->setKeyText(std::move(value));
                    return castState<KeyTextSet>();
                }

                CSSKeyframeRuleBuilder<STATE | StyleSet>& setStyle(std::unique_ptr<protocol::CSS::CSSStyle> value) {
                    static_assert(!(STATE & StyleSet), "property style should not be set yet");
                    m_result->setStyle(std::move(value));
                    return castState<StyleSet>();
                }

                std::unique_ptr<CSSKeyframeRule> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CSSKeyframeRule;
                CSSKeyframeRuleBuilder() : m_result(new CSSKeyframeRule()) { }

                template<int STEP> CSSKeyframeRuleBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CSSKeyframeRuleBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::CSSKeyframeRule> m_result;
        };

        static CSSKeyframeRuleBuilder<0> create() {
            return CSSKeyframeRuleBuilder<0>();
        }

    private:
        CSSKeyframeRule() {
        }

        Maybe<String> m_styleSheetId;
        String m_origin;
        std::unique_ptr<protocol::CSS::Value> m_keyText;
        std::unique_ptr<protocol::CSS::CSSStyle> m_style;
};


class  StyleDeclarationEdit : public Serializable {
        PROTOCOL_DISALLOW_COPY(StyleDeclarationEdit);
    public:
        static std::unique_ptr<StyleDeclarationEdit> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~StyleDeclarationEdit() override { }

        String getStyleSheetId() {
            return m_styleSheetId;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        protocol::CSS::SourceRange* getRange() {
            return m_range.get();
        }
        void setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
            m_range = std::move(value);
        }

        String getText() {
            return m_text;
        }
        void setText(const String& value) {
            m_text = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<StyleDeclarationEdit> clone() const;

        template<int STATE>
        class StyleDeclarationEditBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    StyleSheetIdSet = 1 << 1,
                    RangeSet = 1 << 2,
                    TextSet = 1 << 3,
                    AllFieldsSet = (StyleSheetIdSet | RangeSet | TextSet | 0)
                };


                StyleDeclarationEditBuilder<STATE | StyleSheetIdSet>& setStyleSheetId(const String& value) {
                    static_assert(!(STATE & StyleSheetIdSet), "property styleSheetId should not be set yet");
                    m_result->setStyleSheetId(value);
                    return castState<StyleSheetIdSet>();
                }

                StyleDeclarationEditBuilder<STATE | RangeSet>& setRange(std::unique_ptr<protocol::CSS::SourceRange> value) {
                    static_assert(!(STATE & RangeSet), "property range should not be set yet");
                    m_result->setRange(std::move(value));
                    return castState<RangeSet>();
                }

                StyleDeclarationEditBuilder<STATE | TextSet>& setText(const String& value) {
                    static_assert(!(STATE & TextSet), "property text should not be set yet");
                    m_result->setText(value);
                    return castState<TextSet>();
                }

                std::unique_ptr<StyleDeclarationEdit> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class StyleDeclarationEdit;
                StyleDeclarationEditBuilder() : m_result(new StyleDeclarationEdit()) { }

                template<int STEP> StyleDeclarationEditBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<StyleDeclarationEditBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::StyleDeclarationEdit> m_result;
        };

        static StyleDeclarationEditBuilder<0> create() {
            return StyleDeclarationEditBuilder<0>();
        }

    private:
        StyleDeclarationEdit() {
        }

        String m_styleSheetId;
        std::unique_ptr<protocol::CSS::SourceRange> m_range;
        String m_text;
};


class  FontsUpdatedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(FontsUpdatedNotification);
    public:
        static std::unique_ptr<FontsUpdatedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~FontsUpdatedNotification() override { }

        bool hasFont() {
            return m_font.isJust();
        }
        protocol::CSS::FontFace* getFont(protocol::CSS::FontFace* defaultValue) {
            return m_font.isJust() ? m_font.fromJust() : defaultValue;
        }
        void setFont(std::unique_ptr<protocol::CSS::FontFace> value) {
            m_font = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<FontsUpdatedNotification> clone() const;

        template<int STATE>
        class FontsUpdatedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    AllFieldsSet = (0)
                };


                FontsUpdatedNotificationBuilder<STATE>& setFont(std::unique_ptr<protocol::CSS::FontFace> value) {
                    m_result->setFont(std::move(value));
                    return *this;
                }

                std::unique_ptr<FontsUpdatedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class FontsUpdatedNotification;
                FontsUpdatedNotificationBuilder() : m_result(new FontsUpdatedNotification()) { }

                template<int STEP> FontsUpdatedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<FontsUpdatedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::FontsUpdatedNotification> m_result;
        };

        static FontsUpdatedNotificationBuilder<0> create() {
            return FontsUpdatedNotificationBuilder<0>();
        }

    private:
        FontsUpdatedNotification() {
        }

        Maybe<protocol::CSS::FontFace> m_font;
};


class  StyleSheetAddedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(StyleSheetAddedNotification);
    public:
        static std::unique_ptr<StyleSheetAddedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~StyleSheetAddedNotification() override { }

        protocol::CSS::CSSStyleSheetHeader* getHeader() {
            return m_header.get();
        }
        void setHeader(std::unique_ptr<protocol::CSS::CSSStyleSheetHeader> value) {
            m_header = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<StyleSheetAddedNotification> clone() const;

        template<int STATE>
        class StyleSheetAddedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    HeaderSet = 1 << 1,
                    AllFieldsSet = (HeaderSet | 0)
                };


                StyleSheetAddedNotificationBuilder<STATE | HeaderSet>& setHeader(std::unique_ptr<protocol::CSS::CSSStyleSheetHeader> value) {
                    static_assert(!(STATE & HeaderSet), "property header should not be set yet");
                    m_result->setHeader(std::move(value));
                    return castState<HeaderSet>();
                }

                std::unique_ptr<StyleSheetAddedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class StyleSheetAddedNotification;
                StyleSheetAddedNotificationBuilder() : m_result(new StyleSheetAddedNotification()) { }

                template<int STEP> StyleSheetAddedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<StyleSheetAddedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::StyleSheetAddedNotification> m_result;
        };

        static StyleSheetAddedNotificationBuilder<0> create() {
            return StyleSheetAddedNotificationBuilder<0>();
        }

    private:
        StyleSheetAddedNotification() {
        }

        std::unique_ptr<protocol::CSS::CSSStyleSheetHeader> m_header;
};


class  StyleSheetChangedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(StyleSheetChangedNotification);
    public:
        static std::unique_ptr<StyleSheetChangedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~StyleSheetChangedNotification() override { }

        String getStyleSheetId() {
            return m_styleSheetId;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<StyleSheetChangedNotification> clone() const;

        template<int STATE>
        class StyleSheetChangedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    StyleSheetIdSet = 1 << 1,
                    AllFieldsSet = (StyleSheetIdSet | 0)
                };


                StyleSheetChangedNotificationBuilder<STATE | StyleSheetIdSet>& setStyleSheetId(const String& value) {
                    static_assert(!(STATE & StyleSheetIdSet), "property styleSheetId should not be set yet");
                    m_result->setStyleSheetId(value);
                    return castState<StyleSheetIdSet>();
                }

                std::unique_ptr<StyleSheetChangedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class StyleSheetChangedNotification;
                StyleSheetChangedNotificationBuilder() : m_result(new StyleSheetChangedNotification()) { }

                template<int STEP> StyleSheetChangedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<StyleSheetChangedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::StyleSheetChangedNotification> m_result;
        };

        static StyleSheetChangedNotificationBuilder<0> create() {
            return StyleSheetChangedNotificationBuilder<0>();
        }

    private:
        StyleSheetChangedNotification() {
        }

        String m_styleSheetId;
};


class  StyleSheetRemovedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(StyleSheetRemovedNotification);
    public:
        static std::unique_ptr<StyleSheetRemovedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~StyleSheetRemovedNotification() override { }

        String getStyleSheetId() {
            return m_styleSheetId;
        }
        void setStyleSheetId(const String& value) {
            m_styleSheetId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<StyleSheetRemovedNotification> clone() const;

        template<int STATE>
        class StyleSheetRemovedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    StyleSheetIdSet = 1 << 1,
                    AllFieldsSet = (StyleSheetIdSet | 0)
                };


                StyleSheetRemovedNotificationBuilder<STATE | StyleSheetIdSet>& setStyleSheetId(const String& value) {
                    static_assert(!(STATE & StyleSheetIdSet), "property styleSheetId should not be set yet");
                    m_result->setStyleSheetId(value);
                    return castState<StyleSheetIdSet>();
                }

                std::unique_ptr<StyleSheetRemovedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class StyleSheetRemovedNotification;
                StyleSheetRemovedNotificationBuilder() : m_result(new StyleSheetRemovedNotification()) { }

                template<int STEP> StyleSheetRemovedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<StyleSheetRemovedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::CSS::StyleSheetRemovedNotification> m_result;
        };

        static StyleSheetRemovedNotificationBuilder<0> create() {
            return StyleSheetRemovedNotificationBuilder<0>();
        }

    private:
        StyleSheetRemovedNotification() {
        }

        String m_styleSheetId;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual DispatchResponse addRule(const String& in_styleSheetId, const String& in_ruleText, std::unique_ptr<protocol::CSS::SourceRange> in_location, std::unique_ptr<protocol::CSS::CSSRule>* out_rule) = 0;
        virtual DispatchResponse collectClassNames(const String& in_styleSheetId, std::unique_ptr<protocol::Array<String>>* out_classNames) = 0;
        virtual DispatchResponse createStyleSheet(const String& in_frameId, String* out_styleSheetId) = 0;
        virtual DispatchResponse disable() = 0;
        virtual DispatchResponse enable() = 0;
        virtual DispatchResponse forcePseudoState(int in_nodeId, std::unique_ptr<protocol::Array<String>> in_forcedPseudoClasses) = 0;
        virtual DispatchResponse getBackgroundColors(int in_nodeId, Maybe<protocol::Array<String>>* out_backgroundColors, Maybe<String>* out_computedFontSize, Maybe<String>* out_computedFontWeight, Maybe<String>* out_computedBodyFontSize) = 0;
        virtual DispatchResponse getComputedStyleForNode(int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>* out_computedStyle) = 0;
        virtual DispatchResponse getInlineStylesForNode(int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle) = 0;
        virtual DispatchResponse getMatchedStylesForNode(int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle, Maybe<protocol::Array<protocol::CSS::RuleMatch>>* out_matchedCSSRules, Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>>* out_pseudoElements, Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>>* out_inherited, Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>>* out_cssKeyframesRules) = 0;
        virtual DispatchResponse getMediaQueries(std::unique_ptr<protocol::Array<protocol::CSS::CSSMedia>>* out_medias) = 0;
        virtual DispatchResponse getPlatformFontsForNode(int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::PlatformFontUsage>>* out_fonts) = 0;
        virtual DispatchResponse getStyleSheetText(const String& in_styleSheetId, String* out_text) = 0;
        virtual DispatchResponse setEffectivePropertyValueForNode(int in_nodeId, const String& in_propertyName, const String& in_value) = 0;
        virtual DispatchResponse setKeyframeKey(const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_keyText, std::unique_ptr<protocol::CSS::Value>* out_keyText) = 0;
        virtual DispatchResponse setMediaText(const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_text, std::unique_ptr<protocol::CSS::CSSMedia>* out_media) = 0;
        virtual DispatchResponse setRuleSelector(const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_selector, std::unique_ptr<protocol::CSS::SelectorList>* out_selectorList) = 0;
        virtual DispatchResponse setStyleSheetText(const String& in_styleSheetId, const String& in_text, Maybe<String>* out_sourceMapURL) = 0;
        virtual DispatchResponse setStyleTexts(std::unique_ptr<protocol::Array<protocol::CSS::StyleDeclarationEdit>> in_edits, std::unique_ptr<protocol::Array<protocol::CSS::CSSStyle>>* out_styles) = 0;
        virtual DispatchResponse startRuleUsageTracking() = 0;
        virtual DispatchResponse stopRuleUsageTracking(std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_ruleUsage) = 0;
        virtual DispatchResponse takeCoverageDelta(std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_coverage) = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void fontsUpdated(Maybe<protocol::CSS::FontFace> font = Maybe<protocol::CSS::FontFace>());
        void mediaQueryResultChanged();
        void styleSheetAdded(std::unique_ptr<protocol::CSS::CSSStyleSheetHeader> header);
        void styleSheetChanged(const String& styleSheetId);
        void styleSheetRemoved(const String& styleSheetId);

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

} // namespace CSS
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_CSS_h)

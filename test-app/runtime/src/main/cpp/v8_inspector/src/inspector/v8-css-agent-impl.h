//
// Created by pkanev on 5/11/2017.
//

#ifndef V8_CSS_AGENT_IMPL_H
#define V8_CSS_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/CSS.h>

namespace v8_inspector {

class V8InspectorSessionImpl;

using protocol::ErrorString;
using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;


class V8CSSAgentImpl : public protocol::CSS::Backend {
    public:
        V8CSSAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                       protocol::DictionaryValue* state);

        ~V8CSSAgentImpl() override;

        void enable(ErrorString*) override;
        void disable(ErrorString*) override;
        void getMatchedStylesForNode(ErrorString*, int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle, Maybe<protocol::Array<protocol::CSS::RuleMatch>>* out_matchedCSSRules, Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>>* out_pseudoElements, Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>>* out_inherited, Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>>* out_cssKeyframesRules) override;
        void getInlineStylesForNode(ErrorString*, int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle) override;
        void getComputedStyleForNode(ErrorString*, int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>* out_computedStyle) override;
        void getPlatformFontsForNode(ErrorString*, int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::PlatformFontUsage>>* out_fonts) override;
        void getStyleSheetText(ErrorString*, const String& in_styleSheetId, String* out_text) override;
        void collectClassNames(ErrorString*, const String& in_styleSheetId, std::unique_ptr<protocol::Array<String>>* out_classNames) override;
        void setStyleSheetText(ErrorString*, const String& in_styleSheetId, const String& in_text, Maybe<String>* out_sourceMapURL) override;
        void setRuleSelector(ErrorString*, const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_selector, std::unique_ptr<protocol::CSS::SelectorList>* out_selectorList) override;
        void setKeyframeKey(ErrorString*, const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_keyText, std::unique_ptr<protocol::CSS::Value>* out_keyText) override;
        void setStyleTexts(ErrorString*, std::unique_ptr<protocol::Array<protocol::CSS::StyleDeclarationEdit>> in_edits, std::unique_ptr<protocol::Array<protocol::CSS::CSSStyle>>* out_styles) override;
        void setMediaText(ErrorString*, const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_text, std::unique_ptr<protocol::CSS::CSSMedia>* out_media) override;
        void createStyleSheet(ErrorString*, const String& in_frameId, String* out_styleSheetId) override;
        void addRule(ErrorString*, const String& in_styleSheetId, const String& in_ruleText, std::unique_ptr<protocol::CSS::SourceRange> in_location, std::unique_ptr<protocol::CSS::CSSRule>* out_rule) override;
        void forcePseudoState(ErrorString*, int in_nodeId, std::unique_ptr<protocol::Array<String>> in_forcedPseudoClasses) override;
        void getMediaQueries(ErrorString*, std::unique_ptr<protocol::Array<protocol::CSS::CSSMedia>>* out_medias) override;
        void setEffectivePropertyValueForNode(ErrorString*, int in_nodeId, const String& in_propertyName, const String& in_value) override;
        void getBackgroundColors(ErrorString*, int in_nodeId, Maybe<protocol::Array<String>>* out_backgroundColors, Maybe<String>* out_computedFontSize, Maybe<String>* out_computedFontWeight, Maybe<String>* out_computedBodyFontSize) override;
        // TODO: Pete: Unnecessary - remove before publish
        void startRuleUsageTracking(ErrorString*) override;
        void takeCoverageDelta(ErrorString*, std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_coverage) override;
        void stopRuleUsageTracking(ErrorString*, std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_ruleUsage) override;

        static V8CSSAgentImpl* Instance;
        protocol::CSS::Frontend m_frontend;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        bool m_enabled;

        DISALLOW_COPY_AND_ASSIGN(V8CSSAgentImpl);
};
}


#endif //V8_CSS_AGENT_IMPL_H

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

        void enable(std::unique_ptr<EnableCallback> callback) override;
        void disable(ErrorString*) override;
        void getMatchedStylesForNode(ErrorString*, int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle, Maybe<protocol::Array<protocol::CSS::RuleMatch>>* out_matchedCSSRules, Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>>* out_pseudoElements, Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>>* out_inherited, Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>>* out_cssKeyframesRules) override;
        void getInlineStylesForNode(ErrorString*, int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle) override;
        void getComputedStyleForNode(ErrorString*, int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>* out_computedStyle) override;
        void getPlatformFontsForNode(ErrorString*, int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::PlatformFontUsage>>* out_fonts) override;
        void getStyleSheetText(ErrorString*, const String& in_styleSheetId, String* out_text) override;

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

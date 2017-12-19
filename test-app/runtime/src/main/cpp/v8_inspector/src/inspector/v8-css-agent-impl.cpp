//
// Created by pkanev on 5/11/2017.
//

#include <NativeScriptAssert.h>
#include <ArgConverter.h>
#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
#include "v8-css-agent-impl.h"

namespace v8_inspector {

using tns::ArgConverter;

namespace CSSAgentState {
static const char cssEnabled[] = "cssEnabled";
}

V8CSSAgentImpl::V8CSSAgentImpl(V8InspectorSessionImpl* session,
                               protocol::FrontendChannel* frontendChannel,
                               protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

V8CSSAgentImpl::~V8CSSAgentImpl() { }

void V8CSSAgentImpl::enable(ErrorString* errorString) {
    if (m_enabled) {
        *errorString = "CSS Agent already enabled!";
        return;
    }

    m_state->setBoolean(CSSAgentState::cssEnabled, true);
    m_enabled = true;
}

void V8CSSAgentImpl::disable(ErrorString*) {
    if (!m_enabled) {
        return;
    }

    m_state->setBoolean(CSSAgentState::cssEnabled, false);

    m_enabled = false;
}

// Not supported
void V8CSSAgentImpl::getMatchedStylesForNode(ErrorString* errorString, int in_nodeId,
        Maybe<protocol::CSS::CSSStyle>* out_inlineStyle,
        Maybe<protocol::CSS::CSSStyle>* out_attributesStyle,
        Maybe<protocol::Array<protocol::CSS::RuleMatch>>* out_matchedCSSRules,
        Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>>* out_pseudoElements,
        Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>>* out_inherited,
        Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>>* out_cssKeyframesRules) {

    //// out_inlineStyle
    auto cssPropsArr = protocol::Array<protocol::CSS::CSSProperty>::create();
    auto shorthandPropArr = protocol::Array<protocol::CSS::ShorthandEntry>::create();
    auto inlineStyle = protocol::CSS::CSSStyle::create()
                       .setCssProperties(std::move(cssPropsArr))
                       .setShorthandEntries(std::move(shorthandPropArr))
                       .build();

    //// out_attributesStyle
    auto attrArr = protocol::Array<protocol::CSS::CSSProperty>::create();
    auto attributeStyle = protocol::CSS::CSSStyle::create()
                          .setCssProperties(std::move(attrArr))
                          .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
                          .build();

    //// out_matchedCSSRules
    auto cssSelectorsArr = protocol::Array<protocol::CSS::Value>::create();
    auto cssSelectorList = protocol::CSS::SelectorList::create()
                           .setSelectors(std::move(cssSelectorsArr))
                           .setText("")
                           .build();

    auto cssRule = protocol::CSS::CSSRule::create()
                   .setSelectorList(std::move(cssSelectorList))
                   .setOrigin(protocol::CSS::StyleSheetOriginEnum::Regular)
                   .setStyle(std::move(protocol::CSS::CSSStyle::create()
                                       .setCssProperties(std::move(protocol::Array<protocol::CSS::CSSProperty>::create()))
                                       .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
                                       .build()))
                   .build();

    auto rulesMatchedArr = protocol::Array<protocol::CSS::RuleMatch>::create();

    //// out_pseudoElements
    auto pseudoElementsArr = protocol::Array<protocol::CSS::PseudoElementMatches>::create();

    //// out_inherited
    auto inheritedElementsArr = protocol::Array<protocol::CSS::InheritedStyleEntry>::create();
    auto inheritedelem = protocol::CSS::InheritedStyleEntry::create()
                         .setInlineStyle(std::move(protocol::CSS::CSSStyle::create()
                                         .setCssProperties(std::move(protocol::Array<protocol::CSS::CSSProperty>::create()))
                                         .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
                                         .build()))
                         .setMatchedCSSRules(std::move(protocol::Array<protocol::CSS::RuleMatch>::create()))
                         .build();
    inheritedElementsArr->addItem(std::move(inheritedelem));

    //// out_cssKeyframesRules
    auto cssKeyFramesRulesArr = protocol::Array<protocol::CSS::CSSKeyframesRule>::create();

    *out_inlineStyle = Maybe<protocol::CSS::CSSStyle>(std::move(inlineStyle));
    *out_attributesStyle = Maybe<protocol::CSS::CSSStyle>(std::move(attributeStyle));
    *out_matchedCSSRules = Maybe<protocol::Array<protocol::CSS::RuleMatch>>(std::move(rulesMatchedArr));
    *out_cssKeyframesRules = Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>>(std::move(cssKeyFramesRulesArr));
    *out_inherited = Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>>(std::move(inheritedElementsArr));
    *out_pseudoElements = Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>>(std::move(pseudoElementsArr));
}

// Not supported
void V8CSSAgentImpl::getInlineStylesForNode(ErrorString*, int in_nodeId,
        Maybe<protocol::CSS::CSSStyle>* out_inlineStyle,
        Maybe<protocol::CSS::CSSStyle>* out_attributesStyle) {

    //// out_inlineStyle
    auto cssPropsArr = protocol::Array<protocol::CSS::CSSProperty>::create();
    auto shorthandPropArr = protocol::Array<protocol::CSS::ShorthandEntry>::create();

    auto inlineStyle = protocol::CSS::CSSStyle::create()
                       .setCssProperties(std::move(cssPropsArr))
                       .setShorthandEntries(std::move(shorthandPropArr))
                       .build();

    //// out_attributesStyle
    auto attrArr = protocol::Array<protocol::CSS::CSSProperty>::create();
    auto attributeStyle = protocol::CSS::CSSStyle::create()
                          .setCssProperties(std::move(attrArr))
                          .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
                          .build();

    *out_inlineStyle = Maybe<protocol::CSS::CSSStyle>(std::move(inlineStyle));
    *out_attributesStyle = Maybe<protocol::CSS::CSSStyle>(std::move(attributeStyle));
}

void V8CSSAgentImpl::getComputedStyleForNode(ErrorString* errorString, int in_nodeId,
        std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>* out_computedStyle) {
    auto computedStylePropertyArr = protocol::Array<protocol::CSS::CSSComputedStyleProperty>::create();

    std::string getComputedStylesForNodeString = "getComputedStylesForNode";
    // TODO: Pete: Find a better way to get a hold of the isolate
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        auto getComputedStylesForNode = globalInspectorObject->Get(ArgConverter::ConvertToV8String(isolate, getComputedStylesForNodeString));

        if (!getComputedStylesForNode.IsEmpty() && getComputedStylesForNode->IsFunction()) {
            auto getComputedStylesForNodeFunc = getComputedStylesForNode.As<v8::Function>();
            v8::Local<v8::Value> args[] = { v8::Number::New(isolate, in_nodeId) };
            v8::TryCatch tc;

            auto maybeResult = getComputedStylesForNodeFunc->Call(context, global, 1, args);

            if (tc.HasCaught()) {
                *errorString = utils::Common::getJSCallErrorMessage(getComputedStylesForNodeString, tc.Message()->Get()).c_str();

                *out_computedStyle = std::move(computedStylePropertyArr);
                return;
            }

            v8::Local<v8::Value> outResult;

            if (maybeResult.ToLocal(&outResult)) {
                auto resultString = ArgConverter::ConvertToString(outResult->ToString());
                auto resultCStr = resultString.c_str();
                auto resultJson = protocol::parseJSON(resultCStr);

                protocol::ErrorSupport errorSupport;
                auto computedStyles = protocol::Array<protocol::CSS::CSSComputedStyleProperty>::parse(
                                          resultJson.get(), &errorSupport);

                auto errorSupportString = errorSupport.errors().utf8();
                if (!errorSupportString.empty()) {
                    auto errorMessage = "Error while parsing CSSComputedStyleProperty object. ";
                    DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorSupportString.c_str());
                } else {
                    *out_computedStyle = std::move(computedStyles);

                    return;
                }
            }
        }
    }

    *out_computedStyle = std::move(computedStylePropertyArr);
}

void V8CSSAgentImpl::getPlatformFontsForNode(ErrorString*, int in_nodeId,
        std::unique_ptr<protocol::Array<protocol::CSS::PlatformFontUsage>>* out_fonts) {
    auto fontsArr = protocol::Array<protocol::CSS::PlatformFontUsage>::create();
    auto defaultFont = "System Font";
    fontsArr->addItem(std::move(protocol::CSS::PlatformFontUsage::create()
                                .setFamilyName(defaultFont)
                                .setGlyphCount(1)
                                .setIsCustomFont(false)
                                .build()));
    *out_fonts = std::move(fontsArr);
}

// Not supported
void V8CSSAgentImpl::getStyleSheetText(ErrorString*, const String& in_styleSheetId,
                                       String* out_text) {
    *out_text = "";
}

void V8CSSAgentImpl::collectClassNames(ErrorString*, const String& in_styleSheetId,
                                       std::unique_ptr<protocol::Array<String>>* out_classNames) {

}

void V8CSSAgentImpl::setStyleSheetText(ErrorString*, const String& in_styleSheetId,
                                       const String& in_text, Maybe<String>* out_sourceMapURL) {

}

void V8CSSAgentImpl::setRuleSelector(ErrorString*, const String& in_styleSheetId,
                                     std::unique_ptr<protocol::CSS::SourceRange> in_range,
                                     const String& in_selector,
                                     std::unique_ptr<protocol::CSS::SelectorList>* out_selectorList) {

}

void V8CSSAgentImpl::setKeyframeKey(ErrorString*, const String& in_styleSheetId,
                                    std::unique_ptr<protocol::CSS::SourceRange> in_range,
                                    const String& in_keyText,
                                    std::unique_ptr<protocol::CSS::Value>* out_keyText) {

}

void V8CSSAgentImpl::setStyleTexts(ErrorString*,
                                   std::unique_ptr<protocol::Array<protocol::CSS::StyleDeclarationEdit>> in_edits,
                                   std::unique_ptr<protocol::Array<protocol::CSS::CSSStyle>>* out_styles) {

}

void V8CSSAgentImpl::setMediaText(ErrorString*, const String& in_styleSheetId,
                                  std::unique_ptr<protocol::CSS::SourceRange> in_range,
                                  const String& in_text,
                                  std::unique_ptr<protocol::CSS::CSSMedia>* out_media) {

}

void V8CSSAgentImpl::createStyleSheet(ErrorString*, const String& in_frameId,
                                      String* out_styleSheetId) {

}

void V8CSSAgentImpl::addRule(ErrorString*, const String& in_styleSheetId, const String& in_ruleText,
                             std::unique_ptr<protocol::CSS::SourceRange> in_location,
                             std::unique_ptr<protocol::CSS::CSSRule>* out_rule) {

}

void V8CSSAgentImpl::forcePseudoState(ErrorString*, int in_nodeId,
                                      std::unique_ptr<protocol::Array<String>> in_forcedPseudoClasses) {

}

void V8CSSAgentImpl::getMediaQueries(ErrorString*,
                                     std::unique_ptr<protocol::Array<protocol::CSS::CSSMedia>>* out_medias) {

}

void V8CSSAgentImpl::setEffectivePropertyValueForNode(ErrorString*, int in_nodeId,
        const String& in_propertyName,
        const String& in_value) {

}

void V8CSSAgentImpl::getBackgroundColors(ErrorString*, int in_nodeId,
        Maybe<protocol::Array<String>>* out_backgroundColors,
        Maybe<String>* out_computedFontSize,
        Maybe<String>* out_computedFontWeight,
        Maybe<String>* out_computedBodyFontSize) {

}

void V8CSSAgentImpl::startRuleUsageTracking(ErrorString*) {

}

void V8CSSAgentImpl::takeCoverageDelta(ErrorString*,
                                       std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_coverage) {

}

void V8CSSAgentImpl::stopRuleUsageTracking(ErrorString*,
        std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_ruleUsage) {

}

V8CSSAgentImpl* V8CSSAgentImpl::Instance = 0;
}
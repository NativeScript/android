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

void V8CSSAgentImpl::enable(std::unique_ptr<protocol::CSS::Backend::EnableCallback> callback) {
    if (m_enabled) {
        callback->sendFailure("CSS Agent already enabled!");
        return;
    }

    m_state->setBoolean(CSSAgentState::cssEnabled, true);
    m_enabled = true;
    callback->sendSuccess();
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


V8CSSAgentImpl* V8CSSAgentImpl::Instance = 0;
}
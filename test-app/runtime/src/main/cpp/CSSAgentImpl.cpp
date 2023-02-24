//
// Created by pkanev on 5/11/2017.
//

#include <NativeScriptAssert.h>
#include <v8_inspector/third_party/inspector_protocol/crdtp/json.h>
#include <ArgConverter.h>

#include "CSSAgentImpl.h"
#include "utils/InspectorCommon.h"

namespace tns {

namespace CSSAgentState {
static const char cssEnabled[] = "cssEnabled";
}

CSSAgentImpl::CSSAgentImpl(V8InspectorSessionImpl* session,
                           protocol::FrontendChannel* frontendChannel,
                           protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

CSSAgentImpl::~CSSAgentImpl() { }

void CSSAgentImpl::enable(std::unique_ptr<EnableCallback> callback) {
    if (m_enabled) {
        callback->sendSuccess();
        return;
    }

    m_state->setBoolean(CSSAgentState::cssEnabled, true);
    m_enabled = true;

    callback->sendSuccess();
}

DispatchResponse CSSAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(CSSAgentState::cssEnabled, false);

    m_enabled = false;

    return DispatchResponse::Success();
}

// Not supported
DispatchResponse CSSAgentImpl::getMatchedStylesForNode(int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle, Maybe<protocol::Array<protocol::CSS::RuleMatch>>* out_matchedCSSRules, Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>>* out_pseudoElements, Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>>* out_inherited, Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>>* out_cssKeyframesRules) {
    //// out_inlineStyle
//        auto cssPropsArr = protocol::Array<protocol::CSS::CSSProperty>::create();
//        auto shorthandPropArr = protocol::Array<protocol::CSS::ShorthandEntry>::create();
//        auto inlineStyle = protocol::CSS::CSSStyle::create()
//                .setCssProperties(std::move(cssPropsArr))
//                .setShorthandEntries(std::move(shorthandPropArr))
//                .build();

    //// out_attributesStyle
//        auto attrArr = protocol::Array<protocol::CSS::CSSProperty>::create();
//        auto attributeStyle = protocol::CSS::CSSStyle::create()
//                .setCssProperties(std::move(attrArr))
//                .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
//                .build();

    //// out_matchedCSSRules
//        auto cssSelectorsArr = protocol::Array<protocol::CSS::Value>::create();
//        auto cssSelectorList = protocol::CSS::SelectorList::create()
//                .setSelectors(std::move(cssSelectorsArr))
//                .setText("")
//                .build();

//        auto cssRule = protocol::CSS::CSSRule::create()
//                .setSelectorList(std::move(cssSelectorList))
//                .setOrigin(protocol::CSS::StyleSheetOriginEnum::Regular)
//                .setStyle(std::move(protocol::CSS::CSSStyle::create()
//                                            .setCssProperties(std::move(protocol::Array<protocol::CSS::CSSProperty>::create()))
//                                            .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
//                                            .build()))
//                .build();

//        auto rulesMatchedArr = protocol::Array<protocol::CSS::RuleMatch>::create();

    //// out_pseudoElements
//        auto pseudoElementsArr = protocol::Array<protocol::CSS::PseudoElementMatches>::create();

    //// out_inherited
//        auto inheritedElementsArr = protocol::Array<protocol::CSS::InheritedStyleEntry>::create();
//        auto inheritedelem = protocol::CSS::InheritedStyleEntry::create()
//                .setInlineStyle(std::move(protocol::CSS::CSSStyle::create()
//                                                  .setCssProperties(std::move(protocol::Array<protocol::CSS::CSSProperty>::create()))
//                                                  .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
//                                                  .build()))
//                .setMatchedCSSRules(std::move(protocol::Array<protocol::CSS::RuleMatch>::create()))
//                .build();
//        inheritedElementsArr->addItem(std::move(inheritedelem));

    //// out_cssKeyframesRules
//        auto cssKeyFramesRulesArr = protocol::Array<protocol::CSS::CSSKeyframesRule>::create();

//        *out_inlineStyle = Maybe<protocol::CSS::CSSStyle>(std::move(inlineStyle));
//        *out_attributesStyle = std::move(Maybe<protocol::CSS::CSSStyle>(std::move(attributeStyle)));
//        *out_matchedCSSRules = std::move(Maybe<protocol::Array<protocol::CSS::RuleMatch>>(std::move(rulesMatchedArr)));
//        *out_cssKeyframesRules = std::move(Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>>(std::move(cssKeyFramesRulesArr)));
//        *out_inherited = std::move(Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>>(std::move(inheritedElementsArr)));
//        *out_pseudoElements = std::move(Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>>(std::move(pseudoElementsArr)));

    return DispatchResponse::Success();
}

DispatchResponse CSSAgentImpl::getInlineStylesForNode(int in_nodeId, Maybe<protocol::CSS::CSSStyle>* out_inlineStyle, Maybe<protocol::CSS::CSSStyle>* out_attributesStyle) {
    //// out_inlineStyle
//        auto cssPropsArr = protocol::Array<protocol::CSS::CSSProperty>::create();
//        auto shorthandPropArr = protocol::Array<protocol::CSS::ShorthandEntry>::create();

//        auto inlineStyle = protocol::CSS::CSSStyle::create()
//                .setCssProperties(std::move(cssPropsArr))
//                .setShorthandEntries(std::move(shorthandPropArr))
//                .build();

    //// out_attributesStyle
//        auto attrArr = protocol::Array<protocol::CSS::CSSProperty>::create();
//        auto attributeStyle = protocol::CSS::CSSStyle::create()
//                .setCssProperties(std::move(attrArr))
//                .setShorthandEntries(std::move(protocol::Array<protocol::CSS::ShorthandEntry>::create()))
//                .build();

//        *out_inlineStyle = std::move(Maybe<protocol::CSS::CSSStyle>(std::move(inlineStyle)));
//        *out_attributesStyle = std::move(Maybe<protocol::CSS::CSSStyle>(std::move(attributeStyle)));

    return DispatchResponse::Success();
}

DispatchResponse CSSAgentImpl::getComputedStyleForNode(int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>* out_computedStyle) {
    auto computedStylePropertyArr = std::make_unique<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>();

    std::string getComputedStylesForNodeString = "getComputedStylesForNode";
    // TODO: Pete: Find a better way to get a hold of the isolate
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        v8::Local<v8::Value> getComputedStylesForNode;
        globalInspectorObject->Get(context, ArgConverter::ConvertToV8String(isolate, getComputedStylesForNodeString)).ToLocal(&getComputedStylesForNode);

        if (!getComputedStylesForNode.IsEmpty() && getComputedStylesForNode->IsFunction()) {
            auto getComputedStylesForNodeFunc = getComputedStylesForNode.As<v8::Function>();
            v8::Local<v8::Value> args[] = { v8::Number::New(isolate, in_nodeId) };
            v8::TryCatch tc(isolate);

            auto maybeResult = getComputedStylesForNodeFunc->Call(context, global, 1, args);

            if (tc.HasCaught()) {

                *out_computedStyle = std::move(computedStylePropertyArr);
                return DispatchResponse::ServerError(utils::Common::getJSCallErrorMessage(getComputedStylesForNodeString, tc.Message()->Get()).c_str());
            }

            v8::Local<v8::Value> outResult;

            if (maybeResult.ToLocal(&outResult)) {
                auto resultString = outResult->ToString(context).ToLocalChecked();
                v8_inspector::String16 resultProtocolString = v8_inspector::toProtocolString(isolate, resultString);
                std::vector<uint8_t> cbor;
                v8_crdtp::json::ConvertJSONToCBOR(v8_crdtp::span<uint16_t>(resultProtocolString.characters16(), resultProtocolString.length()), &cbor);
                std::unique_ptr<protocol::Value> resultJson = protocol::Value::parseBinary(cbor.data(), cbor.size());
                protocol::ErrorSupport errorSupport;
                std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>> computedStyles = utils::Common::fromValue<protocol::CSS::CSSComputedStyleProperty>(resultJson.get(), &errorSupport);

                std::vector<uint8_t> json;
                v8_crdtp::json::ConvertCBORToJSON(errorSupport.Errors(), &json);
                auto errorSupportString = v8_inspector::String16(reinterpret_cast<const char*>(json.data()), json.size()).utf8();
                if (!errorSupportString.empty()) {
                    auto errorMessage = "Error while parsing CSSComputedStyleProperty object. ";
                    DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorSupportString.c_str());
                    return DispatchResponse::ServerError(errorMessage);
                } else {
                    *out_computedStyle = std::move(computedStyles);

                    return DispatchResponse::Success();
                }
            }
        }
    }

    *out_computedStyle = std::move(computedStylePropertyArr);

    return DispatchResponse::Success();
}

DispatchResponse CSSAgentImpl::getPlatformFontsForNode(int in_nodeId, std::unique_ptr<protocol::Array<protocol::CSS::PlatformFontUsage>>* out_fonts) {
    auto fontsArr = std::make_unique<protocol::Array<protocol::CSS::PlatformFontUsage>>();
    auto defaultFont = "System Font";
    fontsArr->emplace_back(std::move(protocol::CSS::PlatformFontUsage::create()
                                .setFamilyName(defaultFont)
                                .setGlyphCount(1)
                                .setIsCustomFont(false)
                                .build()));
    *out_fonts = std::move(fontsArr);

    return DispatchResponse::Success();
}

DispatchResponse CSSAgentImpl::getStyleSheetText(const String& in_styleSheetId, String* out_text) {
    *out_text = "";

    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::addRule(const String& in_styleSheetId, const String& in_ruleText, std::unique_ptr<protocol::CSS::SourceRange> in_location, std::unique_ptr<protocol::CSS::CSSRule>* out_rule) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::collectClassNames(const String& in_styleSheetId, std::unique_ptr<protocol::Array<String>>* out_classNames) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::createStyleSheet(const String& in_frameId, String* out_styleSheetId) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::forcePseudoState(int in_nodeId, std::unique_ptr<protocol::Array<String>> in_forcedPseudoClasses) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::getBackgroundColors(int in_nodeId, Maybe<protocol::Array<String>>* out_backgroundColors, Maybe<String>* out_computedFontSize, Maybe<String>* out_computedFontWeight) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::getMediaQueries(std::unique_ptr<protocol::Array<protocol::CSS::CSSMedia>>* out_medias) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::setEffectivePropertyValueForNode(int in_nodeId, const String& in_propertyName, const String& in_value) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::setKeyframeKey(const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_keyText, std::unique_ptr<protocol::CSS::Value>* out_keyText) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::setMediaText(const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_text, std::unique_ptr<protocol::CSS::CSSMedia>* out_media) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::setRuleSelector(const String& in_styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> in_range, const String& in_selector, std::unique_ptr<protocol::CSS::SelectorList>* out_selectorList) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::setStyleSheetText(const String& in_styleSheetId, const String& in_text, Maybe<String>* out_sourceMapURL) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::setStyleTexts(std::unique_ptr<protocol::Array<protocol::CSS::StyleDeclarationEdit>> in_edits, std::unique_ptr<protocol::Array<protocol::CSS::CSSStyle>>* out_styles) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::startRuleUsageTracking() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::stopRuleUsageTracking(std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_ruleUsage) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse CSSAgentImpl::takeCoverageDelta(std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>>* out_coverage) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

CSSAgentImpl* CSSAgentImpl::Instance = 0;
}  // namespace tns

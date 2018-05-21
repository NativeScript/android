// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/CSS.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace CSS {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "CSS";
const char Metainfo::commandPrefix[] = "CSS.";
const char Metainfo::version[] = "1.3";

namespace StyleSheetOriginEnum {
const char* Injected = "injected";
const char* UserAgent = "user-agent";
const char* Inspector = "inspector";
const char* Regular = "regular";
} // namespace StyleSheetOriginEnum

std::unique_ptr<PseudoElementMatches> PseudoElementMatches::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PseudoElementMatches> result(new PseudoElementMatches());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* pseudoTypeValue = object->get("pseudoType");
    errors->setName("pseudoType");
    result->m_pseudoType = ValueConversions<String>::fromValue(pseudoTypeValue, errors);
    protocol::Value* matchesValue = object->get("matches");
    errors->setName("matches");
    result->m_matches = ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::fromValue(matchesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PseudoElementMatches::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("pseudoType", ValueConversions<String>::toValue(m_pseudoType));
    result->setValue("matches", ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::toValue(m_matches.get()));
    return result;
}

std::unique_ptr<PseudoElementMatches> PseudoElementMatches::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<InheritedStyleEntry> InheritedStyleEntry::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<InheritedStyleEntry> result(new InheritedStyleEntry());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* inlineStyleValue = object->get("inlineStyle");
    if (inlineStyleValue) {
        errors->setName("inlineStyle");
        result->m_inlineStyle = ValueConversions<protocol::CSS::CSSStyle>::fromValue(inlineStyleValue, errors);
    }
    protocol::Value* matchedCSSRulesValue = object->get("matchedCSSRules");
    errors->setName("matchedCSSRules");
    result->m_matchedCSSRules = ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::fromValue(matchedCSSRulesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> InheritedStyleEntry::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_inlineStyle.isJust()) {
        result->setValue("inlineStyle", ValueConversions<protocol::CSS::CSSStyle>::toValue(m_inlineStyle.fromJust()));
    }
    result->setValue("matchedCSSRules", ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::toValue(m_matchedCSSRules.get()));
    return result;
}

std::unique_ptr<InheritedStyleEntry> InheritedStyleEntry::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<RuleMatch> RuleMatch::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RuleMatch> result(new RuleMatch());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* ruleValue = object->get("rule");
    errors->setName("rule");
    result->m_rule = ValueConversions<protocol::CSS::CSSRule>::fromValue(ruleValue, errors);
    protocol::Value* matchingSelectorsValue = object->get("matchingSelectors");
    errors->setName("matchingSelectors");
    result->m_matchingSelectors = ValueConversions<protocol::Array<int>>::fromValue(matchingSelectorsValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RuleMatch::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("rule", ValueConversions<protocol::CSS::CSSRule>::toValue(m_rule.get()));
    result->setValue("matchingSelectors", ValueConversions<protocol::Array<int>>::toValue(m_matchingSelectors.get()));
    return result;
}

std::unique_ptr<RuleMatch> RuleMatch::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<Value> Value::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Value> result(new Value());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::fromValue(textValue, errors);
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Value::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("text", ValueConversions<String>::toValue(m_text));
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::toValue(m_range.fromJust()));
    }
    return result;
}

std::unique_ptr<Value> Value::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<SelectorList> SelectorList::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SelectorList> result(new SelectorList());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* selectorsValue = object->get("selectors");
    errors->setName("selectors");
    result->m_selectors = ValueConversions<protocol::Array<protocol::CSS::Value>>::fromValue(selectorsValue, errors);
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::fromValue(textValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SelectorList::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("selectors", ValueConversions<protocol::Array<protocol::CSS::Value>>::toValue(m_selectors.get()));
    result->setValue("text", ValueConversions<String>::toValue(m_text));
    return result;
}

std::unique_ptr<SelectorList> SelectorList::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CSSStyleSheetHeader> CSSStyleSheetHeader::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSStyleSheetHeader> result(new CSSStyleSheetHeader());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    errors->setName("styleSheetId");
    result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* sourceURLValue = object->get("sourceURL");
    errors->setName("sourceURL");
    result->m_sourceURL = ValueConversions<String>::fromValue(sourceURLValue, errors);
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::fromValue(sourceMapURLValue, errors);
    }
    protocol::Value* originValue = object->get("origin");
    errors->setName("origin");
    result->m_origin = ValueConversions<String>::fromValue(originValue, errors);
    protocol::Value* titleValue = object->get("title");
    errors->setName("title");
    result->m_title = ValueConversions<String>::fromValue(titleValue, errors);
    protocol::Value* ownerNodeValue = object->get("ownerNode");
    if (ownerNodeValue) {
        errors->setName("ownerNode");
        result->m_ownerNode = ValueConversions<int>::fromValue(ownerNodeValue, errors);
    }
    protocol::Value* disabledValue = object->get("disabled");
    errors->setName("disabled");
    result->m_disabled = ValueConversions<bool>::fromValue(disabledValue, errors);
    protocol::Value* hasSourceURLValue = object->get("hasSourceURL");
    if (hasSourceURLValue) {
        errors->setName("hasSourceURL");
        result->m_hasSourceURL = ValueConversions<bool>::fromValue(hasSourceURLValue, errors);
    }
    protocol::Value* isInlineValue = object->get("isInline");
    errors->setName("isInline");
    result->m_isInline = ValueConversions<bool>::fromValue(isInlineValue, errors);
    protocol::Value* startLineValue = object->get("startLine");
    errors->setName("startLine");
    result->m_startLine = ValueConversions<double>::fromValue(startLineValue, errors);
    protocol::Value* startColumnValue = object->get("startColumn");
    errors->setName("startColumn");
    result->m_startColumn = ValueConversions<double>::fromValue(startColumnValue, errors);
    protocol::Value* lengthValue = object->get("length");
    errors->setName("length");
    result->m_length = ValueConversions<double>::fromValue(lengthValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSStyleSheetHeader::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId));
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("sourceURL", ValueConversions<String>::toValue(m_sourceURL));
    if (m_sourceMapURL.isJust()) {
        result->setValue("sourceMapURL", ValueConversions<String>::toValue(m_sourceMapURL.fromJust()));
    }
    result->setValue("origin", ValueConversions<String>::toValue(m_origin));
    result->setValue("title", ValueConversions<String>::toValue(m_title));
    if (m_ownerNode.isJust()) {
        result->setValue("ownerNode", ValueConversions<int>::toValue(m_ownerNode.fromJust()));
    }
    result->setValue("disabled", ValueConversions<bool>::toValue(m_disabled));
    if (m_hasSourceURL.isJust()) {
        result->setValue("hasSourceURL", ValueConversions<bool>::toValue(m_hasSourceURL.fromJust()));
    }
    result->setValue("isInline", ValueConversions<bool>::toValue(m_isInline));
    result->setValue("startLine", ValueConversions<double>::toValue(m_startLine));
    result->setValue("startColumn", ValueConversions<double>::toValue(m_startColumn));
    result->setValue("length", ValueConversions<double>::toValue(m_length));
    return result;
}

std::unique_ptr<CSSStyleSheetHeader> CSSStyleSheetHeader::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CSSRule> CSSRule::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSRule> result(new CSSRule());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    if (styleSheetIdValue) {
        errors->setName("styleSheetId");
        result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    }
    protocol::Value* selectorListValue = object->get("selectorList");
    errors->setName("selectorList");
    result->m_selectorList = ValueConversions<protocol::CSS::SelectorList>::fromValue(selectorListValue, errors);
    protocol::Value* originValue = object->get("origin");
    errors->setName("origin");
    result->m_origin = ValueConversions<String>::fromValue(originValue, errors);
    protocol::Value* styleValue = object->get("style");
    errors->setName("style");
    result->m_style = ValueConversions<protocol::CSS::CSSStyle>::fromValue(styleValue, errors);
    protocol::Value* mediaValue = object->get("media");
    if (mediaValue) {
        errors->setName("media");
        result->m_media = ValueConversions<protocol::Array<protocol::CSS::CSSMedia>>::fromValue(mediaValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSRule::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId.fromJust()));
    }
    result->setValue("selectorList", ValueConversions<protocol::CSS::SelectorList>::toValue(m_selectorList.get()));
    result->setValue("origin", ValueConversions<String>::toValue(m_origin));
    result->setValue("style", ValueConversions<protocol::CSS::CSSStyle>::toValue(m_style.get()));
    if (m_media.isJust()) {
        result->setValue("media", ValueConversions<protocol::Array<protocol::CSS::CSSMedia>>::toValue(m_media.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSRule> CSSRule::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<RuleUsage> RuleUsage::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RuleUsage> result(new RuleUsage());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    errors->setName("styleSheetId");
    result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* startOffsetValue = object->get("startOffset");
    errors->setName("startOffset");
    result->m_startOffset = ValueConversions<double>::fromValue(startOffsetValue, errors);
    protocol::Value* endOffsetValue = object->get("endOffset");
    errors->setName("endOffset");
    result->m_endOffset = ValueConversions<double>::fromValue(endOffsetValue, errors);
    protocol::Value* usedValue = object->get("used");
    errors->setName("used");
    result->m_used = ValueConversions<bool>::fromValue(usedValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RuleUsage::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId));
    result->setValue("startOffset", ValueConversions<double>::toValue(m_startOffset));
    result->setValue("endOffset", ValueConversions<double>::toValue(m_endOffset));
    result->setValue("used", ValueConversions<bool>::toValue(m_used));
    return result;
}

std::unique_ptr<RuleUsage> RuleUsage::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<SourceRange> SourceRange::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SourceRange> result(new SourceRange());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* startLineValue = object->get("startLine");
    errors->setName("startLine");
    result->m_startLine = ValueConversions<int>::fromValue(startLineValue, errors);
    protocol::Value* startColumnValue = object->get("startColumn");
    errors->setName("startColumn");
    result->m_startColumn = ValueConversions<int>::fromValue(startColumnValue, errors);
    protocol::Value* endLineValue = object->get("endLine");
    errors->setName("endLine");
    result->m_endLine = ValueConversions<int>::fromValue(endLineValue, errors);
    protocol::Value* endColumnValue = object->get("endColumn");
    errors->setName("endColumn");
    result->m_endColumn = ValueConversions<int>::fromValue(endColumnValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SourceRange::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("startLine", ValueConversions<int>::toValue(m_startLine));
    result->setValue("startColumn", ValueConversions<int>::toValue(m_startColumn));
    result->setValue("endLine", ValueConversions<int>::toValue(m_endLine));
    result->setValue("endColumn", ValueConversions<int>::toValue(m_endColumn));
    return result;
}

std::unique_ptr<SourceRange> SourceRange::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ShorthandEntry> ShorthandEntry::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ShorthandEntry> result(new ShorthandEntry());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::fromValue(valueValue, errors);
    protocol::Value* importantValue = object->get("important");
    if (importantValue) {
        errors->setName("important");
        result->m_important = ValueConversions<bool>::fromValue(importantValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ShorthandEntry::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("value", ValueConversions<String>::toValue(m_value));
    if (m_important.isJust()) {
        result->setValue("important", ValueConversions<bool>::toValue(m_important.fromJust()));
    }
    return result;
}

std::unique_ptr<ShorthandEntry> ShorthandEntry::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CSSComputedStyleProperty> CSSComputedStyleProperty::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSComputedStyleProperty> result(new CSSComputedStyleProperty());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::fromValue(valueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSComputedStyleProperty::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("value", ValueConversions<String>::toValue(m_value));
    return result;
}

std::unique_ptr<CSSComputedStyleProperty> CSSComputedStyleProperty::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CSSStyle> CSSStyle::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSStyle> result(new CSSStyle());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    if (styleSheetIdValue) {
        errors->setName("styleSheetId");
        result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    }
    protocol::Value* cssPropertiesValue = object->get("cssProperties");
    errors->setName("cssProperties");
    result->m_cssProperties = ValueConversions<protocol::Array<protocol::CSS::CSSProperty>>::fromValue(cssPropertiesValue, errors);
    protocol::Value* shorthandEntriesValue = object->get("shorthandEntries");
    errors->setName("shorthandEntries");
    result->m_shorthandEntries = ValueConversions<protocol::Array<protocol::CSS::ShorthandEntry>>::fromValue(shorthandEntriesValue, errors);
    protocol::Value* cssTextValue = object->get("cssText");
    if (cssTextValue) {
        errors->setName("cssText");
        result->m_cssText = ValueConversions<String>::fromValue(cssTextValue, errors);
    }
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSStyle::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId.fromJust()));
    }
    result->setValue("cssProperties", ValueConversions<protocol::Array<protocol::CSS::CSSProperty>>::toValue(m_cssProperties.get()));
    result->setValue("shorthandEntries", ValueConversions<protocol::Array<protocol::CSS::ShorthandEntry>>::toValue(m_shorthandEntries.get()));
    if (m_cssText.isJust()) {
        result->setValue("cssText", ValueConversions<String>::toValue(m_cssText.fromJust()));
    }
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::toValue(m_range.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSStyle> CSSStyle::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CSSProperty> CSSProperty::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSProperty> result(new CSSProperty());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::fromValue(valueValue, errors);
    protocol::Value* importantValue = object->get("important");
    if (importantValue) {
        errors->setName("important");
        result->m_important = ValueConversions<bool>::fromValue(importantValue, errors);
    }
    protocol::Value* implicitValue = object->get("implicit");
    if (implicitValue) {
        errors->setName("implicit");
        result->m_implicit = ValueConversions<bool>::fromValue(implicitValue, errors);
    }
    protocol::Value* textValue = object->get("text");
    if (textValue) {
        errors->setName("text");
        result->m_text = ValueConversions<String>::fromValue(textValue, errors);
    }
    protocol::Value* parsedOkValue = object->get("parsedOk");
    if (parsedOkValue) {
        errors->setName("parsedOk");
        result->m_parsedOk = ValueConversions<bool>::fromValue(parsedOkValue, errors);
    }
    protocol::Value* disabledValue = object->get("disabled");
    if (disabledValue) {
        errors->setName("disabled");
        result->m_disabled = ValueConversions<bool>::fromValue(disabledValue, errors);
    }
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSProperty::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("value", ValueConversions<String>::toValue(m_value));
    if (m_important.isJust()) {
        result->setValue("important", ValueConversions<bool>::toValue(m_important.fromJust()));
    }
    if (m_implicit.isJust()) {
        result->setValue("implicit", ValueConversions<bool>::toValue(m_implicit.fromJust()));
    }
    if (m_text.isJust()) {
        result->setValue("text", ValueConversions<String>::toValue(m_text.fromJust()));
    }
    if (m_parsedOk.isJust()) {
        result->setValue("parsedOk", ValueConversions<bool>::toValue(m_parsedOk.fromJust()));
    }
    if (m_disabled.isJust()) {
        result->setValue("disabled", ValueConversions<bool>::toValue(m_disabled.fromJust()));
    }
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::toValue(m_range.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSProperty> CSSProperty::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* CSSMedia::SourceEnum::MediaRule = "mediaRule";
const char* CSSMedia::SourceEnum::ImportRule = "importRule";
const char* CSSMedia::SourceEnum::LinkedSheet = "linkedSheet";
const char* CSSMedia::SourceEnum::InlineSheet = "inlineSheet";

std::unique_ptr<CSSMedia> CSSMedia::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSMedia> result(new CSSMedia());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::fromValue(textValue, errors);
    protocol::Value* sourceValue = object->get("source");
    errors->setName("source");
    result->m_source = ValueConversions<String>::fromValue(sourceValue, errors);
    protocol::Value* sourceURLValue = object->get("sourceURL");
    if (sourceURLValue) {
        errors->setName("sourceURL");
        result->m_sourceURL = ValueConversions<String>::fromValue(sourceURLValue, errors);
    }
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    }
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    if (styleSheetIdValue) {
        errors->setName("styleSheetId");
        result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    }
    protocol::Value* mediaListValue = object->get("mediaList");
    if (mediaListValue) {
        errors->setName("mediaList");
        result->m_mediaList = ValueConversions<protocol::Array<protocol::CSS::MediaQuery>>::fromValue(mediaListValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSMedia::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("text", ValueConversions<String>::toValue(m_text));
    result->setValue("source", ValueConversions<String>::toValue(m_source));
    if (m_sourceURL.isJust()) {
        result->setValue("sourceURL", ValueConversions<String>::toValue(m_sourceURL.fromJust()));
    }
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::toValue(m_range.fromJust()));
    }
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId.fromJust()));
    }
    if (m_mediaList.isJust()) {
        result->setValue("mediaList", ValueConversions<protocol::Array<protocol::CSS::MediaQuery>>::toValue(m_mediaList.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSMedia> CSSMedia::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<MediaQuery> MediaQuery::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<MediaQuery> result(new MediaQuery());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* expressionsValue = object->get("expressions");
    errors->setName("expressions");
    result->m_expressions = ValueConversions<protocol::Array<protocol::CSS::MediaQueryExpression>>::fromValue(expressionsValue, errors);
    protocol::Value* activeValue = object->get("active");
    errors->setName("active");
    result->m_active = ValueConversions<bool>::fromValue(activeValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> MediaQuery::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("expressions", ValueConversions<protocol::Array<protocol::CSS::MediaQueryExpression>>::toValue(m_expressions.get()));
    result->setValue("active", ValueConversions<bool>::toValue(m_active));
    return result;
}

std::unique_ptr<MediaQuery> MediaQuery::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<MediaQueryExpression> MediaQueryExpression::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<MediaQueryExpression> result(new MediaQueryExpression());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<double>::fromValue(valueValue, errors);
    protocol::Value* unitValue = object->get("unit");
    errors->setName("unit");
    result->m_unit = ValueConversions<String>::fromValue(unitValue, errors);
    protocol::Value* featureValue = object->get("feature");
    errors->setName("feature");
    result->m_feature = ValueConversions<String>::fromValue(featureValue, errors);
    protocol::Value* valueRangeValue = object->get("valueRange");
    if (valueRangeValue) {
        errors->setName("valueRange");
        result->m_valueRange = ValueConversions<protocol::CSS::SourceRange>::fromValue(valueRangeValue, errors);
    }
    protocol::Value* computedLengthValue = object->get("computedLength");
    if (computedLengthValue) {
        errors->setName("computedLength");
        result->m_computedLength = ValueConversions<double>::fromValue(computedLengthValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> MediaQueryExpression::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("value", ValueConversions<double>::toValue(m_value));
    result->setValue("unit", ValueConversions<String>::toValue(m_unit));
    result->setValue("feature", ValueConversions<String>::toValue(m_feature));
    if (m_valueRange.isJust()) {
        result->setValue("valueRange", ValueConversions<protocol::CSS::SourceRange>::toValue(m_valueRange.fromJust()));
    }
    if (m_computedLength.isJust()) {
        result->setValue("computedLength", ValueConversions<double>::toValue(m_computedLength.fromJust()));
    }
    return result;
}

std::unique_ptr<MediaQueryExpression> MediaQueryExpression::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<PlatformFontUsage> PlatformFontUsage::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PlatformFontUsage> result(new PlatformFontUsage());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* familyNameValue = object->get("familyName");
    errors->setName("familyName");
    result->m_familyName = ValueConversions<String>::fromValue(familyNameValue, errors);
    protocol::Value* isCustomFontValue = object->get("isCustomFont");
    errors->setName("isCustomFont");
    result->m_isCustomFont = ValueConversions<bool>::fromValue(isCustomFontValue, errors);
    protocol::Value* glyphCountValue = object->get("glyphCount");
    errors->setName("glyphCount");
    result->m_glyphCount = ValueConversions<double>::fromValue(glyphCountValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PlatformFontUsage::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("familyName", ValueConversions<String>::toValue(m_familyName));
    result->setValue("isCustomFont", ValueConversions<bool>::toValue(m_isCustomFont));
    result->setValue("glyphCount", ValueConversions<double>::toValue(m_glyphCount));
    return result;
}

std::unique_ptr<PlatformFontUsage> PlatformFontUsage::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FontFace> FontFace::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FontFace> result(new FontFace());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* fontFamilyValue = object->get("fontFamily");
    errors->setName("fontFamily");
    result->m_fontFamily = ValueConversions<String>::fromValue(fontFamilyValue, errors);
    protocol::Value* fontStyleValue = object->get("fontStyle");
    errors->setName("fontStyle");
    result->m_fontStyle = ValueConversions<String>::fromValue(fontStyleValue, errors);
    protocol::Value* fontVariantValue = object->get("fontVariant");
    errors->setName("fontVariant");
    result->m_fontVariant = ValueConversions<String>::fromValue(fontVariantValue, errors);
    protocol::Value* fontWeightValue = object->get("fontWeight");
    errors->setName("fontWeight");
    result->m_fontWeight = ValueConversions<String>::fromValue(fontWeightValue, errors);
    protocol::Value* fontStretchValue = object->get("fontStretch");
    errors->setName("fontStretch");
    result->m_fontStretch = ValueConversions<String>::fromValue(fontStretchValue, errors);
    protocol::Value* unicodeRangeValue = object->get("unicodeRange");
    errors->setName("unicodeRange");
    result->m_unicodeRange = ValueConversions<String>::fromValue(unicodeRangeValue, errors);
    protocol::Value* srcValue = object->get("src");
    errors->setName("src");
    result->m_src = ValueConversions<String>::fromValue(srcValue, errors);
    protocol::Value* platformFontFamilyValue = object->get("platformFontFamily");
    errors->setName("platformFontFamily");
    result->m_platformFontFamily = ValueConversions<String>::fromValue(platformFontFamilyValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FontFace::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("fontFamily", ValueConversions<String>::toValue(m_fontFamily));
    result->setValue("fontStyle", ValueConversions<String>::toValue(m_fontStyle));
    result->setValue("fontVariant", ValueConversions<String>::toValue(m_fontVariant));
    result->setValue("fontWeight", ValueConversions<String>::toValue(m_fontWeight));
    result->setValue("fontStretch", ValueConversions<String>::toValue(m_fontStretch));
    result->setValue("unicodeRange", ValueConversions<String>::toValue(m_unicodeRange));
    result->setValue("src", ValueConversions<String>::toValue(m_src));
    result->setValue("platformFontFamily", ValueConversions<String>::toValue(m_platformFontFamily));
    return result;
}

std::unique_ptr<FontFace> FontFace::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CSSKeyframesRule> CSSKeyframesRule::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSKeyframesRule> result(new CSSKeyframesRule());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* animationNameValue = object->get("animationName");
    errors->setName("animationName");
    result->m_animationName = ValueConversions<protocol::CSS::Value>::fromValue(animationNameValue, errors);
    protocol::Value* keyframesValue = object->get("keyframes");
    errors->setName("keyframes");
    result->m_keyframes = ValueConversions<protocol::Array<protocol::CSS::CSSKeyframeRule>>::fromValue(keyframesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSKeyframesRule::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("animationName", ValueConversions<protocol::CSS::Value>::toValue(m_animationName.get()));
    result->setValue("keyframes", ValueConversions<protocol::Array<protocol::CSS::CSSKeyframeRule>>::toValue(m_keyframes.get()));
    return result;
}

std::unique_ptr<CSSKeyframesRule> CSSKeyframesRule::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CSSKeyframeRule> CSSKeyframeRule::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSKeyframeRule> result(new CSSKeyframeRule());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    if (styleSheetIdValue) {
        errors->setName("styleSheetId");
        result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    }
    protocol::Value* originValue = object->get("origin");
    errors->setName("origin");
    result->m_origin = ValueConversions<String>::fromValue(originValue, errors);
    protocol::Value* keyTextValue = object->get("keyText");
    errors->setName("keyText");
    result->m_keyText = ValueConversions<protocol::CSS::Value>::fromValue(keyTextValue, errors);
    protocol::Value* styleValue = object->get("style");
    errors->setName("style");
    result->m_style = ValueConversions<protocol::CSS::CSSStyle>::fromValue(styleValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSKeyframeRule::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId.fromJust()));
    }
    result->setValue("origin", ValueConversions<String>::toValue(m_origin));
    result->setValue("keyText", ValueConversions<protocol::CSS::Value>::toValue(m_keyText.get()));
    result->setValue("style", ValueConversions<protocol::CSS::CSSStyle>::toValue(m_style.get()));
    return result;
}

std::unique_ptr<CSSKeyframeRule> CSSKeyframeRule::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StyleDeclarationEdit> StyleDeclarationEdit::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<StyleDeclarationEdit> result(new StyleDeclarationEdit());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    errors->setName("styleSheetId");
    result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* rangeValue = object->get("range");
    errors->setName("range");
    result->m_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::fromValue(textValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> StyleDeclarationEdit::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId));
    result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::toValue(m_range.get()));
    result->setValue("text", ValueConversions<String>::toValue(m_text));
    return result;
}

std::unique_ptr<StyleDeclarationEdit> StyleDeclarationEdit::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FontsUpdatedNotification> FontsUpdatedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FontsUpdatedNotification> result(new FontsUpdatedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* fontValue = object->get("font");
    if (fontValue) {
        errors->setName("font");
        result->m_font = ValueConversions<protocol::CSS::FontFace>::fromValue(fontValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FontsUpdatedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_font.isJust()) {
        result->setValue("font", ValueConversions<protocol::CSS::FontFace>::toValue(m_font.fromJust()));
    }
    return result;
}

std::unique_ptr<FontsUpdatedNotification> FontsUpdatedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StyleSheetAddedNotification> StyleSheetAddedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<StyleSheetAddedNotification> result(new StyleSheetAddedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* headerValue = object->get("header");
    errors->setName("header");
    result->m_header = ValueConversions<protocol::CSS::CSSStyleSheetHeader>::fromValue(headerValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> StyleSheetAddedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("header", ValueConversions<protocol::CSS::CSSStyleSheetHeader>::toValue(m_header.get()));
    return result;
}

std::unique_ptr<StyleSheetAddedNotification> StyleSheetAddedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StyleSheetChangedNotification> StyleSheetChangedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<StyleSheetChangedNotification> result(new StyleSheetChangedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    errors->setName("styleSheetId");
    result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> StyleSheetChangedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId));
    return result;
}

std::unique_ptr<StyleSheetChangedNotification> StyleSheetChangedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StyleSheetRemovedNotification> StyleSheetRemovedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<StyleSheetRemovedNotification> result(new StyleSheetRemovedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    errors->setName("styleSheetId");
    result->m_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> StyleSheetRemovedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("styleSheetId", ValueConversions<String>::toValue(m_styleSheetId));
    return result;
}

std::unique_ptr<StyleSheetRemovedNotification> StyleSheetRemovedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::fontsUpdated(Maybe<protocol::CSS::FontFace> font) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FontsUpdatedNotification> messageData = FontsUpdatedNotification::create()
            .build();
    if (font.isJust()) {
        messageData->setFont(std::move(font).takeJust());
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("CSS.fontsUpdated", std::move(messageData)));
}

void Frontend::mediaQueryResultChanged() {
    if (!m_frontendChannel) {
        return;
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("CSS.mediaQueryResultChanged"));
}

void Frontend::styleSheetAdded(std::unique_ptr<protocol::CSS::CSSStyleSheetHeader> header) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<StyleSheetAddedNotification> messageData = StyleSheetAddedNotification::create()
            .setHeader(std::move(header))
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("CSS.styleSheetAdded", std::move(messageData)));
}

void Frontend::styleSheetChanged(const String& styleSheetId) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<StyleSheetChangedNotification> messageData = StyleSheetChangedNotification::create()
            .setStyleSheetId(styleSheetId)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("CSS.styleSheetChanged", std::move(messageData)));
}

void Frontend::styleSheetRemoved(const String& styleSheetId) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<StyleSheetRemovedNotification> messageData = StyleSheetRemovedNotification::create()
            .setStyleSheetId(styleSheetId)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("CSS.styleSheetRemoved", std::move(messageData)));
}

void Frontend::flush() {
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification) {
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
    public:
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend, bool fallThroughForNotFound)
            : DispatcherBase(frontendChannel)
            , m_backend(backend)
            , m_fallThroughForNotFound(fallThroughForNotFound) {
            m_dispatchMap["CSS.addRule"] = &DispatcherImpl::addRule;
            m_dispatchMap["CSS.collectClassNames"] = &DispatcherImpl::collectClassNames;
            m_dispatchMap["CSS.createStyleSheet"] = &DispatcherImpl::createStyleSheet;
            m_dispatchMap["CSS.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["CSS.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["CSS.forcePseudoState"] = &DispatcherImpl::forcePseudoState;
            m_dispatchMap["CSS.getBackgroundColors"] = &DispatcherImpl::getBackgroundColors;
            m_dispatchMap["CSS.getComputedStyleForNode"] = &DispatcherImpl::getComputedStyleForNode;
            m_dispatchMap["CSS.getInlineStylesForNode"] = &DispatcherImpl::getInlineStylesForNode;
            m_dispatchMap["CSS.getMatchedStylesForNode"] = &DispatcherImpl::getMatchedStylesForNode;
            m_dispatchMap["CSS.getMediaQueries"] = &DispatcherImpl::getMediaQueries;
            m_dispatchMap["CSS.getPlatformFontsForNode"] = &DispatcherImpl::getPlatformFontsForNode;
            m_dispatchMap["CSS.getStyleSheetText"] = &DispatcherImpl::getStyleSheetText;
            m_dispatchMap["CSS.setEffectivePropertyValueForNode"] = &DispatcherImpl::setEffectivePropertyValueForNode;
            m_dispatchMap["CSS.setKeyframeKey"] = &DispatcherImpl::setKeyframeKey;
            m_dispatchMap["CSS.setMediaText"] = &DispatcherImpl::setMediaText;
            m_dispatchMap["CSS.setRuleSelector"] = &DispatcherImpl::setRuleSelector;
            m_dispatchMap["CSS.setStyleSheetText"] = &DispatcherImpl::setStyleSheetText;
            m_dispatchMap["CSS.setStyleTexts"] = &DispatcherImpl::setStyleTexts;
            m_dispatchMap["CSS.startRuleUsageTracking"] = &DispatcherImpl::startRuleUsageTracking;
            m_dispatchMap["CSS.stopRuleUsageTracking"] = &DispatcherImpl::stopRuleUsageTracking;
            m_dispatchMap["CSS.takeCoverageDelta"] = &DispatcherImpl::takeCoverageDelta;
        }
        ~DispatcherImpl() override { }
        DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
        HashMap<String, String>& redirects() {
            return m_redirects;
        }

    protected:
        using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;
        HashMap<String, String> m_redirects;

        DispatchResponse::Status addRule(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status collectClassNames(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status createStyleSheet(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status forcePseudoState(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getBackgroundColors(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getComputedStyleForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getInlineStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getMatchedStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getMediaQueries(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getPlatformFontsForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status getStyleSheetText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status setEffectivePropertyValueForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status setKeyframeKey(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status setMediaText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status setRuleSelector(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status setStyleSheetText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status setStyleTexts(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status startRuleUsageTracking(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status stopRuleUsageTracking(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        DispatchResponse::Status takeCoverageDelta(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

        Backend* m_backend;
        bool m_fallThroughForNotFound;
};

DispatchResponse::Status DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) {
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        if (m_fallThroughForNotFound) {
            return DispatchResponse::kFallThrough;
        }
        reportProtocolError(callId, DispatchResponse::kMethodNotFound, "'" + method + "' wasn't found", nullptr);
        return DispatchResponse::kError;
    }

    protocol::ErrorSupport errors;
    return (this->*(it->second))(callId, std::move(messageObject), &errors);
}


DispatchResponse::Status DispatcherImpl::addRule(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* ruleTextValue = object ? object->get("ruleText") : nullptr;
    errors->setName("ruleText");
    String in_ruleText = ValueConversions<String>::fromValue(ruleTextValue, errors);
    protocol::Value* locationValue = object ? object->get("location") : nullptr;
    errors->setName("location");
    std::unique_ptr<protocol::CSS::SourceRange> in_location = ValueConversions<protocol::CSS::SourceRange>::fromValue(locationValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::CSS::CSSRule> out_rule;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->addRule(in_styleSheetId, in_ruleText, std::move(in_location), &out_rule);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("rule", ValueConversions<protocol::CSS::CSSRule>::toValue(out_rule.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::collectClassNames(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<String>> out_classNames;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->collectClassNames(in_styleSheetId, &out_classNames);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("classNames", ValueConversions<protocol::Array<String>>::toValue(out_classNames.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::createStyleSheet(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_styleSheetId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->createStyleSheet(in_frameId, &out_styleSheetId);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("styleSheetId", ValueConversions<String>::toValue(out_styleSheetId));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::forcePseudoState(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* forcedPseudoClassesValue = object ? object->get("forcedPseudoClasses") : nullptr;
    errors->setName("forcedPseudoClasses");
    std::unique_ptr<protocol::Array<String>> in_forcedPseudoClasses = ValueConversions<protocol::Array<String>>::fromValue(forcedPseudoClassesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->forcePseudoState(in_nodeId, std::move(in_forcedPseudoClasses));
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getBackgroundColors(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    Maybe<protocol::Array<String>> out_backgroundColors;
    Maybe<String> out_computedFontSize;
    Maybe<String> out_computedFontWeight;
    Maybe<String> out_computedBodyFontSize;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getBackgroundColors(in_nodeId, &out_backgroundColors, &out_computedFontSize, &out_computedFontWeight, &out_computedBodyFontSize);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        if (out_backgroundColors.isJust()) {
            result->setValue("backgroundColors", ValueConversions<protocol::Array<String>>::toValue(out_backgroundColors.fromJust()));
        }
        if (out_computedFontSize.isJust()) {
            result->setValue("computedFontSize", ValueConversions<String>::toValue(out_computedFontSize.fromJust()));
        }
        if (out_computedFontWeight.isJust()) {
            result->setValue("computedFontWeight", ValueConversions<String>::toValue(out_computedFontWeight.fromJust()));
        }
        if (out_computedBodyFontSize.isJust()) {
            result->setValue("computedBodyFontSize", ValueConversions<String>::toValue(out_computedBodyFontSize.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getComputedStyleForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>> out_computedStyle;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getComputedStyleForNode(in_nodeId, &out_computedStyle);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("computedStyle", ValueConversions<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>::toValue(out_computedStyle.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getInlineStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    Maybe<protocol::CSS::CSSStyle> out_inlineStyle;
    Maybe<protocol::CSS::CSSStyle> out_attributesStyle;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getInlineStylesForNode(in_nodeId, &out_inlineStyle, &out_attributesStyle);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        if (out_inlineStyle.isJust()) {
            result->setValue("inlineStyle", ValueConversions<protocol::CSS::CSSStyle>::toValue(out_inlineStyle.fromJust()));
        }
        if (out_attributesStyle.isJust()) {
            result->setValue("attributesStyle", ValueConversions<protocol::CSS::CSSStyle>::toValue(out_attributesStyle.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getMatchedStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    Maybe<protocol::CSS::CSSStyle> out_inlineStyle;
    Maybe<protocol::CSS::CSSStyle> out_attributesStyle;
    Maybe<protocol::Array<protocol::CSS::RuleMatch>> out_matchedCSSRules;
    Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>> out_pseudoElements;
    Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>> out_inherited;
    Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>> out_cssKeyframesRules;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getMatchedStylesForNode(in_nodeId, &out_inlineStyle, &out_attributesStyle, &out_matchedCSSRules, &out_pseudoElements, &out_inherited, &out_cssKeyframesRules);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        if (out_inlineStyle.isJust()) {
            result->setValue("inlineStyle", ValueConversions<protocol::CSS::CSSStyle>::toValue(out_inlineStyle.fromJust()));
        }
        if (out_attributesStyle.isJust()) {
            result->setValue("attributesStyle", ValueConversions<protocol::CSS::CSSStyle>::toValue(out_attributesStyle.fromJust()));
        }
        if (out_matchedCSSRules.isJust()) {
            result->setValue("matchedCSSRules", ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::toValue(out_matchedCSSRules.fromJust()));
        }
        if (out_pseudoElements.isJust()) {
            result->setValue("pseudoElements", ValueConversions<protocol::Array<protocol::CSS::PseudoElementMatches>>::toValue(out_pseudoElements.fromJust()));
        }
        if (out_inherited.isJust()) {
            result->setValue("inherited", ValueConversions<protocol::Array<protocol::CSS::InheritedStyleEntry>>::toValue(out_inherited.fromJust()));
        }
        if (out_cssKeyframesRules.isJust()) {
            result->setValue("cssKeyframesRules", ValueConversions<protocol::Array<protocol::CSS::CSSKeyframesRule>>::toValue(out_cssKeyframesRules.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getMediaQueries(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::CSS::CSSMedia>> out_medias;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getMediaQueries(&out_medias);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("medias", ValueConversions<protocol::Array<protocol::CSS::CSSMedia>>::toValue(out_medias.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getPlatformFontsForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::CSS::PlatformFontUsage>> out_fonts;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getPlatformFontsForNode(in_nodeId, &out_fonts);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("fonts", ValueConversions<protocol::Array<protocol::CSS::PlatformFontUsage>>::toValue(out_fonts.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getStyleSheetText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_text;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getStyleSheetText(in_styleSheetId, &out_text);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("text", ValueConversions<String>::toValue(out_text));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setEffectivePropertyValueForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* propertyNameValue = object ? object->get("propertyName") : nullptr;
    errors->setName("propertyName");
    String in_propertyName = ValueConversions<String>::fromValue(propertyNameValue, errors);
    protocol::Value* valueValue = object ? object->get("value") : nullptr;
    errors->setName("value");
    String in_value = ValueConversions<String>::fromValue(valueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setEffectivePropertyValueForNode(in_nodeId, in_propertyName, in_value);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setKeyframeKey(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* rangeValue = object ? object->get("range") : nullptr;
    errors->setName("range");
    std::unique_ptr<protocol::CSS::SourceRange> in_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    protocol::Value* keyTextValue = object ? object->get("keyText") : nullptr;
    errors->setName("keyText");
    String in_keyText = ValueConversions<String>::fromValue(keyTextValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::CSS::Value> out_keyText;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setKeyframeKey(in_styleSheetId, std::move(in_range), in_keyText, &out_keyText);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("keyText", ValueConversions<protocol::CSS::Value>::toValue(out_keyText.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setMediaText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* rangeValue = object ? object->get("range") : nullptr;
    errors->setName("range");
    std::unique_ptr<protocol::CSS::SourceRange> in_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    errors->setName("text");
    String in_text = ValueConversions<String>::fromValue(textValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::CSS::CSSMedia> out_media;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setMediaText(in_styleSheetId, std::move(in_range), in_text, &out_media);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("media", ValueConversions<protocol::CSS::CSSMedia>::toValue(out_media.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setRuleSelector(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* rangeValue = object ? object->get("range") : nullptr;
    errors->setName("range");
    std::unique_ptr<protocol::CSS::SourceRange> in_range = ValueConversions<protocol::CSS::SourceRange>::fromValue(rangeValue, errors);
    protocol::Value* selectorValue = object ? object->get("selector") : nullptr;
    errors->setName("selector");
    String in_selector = ValueConversions<String>::fromValue(selectorValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::CSS::SelectorList> out_selectorList;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setRuleSelector(in_styleSheetId, std::move(in_range), in_selector, &out_selectorList);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("selectorList", ValueConversions<protocol::CSS::SelectorList>::toValue(out_selectorList.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setStyleSheetText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::fromValue(styleSheetIdValue, errors);
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    errors->setName("text");
    String in_text = ValueConversions<String>::fromValue(textValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    Maybe<String> out_sourceMapURL;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setStyleSheetText(in_styleSheetId, in_text, &out_sourceMapURL);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        if (out_sourceMapURL.isJust()) {
            result->setValue("sourceMapURL", ValueConversions<String>::toValue(out_sourceMapURL.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setStyleTexts(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* editsValue = object ? object->get("edits") : nullptr;
    errors->setName("edits");
    std::unique_ptr<protocol::Array<protocol::CSS::StyleDeclarationEdit>> in_edits = ValueConversions<protocol::Array<protocol::CSS::StyleDeclarationEdit>>::fromValue(editsValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::CSS::CSSStyle>> out_styles;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setStyleTexts(std::move(in_edits), &out_styles);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("styles", ValueConversions<protocol::Array<protocol::CSS::CSSStyle>>::toValue(out_styles.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::startRuleUsageTracking(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->startRuleUsageTracking();
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::stopRuleUsageTracking(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>> out_ruleUsage;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stopRuleUsageTracking(&out_ruleUsage);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("ruleUsage", ValueConversions<protocol::Array<protocol::CSS::RuleUsage>>::toValue(out_ruleUsage.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

DispatchResponse::Status DispatcherImpl::takeCoverageDelta(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::CSS::RuleUsage>> out_coverage;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->takeCoverageDelta(&out_coverage);
    if (response.status() == DispatchResponse::kFallThrough) {
        return response.status();
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("coverage", ValueConversions<protocol::Array<protocol::CSS::RuleUsage>>::toValue(out_coverage.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend) {
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("CSS", std::move(dispatcher));
}

} // CSS
} // namespace v8_inspector
} // namespace protocol

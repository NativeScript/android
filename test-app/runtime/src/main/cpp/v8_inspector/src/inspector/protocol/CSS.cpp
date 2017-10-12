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
const char Metainfo::version[] = "1.2";

namespace StyleSheetOriginEnum {
const char* Injected = "injected";
const char* UserAgent = "user-agent";
const char* Inspector = "inspector";
const char* Regular = "regular";
} // namespace StyleSheetOriginEnum

std::unique_ptr<PseudoElementMatches> PseudoElementMatches::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PseudoElementMatches> result(new PseudoElementMatches());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* pseudoTypeValue = object->get("pseudoType");
    errors->setName("pseudoType");
    result->m_pseudoType = ValueConversions<String>::parse(pseudoTypeValue, errors);
    protocol::Value* matchesValue = object->get("matches");
    errors->setName("matches");
    result->m_matches = ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::parse(matchesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PseudoElementMatches::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("pseudoType", ValueConversions<String>::serialize(m_pseudoType));
    result->setValue("matches", ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::serialize(m_matches.get()));
    return result;
}

std::unique_ptr<PseudoElementMatches> PseudoElementMatches::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<InheritedStyleEntry> InheritedStyleEntry::parse(protocol::Value* value, ErrorSupport* errors) {
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
        result->m_inlineStyle = ValueConversions<protocol::CSS::CSSStyle>::parse(inlineStyleValue, errors);
    }
    protocol::Value* matchedCSSRulesValue = object->get("matchedCSSRules");
    errors->setName("matchedCSSRules");
    result->m_matchedCSSRules = ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::parse(matchedCSSRulesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> InheritedStyleEntry::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_inlineStyle.isJust()) {
        result->setValue("inlineStyle", ValueConversions<protocol::CSS::CSSStyle>::serialize(m_inlineStyle.fromJust()));
    }
    result->setValue("matchedCSSRules", ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::serialize(m_matchedCSSRules.get()));
    return result;
}

std::unique_ptr<InheritedStyleEntry> InheritedStyleEntry::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<RuleMatch> RuleMatch::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RuleMatch> result(new RuleMatch());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* ruleValue = object->get("rule");
    errors->setName("rule");
    result->m_rule = ValueConversions<protocol::CSS::CSSRule>::parse(ruleValue, errors);
    protocol::Value* matchingSelectorsValue = object->get("matchingSelectors");
    errors->setName("matchingSelectors");
    result->m_matchingSelectors = ValueConversions<protocol::Array<int>>::parse(matchingSelectorsValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RuleMatch::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("rule", ValueConversions<protocol::CSS::CSSRule>::serialize(m_rule.get()));
    result->setValue("matchingSelectors", ValueConversions<protocol::Array<int>>::serialize(m_matchingSelectors.get()));
    return result;
}

std::unique_ptr<RuleMatch> RuleMatch::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<Value> Value::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Value> result(new Value());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::parse(textValue, errors);
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::parse(rangeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Value::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("text", ValueConversions<String>::serialize(m_text));
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::serialize(m_range.fromJust()));
    }
    return result;
}

std::unique_ptr<Value> Value::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<SelectorList> SelectorList::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SelectorList> result(new SelectorList());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* selectorsValue = object->get("selectors");
    errors->setName("selectors");
    result->m_selectors = ValueConversions<protocol::Array<protocol::CSS::Value>>::parse(selectorsValue, errors);
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::parse(textValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SelectorList::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("selectors", ValueConversions<protocol::Array<protocol::CSS::Value>>::serialize(m_selectors.get()));
    result->setValue("text", ValueConversions<String>::serialize(m_text));
    return result;
}

std::unique_ptr<SelectorList> SelectorList::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CSSStyleSheetHeader> CSSStyleSheetHeader::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSStyleSheetHeader> result(new CSSStyleSheetHeader());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    errors->setName("styleSheetId");
    result->m_styleSheetId = ValueConversions<String>::parse(styleSheetIdValue, errors);
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* sourceURLValue = object->get("sourceURL");
    errors->setName("sourceURL");
    result->m_sourceURL = ValueConversions<String>::parse(sourceURLValue, errors);
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::parse(sourceMapURLValue, errors);
    }
    protocol::Value* originValue = object->get("origin");
    errors->setName("origin");
    result->m_origin = ValueConversions<String>::parse(originValue, errors);
    protocol::Value* titleValue = object->get("title");
    errors->setName("title");
    result->m_title = ValueConversions<String>::parse(titleValue, errors);
    protocol::Value* ownerNodeValue = object->get("ownerNode");
    if (ownerNodeValue) {
        errors->setName("ownerNode");
        result->m_ownerNode = ValueConversions<int>::parse(ownerNodeValue, errors);
    }
    protocol::Value* disabledValue = object->get("disabled");
    errors->setName("disabled");
    result->m_disabled = ValueConversions<bool>::parse(disabledValue, errors);
    protocol::Value* hasSourceURLValue = object->get("hasSourceURL");
    if (hasSourceURLValue) {
        errors->setName("hasSourceURL");
        result->m_hasSourceURL = ValueConversions<bool>::parse(hasSourceURLValue, errors);
    }
    protocol::Value* isInlineValue = object->get("isInline");
    errors->setName("isInline");
    result->m_isInline = ValueConversions<bool>::parse(isInlineValue, errors);
    protocol::Value* startLineValue = object->get("startLine");
    errors->setName("startLine");
    result->m_startLine = ValueConversions<double>::parse(startLineValue, errors);
    protocol::Value* startColumnValue = object->get("startColumn");
    errors->setName("startColumn");
    result->m_startColumn = ValueConversions<double>::parse(startColumnValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSStyleSheetHeader::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("styleSheetId", ValueConversions<String>::serialize(m_styleSheetId));
    result->setValue("frameId", ValueConversions<String>::serialize(m_frameId));
    result->setValue("sourceURL", ValueConversions<String>::serialize(m_sourceURL));
    if (m_sourceMapURL.isJust()) {
        result->setValue("sourceMapURL", ValueConversions<String>::serialize(m_sourceMapURL.fromJust()));
    }
    result->setValue("origin", ValueConversions<String>::serialize(m_origin));
    result->setValue("title", ValueConversions<String>::serialize(m_title));
    if (m_ownerNode.isJust()) {
        result->setValue("ownerNode", ValueConversions<int>::serialize(m_ownerNode.fromJust()));
    }
    result->setValue("disabled", ValueConversions<bool>::serialize(m_disabled));
    if (m_hasSourceURL.isJust()) {
        result->setValue("hasSourceURL", ValueConversions<bool>::serialize(m_hasSourceURL.fromJust()));
    }
    result->setValue("isInline", ValueConversions<bool>::serialize(m_isInline));
    result->setValue("startLine", ValueConversions<double>::serialize(m_startLine));
    result->setValue("startColumn", ValueConversions<double>::serialize(m_startColumn));
    return result;
}

std::unique_ptr<CSSStyleSheetHeader> CSSStyleSheetHeader::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CSSRule> CSSRule::parse(protocol::Value* value, ErrorSupport* errors) {
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
        result->m_styleSheetId = ValueConversions<String>::parse(styleSheetIdValue, errors);
    }
    protocol::Value* selectorListValue = object->get("selectorList");
    errors->setName("selectorList");
    result->m_selectorList = ValueConversions<protocol::CSS::SelectorList>::parse(selectorListValue, errors);
    protocol::Value* originValue = object->get("origin");
    errors->setName("origin");
    result->m_origin = ValueConversions<String>::parse(originValue, errors);
    protocol::Value* styleValue = object->get("style");
    errors->setName("style");
    result->m_style = ValueConversions<protocol::CSS::CSSStyle>::parse(styleValue, errors);
    protocol::Value* mediaValue = object->get("media");
    if (mediaValue) {
        errors->setName("media");
        result->m_media = ValueConversions<protocol::Array<protocol::CSS::CSSMedia>>::parse(mediaValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSRule::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::serialize(m_styleSheetId.fromJust()));
    }
    result->setValue("selectorList", ValueConversions<protocol::CSS::SelectorList>::serialize(m_selectorList.get()));
    result->setValue("origin", ValueConversions<String>::serialize(m_origin));
    result->setValue("style", ValueConversions<protocol::CSS::CSSStyle>::serialize(m_style.get()));
    if (m_media.isJust()) {
        result->setValue("media", ValueConversions<protocol::Array<protocol::CSS::CSSMedia>>::serialize(m_media.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSRule> CSSRule::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<SourceRange> SourceRange::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SourceRange> result(new SourceRange());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* startLineValue = object->get("startLine");
    errors->setName("startLine");
    result->m_startLine = ValueConversions<int>::parse(startLineValue, errors);
    protocol::Value* startColumnValue = object->get("startColumn");
    errors->setName("startColumn");
    result->m_startColumn = ValueConversions<int>::parse(startColumnValue, errors);
    protocol::Value* endLineValue = object->get("endLine");
    errors->setName("endLine");
    result->m_endLine = ValueConversions<int>::parse(endLineValue, errors);
    protocol::Value* endColumnValue = object->get("endColumn");
    errors->setName("endColumn");
    result->m_endColumn = ValueConversions<int>::parse(endColumnValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SourceRange::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("startLine", ValueConversions<int>::serialize(m_startLine));
    result->setValue("startColumn", ValueConversions<int>::serialize(m_startColumn));
    result->setValue("endLine", ValueConversions<int>::serialize(m_endLine));
    result->setValue("endColumn", ValueConversions<int>::serialize(m_endColumn));
    return result;
}

std::unique_ptr<SourceRange> SourceRange::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<ShorthandEntry> ShorthandEntry::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ShorthandEntry> result(new ShorthandEntry());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::parse(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::parse(valueValue, errors);
    protocol::Value* importantValue = object->get("important");
    if (importantValue) {
        errors->setName("important");
        result->m_important = ValueConversions<bool>::parse(importantValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ShorthandEntry::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::serialize(m_name));
    result->setValue("value", ValueConversions<String>::serialize(m_value));
    if (m_important.isJust()) {
        result->setValue("important", ValueConversions<bool>::serialize(m_important.fromJust()));
    }
    return result;
}

std::unique_ptr<ShorthandEntry> ShorthandEntry::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CSSComputedStyleProperty> CSSComputedStyleProperty::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSComputedStyleProperty> result(new CSSComputedStyleProperty());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::parse(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::parse(valueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSComputedStyleProperty::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::serialize(m_name));
    result->setValue("value", ValueConversions<String>::serialize(m_value));
    return result;
}

std::unique_ptr<CSSComputedStyleProperty> CSSComputedStyleProperty::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CSSStyle> CSSStyle::parse(protocol::Value* value, ErrorSupport* errors) {
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
        result->m_styleSheetId = ValueConversions<String>::parse(styleSheetIdValue, errors);
    }
    protocol::Value* cssPropertiesValue = object->get("cssProperties");
    errors->setName("cssProperties");
    result->m_cssProperties = ValueConversions<protocol::Array<protocol::CSS::CSSProperty>>::parse(cssPropertiesValue, errors);
    protocol::Value* shorthandEntriesValue = object->get("shorthandEntries");
    errors->setName("shorthandEntries");
    result->m_shorthandEntries = ValueConversions<protocol::Array<protocol::CSS::ShorthandEntry>>::parse(shorthandEntriesValue, errors);
    protocol::Value* cssTextValue = object->get("cssText");
    if (cssTextValue) {
        errors->setName("cssText");
        result->m_cssText = ValueConversions<String>::parse(cssTextValue, errors);
    }
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::parse(rangeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSStyle::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::serialize(m_styleSheetId.fromJust()));
    }
    result->setValue("cssProperties", ValueConversions<protocol::Array<protocol::CSS::CSSProperty>>::serialize(m_cssProperties.get()));
    result->setValue("shorthandEntries", ValueConversions<protocol::Array<protocol::CSS::ShorthandEntry>>::serialize(m_shorthandEntries.get()));
    if (m_cssText.isJust()) {
        result->setValue("cssText", ValueConversions<String>::serialize(m_cssText.fromJust()));
    }
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::serialize(m_range.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSStyle> CSSStyle::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CSSProperty> CSSProperty::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSProperty> result(new CSSProperty());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::parse(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::parse(valueValue, errors);
    protocol::Value* importantValue = object->get("important");
    if (importantValue) {
        errors->setName("important");
        result->m_important = ValueConversions<bool>::parse(importantValue, errors);
    }
    protocol::Value* implicitValue = object->get("implicit");
    if (implicitValue) {
        errors->setName("implicit");
        result->m_implicit = ValueConversions<bool>::parse(implicitValue, errors);
    }
    protocol::Value* textValue = object->get("text");
    if (textValue) {
        errors->setName("text");
        result->m_text = ValueConversions<String>::parse(textValue, errors);
    }
    protocol::Value* parsedOkValue = object->get("parsedOk");
    if (parsedOkValue) {
        errors->setName("parsedOk");
        result->m_parsedOk = ValueConversions<bool>::parse(parsedOkValue, errors);
    }
    protocol::Value* disabledValue = object->get("disabled");
    if (disabledValue) {
        errors->setName("disabled");
        result->m_disabled = ValueConversions<bool>::parse(disabledValue, errors);
    }
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::parse(rangeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSProperty::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::serialize(m_name));
    result->setValue("value", ValueConversions<String>::serialize(m_value));
    if (m_important.isJust()) {
        result->setValue("important", ValueConversions<bool>::serialize(m_important.fromJust()));
    }
    if (m_implicit.isJust()) {
        result->setValue("implicit", ValueConversions<bool>::serialize(m_implicit.fromJust()));
    }
    if (m_text.isJust()) {
        result->setValue("text", ValueConversions<String>::serialize(m_text.fromJust()));
    }
    if (m_parsedOk.isJust()) {
        result->setValue("parsedOk", ValueConversions<bool>::serialize(m_parsedOk.fromJust()));
    }
    if (m_disabled.isJust()) {
        result->setValue("disabled", ValueConversions<bool>::serialize(m_disabled.fromJust()));
    }
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::serialize(m_range.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSProperty> CSSProperty::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

const char* CSSMedia::SourceEnum::MediaRule = "mediaRule";
const char* CSSMedia::SourceEnum::ImportRule = "importRule";
const char* CSSMedia::SourceEnum::LinkedSheet = "linkedSheet";
const char* CSSMedia::SourceEnum::InlineSheet = "inlineSheet";

std::unique_ptr<CSSMedia> CSSMedia::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSMedia> result(new CSSMedia());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::parse(textValue, errors);
    protocol::Value* sourceValue = object->get("source");
    errors->setName("source");
    result->m_source = ValueConversions<String>::parse(sourceValue, errors);
    protocol::Value* sourceURLValue = object->get("sourceURL");
    if (sourceURLValue) {
        errors->setName("sourceURL");
        result->m_sourceURL = ValueConversions<String>::parse(sourceURLValue, errors);
    }
    protocol::Value* rangeValue = object->get("range");
    if (rangeValue) {
        errors->setName("range");
        result->m_range = ValueConversions<protocol::CSS::SourceRange>::parse(rangeValue, errors);
    }
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    if (styleSheetIdValue) {
        errors->setName("styleSheetId");
        result->m_styleSheetId = ValueConversions<String>::parse(styleSheetIdValue, errors);
    }
    protocol::Value* mediaListValue = object->get("mediaList");
    if (mediaListValue) {
        errors->setName("mediaList");
        result->m_mediaList = ValueConversions<protocol::Array<protocol::CSS::MediaQuery>>::parse(mediaListValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSMedia::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("text", ValueConversions<String>::serialize(m_text));
    result->setValue("source", ValueConversions<String>::serialize(m_source));
    if (m_sourceURL.isJust()) {
        result->setValue("sourceURL", ValueConversions<String>::serialize(m_sourceURL.fromJust()));
    }
    if (m_range.isJust()) {
        result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::serialize(m_range.fromJust()));
    }
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::serialize(m_styleSheetId.fromJust()));
    }
    if (m_mediaList.isJust()) {
        result->setValue("mediaList", ValueConversions<protocol::Array<protocol::CSS::MediaQuery>>::serialize(m_mediaList.fromJust()));
    }
    return result;
}

std::unique_ptr<CSSMedia> CSSMedia::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<MediaQuery> MediaQuery::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<MediaQuery> result(new MediaQuery());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* expressionsValue = object->get("expressions");
    errors->setName("expressions");
    result->m_expressions = ValueConversions<protocol::Array<protocol::CSS::MediaQueryExpression>>::parse(expressionsValue, errors);
    protocol::Value* activeValue = object->get("active");
    errors->setName("active");
    result->m_active = ValueConversions<bool>::parse(activeValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> MediaQuery::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("expressions", ValueConversions<protocol::Array<protocol::CSS::MediaQueryExpression>>::serialize(m_expressions.get()));
    result->setValue("active", ValueConversions<bool>::serialize(m_active));
    return result;
}

std::unique_ptr<MediaQuery> MediaQuery::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<MediaQueryExpression> MediaQueryExpression::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<MediaQueryExpression> result(new MediaQueryExpression());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<double>::parse(valueValue, errors);
    protocol::Value* unitValue = object->get("unit");
    errors->setName("unit");
    result->m_unit = ValueConversions<String>::parse(unitValue, errors);
    protocol::Value* featureValue = object->get("feature");
    errors->setName("feature");
    result->m_feature = ValueConversions<String>::parse(featureValue, errors);
    protocol::Value* valueRangeValue = object->get("valueRange");
    if (valueRangeValue) {
        errors->setName("valueRange");
        result->m_valueRange = ValueConversions<protocol::CSS::SourceRange>::parse(valueRangeValue, errors);
    }
    protocol::Value* computedLengthValue = object->get("computedLength");
    if (computedLengthValue) {
        errors->setName("computedLength");
        result->m_computedLength = ValueConversions<double>::parse(computedLengthValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> MediaQueryExpression::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("value", ValueConversions<double>::serialize(m_value));
    result->setValue("unit", ValueConversions<String>::serialize(m_unit));
    result->setValue("feature", ValueConversions<String>::serialize(m_feature));
    if (m_valueRange.isJust()) {
        result->setValue("valueRange", ValueConversions<protocol::CSS::SourceRange>::serialize(m_valueRange.fromJust()));
    }
    if (m_computedLength.isJust()) {
        result->setValue("computedLength", ValueConversions<double>::serialize(m_computedLength.fromJust()));
    }
    return result;
}

std::unique_ptr<MediaQueryExpression> MediaQueryExpression::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<PlatformFontUsage> PlatformFontUsage::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PlatformFontUsage> result(new PlatformFontUsage());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* familyNameValue = object->get("familyName");
    errors->setName("familyName");
    result->m_familyName = ValueConversions<String>::parse(familyNameValue, errors);
    protocol::Value* isCustomFontValue = object->get("isCustomFont");
    errors->setName("isCustomFont");
    result->m_isCustomFont = ValueConversions<bool>::parse(isCustomFontValue, errors);
    protocol::Value* glyphCountValue = object->get("glyphCount");
    errors->setName("glyphCount");
    result->m_glyphCount = ValueConversions<double>::parse(glyphCountValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PlatformFontUsage::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("familyName", ValueConversions<String>::serialize(m_familyName));
    result->setValue("isCustomFont", ValueConversions<bool>::serialize(m_isCustomFont));
    result->setValue("glyphCount", ValueConversions<double>::serialize(m_glyphCount));
    return result;
}

std::unique_ptr<PlatformFontUsage> PlatformFontUsage::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CSSKeyframesRule> CSSKeyframesRule::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CSSKeyframesRule> result(new CSSKeyframesRule());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* animationNameValue = object->get("animationName");
    errors->setName("animationName");
    result->m_animationName = ValueConversions<protocol::CSS::Value>::parse(animationNameValue, errors);
    protocol::Value* keyframesValue = object->get("keyframes");
    errors->setName("keyframes");
    result->m_keyframes = ValueConversions<protocol::Array<protocol::CSS::CSSKeyframeRule>>::parse(keyframesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSKeyframesRule::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("animationName", ValueConversions<protocol::CSS::Value>::serialize(m_animationName.get()));
    result->setValue("keyframes", ValueConversions<protocol::Array<protocol::CSS::CSSKeyframeRule>>::serialize(m_keyframes.get()));
    return result;
}

std::unique_ptr<CSSKeyframesRule> CSSKeyframesRule::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<CSSKeyframeRule> CSSKeyframeRule::parse(protocol::Value* value, ErrorSupport* errors) {
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
        result->m_styleSheetId = ValueConversions<String>::parse(styleSheetIdValue, errors);
    }
    protocol::Value* originValue = object->get("origin");
    errors->setName("origin");
    result->m_origin = ValueConversions<String>::parse(originValue, errors);
    protocol::Value* keyTextValue = object->get("keyText");
    errors->setName("keyText");
    result->m_keyText = ValueConversions<protocol::CSS::Value>::parse(keyTextValue, errors);
    protocol::Value* styleValue = object->get("style");
    errors->setName("style");
    result->m_style = ValueConversions<protocol::CSS::CSSStyle>::parse(styleValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CSSKeyframeRule::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_styleSheetId.isJust()) {
        result->setValue("styleSheetId", ValueConversions<String>::serialize(m_styleSheetId.fromJust()));
    }
    result->setValue("origin", ValueConversions<String>::serialize(m_origin));
    result->setValue("keyText", ValueConversions<protocol::CSS::Value>::serialize(m_keyText.get()));
    result->setValue("style", ValueConversions<protocol::CSS::CSSStyle>::serialize(m_style.get()));
    return result;
}

std::unique_ptr<CSSKeyframeRule> CSSKeyframeRule::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<StyleDeclarationEdit> StyleDeclarationEdit::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<StyleDeclarationEdit> result(new StyleDeclarationEdit());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* styleSheetIdValue = object->get("styleSheetId");
    errors->setName("styleSheetId");
    result->m_styleSheetId = ValueConversions<String>::parse(styleSheetIdValue, errors);
    protocol::Value* rangeValue = object->get("range");
    errors->setName("range");
    result->m_range = ValueConversions<protocol::CSS::SourceRange>::parse(rangeValue, errors);
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::parse(textValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> StyleDeclarationEdit::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("styleSheetId", ValueConversions<String>::serialize(m_styleSheetId));
    result->setValue("range", ValueConversions<protocol::CSS::SourceRange>::serialize(m_range.get()));
    result->setValue("text", ValueConversions<String>::serialize(m_text));
    return result;
}

std::unique_ptr<StyleDeclarationEdit> StyleDeclarationEdit::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::mediaQueryResultChanged() {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "CSS.mediaQueryResultChanged");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::fontsUpdated() {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "CSS.fontsUpdated");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::styleSheetChanged(const String& styleSheetId) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "CSS.styleSheetChanged");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("styleSheetId", ValueConversions<String>::serialize(styleSheetId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::styleSheetAdded(std::unique_ptr<protocol::CSS::CSSStyleSheetHeader> header) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "CSS.styleSheetAdded");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("header", ValueConversions<protocol::CSS::CSSStyleSheetHeader>::serialize(header.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::styleSheetRemoved(const String& styleSheetId) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "CSS.styleSheetRemoved");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("styleSheetId", ValueConversions<String>::serialize(styleSheetId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::layoutEditorChange(const String& styleSheetId, std::unique_ptr<protocol::CSS::SourceRange> changeRange) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "CSS.layoutEditorChange");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("styleSheetId", ValueConversions<String>::serialize(styleSheetId));
    paramsObject->setValue("changeRange", ValueConversions<protocol::CSS::SourceRange>::serialize(changeRange.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::flush() {
    m_frontendChannel->flushProtocolNotifications();
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
    public:
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend)
            : DispatcherBase(frontendChannel)
            , m_backend(backend) {
            m_dispatchMap["CSS.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["CSS.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["CSS.getMatchedStylesForNode"] = &DispatcherImpl::getMatchedStylesForNode;
            m_dispatchMap["CSS.getInlineStylesForNode"] = &DispatcherImpl::getInlineStylesForNode;
            m_dispatchMap["CSS.getComputedStyleForNode"] = &DispatcherImpl::getComputedStyleForNode;
            m_dispatchMap["CSS.getPlatformFontsForNode"] = &DispatcherImpl::getPlatformFontsForNode;
            m_dispatchMap["CSS.getStyleSheetText"] = &DispatcherImpl::getStyleSheetText;
        }
        ~DispatcherImpl() override { }
        void dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

    protected:
        using CallHandler = void (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;

        void enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getMatchedStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getInlineStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getComputedStyleForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getPlatformFontsForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getStyleSheetText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

        Backend* m_backend;
};

void DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) {
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        reportProtocolError(callId, MethodNotFound, "'" + method + "' wasn't found", nullptr);
        return;
    }

    protocol::ErrorSupport errors;
    (this->*(it->second))(callId, std::move(messageObject), &errors);
}


class EnableCallbackImpl : public Backend::EnableCallback, public DispatcherBase::Callback {
    public:
        EnableCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId)
            : DispatcherBase::Callback(std::move(backendImpl), callId) { }

        void sendSuccess() override {
            std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
            sendIfActive(std::move(resultObject), ErrorString());
        }

        void sendFailure(const ErrorString& error) override {
            DCHECK(error.length());
            sendIfActive(nullptr, error);
        }

};

void DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    std::unique_ptr<EnableCallbackImpl> callback(new EnableCallbackImpl(weakPtr(), callId));

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    m_backend->enable(std::move(callback));
}

void DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->disable(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::getMatchedStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    Maybe<protocol::CSS::CSSStyle> out_inlineStyle;
    Maybe<protocol::CSS::CSSStyle> out_attributesStyle;
    Maybe<protocol::Array<protocol::CSS::RuleMatch>> out_matchedCSSRules;
    Maybe<protocol::Array<protocol::CSS::PseudoElementMatches>> out_pseudoElements;
    Maybe<protocol::Array<protocol::CSS::InheritedStyleEntry>> out_inherited;
    Maybe<protocol::Array<protocol::CSS::CSSKeyframesRule>> out_cssKeyframesRules;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getMatchedStylesForNode(&error, in_nodeId, &out_inlineStyle, &out_attributesStyle, &out_matchedCSSRules, &out_pseudoElements, &out_inherited, &out_cssKeyframesRules);
    if (!error.length()) {
        if (out_inlineStyle.isJust()) {
            result->setValue("inlineStyle", ValueConversions<protocol::CSS::CSSStyle>::serialize(out_inlineStyle.fromJust()));
        }
        if (out_attributesStyle.isJust()) {
            result->setValue("attributesStyle", ValueConversions<protocol::CSS::CSSStyle>::serialize(out_attributesStyle.fromJust()));
        }
        if (out_matchedCSSRules.isJust()) {
            result->setValue("matchedCSSRules", ValueConversions<protocol::Array<protocol::CSS::RuleMatch>>::serialize(out_matchedCSSRules.fromJust()));
        }
        if (out_pseudoElements.isJust()) {
            result->setValue("pseudoElements", ValueConversions<protocol::Array<protocol::CSS::PseudoElementMatches>>::serialize(out_pseudoElements.fromJust()));
        }
        if (out_inherited.isJust()) {
            result->setValue("inherited", ValueConversions<protocol::Array<protocol::CSS::InheritedStyleEntry>>::serialize(out_inherited.fromJust()));
        }
        if (out_cssKeyframesRules.isJust()) {
            result->setValue("cssKeyframesRules", ValueConversions<protocol::Array<protocol::CSS::CSSKeyframesRule>>::serialize(out_cssKeyframesRules.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::getInlineStylesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    Maybe<protocol::CSS::CSSStyle> out_inlineStyle;
    Maybe<protocol::CSS::CSSStyle> out_attributesStyle;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getInlineStylesForNode(&error, in_nodeId, &out_inlineStyle, &out_attributesStyle);
    if (!error.length()) {
        if (out_inlineStyle.isJust()) {
            result->setValue("inlineStyle", ValueConversions<protocol::CSS::CSSStyle>::serialize(out_inlineStyle.fromJust()));
        }
        if (out_attributesStyle.isJust()) {
            result->setValue("attributesStyle", ValueConversions<protocol::CSS::CSSStyle>::serialize(out_attributesStyle.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::getComputedStyleForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<protocol::CSS::CSSComputedStyleProperty>> out_computedStyle;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getComputedStyleForNode(&error, in_nodeId, &out_computedStyle);
    if (!error.length()) {
        result->setValue("computedStyle", ValueConversions<protocol::Array<protocol::CSS::CSSComputedStyleProperty>>::serialize(out_computedStyle.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::getPlatformFontsForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<protocol::CSS::PlatformFontUsage>> out_fonts;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getPlatformFontsForNode(&error, in_nodeId, &out_fonts);
    if (!error.length()) {
        result->setValue("fonts", ValueConversions<protocol::Array<protocol::CSS::PlatformFontUsage>>::serialize(out_fonts.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::getStyleSheetText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* styleSheetIdValue = object ? object->get("styleSheetId") : nullptr;
    errors->setName("styleSheetId");
    String in_styleSheetId = ValueConversions<String>::parse(styleSheetIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_text;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getStyleSheetText(&error, in_styleSheetId, &out_text);
    if (!error.length()) {
        result->setValue("text", ValueConversions<String>::serialize(out_text));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("CSS", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // CSS
} // namespace v8_inspector
} // namespace protocol

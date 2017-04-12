// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/DOM.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace DOM {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "DOM";
const char Metainfo::commandPrefix[] = "DOM.";
const char Metainfo::version[] = "1.2";

namespace PseudoTypeEnum {
const char* Before = "before";
const char* After = "after";
} // namespace PseudoTypeEnum

namespace ShadowRootTypeEnum {
const char* UserAgent = "user-agent";
const char* Open = "open";
const char* Closed = "closed";
} // namespace ShadowRootTypeEnum

namespace LiveRegionRelevantEnum {
const char* Additions = "additions";
const char* Removals = "removals";
const char* Text = "text";
} // namespace LiveRegionRelevantEnum

std::unique_ptr<Node> Node::parse(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Node> result(new Node());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* nodeTypeValue = object->get("nodeType");
    errors->setName("nodeType");
    result->m_nodeType = ValueConversions<int>::parse(nodeTypeValue, errors);
    protocol::Value* nodeNameValue = object->get("nodeName");
    errors->setName("nodeName");
    result->m_nodeName = ValueConversions<String>::parse(nodeNameValue, errors);
    protocol::Value* localNameValue = object->get("localName");
    errors->setName("localName");
    result->m_localName = ValueConversions<String>::parse(localNameValue, errors);
    protocol::Value* nodeValueValue = object->get("nodeValue");
    errors->setName("nodeValue");
    result->m_nodeValue = ValueConversions<String>::parse(nodeValueValue, errors);
    protocol::Value* childNodeCountValue = object->get("childNodeCount");
    if (childNodeCountValue) {
        errors->setName("childNodeCount");
        result->m_childNodeCount = ValueConversions<int>::parse(childNodeCountValue, errors);
    }
    protocol::Value* childrenValue = object->get("children");
    if (childrenValue) {
        errors->setName("children");
        result->m_children = ValueConversions<protocol::Array<protocol::DOM::Node>>::parse(childrenValue, errors);
    }
    protocol::Value* attributesValue = object->get("attributes");
    if (attributesValue) {
        errors->setName("attributes");
        result->m_attributes = ValueConversions<protocol::Array<String>>::parse(attributesValue, errors);
    }
    protocol::Value* documentURLValue = object->get("documentURL");
    if (documentURLValue) {
        errors->setName("documentURL");
        result->m_documentURL = ValueConversions<String>::parse(documentURLValue, errors);
    }
    protocol::Value* baseURLValue = object->get("baseURL");
    if (baseURLValue) {
        errors->setName("baseURL");
        result->m_baseURL = ValueConversions<String>::parse(baseURLValue, errors);
    }
    protocol::Value* publicIdValue = object->get("publicId");
    if (publicIdValue) {
        errors->setName("publicId");
        result->m_publicId = ValueConversions<String>::parse(publicIdValue, errors);
    }
    protocol::Value* systemIdValue = object->get("systemId");
    if (systemIdValue) {
        errors->setName("systemId");
        result->m_systemId = ValueConversions<String>::parse(systemIdValue, errors);
    }
    protocol::Value* xmlVersionValue = object->get("xmlVersion");
    if (xmlVersionValue) {
        errors->setName("xmlVersion");
        result->m_xmlVersion = ValueConversions<String>::parse(xmlVersionValue, errors);
    }
    protocol::Value* nameValue = object->get("name");
    if (nameValue) {
        errors->setName("name");
        result->m_name = ValueConversions<String>::parse(nameValue, errors);
    }
    protocol::Value* valueValue = object->get("value");
    if (valueValue) {
        errors->setName("value");
        result->m_value = ValueConversions<String>::parse(valueValue, errors);
    }
    protocol::Value* pseudoTypeValue = object->get("pseudoType");
    if (pseudoTypeValue) {
        errors->setName("pseudoType");
        result->m_pseudoType = ValueConversions<String>::parse(pseudoTypeValue, errors);
    }
    protocol::Value* shadowRootTypeValue = object->get("shadowRootType");
    if (shadowRootTypeValue) {
        errors->setName("shadowRootType");
        result->m_shadowRootType = ValueConversions<String>::parse(shadowRootTypeValue, errors);
    }
    protocol::Value* frameIdValue = object->get("frameId");
    if (frameIdValue) {
        errors->setName("frameId");
        result->m_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    }
    protocol::Value* contentDocumentValue = object->get("contentDocument");
    if (contentDocumentValue) {
        errors->setName("contentDocument");
        result->m_contentDocument = ValueConversions<protocol::DOM::Node>::parse(contentDocumentValue, errors);
    }
    protocol::Value* shadowRootsValue = object->get("shadowRoots");
    if (shadowRootsValue) {
        errors->setName("shadowRoots");
        result->m_shadowRoots = ValueConversions<protocol::Array<protocol::DOM::Node>>::parse(shadowRootsValue, errors);
    }
    protocol::Value* templateContentValue = object->get("templateContent");
    if (templateContentValue) {
        errors->setName("templateContent");
        result->m_templateContent = ValueConversions<protocol::DOM::Node>::parse(templateContentValue, errors);
    }
    protocol::Value* pseudoElementsValue = object->get("pseudoElements");
    if (pseudoElementsValue) {
        errors->setName("pseudoElements");
        result->m_pseudoElements = ValueConversions<protocol::Array<protocol::DOM::Node>>::parse(pseudoElementsValue, errors);
    }
    protocol::Value* roleValue = object->get("role");
    if (roleValue) {
        errors->setName("role");
        result->m_role = ValueConversions<String>::parse(roleValue, errors);
    }
    protocol::Value* contentSecurityPolicyHashValue = object->get("contentSecurityPolicyHash");
    if (contentSecurityPolicyHashValue) {
        errors->setName("contentSecurityPolicyHash");
        result->m_contentSecurityPolicyHash = ValueConversions<String>::parse(contentSecurityPolicyHashValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Node::serialize() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodeId", ValueConversions<int>::serialize(m_nodeId));
    result->setValue("nodeType", ValueConversions<int>::serialize(m_nodeType));
    result->setValue("nodeName", ValueConversions<String>::serialize(m_nodeName));
    result->setValue("localName", ValueConversions<String>::serialize(m_localName));
    result->setValue("nodeValue", ValueConversions<String>::serialize(m_nodeValue));
    if (m_childNodeCount.isJust())
        result->setValue("childNodeCount", ValueConversions<int>::serialize(m_childNodeCount.fromJust()));
    if (m_children.isJust())
        result->setValue("children", ValueConversions<protocol::Array<protocol::DOM::Node>>::serialize(m_children.fromJust()));
    if (m_attributes.isJust())
        result->setValue("attributes", ValueConversions<protocol::Array<String>>::serialize(m_attributes.fromJust()));
    if (m_documentURL.isJust())
        result->setValue("documentURL", ValueConversions<String>::serialize(m_documentURL.fromJust()));
    if (m_baseURL.isJust())
        result->setValue("baseURL", ValueConversions<String>::serialize(m_baseURL.fromJust()));
    if (m_publicId.isJust())
        result->setValue("publicId", ValueConversions<String>::serialize(m_publicId.fromJust()));
    if (m_systemId.isJust())
        result->setValue("systemId", ValueConversions<String>::serialize(m_systemId.fromJust()));
    if (m_xmlVersion.isJust())
        result->setValue("xmlVersion", ValueConversions<String>::serialize(m_xmlVersion.fromJust()));
    if (m_name.isJust())
        result->setValue("name", ValueConversions<String>::serialize(m_name.fromJust()));
    if (m_value.isJust())
        result->setValue("value", ValueConversions<String>::serialize(m_value.fromJust()));
    if (m_pseudoType.isJust())
        result->setValue("pseudoType", ValueConversions<String>::serialize(m_pseudoType.fromJust()));
    if (m_shadowRootType.isJust())
        result->setValue("shadowRootType", ValueConversions<String>::serialize(m_shadowRootType.fromJust()));
    if (m_frameId.isJust())
        result->setValue("frameId", ValueConversions<String>::serialize(m_frameId.fromJust()));
    if (m_contentDocument.isJust())
        result->setValue("contentDocument", ValueConversions<protocol::DOM::Node>::serialize(m_contentDocument.fromJust()));
    if (m_shadowRoots.isJust())
        result->setValue("shadowRoots", ValueConversions<protocol::Array<protocol::DOM::Node>>::serialize(m_shadowRoots.fromJust()));
    if (m_templateContent.isJust())
        result->setValue("templateContent", ValueConversions<protocol::DOM::Node>::serialize(m_templateContent.fromJust()));
    if (m_pseudoElements.isJust())
        result->setValue("pseudoElements", ValueConversions<protocol::Array<protocol::DOM::Node>>::serialize(m_pseudoElements.fromJust()));
    if (m_role.isJust())
        result->setValue("role", ValueConversions<String>::serialize(m_role.fromJust()));
    if (m_contentSecurityPolicyHash.isJust())
        result->setValue("contentSecurityPolicyHash", ValueConversions<String>::serialize(m_contentSecurityPolicyHash.fromJust()));
    return result;
}

std::unique_ptr<Node> Node::clone() const
{
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<EventListener> EventListener::parse(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<EventListener> result(new EventListener());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::parse(typeValue, errors);
    protocol::Value* useCaptureValue = object->get("useCapture");
    errors->setName("useCapture");
    result->m_useCapture = ValueConversions<bool>::parse(useCaptureValue, errors);
    protocol::Value* isAttributeValue = object->get("isAttribute");
    errors->setName("isAttribute");
    result->m_isAttribute = ValueConversions<bool>::parse(isAttributeValue, errors);
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* handlerBodyValue = object->get("handlerBody");
    errors->setName("handlerBody");
    result->m_handlerBody = ValueConversions<String>::parse(handlerBodyValue, errors);
    protocol::Value* locationValue = object->get("location");
    if (locationValue) {
        errors->setName("location");
        result->m_location = ValueConversions<protocol::Debugger::Location>::parse(locationValue, errors);
    }
    protocol::Value* sourceNameValue = object->get("sourceName");
    if (sourceNameValue) {
        errors->setName("sourceName");
        result->m_sourceName = ValueConversions<String>::parse(sourceNameValue, errors);
    }
    protocol::Value* handlerValue = object->get("handler");
    if (handlerValue) {
        errors->setName("handler");
        result->m_handler = ValueConversions<protocol::Runtime::RemoteObject>::parse(handlerValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> EventListener::serialize() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("type", ValueConversions<String>::serialize(m_type));
    result->setValue("useCapture", ValueConversions<bool>::serialize(m_useCapture));
    result->setValue("isAttribute", ValueConversions<bool>::serialize(m_isAttribute));
    result->setValue("nodeId", ValueConversions<int>::serialize(m_nodeId));
    result->setValue("handlerBody", ValueConversions<String>::serialize(m_handlerBody));
    if (m_location.isJust())
        result->setValue("location", ValueConversions<protocol::Debugger::Location>::serialize(m_location.fromJust()));
    if (m_sourceName.isJust())
        result->setValue("sourceName", ValueConversions<String>::serialize(m_sourceName.fromJust()));
    if (m_handler.isJust())
        result->setValue("handler", ValueConversions<protocol::Runtime::RemoteObject>::serialize(m_handler.fromJust()));
    return result;
}

std::unique_ptr<EventListener> EventListener::clone() const
{
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

const char* AccessibilityProperties::CheckedEnum::True = "true";
const char* AccessibilityProperties::CheckedEnum::False = "false";
const char* AccessibilityProperties::CheckedEnum::Mixed = "mixed";

const char* AccessibilityProperties::CurrentEnum::True = "true";
const char* AccessibilityProperties::CurrentEnum::False = "false";
const char* AccessibilityProperties::CurrentEnum::Page = "page";
const char* AccessibilityProperties::CurrentEnum::Step = "step";
const char* AccessibilityProperties::CurrentEnum::Location = "location";
const char* AccessibilityProperties::CurrentEnum::Date = "date";
const char* AccessibilityProperties::CurrentEnum::Time = "time";

const char* AccessibilityProperties::InvalidEnum::True = "true";
const char* AccessibilityProperties::InvalidEnum::False = "false";
const char* AccessibilityProperties::InvalidEnum::Grammar = "grammar";
const char* AccessibilityProperties::InvalidEnum::Spelling = "spelling";

const char* AccessibilityProperties::LiveRegionStatusEnum::Assertive = "assertive";
const char* AccessibilityProperties::LiveRegionStatusEnum::Polite = "polite";
const char* AccessibilityProperties::LiveRegionStatusEnum::Off = "off";

std::unique_ptr<AccessibilityProperties> AccessibilityProperties::parse(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<AccessibilityProperties> result(new AccessibilityProperties());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* activeDescendantNodeIdValue = object->get("activeDescendantNodeId");
    if (activeDescendantNodeIdValue) {
        errors->setName("activeDescendantNodeId");
        result->m_activeDescendantNodeId = ValueConversions<int>::parse(activeDescendantNodeIdValue, errors);
    }
    protocol::Value* busyValue = object->get("busy");
    if (busyValue) {
        errors->setName("busy");
        result->m_busy = ValueConversions<bool>::parse(busyValue, errors);
    }
    protocol::Value* checkedValue = object->get("checked");
    if (checkedValue) {
        errors->setName("checked");
        result->m_checked = ValueConversions<String>::parse(checkedValue, errors);
    }
    protocol::Value* childNodeIdsValue = object->get("childNodeIds");
    if (childNodeIdsValue) {
        errors->setName("childNodeIds");
        result->m_childNodeIds = ValueConversions<protocol::Array<int>>::parse(childNodeIdsValue, errors);
    }
    protocol::Value* controlledNodeIdsValue = object->get("controlledNodeIds");
    if (controlledNodeIdsValue) {
        errors->setName("controlledNodeIds");
        result->m_controlledNodeIds = ValueConversions<protocol::Array<int>>::parse(controlledNodeIdsValue, errors);
    }
    protocol::Value* currentValue = object->get("current");
    if (currentValue) {
        errors->setName("current");
        result->m_current = ValueConversions<String>::parse(currentValue, errors);
    }
    protocol::Value* disabledValue = object->get("disabled");
    if (disabledValue) {
        errors->setName("disabled");
        result->m_disabled = ValueConversions<bool>::parse(disabledValue, errors);
    }
    protocol::Value* headingLevelValue = object->get("headingLevel");
    if (headingLevelValue) {
        errors->setName("headingLevel");
        result->m_headingLevel = ValueConversions<double>::parse(headingLevelValue, errors);
    }
    protocol::Value* hierarchyLevelValue = object->get("hierarchyLevel");
    if (hierarchyLevelValue) {
        errors->setName("hierarchyLevel");
        result->m_hierarchyLevel = ValueConversions<double>::parse(hierarchyLevelValue, errors);
    }
    protocol::Value* existsValue = object->get("exists");
    errors->setName("exists");
    result->m_exists = ValueConversions<bool>::parse(existsValue, errors);
    protocol::Value* expandedValue = object->get("expanded");
    if (expandedValue) {
        errors->setName("expanded");
        result->m_expanded = ValueConversions<bool>::parse(expandedValue, errors);
    }
    protocol::Value* flowedNodeIdsValue = object->get("flowedNodeIds");
    if (flowedNodeIdsValue) {
        errors->setName("flowedNodeIds");
        result->m_flowedNodeIds = ValueConversions<protocol::Array<int>>::parse(flowedNodeIdsValue, errors);
    }
    protocol::Value* focusedValue = object->get("focused");
    if (focusedValue) {
        errors->setName("focused");
        result->m_focused = ValueConversions<bool>::parse(focusedValue, errors);
    }
    protocol::Value* ignoredValue = object->get("ignored");
    if (ignoredValue) {
        errors->setName("ignored");
        result->m_ignored = ValueConversions<bool>::parse(ignoredValue, errors);
    }
    protocol::Value* ignoredByDefaultValue = object->get("ignoredByDefault");
    if (ignoredByDefaultValue) {
        errors->setName("ignoredByDefault");
        result->m_ignoredByDefault = ValueConversions<bool>::parse(ignoredByDefaultValue, errors);
    }
    protocol::Value* invalidValue = object->get("invalid");
    if (invalidValue) {
        errors->setName("invalid");
        result->m_invalid = ValueConversions<String>::parse(invalidValue, errors);
    }
    protocol::Value* hiddenValue = object->get("hidden");
    if (hiddenValue) {
        errors->setName("hidden");
        result->m_hidden = ValueConversions<bool>::parse(hiddenValue, errors);
    }
    protocol::Value* labelValue = object->get("label");
    errors->setName("label");
    result->m_label = ValueConversions<String>::parse(labelValue, errors);
    protocol::Value* liveRegionAtomicValue = object->get("liveRegionAtomic");
    if (liveRegionAtomicValue) {
        errors->setName("liveRegionAtomic");
        result->m_liveRegionAtomic = ValueConversions<bool>::parse(liveRegionAtomicValue, errors);
    }
    protocol::Value* liveRegionRelevantValue = object->get("liveRegionRelevant");
    if (liveRegionRelevantValue) {
        errors->setName("liveRegionRelevant");
        result->m_liveRegionRelevant = ValueConversions<protocol::Array<String>>::parse(liveRegionRelevantValue, errors);
    }
    protocol::Value* liveRegionStatusValue = object->get("liveRegionStatus");
    if (liveRegionStatusValue) {
        errors->setName("liveRegionStatus");
        result->m_liveRegionStatus = ValueConversions<String>::parse(liveRegionStatusValue, errors);
    }
    protocol::Value* mouseEventNodeIdValue = object->get("mouseEventNodeId");
    if (mouseEventNodeIdValue) {
        errors->setName("mouseEventNodeId");
        result->m_mouseEventNodeId = ValueConversions<int>::parse(mouseEventNodeIdValue, errors);
    }
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* ownedNodeIdsValue = object->get("ownedNodeIds");
    if (ownedNodeIdsValue) {
        errors->setName("ownedNodeIds");
        result->m_ownedNodeIds = ValueConversions<protocol::Array<int>>::parse(ownedNodeIdsValue, errors);
    }
    protocol::Value* parentNodeIdValue = object->get("parentNodeId");
    if (parentNodeIdValue) {
        errors->setName("parentNodeId");
        result->m_parentNodeId = ValueConversions<int>::parse(parentNodeIdValue, errors);
    }
    protocol::Value* pressedValue = object->get("pressed");
    if (pressedValue) {
        errors->setName("pressed");
        result->m_pressed = ValueConversions<bool>::parse(pressedValue, errors);
    }
    protocol::Value* readonlyValue = object->get("readonly");
    if (readonlyValue) {
        errors->setName("readonly");
        result->m_readonly = ValueConversions<bool>::parse(readonlyValue, errors);
    }
    protocol::Value* requiredValue = object->get("required");
    if (requiredValue) {
        errors->setName("required");
        result->m_required = ValueConversions<bool>::parse(requiredValue, errors);
    }
    protocol::Value* roleValue = object->get("role");
    errors->setName("role");
    result->m_role = ValueConversions<String>::parse(roleValue, errors);
    protocol::Value* selectedValue = object->get("selected");
    if (selectedValue) {
        errors->setName("selected");
        result->m_selected = ValueConversions<bool>::parse(selectedValue, errors);
    }
    protocol::Value* selectedChildNodeIdsValue = object->get("selectedChildNodeIds");
    if (selectedChildNodeIdsValue) {
        errors->setName("selectedChildNodeIds");
        result->m_selectedChildNodeIds = ValueConversions<protocol::Array<int>>::parse(selectedChildNodeIdsValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> AccessibilityProperties::serialize() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_activeDescendantNodeId.isJust())
        result->setValue("activeDescendantNodeId", ValueConversions<int>::serialize(m_activeDescendantNodeId.fromJust()));
    if (m_busy.isJust())
        result->setValue("busy", ValueConversions<bool>::serialize(m_busy.fromJust()));
    if (m_checked.isJust())
        result->setValue("checked", ValueConversions<String>::serialize(m_checked.fromJust()));
    if (m_childNodeIds.isJust())
        result->setValue("childNodeIds", ValueConversions<protocol::Array<int>>::serialize(m_childNodeIds.fromJust()));
    if (m_controlledNodeIds.isJust())
        result->setValue("controlledNodeIds", ValueConversions<protocol::Array<int>>::serialize(m_controlledNodeIds.fromJust()));
    if (m_current.isJust())
        result->setValue("current", ValueConversions<String>::serialize(m_current.fromJust()));
    if (m_disabled.isJust())
        result->setValue("disabled", ValueConversions<bool>::serialize(m_disabled.fromJust()));
    if (m_headingLevel.isJust())
        result->setValue("headingLevel", ValueConversions<double>::serialize(m_headingLevel.fromJust()));
    if (m_hierarchyLevel.isJust())
        result->setValue("hierarchyLevel", ValueConversions<double>::serialize(m_hierarchyLevel.fromJust()));
    result->setValue("exists", ValueConversions<bool>::serialize(m_exists));
    if (m_expanded.isJust())
        result->setValue("expanded", ValueConversions<bool>::serialize(m_expanded.fromJust()));
    if (m_flowedNodeIds.isJust())
        result->setValue("flowedNodeIds", ValueConversions<protocol::Array<int>>::serialize(m_flowedNodeIds.fromJust()));
    if (m_focused.isJust())
        result->setValue("focused", ValueConversions<bool>::serialize(m_focused.fromJust()));
    if (m_ignored.isJust())
        result->setValue("ignored", ValueConversions<bool>::serialize(m_ignored.fromJust()));
    if (m_ignoredByDefault.isJust())
        result->setValue("ignoredByDefault", ValueConversions<bool>::serialize(m_ignoredByDefault.fromJust()));
    if (m_invalid.isJust())
        result->setValue("invalid", ValueConversions<String>::serialize(m_invalid.fromJust()));
    if (m_hidden.isJust())
        result->setValue("hidden", ValueConversions<bool>::serialize(m_hidden.fromJust()));
    result->setValue("label", ValueConversions<String>::serialize(m_label));
    if (m_liveRegionAtomic.isJust())
        result->setValue("liveRegionAtomic", ValueConversions<bool>::serialize(m_liveRegionAtomic.fromJust()));
    if (m_liveRegionRelevant.isJust())
        result->setValue("liveRegionRelevant", ValueConversions<protocol::Array<String>>::serialize(m_liveRegionRelevant.fromJust()));
    if (m_liveRegionStatus.isJust())
        result->setValue("liveRegionStatus", ValueConversions<String>::serialize(m_liveRegionStatus.fromJust()));
    if (m_mouseEventNodeId.isJust())
        result->setValue("mouseEventNodeId", ValueConversions<int>::serialize(m_mouseEventNodeId.fromJust()));
    result->setValue("nodeId", ValueConversions<int>::serialize(m_nodeId));
    if (m_ownedNodeIds.isJust())
        result->setValue("ownedNodeIds", ValueConversions<protocol::Array<int>>::serialize(m_ownedNodeIds.fromJust()));
    if (m_parentNodeId.isJust())
        result->setValue("parentNodeId", ValueConversions<int>::serialize(m_parentNodeId.fromJust()));
    if (m_pressed.isJust())
        result->setValue("pressed", ValueConversions<bool>::serialize(m_pressed.fromJust()));
    if (m_readonly.isJust())
        result->setValue("readonly", ValueConversions<bool>::serialize(m_readonly.fromJust()));
    if (m_required.isJust())
        result->setValue("required", ValueConversions<bool>::serialize(m_required.fromJust()));
    result->setValue("role", ValueConversions<String>::serialize(m_role));
    if (m_selected.isJust())
        result->setValue("selected", ValueConversions<bool>::serialize(m_selected.fromJust()));
    if (m_selectedChildNodeIds.isJust())
        result->setValue("selectedChildNodeIds", ValueConversions<protocol::Array<int>>::serialize(m_selectedChildNodeIds.fromJust()));
    return result;
}

std::unique_ptr<AccessibilityProperties> AccessibilityProperties::clone() const
{
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<RGBAColor> RGBAColor::parse(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RGBAColor> result(new RGBAColor());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* rValue = object->get("r");
    errors->setName("r");
    result->m_r = ValueConversions<int>::parse(rValue, errors);
    protocol::Value* gValue = object->get("g");
    errors->setName("g");
    result->m_g = ValueConversions<int>::parse(gValue, errors);
    protocol::Value* bValue = object->get("b");
    errors->setName("b");
    result->m_b = ValueConversions<int>::parse(bValue, errors);
    protocol::Value* aValue = object->get("a");
    if (aValue) {
        errors->setName("a");
        result->m_a = ValueConversions<double>::parse(aValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RGBAColor::serialize() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("r", ValueConversions<int>::serialize(m_r));
    result->setValue("g", ValueConversions<int>::serialize(m_g));
    result->setValue("b", ValueConversions<int>::serialize(m_b));
    if (m_a.isJust())
        result->setValue("a", ValueConversions<double>::serialize(m_a.fromJust()));
    return result;
}

std::unique_ptr<RGBAColor> RGBAColor::clone() const
{
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<HighlightConfig> HighlightConfig::parse(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<HighlightConfig> result(new HighlightConfig());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* showInfoValue = object->get("showInfo");
    if (showInfoValue) {
        errors->setName("showInfo");
        result->m_showInfo = ValueConversions<bool>::parse(showInfoValue, errors);
    }
    protocol::Value* contentColorValue = object->get("contentColor");
    if (contentColorValue) {
        errors->setName("contentColor");
        result->m_contentColor = ValueConversions<protocol::DOM::RGBAColor>::parse(contentColorValue, errors);
    }
    protocol::Value* paddingColorValue = object->get("paddingColor");
    if (paddingColorValue) {
        errors->setName("paddingColor");
        result->m_paddingColor = ValueConversions<protocol::DOM::RGBAColor>::parse(paddingColorValue, errors);
    }
    protocol::Value* borderColorValue = object->get("borderColor");
    if (borderColorValue) {
        errors->setName("borderColor");
        result->m_borderColor = ValueConversions<protocol::DOM::RGBAColor>::parse(borderColorValue, errors);
    }
    protocol::Value* marginColorValue = object->get("marginColor");
    if (marginColorValue) {
        errors->setName("marginColor");
        result->m_marginColor = ValueConversions<protocol::DOM::RGBAColor>::parse(marginColorValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> HighlightConfig::serialize() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_showInfo.isJust())
        result->setValue("showInfo", ValueConversions<bool>::serialize(m_showInfo.fromJust()));
    if (m_contentColor.isJust())
        result->setValue("contentColor", ValueConversions<protocol::DOM::RGBAColor>::serialize(m_contentColor.fromJust()));
    if (m_paddingColor.isJust())
        result->setValue("paddingColor", ValueConversions<protocol::DOM::RGBAColor>::serialize(m_paddingColor.fromJust()));
    if (m_borderColor.isJust())
        result->setValue("borderColor", ValueConversions<protocol::DOM::RGBAColor>::serialize(m_borderColor.fromJust()));
    if (m_marginColor.isJust())
        result->setValue("marginColor", ValueConversions<protocol::DOM::RGBAColor>::serialize(m_marginColor.fromJust()));
    return result;
}

std::unique_ptr<HighlightConfig> HighlightConfig::clone() const
{
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::documentUpdated()
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.documentUpdated");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::setChildNodes(int parentId, std::unique_ptr<protocol::Array<protocol::DOM::Node>> nodes)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.setChildNodes");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("parentId", ValueConversions<int>::serialize(parentId));
    paramsObject->setValue("nodes", ValueConversions<protocol::Array<protocol::DOM::Node>>::serialize(nodes.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::attributeModified(int nodeId, const String& name, const String& value)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.attributeModified");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("nodeId", ValueConversions<int>::serialize(nodeId));
    paramsObject->setValue("name", ValueConversions<String>::serialize(name));
    paramsObject->setValue("value", ValueConversions<String>::serialize(value));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::attributeRemoved(int nodeId, const String& name)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.attributeRemoved");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("nodeId", ValueConversions<int>::serialize(nodeId));
    paramsObject->setValue("name", ValueConversions<String>::serialize(name));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::inlineStyleInvalidated(std::unique_ptr<protocol::Array<int>> nodeIds)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.inlineStyleInvalidated");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("nodeIds", ValueConversions<protocol::Array<int>>::serialize(nodeIds.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::characterDataModified(int nodeId, const String& characterData)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.characterDataModified");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("nodeId", ValueConversions<int>::serialize(nodeId));
    paramsObject->setValue("characterData", ValueConversions<String>::serialize(characterData));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::childNodeCountUpdated(int nodeId, int childNodeCount)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.childNodeCountUpdated");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("nodeId", ValueConversions<int>::serialize(nodeId));
    paramsObject->setValue("childNodeCount", ValueConversions<int>::serialize(childNodeCount));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::childNodeInserted(int parentNodeId, int previousNodeId, std::unique_ptr<protocol::DOM::Node> node)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.childNodeInserted");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("parentNodeId", ValueConversions<int>::serialize(parentNodeId));
    paramsObject->setValue("previousNodeId", ValueConversions<int>::serialize(previousNodeId));
    paramsObject->setValue("node", ValueConversions<protocol::DOM::Node>::serialize(node.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::childNodeRemoved(int parentNodeId, int nodeId)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.childNodeRemoved");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("parentNodeId", ValueConversions<int>::serialize(parentNodeId));
    paramsObject->setValue("nodeId", ValueConversions<int>::serialize(nodeId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::shadowRootPushed(int hostId, std::unique_ptr<protocol::DOM::Node> root)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.shadowRootPushed");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("hostId", ValueConversions<int>::serialize(hostId));
    paramsObject->setValue("root", ValueConversions<protocol::DOM::Node>::serialize(root.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::shadowRootPopped(int hostId, int rootId)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.shadowRootPopped");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("hostId", ValueConversions<int>::serialize(hostId));
    paramsObject->setValue("rootId", ValueConversions<int>::serialize(rootId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::pseudoElementAdded(int parentId, std::unique_ptr<protocol::DOM::Node> pseudoElement)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.pseudoElementAdded");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("parentId", ValueConversions<int>::serialize(parentId));
    paramsObject->setValue("pseudoElement", ValueConversions<protocol::DOM::Node>::serialize(pseudoElement.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::pseudoElementRemoved(int parentId, int pseudoElementId)
{
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "DOM.pseudoElementRemoved");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("parentId", ValueConversions<int>::serialize(parentId));
    paramsObject->setValue("pseudoElementId", ValueConversions<int>::serialize(pseudoElementId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
}

void Frontend::flush()
{
    m_frontendChannel->flushProtocolNotifications();
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
public:
    DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend)
        : DispatcherBase(frontendChannel)
        , m_backend(backend) {
        m_dispatchMap["DOM.getDocument"] = &DispatcherImpl::getDocument;
        m_dispatchMap["DOM.requestChildNodes"] = &DispatcherImpl::requestChildNodes;
        m_dispatchMap["DOM.querySelector"] = &DispatcherImpl::querySelector;
        m_dispatchMap["DOM.querySelectorAll"] = &DispatcherImpl::querySelectorAll;
        m_dispatchMap["DOM.setNodeName"] = &DispatcherImpl::setNodeName;
        m_dispatchMap["DOM.setNodeValue"] = &DispatcherImpl::setNodeValue;
        m_dispatchMap["DOM.removeNode"] = &DispatcherImpl::removeNode;
        m_dispatchMap["DOM.setAttributeValue"] = &DispatcherImpl::setAttributeValue;
        m_dispatchMap["DOM.setAttributesAsText"] = &DispatcherImpl::setAttributesAsText;
        m_dispatchMap["DOM.removeAttribute"] = &DispatcherImpl::removeAttribute;
        m_dispatchMap["DOM.getEventListenersForNode"] = &DispatcherImpl::getEventListenersForNode;
        m_dispatchMap["DOM.getAccessibilityPropertiesForNode"] = &DispatcherImpl::getAccessibilityPropertiesForNode;
        m_dispatchMap["DOM.getOuterHTML"] = &DispatcherImpl::getOuterHTML;
        m_dispatchMap["DOM.setOuterHTML"] = &DispatcherImpl::setOuterHTML;
        m_dispatchMap["DOM.performSearch"] = &DispatcherImpl::performSearch;
        m_dispatchMap["DOM.getSearchResults"] = &DispatcherImpl::getSearchResults;
        m_dispatchMap["DOM.discardSearchResults"] = &DispatcherImpl::discardSearchResults;
        m_dispatchMap["DOM.requestNode"] = &DispatcherImpl::requestNode;
        m_dispatchMap["DOM.setInspectModeEnabled"] = &DispatcherImpl::setInspectModeEnabled;
        m_dispatchMap["DOM.highlightRect"] = &DispatcherImpl::highlightRect;
        m_dispatchMap["DOM.highlightQuad"] = &DispatcherImpl::highlightQuad;
        m_dispatchMap["DOM.highlightSelector"] = &DispatcherImpl::highlightSelector;
        m_dispatchMap["DOM.highlightNode"] = &DispatcherImpl::highlightNode;
        m_dispatchMap["DOM.hideHighlight"] = &DispatcherImpl::hideHighlight;
        m_dispatchMap["DOM.highlightFrame"] = &DispatcherImpl::highlightFrame;
        m_dispatchMap["DOM.pushNodeByPathToFrontend"] = &DispatcherImpl::pushNodeByPathToFrontend;
        m_dispatchMap["DOM.pushNodeByBackendIdToFrontend"] = &DispatcherImpl::pushNodeByBackendIdToFrontend;
        m_dispatchMap["DOM.releaseBackendNodeIds"] = &DispatcherImpl::releaseBackendNodeIds;
        m_dispatchMap["DOM.resolveNode"] = &DispatcherImpl::resolveNode;
        m_dispatchMap["DOM.getAttributes"] = &DispatcherImpl::getAttributes;
        m_dispatchMap["DOM.moveTo"] = &DispatcherImpl::moveTo;
        m_dispatchMap["DOM.undo"] = &DispatcherImpl::undo;
        m_dispatchMap["DOM.redo"] = &DispatcherImpl::redo;
        m_dispatchMap["DOM.markUndoableState"] = &DispatcherImpl::markUndoableState;
        m_dispatchMap["DOM.focus"] = &DispatcherImpl::focus;
    }
    ~DispatcherImpl() override { }
    void dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

protected:
    using CallHandler = void (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String, CallHandler>;
    DispatchMap m_dispatchMap;

    void getDocument(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void requestChildNodes(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void querySelector(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void querySelectorAll(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void setNodeName(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void setNodeValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void removeNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void setAttributeValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void setAttributesAsText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void removeAttribute(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void getEventListenersForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void getAccessibilityPropertiesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void getOuterHTML(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void setOuterHTML(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void performSearch(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void getSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void discardSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void requestNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void setInspectModeEnabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void highlightRect(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void highlightQuad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void highlightSelector(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void highlightNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void highlightFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void pushNodeByPathToFrontend(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void pushNodeByBackendIdToFrontend(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void releaseBackendNodeIds(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void resolveNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void getAttributes(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void moveTo(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void undo(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void redo(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void markUndoableState(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void focus(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

    Backend* m_backend;
};

void DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject)
{
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        reportProtocolError(callId, MethodNotFound, "'" + method + "' wasn't found", nullptr);
        return;
    }

    protocol::ErrorSupport errors;
    (this->*(it->second))(callId, std::move(messageObject), &errors);
}


void DispatcherImpl::getDocument(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::DOM::Node> out_root;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getDocument(&error, &out_root);
    if (!error.length()) {
        result->setValue("root", ValueConversions<protocol::DOM::Node>::serialize(out_root.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::requestChildNodes(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* depthValue = object ? object->get("depth") : nullptr;
    Maybe<int> in_depth;
    if (depthValue) {
        errors->setName("depth");
        in_depth = ValueConversions<int>::parse(depthValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->requestChildNodes(&error, in_nodeId, in_depth);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::querySelector(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* selectorValue = object ? object->get("selector") : nullptr;
    errors->setName("selector");
    String in_selector = ValueConversions<String>::parse(selectorValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    int out_nodeId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->querySelector(&error, in_nodeId, in_selector, &out_nodeId);
    if (!error.length()) {
        result->setValue("nodeId", ValueConversions<int>::serialize(out_nodeId));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::querySelectorAll(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* selectorValue = object ? object->get("selector") : nullptr;
    errors->setName("selector");
    String in_selector = ValueConversions<String>::parse(selectorValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<int>> out_nodeIds;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->querySelectorAll(&error, in_nodeId, in_selector, &out_nodeIds);
    if (!error.length()) {
        result->setValue("nodeIds", ValueConversions<protocol::Array<int>>::serialize(out_nodeIds.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::setNodeName(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* nameValue = object ? object->get("name") : nullptr;
    errors->setName("name");
    String in_name = ValueConversions<String>::parse(nameValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    int out_nodeId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setNodeName(&error, in_nodeId, in_name, &out_nodeId);
    if (!error.length()) {
        result->setValue("nodeId", ValueConversions<int>::serialize(out_nodeId));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::setNodeValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* valueValue = object ? object->get("value") : nullptr;
    errors->setName("value");
    String in_value = ValueConversions<String>::parse(valueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setNodeValue(&error, in_nodeId, in_value);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::removeNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
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

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->removeNode(&error, in_nodeId);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::setAttributeValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* nameValue = object ? object->get("name") : nullptr;
    errors->setName("name");
    String in_name = ValueConversions<String>::parse(nameValue, errors);
    protocol::Value* valueValue = object ? object->get("value") : nullptr;
    errors->setName("value");
    String in_value = ValueConversions<String>::parse(valueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setAttributeValue(&error, in_nodeId, in_name, in_value);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::setAttributesAsText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    errors->setName("text");
    String in_text = ValueConversions<String>::parse(textValue, errors);
    protocol::Value* nameValue = object ? object->get("name") : nullptr;
    Maybe<String> in_name;
    if (nameValue) {
        errors->setName("name");
        in_name = ValueConversions<String>::parse(nameValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setAttributesAsText(&error, in_nodeId, in_text, in_name);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::removeAttribute(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* nameValue = object ? object->get("name") : nullptr;
    errors->setName("name");
    String in_name = ValueConversions<String>::parse(nameValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->removeAttribute(&error, in_nodeId, in_name);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::getEventListenersForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    Maybe<String> in_objectGroup;
    if (objectGroupValue) {
        errors->setName("objectGroup");
        in_objectGroup = ValueConversions<String>::parse(objectGroupValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<protocol::DOM::EventListener>> out_listeners;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getEventListenersForNode(&error, in_nodeId, in_objectGroup, &out_listeners);
    if (!error.length()) {
        result->setValue("listeners", ValueConversions<protocol::Array<protocol::DOM::EventListener>>::serialize(out_listeners.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::getAccessibilityPropertiesForNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
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
    std::unique_ptr<protocol::DOM::AccessibilityProperties> out_properties;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getAccessibilityPropertiesForNode(&error, in_nodeId, &out_properties);
    if (!error.length()) {
        result->setValue("properties", ValueConversions<protocol::DOM::AccessibilityProperties>::serialize(out_properties.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::getOuterHTML(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
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
    String out_outerHTML;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getOuterHTML(&error, in_nodeId, &out_outerHTML);
    if (!error.length()) {
        result->setValue("outerHTML", ValueConversions<String>::serialize(out_outerHTML));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::setOuterHTML(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* outerHTMLValue = object ? object->get("outerHTML") : nullptr;
    errors->setName("outerHTML");
    String in_outerHTML = ValueConversions<String>::parse(outerHTMLValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setOuterHTML(&error, in_nodeId, in_outerHTML);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::performSearch(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* queryValue = object ? object->get("query") : nullptr;
    errors->setName("query");
    String in_query = ValueConversions<String>::parse(queryValue, errors);
    protocol::Value* nodeIdsValue = object ? object->get("nodeIds") : nullptr;
    Maybe<protocol::Array<int>> in_nodeIds;
    if (nodeIdsValue) {
        errors->setName("nodeIds");
        in_nodeIds = ValueConversions<protocol::Array<int>>::parse(nodeIdsValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_searchId;
    int out_resultCount;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->performSearch(&error, in_query, in_nodeIds, &out_searchId, &out_resultCount);
    if (!error.length()) {
        result->setValue("searchId", ValueConversions<String>::serialize(out_searchId));
        result->setValue("resultCount", ValueConversions<int>::serialize(out_resultCount));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::getSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* searchIdValue = object ? object->get("searchId") : nullptr;
    errors->setName("searchId");
    String in_searchId = ValueConversions<String>::parse(searchIdValue, errors);
    protocol::Value* fromIndexValue = object ? object->get("fromIndex") : nullptr;
    errors->setName("fromIndex");
    int in_fromIndex = ValueConversions<int>::parse(fromIndexValue, errors);
    protocol::Value* toIndexValue = object ? object->get("toIndex") : nullptr;
    errors->setName("toIndex");
    int in_toIndex = ValueConversions<int>::parse(toIndexValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<int>> out_nodeIds;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getSearchResults(&error, in_searchId, in_fromIndex, in_toIndex, &out_nodeIds);
    if (!error.length()) {
        result->setValue("nodeIds", ValueConversions<protocol::Array<int>>::serialize(out_nodeIds.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::discardSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* searchIdValue = object ? object->get("searchId") : nullptr;
    errors->setName("searchId");
    String in_searchId = ValueConversions<String>::parse(searchIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->discardSearchResults(&error, in_searchId);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::requestNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* objectIdValue = object ? object->get("objectId") : nullptr;
    errors->setName("objectId");
    String in_objectId = ValueConversions<String>::parse(objectIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    int out_nodeId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->requestNode(&error, in_objectId, &out_nodeId);
    if (!error.length()) {
        result->setValue("nodeId", ValueConversions<int>::serialize(out_nodeId));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::setInspectModeEnabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* enabledValue = object ? object->get("enabled") : nullptr;
    errors->setName("enabled");
    bool in_enabled = ValueConversions<bool>::parse(enabledValue, errors);
    protocol::Value* highlightConfigValue = object ? object->get("highlightConfig") : nullptr;
    Maybe<protocol::DOM::HighlightConfig> in_highlightConfig;
    if (highlightConfigValue) {
        errors->setName("highlightConfig");
        in_highlightConfig = ValueConversions<protocol::DOM::HighlightConfig>::parse(highlightConfigValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setInspectModeEnabled(&error, in_enabled, in_highlightConfig);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::highlightRect(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* xValue = object ? object->get("x") : nullptr;
    errors->setName("x");
    int in_x = ValueConversions<int>::parse(xValue, errors);
    protocol::Value* yValue = object ? object->get("y") : nullptr;
    errors->setName("y");
    int in_y = ValueConversions<int>::parse(yValue, errors);
    protocol::Value* widthValue = object ? object->get("width") : nullptr;
    errors->setName("width");
    int in_width = ValueConversions<int>::parse(widthValue, errors);
    protocol::Value* heightValue = object ? object->get("height") : nullptr;
    errors->setName("height");
    int in_height = ValueConversions<int>::parse(heightValue, errors);
    protocol::Value* colorValue = object ? object->get("color") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_color;
    if (colorValue) {
        errors->setName("color");
        in_color = ValueConversions<protocol::DOM::RGBAColor>::parse(colorValue, errors);
    }
    protocol::Value* outlineColorValue = object ? object->get("outlineColor") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_outlineColor;
    if (outlineColorValue) {
        errors->setName("outlineColor");
        in_outlineColor = ValueConversions<protocol::DOM::RGBAColor>::parse(outlineColorValue, errors);
    }
    protocol::Value* usePageCoordinatesValue = object ? object->get("usePageCoordinates") : nullptr;
    Maybe<bool> in_usePageCoordinates;
    if (usePageCoordinatesValue) {
        errors->setName("usePageCoordinates");
        in_usePageCoordinates = ValueConversions<bool>::parse(usePageCoordinatesValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->highlightRect(&error, in_x, in_y, in_width, in_height, in_color, in_outlineColor, in_usePageCoordinates);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::highlightQuad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* quadValue = object ? object->get("quad") : nullptr;
    errors->setName("quad");
    std::unique_ptr<protocol::Array<double>> in_quad = ValueConversions<protocol::Array<double>>::parse(quadValue, errors);
    protocol::Value* colorValue = object ? object->get("color") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_color;
    if (colorValue) {
        errors->setName("color");
        in_color = ValueConversions<protocol::DOM::RGBAColor>::parse(colorValue, errors);
    }
    protocol::Value* outlineColorValue = object ? object->get("outlineColor") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_outlineColor;
    if (outlineColorValue) {
        errors->setName("outlineColor");
        in_outlineColor = ValueConversions<protocol::DOM::RGBAColor>::parse(outlineColorValue, errors);
    }
    protocol::Value* usePageCoordinatesValue = object ? object->get("usePageCoordinates") : nullptr;
    Maybe<bool> in_usePageCoordinates;
    if (usePageCoordinatesValue) {
        errors->setName("usePageCoordinates");
        in_usePageCoordinates = ValueConversions<bool>::parse(usePageCoordinatesValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->highlightQuad(&error, std::move(in_quad), in_color, in_outlineColor, in_usePageCoordinates);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::highlightSelector(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* highlightConfigValue = object ? object->get("highlightConfig") : nullptr;
    errors->setName("highlightConfig");
    std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig = ValueConversions<protocol::DOM::HighlightConfig>::parse(highlightConfigValue, errors);
    protocol::Value* selectorStringValue = object ? object->get("selectorString") : nullptr;
    errors->setName("selectorString");
    String in_selectorString = ValueConversions<String>::parse(selectorStringValue, errors);
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    Maybe<String> in_frameId;
    if (frameIdValue) {
        errors->setName("frameId");
        in_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->highlightSelector(&error, std::move(in_highlightConfig), in_selectorString, in_frameId);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::highlightNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* highlightConfigValue = object ? object->get("highlightConfig") : nullptr;
    errors->setName("highlightConfig");
    std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig = ValueConversions<protocol::DOM::HighlightConfig>::parse(highlightConfigValue, errors);
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    Maybe<int> in_nodeId;
    if (nodeIdValue) {
        errors->setName("nodeId");
        in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    }
    protocol::Value* objectIdValue = object ? object->get("objectId") : nullptr;
    Maybe<String> in_objectId;
    if (objectIdValue) {
        errors->setName("objectId");
        in_objectId = ValueConversions<String>::parse(objectIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->highlightNode(&error, std::move(in_highlightConfig), in_nodeId, in_objectId);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->hideHighlight(&error);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::highlightFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* contentColorValue = object ? object->get("contentColor") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_contentColor;
    if (contentColorValue) {
        errors->setName("contentColor");
        in_contentColor = ValueConversions<protocol::DOM::RGBAColor>::parse(contentColorValue, errors);
    }
    protocol::Value* contentOutlineColorValue = object ? object->get("contentOutlineColor") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_contentOutlineColor;
    if (contentOutlineColorValue) {
        errors->setName("contentOutlineColor");
        in_contentOutlineColor = ValueConversions<protocol::DOM::RGBAColor>::parse(contentOutlineColorValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->highlightFrame(&error, in_frameId, in_contentColor, in_contentOutlineColor);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::pushNodeByPathToFrontend(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* pathValue = object ? object->get("path") : nullptr;
    errors->setName("path");
    String in_path = ValueConversions<String>::parse(pathValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    int out_nodeId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->pushNodeByPathToFrontend(&error, in_path, &out_nodeId);
    if (!error.length()) {
        result->setValue("nodeId", ValueConversions<int>::serialize(out_nodeId));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::pushNodeByBackendIdToFrontend(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* backendNodeIdValue = object ? object->get("backendNodeId") : nullptr;
    errors->setName("backendNodeId");
    int in_backendNodeId = ValueConversions<int>::parse(backendNodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    int out_nodeId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->pushNodeByBackendIdToFrontend(&error, in_backendNodeId, &out_nodeId);
    if (!error.length()) {
        result->setValue("nodeId", ValueConversions<int>::serialize(out_nodeId));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::releaseBackendNodeIds(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeGroupValue = object ? object->get("nodeGroup") : nullptr;
    errors->setName("nodeGroup");
    String in_nodeGroup = ValueConversions<String>::parse(nodeGroupValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->releaseBackendNodeIds(&error, in_nodeGroup);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::resolveNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    Maybe<String> in_objectGroup;
    if (objectGroupValue) {
        errors->setName("objectGroup");
        in_objectGroup = ValueConversions<String>::parse(objectGroupValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Runtime::RemoteObject> out_object;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->resolveNode(&error, in_nodeId, in_objectGroup, &out_object);
    if (!error.length()) {
        result->setValue("object", ValueConversions<protocol::Runtime::RemoteObject>::serialize(out_object.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::getAttributes(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
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
    std::unique_ptr<protocol::Array<String>> out_attributes;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getAttributes(&error, in_nodeId, &out_attributes);
    if (!error.length()) {
        result->setValue("attributes", ValueConversions<protocol::Array<String>>::serialize(out_attributes.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::moveTo(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    protocol::Value* targetNodeIdValue = object ? object->get("targetNodeId") : nullptr;
    errors->setName("targetNodeId");
    int in_targetNodeId = ValueConversions<int>::parse(targetNodeIdValue, errors);
    protocol::Value* insertBeforeNodeIdValue = object ? object->get("insertBeforeNodeId") : nullptr;
    Maybe<int> in_insertBeforeNodeId;
    if (insertBeforeNodeIdValue) {
        errors->setName("insertBeforeNodeId");
        in_insertBeforeNodeId = ValueConversions<int>::parse(insertBeforeNodeIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    int out_nodeId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->moveTo(&error, in_nodeId, in_targetNodeId, in_insertBeforeNodeId, &out_nodeId);
    if (!error.length()) {
        result->setValue("nodeId", ValueConversions<int>::serialize(out_nodeId));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, error, std::move(result));
}

void DispatcherImpl::undo(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->undo(&error);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::redo(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->redo(&error);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::markUndoableState(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->markUndoableState(&error);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

void DispatcherImpl::focus(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
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

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->focus(&error, in_nodeId);
    if (weak->get())
        weak->get()->sendResponse(callId, error);
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend)
{
    dispatcher->registerBackend("DOM", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // DOM
} // namespace v8_inspector
} // namespace protocol

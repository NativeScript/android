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

std::unique_ptr<Node> Node::fromValue(protocol::Value* value, ErrorSupport* errors)
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
    result->m_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* nodeTypeValue = object->get("nodeType");
    errors->setName("nodeType");
    result->m_nodeType = ValueConversions<int>::fromValue(nodeTypeValue, errors);
    protocol::Value* nodeNameValue = object->get("nodeName");
    errors->setName("nodeName");
    result->m_nodeName = ValueConversions<String>::fromValue(nodeNameValue, errors);
    protocol::Value* localNameValue = object->get("localName");
    errors->setName("localName");
    result->m_localName = ValueConversions<String>::fromValue(localNameValue, errors);
    protocol::Value* nodeValueValue = object->get("nodeValue");
    errors->setName("nodeValue");
    result->m_nodeValue = ValueConversions<String>::fromValue(nodeValueValue, errors);
    protocol::Value* childNodeCountValue = object->get("childNodeCount");
    if (childNodeCountValue) {
        errors->setName("childNodeCount");
        result->m_childNodeCount = ValueConversions<int>::fromValue(childNodeCountValue, errors);
    }
    protocol::Value* childrenValue = object->get("children");
    if (childrenValue) {
        errors->setName("children");
        result->m_children = ValueConversions<protocol::Array<protocol::DOM::Node>>::fromValue(childrenValue, errors);
    }
    protocol::Value* attributesValue = object->get("attributes");
    if (attributesValue) {
        errors->setName("attributes");
        result->m_attributes = ValueConversions<protocol::Array<String>>::fromValue(attributesValue, errors);
    }
    protocol::Value* documentURLValue = object->get("documentURL");
    if (documentURLValue) {
        errors->setName("documentURL");
        result->m_documentURL = ValueConversions<String>::fromValue(documentURLValue, errors);
    }
    protocol::Value* baseURLValue = object->get("baseURL");
    if (baseURLValue) {
        errors->setName("baseURL");
        result->m_baseURL = ValueConversions<String>::fromValue(baseURLValue, errors);
    }
    protocol::Value* publicIdValue = object->get("publicId");
    if (publicIdValue) {
        errors->setName("publicId");
        result->m_publicId = ValueConversions<String>::fromValue(publicIdValue, errors);
    }
    protocol::Value* systemIdValue = object->get("systemId");
    if (systemIdValue) {
        errors->setName("systemId");
        result->m_systemId = ValueConversions<String>::fromValue(systemIdValue, errors);
    }
    protocol::Value* xmlVersionValue = object->get("xmlVersion");
    if (xmlVersionValue) {
        errors->setName("xmlVersion");
        result->m_xmlVersion = ValueConversions<String>::fromValue(xmlVersionValue, errors);
    }
    protocol::Value* nameValue = object->get("name");
    if (nameValue) {
        errors->setName("name");
        result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    }
    protocol::Value* valueValue = object->get("value");
    if (valueValue) {
        errors->setName("value");
        result->m_value = ValueConversions<String>::fromValue(valueValue, errors);
    }
    protocol::Value* pseudoTypeValue = object->get("pseudoType");
    if (pseudoTypeValue) {
        errors->setName("pseudoType");
        result->m_pseudoType = ValueConversions<String>::fromValue(pseudoTypeValue, errors);
    }
    protocol::Value* shadowRootTypeValue = object->get("shadowRootType");
    if (shadowRootTypeValue) {
        errors->setName("shadowRootType");
        result->m_shadowRootType = ValueConversions<String>::fromValue(shadowRootTypeValue, errors);
    }
    protocol::Value* frameIdValue = object->get("frameId");
    if (frameIdValue) {
        errors->setName("frameId");
        result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    }
    protocol::Value* contentDocumentValue = object->get("contentDocument");
    if (contentDocumentValue) {
        errors->setName("contentDocument");
        result->m_contentDocument = ValueConversions<protocol::DOM::Node>::fromValue(contentDocumentValue, errors);
    }
    protocol::Value* shadowRootsValue = object->get("shadowRoots");
    if (shadowRootsValue) {
        errors->setName("shadowRoots");
        result->m_shadowRoots = ValueConversions<protocol::Array<protocol::DOM::Node>>::fromValue(shadowRootsValue, errors);
    }
    protocol::Value* templateContentValue = object->get("templateContent");
    if (templateContentValue) {
        errors->setName("templateContent");
        result->m_templateContent = ValueConversions<protocol::DOM::Node>::fromValue(templateContentValue, errors);
    }
    protocol::Value* pseudoElementsValue = object->get("pseudoElements");
    if (pseudoElementsValue) {
        errors->setName("pseudoElements");
        result->m_pseudoElements = ValueConversions<protocol::Array<protocol::DOM::Node>>::fromValue(pseudoElementsValue, errors);
    }
    protocol::Value* roleValue = object->get("role");
    if (roleValue) {
        errors->setName("role");
        result->m_role = ValueConversions<String>::fromValue(roleValue, errors);
    }
    protocol::Value* contentSecurityPolicyHashValue = object->get("contentSecurityPolicyHash");
    if (contentSecurityPolicyHashValue) {
        errors->setName("contentSecurityPolicyHash");
        result->m_contentSecurityPolicyHash = ValueConversions<String>::fromValue(contentSecurityPolicyHashValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Node::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodeId", ValueConversions<int>::toValue(m_nodeId));
    result->setValue("nodeType", ValueConversions<int>::toValue(m_nodeType));
    result->setValue("nodeName", ValueConversions<String>::toValue(m_nodeName));
    result->setValue("localName", ValueConversions<String>::toValue(m_localName));
    result->setValue("nodeValue", ValueConversions<String>::toValue(m_nodeValue));
    if (m_childNodeCount.isJust())
        result->setValue("childNodeCount", ValueConversions<int>::toValue(m_childNodeCount.fromJust()));
    if (m_children.isJust())
        result->setValue("children", ValueConversions<protocol::Array<protocol::DOM::Node>>::toValue(m_children.fromJust()));
    if (m_attributes.isJust())
        result->setValue("attributes", ValueConversions<protocol::Array<String>>::toValue(m_attributes.fromJust()));
    if (m_documentURL.isJust())
        result->setValue("documentURL", ValueConversions<String>::toValue(m_documentURL.fromJust()));
    if (m_baseURL.isJust())
        result->setValue("baseURL", ValueConversions<String>::toValue(m_baseURL.fromJust()));
    if (m_publicId.isJust())
        result->setValue("publicId", ValueConversions<String>::toValue(m_publicId.fromJust()));
    if (m_systemId.isJust())
        result->setValue("systemId", ValueConversions<String>::toValue(m_systemId.fromJust()));
    if (m_xmlVersion.isJust())
        result->setValue("xmlVersion", ValueConversions<String>::toValue(m_xmlVersion.fromJust()));
    if (m_name.isJust())
        result->setValue("name", ValueConversions<String>::toValue(m_name.fromJust()));
    if (m_value.isJust())
        result->setValue("value", ValueConversions<String>::toValue(m_value.fromJust()));
    if (m_pseudoType.isJust())
        result->setValue("pseudoType", ValueConversions<String>::toValue(m_pseudoType.fromJust()));
    if (m_shadowRootType.isJust())
        result->setValue("shadowRootType", ValueConversions<String>::toValue(m_shadowRootType.fromJust()));
    if (m_frameId.isJust())
        result->setValue("frameId", ValueConversions<String>::toValue(m_frameId.fromJust()));
    if (m_contentDocument.isJust())
        result->setValue("contentDocument", ValueConversions<protocol::DOM::Node>::toValue(m_contentDocument.fromJust()));
    if (m_shadowRoots.isJust())
        result->setValue("shadowRoots", ValueConversions<protocol::Array<protocol::DOM::Node>>::toValue(m_shadowRoots.fromJust()));
    if (m_templateContent.isJust())
        result->setValue("templateContent", ValueConversions<protocol::DOM::Node>::toValue(m_templateContent.fromJust()));
    if (m_pseudoElements.isJust())
        result->setValue("pseudoElements", ValueConversions<protocol::Array<protocol::DOM::Node>>::toValue(m_pseudoElements.fromJust()));
    if (m_role.isJust())
        result->setValue("role", ValueConversions<String>::toValue(m_role.fromJust()));
    if (m_contentSecurityPolicyHash.isJust())
        result->setValue("contentSecurityPolicyHash", ValueConversions<String>::toValue(m_contentSecurityPolicyHash.fromJust()));
    return result;
}

std::unique_ptr<Node> Node::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<RGBAColor> RGBAColor::fromValue(protocol::Value* value, ErrorSupport* errors)
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
    result->m_r = ValueConversions<int>::fromValue(rValue, errors);
    protocol::Value* gValue = object->get("g");
    errors->setName("g");
    result->m_g = ValueConversions<int>::fromValue(gValue, errors);
    protocol::Value* bValue = object->get("b");
    errors->setName("b");
    result->m_b = ValueConversions<int>::fromValue(bValue, errors);
    protocol::Value* aValue = object->get("a");
    if (aValue) {
        errors->setName("a");
        result->m_a = ValueConversions<double>::fromValue(aValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RGBAColor::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("r", ValueConversions<int>::toValue(m_r));
    result->setValue("g", ValueConversions<int>::toValue(m_g));
    result->setValue("b", ValueConversions<int>::toValue(m_b));
    if (m_a.isJust())
        result->setValue("a", ValueConversions<double>::toValue(m_a.fromJust()));
    return result;
}

std::unique_ptr<RGBAColor> RGBAColor::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<HighlightConfig> HighlightConfig::fromValue(protocol::Value* value, ErrorSupport* errors)
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
        result->m_showInfo = ValueConversions<bool>::fromValue(showInfoValue, errors);
    }
    protocol::Value* contentColorValue = object->get("contentColor");
    if (contentColorValue) {
        errors->setName("contentColor");
        result->m_contentColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(contentColorValue, errors);
    }
    protocol::Value* paddingColorValue = object->get("paddingColor");
    if (paddingColorValue) {
        errors->setName("paddingColor");
        result->m_paddingColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(paddingColorValue, errors);
    }
    protocol::Value* borderColorValue = object->get("borderColor");
    if (borderColorValue) {
        errors->setName("borderColor");
        result->m_borderColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(borderColorValue, errors);
    }
    protocol::Value* marginColorValue = object->get("marginColor");
    if (marginColorValue) {
        errors->setName("marginColor");
        result->m_marginColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(marginColorValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> HighlightConfig::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_showInfo.isJust())
        result->setValue("showInfo", ValueConversions<bool>::toValue(m_showInfo.fromJust()));
    if (m_contentColor.isJust())
        result->setValue("contentColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_contentColor.fromJust()));
    if (m_paddingColor.isJust())
        result->setValue("paddingColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_paddingColor.fromJust()));
    if (m_borderColor.isJust())
        result->setValue("borderColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_borderColor.fromJust()));
    if (m_marginColor.isJust())
        result->setValue("marginColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_marginColor.fromJust()));
    return result;
}

std::unique_ptr<HighlightConfig> HighlightConfig::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<SetChildNodesNotification> SetChildNodesNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SetChildNodesNotification> result(new SetChildNodesNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* parentIdValue = object->get("parentId");
    errors->setName("parentId");
    result->m_parentId = ValueConversions<int>::fromValue(parentIdValue, errors);
    protocol::Value* nodesValue = object->get("nodes");
    errors->setName("nodes");
    result->m_nodes = ValueConversions<protocol::Array<protocol::DOM::Node>>::fromValue(nodesValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SetChildNodesNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("parentId", ValueConversions<int>::toValue(m_parentId));
    result->setValue("nodes", ValueConversions<protocol::Array<protocol::DOM::Node>>::toValue(m_nodes.get()));
    return result;
}

std::unique_ptr<SetChildNodesNotification> SetChildNodesNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<AttributeModifiedNotification> AttributeModifiedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<AttributeModifiedNotification> result(new AttributeModifiedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::fromValue(valueValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> AttributeModifiedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodeId", ValueConversions<int>::toValue(m_nodeId));
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("value", ValueConversions<String>::toValue(m_value));
    return result;
}

std::unique_ptr<AttributeModifiedNotification> AttributeModifiedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<AttributeRemovedNotification> AttributeRemovedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<AttributeRemovedNotification> result(new AttributeRemovedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> AttributeRemovedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodeId", ValueConversions<int>::toValue(m_nodeId));
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    return result;
}

std::unique_ptr<AttributeRemovedNotification> AttributeRemovedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<InlineStyleInvalidatedNotification> InlineStyleInvalidatedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<InlineStyleInvalidatedNotification> result(new InlineStyleInvalidatedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nodeIdsValue = object->get("nodeIds");
    errors->setName("nodeIds");
    result->m_nodeIds = ValueConversions<protocol::Array<int>>::fromValue(nodeIdsValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> InlineStyleInvalidatedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodeIds", ValueConversions<protocol::Array<int>>::toValue(m_nodeIds.get()));
    return result;
}

std::unique_ptr<InlineStyleInvalidatedNotification> InlineStyleInvalidatedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CharacterDataModifiedNotification> CharacterDataModifiedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CharacterDataModifiedNotification> result(new CharacterDataModifiedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* characterDataValue = object->get("characterData");
    errors->setName("characterData");
    result->m_characterData = ValueConversions<String>::fromValue(characterDataValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CharacterDataModifiedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodeId", ValueConversions<int>::toValue(m_nodeId));
    result->setValue("characterData", ValueConversions<String>::toValue(m_characterData));
    return result;
}

std::unique_ptr<CharacterDataModifiedNotification> CharacterDataModifiedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ChildNodeCountUpdatedNotification> ChildNodeCountUpdatedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ChildNodeCountUpdatedNotification> result(new ChildNodeCountUpdatedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* childNodeCountValue = object->get("childNodeCount");
    errors->setName("childNodeCount");
    result->m_childNodeCount = ValueConversions<int>::fromValue(childNodeCountValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ChildNodeCountUpdatedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("nodeId", ValueConversions<int>::toValue(m_nodeId));
    result->setValue("childNodeCount", ValueConversions<int>::toValue(m_childNodeCount));
    return result;
}

std::unique_ptr<ChildNodeCountUpdatedNotification> ChildNodeCountUpdatedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ChildNodeInsertedNotification> ChildNodeInsertedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ChildNodeInsertedNotification> result(new ChildNodeInsertedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* parentNodeIdValue = object->get("parentNodeId");
    errors->setName("parentNodeId");
    result->m_parentNodeId = ValueConversions<int>::fromValue(parentNodeIdValue, errors);
    protocol::Value* previousNodeIdValue = object->get("previousNodeId");
    errors->setName("previousNodeId");
    result->m_previousNodeId = ValueConversions<int>::fromValue(previousNodeIdValue, errors);
    protocol::Value* nodeValue = object->get("node");
    errors->setName("node");
    result->m_node = ValueConversions<protocol::DOM::Node>::fromValue(nodeValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ChildNodeInsertedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("parentNodeId", ValueConversions<int>::toValue(m_parentNodeId));
    result->setValue("previousNodeId", ValueConversions<int>::toValue(m_previousNodeId));
    result->setValue("node", ValueConversions<protocol::DOM::Node>::toValue(m_node.get()));
    return result;
}

std::unique_ptr<ChildNodeInsertedNotification> ChildNodeInsertedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ChildNodeRemovedNotification> ChildNodeRemovedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ChildNodeRemovedNotification> result(new ChildNodeRemovedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* parentNodeIdValue = object->get("parentNodeId");
    errors->setName("parentNodeId");
    result->m_parentNodeId = ValueConversions<int>::fromValue(parentNodeIdValue, errors);
    protocol::Value* nodeIdValue = object->get("nodeId");
    errors->setName("nodeId");
    result->m_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ChildNodeRemovedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("parentNodeId", ValueConversions<int>::toValue(m_parentNodeId));
    result->setValue("nodeId", ValueConversions<int>::toValue(m_nodeId));
    return result;
}

std::unique_ptr<ChildNodeRemovedNotification> ChildNodeRemovedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ShadowRootPushedNotification> ShadowRootPushedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ShadowRootPushedNotification> result(new ShadowRootPushedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* hostIdValue = object->get("hostId");
    errors->setName("hostId");
    result->m_hostId = ValueConversions<int>::fromValue(hostIdValue, errors);
    protocol::Value* rootValue = object->get("root");
    errors->setName("root");
    result->m_root = ValueConversions<protocol::DOM::Node>::fromValue(rootValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ShadowRootPushedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("hostId", ValueConversions<int>::toValue(m_hostId));
    result->setValue("root", ValueConversions<protocol::DOM::Node>::toValue(m_root.get()));
    return result;
}

std::unique_ptr<ShadowRootPushedNotification> ShadowRootPushedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ShadowRootPoppedNotification> ShadowRootPoppedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ShadowRootPoppedNotification> result(new ShadowRootPoppedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* hostIdValue = object->get("hostId");
    errors->setName("hostId");
    result->m_hostId = ValueConversions<int>::fromValue(hostIdValue, errors);
    protocol::Value* rootIdValue = object->get("rootId");
    errors->setName("rootId");
    result->m_rootId = ValueConversions<int>::fromValue(rootIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ShadowRootPoppedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("hostId", ValueConversions<int>::toValue(m_hostId));
    result->setValue("rootId", ValueConversions<int>::toValue(m_rootId));
    return result;
}

std::unique_ptr<ShadowRootPoppedNotification> ShadowRootPoppedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<PseudoElementAddedNotification> PseudoElementAddedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PseudoElementAddedNotification> result(new PseudoElementAddedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* parentIdValue = object->get("parentId");
    errors->setName("parentId");
    result->m_parentId = ValueConversions<int>::fromValue(parentIdValue, errors);
    protocol::Value* pseudoElementValue = object->get("pseudoElement");
    errors->setName("pseudoElement");
    result->m_pseudoElement = ValueConversions<protocol::DOM::Node>::fromValue(pseudoElementValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PseudoElementAddedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("parentId", ValueConversions<int>::toValue(m_parentId));
    result->setValue("pseudoElement", ValueConversions<protocol::DOM::Node>::toValue(m_pseudoElement.get()));
    return result;
}

std::unique_ptr<PseudoElementAddedNotification> PseudoElementAddedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<PseudoElementRemovedNotification> PseudoElementRemovedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PseudoElementRemovedNotification> result(new PseudoElementRemovedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* parentIdValue = object->get("parentId");
    errors->setName("parentId");
    result->m_parentId = ValueConversions<int>::fromValue(parentIdValue, errors);
    protocol::Value* pseudoElementIdValue = object->get("pseudoElementId");
    errors->setName("pseudoElementId");
    result->m_pseudoElementId = ValueConversions<int>::fromValue(pseudoElementIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PseudoElementRemovedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("parentId", ValueConversions<int>::toValue(m_parentId));
    result->setValue("pseudoElementId", ValueConversions<int>::toValue(m_pseudoElementId));
    return result;
}

std::unique_ptr<PseudoElementRemovedNotification> PseudoElementRemovedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::documentUpdated()
{
    if (!m_frontendChannel)
        return;
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.documentUpdated"));
}

void Frontend::setChildNodes(int parentId, std::unique_ptr<protocol::Array<protocol::DOM::Node>> nodes)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<SetChildNodesNotification> messageData = SetChildNodesNotification::create()
        .setParentId(parentId)
        .setNodes(std::move(nodes))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.setChildNodes", std::move(messageData)));
}

void Frontend::attributeModified(int nodeId, const String& name, const String& value)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<AttributeModifiedNotification> messageData = AttributeModifiedNotification::create()
        .setNodeId(nodeId)
        .setName(name)
        .setValue(value)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.attributeModified", std::move(messageData)));
}

void Frontend::attributeRemoved(int nodeId, const String& name)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<AttributeRemovedNotification> messageData = AttributeRemovedNotification::create()
        .setNodeId(nodeId)
        .setName(name)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.attributeRemoved", std::move(messageData)));
}

void Frontend::inlineStyleInvalidated(std::unique_ptr<protocol::Array<int>> nodeIds)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<InlineStyleInvalidatedNotification> messageData = InlineStyleInvalidatedNotification::create()
        .setNodeIds(std::move(nodeIds))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.inlineStyleInvalidated", std::move(messageData)));
}

void Frontend::characterDataModified(int nodeId, const String& characterData)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<CharacterDataModifiedNotification> messageData = CharacterDataModifiedNotification::create()
        .setNodeId(nodeId)
        .setCharacterData(characterData)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.characterDataModified", std::move(messageData)));
}

void Frontend::childNodeCountUpdated(int nodeId, int childNodeCount)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ChildNodeCountUpdatedNotification> messageData = ChildNodeCountUpdatedNotification::create()
        .setNodeId(nodeId)
        .setChildNodeCount(childNodeCount)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.childNodeCountUpdated", std::move(messageData)));
}

void Frontend::childNodeInserted(int parentNodeId, int previousNodeId, std::unique_ptr<protocol::DOM::Node> node)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ChildNodeInsertedNotification> messageData = ChildNodeInsertedNotification::create()
        .setParentNodeId(parentNodeId)
        .setPreviousNodeId(previousNodeId)
        .setNode(std::move(node))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.childNodeInserted", std::move(messageData)));
}

void Frontend::childNodeRemoved(int parentNodeId, int nodeId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ChildNodeRemovedNotification> messageData = ChildNodeRemovedNotification::create()
        .setParentNodeId(parentNodeId)
        .setNodeId(nodeId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.childNodeRemoved", std::move(messageData)));
}

void Frontend::shadowRootPushed(int hostId, std::unique_ptr<protocol::DOM::Node> root)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ShadowRootPushedNotification> messageData = ShadowRootPushedNotification::create()
        .setHostId(hostId)
        .setRoot(std::move(root))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.shadowRootPushed", std::move(messageData)));
}

void Frontend::shadowRootPopped(int hostId, int rootId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ShadowRootPoppedNotification> messageData = ShadowRootPoppedNotification::create()
        .setHostId(hostId)
        .setRootId(rootId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.shadowRootPopped", std::move(messageData)));
}

void Frontend::pseudoElementAdded(int parentId, std::unique_ptr<protocol::DOM::Node> pseudoElement)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<PseudoElementAddedNotification> messageData = PseudoElementAddedNotification::create()
        .setParentId(parentId)
        .setPseudoElement(std::move(pseudoElement))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.pseudoElementAdded", std::move(messageData)));
}

void Frontend::pseudoElementRemoved(int parentId, int pseudoElementId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<PseudoElementRemovedNotification> messageData = PseudoElementRemovedNotification::create()
        .setParentId(parentId)
        .setPseudoElementId(pseudoElementId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("DOM.pseudoElementRemoved", std::move(messageData)));
}

void Frontend::flush()
{
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification)
{
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
public:
    DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend, bool fallThroughForNotFound)
        : DispatcherBase(frontendChannel)
        , m_backend(backend)
        , m_fallThroughForNotFound(fallThroughForNotFound) {
        m_dispatchMap["DOM.enable"] = &DispatcherImpl::enable;
        m_dispatchMap["DOM.disable"] = &DispatcherImpl::disable;
        m_dispatchMap["DOM.getDocument"] = &DispatcherImpl::getDocument;
        m_dispatchMap["DOM.removeNode"] = &DispatcherImpl::removeNode;
        m_dispatchMap["DOM.setAttributeValue"] = &DispatcherImpl::setAttributeValue;
        m_dispatchMap["DOM.setAttributesAsText"] = &DispatcherImpl::setAttributesAsText;
        m_dispatchMap["DOM.removeAttribute"] = &DispatcherImpl::removeAttribute;
        m_dispatchMap["DOM.performSearch"] = &DispatcherImpl::performSearch;
        m_dispatchMap["DOM.getSearchResults"] = &DispatcherImpl::getSearchResults;
        m_dispatchMap["DOM.discardSearchResults"] = &DispatcherImpl::discardSearchResults;
        m_dispatchMap["DOM.highlightNode"] = &DispatcherImpl::highlightNode;
        m_dispatchMap["DOM.hideHighlight"] = &DispatcherImpl::hideHighlight;
        m_dispatchMap["DOM.resolveNode"] = &DispatcherImpl::resolveNode;
    }
    ~DispatcherImpl() override { }
    DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

protected:
    using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String, CallHandler>;
    DispatchMap m_dispatchMap;

    DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getDocument(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status removeNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setAttributeValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setAttributesAsText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status removeAttribute(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status performSearch(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status discardSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status highlightNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status resolveNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

    Backend* m_backend;
    bool m_fallThroughForNotFound;
};

DispatchResponse::Status DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject)
{
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        if (m_fallThroughForNotFound)
            return DispatchResponse::kFallThrough;
        reportProtocolError(callId, DispatchResponse::kMethodNotFound, "'" + method + "' wasn't found", nullptr);
        return DispatchResponse::kError;
    }

    protocol::ErrorSupport errors;
    return (this->*(it->second))(callId, std::move(messageObject), &errors);
}


DispatchResponse::Status DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getDocument(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Declare output parameters.
    std::unique_ptr<protocol::DOM::Node> out_root;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getDocument(&out_root);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("root", ValueConversions<protocol::DOM::Node>::toValue(out_root.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::removeNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
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

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->removeNode(in_nodeId);
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setAttributeValue(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* nameValue = object ? object->get("name") : nullptr;
    errors->setName("name");
    String in_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* valueValue = object ? object->get("value") : nullptr;
    errors->setName("value");
    String in_value = ValueConversions<String>::fromValue(valueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setAttributeValue(in_nodeId, in_name, in_value);
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setAttributesAsText(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    errors->setName("text");
    String in_text = ValueConversions<String>::fromValue(textValue, errors);
    protocol::Value* nameValue = object ? object->get("name") : nullptr;
    Maybe<String> in_name;
    if (nameValue) {
        errors->setName("name");
        in_name = ValueConversions<String>::fromValue(nameValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setAttributesAsText(in_nodeId, in_text, std::move(in_name));
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::removeAttribute(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* nameValue = object ? object->get("name") : nullptr;
    errors->setName("name");
    String in_name = ValueConversions<String>::fromValue(nameValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->removeAttribute(in_nodeId, in_name);
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::performSearch(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* queryValue = object ? object->get("query") : nullptr;
    errors->setName("query");
    String in_query = ValueConversions<String>::fromValue(queryValue, errors);
    protocol::Value* nodeIdsValue = object ? object->get("nodeIds") : nullptr;
    Maybe<protocol::Array<int>> in_nodeIds;
    if (nodeIdsValue) {
        errors->setName("nodeIds");
        in_nodeIds = ValueConversions<protocol::Array<int>>::fromValue(nodeIdsValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_searchId;
    int out_resultCount;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->performSearch(in_query, std::move(in_nodeIds), &out_searchId, &out_resultCount);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("searchId", ValueConversions<String>::toValue(out_searchId));
        result->setValue("resultCount", ValueConversions<int>::toValue(out_resultCount));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* searchIdValue = object ? object->get("searchId") : nullptr;
    errors->setName("searchId");
    String in_searchId = ValueConversions<String>::fromValue(searchIdValue, errors);
    protocol::Value* fromIndexValue = object ? object->get("fromIndex") : nullptr;
    errors->setName("fromIndex");
    int in_fromIndex = ValueConversions<int>::fromValue(fromIndexValue, errors);
    protocol::Value* toIndexValue = object ? object->get("toIndex") : nullptr;
    errors->setName("toIndex");
    int in_toIndex = ValueConversions<int>::fromValue(toIndexValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<int>> out_nodeIds;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getSearchResults(in_searchId, in_fromIndex, in_toIndex, &out_nodeIds);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("nodeIds", ValueConversions<protocol::Array<int>>::toValue(out_nodeIds.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::discardSearchResults(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* searchIdValue = object ? object->get("searchId") : nullptr;
    errors->setName("searchId");
    String in_searchId = ValueConversions<String>::fromValue(searchIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->discardSearchResults(in_searchId);
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::highlightNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* highlightConfigValue = object ? object->get("highlightConfig") : nullptr;
    errors->setName("highlightConfig");
    std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig = ValueConversions<protocol::DOM::HighlightConfig>::fromValue(highlightConfigValue, errors);
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    Maybe<int> in_nodeId;
    if (nodeIdValue) {
        errors->setName("nodeId");
        in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    }
    protocol::Value* objectIdValue = object ? object->get("objectId") : nullptr;
    Maybe<String> in_objectId;
    if (objectIdValue) {
        errors->setName("objectId");
        in_objectId = ValueConversions<String>::fromValue(objectIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->highlightNode(std::move(in_highlightConfig), std::move(in_nodeId), std::move(in_objectId));
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->hideHighlight();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::resolveNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    errors->setName("nodeId");
    int in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    Maybe<String> in_objectGroup;
    if (objectGroupValue) {
        errors->setName("objectGroup");
        in_objectGroup = ValueConversions<String>::fromValue(objectGroupValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Runtime::RemoteObject> out_object;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->resolveNode(in_nodeId, std::move(in_objectGroup), &out_object);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("object", ValueConversions<protocol::Runtime::RemoteObject>::toValue(out_object.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend)
{
    dispatcher->registerBackend("DOM", std::unique_ptr<protocol::DispatcherBase>(new DispatcherImpl(dispatcher->channel(), backend, dispatcher->fallThroughForNotFound())));
}

} // DOM
} // namespace v8_inspector
} // namespace protocol

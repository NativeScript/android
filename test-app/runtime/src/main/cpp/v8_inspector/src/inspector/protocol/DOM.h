// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_DOM_h
#define v8_inspector_protocol_DOM_h

#include "src/inspector/protocol/Protocol.h"
#include "src/inspector/protocol/Runtime.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.

namespace v8_inspector {
namespace protocol {
namespace DOM {

// ------------- Forward and enum declarations.
using NodeId = int;
using BackendNodeId = int;
using PseudoType = String;
using ShadowRootType = String;
class Node;
class RGBAColor;
class HighlightConfig;
using DocumentUpdatedNotification = Object;
class SetChildNodesNotification;
class AttributeModifiedNotification;
class AttributeRemovedNotification;
class InlineStyleInvalidatedNotification;
class CharacterDataModifiedNotification;
class ChildNodeCountUpdatedNotification;
class ChildNodeInsertedNotification;
class ChildNodeRemovedNotification;
class ShadowRootPushedNotification;
class ShadowRootPoppedNotification;
class PseudoElementAddedNotification;
class PseudoElementRemovedNotification;

namespace PseudoTypeEnum {
 extern const char* Before;
 extern const char* After;
} // namespace PseudoTypeEnum

namespace ShadowRootTypeEnum {
 extern const char* UserAgent;
 extern const char* Open;
 extern const char* Closed;
} // namespace ShadowRootTypeEnum

// ------------- Type and builder declarations.

class  Node : public Serializable{
    PROTOCOL_DISALLOW_COPY(Node);
public:
    static std::unique_ptr<Node> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~Node() override { }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    int getNodeType() { return m_nodeType; }
    void setNodeType(int value) { m_nodeType = value; }

    String getNodeName() { return m_nodeName; }
    void setNodeName(const String& value) { m_nodeName = value; }

    String getLocalName() { return m_localName; }
    void setLocalName(const String& value) { m_localName = value; }

    String getNodeValue() { return m_nodeValue; }
    void setNodeValue(const String& value) { m_nodeValue = value; }

    bool hasChildNodeCount() { return m_childNodeCount.isJust(); }
    int getChildNodeCount(int defaultValue) { return m_childNodeCount.isJust() ? m_childNodeCount.fromJust() : defaultValue; }
    void setChildNodeCount(int value) { m_childNodeCount = value; }

    bool hasChildren() { return m_children.isJust(); }
    protocol::Array<protocol::DOM::Node>* getChildren(protocol::Array<protocol::DOM::Node>* defaultValue) { return m_children.isJust() ? m_children.fromJust() : defaultValue; }
    void setChildren(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value) { m_children = std::move(value); }

    bool hasAttributes() { return m_attributes.isJust(); }
    protocol::Array<String>* getAttributes(protocol::Array<String>* defaultValue) { return m_attributes.isJust() ? m_attributes.fromJust() : defaultValue; }
    void setAttributes(std::unique_ptr<protocol::Array<String>> value) { m_attributes = std::move(value); }

    bool hasDocumentURL() { return m_documentURL.isJust(); }
    String getDocumentURL(const String& defaultValue) { return m_documentURL.isJust() ? m_documentURL.fromJust() : defaultValue; }
    void setDocumentURL(const String& value) { m_documentURL = value; }

    bool hasBaseURL() { return m_baseURL.isJust(); }
    String getBaseURL(const String& defaultValue) { return m_baseURL.isJust() ? m_baseURL.fromJust() : defaultValue; }
    void setBaseURL(const String& value) { m_baseURL = value; }

    bool hasPublicId() { return m_publicId.isJust(); }
    String getPublicId(const String& defaultValue) { return m_publicId.isJust() ? m_publicId.fromJust() : defaultValue; }
    void setPublicId(const String& value) { m_publicId = value; }

    bool hasSystemId() { return m_systemId.isJust(); }
    String getSystemId(const String& defaultValue) { return m_systemId.isJust() ? m_systemId.fromJust() : defaultValue; }
    void setSystemId(const String& value) { m_systemId = value; }

    bool hasXmlVersion() { return m_xmlVersion.isJust(); }
    String getXmlVersion(const String& defaultValue) { return m_xmlVersion.isJust() ? m_xmlVersion.fromJust() : defaultValue; }
    void setXmlVersion(const String& value) { m_xmlVersion = value; }

    bool hasName() { return m_name.isJust(); }
    String getName(const String& defaultValue) { return m_name.isJust() ? m_name.fromJust() : defaultValue; }
    void setName(const String& value) { m_name = value; }

    bool hasValue() { return m_value.isJust(); }
    String getValue(const String& defaultValue) { return m_value.isJust() ? m_value.fromJust() : defaultValue; }
    void setValue(const String& value) { m_value = value; }

    bool hasPseudoType() { return m_pseudoType.isJust(); }
    String getPseudoType(const String& defaultValue) { return m_pseudoType.isJust() ? m_pseudoType.fromJust() : defaultValue; }
    void setPseudoType(const String& value) { m_pseudoType = value; }

    bool hasShadowRootType() { return m_shadowRootType.isJust(); }
    String getShadowRootType(const String& defaultValue) { return m_shadowRootType.isJust() ? m_shadowRootType.fromJust() : defaultValue; }
    void setShadowRootType(const String& value) { m_shadowRootType = value; }

    bool hasFrameId() { return m_frameId.isJust(); }
    String getFrameId(const String& defaultValue) { return m_frameId.isJust() ? m_frameId.fromJust() : defaultValue; }
    void setFrameId(const String& value) { m_frameId = value; }

    bool hasContentDocument() { return m_contentDocument.isJust(); }
    protocol::DOM::Node* getContentDocument(protocol::DOM::Node* defaultValue) { return m_contentDocument.isJust() ? m_contentDocument.fromJust() : defaultValue; }
    void setContentDocument(std::unique_ptr<protocol::DOM::Node> value) { m_contentDocument = std::move(value); }

    bool hasShadowRoots() { return m_shadowRoots.isJust(); }
    protocol::Array<protocol::DOM::Node>* getShadowRoots(protocol::Array<protocol::DOM::Node>* defaultValue) { return m_shadowRoots.isJust() ? m_shadowRoots.fromJust() : defaultValue; }
    void setShadowRoots(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value) { m_shadowRoots = std::move(value); }

    bool hasTemplateContent() { return m_templateContent.isJust(); }
    protocol::DOM::Node* getTemplateContent(protocol::DOM::Node* defaultValue) { return m_templateContent.isJust() ? m_templateContent.fromJust() : defaultValue; }
    void setTemplateContent(std::unique_ptr<protocol::DOM::Node> value) { m_templateContent = std::move(value); }

    bool hasPseudoElements() { return m_pseudoElements.isJust(); }
    protocol::Array<protocol::DOM::Node>* getPseudoElements(protocol::Array<protocol::DOM::Node>* defaultValue) { return m_pseudoElements.isJust() ? m_pseudoElements.fromJust() : defaultValue; }
    void setPseudoElements(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value) { m_pseudoElements = std::move(value); }

    bool hasRole() { return m_role.isJust(); }
    String getRole(const String& defaultValue) { return m_role.isJust() ? m_role.fromJust() : defaultValue; }
    void setRole(const String& value) { m_role = value; }

    bool hasContentSecurityPolicyHash() { return m_contentSecurityPolicyHash.isJust(); }
    String getContentSecurityPolicyHash(const String& defaultValue) { return m_contentSecurityPolicyHash.isJust() ? m_contentSecurityPolicyHash.fromJust() : defaultValue; }
    void setContentSecurityPolicyHash(const String& value) { m_contentSecurityPolicyHash = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<Node> clone() const;

    template<int STATE>
    class NodeBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodeIdSet = 1 << 1,
            NodeTypeSet = 1 << 2,
            NodeNameSet = 1 << 3,
            LocalNameSet = 1 << 4,
            NodeValueSet = 1 << 5,
            AllFieldsSet = (NodeIdSet | NodeTypeSet | NodeNameSet | LocalNameSet | NodeValueSet | 0)};


        NodeBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        NodeBuilder<STATE | NodeTypeSet>& setNodeType(int value)
        {
            static_assert(!(STATE & NodeTypeSet), "property nodeType should not be set yet");
            m_result->setNodeType(value);
            return castState<NodeTypeSet>();
        }

        NodeBuilder<STATE | NodeNameSet>& setNodeName(const String& value)
        {
            static_assert(!(STATE & NodeNameSet), "property nodeName should not be set yet");
            m_result->setNodeName(value);
            return castState<NodeNameSet>();
        }

        NodeBuilder<STATE | LocalNameSet>& setLocalName(const String& value)
        {
            static_assert(!(STATE & LocalNameSet), "property localName should not be set yet");
            m_result->setLocalName(value);
            return castState<LocalNameSet>();
        }

        NodeBuilder<STATE | NodeValueSet>& setNodeValue(const String& value)
        {
            static_assert(!(STATE & NodeValueSet), "property nodeValue should not be set yet");
            m_result->setNodeValue(value);
            return castState<NodeValueSet>();
        }

        NodeBuilder<STATE>& setChildNodeCount(int value)
        {
            m_result->setChildNodeCount(value);
            return *this;
        }

        NodeBuilder<STATE>& setChildren(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value)
        {
            m_result->setChildren(std::move(value));
            return *this;
        }

        NodeBuilder<STATE>& setAttributes(std::unique_ptr<protocol::Array<String>> value)
        {
            m_result->setAttributes(std::move(value));
            return *this;
        }

        NodeBuilder<STATE>& setDocumentURL(const String& value)
        {
            m_result->setDocumentURL(value);
            return *this;
        }

        NodeBuilder<STATE>& setBaseURL(const String& value)
        {
            m_result->setBaseURL(value);
            return *this;
        }

        NodeBuilder<STATE>& setPublicId(const String& value)
        {
            m_result->setPublicId(value);
            return *this;
        }

        NodeBuilder<STATE>& setSystemId(const String& value)
        {
            m_result->setSystemId(value);
            return *this;
        }

        NodeBuilder<STATE>& setXmlVersion(const String& value)
        {
            m_result->setXmlVersion(value);
            return *this;
        }

        NodeBuilder<STATE>& setName(const String& value)
        {
            m_result->setName(value);
            return *this;
        }

        NodeBuilder<STATE>& setValue(const String& value)
        {
            m_result->setValue(value);
            return *this;
        }

        NodeBuilder<STATE>& setPseudoType(const String& value)
        {
            m_result->setPseudoType(value);
            return *this;
        }

        NodeBuilder<STATE>& setShadowRootType(const String& value)
        {
            m_result->setShadowRootType(value);
            return *this;
        }

        NodeBuilder<STATE>& setFrameId(const String& value)
        {
            m_result->setFrameId(value);
            return *this;
        }

        NodeBuilder<STATE>& setContentDocument(std::unique_ptr<protocol::DOM::Node> value)
        {
            m_result->setContentDocument(std::move(value));
            return *this;
        }

        NodeBuilder<STATE>& setShadowRoots(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value)
        {
            m_result->setShadowRoots(std::move(value));
            return *this;
        }

        NodeBuilder<STATE>& setTemplateContent(std::unique_ptr<protocol::DOM::Node> value)
        {
            m_result->setTemplateContent(std::move(value));
            return *this;
        }

        NodeBuilder<STATE>& setPseudoElements(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value)
        {
            m_result->setPseudoElements(std::move(value));
            return *this;
        }

        NodeBuilder<STATE>& setRole(const String& value)
        {
            m_result->setRole(value);
            return *this;
        }

        NodeBuilder<STATE>& setContentSecurityPolicyHash(const String& value)
        {
            m_result->setContentSecurityPolicyHash(value);
            return *this;
        }

        std::unique_ptr<Node> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class Node;
        NodeBuilder() : m_result(new Node()) { }

        template<int STEP> NodeBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<NodeBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::Node> m_result;
    };

    static NodeBuilder<0> create()
    {
        return NodeBuilder<0>();
    }

private:
    Node()
    {
          m_nodeId = 0;
          m_nodeType = 0;
    }

    int m_nodeId;
    int m_nodeType;
    String m_nodeName;
    String m_localName;
    String m_nodeValue;
    Maybe<int> m_childNodeCount;
    Maybe<protocol::Array<protocol::DOM::Node>> m_children;
    Maybe<protocol::Array<String>> m_attributes;
    Maybe<String> m_documentURL;
    Maybe<String> m_baseURL;
    Maybe<String> m_publicId;
    Maybe<String> m_systemId;
    Maybe<String> m_xmlVersion;
    Maybe<String> m_name;
    Maybe<String> m_value;
    Maybe<String> m_pseudoType;
    Maybe<String> m_shadowRootType;
    Maybe<String> m_frameId;
    Maybe<protocol::DOM::Node> m_contentDocument;
    Maybe<protocol::Array<protocol::DOM::Node>> m_shadowRoots;
    Maybe<protocol::DOM::Node> m_templateContent;
    Maybe<protocol::Array<protocol::DOM::Node>> m_pseudoElements;
    Maybe<String> m_role;
    Maybe<String> m_contentSecurityPolicyHash;
};


class  RGBAColor : public Serializable{
    PROTOCOL_DISALLOW_COPY(RGBAColor);
public:
    static std::unique_ptr<RGBAColor> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~RGBAColor() override { }

    int getR() { return m_r; }
    void setR(int value) { m_r = value; }

    int getG() { return m_g; }
    void setG(int value) { m_g = value; }

    int getB() { return m_b; }
    void setB(int value) { m_b = value; }

    bool hasA() { return m_a.isJust(); }
    double getA(double defaultValue) { return m_a.isJust() ? m_a.fromJust() : defaultValue; }
    void setA(double value) { m_a = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<RGBAColor> clone() const;

    template<int STATE>
    class RGBAColorBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            RSet = 1 << 1,
            GSet = 1 << 2,
            BSet = 1 << 3,
            AllFieldsSet = (RSet | GSet | BSet | 0)};


        RGBAColorBuilder<STATE | RSet>& setR(int value)
        {
            static_assert(!(STATE & RSet), "property r should not be set yet");
            m_result->setR(value);
            return castState<RSet>();
        }

        RGBAColorBuilder<STATE | GSet>& setG(int value)
        {
            static_assert(!(STATE & GSet), "property g should not be set yet");
            m_result->setG(value);
            return castState<GSet>();
        }

        RGBAColorBuilder<STATE | BSet>& setB(int value)
        {
            static_assert(!(STATE & BSet), "property b should not be set yet");
            m_result->setB(value);
            return castState<BSet>();
        }

        RGBAColorBuilder<STATE>& setA(double value)
        {
            m_result->setA(value);
            return *this;
        }

        std::unique_ptr<RGBAColor> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class RGBAColor;
        RGBAColorBuilder() : m_result(new RGBAColor()) { }

        template<int STEP> RGBAColorBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<RGBAColorBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::RGBAColor> m_result;
    };

    static RGBAColorBuilder<0> create()
    {
        return RGBAColorBuilder<0>();
    }

private:
    RGBAColor()
    {
          m_r = 0;
          m_g = 0;
          m_b = 0;
    }

    int m_r;
    int m_g;
    int m_b;
    Maybe<double> m_a;
};


class  HighlightConfig : public Serializable{
    PROTOCOL_DISALLOW_COPY(HighlightConfig);
public:
    static std::unique_ptr<HighlightConfig> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~HighlightConfig() override { }

    bool hasShowInfo() { return m_showInfo.isJust(); }
    bool getShowInfo(bool defaultValue) { return m_showInfo.isJust() ? m_showInfo.fromJust() : defaultValue; }
    void setShowInfo(bool value) { m_showInfo = value; }

    bool hasContentColor() { return m_contentColor.isJust(); }
    protocol::DOM::RGBAColor* getContentColor(protocol::DOM::RGBAColor* defaultValue) { return m_contentColor.isJust() ? m_contentColor.fromJust() : defaultValue; }
    void setContentColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_contentColor = std::move(value); }

    bool hasPaddingColor() { return m_paddingColor.isJust(); }
    protocol::DOM::RGBAColor* getPaddingColor(protocol::DOM::RGBAColor* defaultValue) { return m_paddingColor.isJust() ? m_paddingColor.fromJust() : defaultValue; }
    void setPaddingColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_paddingColor = std::move(value); }

    bool hasBorderColor() { return m_borderColor.isJust(); }
    protocol::DOM::RGBAColor* getBorderColor(protocol::DOM::RGBAColor* defaultValue) { return m_borderColor.isJust() ? m_borderColor.fromJust() : defaultValue; }
    void setBorderColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_borderColor = std::move(value); }

    bool hasMarginColor() { return m_marginColor.isJust(); }
    protocol::DOM::RGBAColor* getMarginColor(protocol::DOM::RGBAColor* defaultValue) { return m_marginColor.isJust() ? m_marginColor.fromJust() : defaultValue; }
    void setMarginColor(std::unique_ptr<protocol::DOM::RGBAColor> value) { m_marginColor = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<HighlightConfig> clone() const;

    template<int STATE>
    class HighlightConfigBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            AllFieldsSet = (0)};


        HighlightConfigBuilder<STATE>& setShowInfo(bool value)
        {
            m_result->setShowInfo(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setContentColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setContentColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setPaddingColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setPaddingColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setBorderColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setBorderColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setMarginColor(std::unique_ptr<protocol::DOM::RGBAColor> value)
        {
            m_result->setMarginColor(std::move(value));
            return *this;
        }

        std::unique_ptr<HighlightConfig> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class HighlightConfig;
        HighlightConfigBuilder() : m_result(new HighlightConfig()) { }

        template<int STEP> HighlightConfigBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<HighlightConfigBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::HighlightConfig> m_result;
    };

    static HighlightConfigBuilder<0> create()
    {
        return HighlightConfigBuilder<0>();
    }

private:
    HighlightConfig()
    {
    }

    Maybe<bool> m_showInfo;
    Maybe<protocol::DOM::RGBAColor> m_contentColor;
    Maybe<protocol::DOM::RGBAColor> m_paddingColor;
    Maybe<protocol::DOM::RGBAColor> m_borderColor;
    Maybe<protocol::DOM::RGBAColor> m_marginColor;
};


class  SetChildNodesNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(SetChildNodesNotification);
public:
    static std::unique_ptr<SetChildNodesNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~SetChildNodesNotification() override { }

    int getParentId() { return m_parentId; }
    void setParentId(int value) { m_parentId = value; }

    protocol::Array<protocol::DOM::Node>* getNodes() { return m_nodes.get(); }
    void setNodes(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value) { m_nodes = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<SetChildNodesNotification> clone() const;

    template<int STATE>
    class SetChildNodesNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ParentIdSet = 1 << 1,
            NodesSet = 1 << 2,
            AllFieldsSet = (ParentIdSet | NodesSet | 0)};


        SetChildNodesNotificationBuilder<STATE | ParentIdSet>& setParentId(int value)
        {
            static_assert(!(STATE & ParentIdSet), "property parentId should not be set yet");
            m_result->setParentId(value);
            return castState<ParentIdSet>();
        }

        SetChildNodesNotificationBuilder<STATE | NodesSet>& setNodes(std::unique_ptr<protocol::Array<protocol::DOM::Node>> value)
        {
            static_assert(!(STATE & NodesSet), "property nodes should not be set yet");
            m_result->setNodes(std::move(value));
            return castState<NodesSet>();
        }

        std::unique_ptr<SetChildNodesNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class SetChildNodesNotification;
        SetChildNodesNotificationBuilder() : m_result(new SetChildNodesNotification()) { }

        template<int STEP> SetChildNodesNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<SetChildNodesNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::SetChildNodesNotification> m_result;
    };

    static SetChildNodesNotificationBuilder<0> create()
    {
        return SetChildNodesNotificationBuilder<0>();
    }

private:
    SetChildNodesNotification()
    {
          m_parentId = 0;
    }

    int m_parentId;
    std::unique_ptr<protocol::Array<protocol::DOM::Node>> m_nodes;
};


class  AttributeModifiedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(AttributeModifiedNotification);
public:
    static std::unique_ptr<AttributeModifiedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~AttributeModifiedNotification() override { }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    String getName() { return m_name; }
    void setName(const String& value) { m_name = value; }

    String getValue() { return m_value; }
    void setValue(const String& value) { m_value = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<AttributeModifiedNotification> clone() const;

    template<int STATE>
    class AttributeModifiedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodeIdSet = 1 << 1,
            NameSet = 1 << 2,
            ValueSet = 1 << 3,
            AllFieldsSet = (NodeIdSet | NameSet | ValueSet | 0)};


        AttributeModifiedNotificationBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        AttributeModifiedNotificationBuilder<STATE | NameSet>& setName(const String& value)
        {
            static_assert(!(STATE & NameSet), "property name should not be set yet");
            m_result->setName(value);
            return castState<NameSet>();
        }

        AttributeModifiedNotificationBuilder<STATE | ValueSet>& setValue(const String& value)
        {
            static_assert(!(STATE & ValueSet), "property value should not be set yet");
            m_result->setValue(value);
            return castState<ValueSet>();
        }

        std::unique_ptr<AttributeModifiedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class AttributeModifiedNotification;
        AttributeModifiedNotificationBuilder() : m_result(new AttributeModifiedNotification()) { }

        template<int STEP> AttributeModifiedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<AttributeModifiedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::AttributeModifiedNotification> m_result;
    };

    static AttributeModifiedNotificationBuilder<0> create()
    {
        return AttributeModifiedNotificationBuilder<0>();
    }

private:
    AttributeModifiedNotification()
    {
          m_nodeId = 0;
    }

    int m_nodeId;
    String m_name;
    String m_value;
};


class  AttributeRemovedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(AttributeRemovedNotification);
public:
    static std::unique_ptr<AttributeRemovedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~AttributeRemovedNotification() override { }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    String getName() { return m_name; }
    void setName(const String& value) { m_name = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<AttributeRemovedNotification> clone() const;

    template<int STATE>
    class AttributeRemovedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodeIdSet = 1 << 1,
            NameSet = 1 << 2,
            AllFieldsSet = (NodeIdSet | NameSet | 0)};


        AttributeRemovedNotificationBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        AttributeRemovedNotificationBuilder<STATE | NameSet>& setName(const String& value)
        {
            static_assert(!(STATE & NameSet), "property name should not be set yet");
            m_result->setName(value);
            return castState<NameSet>();
        }

        std::unique_ptr<AttributeRemovedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class AttributeRemovedNotification;
        AttributeRemovedNotificationBuilder() : m_result(new AttributeRemovedNotification()) { }

        template<int STEP> AttributeRemovedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<AttributeRemovedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::AttributeRemovedNotification> m_result;
    };

    static AttributeRemovedNotificationBuilder<0> create()
    {
        return AttributeRemovedNotificationBuilder<0>();
    }

private:
    AttributeRemovedNotification()
    {
          m_nodeId = 0;
    }

    int m_nodeId;
    String m_name;
};


class  InlineStyleInvalidatedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(InlineStyleInvalidatedNotification);
public:
    static std::unique_ptr<InlineStyleInvalidatedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~InlineStyleInvalidatedNotification() override { }

    protocol::Array<int>* getNodeIds() { return m_nodeIds.get(); }
    void setNodeIds(std::unique_ptr<protocol::Array<int>> value) { m_nodeIds = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<InlineStyleInvalidatedNotification> clone() const;

    template<int STATE>
    class InlineStyleInvalidatedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodeIdsSet = 1 << 1,
            AllFieldsSet = (NodeIdsSet | 0)};


        InlineStyleInvalidatedNotificationBuilder<STATE | NodeIdsSet>& setNodeIds(std::unique_ptr<protocol::Array<int>> value)
        {
            static_assert(!(STATE & NodeIdsSet), "property nodeIds should not be set yet");
            m_result->setNodeIds(std::move(value));
            return castState<NodeIdsSet>();
        }

        std::unique_ptr<InlineStyleInvalidatedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class InlineStyleInvalidatedNotification;
        InlineStyleInvalidatedNotificationBuilder() : m_result(new InlineStyleInvalidatedNotification()) { }

        template<int STEP> InlineStyleInvalidatedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<InlineStyleInvalidatedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::InlineStyleInvalidatedNotification> m_result;
    };

    static InlineStyleInvalidatedNotificationBuilder<0> create()
    {
        return InlineStyleInvalidatedNotificationBuilder<0>();
    }

private:
    InlineStyleInvalidatedNotification()
    {
    }

    std::unique_ptr<protocol::Array<int>> m_nodeIds;
};


class  CharacterDataModifiedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(CharacterDataModifiedNotification);
public:
    static std::unique_ptr<CharacterDataModifiedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~CharacterDataModifiedNotification() override { }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    String getCharacterData() { return m_characterData; }
    void setCharacterData(const String& value) { m_characterData = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<CharacterDataModifiedNotification> clone() const;

    template<int STATE>
    class CharacterDataModifiedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodeIdSet = 1 << 1,
            CharacterDataSet = 1 << 2,
            AllFieldsSet = (NodeIdSet | CharacterDataSet | 0)};


        CharacterDataModifiedNotificationBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        CharacterDataModifiedNotificationBuilder<STATE | CharacterDataSet>& setCharacterData(const String& value)
        {
            static_assert(!(STATE & CharacterDataSet), "property characterData should not be set yet");
            m_result->setCharacterData(value);
            return castState<CharacterDataSet>();
        }

        std::unique_ptr<CharacterDataModifiedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class CharacterDataModifiedNotification;
        CharacterDataModifiedNotificationBuilder() : m_result(new CharacterDataModifiedNotification()) { }

        template<int STEP> CharacterDataModifiedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<CharacterDataModifiedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::CharacterDataModifiedNotification> m_result;
    };

    static CharacterDataModifiedNotificationBuilder<0> create()
    {
        return CharacterDataModifiedNotificationBuilder<0>();
    }

private:
    CharacterDataModifiedNotification()
    {
          m_nodeId = 0;
    }

    int m_nodeId;
    String m_characterData;
};


class  ChildNodeCountUpdatedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ChildNodeCountUpdatedNotification);
public:
    static std::unique_ptr<ChildNodeCountUpdatedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ChildNodeCountUpdatedNotification() override { }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    int getChildNodeCount() { return m_childNodeCount; }
    void setChildNodeCount(int value) { m_childNodeCount = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ChildNodeCountUpdatedNotification> clone() const;

    template<int STATE>
    class ChildNodeCountUpdatedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodeIdSet = 1 << 1,
            ChildNodeCountSet = 1 << 2,
            AllFieldsSet = (NodeIdSet | ChildNodeCountSet | 0)};


        ChildNodeCountUpdatedNotificationBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        ChildNodeCountUpdatedNotificationBuilder<STATE | ChildNodeCountSet>& setChildNodeCount(int value)
        {
            static_assert(!(STATE & ChildNodeCountSet), "property childNodeCount should not be set yet");
            m_result->setChildNodeCount(value);
            return castState<ChildNodeCountSet>();
        }

        std::unique_ptr<ChildNodeCountUpdatedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ChildNodeCountUpdatedNotification;
        ChildNodeCountUpdatedNotificationBuilder() : m_result(new ChildNodeCountUpdatedNotification()) { }

        template<int STEP> ChildNodeCountUpdatedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ChildNodeCountUpdatedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::ChildNodeCountUpdatedNotification> m_result;
    };

    static ChildNodeCountUpdatedNotificationBuilder<0> create()
    {
        return ChildNodeCountUpdatedNotificationBuilder<0>();
    }

private:
    ChildNodeCountUpdatedNotification()
    {
          m_nodeId = 0;
          m_childNodeCount = 0;
    }

    int m_nodeId;
    int m_childNodeCount;
};


class  ChildNodeInsertedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ChildNodeInsertedNotification);
public:
    static std::unique_ptr<ChildNodeInsertedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ChildNodeInsertedNotification() override { }

    int getParentNodeId() { return m_parentNodeId; }
    void setParentNodeId(int value) { m_parentNodeId = value; }

    int getPreviousNodeId() { return m_previousNodeId; }
    void setPreviousNodeId(int value) { m_previousNodeId = value; }

    protocol::DOM::Node* getNode() { return m_node.get(); }
    void setNode(std::unique_ptr<protocol::DOM::Node> value) { m_node = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ChildNodeInsertedNotification> clone() const;

    template<int STATE>
    class ChildNodeInsertedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ParentNodeIdSet = 1 << 1,
            PreviousNodeIdSet = 1 << 2,
            NodeSet = 1 << 3,
            AllFieldsSet = (ParentNodeIdSet | PreviousNodeIdSet | NodeSet | 0)};


        ChildNodeInsertedNotificationBuilder<STATE | ParentNodeIdSet>& setParentNodeId(int value)
        {
            static_assert(!(STATE & ParentNodeIdSet), "property parentNodeId should not be set yet");
            m_result->setParentNodeId(value);
            return castState<ParentNodeIdSet>();
        }

        ChildNodeInsertedNotificationBuilder<STATE | PreviousNodeIdSet>& setPreviousNodeId(int value)
        {
            static_assert(!(STATE & PreviousNodeIdSet), "property previousNodeId should not be set yet");
            m_result->setPreviousNodeId(value);
            return castState<PreviousNodeIdSet>();
        }

        ChildNodeInsertedNotificationBuilder<STATE | NodeSet>& setNode(std::unique_ptr<protocol::DOM::Node> value)
        {
            static_assert(!(STATE & NodeSet), "property node should not be set yet");
            m_result->setNode(std::move(value));
            return castState<NodeSet>();
        }

        std::unique_ptr<ChildNodeInsertedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ChildNodeInsertedNotification;
        ChildNodeInsertedNotificationBuilder() : m_result(new ChildNodeInsertedNotification()) { }

        template<int STEP> ChildNodeInsertedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ChildNodeInsertedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::ChildNodeInsertedNotification> m_result;
    };

    static ChildNodeInsertedNotificationBuilder<0> create()
    {
        return ChildNodeInsertedNotificationBuilder<0>();
    }

private:
    ChildNodeInsertedNotification()
    {
          m_parentNodeId = 0;
          m_previousNodeId = 0;
    }

    int m_parentNodeId;
    int m_previousNodeId;
    std::unique_ptr<protocol::DOM::Node> m_node;
};


class  ChildNodeRemovedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ChildNodeRemovedNotification);
public:
    static std::unique_ptr<ChildNodeRemovedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ChildNodeRemovedNotification() override { }

    int getParentNodeId() { return m_parentNodeId; }
    void setParentNodeId(int value) { m_parentNodeId = value; }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ChildNodeRemovedNotification> clone() const;

    template<int STATE>
    class ChildNodeRemovedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ParentNodeIdSet = 1 << 1,
            NodeIdSet = 1 << 2,
            AllFieldsSet = (ParentNodeIdSet | NodeIdSet | 0)};


        ChildNodeRemovedNotificationBuilder<STATE | ParentNodeIdSet>& setParentNodeId(int value)
        {
            static_assert(!(STATE & ParentNodeIdSet), "property parentNodeId should not be set yet");
            m_result->setParentNodeId(value);
            return castState<ParentNodeIdSet>();
        }

        ChildNodeRemovedNotificationBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        std::unique_ptr<ChildNodeRemovedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ChildNodeRemovedNotification;
        ChildNodeRemovedNotificationBuilder() : m_result(new ChildNodeRemovedNotification()) { }

        template<int STEP> ChildNodeRemovedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ChildNodeRemovedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::ChildNodeRemovedNotification> m_result;
    };

    static ChildNodeRemovedNotificationBuilder<0> create()
    {
        return ChildNodeRemovedNotificationBuilder<0>();
    }

private:
    ChildNodeRemovedNotification()
    {
          m_parentNodeId = 0;
          m_nodeId = 0;
    }

    int m_parentNodeId;
    int m_nodeId;
};


class  ShadowRootPushedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ShadowRootPushedNotification);
public:
    static std::unique_ptr<ShadowRootPushedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ShadowRootPushedNotification() override { }

    int getHostId() { return m_hostId; }
    void setHostId(int value) { m_hostId = value; }

    protocol::DOM::Node* getRoot() { return m_root.get(); }
    void setRoot(std::unique_ptr<protocol::DOM::Node> value) { m_root = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ShadowRootPushedNotification> clone() const;

    template<int STATE>
    class ShadowRootPushedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            HostIdSet = 1 << 1,
            RootSet = 1 << 2,
            AllFieldsSet = (HostIdSet | RootSet | 0)};


        ShadowRootPushedNotificationBuilder<STATE | HostIdSet>& setHostId(int value)
        {
            static_assert(!(STATE & HostIdSet), "property hostId should not be set yet");
            m_result->setHostId(value);
            return castState<HostIdSet>();
        }

        ShadowRootPushedNotificationBuilder<STATE | RootSet>& setRoot(std::unique_ptr<protocol::DOM::Node> value)
        {
            static_assert(!(STATE & RootSet), "property root should not be set yet");
            m_result->setRoot(std::move(value));
            return castState<RootSet>();
        }

        std::unique_ptr<ShadowRootPushedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ShadowRootPushedNotification;
        ShadowRootPushedNotificationBuilder() : m_result(new ShadowRootPushedNotification()) { }

        template<int STEP> ShadowRootPushedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ShadowRootPushedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::ShadowRootPushedNotification> m_result;
    };

    static ShadowRootPushedNotificationBuilder<0> create()
    {
        return ShadowRootPushedNotificationBuilder<0>();
    }

private:
    ShadowRootPushedNotification()
    {
          m_hostId = 0;
    }

    int m_hostId;
    std::unique_ptr<protocol::DOM::Node> m_root;
};


class  ShadowRootPoppedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ShadowRootPoppedNotification);
public:
    static std::unique_ptr<ShadowRootPoppedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ShadowRootPoppedNotification() override { }

    int getHostId() { return m_hostId; }
    void setHostId(int value) { m_hostId = value; }

    int getRootId() { return m_rootId; }
    void setRootId(int value) { m_rootId = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ShadowRootPoppedNotification> clone() const;

    template<int STATE>
    class ShadowRootPoppedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            HostIdSet = 1 << 1,
            RootIdSet = 1 << 2,
            AllFieldsSet = (HostIdSet | RootIdSet | 0)};


        ShadowRootPoppedNotificationBuilder<STATE | HostIdSet>& setHostId(int value)
        {
            static_assert(!(STATE & HostIdSet), "property hostId should not be set yet");
            m_result->setHostId(value);
            return castState<HostIdSet>();
        }

        ShadowRootPoppedNotificationBuilder<STATE | RootIdSet>& setRootId(int value)
        {
            static_assert(!(STATE & RootIdSet), "property rootId should not be set yet");
            m_result->setRootId(value);
            return castState<RootIdSet>();
        }

        std::unique_ptr<ShadowRootPoppedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ShadowRootPoppedNotification;
        ShadowRootPoppedNotificationBuilder() : m_result(new ShadowRootPoppedNotification()) { }

        template<int STEP> ShadowRootPoppedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ShadowRootPoppedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::ShadowRootPoppedNotification> m_result;
    };

    static ShadowRootPoppedNotificationBuilder<0> create()
    {
        return ShadowRootPoppedNotificationBuilder<0>();
    }

private:
    ShadowRootPoppedNotification()
    {
          m_hostId = 0;
          m_rootId = 0;
    }

    int m_hostId;
    int m_rootId;
};


class  PseudoElementAddedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(PseudoElementAddedNotification);
public:
    static std::unique_ptr<PseudoElementAddedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~PseudoElementAddedNotification() override { }

    int getParentId() { return m_parentId; }
    void setParentId(int value) { m_parentId = value; }

    protocol::DOM::Node* getPseudoElement() { return m_pseudoElement.get(); }
    void setPseudoElement(std::unique_ptr<protocol::DOM::Node> value) { m_pseudoElement = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<PseudoElementAddedNotification> clone() const;

    template<int STATE>
    class PseudoElementAddedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ParentIdSet = 1 << 1,
            PseudoElementSet = 1 << 2,
            AllFieldsSet = (ParentIdSet | PseudoElementSet | 0)};


        PseudoElementAddedNotificationBuilder<STATE | ParentIdSet>& setParentId(int value)
        {
            static_assert(!(STATE & ParentIdSet), "property parentId should not be set yet");
            m_result->setParentId(value);
            return castState<ParentIdSet>();
        }

        PseudoElementAddedNotificationBuilder<STATE | PseudoElementSet>& setPseudoElement(std::unique_ptr<protocol::DOM::Node> value)
        {
            static_assert(!(STATE & PseudoElementSet), "property pseudoElement should not be set yet");
            m_result->setPseudoElement(std::move(value));
            return castState<PseudoElementSet>();
        }

        std::unique_ptr<PseudoElementAddedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class PseudoElementAddedNotification;
        PseudoElementAddedNotificationBuilder() : m_result(new PseudoElementAddedNotification()) { }

        template<int STEP> PseudoElementAddedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<PseudoElementAddedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::PseudoElementAddedNotification> m_result;
    };

    static PseudoElementAddedNotificationBuilder<0> create()
    {
        return PseudoElementAddedNotificationBuilder<0>();
    }

private:
    PseudoElementAddedNotification()
    {
          m_parentId = 0;
    }

    int m_parentId;
    std::unique_ptr<protocol::DOM::Node> m_pseudoElement;
};


class  PseudoElementRemovedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(PseudoElementRemovedNotification);
public:
    static std::unique_ptr<PseudoElementRemovedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~PseudoElementRemovedNotification() override { }

    int getParentId() { return m_parentId; }
    void setParentId(int value) { m_parentId = value; }

    int getPseudoElementId() { return m_pseudoElementId; }
    void setPseudoElementId(int value) { m_pseudoElementId = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<PseudoElementRemovedNotification> clone() const;

    template<int STATE>
    class PseudoElementRemovedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ParentIdSet = 1 << 1,
            PseudoElementIdSet = 1 << 2,
            AllFieldsSet = (ParentIdSet | PseudoElementIdSet | 0)};


        PseudoElementRemovedNotificationBuilder<STATE | ParentIdSet>& setParentId(int value)
        {
            static_assert(!(STATE & ParentIdSet), "property parentId should not be set yet");
            m_result->setParentId(value);
            return castState<ParentIdSet>();
        }

        PseudoElementRemovedNotificationBuilder<STATE | PseudoElementIdSet>& setPseudoElementId(int value)
        {
            static_assert(!(STATE & PseudoElementIdSet), "property pseudoElementId should not be set yet");
            m_result->setPseudoElementId(value);
            return castState<PseudoElementIdSet>();
        }

        std::unique_ptr<PseudoElementRemovedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class PseudoElementRemovedNotification;
        PseudoElementRemovedNotificationBuilder() : m_result(new PseudoElementRemovedNotification()) { }

        template<int STEP> PseudoElementRemovedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<PseudoElementRemovedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::PseudoElementRemovedNotification> m_result;
    };

    static PseudoElementRemovedNotificationBuilder<0> create()
    {
        return PseudoElementRemovedNotificationBuilder<0>();
    }

private:
    PseudoElementRemovedNotification()
    {
          m_parentId = 0;
          m_pseudoElementId = 0;
    }

    int m_parentId;
    int m_pseudoElementId;
};


// ------------- Backend interface.

class  Backend {
public:
    virtual ~Backend() { }

    virtual DispatchResponse enable() = 0;
    virtual DispatchResponse disable() = 0;
    virtual DispatchResponse getDocument(std::unique_ptr<protocol::DOM::Node>* out_root) = 0;
    virtual DispatchResponse removeNode(int in_nodeId) = 0;
    virtual DispatchResponse setAttributeValue(int in_nodeId, const String& in_name, const String& in_value) = 0;
    virtual DispatchResponse setAttributesAsText(int in_nodeId, const String& in_text, Maybe<String> in_name) = 0;
    virtual DispatchResponse removeAttribute(int in_nodeId, const String& in_name) = 0;
    virtual DispatchResponse performSearch(const String& in_query, Maybe<protocol::Array<int>> in_nodeIds, String* out_searchId, int* out_resultCount) = 0;
    virtual DispatchResponse getSearchResults(const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) = 0;
    virtual DispatchResponse discardSearchResults(const String& in_searchId) = 0;
    virtual DispatchResponse highlightNode(std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, Maybe<int> in_nodeId, Maybe<String> in_objectId) = 0;
    virtual DispatchResponse hideHighlight() = 0;
    virtual DispatchResponse resolveNode(int in_nodeId, Maybe<String> in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) = 0;

};

// ------------- Frontend interface.

class  Frontend {
public:
    explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
    void documentUpdated();
    void setChildNodes(int parentId, std::unique_ptr<protocol::Array<protocol::DOM::Node>> nodes);
    void attributeModified(int nodeId, const String& name, const String& value);
    void attributeRemoved(int nodeId, const String& name);
    void inlineStyleInvalidated(std::unique_ptr<protocol::Array<int>> nodeIds);
    void characterDataModified(int nodeId, const String& characterData);
    void childNodeCountUpdated(int nodeId, int childNodeCount);
    void childNodeInserted(int parentNodeId, int previousNodeId, std::unique_ptr<protocol::DOM::Node> node);
    void childNodeRemoved(int parentNodeId, int nodeId);
    void shadowRootPushed(int hostId, std::unique_ptr<protocol::DOM::Node> root);
    void shadowRootPopped(int hostId, int rootId);
    void pseudoElementAdded(int parentId, std::unique_ptr<protocol::DOM::Node> pseudoElement);
    void pseudoElementRemoved(int parentId, int pseudoElementId);

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

} // namespace DOM
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_DOM_h)

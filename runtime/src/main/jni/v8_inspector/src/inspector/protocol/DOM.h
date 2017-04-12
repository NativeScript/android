// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_DOM_h
#define v8_inspector_protocol_DOM_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.

namespace v8_inspector {
namespace protocol {
namespace DOM {

// ------------- Forward and enum declarations.
// Unique DOM node identifier.
using NodeId = int;
// Unique DOM node identifier used to reference a node that may not have been pushed to the front-end.
using BackendNodeId = int;
// Pseudo element type.
using PseudoType = String;
// Shadow root type.
using ShadowRootType = String;
// Token values of @aria-relevant attribute.
using LiveRegionRelevant = String;
// DOM interaction is implemented in terms of mirror objects that represent the actual DOM nodes. DOMNode is a base node mirror type.
class Node;
// A structure holding event listener properties.
class EventListener;
// A structure holding accessibility properties.
class AccessibilityProperties;
// A structure holding an RGBA color.
class RGBAColor;
// Configuration data for the highlighting of page elements.
class HighlightConfig;

namespace PseudoTypeEnum {
 extern const char* Before;
 extern const char* After;
} // namespace PseudoTypeEnum

namespace ShadowRootTypeEnum {
 extern const char* UserAgent;
 extern const char* Open;
 extern const char* Closed;
} // namespace ShadowRootTypeEnum

namespace LiveRegionRelevantEnum {
 extern const char* Additions;
 extern const char* Removals;
 extern const char* Text;
} // namespace LiveRegionRelevantEnum

// ------------- Type and builder declarations.

// DOM interaction is implemented in terms of mirror objects that represent the actual DOM nodes. DOMNode is a base node mirror type.
class  Node {
    PROTOCOL_DISALLOW_COPY(Node);
public:
    static std::unique_ptr<Node> parse(protocol::Value* value, ErrorSupport* errors);

    ~Node() { }

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

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
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


// A structure holding event listener properties.
class  EventListener {
    PROTOCOL_DISALLOW_COPY(EventListener);
public:
    static std::unique_ptr<EventListener> parse(protocol::Value* value, ErrorSupport* errors);

    ~EventListener() { }

    String getType() { return m_type; }
    void setType(const String& value) { m_type = value; }

    bool getUseCapture() { return m_useCapture; }
    void setUseCapture(bool value) { m_useCapture = value; }

    bool getIsAttribute() { return m_isAttribute; }
    void setIsAttribute(bool value) { m_isAttribute = value; }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    String getHandlerBody() { return m_handlerBody; }
    void setHandlerBody(const String& value) { m_handlerBody = value; }

    bool hasLocation() { return m_location.isJust(); }
    protocol::Debugger::Location* getLocation(protocol::Debugger::Location* defaultValue) { return m_location.isJust() ? m_location.fromJust() : defaultValue; }
    void setLocation(std::unique_ptr<protocol::Debugger::Location> value) { m_location = std::move(value); }

    bool hasSourceName() { return m_sourceName.isJust(); }
    String getSourceName(const String& defaultValue) { return m_sourceName.isJust() ? m_sourceName.fromJust() : defaultValue; }
    void setSourceName(const String& value) { m_sourceName = value; }

    bool hasHandler() { return m_handler.isJust(); }
    protocol::Runtime::RemoteObject* getHandler(protocol::Runtime::RemoteObject* defaultValue) { return m_handler.isJust() ? m_handler.fromJust() : defaultValue; }
    void setHandler(std::unique_ptr<protocol::Runtime::RemoteObject> value) { m_handler = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<EventListener> clone() const;

    template<int STATE>
    class EventListenerBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          TypeSet = 1 << 1,
          UseCaptureSet = 1 << 2,
          IsAttributeSet = 1 << 3,
          NodeIdSet = 1 << 4,
          HandlerBodySet = 1 << 5,
            AllFieldsSet = (TypeSet | UseCaptureSet | IsAttributeSet | NodeIdSet | HandlerBodySet | 0)};


        EventListenerBuilder<STATE | TypeSet>& setType(const String& value)
        {
            static_assert(!(STATE & TypeSet), "property type should not be set yet");
            m_result->setType(value);
            return castState<TypeSet>();
        }

        EventListenerBuilder<STATE | UseCaptureSet>& setUseCapture(bool value)
        {
            static_assert(!(STATE & UseCaptureSet), "property useCapture should not be set yet");
            m_result->setUseCapture(value);
            return castState<UseCaptureSet>();
        }

        EventListenerBuilder<STATE | IsAttributeSet>& setIsAttribute(bool value)
        {
            static_assert(!(STATE & IsAttributeSet), "property isAttribute should not be set yet");
            m_result->setIsAttribute(value);
            return castState<IsAttributeSet>();
        }

        EventListenerBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        EventListenerBuilder<STATE | HandlerBodySet>& setHandlerBody(const String& value)
        {
            static_assert(!(STATE & HandlerBodySet), "property handlerBody should not be set yet");
            m_result->setHandlerBody(value);
            return castState<HandlerBodySet>();
        }

        EventListenerBuilder<STATE>& setLocation(std::unique_ptr<protocol::Debugger::Location> value)
        {
            m_result->setLocation(std::move(value));
            return *this;
        }

        EventListenerBuilder<STATE>& setSourceName(const String& value)
        {
            m_result->setSourceName(value);
            return *this;
        }

        EventListenerBuilder<STATE>& setHandler(std::unique_ptr<protocol::Runtime::RemoteObject> value)
        {
            m_result->setHandler(std::move(value));
            return *this;
        }

        std::unique_ptr<EventListener> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class EventListener;
        EventListenerBuilder() : m_result(new EventListener()) { }

        template<int STEP> EventListenerBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<EventListenerBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::EventListener> m_result;
    };

    static EventListenerBuilder<0> create()
    {
        return EventListenerBuilder<0>();
    }

private:
    EventListener()
    {
          m_useCapture = false;
          m_isAttribute = false;
          m_nodeId = 0;
    }

    String m_type;
    bool m_useCapture;
    bool m_isAttribute;
    int m_nodeId;
    String m_handlerBody;
    Maybe<protocol::Debugger::Location> m_location;
    Maybe<String> m_sourceName;
    Maybe<protocol::Runtime::RemoteObject> m_handler;
};


// A structure holding accessibility properties.
class  AccessibilityProperties {
    PROTOCOL_DISALLOW_COPY(AccessibilityProperties);
public:
    static std::unique_ptr<AccessibilityProperties> parse(protocol::Value* value, ErrorSupport* errors);

    ~AccessibilityProperties() { }

    bool hasActiveDescendantNodeId() { return m_activeDescendantNodeId.isJust(); }
    int getActiveDescendantNodeId(int defaultValue) { return m_activeDescendantNodeId.isJust() ? m_activeDescendantNodeId.fromJust() : defaultValue; }
    void setActiveDescendantNodeId(int value) { m_activeDescendantNodeId = value; }

    bool hasBusy() { return m_busy.isJust(); }
    bool getBusy(bool defaultValue) { return m_busy.isJust() ? m_busy.fromJust() : defaultValue; }
    void setBusy(bool value) { m_busy = value; }

    struct  CheckedEnum {
        static const char* True;
        static const char* False;
        static const char* Mixed;
    }; // CheckedEnum

    bool hasChecked() { return m_checked.isJust(); }
    String getChecked(const String& defaultValue) { return m_checked.isJust() ? m_checked.fromJust() : defaultValue; }
    void setChecked(const String& value) { m_checked = value; }

    bool hasChildNodeIds() { return m_childNodeIds.isJust(); }
    protocol::Array<int>* getChildNodeIds(protocol::Array<int>* defaultValue) { return m_childNodeIds.isJust() ? m_childNodeIds.fromJust() : defaultValue; }
    void setChildNodeIds(std::unique_ptr<protocol::Array<int>> value) { m_childNodeIds = std::move(value); }

    bool hasControlledNodeIds() { return m_controlledNodeIds.isJust(); }
    protocol::Array<int>* getControlledNodeIds(protocol::Array<int>* defaultValue) { return m_controlledNodeIds.isJust() ? m_controlledNodeIds.fromJust() : defaultValue; }
    void setControlledNodeIds(std::unique_ptr<protocol::Array<int>> value) { m_controlledNodeIds = std::move(value); }

    struct  CurrentEnum {
        static const char* True;
        static const char* False;
        static const char* Page;
        static const char* Step;
        static const char* Location;
        static const char* Date;
        static const char* Time;
    }; // CurrentEnum

    bool hasCurrent() { return m_current.isJust(); }
    String getCurrent(const String& defaultValue) { return m_current.isJust() ? m_current.fromJust() : defaultValue; }
    void setCurrent(const String& value) { m_current = value; }

    bool hasDisabled() { return m_disabled.isJust(); }
    bool getDisabled(bool defaultValue) { return m_disabled.isJust() ? m_disabled.fromJust() : defaultValue; }
    void setDisabled(bool value) { m_disabled = value; }

    bool hasHeadingLevel() { return m_headingLevel.isJust(); }
    double getHeadingLevel(double defaultValue) { return m_headingLevel.isJust() ? m_headingLevel.fromJust() : defaultValue; }
    void setHeadingLevel(double value) { m_headingLevel = value; }

    bool hasHierarchyLevel() { return m_hierarchyLevel.isJust(); }
    double getHierarchyLevel(double defaultValue) { return m_hierarchyLevel.isJust() ? m_hierarchyLevel.fromJust() : defaultValue; }
    void setHierarchyLevel(double value) { m_hierarchyLevel = value; }

    bool getExists() { return m_exists; }
    void setExists(bool value) { m_exists = value; }

    bool hasExpanded() { return m_expanded.isJust(); }
    bool getExpanded(bool defaultValue) { return m_expanded.isJust() ? m_expanded.fromJust() : defaultValue; }
    void setExpanded(bool value) { m_expanded = value; }

    bool hasFlowedNodeIds() { return m_flowedNodeIds.isJust(); }
    protocol::Array<int>* getFlowedNodeIds(protocol::Array<int>* defaultValue) { return m_flowedNodeIds.isJust() ? m_flowedNodeIds.fromJust() : defaultValue; }
    void setFlowedNodeIds(std::unique_ptr<protocol::Array<int>> value) { m_flowedNodeIds = std::move(value); }

    bool hasFocused() { return m_focused.isJust(); }
    bool getFocused(bool defaultValue) { return m_focused.isJust() ? m_focused.fromJust() : defaultValue; }
    void setFocused(bool value) { m_focused = value; }

    bool hasIgnored() { return m_ignored.isJust(); }
    bool getIgnored(bool defaultValue) { return m_ignored.isJust() ? m_ignored.fromJust() : defaultValue; }
    void setIgnored(bool value) { m_ignored = value; }

    bool hasIgnoredByDefault() { return m_ignoredByDefault.isJust(); }
    bool getIgnoredByDefault(bool defaultValue) { return m_ignoredByDefault.isJust() ? m_ignoredByDefault.fromJust() : defaultValue; }
    void setIgnoredByDefault(bool value) { m_ignoredByDefault = value; }

    struct  InvalidEnum {
        static const char* True;
        static const char* False;
        static const char* Grammar;
        static const char* Spelling;
    }; // InvalidEnum

    bool hasInvalid() { return m_invalid.isJust(); }
    String getInvalid(const String& defaultValue) { return m_invalid.isJust() ? m_invalid.fromJust() : defaultValue; }
    void setInvalid(const String& value) { m_invalid = value; }

    bool hasHidden() { return m_hidden.isJust(); }
    bool getHidden(bool defaultValue) { return m_hidden.isJust() ? m_hidden.fromJust() : defaultValue; }
    void setHidden(bool value) { m_hidden = value; }

    String getLabel() { return m_label; }
    void setLabel(const String& value) { m_label = value; }

    bool hasLiveRegionAtomic() { return m_liveRegionAtomic.isJust(); }
    bool getLiveRegionAtomic(bool defaultValue) { return m_liveRegionAtomic.isJust() ? m_liveRegionAtomic.fromJust() : defaultValue; }
    void setLiveRegionAtomic(bool value) { m_liveRegionAtomic = value; }

    bool hasLiveRegionRelevant() { return m_liveRegionRelevant.isJust(); }
    protocol::Array<String>* getLiveRegionRelevant(protocol::Array<String>* defaultValue) { return m_liveRegionRelevant.isJust() ? m_liveRegionRelevant.fromJust() : defaultValue; }
    void setLiveRegionRelevant(std::unique_ptr<protocol::Array<String>> value) { m_liveRegionRelevant = std::move(value); }

    struct  LiveRegionStatusEnum {
        static const char* Assertive;
        static const char* Polite;
        static const char* Off;
    }; // LiveRegionStatusEnum

    bool hasLiveRegionStatus() { return m_liveRegionStatus.isJust(); }
    String getLiveRegionStatus(const String& defaultValue) { return m_liveRegionStatus.isJust() ? m_liveRegionStatus.fromJust() : defaultValue; }
    void setLiveRegionStatus(const String& value) { m_liveRegionStatus = value; }

    bool hasMouseEventNodeId() { return m_mouseEventNodeId.isJust(); }
    int getMouseEventNodeId(int defaultValue) { return m_mouseEventNodeId.isJust() ? m_mouseEventNodeId.fromJust() : defaultValue; }
    void setMouseEventNodeId(int value) { m_mouseEventNodeId = value; }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    bool hasOwnedNodeIds() { return m_ownedNodeIds.isJust(); }
    protocol::Array<int>* getOwnedNodeIds(protocol::Array<int>* defaultValue) { return m_ownedNodeIds.isJust() ? m_ownedNodeIds.fromJust() : defaultValue; }
    void setOwnedNodeIds(std::unique_ptr<protocol::Array<int>> value) { m_ownedNodeIds = std::move(value); }

    bool hasParentNodeId() { return m_parentNodeId.isJust(); }
    int getParentNodeId(int defaultValue) { return m_parentNodeId.isJust() ? m_parentNodeId.fromJust() : defaultValue; }
    void setParentNodeId(int value) { m_parentNodeId = value; }

    bool hasPressed() { return m_pressed.isJust(); }
    bool getPressed(bool defaultValue) { return m_pressed.isJust() ? m_pressed.fromJust() : defaultValue; }
    void setPressed(bool value) { m_pressed = value; }

    bool hasReadonly() { return m_readonly.isJust(); }
    bool getReadonly(bool defaultValue) { return m_readonly.isJust() ? m_readonly.fromJust() : defaultValue; }
    void setReadonly(bool value) { m_readonly = value; }

    bool hasRequired() { return m_required.isJust(); }
    bool getRequired(bool defaultValue) { return m_required.isJust() ? m_required.fromJust() : defaultValue; }
    void setRequired(bool value) { m_required = value; }

    String getRole() { return m_role; }
    void setRole(const String& value) { m_role = value; }

    bool hasSelected() { return m_selected.isJust(); }
    bool getSelected(bool defaultValue) { return m_selected.isJust() ? m_selected.fromJust() : defaultValue; }
    void setSelected(bool value) { m_selected = value; }

    bool hasSelectedChildNodeIds() { return m_selectedChildNodeIds.isJust(); }
    protocol::Array<int>* getSelectedChildNodeIds(protocol::Array<int>* defaultValue) { return m_selectedChildNodeIds.isJust() ? m_selectedChildNodeIds.fromJust() : defaultValue; }
    void setSelectedChildNodeIds(std::unique_ptr<protocol::Array<int>> value) { m_selectedChildNodeIds = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<AccessibilityProperties> clone() const;

    template<int STATE>
    class AccessibilityPropertiesBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          ExistsSet = 1 << 1,
          LabelSet = 1 << 2,
          NodeIdSet = 1 << 3,
          RoleSet = 1 << 4,
            AllFieldsSet = (ExistsSet | LabelSet | NodeIdSet | RoleSet | 0)};


        AccessibilityPropertiesBuilder<STATE>& setActiveDescendantNodeId(int value)
        {
            m_result->setActiveDescendantNodeId(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setBusy(bool value)
        {
            m_result->setBusy(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setChecked(const String& value)
        {
            m_result->setChecked(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setChildNodeIds(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setChildNodeIds(std::move(value));
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setControlledNodeIds(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setControlledNodeIds(std::move(value));
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setCurrent(const String& value)
        {
            m_result->setCurrent(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setDisabled(bool value)
        {
            m_result->setDisabled(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setHeadingLevel(double value)
        {
            m_result->setHeadingLevel(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setHierarchyLevel(double value)
        {
            m_result->setHierarchyLevel(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE | ExistsSet>& setExists(bool value)
        {
            static_assert(!(STATE & ExistsSet), "property exists should not be set yet");
            m_result->setExists(value);
            return castState<ExistsSet>();
        }

        AccessibilityPropertiesBuilder<STATE>& setExpanded(bool value)
        {
            m_result->setExpanded(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setFlowedNodeIds(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setFlowedNodeIds(std::move(value));
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setFocused(bool value)
        {
            m_result->setFocused(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setIgnored(bool value)
        {
            m_result->setIgnored(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setIgnoredByDefault(bool value)
        {
            m_result->setIgnoredByDefault(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setInvalid(const String& value)
        {
            m_result->setInvalid(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setHidden(bool value)
        {
            m_result->setHidden(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE | LabelSet>& setLabel(const String& value)
        {
            static_assert(!(STATE & LabelSet), "property label should not be set yet");
            m_result->setLabel(value);
            return castState<LabelSet>();
        }

        AccessibilityPropertiesBuilder<STATE>& setLiveRegionAtomic(bool value)
        {
            m_result->setLiveRegionAtomic(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setLiveRegionRelevant(std::unique_ptr<protocol::Array<String>> value)
        {
            m_result->setLiveRegionRelevant(std::move(value));
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setLiveRegionStatus(const String& value)
        {
            m_result->setLiveRegionStatus(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setMouseEventNodeId(int value)
        {
            m_result->setMouseEventNodeId(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        AccessibilityPropertiesBuilder<STATE>& setOwnedNodeIds(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setOwnedNodeIds(std::move(value));
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setParentNodeId(int value)
        {
            m_result->setParentNodeId(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setPressed(bool value)
        {
            m_result->setPressed(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setReadonly(bool value)
        {
            m_result->setReadonly(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setRequired(bool value)
        {
            m_result->setRequired(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE | RoleSet>& setRole(const String& value)
        {
            static_assert(!(STATE & RoleSet), "property role should not be set yet");
            m_result->setRole(value);
            return castState<RoleSet>();
        }

        AccessibilityPropertiesBuilder<STATE>& setSelected(bool value)
        {
            m_result->setSelected(value);
            return *this;
        }

        AccessibilityPropertiesBuilder<STATE>& setSelectedChildNodeIds(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setSelectedChildNodeIds(std::move(value));
            return *this;
        }

        std::unique_ptr<AccessibilityProperties> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class AccessibilityProperties;
        AccessibilityPropertiesBuilder() : m_result(new AccessibilityProperties()) { }

        template<int STEP> AccessibilityPropertiesBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<AccessibilityPropertiesBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::DOM::AccessibilityProperties> m_result;
    };

    static AccessibilityPropertiesBuilder<0> create()
    {
        return AccessibilityPropertiesBuilder<0>();
    }

private:
    AccessibilityProperties()
    {
          m_exists = false;
          m_nodeId = 0;
    }

    Maybe<int> m_activeDescendantNodeId;
    Maybe<bool> m_busy;
    Maybe<String> m_checked;
    Maybe<protocol::Array<int>> m_childNodeIds;
    Maybe<protocol::Array<int>> m_controlledNodeIds;
    Maybe<String> m_current;
    Maybe<bool> m_disabled;
    Maybe<double> m_headingLevel;
    Maybe<double> m_hierarchyLevel;
    bool m_exists;
    Maybe<bool> m_expanded;
    Maybe<protocol::Array<int>> m_flowedNodeIds;
    Maybe<bool> m_focused;
    Maybe<bool> m_ignored;
    Maybe<bool> m_ignoredByDefault;
    Maybe<String> m_invalid;
    Maybe<bool> m_hidden;
    String m_label;
    Maybe<bool> m_liveRegionAtomic;
    Maybe<protocol::Array<String>> m_liveRegionRelevant;
    Maybe<String> m_liveRegionStatus;
    Maybe<int> m_mouseEventNodeId;
    int m_nodeId;
    Maybe<protocol::Array<int>> m_ownedNodeIds;
    Maybe<int> m_parentNodeId;
    Maybe<bool> m_pressed;
    Maybe<bool> m_readonly;
    Maybe<bool> m_required;
    String m_role;
    Maybe<bool> m_selected;
    Maybe<protocol::Array<int>> m_selectedChildNodeIds;
};


// A structure holding an RGBA color.
class  RGBAColor {
    PROTOCOL_DISALLOW_COPY(RGBAColor);
public:
    static std::unique_ptr<RGBAColor> parse(protocol::Value* value, ErrorSupport* errors);

    ~RGBAColor() { }

    int getR() { return m_r; }
    void setR(int value) { m_r = value; }

    int getG() { return m_g; }
    void setG(int value) { m_g = value; }

    int getB() { return m_b; }
    void setB(int value) { m_b = value; }

    bool hasA() { return m_a.isJust(); }
    double getA(double defaultValue) { return m_a.isJust() ? m_a.fromJust() : defaultValue; }
    void setA(double value) { m_a = value; }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
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


// Configuration data for the highlighting of page elements.
class  HighlightConfig {
    PROTOCOL_DISALLOW_COPY(HighlightConfig);
public:
    static std::unique_ptr<HighlightConfig> parse(protocol::Value* value, ErrorSupport* errors);

    ~HighlightConfig() { }

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

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
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


// ------------- Backend interface.

class  Backend {
public:
    virtual ~Backend() { }

    virtual void getDocument(ErrorString*, std::unique_ptr<protocol::DOM::Node>* out_root) = 0;
    virtual void requestChildNodes(ErrorString*, int in_nodeId, const Maybe<int>& in_depth) = 0;
    virtual void querySelector(ErrorString*, int in_nodeId, const String& in_selector, int* out_nodeId) = 0;
    virtual void querySelectorAll(ErrorString*, int in_nodeId, const String& in_selector, std::unique_ptr<protocol::Array<int>>* out_nodeIds) = 0;
    virtual void setNodeName(ErrorString*, int in_nodeId, const String& in_name, int* out_nodeId) = 0;
    virtual void setNodeValue(ErrorString*, int in_nodeId, const String& in_value) = 0;
    virtual void removeNode(ErrorString*, int in_nodeId) = 0;
    virtual void setAttributeValue(ErrorString*, int in_nodeId, const String& in_name, const String& in_value) = 0;
    virtual void setAttributesAsText(ErrorString*, int in_nodeId, const String& in_text, const Maybe<String>& in_name) = 0;
    virtual void removeAttribute(ErrorString*, int in_nodeId, const String& in_name) = 0;
    virtual void getEventListenersForNode(ErrorString*, int in_nodeId, const Maybe<String>& in_objectGroup, std::unique_ptr<protocol::Array<protocol::DOM::EventListener>>* out_listeners) = 0;
    virtual void getAccessibilityPropertiesForNode(ErrorString*, int in_nodeId, std::unique_ptr<protocol::DOM::AccessibilityProperties>* out_properties) = 0;
    virtual void getOuterHTML(ErrorString*, int in_nodeId, String* out_outerHTML) = 0;
    virtual void setOuterHTML(ErrorString*, int in_nodeId, const String& in_outerHTML) = 0;
    virtual void performSearch(ErrorString*, const String& in_query, const Maybe<protocol::Array<int>>& in_nodeIds, String* out_searchId, int* out_resultCount) = 0;
    virtual void getSearchResults(ErrorString*, const String& in_searchId, int in_fromIndex, int in_toIndex, std::unique_ptr<protocol::Array<int>>* out_nodeIds) = 0;
    virtual void discardSearchResults(ErrorString*, const String& in_searchId) = 0;
    virtual void requestNode(ErrorString*, const String& in_objectId, int* out_nodeId) = 0;
    virtual void setInspectModeEnabled(ErrorString*, bool in_enabled, const Maybe<protocol::DOM::HighlightConfig>& in_highlightConfig) = 0;
    virtual void highlightRect(ErrorString*, int in_x, int in_y, int in_width, int in_height, const Maybe<protocol::DOM::RGBAColor>& in_color, const Maybe<protocol::DOM::RGBAColor>& in_outlineColor, const Maybe<bool>& in_usePageCoordinates) = 0;
    virtual void highlightQuad(ErrorString*, std::unique_ptr<protocol::Array<double>> in_quad, const Maybe<protocol::DOM::RGBAColor>& in_color, const Maybe<protocol::DOM::RGBAColor>& in_outlineColor, const Maybe<bool>& in_usePageCoordinates) = 0;
    virtual void highlightSelector(ErrorString*, std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, const String& in_selectorString, const Maybe<String>& in_frameId) = 0;
    virtual void highlightNode(ErrorString*, std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig, const Maybe<int>& in_nodeId, const Maybe<String>& in_objectId) = 0;
    virtual void hideHighlight(ErrorString*) = 0;
    virtual void highlightFrame(ErrorString*, const String& in_frameId, const Maybe<protocol::DOM::RGBAColor>& in_contentColor, const Maybe<protocol::DOM::RGBAColor>& in_contentOutlineColor) = 0;
    virtual void pushNodeByPathToFrontend(ErrorString*, const String& in_path, int* out_nodeId) = 0;
    virtual void pushNodeByBackendIdToFrontend(ErrorString*, int in_backendNodeId, int* out_nodeId) = 0;
    virtual void releaseBackendNodeIds(ErrorString*, const String& in_nodeGroup) = 0;
    virtual void resolveNode(ErrorString*, int in_nodeId, const Maybe<String>& in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) = 0;
    virtual void getAttributes(ErrorString*, int in_nodeId, std::unique_ptr<protocol::Array<String>>* out_attributes) = 0;
    virtual void moveTo(ErrorString*, int in_nodeId, int in_targetNodeId, const Maybe<int>& in_insertBeforeNodeId, int* out_nodeId) = 0;
    virtual void undo(ErrorString*) = 0;
    virtual void redo(ErrorString*) = 0;
    virtual void markUndoableState(ErrorString*) = 0;
    virtual void focus(ErrorString*, int in_nodeId) = 0;

    virtual void disable(ErrorString*) { }
};

// ------------- Frontend interface.

class  Frontend {
public:
    Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
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

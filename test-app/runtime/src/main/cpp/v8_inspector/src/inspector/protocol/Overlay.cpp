// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Overlay.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Overlay {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Overlay";
const char Metainfo::commandPrefix[] = "Overlay.";
const char Metainfo::version[] = "1.2";

std::unique_ptr<HighlightConfig> HighlightConfig::parse(protocol::Value* value, ErrorSupport* errors) {
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
    protocol::Value* showRulersValue = object->get("showRulers");
    if (showRulersValue) {
        errors->setName("showRulers");
        result->m_showRulers = ValueConversions<bool>::parse(showRulersValue, errors);
    }
    protocol::Value* showExtensionLinesValue = object->get("showExtensionLines");
    if (showExtensionLinesValue) {
        errors->setName("showExtensionLines");
        result->m_showExtensionLines = ValueConversions<bool>::parse(showExtensionLinesValue, errors);
    }
    protocol::Value* displayAsMaterialValue = object->get("displayAsMaterial");
    if (displayAsMaterialValue) {
        errors->setName("displayAsMaterial");
        result->m_displayAsMaterial = ValueConversions<bool>::parse(displayAsMaterialValue, errors);
    }
    protocol::Value* contentColorValue = object->get("contentColor");
    if (contentColorValue) {
        errors->setName("contentColor");
        result->m_contentColor = ValueConversions<protocol::DOM::RGBA>::parse(contentColorValue, errors);
    }
    protocol::Value* paddingColorValue = object->get("paddingColor");
    if (paddingColorValue) {
        errors->setName("paddingColor");
        result->m_paddingColor = ValueConversions<protocol::DOM::RGBA>::parse(paddingColorValue, errors);
    }
    protocol::Value* borderColorValue = object->get("borderColor");
    if (borderColorValue) {
        errors->setName("borderColor");
        result->m_borderColor = ValueConversions<protocol::DOM::RGBA>::parse(borderColorValue, errors);
    }
    protocol::Value* marginColorValue = object->get("marginColor");
    if (marginColorValue) {
        errors->setName("marginColor");
        result->m_marginColor = ValueConversions<protocol::DOM::RGBA>::parse(marginColorValue, errors);
    }
    protocol::Value* eventTargetColorValue = object->get("eventTargetColor");
    if (eventTargetColorValue) {
        errors->setName("eventTargetColor");
        result->m_eventTargetColor = ValueConversions<protocol::DOM::RGBA>::parse(eventTargetColorValue, errors);
    }
    protocol::Value* shapeColorValue = object->get("shapeColor");
    if (shapeColorValue) {
        errors->setName("shapeColor");
        result->m_shapeColor = ValueConversions<protocol::DOM::RGBA>::parse(shapeColorValue, errors);
    }
    protocol::Value* shapeMarginColorValue = object->get("shapeMarginColor");
    if (shapeMarginColorValue) {
        errors->setName("shapeMarginColor");
        result->m_shapeMarginColor = ValueConversions<protocol::DOM::RGBA>::parse(shapeMarginColorValue, errors);
    }
    protocol::Value* selectorListValue = object->get("selectorList");
    if (selectorListValue) {
        errors->setName("selectorList");
        result->m_selectorList = ValueConversions<String>::parse(selectorListValue, errors);
    }
    protocol::Value* cssGridColorValue = object->get("cssGridColor");
    if (cssGridColorValue) {
        errors->setName("cssGridColor");
        result->m_cssGridColor = ValueConversions<protocol::DOM::RGBA>::parse(cssGridColorValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> HighlightConfig::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_showInfo.isJust()) {
        result->setValue("showInfo", ValueConversions<bool>::serialize(m_showInfo.fromJust()));
    }
    if (m_showRulers.isJust()) {
        result->setValue("showRulers", ValueConversions<bool>::serialize(m_showRulers.fromJust()));
    }
    if (m_showExtensionLines.isJust()) {
        result->setValue("showExtensionLines", ValueConversions<bool>::serialize(m_showExtensionLines.fromJust()));
    }
    if (m_displayAsMaterial.isJust()) {
        result->setValue("displayAsMaterial", ValueConversions<bool>::serialize(m_displayAsMaterial.fromJust()));
    }
    if (m_contentColor.isJust()) {
        result->setValue("contentColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_contentColor.fromJust()));
    }
    if (m_paddingColor.isJust()) {
        result->setValue("paddingColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_paddingColor.fromJust()));
    }
    if (m_borderColor.isJust()) {
        result->setValue("borderColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_borderColor.fromJust()));
    }
    if (m_marginColor.isJust()) {
        result->setValue("marginColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_marginColor.fromJust()));
    }
    if (m_eventTargetColor.isJust()) {
        result->setValue("eventTargetColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_eventTargetColor.fromJust()));
    }
    if (m_shapeColor.isJust()) {
        result->setValue("shapeColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_shapeColor.fromJust()));
    }
    if (m_shapeMarginColor.isJust()) {
        result->setValue("shapeMarginColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_shapeMarginColor.fromJust()));
    }
    if (m_selectorList.isJust()) {
        result->setValue("selectorList", ValueConversions<String>::serialize(m_selectorList.fromJust()));
    }
    if (m_cssGridColor.isJust()) {
        result->setValue("cssGridColor", ValueConversions<protocol::DOM::RGBA>::serialize(m_cssGridColor.fromJust()));
    }
    return result;
}

std::unique_ptr<HighlightConfig> HighlightConfig::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::screenshotRequested(std::unique_ptr<protocol::Page::Viewport> viewport) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Overlay.screenshotRequested");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("viewport", ValueConversions<protocol::Page::Viewport>::serialize(viewport.get()));
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
            m_dispatchMap["Overlay.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["Overlay.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["Overlay.setShowFPSCounter"] = &DispatcherImpl::setShowFPSCounter;
            m_dispatchMap["Overlay.setPausedInDebuggerMessage"] = &DispatcherImpl::setPausedInDebuggerMessage;
            m_dispatchMap["Overlay.highlightNode"] = &DispatcherImpl::highlightNode;
            m_dispatchMap["Overlay.highlightFrame"] = &DispatcherImpl::highlightFrame;
            m_dispatchMap["Overlay.hideHighlight"] = &DispatcherImpl::hideHighlight;
            m_dispatchMap["Overlay.getHighlightObjectForTest"] = &DispatcherImpl::getHighlightObjectForTest;
        }
        ~DispatcherImpl() override { }
        void dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

    protected:
        using CallHandler = void (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;

        void enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setShowFPSCounter(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setPausedInDebuggerMessage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void highlightNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void highlightFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getHighlightObjectForTest(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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


void DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->enable(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->disable(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::setShowFPSCounter(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* showValue = object ? object->get("show") : nullptr;
    errors->setName("show");
    bool in_show = ValueConversions<bool>::parse(showValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setShowFPSCounter(&error, in_show);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::setPausedInDebuggerMessage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* messageValue = object ? object->get("message") : nullptr;
    Maybe<String> in_message;
    if (messageValue) {
        errors->setName("message");
        in_message = ValueConversions<String>::parse(messageValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setPausedInDebuggerMessage(&error, in_message);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::highlightNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* highlightConfigValue = object ? object->get("highlightConfig") : nullptr;
    errors->setName("highlightConfig");
    std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig = ValueConversions<protocol::Overlay::HighlightConfig>::parse(highlightConfigValue, errors);
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    Maybe<int> in_nodeId;
    if (nodeIdValue) {
        errors->setName("nodeId");
        in_nodeId = ValueConversions<int>::parse(nodeIdValue, errors);
    }
    protocol::Value* backendNodeIdValue = object ? object->get("backendNodeId") : nullptr;
    Maybe<int> in_backendNodeId;
    if (backendNodeIdValue) {
        errors->setName("backendNodeId");
        in_backendNodeId = ValueConversions<int>::parse(backendNodeIdValue, errors);
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
    m_backend->highlightNode(&error, std::move(in_highlightConfig), in_nodeId, in_backendNodeId, in_objectId);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::highlightFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* contentColorValue = object ? object->get("contentColor") : nullptr;
    Maybe<protocol::DOM::RGBA> in_contentColor;
    if (contentColorValue) {
        errors->setName("contentColor");
        in_contentColor = ValueConversions<protocol::DOM::RGBA>::parse(contentColorValue, errors);
    }
    protocol::Value* contentOutlineColorValue = object ? object->get("contentOutlineColor") : nullptr;
    Maybe<protocol::DOM::RGBA> in_contentOutlineColor;
    if (contentOutlineColorValue) {
        errors->setName("contentOutlineColor");
        in_contentOutlineColor = ValueConversions<protocol::DOM::RGBA>::parse(contentOutlineColorValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->highlightFrame(&error, in_frameId, in_contentColor, in_contentOutlineColor);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->hideHighlight(&error);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::getHighlightObjectForTest(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
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
    std::unique_ptr<protocol::DictionaryValue> out_highlight;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getHighlightObjectForTest(&error, in_nodeId, &out_highlight);
    if (!error.length()) {
        result->setValue("highlight", ValueConversions<protocol::DictionaryValue>::serialize(out_highlight.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("Overlay", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // Overlay
} // namespace v8_inspector
} // namespace protocol

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
const char Metainfo::version[] = "1.3";

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
    protocol::Value* showRulersValue = object->get("showRulers");
    if (showRulersValue) {
        errors->setName("showRulers");
        result->m_showRulers = ValueConversions<bool>::fromValue(showRulersValue, errors);
    }
    protocol::Value* showExtensionLinesValue = object->get("showExtensionLines");
    if (showExtensionLinesValue) {
        errors->setName("showExtensionLines");
        result->m_showExtensionLines = ValueConversions<bool>::fromValue(showExtensionLinesValue, errors);
    }
    protocol::Value* displayAsMaterialValue = object->get("displayAsMaterial");
    if (displayAsMaterialValue) {
        errors->setName("displayAsMaterial");
        result->m_displayAsMaterial = ValueConversions<bool>::fromValue(displayAsMaterialValue, errors);
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
    protocol::Value* eventTargetColorValue = object->get("eventTargetColor");
    if (eventTargetColorValue) {
        errors->setName("eventTargetColor");
        result->m_eventTargetColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(eventTargetColorValue, errors);
    }
    protocol::Value* shapeColorValue = object->get("shapeColor");
    if (shapeColorValue) {
        errors->setName("shapeColor");
        result->m_shapeColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(shapeColorValue, errors);
    }
    protocol::Value* shapeMarginColorValue = object->get("shapeMarginColor");
    if (shapeMarginColorValue) {
        errors->setName("shapeMarginColor");
        result->m_shapeMarginColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(shapeMarginColorValue, errors);
    }
    protocol::Value* selectorListValue = object->get("selectorList");
    if (selectorListValue) {
        errors->setName("selectorList");
        result->m_selectorList = ValueConversions<String>::fromValue(selectorListValue, errors);
    }
    protocol::Value* cssGridColorValue = object->get("cssGridColor");
    if (cssGridColorValue) {
        errors->setName("cssGridColor");
        result->m_cssGridColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(cssGridColorValue, errors);
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
    if (m_showRulers.isJust())
        result->setValue("showRulers", ValueConversions<bool>::toValue(m_showRulers.fromJust()));
    if (m_showExtensionLines.isJust())
        result->setValue("showExtensionLines", ValueConversions<bool>::toValue(m_showExtensionLines.fromJust()));
    if (m_displayAsMaterial.isJust())
        result->setValue("displayAsMaterial", ValueConversions<bool>::toValue(m_displayAsMaterial.fromJust()));
    if (m_contentColor.isJust())
        result->setValue("contentColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_contentColor.fromJust()));
    if (m_paddingColor.isJust())
        result->setValue("paddingColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_paddingColor.fromJust()));
    if (m_borderColor.isJust())
        result->setValue("borderColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_borderColor.fromJust()));
    if (m_marginColor.isJust())
        result->setValue("marginColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_marginColor.fromJust()));
    if (m_eventTargetColor.isJust())
        result->setValue("eventTargetColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_eventTargetColor.fromJust()));
    if (m_shapeColor.isJust())
        result->setValue("shapeColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_shapeColor.fromJust()));
    if (m_shapeMarginColor.isJust())
        result->setValue("shapeMarginColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_shapeMarginColor.fromJust()));
    if (m_selectorList.isJust())
        result->setValue("selectorList", ValueConversions<String>::toValue(m_selectorList.fromJust()));
    if (m_cssGridColor.isJust())
        result->setValue("cssGridColor", ValueConversions<protocol::DOM::RGBAColor>::toValue(m_cssGridColor.fromJust()));
    return result;
}

std::unique_ptr<HighlightConfig> HighlightConfig::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScreenshotRequestedNotification> ScreenshotRequestedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScreenshotRequestedNotification> result(new ScreenshotRequestedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* viewportValue = object->get("viewport");
    errors->setName("viewport");
    result->m_viewport = ValueConversions<protocol::Page::Viewport>::fromValue(viewportValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScreenshotRequestedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("viewport", ValueConversions<protocol::Page::Viewport>::toValue(m_viewport.get()));
    return result;
}

std::unique_ptr<ScreenshotRequestedNotification> ScreenshotRequestedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::screenshotRequested(std::unique_ptr<protocol::Page::Viewport> viewport)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ScreenshotRequestedNotification> messageData = ScreenshotRequestedNotification::create()
        .setViewport(std::move(viewport))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Overlay.screenshotRequested", std::move(messageData)));
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
    DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
    HashMap<String, String>& redirects() { return m_redirects; }

protected:
    using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String, CallHandler>;
    DispatchMap m_dispatchMap;
    HashMap<String, String> m_redirects;

    DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setShowFPSCounter(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setPausedInDebuggerMessage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status highlightNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status highlightFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getHighlightObjectForTest(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setShowFPSCounter(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* showValue = object ? object->get("show") : nullptr;
    errors->setName("show");
    bool in_show = ValueConversions<bool>::fromValue(showValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setShowFPSCounter(in_show);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setPausedInDebuggerMessage(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* messageValue = object ? object->get("message") : nullptr;
    Maybe<String> in_message;
    if (messageValue) {
        errors->setName("message");
        in_message = ValueConversions<String>::fromValue(messageValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setPausedInDebuggerMessage(std::move(in_message));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
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
    std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig = ValueConversions<protocol::Overlay::HighlightConfig>::fromValue(highlightConfigValue, errors);
    protocol::Value* nodeIdValue = object ? object->get("nodeId") : nullptr;
    Maybe<int> in_nodeId;
    if (nodeIdValue) {
        errors->setName("nodeId");
        in_nodeId = ValueConversions<int>::fromValue(nodeIdValue, errors);
    }
    protocol::Value* backendNodeIdValue = object ? object->get("backendNodeId") : nullptr;
    Maybe<int> in_backendNodeId;
    if (backendNodeIdValue) {
        errors->setName("backendNodeId");
        in_backendNodeId = ValueConversions<int>::fromValue(backendNodeIdValue, errors);
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
    DispatchResponse response = m_backend->highlightNode(std::move(in_highlightConfig), std::move(in_nodeId), std::move(in_backendNodeId), std::move(in_objectId));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::highlightFrame(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* contentColorValue = object ? object->get("contentColor") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_contentColor;
    if (contentColorValue) {
        errors->setName("contentColor");
        in_contentColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(contentColorValue, errors);
    }
    protocol::Value* contentOutlineColorValue = object ? object->get("contentOutlineColor") : nullptr;
    Maybe<protocol::DOM::RGBAColor> in_contentOutlineColor;
    if (contentOutlineColorValue) {
        errors->setName("contentOutlineColor");
        in_contentOutlineColor = ValueConversions<protocol::DOM::RGBAColor>::fromValue(contentOutlineColorValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->highlightFrame(in_frameId, std::move(in_contentColor), std::move(in_contentOutlineColor));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::hideHighlight(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->hideHighlight();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getHighlightObjectForTest(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
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
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> out_highlight;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getHighlightObjectForTest(in_nodeId, &out_highlight);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("highlight", ValueConversions<protocol::DictionaryValue>::toValue(out_highlight.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
{
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Overlay", std::move(dispatcher));
}

} // Overlay
} // namespace v8_inspector
} // namespace protocol

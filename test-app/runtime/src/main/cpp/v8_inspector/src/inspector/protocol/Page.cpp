// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Page.h"
#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Page {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Page";
const char Metainfo::commandPrefix[] = "Page.";
const char Metainfo::version[] = "1.3";

namespace ResourceTypeEnum {
const char* Document = "Document";
const char* Stylesheet = "Stylesheet";
const char* Image = "Image";
const char* Font = "Font";
const char* Script = "Script";
const char* XHR = "XHR";
const char* WebSocket = "WebSocket";
const char* Other = "Other";
} // namespace ResourceTypeEnum

std::unique_ptr<Frame> Frame::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Frame> result(new Frame());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<String>::fromValue(idValue, errors);
    protocol::Value* parentIdValue = object->get("parentId");
    if (parentIdValue) {
        errors->setName("parentId");
        result->m_parentId = ValueConversions<String>::fromValue(parentIdValue, errors);
    }
    protocol::Value* loaderIdValue = object->get("loaderId");
    errors->setName("loaderId");
    result->m_loaderId = ValueConversions<String>::fromValue(loaderIdValue, errors);
    protocol::Value* nameValue = object->get("name");
    if (nameValue) {
        errors->setName("name");
        result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    }
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* securityOriginValue = object->get("securityOrigin");
    errors->setName("securityOrigin");
    result->m_securityOrigin = ValueConversions<String>::fromValue(securityOriginValue, errors);
    protocol::Value* mimeTypeValue = object->get("mimeType");
    errors->setName("mimeType");
    result->m_mimeType = ValueConversions<String>::fromValue(mimeTypeValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Frame::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<String>::toValue(m_id));
    if (m_parentId.isJust())
        result->setValue("parentId", ValueConversions<String>::toValue(m_parentId.fromJust()));
    result->setValue("loaderId", ValueConversions<String>::toValue(m_loaderId));
    if (m_name.isJust())
        result->setValue("name", ValueConversions<String>::toValue(m_name.fromJust()));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("securityOrigin", ValueConversions<String>::toValue(m_securityOrigin));
    result->setValue("mimeType", ValueConversions<String>::toValue(m_mimeType));
    return result;
}

std::unique_ptr<Frame> Frame::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameResource> FrameResource::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameResource> result(new FrameResource());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* mimeTypeValue = object->get("mimeType");
    errors->setName("mimeType");
    result->m_mimeType = ValueConversions<String>::fromValue(mimeTypeValue, errors);
    protocol::Value* failedValue = object->get("failed");
    if (failedValue) {
        errors->setName("failed");
        result->m_failed = ValueConversions<bool>::fromValue(failedValue, errors);
    }
    protocol::Value* canceledValue = object->get("canceled");
    if (canceledValue) {
        errors->setName("canceled");
        result->m_canceled = ValueConversions<bool>::fromValue(canceledValue, errors);
    }
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::fromValue(sourceMapURLValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameResource::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    result->setValue("mimeType", ValueConversions<String>::toValue(m_mimeType));
    if (m_failed.isJust())
        result->setValue("failed", ValueConversions<bool>::toValue(m_failed.fromJust()));
    if (m_canceled.isJust())
        result->setValue("canceled", ValueConversions<bool>::toValue(m_canceled.fromJust()));
    if (m_sourceMapURL.isJust())
        result->setValue("sourceMapURL", ValueConversions<String>::toValue(m_sourceMapURL.fromJust()));
    return result;
}

std::unique_ptr<FrameResource> FrameResource::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameResourceTree> FrameResourceTree::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameResourceTree> result(new FrameResourceTree());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameValue = object->get("frame");
    errors->setName("frame");
    result->m_frame = ValueConversions<protocol::Page::Frame>::fromValue(frameValue, errors);
    protocol::Value* childFramesValue = object->get("childFrames");
    if (childFramesValue) {
        errors->setName("childFrames");
        result->m_childFrames = ValueConversions<protocol::Array<protocol::Page::FrameResourceTree>>::fromValue(childFramesValue, errors);
    }
    protocol::Value* resourcesValue = object->get("resources");
    errors->setName("resources");
    result->m_resources = ValueConversions<protocol::Array<protocol::Page::FrameResource>>::fromValue(resourcesValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameResourceTree::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frame", ValueConversions<protocol::Page::Frame>::toValue(m_frame.get()));
    if (m_childFrames.isJust())
        result->setValue("childFrames", ValueConversions<protocol::Array<protocol::Page::FrameResourceTree>>::toValue(m_childFrames.fromJust()));
    result->setValue("resources", ValueConversions<protocol::Array<protocol::Page::FrameResource>>::toValue(m_resources.get()));
    return result;
}

std::unique_ptr<FrameResourceTree> FrameResourceTree::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<SearchResult> SearchResult::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SearchResult> result(new SearchResult());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* matchesCountValue = object->get("matchesCount");
    errors->setName("matchesCount");
    result->m_matchesCount = ValueConversions<double>::fromValue(matchesCountValue, errors);
    protocol::Value* requestIdValue = object->get("requestId");
    if (requestIdValue) {
        errors->setName("requestId");
        result->m_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SearchResult::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("matchesCount", ValueConversions<double>::toValue(m_matchesCount));
    if (m_requestId.isJust())
        result->setValue("requestId", ValueConversions<String>::toValue(m_requestId.fromJust()));
    return result;
}

std::unique_ptr<SearchResult> SearchResult::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<Viewport> Viewport::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Viewport> result(new Viewport());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* xValue = object->get("x");
    errors->setName("x");
    result->m_x = ValueConversions<double>::fromValue(xValue, errors);
    protocol::Value* yValue = object->get("y");
    errors->setName("y");
    result->m_y = ValueConversions<double>::fromValue(yValue, errors);
    protocol::Value* widthValue = object->get("width");
    errors->setName("width");
    result->m_width = ValueConversions<double>::fromValue(widthValue, errors);
    protocol::Value* heightValue = object->get("height");
    errors->setName("height");
    result->m_height = ValueConversions<double>::fromValue(heightValue, errors);
    protocol::Value* scaleValue = object->get("scale");
    errors->setName("scale");
    result->m_scale = ValueConversions<double>::fromValue(scaleValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Viewport::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("x", ValueConversions<double>::toValue(m_x));
    result->setValue("y", ValueConversions<double>::toValue(m_y));
    result->setValue("width", ValueConversions<double>::toValue(m_width));
    result->setValue("height", ValueConversions<double>::toValue(m_height));
    result->setValue("scale", ValueConversions<double>::toValue(m_scale));
    return result;
}

std::unique_ptr<Viewport> Viewport::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<LoadEventFiredNotification> LoadEventFiredNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<LoadEventFiredNotification> result(new LoadEventFiredNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LoadEventFiredNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    return result;
}

std::unique_ptr<LoadEventFiredNotification> LoadEventFiredNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameDetachedNotification> FrameDetachedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameDetachedNotification> result(new FrameDetachedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameDetachedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    return result;
}

std::unique_ptr<FrameDetachedNotification> FrameDetachedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameStartedLoadingNotification> FrameStartedLoadingNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameStartedLoadingNotification> result(new FrameStartedLoadingNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameStartedLoadingNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    return result;
}

std::unique_ptr<FrameStartedLoadingNotification> FrameStartedLoadingNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameStoppedLoadingNotification> FrameStoppedLoadingNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameStoppedLoadingNotification> result(new FrameStoppedLoadingNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameStoppedLoadingNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    return result;
}

std::unique_ptr<FrameStoppedLoadingNotification> FrameStoppedLoadingNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::loadEventFired(double timestamp)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<LoadEventFiredNotification> messageData = LoadEventFiredNotification::create()
        .setTimestamp(timestamp)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.loadEventFired", std::move(messageData)));
}

void Frontend::frameDetached(const String& frameId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<FrameDetachedNotification> messageData = FrameDetachedNotification::create()
        .setFrameId(frameId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameDetached", std::move(messageData)));
}

void Frontend::frameStartedLoading(const String& frameId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<FrameStartedLoadingNotification> messageData = FrameStartedLoadingNotification::create()
        .setFrameId(frameId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameStartedLoading", std::move(messageData)));
}

void Frontend::frameStoppedLoading(const String& frameId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<FrameStoppedLoadingNotification> messageData = FrameStoppedLoadingNotification::create()
        .setFrameId(frameId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameStoppedLoading", std::move(messageData)));
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
        m_dispatchMap["Page.enable"] = &DispatcherImpl::enable;
        m_dispatchMap["Page.disable"] = &DispatcherImpl::disable;
        m_dispatchMap["Page.addScriptToEvaluateOnLoad"] = &DispatcherImpl::addScriptToEvaluateOnLoad;
        m_dispatchMap["Page.removeScriptToEvaluateOnLoad"] = &DispatcherImpl::removeScriptToEvaluateOnLoad;
        m_dispatchMap["Page.reload"] = &DispatcherImpl::reload;
        m_dispatchMap["Page.getResourceTree"] = &DispatcherImpl::getResourceTree;
        m_dispatchMap["Page.getResourceContent"] = &DispatcherImpl::getResourceContent;
        m_dispatchMap["Page.searchInResource"] = &DispatcherImpl::searchInResource;
        m_dispatchMap["Page.searchInResources"] = &DispatcherImpl::searchInResources;
        m_dispatchMap["Page.setDocumentContent"] = &DispatcherImpl::setDocumentContent;
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
    DispatchResponse::Status addScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status removeScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status reload(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getResourceTree(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getResourceContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status searchInResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status searchInResources(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setDocumentContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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

DispatchResponse::Status DispatcherImpl::addScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptSourceValue = object ? object->get("scriptSource") : nullptr;
    errors->setName("scriptSource");
    String in_scriptSource = ValueConversions<String>::fromValue(scriptSourceValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_identifier;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->addScriptToEvaluateOnLoad(in_scriptSource, &out_identifier);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("identifier", ValueConversions<String>::toValue(out_identifier));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::removeScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* identifierValue = object ? object->get("identifier") : nullptr;
    errors->setName("identifier");
    String in_identifier = ValueConversions<String>::fromValue(identifierValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->removeScriptToEvaluateOnLoad(in_identifier);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::reload(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* ignoreCacheValue = object ? object->get("ignoreCache") : nullptr;
    Maybe<bool> in_ignoreCache;
    if (ignoreCacheValue) {
        errors->setName("ignoreCache");
        in_ignoreCache = ValueConversions<bool>::fromValue(ignoreCacheValue, errors);
    }
    protocol::Value* scriptToEvaluateOnLoadValue = object ? object->get("scriptToEvaluateOnLoad") : nullptr;
    Maybe<String> in_scriptToEvaluateOnLoad;
    if (scriptToEvaluateOnLoadValue) {
        errors->setName("scriptToEvaluateOnLoad");
        in_scriptToEvaluateOnLoad = ValueConversions<String>::fromValue(scriptToEvaluateOnLoadValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->reload(std::move(in_ignoreCache), std::move(in_scriptToEvaluateOnLoad));
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getResourceTree(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Declare output parameters.
    std::unique_ptr<protocol::Page::FrameResourceTree> out_frameTree;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getResourceTree(&out_frameTree);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("frameTree", ValueConversions<protocol::Page::FrameResourceTree>::toValue(out_frameTree.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::getResourceContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::fromValue(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    String out_content;
    bool out_base64Encoded;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getResourceContent(in_frameId, in_url, &out_content, &out_base64Encoded);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("content", ValueConversions<String>::toValue(out_content));
        result->setValue("base64Encoded", ValueConversions<bool>::toValue(out_base64Encoded));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::searchInResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* queryValue = object ? object->get("query") : nullptr;
    errors->setName("query");
    String in_query = ValueConversions<String>::fromValue(queryValue, errors);
    protocol::Value* caseSensitiveValue = object ? object->get("caseSensitive") : nullptr;
    Maybe<bool> in_caseSensitive;
    if (caseSensitiveValue) {
        errors->setName("caseSensitive");
        in_caseSensitive = ValueConversions<bool>::fromValue(caseSensitiveValue, errors);
    }
    protocol::Value* isRegexValue = object ? object->get("isRegex") : nullptr;
    Maybe<bool> in_isRegex;
    if (isRegexValue) {
        errors->setName("isRegex");
        in_isRegex = ValueConversions<bool>::fromValue(isRegexValue, errors);
    }
    protocol::Value* requestIdValue = object ? object->get("requestId") : nullptr;
    Maybe<String> in_requestId;
    if (requestIdValue) {
        errors->setName("requestId");
        in_requestId = ValueConversions<String>::fromValue(requestIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::GenericTypes::SearchMatch>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->searchInResource(in_frameId, in_url, in_query, std::move(in_caseSensitive), std::move(in_isRegex), std::move(in_requestId), &out_result);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::GenericTypes::SearchMatch>>::toValue(out_result.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::searchInResources(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    errors->setName("text");
    String in_text = ValueConversions<String>::fromValue(textValue, errors);
    protocol::Value* caseSensitiveValue = object ? object->get("caseSensitive") : nullptr;
    Maybe<bool> in_caseSensitive;
    if (caseSensitiveValue) {
        errors->setName("caseSensitive");
        in_caseSensitive = ValueConversions<bool>::fromValue(caseSensitiveValue, errors);
    }
    protocol::Value* isRegexValue = object ? object->get("isRegex") : nullptr;
    Maybe<bool> in_isRegex;
    if (isRegexValue) {
        errors->setName("isRegex");
        in_isRegex = ValueConversions<bool>::fromValue(isRegexValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Page::SearchResult>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->searchInResources(in_text, std::move(in_caseSensitive), std::move(in_isRegex), &out_result);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::Page::SearchResult>>::toValue(out_result.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::setDocumentContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* htmlValue = object ? object->get("html") : nullptr;
    errors->setName("html");
    String in_html = ValueConversions<String>::fromValue(htmlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setDocumentContent(in_frameId, in_html);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
{
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Page", std::move(dispatcher));
}

} // Page
} // namespace v8_inspector
} // namespace protocol

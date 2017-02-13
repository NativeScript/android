// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Page.h"

#include "src/inspector/protocol/Protocol.h"
#include "GenericTypes.h"

namespace v8_inspector {
namespace protocol {
namespace Page {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Page";
const char Metainfo::commandPrefix[] = "Page.";
const char Metainfo::version[] = "1.2";

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

namespace CoordinateSystemEnum {
const char* Viewport = "Viewport";
const char* Page = "Page";
} // namespace CoordinateSystemEnum

std::unique_ptr<Frame> Frame::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Frame> result(new Frame());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<String>::parse(idValue, errors);
    protocol::Value* parentIdValue = object->get("parentId");
    if (parentIdValue) {
        errors->setName("parentId");
        result->m_parentId = ValueConversions<String>::parse(parentIdValue, errors);
    }
    protocol::Value* loaderIdValue = object->get("loaderId");
    errors->setName("loaderId");
    result->m_loaderId = ValueConversions<String>::parse(loaderIdValue, errors);
    protocol::Value* nameValue = object->get("name");
    if (nameValue) {
        errors->setName("name");
        result->m_name = ValueConversions<String>::parse(nameValue, errors);
    }
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::parse(urlValue, errors);
    protocol::Value* securityOriginValue = object->get("securityOrigin");
    errors->setName("securityOrigin");
    result->m_securityOrigin = ValueConversions<String>::parse(securityOriginValue, errors);
    protocol::Value* mimeTypeValue = object->get("mimeType");
    errors->setName("mimeType");
    result->m_mimeType = ValueConversions<String>::parse(mimeTypeValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Frame::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<String>::serialize(m_id));
    if (m_parentId.isJust()) {
        result->setValue("parentId", ValueConversions<String>::serialize(m_parentId.fromJust()));
    }
    result->setValue("loaderId", ValueConversions<String>::serialize(m_loaderId));
    if (m_name.isJust()) {
        result->setValue("name", ValueConversions<String>::serialize(m_name.fromJust()));
    }
    result->setValue("url", ValueConversions<String>::serialize(m_url));
    result->setValue("securityOrigin", ValueConversions<String>::serialize(m_securityOrigin));
    result->setValue("mimeType", ValueConversions<String>::serialize(m_mimeType));
    return result;
}

std::unique_ptr<Frame> Frame::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<FrameResource> FrameResource::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameResource> result(new FrameResource());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::parse(urlValue, errors);
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::parse(typeValue, errors);
    protocol::Value* mimeTypeValue = object->get("mimeType");
    errors->setName("mimeType");
    result->m_mimeType = ValueConversions<String>::parse(mimeTypeValue, errors);
    protocol::Value* failedValue = object->get("failed");
    if (failedValue) {
        errors->setName("failed");
        result->m_failed = ValueConversions<bool>::parse(failedValue, errors);
    }
    protocol::Value* canceledValue = object->get("canceled");
    if (canceledValue) {
        errors->setName("canceled");
        result->m_canceled = ValueConversions<bool>::parse(canceledValue, errors);
    }
    protocol::Value* sourceMapURLValue = object->get("sourceMapURL");
    if (sourceMapURLValue) {
        errors->setName("sourceMapURL");
        result->m_sourceMapURL = ValueConversions<String>::parse(sourceMapURLValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameResource::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::serialize(m_url));
    result->setValue("type", ValueConversions<String>::serialize(m_type));
    result->setValue("mimeType", ValueConversions<String>::serialize(m_mimeType));
    if (m_failed.isJust()) {
        result->setValue("failed", ValueConversions<bool>::serialize(m_failed.fromJust()));
    }
    if (m_canceled.isJust()) {
        result->setValue("canceled", ValueConversions<bool>::serialize(m_canceled.fromJust()));
    }
    if (m_sourceMapURL.isJust()) {
        result->setValue("sourceMapURL", ValueConversions<String>::serialize(m_sourceMapURL.fromJust()));
    }
    return result;
}

std::unique_ptr<FrameResource> FrameResource::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<FrameResourceTree> FrameResourceTree::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameResourceTree> result(new FrameResourceTree());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameValue = object->get("frame");
    errors->setName("frame");
    result->m_frame = ValueConversions<protocol::Page::Frame>::parse(frameValue, errors);
    protocol::Value* childFramesValue = object->get("childFrames");
    if (childFramesValue) {
        errors->setName("childFrames");
        result->m_childFrames = ValueConversions<protocol::Array<protocol::Page::FrameResourceTree>>::parse(childFramesValue, errors);
    }
    protocol::Value* resourcesValue = object->get("resources");
    errors->setName("resources");
    result->m_resources = ValueConversions<protocol::Array<protocol::Page::FrameResource>>::parse(resourcesValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameResourceTree::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frame", ValueConversions<protocol::Page::Frame>::serialize(m_frame.get()));
    if (m_childFrames.isJust()) {
        result->setValue("childFrames", ValueConversions<protocol::Array<protocol::Page::FrameResourceTree>>::serialize(m_childFrames.fromJust()));
    }
    result->setValue("resources", ValueConversions<protocol::Array<protocol::Page::FrameResource>>::serialize(m_resources.get()));
    return result;
}

std::unique_ptr<FrameResourceTree> FrameResourceTree::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<SearchResult> SearchResult::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<SearchResult> result(new SearchResult());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::parse(urlValue, errors);
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* matchesCountValue = object->get("matchesCount");
    errors->setName("matchesCount");
    result->m_matchesCount = ValueConversions<double>::parse(matchesCountValue, errors);
    protocol::Value* requestIdValue = object->get("requestId");
    if (requestIdValue) {
        errors->setName("requestId");
        result->m_requestId = ValueConversions<String>::parse(requestIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> SearchResult::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::serialize(m_url));
    result->setValue("frameId", ValueConversions<String>::serialize(m_frameId));
    result->setValue("matchesCount", ValueConversions<double>::serialize(m_matchesCount));
    if (m_requestId.isJust()) {
        result->setValue("requestId", ValueConversions<String>::serialize(m_requestId.fromJust()));
    }
    return result;
}

std::unique_ptr<SearchResult> SearchResult::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

std::unique_ptr<Cookie> Cookie::parse(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<Cookie> result(new Cookie());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::parse(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<String>::parse(valueValue, errors);
    protocol::Value* domainValue = object->get("domain");
    errors->setName("domain");
    result->m_domain = ValueConversions<String>::parse(domainValue, errors);
    protocol::Value* pathValue = object->get("path");
    errors->setName("path");
    result->m_path = ValueConversions<String>::parse(pathValue, errors);
    protocol::Value* expiresValue = object->get("expires");
    errors->setName("expires");
    result->m_expires = ValueConversions<double>::parse(expiresValue, errors);
    protocol::Value* sizeValue = object->get("size");
    errors->setName("size");
    result->m_size = ValueConversions<int>::parse(sizeValue, errors);
    protocol::Value* httpOnlyValue = object->get("httpOnly");
    errors->setName("httpOnly");
    result->m_httpOnly = ValueConversions<bool>::parse(httpOnlyValue, errors);
    protocol::Value* secureValue = object->get("secure");
    errors->setName("secure");
    result->m_secure = ValueConversions<bool>::parse(secureValue, errors);
    protocol::Value* sessionValue = object->get("session");
    errors->setName("session");
    result->m_session = ValueConversions<bool>::parse(sessionValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Cookie::serialize() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::serialize(m_name));
    result->setValue("value", ValueConversions<String>::serialize(m_value));
    result->setValue("domain", ValueConversions<String>::serialize(m_domain));
    result->setValue("path", ValueConversions<String>::serialize(m_path));
    result->setValue("expires", ValueConversions<double>::serialize(m_expires));
    result->setValue("size", ValueConversions<int>::serialize(m_size));
    result->setValue("httpOnly", ValueConversions<bool>::serialize(m_httpOnly));
    result->setValue("secure", ValueConversions<bool>::serialize(m_secure));
    result->setValue("session", ValueConversions<bool>::serialize(m_session));
    return result;
}

std::unique_ptr<Cookie> Cookie::clone() const {
    ErrorSupport errors;
    return parse(serialize().get(), &errors);
}

// ------------- Enum values from params.


namespace GetScriptExecutionStatus {
namespace ResultEnum {
const char* Allowed = "allowed";
const char* Disabled = "disabled";
const char* Forbidden = "forbidden";
} // namespace ResultEnum
} // namespace GetScriptExecutionStatus

// ------------- Frontend notifications.

void Frontend::domContentEventFired(double timestamp) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.domContentEventFired");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::loadEventFired(double timestamp) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.loadEventFired");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("timestamp", ValueConversions<double>::serialize(timestamp));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::frameNavigated(std::unique_ptr<protocol::Page::Frame> frame) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.frameNavigated");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("frame", ValueConversions<protocol::Page::Frame>::serialize(frame.get()));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::frameDetached(const String& frameId) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.frameDetached");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::frameStartedLoading(const String& frameId) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.frameStartedLoading");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::frameStoppedLoading(const String& frameId) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.frameStoppedLoading");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::frameScheduledNavigation(const String& frameId, double delay) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.frameScheduledNavigation");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    paramsObject->setValue("delay", ValueConversions<double>::serialize(delay));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::frameClearedScheduledNavigation(const String& frameId) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.frameClearedScheduledNavigation");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("frameId", ValueConversions<String>::serialize(frameId));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::javascriptDialogOpening(const String& message) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.javascriptDialogOpening");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("message", ValueConversions<String>::serialize(message));
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::javascriptDialogClosed() {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.javascriptDialogClosed");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    jsonMessage->setObject("params", std::move(paramsObject));
    if (m_frontendChannel) {
        m_frontendChannel->sendProtocolNotification(jsonMessage->toJSONString());
    }
}

void Frontend::scriptsEnabled(bool isEnabled) {
    std::unique_ptr<protocol::DictionaryValue> jsonMessage = DictionaryValue::create();
    jsonMessage->setString("method", "Page.scriptsEnabled");
    std::unique_ptr<protocol::DictionaryValue> paramsObject = DictionaryValue::create();
    paramsObject->setValue("isEnabled", ValueConversions<bool>::serialize(isEnabled));
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
            m_dispatchMap["Page.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["Page.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["Page.addScriptToEvaluateOnLoad"] = &DispatcherImpl::addScriptToEvaluateOnLoad;
            m_dispatchMap["Page.removeScriptToEvaluateOnLoad"] = &DispatcherImpl::removeScriptToEvaluateOnLoad;
            m_dispatchMap["Page.reload"] = &DispatcherImpl::reload;
            m_dispatchMap["Page.navigate"] = &DispatcherImpl::navigate;
            m_dispatchMap["Page.getCookies"] = &DispatcherImpl::getCookies;
            m_dispatchMap["Page.deleteCookie"] = &DispatcherImpl::deleteCookie;
            m_dispatchMap["Page.getResourceTree"] = &DispatcherImpl::getResourceTree;
            m_dispatchMap["Page.getResourceContent"] = &DispatcherImpl::getResourceContent;
            m_dispatchMap["Page.searchInResource"] = &DispatcherImpl::searchInResource;
            m_dispatchMap["Page.searchInResources"] = &DispatcherImpl::searchInResources;
            m_dispatchMap["Page.setDocumentContent"] = &DispatcherImpl::setDocumentContent;
            m_dispatchMap["Page.setShowPaintRects"] = &DispatcherImpl::setShowPaintRects;
            m_dispatchMap["Page.getScriptExecutionStatus"] = &DispatcherImpl::getScriptExecutionStatus;
            m_dispatchMap["Page.setScriptExecutionDisabled"] = &DispatcherImpl::setScriptExecutionDisabled;
            m_dispatchMap["Page.setTouchEmulationEnabled"] = &DispatcherImpl::setTouchEmulationEnabled;
            m_dispatchMap["Page.setEmulatedMedia"] = &DispatcherImpl::setEmulatedMedia;
            m_dispatchMap["Page.getCompositingBordersVisible"] = &DispatcherImpl::getCompositingBordersVisible;
            m_dispatchMap["Page.setCompositingBordersVisible"] = &DispatcherImpl::setCompositingBordersVisible;
            m_dispatchMap["Page.snapshotNode"] = &DispatcherImpl::snapshotNode;
            m_dispatchMap["Page.snapshotRect"] = &DispatcherImpl::snapshotRect;
            m_dispatchMap["Page.handleJavaScriptDialog"] = &DispatcherImpl::handleJavaScriptDialog;
            m_dispatchMap["Page.archive"] = &DispatcherImpl::archive;
        }
        ~DispatcherImpl() override { }
        void dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;

    protected:
        using CallHandler = void (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;

        void enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void addScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void removeScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void reload(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void navigate(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getCookies(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void deleteCookie(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getResourceTree(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getResourceContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void searchInResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void searchInResources(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setDocumentContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setShowPaintRects(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getScriptExecutionStatus(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setScriptExecutionDisabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setTouchEmulationEnabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setEmulatedMedia(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getCompositingBordersVisible(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setCompositingBordersVisible(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void snapshotNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void snapshotRect(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void handleJavaScriptDialog(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void archive(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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

void DispatcherImpl::addScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptSourceValue = object ? object->get("scriptSource") : nullptr;
    errors->setName("scriptSource");
    String in_scriptSource = ValueConversions<String>::parse(scriptSourceValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_identifier;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->addScriptToEvaluateOnLoad(&error, in_scriptSource, &out_identifier);
    if (!error.length()) {
        result->setValue("identifier", ValueConversions<String>::serialize(out_identifier));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::removeScriptToEvaluateOnLoad(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* identifierValue = object ? object->get("identifier") : nullptr;
    errors->setName("identifier");
    String in_identifier = ValueConversions<String>::parse(identifierValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->removeScriptToEvaluateOnLoad(&error, in_identifier);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::reload(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* ignoreCacheValue = object ? object->get("ignoreCache") : nullptr;
    Maybe<bool> in_ignoreCache;
    if (ignoreCacheValue) {
        errors->setName("ignoreCache");
        in_ignoreCache = ValueConversions<bool>::parse(ignoreCacheValue, errors);
    }
    protocol::Value* scriptToEvaluateOnLoadValue = object ? object->get("scriptToEvaluateOnLoad") : nullptr;
    Maybe<String> in_scriptToEvaluateOnLoad;
    if (scriptToEvaluateOnLoadValue) {
        errors->setName("scriptToEvaluateOnLoad");
        in_scriptToEvaluateOnLoad = ValueConversions<String>::parse(scriptToEvaluateOnLoadValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->reload(&error, in_ignoreCache, in_scriptToEvaluateOnLoad);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::navigate(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::parse(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->navigate(&error, in_url);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::getCookies(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<protocol::Page::Cookie>> out_cookies;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getCookies(&error, &out_cookies);
    if (!error.length()) {
        result->setValue("cookies", ValueConversions<protocol::Array<protocol::Page::Cookie>>::serialize(out_cookies.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::deleteCookie(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* cookieNameValue = object ? object->get("cookieName") : nullptr;
    errors->setName("cookieName");
    String in_cookieName = ValueConversions<String>::parse(cookieNameValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::parse(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->deleteCookie(&error, in_cookieName, in_url);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::getResourceTree(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Page::FrameResourceTree> out_frameTree;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getResourceTree(&error, &out_frameTree);
    if (!error.length()) {
        result->setValue("frameTree", ValueConversions<protocol::Page::FrameResourceTree>::serialize(out_frameTree.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::getResourceContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::parse(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_content;
    bool out_base64Encoded;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getResourceContent(&error, in_frameId, in_url, &out_content, &out_base64Encoded);
    if (!error.length()) {
        result->setValue("content", ValueConversions<String>::serialize(out_content));
        result->setValue("base64Encoded", ValueConversions<bool>::serialize(out_base64Encoded));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::searchInResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::parse(urlValue, errors);
    protocol::Value* queryValue = object ? object->get("query") : nullptr;
    errors->setName("query");
    String in_query = ValueConversions<String>::parse(queryValue, errors);
    protocol::Value* caseSensitiveValue = object ? object->get("caseSensitive") : nullptr;
    Maybe<bool> in_caseSensitive;
    if (caseSensitiveValue) {
        errors->setName("caseSensitive");
        in_caseSensitive = ValueConversions<bool>::parse(caseSensitiveValue, errors);
    }
    protocol::Value* isRegexValue = object ? object->get("isRegex") : nullptr;
    Maybe<bool> in_isRegex;
    if (isRegexValue) {
        errors->setName("isRegex");
        in_isRegex = ValueConversions<bool>::parse(isRegexValue, errors);
    }
    protocol::Value* requestIdValue = object ? object->get("requestId") : nullptr;
    Maybe<String> in_requestId;
    if (requestIdValue) {
        errors->setName("requestId");
        in_requestId = ValueConversions<String>::parse(requestIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<protocol::GenericTypes::SearchMatch>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->searchInResource(&error, in_frameId, in_url, in_query, in_caseSensitive, in_isRegex, in_requestId, &out_result);
    if (!error.length()) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::GenericTypes::SearchMatch>>::serialize(out_result.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::searchInResources(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    errors->setName("text");
    String in_text = ValueConversions<String>::parse(textValue, errors);
    protocol::Value* caseSensitiveValue = object ? object->get("caseSensitive") : nullptr;
    Maybe<bool> in_caseSensitive;
    if (caseSensitiveValue) {
        errors->setName("caseSensitive");
        in_caseSensitive = ValueConversions<bool>::parse(caseSensitiveValue, errors);
    }
    protocol::Value* isRegexValue = object ? object->get("isRegex") : nullptr;
    Maybe<bool> in_isRegex;
    if (isRegexValue) {
        errors->setName("isRegex");
        in_isRegex = ValueConversions<bool>::parse(isRegexValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    std::unique_ptr<protocol::Array<protocol::Page::SearchResult>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->searchInResources(&error, in_text, in_caseSensitive, in_isRegex, &out_result);
    if (!error.length()) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::Page::SearchResult>>::serialize(out_result.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::setDocumentContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::parse(frameIdValue, errors);
    protocol::Value* htmlValue = object ? object->get("html") : nullptr;
    errors->setName("html");
    String in_html = ValueConversions<String>::parse(htmlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setDocumentContent(&error, in_frameId, in_html);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::setShowPaintRects(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* resultValue = object ? object->get("result") : nullptr;
    errors->setName("result");
    bool in_result = ValueConversions<bool>::parse(resultValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setShowPaintRects(&error, in_result);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::getScriptExecutionStatus(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getScriptExecutionStatus(&error, &out_result);
    if (!error.length()) {
        result->setValue("result", ValueConversions<String>::serialize(out_result));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::setScriptExecutionDisabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* valueValue = object ? object->get("value") : nullptr;
    errors->setName("value");
    bool in_value = ValueConversions<bool>::parse(valueValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setScriptExecutionDisabled(&error, in_value);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::setTouchEmulationEnabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* enabledValue = object ? object->get("enabled") : nullptr;
    errors->setName("enabled");
    bool in_enabled = ValueConversions<bool>::parse(enabledValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setTouchEmulationEnabled(&error, in_enabled);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::setEmulatedMedia(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* mediaValue = object ? object->get("media") : nullptr;
    errors->setName("media");
    String in_media = ValueConversions<String>::parse(mediaValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setEmulatedMedia(&error, in_media);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::getCompositingBordersVisible(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    bool out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->getCompositingBordersVisible(&error, &out_result);
    if (!error.length()) {
        result->setValue("result", ValueConversions<bool>::serialize(out_result));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::setCompositingBordersVisible(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* visibleValue = object ? object->get("visible") : nullptr;
    errors->setName("visible");
    bool in_visible = ValueConversions<bool>::parse(visibleValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->setCompositingBordersVisible(&error, in_visible);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::snapshotNode(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
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
    String out_dataURL;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->snapshotNode(&error, in_nodeId, &out_dataURL);
    if (!error.length()) {
        result->setValue("dataURL", ValueConversions<String>::serialize(out_dataURL));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::snapshotRect(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
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
    protocol::Value* coordinateSystemValue = object ? object->get("coordinateSystem") : nullptr;
    errors->setName("coordinateSystem");
    String in_coordinateSystem = ValueConversions<String>::parse(coordinateSystemValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_dataURL;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->snapshotRect(&error, in_x, in_y, in_width, in_height, in_coordinateSystem, &out_dataURL);
    if (!error.length()) {
        result->setValue("dataURL", ValueConversions<String>::serialize(out_dataURL));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

void DispatcherImpl::handleJavaScriptDialog(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* acceptValue = object ? object->get("accept") : nullptr;
    errors->setName("accept");
    bool in_accept = ValueConversions<bool>::parse(acceptValue, errors);
    protocol::Value* promptTextValue = object ? object->get("promptText") : nullptr;
    Maybe<String> in_promptText;
    if (promptTextValue) {
        errors->setName("promptText");
        in_promptText = ValueConversions<String>::parse(promptTextValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, InvalidParams, kInvalidRequest, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->handleJavaScriptDialog(&error, in_accept, in_promptText);
    if (weak->get()) {
        weak->get()->sendResponse(callId, error);
    }
}

void DispatcherImpl::archive(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    String out_data;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    ErrorString error;
    m_backend->archive(&error, &out_data);
    if (!error.length()) {
        result->setValue("data", ValueConversions<String>::serialize(out_data));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, error, std::move(result));
    }
}

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("Page", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // Page
} // namespace v8_inspector
} // namespace protocol

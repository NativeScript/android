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

// ------------- Enum values from params.


// ------------- Frontend notifications.

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
            m_dispatchMap["Page.getResourceTree"] = &DispatcherImpl::getResourceTree;
            m_dispatchMap["Page.getResourceContent"] = &DispatcherImpl::getResourceContent;
            m_dispatchMap["Page.searchInResource"] = &DispatcherImpl::searchInResource;
            m_dispatchMap["Page.searchInResources"] = &DispatcherImpl::searchInResources;
            m_dispatchMap["Page.setDocumentContent"] = &DispatcherImpl::setDocumentContent;
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
        void getResourceTree(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getResourceContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void searchInResource(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void searchInResources(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setDocumentContent(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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

// static
void Dispatcher::wire(UberDispatcher* dispatcher, Backend* backend) {
    dispatcher->registerBackend("Page", wrapUnique(new DispatcherImpl(dispatcher->channel(), backend)));
}

} // Page
} // namespace v8_inspector
} // namespace protocol

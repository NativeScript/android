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
const char Document[] = "Document";
const char Stylesheet[] = "Stylesheet";
const char Image[] = "Image";
const char Media[] = "Media";
const char Font[] = "Font";
const char Script[] = "Script";
const char TextTrack[] = "TextTrack";
const char XHR[] = "XHR";
const char Fetch[] = "Fetch";
const char EventSource[] = "EventSource";
const char WebSocket[] = "WebSocket";
const char Manifest[] = "Manifest";
const char Other[] = "Other";
} // namespace ResourceTypeEnum

std::unique_ptr<Frame> Frame::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    protocol::Value* unreachableUrlValue = object->get("unreachableUrl");
    if (unreachableUrlValue) {
        errors->setName("unreachableUrl");
        result->m_unreachableUrl = ValueConversions<String>::fromValue(unreachableUrlValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Frame::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<String>::toValue(m_id));
    if (m_parentId.isJust()) {
        result->setValue("parentId", ValueConversions<String>::toValue(m_parentId.fromJust()));
    }
    result->setValue("loaderId", ValueConversions<String>::toValue(m_loaderId));
    if (m_name.isJust()) {
        result->setValue("name", ValueConversions<String>::toValue(m_name.fromJust()));
    }
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("securityOrigin", ValueConversions<String>::toValue(m_securityOrigin));
    result->setValue("mimeType", ValueConversions<String>::toValue(m_mimeType));
    if (m_unreachableUrl.isJust()) {
        result->setValue("unreachableUrl", ValueConversions<String>::toValue(m_unreachableUrl.fromJust()));
    }
    return result;
}

std::unique_ptr<Frame> Frame::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameResource> FrameResource::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    protocol::Value* lastModifiedValue = object->get("lastModified");
    if (lastModifiedValue) {
        errors->setName("lastModified");
        result->m_lastModified = ValueConversions<double>::fromValue(lastModifiedValue, errors);
    }
    protocol::Value* contentSizeValue = object->get("contentSize");
    if (contentSizeValue) {
        errors->setName("contentSize");
        result->m_contentSize = ValueConversions<double>::fromValue(contentSizeValue, errors);
    }
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
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameResource::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    result->setValue("mimeType", ValueConversions<String>::toValue(m_mimeType));
    if (m_lastModified.isJust()) {
        result->setValue("lastModified", ValueConversions<double>::toValue(m_lastModified.fromJust()));
    }
    if (m_contentSize.isJust()) {
        result->setValue("contentSize", ValueConversions<double>::toValue(m_contentSize.fromJust()));
    }
    if (m_failed.isJust()) {
        result->setValue("failed", ValueConversions<bool>::toValue(m_failed.fromJust()));
    }
    if (m_canceled.isJust()) {
        result->setValue("canceled", ValueConversions<bool>::toValue(m_canceled.fromJust()));
    }
    return result;
}

std::unique_ptr<FrameResource> FrameResource::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameResourceTree> FrameResourceTree::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameResourceTree::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frame", ValueConversions<protocol::Page::Frame>::toValue(m_frame.get()));
    if (m_childFrames.isJust()) {
        result->setValue("childFrames", ValueConversions<protocol::Array<protocol::Page::FrameResourceTree>>::toValue(m_childFrames.fromJust()));
    }
    result->setValue("resources", ValueConversions<protocol::Array<protocol::Page::FrameResource>>::toValue(m_resources.get()));
    return result;
}

std::unique_ptr<FrameResourceTree> FrameResourceTree::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameTree> FrameTree::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameTree> result(new FrameTree());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameValue = object->get("frame");
    errors->setName("frame");
    result->m_frame = ValueConversions<protocol::Page::Frame>::fromValue(frameValue, errors);
    protocol::Value* childFramesValue = object->get("childFrames");
    if (childFramesValue) {
        errors->setName("childFrames");
        result->m_childFrames = ValueConversions<protocol::Array<protocol::Page::FrameTree>>::fromValue(childFramesValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameTree::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frame", ValueConversions<protocol::Page::Frame>::toValue(m_frame.get()));
    if (m_childFrames.isJust()) {
        result->setValue("childFrames", ValueConversions<protocol::Array<protocol::Page::FrameTree>>::toValue(m_childFrames.fromJust()));
    }
    return result;
}

std::unique_ptr<FrameTree> FrameTree::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

namespace TransitionTypeEnum {
const char Link[] = "link";
const char Typed[] = "typed";
const char Auto_bookmark[] = "auto_bookmark";
const char Auto_subframe[] = "auto_subframe";
const char Manual_subframe[] = "manual_subframe";
const char Generated[] = "generated";
const char Auto_toplevel[] = "auto_toplevel";
const char Form_submit[] = "form_submit";
const char Reload[] = "reload";
const char Keyword[] = "keyword";
const char Keyword_generated[] = "keyword_generated";
const char Other[] = "other";
} // namespace TransitionTypeEnum

std::unique_ptr<NavigationEntry> NavigationEntry::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<NavigationEntry> result(new NavigationEntry());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<int>::fromValue(idValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* userTypedURLValue = object->get("userTypedURL");
    errors->setName("userTypedURL");
    result->m_userTypedURL = ValueConversions<String>::fromValue(userTypedURLValue, errors);
    protocol::Value* titleValue = object->get("title");
    errors->setName("title");
    result->m_title = ValueConversions<String>::fromValue(titleValue, errors);
    protocol::Value* transitionTypeValue = object->get("transitionType");
    errors->setName("transitionType");
    result->m_transitionType = ValueConversions<String>::fromValue(transitionTypeValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> NavigationEntry::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<int>::toValue(m_id));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("userTypedURL", ValueConversions<String>::toValue(m_userTypedURL));
    result->setValue("title", ValueConversions<String>::toValue(m_title));
    result->setValue("transitionType", ValueConversions<String>::toValue(m_transitionType));
    return result;
}

std::unique_ptr<NavigationEntry> NavigationEntry::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScreencastFrameMetadata> ScreencastFrameMetadata::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScreencastFrameMetadata> result(new ScreencastFrameMetadata());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* offsetTopValue = object->get("offsetTop");
    errors->setName("offsetTop");
    result->m_offsetTop = ValueConversions<double>::fromValue(offsetTopValue, errors);
    protocol::Value* pageScaleFactorValue = object->get("pageScaleFactor");
    errors->setName("pageScaleFactor");
    result->m_pageScaleFactor = ValueConversions<double>::fromValue(pageScaleFactorValue, errors);
    protocol::Value* deviceWidthValue = object->get("deviceWidth");
    errors->setName("deviceWidth");
    result->m_deviceWidth = ValueConversions<double>::fromValue(deviceWidthValue, errors);
    protocol::Value* deviceHeightValue = object->get("deviceHeight");
    errors->setName("deviceHeight");
    result->m_deviceHeight = ValueConversions<double>::fromValue(deviceHeightValue, errors);
    protocol::Value* scrollOffsetXValue = object->get("scrollOffsetX");
    errors->setName("scrollOffsetX");
    result->m_scrollOffsetX = ValueConversions<double>::fromValue(scrollOffsetXValue, errors);
    protocol::Value* scrollOffsetYValue = object->get("scrollOffsetY");
    errors->setName("scrollOffsetY");
    result->m_scrollOffsetY = ValueConversions<double>::fromValue(scrollOffsetYValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    if (timestampValue) {
        errors->setName("timestamp");
        result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScreencastFrameMetadata::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("offsetTop", ValueConversions<double>::toValue(m_offsetTop));
    result->setValue("pageScaleFactor", ValueConversions<double>::toValue(m_pageScaleFactor));
    result->setValue("deviceWidth", ValueConversions<double>::toValue(m_deviceWidth));
    result->setValue("deviceHeight", ValueConversions<double>::toValue(m_deviceHeight));
    result->setValue("scrollOffsetX", ValueConversions<double>::toValue(m_scrollOffsetX));
    result->setValue("scrollOffsetY", ValueConversions<double>::toValue(m_scrollOffsetY));
    if (m_timestamp.isJust()) {
        result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp.fromJust()));
    }
    return result;
}

std::unique_ptr<ScreencastFrameMetadata> ScreencastFrameMetadata::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

namespace DialogTypeEnum {
const char Alert[] = "alert";
const char Confirm[] = "confirm";
const char Prompt[] = "prompt";
const char Beforeunload[] = "beforeunload";
} // namespace DialogTypeEnum

std::unique_ptr<AppManifestError> AppManifestError::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<AppManifestError> result(new AppManifestError());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* messageValue = object->get("message");
    errors->setName("message");
    result->m_message = ValueConversions<String>::fromValue(messageValue, errors);
    protocol::Value* criticalValue = object->get("critical");
    errors->setName("critical");
    result->m_critical = ValueConversions<int>::fromValue(criticalValue, errors);
    protocol::Value* lineValue = object->get("line");
    errors->setName("line");
    result->m_line = ValueConversions<int>::fromValue(lineValue, errors);
    protocol::Value* columnValue = object->get("column");
    errors->setName("column");
    result->m_column = ValueConversions<int>::fromValue(columnValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> AppManifestError::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("message", ValueConversions<String>::toValue(m_message));
    result->setValue("critical", ValueConversions<int>::toValue(m_critical));
    result->setValue("line", ValueConversions<int>::toValue(m_line));
    result->setValue("column", ValueConversions<int>::toValue(m_column));
    return result;
}

std::unique_ptr<AppManifestError> AppManifestError::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<LayoutViewport> LayoutViewport::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<LayoutViewport> result(new LayoutViewport());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* pageXValue = object->get("pageX");
    errors->setName("pageX");
    result->m_pageX = ValueConversions<int>::fromValue(pageXValue, errors);
    protocol::Value* pageYValue = object->get("pageY");
    errors->setName("pageY");
    result->m_pageY = ValueConversions<int>::fromValue(pageYValue, errors);
    protocol::Value* clientWidthValue = object->get("clientWidth");
    errors->setName("clientWidth");
    result->m_clientWidth = ValueConversions<int>::fromValue(clientWidthValue, errors);
    protocol::Value* clientHeightValue = object->get("clientHeight");
    errors->setName("clientHeight");
    result->m_clientHeight = ValueConversions<int>::fromValue(clientHeightValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LayoutViewport::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("pageX", ValueConversions<int>::toValue(m_pageX));
    result->setValue("pageY", ValueConversions<int>::toValue(m_pageY));
    result->setValue("clientWidth", ValueConversions<int>::toValue(m_clientWidth));
    result->setValue("clientHeight", ValueConversions<int>::toValue(m_clientHeight));
    return result;
}

std::unique_ptr<LayoutViewport> LayoutViewport::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<VisualViewport> VisualViewport::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<VisualViewport> result(new VisualViewport());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* offsetXValue = object->get("offsetX");
    errors->setName("offsetX");
    result->m_offsetX = ValueConversions<double>::fromValue(offsetXValue, errors);
    protocol::Value* offsetYValue = object->get("offsetY");
    errors->setName("offsetY");
    result->m_offsetY = ValueConversions<double>::fromValue(offsetYValue, errors);
    protocol::Value* pageXValue = object->get("pageX");
    errors->setName("pageX");
    result->m_pageX = ValueConversions<double>::fromValue(pageXValue, errors);
    protocol::Value* pageYValue = object->get("pageY");
    errors->setName("pageY");
    result->m_pageY = ValueConversions<double>::fromValue(pageYValue, errors);
    protocol::Value* clientWidthValue = object->get("clientWidth");
    errors->setName("clientWidth");
    result->m_clientWidth = ValueConversions<double>::fromValue(clientWidthValue, errors);
    protocol::Value* clientHeightValue = object->get("clientHeight");
    errors->setName("clientHeight");
    result->m_clientHeight = ValueConversions<double>::fromValue(clientHeightValue, errors);
    protocol::Value* scaleValue = object->get("scale");
    errors->setName("scale");
    result->m_scale = ValueConversions<double>::fromValue(scaleValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> VisualViewport::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("offsetX", ValueConversions<double>::toValue(m_offsetX));
    result->setValue("offsetY", ValueConversions<double>::toValue(m_offsetY));
    result->setValue("pageX", ValueConversions<double>::toValue(m_pageX));
    result->setValue("pageY", ValueConversions<double>::toValue(m_pageY));
    result->setValue("clientWidth", ValueConversions<double>::toValue(m_clientWidth));
    result->setValue("clientHeight", ValueConversions<double>::toValue(m_clientHeight));
    result->setValue("scale", ValueConversions<double>::toValue(m_scale));
    return result;
}

std::unique_ptr<VisualViewport> VisualViewport::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<Viewport> Viewport::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> Viewport::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("x", ValueConversions<double>::toValue(m_x));
    result->setValue("y", ValueConversions<double>::toValue(m_y));
    result->setValue("width", ValueConversions<double>::toValue(m_width));
    result->setValue("height", ValueConversions<double>::toValue(m_height));
    result->setValue("scale", ValueConversions<double>::toValue(m_scale));
    return result;
}

std::unique_ptr<Viewport> Viewport::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<DomContentEventFiredNotification> DomContentEventFiredNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<DomContentEventFiredNotification> result(new DomContentEventFiredNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> DomContentEventFiredNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    return result;
}

std::unique_ptr<DomContentEventFiredNotification> DomContentEventFiredNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameAttachedNotification> FrameAttachedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameAttachedNotification> result(new FrameAttachedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* parentFrameIdValue = object->get("parentFrameId");
    errors->setName("parentFrameId");
    result->m_parentFrameId = ValueConversions<String>::fromValue(parentFrameIdValue, errors);
    protocol::Value* stackValue = object->get("stack");
    if (stackValue) {
        errors->setName("stack");
        result->m_stack = ValueConversions<protocol::Runtime::StackTrace>::fromValue(stackValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameAttachedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("parentFrameId", ValueConversions<String>::toValue(m_parentFrameId));
    if (m_stack.isJust()) {
        result->setValue("stack", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_stack.fromJust()));
    }
    return result;
}

std::unique_ptr<FrameAttachedNotification> FrameAttachedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameClearedScheduledNavigationNotification> FrameClearedScheduledNavigationNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameClearedScheduledNavigationNotification> result(new FrameClearedScheduledNavigationNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameClearedScheduledNavigationNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    return result;
}

std::unique_ptr<FrameClearedScheduledNavigationNotification> FrameClearedScheduledNavigationNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameDetachedNotification> FrameDetachedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameDetachedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    return result;
}

std::unique_ptr<FrameDetachedNotification> FrameDetachedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameNavigatedNotification> FrameNavigatedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameNavigatedNotification> result(new FrameNavigatedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameValue = object->get("frame");
    errors->setName("frame");
    result->m_frame = ValueConversions<protocol::Page::Frame>::fromValue(frameValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameNavigatedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frame", ValueConversions<protocol::Page::Frame>::toValue(m_frame.get()));
    return result;
}

std::unique_ptr<FrameNavigatedNotification> FrameNavigatedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* FrameScheduledNavigationNotification::ReasonEnum::FormSubmissionGet = "formSubmissionGet";
const char* FrameScheduledNavigationNotification::ReasonEnum::FormSubmissionPost = "formSubmissionPost";
const char* FrameScheduledNavigationNotification::ReasonEnum::HttpHeaderRefresh = "httpHeaderRefresh";
const char* FrameScheduledNavigationNotification::ReasonEnum::ScriptInitiated = "scriptInitiated";
const char* FrameScheduledNavigationNotification::ReasonEnum::MetaTagRefresh = "metaTagRefresh";
const char* FrameScheduledNavigationNotification::ReasonEnum::PageBlockInterstitial = "pageBlockInterstitial";
const char* FrameScheduledNavigationNotification::ReasonEnum::Reload = "reload";

std::unique_ptr<FrameScheduledNavigationNotification> FrameScheduledNavigationNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<FrameScheduledNavigationNotification> result(new FrameScheduledNavigationNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* delayValue = object->get("delay");
    errors->setName("delay");
    result->m_delay = ValueConversions<double>::fromValue(delayValue, errors);
    protocol::Value* reasonValue = object->get("reason");
    errors->setName("reason");
    result->m_reason = ValueConversions<String>::fromValue(reasonValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameScheduledNavigationNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("delay", ValueConversions<double>::toValue(m_delay));
    result->setValue("reason", ValueConversions<String>::toValue(m_reason));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    return result;
}

std::unique_ptr<FrameScheduledNavigationNotification> FrameScheduledNavigationNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameStartedLoadingNotification> FrameStartedLoadingNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameStartedLoadingNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    return result;
}

std::unique_ptr<FrameStartedLoadingNotification> FrameStartedLoadingNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<FrameStoppedLoadingNotification> FrameStoppedLoadingNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> FrameStoppedLoadingNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    return result;
}

std::unique_ptr<FrameStoppedLoadingNotification> FrameStoppedLoadingNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<JavascriptDialogClosedNotification> JavascriptDialogClosedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<JavascriptDialogClosedNotification> result(new JavascriptDialogClosedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* resultValue = object->get("result");
    errors->setName("result");
    result->m_result = ValueConversions<bool>::fromValue(resultValue, errors);
    protocol::Value* userInputValue = object->get("userInput");
    errors->setName("userInput");
    result->m_userInput = ValueConversions<String>::fromValue(userInputValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> JavascriptDialogClosedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("result", ValueConversions<bool>::toValue(m_result));
    result->setValue("userInput", ValueConversions<String>::toValue(m_userInput));
    return result;
}

std::unique_ptr<JavascriptDialogClosedNotification> JavascriptDialogClosedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<JavascriptDialogOpeningNotification> JavascriptDialogOpeningNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<JavascriptDialogOpeningNotification> result(new JavascriptDialogOpeningNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* messageValue = object->get("message");
    errors->setName("message");
    result->m_message = ValueConversions<String>::fromValue(messageValue, errors);
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* hasBrowserHandlerValue = object->get("hasBrowserHandler");
    errors->setName("hasBrowserHandler");
    result->m_hasBrowserHandler = ValueConversions<bool>::fromValue(hasBrowserHandlerValue, errors);
    protocol::Value* defaultPromptValue = object->get("defaultPrompt");
    if (defaultPromptValue) {
        errors->setName("defaultPrompt");
        result->m_defaultPrompt = ValueConversions<String>::fromValue(defaultPromptValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> JavascriptDialogOpeningNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("message", ValueConversions<String>::toValue(m_message));
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    result->setValue("hasBrowserHandler", ValueConversions<bool>::toValue(m_hasBrowserHandler));
    if (m_defaultPrompt.isJust()) {
        result->setValue("defaultPrompt", ValueConversions<String>::toValue(m_defaultPrompt.fromJust()));
    }
    return result;
}

std::unique_ptr<JavascriptDialogOpeningNotification> JavascriptDialogOpeningNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<LifecycleEventNotification> LifecycleEventNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<LifecycleEventNotification> result(new LifecycleEventNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* loaderIdValue = object->get("loaderId");
    errors->setName("loaderId");
    result->m_loaderId = ValueConversions<String>::fromValue(loaderIdValue, errors);
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LifecycleEventNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("loaderId", ValueConversions<String>::toValue(m_loaderId));
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    return result;
}

std::unique_ptr<LifecycleEventNotification> LifecycleEventNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<LoadEventFiredNotification> LoadEventFiredNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
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
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> LoadEventFiredNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    return result;
}

std::unique_ptr<LoadEventFiredNotification> LoadEventFiredNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<NavigatedWithinDocumentNotification> NavigatedWithinDocumentNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<NavigatedWithinDocumentNotification> result(new NavigatedWithinDocumentNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* frameIdValue = object->get("frameId");
    errors->setName("frameId");
    result->m_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> NavigatedWithinDocumentNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("frameId", ValueConversions<String>::toValue(m_frameId));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    return result;
}

std::unique_ptr<NavigatedWithinDocumentNotification> NavigatedWithinDocumentNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScreencastFrameNotification> ScreencastFrameNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScreencastFrameNotification> result(new ScreencastFrameNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* dataValue = object->get("data");
    errors->setName("data");
    result->m_data = ValueConversions<String>::fromValue(dataValue, errors);
    protocol::Value* metadataValue = object->get("metadata");
    errors->setName("metadata");
    result->m_metadata = ValueConversions<protocol::Page::ScreencastFrameMetadata>::fromValue(metadataValue, errors);
    protocol::Value* sessionIdValue = object->get("sessionId");
    errors->setName("sessionId");
    result->m_sessionId = ValueConversions<int>::fromValue(sessionIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScreencastFrameNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("data", ValueConversions<String>::toValue(m_data));
    result->setValue("metadata", ValueConversions<protocol::Page::ScreencastFrameMetadata>::toValue(m_metadata.get()));
    result->setValue("sessionId", ValueConversions<int>::toValue(m_sessionId));
    return result;
}

std::unique_ptr<ScreencastFrameNotification> ScreencastFrameNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ScreencastVisibilityChangedNotification> ScreencastVisibilityChangedNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ScreencastVisibilityChangedNotification> result(new ScreencastVisibilityChangedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* visibleValue = object->get("visible");
    errors->setName("visible");
    result->m_visible = ValueConversions<bool>::fromValue(visibleValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ScreencastVisibilityChangedNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("visible", ValueConversions<bool>::toValue(m_visible));
    return result;
}

std::unique_ptr<ScreencastVisibilityChangedNotification> ScreencastVisibilityChangedNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<WindowOpenNotification> WindowOpenNotification::fromValue(protocol::Value* value, ErrorSupport* errors) {
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<WindowOpenNotification> result(new WindowOpenNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* windowNameValue = object->get("windowName");
    errors->setName("windowName");
    result->m_windowName = ValueConversions<String>::fromValue(windowNameValue, errors);
    protocol::Value* windowFeaturesValue = object->get("windowFeatures");
    errors->setName("windowFeatures");
    result->m_windowFeatures = ValueConversions<protocol::Array<String>>::fromValue(windowFeaturesValue, errors);
    protocol::Value* userGestureValue = object->get("userGesture");
    errors->setName("userGesture");
    result->m_userGesture = ValueConversions<bool>::fromValue(userGestureValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<protocol::DictionaryValue> WindowOpenNotification::toValue() const {
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("windowName", ValueConversions<String>::toValue(m_windowName));
    result->setValue("windowFeatures", ValueConversions<protocol::Array<String>>::toValue(m_windowFeatures.get()));
    result->setValue("userGesture", ValueConversions<bool>::toValue(m_userGesture));
    return result;
}

std::unique_ptr<WindowOpenNotification> WindowOpenNotification::clone() const {
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


namespace CaptureScreenshot {
namespace FormatEnum {
const char* Jpeg = "jpeg";
const char* Png = "png";
} // namespace FormatEnum
} // namespace CaptureScreenshot

namespace SetDownloadBehavior {
namespace BehaviorEnum {
const char* Deny = "deny";
const char* Allow = "allow";
const char* Default = "default";
} // namespace BehaviorEnum
} // namespace SetDownloadBehavior

namespace SetTouchEmulationEnabled {
namespace ConfigurationEnum {
const char* Mobile = "mobile";
const char* Desktop = "desktop";
} // namespace ConfigurationEnum
} // namespace SetTouchEmulationEnabled

namespace StartScreencast {
namespace FormatEnum {
const char* Jpeg = "jpeg";
const char* Png = "png";
} // namespace FormatEnum
} // namespace StartScreencast

namespace FrameScheduledNavigation {
namespace ReasonEnum {
const char* FormSubmissionGet = "formSubmissionGet";
const char* FormSubmissionPost = "formSubmissionPost";
const char* HttpHeaderRefresh = "httpHeaderRefresh";
const char* ScriptInitiated = "scriptInitiated";
const char* MetaTagRefresh = "metaTagRefresh";
const char* PageBlockInterstitial = "pageBlockInterstitial";
const char* Reload = "reload";
} // namespace ReasonEnum
} // namespace FrameScheduledNavigation

// ------------- Frontend notifications.

void Frontend::domContentEventFired(double timestamp) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<DomContentEventFiredNotification> messageData = DomContentEventFiredNotification::create()
            .setTimestamp(timestamp)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.domContentEventFired", std::move(messageData)));
}

void Frontend::frameAttached(const String& frameId, const String& parentFrameId, Maybe<protocol::Runtime::StackTrace> stack) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FrameAttachedNotification> messageData = FrameAttachedNotification::create()
            .setFrameId(frameId)
            .setParentFrameId(parentFrameId)
            .build();
    if (stack.isJust()) {
        messageData->setStack(std::move(stack).takeJust());
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameAttached", std::move(messageData)));
}

void Frontend::frameClearedScheduledNavigation(const String& frameId) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FrameClearedScheduledNavigationNotification> messageData = FrameClearedScheduledNavigationNotification::create()
            .setFrameId(frameId)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameClearedScheduledNavigation", std::move(messageData)));
}

void Frontend::frameDetached(const String& frameId) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FrameDetachedNotification> messageData = FrameDetachedNotification::create()
            .setFrameId(frameId)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameDetached", std::move(messageData)));
}

void Frontend::frameNavigated(std::unique_ptr<protocol::Page::Frame> frame) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FrameNavigatedNotification> messageData = FrameNavigatedNotification::create()
            .setFrame(std::move(frame))
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameNavigated", std::move(messageData)));
}

void Frontend::frameResized() {
    if (!m_frontendChannel) {
        return;
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameResized"));
}

void Frontend::frameScheduledNavigation(const String& frameId, double delay, const String& reason, const String& url) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FrameScheduledNavigationNotification> messageData = FrameScheduledNavigationNotification::create()
            .setFrameId(frameId)
            .setDelay(delay)
            .setReason(reason)
            .setUrl(url)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameScheduledNavigation", std::move(messageData)));
}

void Frontend::frameStartedLoading(const String& frameId) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FrameStartedLoadingNotification> messageData = FrameStartedLoadingNotification::create()
            .setFrameId(frameId)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameStartedLoading", std::move(messageData)));
}

void Frontend::frameStoppedLoading(const String& frameId) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<FrameStoppedLoadingNotification> messageData = FrameStoppedLoadingNotification::create()
            .setFrameId(frameId)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.frameStoppedLoading", std::move(messageData)));
}

void Frontend::interstitialHidden() {
    if (!m_frontendChannel) {
        return;
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.interstitialHidden"));
}

void Frontend::interstitialShown() {
    if (!m_frontendChannel) {
        return;
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.interstitialShown"));
}

void Frontend::javascriptDialogClosed(bool result, const String& userInput) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<JavascriptDialogClosedNotification> messageData = JavascriptDialogClosedNotification::create()
            .setResult(result)
            .setUserInput(userInput)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.javascriptDialogClosed", std::move(messageData)));
}

void Frontend::javascriptDialogOpening(const String& url, const String& message, const String& type, bool hasBrowserHandler, Maybe<String> defaultPrompt) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<JavascriptDialogOpeningNotification> messageData = JavascriptDialogOpeningNotification::create()
            .setUrl(url)
            .setMessage(message)
            .setType(type)
            .setHasBrowserHandler(hasBrowserHandler)
            .build();
    if (defaultPrompt.isJust()) {
        messageData->setDefaultPrompt(std::move(defaultPrompt).takeJust());
    }
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.javascriptDialogOpening", std::move(messageData)));
}

void Frontend::lifecycleEvent(const String& frameId, const String& loaderId, const String& name, double timestamp) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<LifecycleEventNotification> messageData = LifecycleEventNotification::create()
            .setFrameId(frameId)
            .setLoaderId(loaderId)
            .setName(name)
            .setTimestamp(timestamp)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.lifecycleEvent", std::move(messageData)));
}

void Frontend::loadEventFired(double timestamp) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<LoadEventFiredNotification> messageData = LoadEventFiredNotification::create()
            .setTimestamp(timestamp)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.loadEventFired", std::move(messageData)));
}

void Frontend::navigatedWithinDocument(const String& frameId, const String& url) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<NavigatedWithinDocumentNotification> messageData = NavigatedWithinDocumentNotification::create()
            .setFrameId(frameId)
            .setUrl(url)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.navigatedWithinDocument", std::move(messageData)));
}

void Frontend::screencastFrame(const String& data, std::unique_ptr<protocol::Page::ScreencastFrameMetadata> metadata, int sessionId) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<ScreencastFrameNotification> messageData = ScreencastFrameNotification::create()
            .setData(data)
            .setMetadata(std::move(metadata))
            .setSessionId(sessionId)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.screencastFrame", std::move(messageData)));
}

void Frontend::screencastVisibilityChanged(bool visible) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<ScreencastVisibilityChangedNotification> messageData = ScreencastVisibilityChangedNotification::create()
            .setVisible(visible)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.screencastVisibilityChanged", std::move(messageData)));
}

void Frontend::windowOpen(const String& url, const String& windowName, std::unique_ptr<protocol::Array<String>> windowFeatures, bool userGesture) {
    if (!m_frontendChannel) {
        return;
    }
    std::unique_ptr<WindowOpenNotification> messageData = WindowOpenNotification::create()
            .setUrl(url)
            .setWindowName(windowName)
            .setWindowFeatures(std::move(windowFeatures))
            .setUserGesture(userGesture)
            .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Page.windowOpen", std::move(messageData)));
}

void Frontend::flush() {
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification) {
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
    public:
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend)
            : DispatcherBase(frontendChannel)
            , m_backend(backend) {
            m_dispatchMap["Page.addScriptToEvaluateOnLoad"] = &DispatcherImpl::addScriptToEvaluateOnLoad;
            m_dispatchMap["Page.addScriptToEvaluateOnNewDocument"] = &DispatcherImpl::addScriptToEvaluateOnNewDocument;
            m_dispatchMap["Page.bringToFront"] = &DispatcherImpl::bringToFront;
            m_dispatchMap["Page.captureScreenshot"] = &DispatcherImpl::captureScreenshot;
            m_redirects["Page.clearDeviceMetricsOverride"] = "Emulation.clearDeviceMetricsOverride";
            m_redirects["Page.clearDeviceOrientationOverride"] = "DeviceOrientation.clearDeviceOrientationOverride";
            m_redirects["Page.clearGeolocationOverride"] = "Emulation.clearGeolocationOverride";
            m_dispatchMap["Page.createIsolatedWorld"] = &DispatcherImpl::createIsolatedWorld;
            m_redirects["Page.deleteCookie"] = "Network.deleteCookie";
            m_dispatchMap["Page.disable"] = &DispatcherImpl::disable;
            m_dispatchMap["Page.enable"] = &DispatcherImpl::enable;
            m_dispatchMap["Page.getAppManifest"] = &DispatcherImpl::getAppManifest;
            m_redirects["Page.getCookies"] = "Network.getCookies";
            m_dispatchMap["Page.getFrameTree"] = &DispatcherImpl::getFrameTree;
            m_dispatchMap["Page.getLayoutMetrics"] = &DispatcherImpl::getLayoutMetrics;
            m_dispatchMap["Page.getNavigationHistory"] = &DispatcherImpl::getNavigationHistory;
            m_dispatchMap["Page.getResourceContent"] = &DispatcherImpl::getResourceContent;
            m_dispatchMap["Page.getResourceTree"] = &DispatcherImpl::getResourceTree;
            m_dispatchMap["Page.handleJavaScriptDialog"] = &DispatcherImpl::handleJavaScriptDialog;
            m_dispatchMap["Page.navigate"] = &DispatcherImpl::navigate;
            m_dispatchMap["Page.navigateToHistoryEntry"] = &DispatcherImpl::navigateToHistoryEntry;
            m_dispatchMap["Page.printToPDF"] = &DispatcherImpl::printToPDF;
            m_dispatchMap["Page.reload"] = &DispatcherImpl::reload;
            m_dispatchMap["Page.removeScriptToEvaluateOnLoad"] = &DispatcherImpl::removeScriptToEvaluateOnLoad;
            m_dispatchMap["Page.removeScriptToEvaluateOnNewDocument"] = &DispatcherImpl::removeScriptToEvaluateOnNewDocument;
            m_dispatchMap["Page.requestAppBanner"] = &DispatcherImpl::requestAppBanner;
            m_dispatchMap["Page.screencastFrameAck"] = &DispatcherImpl::screencastFrameAck;
            m_dispatchMap["Page.searchInResource"] = &DispatcherImpl::searchInResource;
            m_dispatchMap["Page.setAdBlockingEnabled"] = &DispatcherImpl::setAdBlockingEnabled;
            m_dispatchMap["Page.setBypassCSP"] = &DispatcherImpl::setBypassCSP;
            m_redirects["Page.setDeviceMetricsOverride"] = "Emulation.setDeviceMetricsOverride";
            m_redirects["Page.setDeviceOrientationOverride"] = "DeviceOrientation.setDeviceOrientationOverride";
            m_dispatchMap["Page.setDocumentContent"] = &DispatcherImpl::setDocumentContent;
            m_dispatchMap["Page.setDownloadBehavior"] = &DispatcherImpl::setDownloadBehavior;
            m_redirects["Page.setGeolocationOverride"] = "Emulation.setGeolocationOverride";
            m_dispatchMap["Page.setLifecycleEventsEnabled"] = &DispatcherImpl::setLifecycleEventsEnabled;
            m_redirects["Page.setTouchEmulationEnabled"] = "Emulation.setTouchEmulationEnabled";
            m_dispatchMap["Page.startScreencast"] = &DispatcherImpl::startScreencast;
            m_dispatchMap["Page.stopLoading"] = &DispatcherImpl::stopLoading;
            m_dispatchMap["Page.crash"] = &DispatcherImpl::crash;
            m_dispatchMap["Page.stopScreencast"] = &DispatcherImpl::stopScreencast;
        }
        ~DispatcherImpl() override { }
        bool canDispatch(const String& method) override;
        void dispatch(int callId, const String& method, const String& message, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
        std::unordered_map<String, String>& redirects() {
            return m_redirects;
        }

    protected:
        using CallHandler = void (DispatcherImpl::*)(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = std::unordered_map<String, CallHandler>;
        DispatchMap m_dispatchMap;
        std::unordered_map<String, String> m_redirects;

        void addScriptToEvaluateOnLoad(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void addScriptToEvaluateOnNewDocument(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void bringToFront(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void captureScreenshot(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void createIsolatedWorld(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void disable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void enable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getAppManifest(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getFrameTree(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getLayoutMetrics(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getNavigationHistory(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getResourceContent(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void getResourceTree(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void handleJavaScriptDialog(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void navigate(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void navigateToHistoryEntry(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void printToPDF(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void reload(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void removeScriptToEvaluateOnLoad(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void removeScriptToEvaluateOnNewDocument(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void requestAppBanner(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void screencastFrameAck(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void searchInResource(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setAdBlockingEnabled(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setBypassCSP(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setDocumentContent(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setDownloadBehavior(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void setLifecycleEventsEnabled(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void startScreencast(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void stopLoading(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void crash(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
        void stopScreencast(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

        Backend* m_backend;
};

bool DispatcherImpl::canDispatch(const String& method) {
    return m_dispatchMap.find(method) != m_dispatchMap.end();
}

void DispatcherImpl::dispatch(int callId, const String& method, const String& message, std::unique_ptr<protocol::DictionaryValue> messageObject) {
    std::unordered_map<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    DCHECK(it != m_dispatchMap.end());
    protocol::ErrorSupport errors;
    (this->*(it->second))(callId, method, message, std::move(messageObject), &errors);
}


void DispatcherImpl::addScriptToEvaluateOnLoad(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptSourceValue = object ? object->get("scriptSource") : nullptr;
    errors->setName("scriptSource");
    String in_scriptSource = ValueConversions<String>::fromValue(scriptSourceValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    String out_identifier;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->addScriptToEvaluateOnLoad(in_scriptSource, &out_identifier);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("identifier", ValueConversions<String>::toValue(out_identifier));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::addScriptToEvaluateOnNewDocument(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* sourceValue = object ? object->get("source") : nullptr;
    errors->setName("source");
    String in_source = ValueConversions<String>::fromValue(sourceValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    String out_identifier;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->addScriptToEvaluateOnNewDocument(in_source, &out_identifier);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("identifier", ValueConversions<String>::toValue(out_identifier));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::bringToFront(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->bringToFront();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::captureScreenshot(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* formatValue = object ? object->get("format") : nullptr;
    Maybe<String> in_format;
    if (formatValue) {
        errors->setName("format");
        in_format = ValueConversions<String>::fromValue(formatValue, errors);
    }
    protocol::Value* qualityValue = object ? object->get("quality") : nullptr;
    Maybe<int> in_quality;
    if (qualityValue) {
        errors->setName("quality");
        in_quality = ValueConversions<int>::fromValue(qualityValue, errors);
    }
    protocol::Value* clipValue = object ? object->get("clip") : nullptr;
    Maybe<protocol::Page::Viewport> in_clip;
    if (clipValue) {
        errors->setName("clip");
        in_clip = ValueConversions<protocol::Page::Viewport>::fromValue(clipValue, errors);
    }
    protocol::Value* fromSurfaceValue = object ? object->get("fromSurface") : nullptr;
    Maybe<bool> in_fromSurface;
    if (fromSurfaceValue) {
        errors->setName("fromSurface");
        in_fromSurface = ValueConversions<bool>::fromValue(fromSurfaceValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    String out_data;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->captureScreenshot(std::move(in_format), std::move(in_quality), std::move(in_clip), std::move(in_fromSurface), &out_data);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("data", ValueConversions<String>::toValue(out_data));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::createIsolatedWorld(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    errors->setName("frameId");
    String in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    protocol::Value* worldNameValue = object ? object->get("worldName") : nullptr;
    Maybe<String> in_worldName;
    if (worldNameValue) {
        errors->setName("worldName");
        in_worldName = ValueConversions<String>::fromValue(worldNameValue, errors);
    }
    protocol::Value* grantUniveralAccessValue = object ? object->get("grantUniveralAccess") : nullptr;
    Maybe<bool> in_grantUniveralAccess;
    if (grantUniveralAccessValue) {
        errors->setName("grantUniveralAccess");
        in_grantUniveralAccess = ValueConversions<bool>::fromValue(grantUniveralAccessValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    int out_executionContextId;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->createIsolatedWorld(in_frameId, std::move(in_worldName), std::move(in_grantUniveralAccess), &out_executionContextId);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("executionContextId", ValueConversions<int>::toValue(out_executionContextId));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::disable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::enable(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::getAppManifest(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    String out_url;
    std::unique_ptr<protocol::Array<protocol::Page::AppManifestError>> out_errors;
    Maybe<String> out_data;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getAppManifest(&out_url, &out_errors, &out_data);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("url", ValueConversions<String>::toValue(out_url));
        result->setValue("errors", ValueConversions<protocol::Array<protocol::Page::AppManifestError>>::toValue(out_errors.get()));
        if (out_data.isJust()) {
            result->setValue("data", ValueConversions<String>::toValue(out_data.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::getFrameTree(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Page::FrameTree> out_frameTree;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getFrameTree(&out_frameTree);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("frameTree", ValueConversions<protocol::Page::FrameTree>::toValue(out_frameTree.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::getLayoutMetrics(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Page::LayoutViewport> out_layoutViewport;
    std::unique_ptr<protocol::Page::VisualViewport> out_visualViewport;
    std::unique_ptr<protocol::DOM::Rect> out_contentSize;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getLayoutMetrics(&out_layoutViewport, &out_visualViewport, &out_contentSize);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("layoutViewport", ValueConversions<protocol::Page::LayoutViewport>::toValue(out_layoutViewport.get()));
        result->setValue("visualViewport", ValueConversions<protocol::Page::VisualViewport>::toValue(out_visualViewport.get()));
        result->setValue("contentSize", ValueConversions<protocol::DOM::Rect>::toValue(out_contentSize.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::getNavigationHistory(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    int out_currentIndex;
    std::unique_ptr<protocol::Array<protocol::Page::NavigationEntry>> out_entries;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getNavigationHistory(&out_currentIndex, &out_entries);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("currentIndex", ValueConversions<int>::toValue(out_currentIndex));
        result->setValue("entries", ValueConversions<protocol::Array<protocol::Page::NavigationEntry>>::toValue(out_entries.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::getResourceContent(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
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
        return;
    }
    // Declare output parameters.
    String out_content;
    bool out_base64Encoded;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getResourceContent(in_frameId, in_url, &out_content, &out_base64Encoded);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("content", ValueConversions<String>::toValue(out_content));
        result->setValue("base64Encoded", ValueConversions<bool>::toValue(out_base64Encoded));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::getResourceTree(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Declare output parameters.
    std::unique_ptr<protocol::Page::FrameResourceTree> out_frameTree;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getResourceTree(&out_frameTree);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("frameTree", ValueConversions<protocol::Page::FrameResourceTree>::toValue(out_frameTree.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::handleJavaScriptDialog(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* acceptValue = object ? object->get("accept") : nullptr;
    errors->setName("accept");
    bool in_accept = ValueConversions<bool>::fromValue(acceptValue, errors);
    protocol::Value* promptTextValue = object ? object->get("promptText") : nullptr;
    Maybe<String> in_promptText;
    if (promptTextValue) {
        errors->setName("promptText");
        in_promptText = ValueConversions<String>::fromValue(promptTextValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->handleJavaScriptDialog(in_accept, std::move(in_promptText));
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::navigate(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* urlValue = object ? object->get("url") : nullptr;
    errors->setName("url");
    String in_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* referrerValue = object ? object->get("referrer") : nullptr;
    Maybe<String> in_referrer;
    if (referrerValue) {
        errors->setName("referrer");
        in_referrer = ValueConversions<String>::fromValue(referrerValue, errors);
    }
    protocol::Value* transitionTypeValue = object ? object->get("transitionType") : nullptr;
    Maybe<String> in_transitionType;
    if (transitionTypeValue) {
        errors->setName("transitionType");
        in_transitionType = ValueConversions<String>::fromValue(transitionTypeValue, errors);
    }
    protocol::Value* frameIdValue = object ? object->get("frameId") : nullptr;
    Maybe<String> in_frameId;
    if (frameIdValue) {
        errors->setName("frameId");
        in_frameId = ValueConversions<String>::fromValue(frameIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    String out_frameId;
    Maybe<String> out_loaderId;
    Maybe<String> out_errorText;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->navigate(in_url, std::move(in_referrer), std::move(in_transitionType), std::move(in_frameId), &out_frameId, &out_loaderId, &out_errorText);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("frameId", ValueConversions<String>::toValue(out_frameId));
        if (out_loaderId.isJust()) {
            result->setValue("loaderId", ValueConversions<String>::toValue(out_loaderId.fromJust()));
        }
        if (out_errorText.isJust()) {
            result->setValue("errorText", ValueConversions<String>::toValue(out_errorText.fromJust()));
        }
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::navigateToHistoryEntry(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* entryIdValue = object ? object->get("entryId") : nullptr;
    errors->setName("entryId");
    int in_entryId = ValueConversions<int>::fromValue(entryIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->navigateToHistoryEntry(in_entryId);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::printToPDF(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* landscapeValue = object ? object->get("landscape") : nullptr;
    Maybe<bool> in_landscape;
    if (landscapeValue) {
        errors->setName("landscape");
        in_landscape = ValueConversions<bool>::fromValue(landscapeValue, errors);
    }
    protocol::Value* displayHeaderFooterValue = object ? object->get("displayHeaderFooter") : nullptr;
    Maybe<bool> in_displayHeaderFooter;
    if (displayHeaderFooterValue) {
        errors->setName("displayHeaderFooter");
        in_displayHeaderFooter = ValueConversions<bool>::fromValue(displayHeaderFooterValue, errors);
    }
    protocol::Value* printBackgroundValue = object ? object->get("printBackground") : nullptr;
    Maybe<bool> in_printBackground;
    if (printBackgroundValue) {
        errors->setName("printBackground");
        in_printBackground = ValueConversions<bool>::fromValue(printBackgroundValue, errors);
    }
    protocol::Value* scaleValue = object ? object->get("scale") : nullptr;
    Maybe<double> in_scale;
    if (scaleValue) {
        errors->setName("scale");
        in_scale = ValueConversions<double>::fromValue(scaleValue, errors);
    }
    protocol::Value* paperWidthValue = object ? object->get("paperWidth") : nullptr;
    Maybe<double> in_paperWidth;
    if (paperWidthValue) {
        errors->setName("paperWidth");
        in_paperWidth = ValueConversions<double>::fromValue(paperWidthValue, errors);
    }
    protocol::Value* paperHeightValue = object ? object->get("paperHeight") : nullptr;
    Maybe<double> in_paperHeight;
    if (paperHeightValue) {
        errors->setName("paperHeight");
        in_paperHeight = ValueConversions<double>::fromValue(paperHeightValue, errors);
    }
    protocol::Value* marginTopValue = object ? object->get("marginTop") : nullptr;
    Maybe<double> in_marginTop;
    if (marginTopValue) {
        errors->setName("marginTop");
        in_marginTop = ValueConversions<double>::fromValue(marginTopValue, errors);
    }
    protocol::Value* marginBottomValue = object ? object->get("marginBottom") : nullptr;
    Maybe<double> in_marginBottom;
    if (marginBottomValue) {
        errors->setName("marginBottom");
        in_marginBottom = ValueConversions<double>::fromValue(marginBottomValue, errors);
    }
    protocol::Value* marginLeftValue = object ? object->get("marginLeft") : nullptr;
    Maybe<double> in_marginLeft;
    if (marginLeftValue) {
        errors->setName("marginLeft");
        in_marginLeft = ValueConversions<double>::fromValue(marginLeftValue, errors);
    }
    protocol::Value* marginRightValue = object ? object->get("marginRight") : nullptr;
    Maybe<double> in_marginRight;
    if (marginRightValue) {
        errors->setName("marginRight");
        in_marginRight = ValueConversions<double>::fromValue(marginRightValue, errors);
    }
    protocol::Value* pageRangesValue = object ? object->get("pageRanges") : nullptr;
    Maybe<String> in_pageRanges;
    if (pageRangesValue) {
        errors->setName("pageRanges");
        in_pageRanges = ValueConversions<String>::fromValue(pageRangesValue, errors);
    }
    protocol::Value* ignoreInvalidPageRangesValue = object ? object->get("ignoreInvalidPageRanges") : nullptr;
    Maybe<bool> in_ignoreInvalidPageRanges;
    if (ignoreInvalidPageRangesValue) {
        errors->setName("ignoreInvalidPageRanges");
        in_ignoreInvalidPageRanges = ValueConversions<bool>::fromValue(ignoreInvalidPageRangesValue, errors);
    }
    protocol::Value* headerTemplateValue = object ? object->get("headerTemplate") : nullptr;
    Maybe<String> in_headerTemplate;
    if (headerTemplateValue) {
        errors->setName("headerTemplate");
        in_headerTemplate = ValueConversions<String>::fromValue(headerTemplateValue, errors);
    }
    protocol::Value* footerTemplateValue = object ? object->get("footerTemplate") : nullptr;
    Maybe<String> in_footerTemplate;
    if (footerTemplateValue) {
        errors->setName("footerTemplate");
        in_footerTemplate = ValueConversions<String>::fromValue(footerTemplateValue, errors);
    }
    protocol::Value* preferCSSPageSizeValue = object ? object->get("preferCSSPageSize") : nullptr;
    Maybe<bool> in_preferCSSPageSize;
    if (preferCSSPageSizeValue) {
        errors->setName("preferCSSPageSize");
        in_preferCSSPageSize = ValueConversions<bool>::fromValue(preferCSSPageSizeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    String out_data;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->printToPDF(std::move(in_landscape), std::move(in_displayHeaderFooter), std::move(in_printBackground), std::move(in_scale), std::move(in_paperWidth), std::move(in_paperHeight), std::move(in_marginTop), std::move(in_marginBottom), std::move(in_marginLeft), std::move(in_marginRight), std::move(in_pageRanges), std::move(in_ignoreInvalidPageRanges), std::move(in_headerTemplate), std::move(in_footerTemplate), std::move(in_preferCSSPageSize), &out_data);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("data", ValueConversions<String>::toValue(out_data));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::reload(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
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
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->reload(std::move(in_ignoreCache), std::move(in_scriptToEvaluateOnLoad));
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::removeScriptToEvaluateOnLoad(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* identifierValue = object ? object->get("identifier") : nullptr;
    errors->setName("identifier");
    String in_identifier = ValueConversions<String>::fromValue(identifierValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->removeScriptToEvaluateOnLoad(in_identifier);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::removeScriptToEvaluateOnNewDocument(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* identifierValue = object ? object->get("identifier") : nullptr;
    errors->setName("identifier");
    String in_identifier = ValueConversions<String>::fromValue(identifierValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->removeScriptToEvaluateOnNewDocument(in_identifier);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::requestAppBanner(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->requestAppBanner();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::screencastFrameAck(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* sessionIdValue = object ? object->get("sessionId") : nullptr;
    errors->setName("sessionId");
    int in_sessionId = ValueConversions<int>::fromValue(sessionIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->screencastFrameAck(in_sessionId);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::searchInResource(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
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
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>> out_result;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->searchInResource(in_frameId, in_url, in_query, std::move(in_caseSensitive), std::move(in_isRegex), &out_result);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::Debugger::SearchMatch>>::toValue(out_result.get()));
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response, std::move(result));
    }
    return;
}

void DispatcherImpl::setAdBlockingEnabled(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* enabledValue = object ? object->get("enabled") : nullptr;
    errors->setName("enabled");
    bool in_enabled = ValueConversions<bool>::fromValue(enabledValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setAdBlockingEnabled(in_enabled);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::setBypassCSP(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* enabledValue = object ? object->get("enabled") : nullptr;
    errors->setName("enabled");
    bool in_enabled = ValueConversions<bool>::fromValue(enabledValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setBypassCSP(in_enabled);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::setDocumentContent(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
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
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setDocumentContent(in_frameId, in_html);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::setDownloadBehavior(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* behaviorValue = object ? object->get("behavior") : nullptr;
    errors->setName("behavior");
    String in_behavior = ValueConversions<String>::fromValue(behaviorValue, errors);
    protocol::Value* downloadPathValue = object ? object->get("downloadPath") : nullptr;
    Maybe<String> in_downloadPath;
    if (downloadPathValue) {
        errors->setName("downloadPath");
        in_downloadPath = ValueConversions<String>::fromValue(downloadPathValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setDownloadBehavior(in_behavior, std::move(in_downloadPath));
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::setLifecycleEventsEnabled(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* enabledValue = object ? object->get("enabled") : nullptr;
    errors->setName("enabled");
    bool in_enabled = ValueConversions<bool>::fromValue(enabledValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setLifecycleEventsEnabled(in_enabled);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::startScreencast(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* formatValue = object ? object->get("format") : nullptr;
    Maybe<String> in_format;
    if (formatValue) {
        errors->setName("format");
        in_format = ValueConversions<String>::fromValue(formatValue, errors);
    }
    protocol::Value* qualityValue = object ? object->get("quality") : nullptr;
    Maybe<int> in_quality;
    if (qualityValue) {
        errors->setName("quality");
        in_quality = ValueConversions<int>::fromValue(qualityValue, errors);
    }
    protocol::Value* maxWidthValue = object ? object->get("maxWidth") : nullptr;
    Maybe<int> in_maxWidth;
    if (maxWidthValue) {
        errors->setName("maxWidth");
        in_maxWidth = ValueConversions<int>::fromValue(maxWidthValue, errors);
    }
    protocol::Value* maxHeightValue = object ? object->get("maxHeight") : nullptr;
    Maybe<int> in_maxHeight;
    if (maxHeightValue) {
        errors->setName("maxHeight");
        in_maxHeight = ValueConversions<int>::fromValue(maxHeightValue, errors);
    }
    protocol::Value* everyNthFrameValue = object ? object->get("everyNthFrame") : nullptr;
    Maybe<int> in_everyNthFrame;
    if (everyNthFrameValue) {
        errors->setName("everyNthFrame");
        in_everyNthFrame = ValueConversions<int>::fromValue(everyNthFrameValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->startScreencast(std::move(in_format), std::move(in_quality), std::move(in_maxWidth), std::move(in_maxHeight), std::move(in_everyNthFrame));
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::stopLoading(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stopLoading();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::crash(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->crash();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

void DispatcherImpl::stopScreencast(int callId, const String& method, const String& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors) {

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->stopScreencast();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get()) {
        weak->get()->sendResponse(callId, response);
    }
    return;
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend) {
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Page", std::move(dispatcher));
}

} // Page
} // namespace v8_inspector
} // namespace protocol

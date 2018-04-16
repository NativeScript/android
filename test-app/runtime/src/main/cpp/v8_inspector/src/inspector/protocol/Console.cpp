// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Console.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
    namespace protocol {
        namespace Console {

// ------------- Enum values from types.

            const char Metainfo::domainName[] = "Console";
            const char Metainfo::commandPrefix[] = "Console.";
            const char Metainfo::version[] = "1.3";

            const char* ConsoleMessage::SourceEnum::Xml = "xml";
            const char* ConsoleMessage::SourceEnum::Javascript = "javascript";
            const char* ConsoleMessage::SourceEnum::Network = "network";
            const char* ConsoleMessage::SourceEnum::ConsoleApi = "console-api";
            const char* ConsoleMessage::SourceEnum::Storage = "storage";
            const char* ConsoleMessage::SourceEnum::Appcache = "appcache";
            const char* ConsoleMessage::SourceEnum::Rendering = "rendering";
            const char* ConsoleMessage::SourceEnum::Security = "security";
            const char* ConsoleMessage::SourceEnum::Other = "other";
            const char* ConsoleMessage::SourceEnum::Deprecation = "deprecation";
            const char* ConsoleMessage::SourceEnum::Worker = "worker";

            const char* ConsoleMessage::LevelEnum::Log = "log";
            const char* ConsoleMessage::LevelEnum::Warning = "warning";
            const char* ConsoleMessage::LevelEnum::Error = "error";
            const char* ConsoleMessage::LevelEnum::Debug = "debug";
            const char* ConsoleMessage::LevelEnum::Info = "info";

            std::unique_ptr<ConsoleMessage> ConsoleMessage::fromValue(protocol::Value* value, ErrorSupport* errors)
            {
                if (!value || value->type() != protocol::Value::TypeObject) {
                    errors->addError("object expected");
                    return nullptr;
                }

                std::unique_ptr<ConsoleMessage> result(new ConsoleMessage());
                protocol::DictionaryValue* object = DictionaryValue::cast(value);
                errors->push();
                protocol::Value* sourceValue = object->get("source");
                errors->setName("source");
                result->m_source = ValueConversions<String>::fromValue(sourceValue, errors);
                protocol::Value* levelValue = object->get("level");
                errors->setName("level");
                result->m_level = ValueConversions<String>::fromValue(levelValue, errors);
                protocol::Value* textValue = object->get("text");
                errors->setName("text");
                result->m_text = ValueConversions<String>::fromValue(textValue, errors);
                protocol::Value* urlValue = object->get("url");
                if (urlValue) {
                    errors->setName("url");
                    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
                }
                protocol::Value* lineValue = object->get("line");
                if (lineValue) {
                    errors->setName("line");
                    result->m_line = ValueConversions<int>::fromValue(lineValue, errors);
                }
                protocol::Value* columnValue = object->get("column");
                if (columnValue) {
                    errors->setName("column");
                    result->m_column = ValueConversions<int>::fromValue(columnValue, errors);
                }
                errors->pop();
                if (errors->hasErrors())
                    return nullptr;
                return result;
            }

            std::unique_ptr<protocol::DictionaryValue> ConsoleMessage::toValue() const
            {
                std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
                result->setValue("source", ValueConversions<String>::toValue(m_source));
                result->setValue("level", ValueConversions<String>::toValue(m_level));
                result->setValue("text", ValueConversions<String>::toValue(m_text));
                if (m_url.isJust())
                    result->setValue("url", ValueConversions<String>::toValue(m_url.fromJust()));
                if (m_line.isJust())
                    result->setValue("line", ValueConversions<int>::toValue(m_line.fromJust()));
                if (m_column.isJust())
                    result->setValue("column", ValueConversions<int>::toValue(m_column.fromJust()));
                return result;
            }

            std::unique_ptr<ConsoleMessage> ConsoleMessage::clone() const
            {
                ErrorSupport errors;
                return fromValue(toValue().get(), &errors);
            }

            std::unique_ptr<MessageAddedNotification> MessageAddedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
            {
                if (!value || value->type() != protocol::Value::TypeObject) {
                    errors->addError("object expected");
                    return nullptr;
                }

                std::unique_ptr<MessageAddedNotification> result(new MessageAddedNotification());
                protocol::DictionaryValue* object = DictionaryValue::cast(value);
                errors->push();
                protocol::Value* messageValue = object->get("message");
                errors->setName("message");
                result->m_message = ValueConversions<protocol::Console::ConsoleMessage>::fromValue(messageValue, errors);
                errors->pop();
                if (errors->hasErrors())
                    return nullptr;
                return result;
            }

            std::unique_ptr<protocol::DictionaryValue> MessageAddedNotification::toValue() const
            {
                std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
                result->setValue("message", ValueConversions<protocol::Console::ConsoleMessage>::toValue(m_message.get()));
                return result;
            }

            std::unique_ptr<MessageAddedNotification> MessageAddedNotification::clone() const
            {
                ErrorSupport errors;
                return fromValue(toValue().get(), &errors);
            }

// ------------- Enum values from params.


// ------------- Frontend notifications.

            void Frontend::messageAdded(std::unique_ptr<protocol::Console::ConsoleMessage> message)
            {
                if (!m_frontendChannel)
                    return;
                std::unique_ptr<MessageAddedNotification> messageData = MessageAddedNotification::create()
                        .setMessage(std::move(message))
                        .build();
                m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Console.messageAdded", std::move(messageData)));
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
                    m_dispatchMap["Console.clearMessages"] = &DispatcherImpl::clearMessages;
                    m_dispatchMap["Console.disable"] = &DispatcherImpl::disable;
                    m_dispatchMap["Console.enable"] = &DispatcherImpl::enable;
                }
                ~DispatcherImpl() override { }
                DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
                HashMap<String, String>& redirects() { return m_redirects; }

            protected:
                using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
                using DispatchMap = protocol::HashMap<String, CallHandler>;
                DispatchMap m_dispatchMap;
                HashMap<String, String> m_redirects;

                DispatchResponse::Status clearMessages(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
                DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
                DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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


            DispatchResponse::Status DispatcherImpl::clearMessages(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
            {

                std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
                DispatchResponse response = m_backend->clearMessages();
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

// static
            void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
            {
                std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
                uber->setupRedirects(dispatcher->redirects());
                uber->registerBackend("Console", std::move(dispatcher));
            }

        } // Console
    } // namespace v8_inspector
} // namespace protocol

// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Log_h
#define v8_inspector_protocol_Log_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "src/inspector/protocol/Runtime.h"
#include "src/inspector/protocol/Network.h"

namespace v8_inspector {
namespace protocol {
namespace Log {

// ------------- Forward and enum declarations.
class LogEntry;
class ViolationSetting;
class EntryAddedNotification;

// ------------- Type and builder declarations.

class  LogEntry : public Serializable {
        PROTOCOL_DISALLOW_COPY(LogEntry);
    public:
        static std::unique_ptr<LogEntry> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~LogEntry() override { }

        struct  SourceEnum {
            static const char* Xml;
            static const char* Javascript;
            static const char* Network;
            static const char* Storage;
            static const char* Appcache;
            static const char* Rendering;
            static const char* Security;
            static const char* Deprecation;
            static const char* Worker;
            static const char* Violation;
            static const char* Intervention;
            static const char* Recommendation;
            static const char* Other;
        }; // SourceEnum

        String getSource() {
            return m_source;
        }
        void setSource(const String& value) {
            m_source = value;
        }

        struct  LevelEnum {
            static const char* Verbose;
            static const char* Info;
            static const char* Warning;
            static const char* Error;
        }; // LevelEnum

        String getLevel() {
            return m_level;
        }
        void setLevel(const String& value) {
            m_level = value;
        }

        String getText() {
            return m_text;
        }
        void setText(const String& value) {
            m_text = value;
        }

        double getTimestamp() {
            return m_timestamp;
        }
        void setTimestamp(double value) {
            m_timestamp = value;
        }

        bool hasUrl() {
            return m_url.isJust();
        }
        String getUrl(const String& defaultValue) {
            return m_url.isJust() ? m_url.fromJust() : defaultValue;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        bool hasLineNumber() {
            return m_lineNumber.isJust();
        }
        int getLineNumber(int defaultValue) {
            return m_lineNumber.isJust() ? m_lineNumber.fromJust() : defaultValue;
        }
        void setLineNumber(int value) {
            m_lineNumber = value;
        }

        bool hasStackTrace() {
            return m_stackTrace.isJust();
        }
        protocol::Runtime::StackTrace* getStackTrace(protocol::Runtime::StackTrace* defaultValue) {
            return m_stackTrace.isJust() ? m_stackTrace.fromJust() : defaultValue;
        }
        void setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
            m_stackTrace = std::move(value);
        }

        bool hasNetworkRequestId() {
            return m_networkRequestId.isJust();
        }
        String getNetworkRequestId(const String& defaultValue) {
            return m_networkRequestId.isJust() ? m_networkRequestId.fromJust() : defaultValue;
        }
        void setNetworkRequestId(const String& value) {
            m_networkRequestId = value;
        }

        bool hasWorkerId() {
            return m_workerId.isJust();
        }
        String getWorkerId(const String& defaultValue) {
            return m_workerId.isJust() ? m_workerId.fromJust() : defaultValue;
        }
        void setWorkerId(const String& value) {
            m_workerId = value;
        }

        bool hasArgs() {
            return m_args.isJust();
        }
        protocol::Array<protocol::Runtime::RemoteObject>* getArgs(protocol::Array<protocol::Runtime::RemoteObject>* defaultValue) {
            return m_args.isJust() ? m_args.fromJust() : defaultValue;
        }
        void setArgs(std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> value) {
            m_args = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<LogEntry> clone() const;

        template<int STATE>
        class LogEntryBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    SourceSet = 1 << 1,
                    LevelSet = 1 << 2,
                    TextSet = 1 << 3,
                    TimestampSet = 1 << 4,
                    AllFieldsSet = (SourceSet | LevelSet | TextSet | TimestampSet | 0)
                };


                LogEntryBuilder<STATE | SourceSet>& setSource(const String& value) {
                    static_assert(!(STATE & SourceSet), "property source should not be set yet");
                    m_result->setSource(value);
                    return castState<SourceSet>();
                }

                LogEntryBuilder<STATE | LevelSet>& setLevel(const String& value) {
                    static_assert(!(STATE & LevelSet), "property level should not be set yet");
                    m_result->setLevel(value);
                    return castState<LevelSet>();
                }

                LogEntryBuilder<STATE | TextSet>& setText(const String& value) {
                    static_assert(!(STATE & TextSet), "property text should not be set yet");
                    m_result->setText(value);
                    return castState<TextSet>();
                }

                LogEntryBuilder<STATE | TimestampSet>& setTimestamp(double value) {
                    static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
                    m_result->setTimestamp(value);
                    return castState<TimestampSet>();
                }

                LogEntryBuilder<STATE>& setUrl(const String& value) {
                    m_result->setUrl(value);
                    return *this;
                }

                LogEntryBuilder<STATE>& setLineNumber(int value) {
                    m_result->setLineNumber(value);
                    return *this;
                }

                LogEntryBuilder<STATE>& setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setStackTrace(std::move(value));
                    return *this;
                }

                LogEntryBuilder<STATE>& setNetworkRequestId(const String& value) {
                    m_result->setNetworkRequestId(value);
                    return *this;
                }

                LogEntryBuilder<STATE>& setWorkerId(const String& value) {
                    m_result->setWorkerId(value);
                    return *this;
                }

                LogEntryBuilder<STATE>& setArgs(std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> value) {
                    m_result->setArgs(std::move(value));
                    return *this;
                }

                std::unique_ptr<LogEntry> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class LogEntry;
                LogEntryBuilder() : m_result(new LogEntry()) { }

                template<int STEP> LogEntryBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<LogEntryBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Log::LogEntry> m_result;
        };

        static LogEntryBuilder<0> create() {
            return LogEntryBuilder<0>();
        }

    private:
        LogEntry() {
            m_timestamp = 0;
        }

        String m_source;
        String m_level;
        String m_text;
        double m_timestamp;
        Maybe<String> m_url;
        Maybe<int> m_lineNumber;
        Maybe<protocol::Runtime::StackTrace> m_stackTrace;
        Maybe<String> m_networkRequestId;
        Maybe<String> m_workerId;
        Maybe<protocol::Array<protocol::Runtime::RemoteObject>> m_args;
};


class  ViolationSetting : public Serializable {
        PROTOCOL_DISALLOW_COPY(ViolationSetting);
    public:
        static std::unique_ptr<ViolationSetting> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ViolationSetting() override { }

        struct  NameEnum {
            static const char* LongTask;
            static const char* LongLayout;
            static const char* BlockedEvent;
            static const char* BlockedParser;
            static const char* DiscouragedAPIUse;
            static const char* Handler;
            static const char* RecurringHandler;
        }; // NameEnum

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        double getThreshold() {
            return m_threshold;
        }
        void setThreshold(double value) {
            m_threshold = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ViolationSetting> clone() const;

        template<int STATE>
        class ViolationSettingBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    ThresholdSet = 1 << 2,
                    AllFieldsSet = (NameSet | ThresholdSet | 0)
                };


                ViolationSettingBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                ViolationSettingBuilder<STATE | ThresholdSet>& setThreshold(double value) {
                    static_assert(!(STATE & ThresholdSet), "property threshold should not be set yet");
                    m_result->setThreshold(value);
                    return castState<ThresholdSet>();
                }

                std::unique_ptr<ViolationSetting> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ViolationSetting;
                ViolationSettingBuilder() : m_result(new ViolationSetting()) { }

                template<int STEP> ViolationSettingBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ViolationSettingBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Log::ViolationSetting> m_result;
        };

        static ViolationSettingBuilder<0> create() {
            return ViolationSettingBuilder<0>();
        }

    private:
        ViolationSetting() {
            m_threshold = 0;
        }

        String m_name;
        double m_threshold;
};


class  EntryAddedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(EntryAddedNotification);
    public:
        static std::unique_ptr<EntryAddedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~EntryAddedNotification() override { }

        protocol::Log::LogEntry* getEntry() {
            return m_entry.get();
        }
        void setEntry(std::unique_ptr<protocol::Log::LogEntry> value) {
            m_entry = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<EntryAddedNotification> clone() const;

        template<int STATE>
        class EntryAddedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    EntrySet = 1 << 1,
                    AllFieldsSet = (EntrySet | 0)
                };


                EntryAddedNotificationBuilder<STATE | EntrySet>& setEntry(std::unique_ptr<protocol::Log::LogEntry> value) {
                    static_assert(!(STATE & EntrySet), "property entry should not be set yet");
                    m_result->setEntry(std::move(value));
                    return castState<EntrySet>();
                }

                std::unique_ptr<EntryAddedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class EntryAddedNotification;
                EntryAddedNotificationBuilder() : m_result(new EntryAddedNotification()) { }

                template<int STEP> EntryAddedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<EntryAddedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Log::EntryAddedNotification> m_result;
        };

        static EntryAddedNotificationBuilder<0> create() {
            return EntryAddedNotificationBuilder<0>();
        }

    private:
        EntryAddedNotification() {
        }

        std::unique_ptr<protocol::Log::LogEntry> m_entry;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual DispatchResponse clear() = 0;
        virtual DispatchResponse disable() = 0;
        virtual DispatchResponse enable() = 0;
        virtual DispatchResponse startViolationsReport(std::unique_ptr<protocol::Array<protocol::Log::ViolationSetting>> in_config) = 0;
        virtual DispatchResponse stopViolationsReport() = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void entryAdded(std::unique_ptr<protocol::Log::LogEntry> entry);

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

} // namespace Log
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Log_h)

// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Debugger_h
#define v8_inspector_protocol_Debugger_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "src/inspector/protocol/Runtime.h"
#include "include/inspector/Debugger.h"

namespace v8_inspector {
namespace protocol {
namespace Debugger {

// ------------- Forward and enum declarations.
using BreakpointId = String;
using CallFrameId = String;
class Location;
class ScriptPosition;
class CallFrame;
class Scope;
class SearchMatch;
class BreakLocation;
class BreakpointResolvedNotification;
class PausedNotification;
using ResumedNotification = Object;
class ScriptFailedToParseNotification;
class ScriptParsedNotification;

namespace ContinueToLocation {
namespace TargetCallFramesEnum {
extern const char* Any;
extern const char* Current;
} // TargetCallFramesEnum
} // ContinueToLocation

namespace SetPauseOnExceptions {
namespace StateEnum {
extern const char* None;
extern const char* Uncaught;
extern const char* All;
} // StateEnum
} // SetPauseOnExceptions

namespace Paused {
namespace ReasonEnum {
extern const char* XHR;
extern const char* DOM;
extern const char* EventListener;
extern const char* Exception;
extern const char* Assert;
extern const char* DebugCommand;
extern const char* PromiseRejection;
extern const char* OOM;
extern const char* Other;
extern const char* Ambiguous;
} // ReasonEnum
} // Paused

// ------------- Type and builder declarations.

class  Location : public Serializable {
        PROTOCOL_DISALLOW_COPY(Location);
    public:
        static std::unique_ptr<Location> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~Location() override { }

        String getScriptId() {
            return m_scriptId;
        }
        void setScriptId(const String& value) {
            m_scriptId = value;
        }

        int getLineNumber() {
            return m_lineNumber;
        }
        void setLineNumber(int value) {
            m_lineNumber = value;
        }

        bool hasColumnNumber() {
            return m_columnNumber.isJust();
        }
        int getColumnNumber(int defaultValue) {
            return m_columnNumber.isJust() ? m_columnNumber.fromJust() : defaultValue;
        }
        void setColumnNumber(int value) {
            m_columnNumber = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<Location> clone() const;

        template<int STATE>
        class LocationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ScriptIdSet = 1 << 1,
                    LineNumberSet = 1 << 2,
                    AllFieldsSet = (ScriptIdSet | LineNumberSet | 0)
                };


                LocationBuilder<STATE | ScriptIdSet>& setScriptId(const String& value) {
                    static_assert(!(STATE & ScriptIdSet), "property scriptId should not be set yet");
                    m_result->setScriptId(value);
                    return castState<ScriptIdSet>();
                }

                LocationBuilder<STATE | LineNumberSet>& setLineNumber(int value) {
                    static_assert(!(STATE & LineNumberSet), "property lineNumber should not be set yet");
                    m_result->setLineNumber(value);
                    return castState<LineNumberSet>();
                }

                LocationBuilder<STATE>& setColumnNumber(int value) {
                    m_result->setColumnNumber(value);
                    return *this;
                }

                std::unique_ptr<Location> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Location;
                LocationBuilder() : m_result(new Location()) { }

                template<int STEP> LocationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<LocationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::Location> m_result;
        };

        static LocationBuilder<0> create() {
            return LocationBuilder<0>();
        }

    private:
        Location() {
            m_lineNumber = 0;
        }

        String m_scriptId;
        int m_lineNumber;
        Maybe<int> m_columnNumber;
};


class  ScriptPosition : public Serializable {
        PROTOCOL_DISALLOW_COPY(ScriptPosition);
    public:
        static std::unique_ptr<ScriptPosition> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ScriptPosition() override { }

        int getLineNumber() {
            return m_lineNumber;
        }
        void setLineNumber(int value) {
            m_lineNumber = value;
        }

        int getColumnNumber() {
            return m_columnNumber;
        }
        void setColumnNumber(int value) {
            m_columnNumber = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ScriptPosition> clone() const;

        template<int STATE>
        class ScriptPositionBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    LineNumberSet = 1 << 1,
                    ColumnNumberSet = 1 << 2,
                    AllFieldsSet = (LineNumberSet | ColumnNumberSet | 0)
                };


                ScriptPositionBuilder<STATE | LineNumberSet>& setLineNumber(int value) {
                    static_assert(!(STATE & LineNumberSet), "property lineNumber should not be set yet");
                    m_result->setLineNumber(value);
                    return castState<LineNumberSet>();
                }

                ScriptPositionBuilder<STATE | ColumnNumberSet>& setColumnNumber(int value) {
                    static_assert(!(STATE & ColumnNumberSet), "property columnNumber should not be set yet");
                    m_result->setColumnNumber(value);
                    return castState<ColumnNumberSet>();
                }

                std::unique_ptr<ScriptPosition> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ScriptPosition;
                ScriptPositionBuilder() : m_result(new ScriptPosition()) { }

                template<int STEP> ScriptPositionBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ScriptPositionBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::ScriptPosition> m_result;
        };

        static ScriptPositionBuilder<0> create() {
            return ScriptPositionBuilder<0>();
        }

    private:
        ScriptPosition() {
            m_lineNumber = 0;
            m_columnNumber = 0;
        }

        int m_lineNumber;
        int m_columnNumber;
};


class  CallFrame : public Serializable {
        PROTOCOL_DISALLOW_COPY(CallFrame);
    public:
        static std::unique_ptr<CallFrame> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CallFrame() override { }

        String getCallFrameId() {
            return m_callFrameId;
        }
        void setCallFrameId(const String& value) {
            m_callFrameId = value;
        }

        String getFunctionName() {
            return m_functionName;
        }
        void setFunctionName(const String& value) {
            m_functionName = value;
        }

        bool hasFunctionLocation() {
            return m_functionLocation.isJust();
        }
        protocol::Debugger::Location* getFunctionLocation(protocol::Debugger::Location* defaultValue) {
            return m_functionLocation.isJust() ? m_functionLocation.fromJust() : defaultValue;
        }
        void setFunctionLocation(std::unique_ptr<protocol::Debugger::Location> value) {
            m_functionLocation = std::move(value);
        }

        protocol::Debugger::Location* getLocation() {
            return m_location.get();
        }
        void setLocation(std::unique_ptr<protocol::Debugger::Location> value) {
            m_location = std::move(value);
        }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        protocol::Array<protocol::Debugger::Scope>* getScopeChain() {
            return m_scopeChain.get();
        }
        void setScopeChain(std::unique_ptr<protocol::Array<protocol::Debugger::Scope>> value) {
            m_scopeChain = std::move(value);
        }

        protocol::Runtime::RemoteObject* getThis() {
            return m_this.get();
        }
        void setThis(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_this = std::move(value);
        }

        bool hasReturnValue() {
            return m_returnValue.isJust();
        }
        protocol::Runtime::RemoteObject* getReturnValue(protocol::Runtime::RemoteObject* defaultValue) {
            return m_returnValue.isJust() ? m_returnValue.fromJust() : defaultValue;
        }
        void setReturnValue(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_returnValue = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CallFrame> clone() const;

        template<int STATE>
        class CallFrameBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    CallFrameIdSet = 1 << 1,
                    FunctionNameSet = 1 << 2,
                    LocationSet = 1 << 3,
                    UrlSet = 1 << 4,
                    ScopeChainSet = 1 << 5,
                    ThisSet = 1 << 6,
                    AllFieldsSet = (CallFrameIdSet | FunctionNameSet | LocationSet | UrlSet | ScopeChainSet | ThisSet | 0)
                };


                CallFrameBuilder<STATE | CallFrameIdSet>& setCallFrameId(const String& value) {
                    static_assert(!(STATE & CallFrameIdSet), "property callFrameId should not be set yet");
                    m_result->setCallFrameId(value);
                    return castState<CallFrameIdSet>();
                }

                CallFrameBuilder<STATE | FunctionNameSet>& setFunctionName(const String& value) {
                    static_assert(!(STATE & FunctionNameSet), "property functionName should not be set yet");
                    m_result->setFunctionName(value);
                    return castState<FunctionNameSet>();
                }

                CallFrameBuilder<STATE>& setFunctionLocation(std::unique_ptr<protocol::Debugger::Location> value) {
                    m_result->setFunctionLocation(std::move(value));
                    return *this;
                }

                CallFrameBuilder<STATE | LocationSet>& setLocation(std::unique_ptr<protocol::Debugger::Location> value) {
                    static_assert(!(STATE & LocationSet), "property location should not be set yet");
                    m_result->setLocation(std::move(value));
                    return castState<LocationSet>();
                }

                CallFrameBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                CallFrameBuilder<STATE | ScopeChainSet>& setScopeChain(std::unique_ptr<protocol::Array<protocol::Debugger::Scope>> value) {
                    static_assert(!(STATE & ScopeChainSet), "property scopeChain should not be set yet");
                    m_result->setScopeChain(std::move(value));
                    return castState<ScopeChainSet>();
                }

                CallFrameBuilder<STATE | ThisSet>& setThis(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    static_assert(!(STATE & ThisSet), "property this should not be set yet");
                    m_result->setThis(std::move(value));
                    return castState<ThisSet>();
                }

                CallFrameBuilder<STATE>& setReturnValue(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    m_result->setReturnValue(std::move(value));
                    return *this;
                }

                std::unique_ptr<CallFrame> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CallFrame;
                CallFrameBuilder() : m_result(new CallFrame()) { }

                template<int STEP> CallFrameBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CallFrameBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::CallFrame> m_result;
        };

        static CallFrameBuilder<0> create() {
            return CallFrameBuilder<0>();
        }

    private:
        CallFrame() {
        }

        String m_callFrameId;
        String m_functionName;
        Maybe<protocol::Debugger::Location> m_functionLocation;
        std::unique_ptr<protocol::Debugger::Location> m_location;
        String m_url;
        std::unique_ptr<protocol::Array<protocol::Debugger::Scope>> m_scopeChain;
        std::unique_ptr<protocol::Runtime::RemoteObject> m_this;
        Maybe<protocol::Runtime::RemoteObject> m_returnValue;
};


class  Scope : public Serializable {
        PROTOCOL_DISALLOW_COPY(Scope);
    public:
        static std::unique_ptr<Scope> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~Scope() override { }

        struct  TypeEnum {
            static const char* Global;
            static const char* Local;
            static const char* With;
            static const char* Closure;
            static const char* Catch;
            static const char* Block;
            static const char* Script;
            static const char* Eval;
            static const char* Module;
        }; // TypeEnum

        String getType() {
            return m_type;
        }
        void setType(const String& value) {
            m_type = value;
        }

        protocol::Runtime::RemoteObject* getObject() {
            return m_object.get();
        }
        void setObject(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_object = std::move(value);
        }

        bool hasName() {
            return m_name.isJust();
        }
        String getName(const String& defaultValue) {
            return m_name.isJust() ? m_name.fromJust() : defaultValue;
        }
        void setName(const String& value) {
            m_name = value;
        }

        bool hasStartLocation() {
            return m_startLocation.isJust();
        }
        protocol::Debugger::Location* getStartLocation(protocol::Debugger::Location* defaultValue) {
            return m_startLocation.isJust() ? m_startLocation.fromJust() : defaultValue;
        }
        void setStartLocation(std::unique_ptr<protocol::Debugger::Location> value) {
            m_startLocation = std::move(value);
        }

        bool hasEndLocation() {
            return m_endLocation.isJust();
        }
        protocol::Debugger::Location* getEndLocation(protocol::Debugger::Location* defaultValue) {
            return m_endLocation.isJust() ? m_endLocation.fromJust() : defaultValue;
        }
        void setEndLocation(std::unique_ptr<protocol::Debugger::Location> value) {
            m_endLocation = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<Scope> clone() const;

        template<int STATE>
        class ScopeBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TypeSet = 1 << 1,
                    ObjectSet = 1 << 2,
                    AllFieldsSet = (TypeSet | ObjectSet | 0)
                };


                ScopeBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                ScopeBuilder<STATE | ObjectSet>& setObject(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    static_assert(!(STATE & ObjectSet), "property object should not be set yet");
                    m_result->setObject(std::move(value));
                    return castState<ObjectSet>();
                }

                ScopeBuilder<STATE>& setName(const String& value) {
                    m_result->setName(value);
                    return *this;
                }

                ScopeBuilder<STATE>& setStartLocation(std::unique_ptr<protocol::Debugger::Location> value) {
                    m_result->setStartLocation(std::move(value));
                    return *this;
                }

                ScopeBuilder<STATE>& setEndLocation(std::unique_ptr<protocol::Debugger::Location> value) {
                    m_result->setEndLocation(std::move(value));
                    return *this;
                }

                std::unique_ptr<Scope> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class Scope;
                ScopeBuilder() : m_result(new Scope()) { }

                template<int STEP> ScopeBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ScopeBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::Scope> m_result;
        };

        static ScopeBuilder<0> create() {
            return ScopeBuilder<0>();
        }

    private:
        Scope() {
        }

        String m_type;
        std::unique_ptr<protocol::Runtime::RemoteObject> m_object;
        Maybe<String> m_name;
        Maybe<protocol::Debugger::Location> m_startLocation;
        Maybe<protocol::Debugger::Location> m_endLocation;
};


class  SearchMatch : public Serializable, public API::SearchMatch {
        PROTOCOL_DISALLOW_COPY(SearchMatch);
    public:
        static std::unique_ptr<SearchMatch> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~SearchMatch() override { }

        double getLineNumber() {
            return m_lineNumber;
        }
        void setLineNumber(double value) {
            m_lineNumber = value;
        }

        String getLineContent() {
            return m_lineContent;
        }
        void setLineContent(const String& value) {
            m_lineContent = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<SearchMatch> clone() const;
        std::unique_ptr<StringBuffer> toJSONString() const override;

        template<int STATE>
        class SearchMatchBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    LineNumberSet = 1 << 1,
                    LineContentSet = 1 << 2,
                    AllFieldsSet = (LineNumberSet | LineContentSet | 0)
                };


                SearchMatchBuilder<STATE | LineNumberSet>& setLineNumber(double value) {
                    static_assert(!(STATE & LineNumberSet), "property lineNumber should not be set yet");
                    m_result->setLineNumber(value);
                    return castState<LineNumberSet>();
                }

                SearchMatchBuilder<STATE | LineContentSet>& setLineContent(const String& value) {
                    static_assert(!(STATE & LineContentSet), "property lineContent should not be set yet");
                    m_result->setLineContent(value);
                    return castState<LineContentSet>();
                }

                std::unique_ptr<SearchMatch> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class SearchMatch;
                SearchMatchBuilder() : m_result(new SearchMatch()) { }

                template<int STEP> SearchMatchBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<SearchMatchBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::SearchMatch> m_result;
        };

        static SearchMatchBuilder<0> create() {
            return SearchMatchBuilder<0>();
        }

    private:
        SearchMatch() {
            m_lineNumber = 0;
        }

        double m_lineNumber;
        String m_lineContent;
};


class  BreakLocation : public Serializable {
        PROTOCOL_DISALLOW_COPY(BreakLocation);
    public:
        static std::unique_ptr<BreakLocation> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~BreakLocation() override { }

        String getScriptId() {
            return m_scriptId;
        }
        void setScriptId(const String& value) {
            m_scriptId = value;
        }

        int getLineNumber() {
            return m_lineNumber;
        }
        void setLineNumber(int value) {
            m_lineNumber = value;
        }

        bool hasColumnNumber() {
            return m_columnNumber.isJust();
        }
        int getColumnNumber(int defaultValue) {
            return m_columnNumber.isJust() ? m_columnNumber.fromJust() : defaultValue;
        }
        void setColumnNumber(int value) {
            m_columnNumber = value;
        }

        struct  TypeEnum {
            static const char* DebuggerStatement;
            static const char* Call;
            static const char* Return;
        }; // TypeEnum

        bool hasType() {
            return m_type.isJust();
        }
        String getType(const String& defaultValue) {
            return m_type.isJust() ? m_type.fromJust() : defaultValue;
        }
        void setType(const String& value) {
            m_type = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<BreakLocation> clone() const;

        template<int STATE>
        class BreakLocationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ScriptIdSet = 1 << 1,
                    LineNumberSet = 1 << 2,
                    AllFieldsSet = (ScriptIdSet | LineNumberSet | 0)
                };


                BreakLocationBuilder<STATE | ScriptIdSet>& setScriptId(const String& value) {
                    static_assert(!(STATE & ScriptIdSet), "property scriptId should not be set yet");
                    m_result->setScriptId(value);
                    return castState<ScriptIdSet>();
                }

                BreakLocationBuilder<STATE | LineNumberSet>& setLineNumber(int value) {
                    static_assert(!(STATE & LineNumberSet), "property lineNumber should not be set yet");
                    m_result->setLineNumber(value);
                    return castState<LineNumberSet>();
                }

                BreakLocationBuilder<STATE>& setColumnNumber(int value) {
                    m_result->setColumnNumber(value);
                    return *this;
                }

                BreakLocationBuilder<STATE>& setType(const String& value) {
                    m_result->setType(value);
                    return *this;
                }

                std::unique_ptr<BreakLocation> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class BreakLocation;
                BreakLocationBuilder() : m_result(new BreakLocation()) { }

                template<int STEP> BreakLocationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<BreakLocationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::BreakLocation> m_result;
        };

        static BreakLocationBuilder<0> create() {
            return BreakLocationBuilder<0>();
        }

    private:
        BreakLocation() {
            m_lineNumber = 0;
        }

        String m_scriptId;
        int m_lineNumber;
        Maybe<int> m_columnNumber;
        Maybe<String> m_type;
};


class  BreakpointResolvedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(BreakpointResolvedNotification);
    public:
        static std::unique_ptr<BreakpointResolvedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~BreakpointResolvedNotification() override { }

        String getBreakpointId() {
            return m_breakpointId;
        }
        void setBreakpointId(const String& value) {
            m_breakpointId = value;
        }

        protocol::Debugger::Location* getLocation() {
            return m_location.get();
        }
        void setLocation(std::unique_ptr<protocol::Debugger::Location> value) {
            m_location = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<BreakpointResolvedNotification> clone() const;

        template<int STATE>
        class BreakpointResolvedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    BreakpointIdSet = 1 << 1,
                    LocationSet = 1 << 2,
                    AllFieldsSet = (BreakpointIdSet | LocationSet | 0)
                };


                BreakpointResolvedNotificationBuilder<STATE | BreakpointIdSet>& setBreakpointId(const String& value) {
                    static_assert(!(STATE & BreakpointIdSet), "property breakpointId should not be set yet");
                    m_result->setBreakpointId(value);
                    return castState<BreakpointIdSet>();
                }

                BreakpointResolvedNotificationBuilder<STATE | LocationSet>& setLocation(std::unique_ptr<protocol::Debugger::Location> value) {
                    static_assert(!(STATE & LocationSet), "property location should not be set yet");
                    m_result->setLocation(std::move(value));
                    return castState<LocationSet>();
                }

                std::unique_ptr<BreakpointResolvedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class BreakpointResolvedNotification;
                BreakpointResolvedNotificationBuilder() : m_result(new BreakpointResolvedNotification()) { }

                template<int STEP> BreakpointResolvedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<BreakpointResolvedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::BreakpointResolvedNotification> m_result;
        };

        static BreakpointResolvedNotificationBuilder<0> create() {
            return BreakpointResolvedNotificationBuilder<0>();
        }

    private:
        BreakpointResolvedNotification() {
        }

        String m_breakpointId;
        std::unique_ptr<protocol::Debugger::Location> m_location;
};


class  PausedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(PausedNotification);
    public:
        static std::unique_ptr<PausedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~PausedNotification() override { }

        protocol::Array<protocol::Debugger::CallFrame>* getCallFrames() {
            return m_callFrames.get();
        }
        void setCallFrames(std::unique_ptr<protocol::Array<protocol::Debugger::CallFrame>> value) {
            m_callFrames = std::move(value);
        }

        struct  ReasonEnum {
            static const char* XHR;
            static const char* DOM;
            static const char* EventListener;
            static const char* Exception;
            static const char* Assert;
            static const char* DebugCommand;
            static const char* PromiseRejection;
            static const char* OOM;
            static const char* Other;
            static const char* Ambiguous;
        }; // ReasonEnum

        String getReason() {
            return m_reason;
        }
        void setReason(const String& value) {
            m_reason = value;
        }

        bool hasData() {
            return m_data.isJust();
        }
        protocol::DictionaryValue* getData(protocol::DictionaryValue* defaultValue) {
            return m_data.isJust() ? m_data.fromJust() : defaultValue;
        }
        void setData(std::unique_ptr<protocol::DictionaryValue> value) {
            m_data = std::move(value);
        }

        bool hasHitBreakpoints() {
            return m_hitBreakpoints.isJust();
        }
        protocol::Array<String>* getHitBreakpoints(protocol::Array<String>* defaultValue) {
            return m_hitBreakpoints.isJust() ? m_hitBreakpoints.fromJust() : defaultValue;
        }
        void setHitBreakpoints(std::unique_ptr<protocol::Array<String>> value) {
            m_hitBreakpoints = std::move(value);
        }

        bool hasAsyncStackTrace() {
            return m_asyncStackTrace.isJust();
        }
        protocol::Runtime::StackTrace* getAsyncStackTrace(protocol::Runtime::StackTrace* defaultValue) {
            return m_asyncStackTrace.isJust() ? m_asyncStackTrace.fromJust() : defaultValue;
        }
        void setAsyncStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
            m_asyncStackTrace = std::move(value);
        }

        bool hasAsyncStackTraceId() {
            return m_asyncStackTraceId.isJust();
        }
        protocol::Runtime::StackTraceId* getAsyncStackTraceId(protocol::Runtime::StackTraceId* defaultValue) {
            return m_asyncStackTraceId.isJust() ? m_asyncStackTraceId.fromJust() : defaultValue;
        }
        void setAsyncStackTraceId(std::unique_ptr<protocol::Runtime::StackTraceId> value) {
            m_asyncStackTraceId = std::move(value);
        }

        bool hasAsyncCallStackTraceId() {
            return m_asyncCallStackTraceId.isJust();
        }
        protocol::Runtime::StackTraceId* getAsyncCallStackTraceId(protocol::Runtime::StackTraceId* defaultValue) {
            return m_asyncCallStackTraceId.isJust() ? m_asyncCallStackTraceId.fromJust() : defaultValue;
        }
        void setAsyncCallStackTraceId(std::unique_ptr<protocol::Runtime::StackTraceId> value) {
            m_asyncCallStackTraceId = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<PausedNotification> clone() const;

        template<int STATE>
        class PausedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    CallFramesSet = 1 << 1,
                    ReasonSet = 1 << 2,
                    AllFieldsSet = (CallFramesSet | ReasonSet | 0)
                };


                PausedNotificationBuilder<STATE | CallFramesSet>& setCallFrames(std::unique_ptr<protocol::Array<protocol::Debugger::CallFrame>> value) {
                    static_assert(!(STATE & CallFramesSet), "property callFrames should not be set yet");
                    m_result->setCallFrames(std::move(value));
                    return castState<CallFramesSet>();
                }

                PausedNotificationBuilder<STATE | ReasonSet>& setReason(const String& value) {
                    static_assert(!(STATE & ReasonSet), "property reason should not be set yet");
                    m_result->setReason(value);
                    return castState<ReasonSet>();
                }

                PausedNotificationBuilder<STATE>& setData(std::unique_ptr<protocol::DictionaryValue> value) {
                    m_result->setData(std::move(value));
                    return *this;
                }

                PausedNotificationBuilder<STATE>& setHitBreakpoints(std::unique_ptr<protocol::Array<String>> value) {
                    m_result->setHitBreakpoints(std::move(value));
                    return *this;
                }

                PausedNotificationBuilder<STATE>& setAsyncStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setAsyncStackTrace(std::move(value));
                    return *this;
                }

                PausedNotificationBuilder<STATE>& setAsyncStackTraceId(std::unique_ptr<protocol::Runtime::StackTraceId> value) {
                    m_result->setAsyncStackTraceId(std::move(value));
                    return *this;
                }

                PausedNotificationBuilder<STATE>& setAsyncCallStackTraceId(std::unique_ptr<protocol::Runtime::StackTraceId> value) {
                    m_result->setAsyncCallStackTraceId(std::move(value));
                    return *this;
                }

                std::unique_ptr<PausedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class PausedNotification;
                PausedNotificationBuilder() : m_result(new PausedNotification()) { }

                template<int STEP> PausedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<PausedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::PausedNotification> m_result;
        };

        static PausedNotificationBuilder<0> create() {
            return PausedNotificationBuilder<0>();
        }

    private:
        PausedNotification() {
        }

        std::unique_ptr<protocol::Array<protocol::Debugger::CallFrame>> m_callFrames;
        String m_reason;
        Maybe<protocol::DictionaryValue> m_data;
        Maybe<protocol::Array<String>> m_hitBreakpoints;
        Maybe<protocol::Runtime::StackTrace> m_asyncStackTrace;
        Maybe<protocol::Runtime::StackTraceId> m_asyncStackTraceId;
        Maybe<protocol::Runtime::StackTraceId> m_asyncCallStackTraceId;
};


class  ScriptFailedToParseNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(ScriptFailedToParseNotification);
    public:
        static std::unique_ptr<ScriptFailedToParseNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ScriptFailedToParseNotification() override { }

        String getScriptId() {
            return m_scriptId;
        }
        void setScriptId(const String& value) {
            m_scriptId = value;
        }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        int getStartLine() {
            return m_startLine;
        }
        void setStartLine(int value) {
            m_startLine = value;
        }

        int getStartColumn() {
            return m_startColumn;
        }
        void setStartColumn(int value) {
            m_startColumn = value;
        }

        int getEndLine() {
            return m_endLine;
        }
        void setEndLine(int value) {
            m_endLine = value;
        }

        int getEndColumn() {
            return m_endColumn;
        }
        void setEndColumn(int value) {
            m_endColumn = value;
        }

        int getExecutionContextId() {
            return m_executionContextId;
        }
        void setExecutionContextId(int value) {
            m_executionContextId = value;
        }

        String getHash() {
            return m_hash;
        }
        void setHash(const String& value) {
            m_hash = value;
        }

        bool hasExecutionContextAuxData() {
            return m_executionContextAuxData.isJust();
        }
        protocol::DictionaryValue* getExecutionContextAuxData(protocol::DictionaryValue* defaultValue) {
            return m_executionContextAuxData.isJust() ? m_executionContextAuxData.fromJust() : defaultValue;
        }
        void setExecutionContextAuxData(std::unique_ptr<protocol::DictionaryValue> value) {
            m_executionContextAuxData = std::move(value);
        }

        bool hasSourceMapURL() {
            return m_sourceMapURL.isJust();
        }
        String getSourceMapURL(const String& defaultValue) {
            return m_sourceMapURL.isJust() ? m_sourceMapURL.fromJust() : defaultValue;
        }
        void setSourceMapURL(const String& value) {
            m_sourceMapURL = value;
        }

        bool hasHasSourceURL() {
            return m_hasSourceURL.isJust();
        }
        bool getHasSourceURL(bool defaultValue) {
            return m_hasSourceURL.isJust() ? m_hasSourceURL.fromJust() : defaultValue;
        }
        void setHasSourceURL(bool value) {
            m_hasSourceURL = value;
        }

        bool hasIsModule() {
            return m_isModule.isJust();
        }
        bool getIsModule(bool defaultValue) {
            return m_isModule.isJust() ? m_isModule.fromJust() : defaultValue;
        }
        void setIsModule(bool value) {
            m_isModule = value;
        }

        bool hasLength() {
            return m_length.isJust();
        }
        int getLength(int defaultValue) {
            return m_length.isJust() ? m_length.fromJust() : defaultValue;
        }
        void setLength(int value) {
            m_length = value;
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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ScriptFailedToParseNotification> clone() const;

        template<int STATE>
        class ScriptFailedToParseNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ScriptIdSet = 1 << 1,
                    UrlSet = 1 << 2,
                    StartLineSet = 1 << 3,
                    StartColumnSet = 1 << 4,
                    EndLineSet = 1 << 5,
                    EndColumnSet = 1 << 6,
                    ExecutionContextIdSet = 1 << 7,
                    HashSet = 1 << 8,
                    AllFieldsSet = (ScriptIdSet | UrlSet | StartLineSet | StartColumnSet | EndLineSet | EndColumnSet | ExecutionContextIdSet | HashSet | 0)
                };


                ScriptFailedToParseNotificationBuilder<STATE | ScriptIdSet>& setScriptId(const String& value) {
                    static_assert(!(STATE & ScriptIdSet), "property scriptId should not be set yet");
                    m_result->setScriptId(value);
                    return castState<ScriptIdSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE | StartLineSet>& setStartLine(int value) {
                    static_assert(!(STATE & StartLineSet), "property startLine should not be set yet");
                    m_result->setStartLine(value);
                    return castState<StartLineSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE | StartColumnSet>& setStartColumn(int value) {
                    static_assert(!(STATE & StartColumnSet), "property startColumn should not be set yet");
                    m_result->setStartColumn(value);
                    return castState<StartColumnSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE | EndLineSet>& setEndLine(int value) {
                    static_assert(!(STATE & EndLineSet), "property endLine should not be set yet");
                    m_result->setEndLine(value);
                    return castState<EndLineSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE | EndColumnSet>& setEndColumn(int value) {
                    static_assert(!(STATE & EndColumnSet), "property endColumn should not be set yet");
                    m_result->setEndColumn(value);
                    return castState<EndColumnSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE | ExecutionContextIdSet>& setExecutionContextId(int value) {
                    static_assert(!(STATE & ExecutionContextIdSet), "property executionContextId should not be set yet");
                    m_result->setExecutionContextId(value);
                    return castState<ExecutionContextIdSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE | HashSet>& setHash(const String& value) {
                    static_assert(!(STATE & HashSet), "property hash should not be set yet");
                    m_result->setHash(value);
                    return castState<HashSet>();
                }

                ScriptFailedToParseNotificationBuilder<STATE>& setExecutionContextAuxData(std::unique_ptr<protocol::DictionaryValue> value) {
                    m_result->setExecutionContextAuxData(std::move(value));
                    return *this;
                }

                ScriptFailedToParseNotificationBuilder<STATE>& setSourceMapURL(const String& value) {
                    m_result->setSourceMapURL(value);
                    return *this;
                }

                ScriptFailedToParseNotificationBuilder<STATE>& setHasSourceURL(bool value) {
                    m_result->setHasSourceURL(value);
                    return *this;
                }

                ScriptFailedToParseNotificationBuilder<STATE>& setIsModule(bool value) {
                    m_result->setIsModule(value);
                    return *this;
                }

                ScriptFailedToParseNotificationBuilder<STATE>& setLength(int value) {
                    m_result->setLength(value);
                    return *this;
                }

                ScriptFailedToParseNotificationBuilder<STATE>& setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setStackTrace(std::move(value));
                    return *this;
                }

                std::unique_ptr<ScriptFailedToParseNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ScriptFailedToParseNotification;
                ScriptFailedToParseNotificationBuilder() : m_result(new ScriptFailedToParseNotification()) { }

                template<int STEP> ScriptFailedToParseNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ScriptFailedToParseNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::ScriptFailedToParseNotification> m_result;
        };

        static ScriptFailedToParseNotificationBuilder<0> create() {
            return ScriptFailedToParseNotificationBuilder<0>();
        }

    private:
        ScriptFailedToParseNotification() {
            m_startLine = 0;
            m_startColumn = 0;
            m_endLine = 0;
            m_endColumn = 0;
            m_executionContextId = 0;
        }

        String m_scriptId;
        String m_url;
        int m_startLine;
        int m_startColumn;
        int m_endLine;
        int m_endColumn;
        int m_executionContextId;
        String m_hash;
        Maybe<protocol::DictionaryValue> m_executionContextAuxData;
        Maybe<String> m_sourceMapURL;
        Maybe<bool> m_hasSourceURL;
        Maybe<bool> m_isModule;
        Maybe<int> m_length;
        Maybe<protocol::Runtime::StackTrace> m_stackTrace;
};


class  ScriptParsedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(ScriptParsedNotification);
    public:
        static std::unique_ptr<ScriptParsedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ScriptParsedNotification() override { }

        String getScriptId() {
            return m_scriptId;
        }
        void setScriptId(const String& value) {
            m_scriptId = value;
        }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        int getStartLine() {
            return m_startLine;
        }
        void setStartLine(int value) {
            m_startLine = value;
        }

        int getStartColumn() {
            return m_startColumn;
        }
        void setStartColumn(int value) {
            m_startColumn = value;
        }

        int getEndLine() {
            return m_endLine;
        }
        void setEndLine(int value) {
            m_endLine = value;
        }

        int getEndColumn() {
            return m_endColumn;
        }
        void setEndColumn(int value) {
            m_endColumn = value;
        }

        int getExecutionContextId() {
            return m_executionContextId;
        }
        void setExecutionContextId(int value) {
            m_executionContextId = value;
        }

        String getHash() {
            return m_hash;
        }
        void setHash(const String& value) {
            m_hash = value;
        }

        bool hasExecutionContextAuxData() {
            return m_executionContextAuxData.isJust();
        }
        protocol::DictionaryValue* getExecutionContextAuxData(protocol::DictionaryValue* defaultValue) {
            return m_executionContextAuxData.isJust() ? m_executionContextAuxData.fromJust() : defaultValue;
        }
        void setExecutionContextAuxData(std::unique_ptr<protocol::DictionaryValue> value) {
            m_executionContextAuxData = std::move(value);
        }

        bool hasIsLiveEdit() {
            return m_isLiveEdit.isJust();
        }
        bool getIsLiveEdit(bool defaultValue) {
            return m_isLiveEdit.isJust() ? m_isLiveEdit.fromJust() : defaultValue;
        }
        void setIsLiveEdit(bool value) {
            m_isLiveEdit = value;
        }

        bool hasSourceMapURL() {
            return m_sourceMapURL.isJust();
        }
        String getSourceMapURL(const String& defaultValue) {
            return m_sourceMapURL.isJust() ? m_sourceMapURL.fromJust() : defaultValue;
        }
        void setSourceMapURL(const String& value) {
            m_sourceMapURL = value;
        }

        bool hasHasSourceURL() {
            return m_hasSourceURL.isJust();
        }
        bool getHasSourceURL(bool defaultValue) {
            return m_hasSourceURL.isJust() ? m_hasSourceURL.fromJust() : defaultValue;
        }
        void setHasSourceURL(bool value) {
            m_hasSourceURL = value;
        }

        bool hasIsModule() {
            return m_isModule.isJust();
        }
        bool getIsModule(bool defaultValue) {
            return m_isModule.isJust() ? m_isModule.fromJust() : defaultValue;
        }
        void setIsModule(bool value) {
            m_isModule = value;
        }

        bool hasLength() {
            return m_length.isJust();
        }
        int getLength(int defaultValue) {
            return m_length.isJust() ? m_length.fromJust() : defaultValue;
        }
        void setLength(int value) {
            m_length = value;
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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ScriptParsedNotification> clone() const;

        template<int STATE>
        class ScriptParsedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ScriptIdSet = 1 << 1,
                    UrlSet = 1 << 2,
                    StartLineSet = 1 << 3,
                    StartColumnSet = 1 << 4,
                    EndLineSet = 1 << 5,
                    EndColumnSet = 1 << 6,
                    ExecutionContextIdSet = 1 << 7,
                    HashSet = 1 << 8,
                    AllFieldsSet = (ScriptIdSet | UrlSet | StartLineSet | StartColumnSet | EndLineSet | EndColumnSet | ExecutionContextIdSet | HashSet | 0)
                };


                ScriptParsedNotificationBuilder<STATE | ScriptIdSet>& setScriptId(const String& value) {
                    static_assert(!(STATE & ScriptIdSet), "property scriptId should not be set yet");
                    m_result->setScriptId(value);
                    return castState<ScriptIdSet>();
                }

                ScriptParsedNotificationBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                ScriptParsedNotificationBuilder<STATE | StartLineSet>& setStartLine(int value) {
                    static_assert(!(STATE & StartLineSet), "property startLine should not be set yet");
                    m_result->setStartLine(value);
                    return castState<StartLineSet>();
                }

                ScriptParsedNotificationBuilder<STATE | StartColumnSet>& setStartColumn(int value) {
                    static_assert(!(STATE & StartColumnSet), "property startColumn should not be set yet");
                    m_result->setStartColumn(value);
                    return castState<StartColumnSet>();
                }

                ScriptParsedNotificationBuilder<STATE | EndLineSet>& setEndLine(int value) {
                    static_assert(!(STATE & EndLineSet), "property endLine should not be set yet");
                    m_result->setEndLine(value);
                    return castState<EndLineSet>();
                }

                ScriptParsedNotificationBuilder<STATE | EndColumnSet>& setEndColumn(int value) {
                    static_assert(!(STATE & EndColumnSet), "property endColumn should not be set yet");
                    m_result->setEndColumn(value);
                    return castState<EndColumnSet>();
                }

                ScriptParsedNotificationBuilder<STATE | ExecutionContextIdSet>& setExecutionContextId(int value) {
                    static_assert(!(STATE & ExecutionContextIdSet), "property executionContextId should not be set yet");
                    m_result->setExecutionContextId(value);
                    return castState<ExecutionContextIdSet>();
                }

                ScriptParsedNotificationBuilder<STATE | HashSet>& setHash(const String& value) {
                    static_assert(!(STATE & HashSet), "property hash should not be set yet");
                    m_result->setHash(value);
                    return castState<HashSet>();
                }

                ScriptParsedNotificationBuilder<STATE>& setExecutionContextAuxData(std::unique_ptr<protocol::DictionaryValue> value) {
                    m_result->setExecutionContextAuxData(std::move(value));
                    return *this;
                }

                ScriptParsedNotificationBuilder<STATE>& setIsLiveEdit(bool value) {
                    m_result->setIsLiveEdit(value);
                    return *this;
                }

                ScriptParsedNotificationBuilder<STATE>& setSourceMapURL(const String& value) {
                    m_result->setSourceMapURL(value);
                    return *this;
                }

                ScriptParsedNotificationBuilder<STATE>& setHasSourceURL(bool value) {
                    m_result->setHasSourceURL(value);
                    return *this;
                }

                ScriptParsedNotificationBuilder<STATE>& setIsModule(bool value) {
                    m_result->setIsModule(value);
                    return *this;
                }

                ScriptParsedNotificationBuilder<STATE>& setLength(int value) {
                    m_result->setLength(value);
                    return *this;
                }

                ScriptParsedNotificationBuilder<STATE>& setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setStackTrace(std::move(value));
                    return *this;
                }

                std::unique_ptr<ScriptParsedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ScriptParsedNotification;
                ScriptParsedNotificationBuilder() : m_result(new ScriptParsedNotification()) { }

                template<int STEP> ScriptParsedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ScriptParsedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Debugger::ScriptParsedNotification> m_result;
        };

        static ScriptParsedNotificationBuilder<0> create() {
            return ScriptParsedNotificationBuilder<0>();
        }

    private:
        ScriptParsedNotification() {
            m_startLine = 0;
            m_startColumn = 0;
            m_endLine = 0;
            m_endColumn = 0;
            m_executionContextId = 0;
        }

        String m_scriptId;
        String m_url;
        int m_startLine;
        int m_startColumn;
        int m_endLine;
        int m_endColumn;
        int m_executionContextId;
        String m_hash;
        Maybe<protocol::DictionaryValue> m_executionContextAuxData;
        Maybe<bool> m_isLiveEdit;
        Maybe<String> m_sourceMapURL;
        Maybe<bool> m_hasSourceURL;
        Maybe<bool> m_isModule;
        Maybe<int> m_length;
        Maybe<protocol::Runtime::StackTrace> m_stackTrace;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual DispatchResponse continueToLocation(std::unique_ptr<protocol::Debugger::Location> in_location, Maybe<String> in_targetCallFrames) = 0;
        virtual DispatchResponse disable() = 0;
        virtual DispatchResponse enable(String* out_debuggerId) = 0;
        virtual DispatchResponse evaluateOnCallFrame(const String& in_callFrameId, const String& in_expression, Maybe<String> in_objectGroup, Maybe<bool> in_includeCommandLineAPI, Maybe<bool> in_silent, Maybe<bool> in_returnByValue, Maybe<bool> in_generatePreview, Maybe<bool> in_throwOnSideEffect, Maybe<double> in_timeout, std::unique_ptr<protocol::Runtime::RemoteObject>* out_result, Maybe<protocol::Runtime::ExceptionDetails>* out_exceptionDetails) = 0;
        virtual DispatchResponse getPossibleBreakpoints(std::unique_ptr<protocol::Debugger::Location> in_start, Maybe<protocol::Debugger::Location> in_end, Maybe<bool> in_restrictToFunction, std::unique_ptr<protocol::Array<protocol::Debugger::BreakLocation>>* out_locations) = 0;
        virtual DispatchResponse getScriptSource(const String& in_scriptId, String* out_scriptSource) = 0;
        virtual DispatchResponse getStackTrace(std::unique_ptr<protocol::Runtime::StackTraceId> in_stackTraceId, std::unique_ptr<protocol::Runtime::StackTrace>* out_stackTrace) = 0;
        virtual DispatchResponse pause() = 0;
        virtual DispatchResponse pauseOnAsyncCall(std::unique_ptr<protocol::Runtime::StackTraceId> in_parentStackTraceId) = 0;
        virtual DispatchResponse removeBreakpoint(const String& in_breakpointId) = 0;
        virtual DispatchResponse restartFrame(const String& in_callFrameId, std::unique_ptr<protocol::Array<protocol::Debugger::CallFrame>>* out_callFrames, Maybe<protocol::Runtime::StackTrace>* out_asyncStackTrace, Maybe<protocol::Runtime::StackTraceId>* out_asyncStackTraceId) = 0;
        virtual DispatchResponse resume() = 0;
        class  ScheduleStepIntoAsyncCallback {
            public:
                virtual void sendSuccess() = 0;
                virtual void sendFailure(const DispatchResponse&) = 0;
                virtual void fallThrough() = 0;
                virtual ~ScheduleStepIntoAsyncCallback() { }
        };
        virtual void scheduleStepIntoAsync(std::unique_ptr<ScheduleStepIntoAsyncCallback> callback) = 0;
        virtual DispatchResponse searchInContent(const String& in_scriptId, const String& in_query, Maybe<bool> in_caseSensitive, Maybe<bool> in_isRegex, std::unique_ptr<protocol::Array<protocol::Debugger::SearchMatch>>* out_result) = 0;
        virtual DispatchResponse setAsyncCallStackDepth(int in_maxDepth) = 0;
        virtual DispatchResponse setBlackboxPatterns(std::unique_ptr<protocol::Array<String>> in_patterns) = 0;
        virtual DispatchResponse setBlackboxedRanges(const String& in_scriptId, std::unique_ptr<protocol::Array<protocol::Debugger::ScriptPosition>> in_positions) = 0;
        virtual DispatchResponse setBreakpoint(std::unique_ptr<protocol::Debugger::Location> in_location, Maybe<String> in_condition, String* out_breakpointId, std::unique_ptr<protocol::Debugger::Location>* out_actualLocation) = 0;
        virtual DispatchResponse setBreakpointByUrl(int in_lineNumber, Maybe<String> in_url, Maybe<String> in_urlRegex, Maybe<String> in_scriptHash, Maybe<int> in_columnNumber, Maybe<String> in_condition, String* out_breakpointId, std::unique_ptr<protocol::Array<protocol::Debugger::Location>>* out_locations) = 0;
        virtual DispatchResponse setBreakpointOnFunctionCall(const String& in_objectId, Maybe<String> in_condition, String* out_breakpointId) = 0;
        virtual DispatchResponse setBreakpointsActive(bool in_active) = 0;
        virtual DispatchResponse setPauseOnExceptions(const String& in_state) = 0;
        virtual DispatchResponse setReturnValue(std::unique_ptr<protocol::Runtime::CallArgument> in_newValue) = 0;
        virtual DispatchResponse setScriptSource(const String& in_scriptId, const String& in_scriptSource, Maybe<bool> in_dryRun, Maybe<protocol::Array<protocol::Debugger::CallFrame>>* out_callFrames, Maybe<bool>* out_stackChanged, Maybe<protocol::Runtime::StackTrace>* out_asyncStackTrace, Maybe<protocol::Runtime::StackTraceId>* out_asyncStackTraceId, Maybe<protocol::Runtime::ExceptionDetails>* out_exceptionDetails) = 0;
        virtual DispatchResponse setSkipAllPauses(bool in_skip) = 0;
        virtual DispatchResponse setVariableValue(int in_scopeNumber, const String& in_variableName, std::unique_ptr<protocol::Runtime::CallArgument> in_newValue, const String& in_callFrameId) = 0;
        virtual DispatchResponse stepInto(Maybe<bool> in_breakOnAsyncCall) = 0;
        virtual DispatchResponse stepOut() = 0;
        virtual DispatchResponse stepOver() = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void breakpointResolved(const String& breakpointId, std::unique_ptr<protocol::Debugger::Location> location);
        void paused(std::unique_ptr<protocol::Array<protocol::Debugger::CallFrame>> callFrames, const String& reason, Maybe<protocol::DictionaryValue> data = Maybe<protocol::DictionaryValue>(), Maybe<protocol::Array<String>> hitBreakpoints = Maybe<protocol::Array<String>>(), Maybe<protocol::Runtime::StackTrace> asyncStackTrace = Maybe<protocol::Runtime::StackTrace>(), Maybe<protocol::Runtime::StackTraceId> asyncStackTraceId = Maybe<protocol::Runtime::StackTraceId>(), Maybe<protocol::Runtime::StackTraceId> asyncCallStackTraceId = Maybe<protocol::Runtime::StackTraceId>());
        void resumed();
        void scriptFailedToParse(const String& scriptId, const String& url, int startLine, int startColumn, int endLine, int endColumn, int executionContextId, const String& hash, Maybe<protocol::DictionaryValue> executionContextAuxData = Maybe<protocol::DictionaryValue>(), Maybe<String> sourceMapURL = Maybe<String>(), Maybe<bool> hasSourceURL = Maybe<bool>(), Maybe<bool> isModule = Maybe<bool>(), Maybe<int> length = Maybe<int>(), Maybe<protocol::Runtime::StackTrace> stackTrace = Maybe<protocol::Runtime::StackTrace>());
        void scriptParsed(const String& scriptId, const String& url, int startLine, int startColumn, int endLine, int endColumn, int executionContextId, const String& hash, Maybe<protocol::DictionaryValue> executionContextAuxData = Maybe<protocol::DictionaryValue>(), Maybe<bool> isLiveEdit = Maybe<bool>(), Maybe<String> sourceMapURL = Maybe<String>(), Maybe<bool> hasSourceURL = Maybe<bool>(), Maybe<bool> isModule = Maybe<bool>(), Maybe<int> length = Maybe<int>(), Maybe<protocol::Runtime::StackTrace> stackTrace = Maybe<protocol::Runtime::StackTrace>());

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

} // namespace Debugger
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Debugger_h)

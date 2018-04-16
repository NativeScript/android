// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Profiler_h
#define v8_inspector_protocol_Profiler_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "src/inspector/protocol/Runtime.h"
#include "src/inspector/protocol/Debugger.h"

namespace v8_inspector {
namespace protocol {
namespace Profiler {

// ------------- Forward and enum declarations.
class ProfileNode;
class Profile;
class PositionTickInfo;
class CoverageRange;
class FunctionCoverage;
class ScriptCoverage;
class TypeObject;
class TypeProfileEntry;
class ScriptTypeProfile;
class ConsoleProfileFinishedNotification;
class ConsoleProfileStartedNotification;

// ------------- Type and builder declarations.

class  ProfileNode : public Serializable{
    PROTOCOL_DISALLOW_COPY(ProfileNode);
public:
    static std::unique_ptr<ProfileNode> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ProfileNode() override { }

    int getId() { return m_id; }
    void setId(int value) { m_id = value; }

    protocol::Runtime::CallFrame* getCallFrame() { return m_callFrame.get(); }
    void setCallFrame(std::unique_ptr<protocol::Runtime::CallFrame> value) { m_callFrame = std::move(value); }

    bool hasHitCount() { return m_hitCount.isJust(); }
    int getHitCount(int defaultValue) { return m_hitCount.isJust() ? m_hitCount.fromJust() : defaultValue; }
    void setHitCount(int value) { m_hitCount = value; }

    bool hasChildren() { return m_children.isJust(); }
    protocol::Array<int>* getChildren(protocol::Array<int>* defaultValue) { return m_children.isJust() ? m_children.fromJust() : defaultValue; }
    void setChildren(std::unique_ptr<protocol::Array<int>> value) { m_children = std::move(value); }

    bool hasDeoptReason() { return m_deoptReason.isJust(); }
    String getDeoptReason(const String& defaultValue) { return m_deoptReason.isJust() ? m_deoptReason.fromJust() : defaultValue; }
    void setDeoptReason(const String& value) { m_deoptReason = value; }

    bool hasPositionTicks() { return m_positionTicks.isJust(); }
    protocol::Array<protocol::Profiler::PositionTickInfo>* getPositionTicks(protocol::Array<protocol::Profiler::PositionTickInfo>* defaultValue) { return m_positionTicks.isJust() ? m_positionTicks.fromJust() : defaultValue; }
    void setPositionTicks(std::unique_ptr<protocol::Array<protocol::Profiler::PositionTickInfo>> value) { m_positionTicks = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ProfileNode> clone() const;

    template<int STATE>
    class ProfileNodeBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            IdSet = 1 << 1,
            CallFrameSet = 1 << 2,
            AllFieldsSet = (IdSet | CallFrameSet | 0)};


        ProfileNodeBuilder<STATE | IdSet>& setId(int value)
        {
            static_assert(!(STATE & IdSet), "property id should not be set yet");
            m_result->setId(value);
            return castState<IdSet>();
        }

        ProfileNodeBuilder<STATE | CallFrameSet>& setCallFrame(std::unique_ptr<protocol::Runtime::CallFrame> value)
        {
            static_assert(!(STATE & CallFrameSet), "property callFrame should not be set yet");
            m_result->setCallFrame(std::move(value));
            return castState<CallFrameSet>();
        }

        ProfileNodeBuilder<STATE>& setHitCount(int value)
        {
            m_result->setHitCount(value);
            return *this;
        }

        ProfileNodeBuilder<STATE>& setChildren(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setChildren(std::move(value));
            return *this;
        }

        ProfileNodeBuilder<STATE>& setDeoptReason(const String& value)
        {
            m_result->setDeoptReason(value);
            return *this;
        }

        ProfileNodeBuilder<STATE>& setPositionTicks(std::unique_ptr<protocol::Array<protocol::Profiler::PositionTickInfo>> value)
        {
            m_result->setPositionTicks(std::move(value));
            return *this;
        }

        std::unique_ptr<ProfileNode> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ProfileNode;
        ProfileNodeBuilder() : m_result(new ProfileNode()) { }

        template<int STEP> ProfileNodeBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ProfileNodeBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::ProfileNode> m_result;
    };

    static ProfileNodeBuilder<0> create()
    {
        return ProfileNodeBuilder<0>();
    }

private:
    ProfileNode()
    {
          m_id = 0;
    }

    int m_id;
    std::unique_ptr<protocol::Runtime::CallFrame> m_callFrame;
    Maybe<int> m_hitCount;
    Maybe<protocol::Array<int>> m_children;
    Maybe<String> m_deoptReason;
    Maybe<protocol::Array<protocol::Profiler::PositionTickInfo>> m_positionTicks;
};


class  Profile : public Serializable{
    PROTOCOL_DISALLOW_COPY(Profile);
public:
    static std::unique_ptr<Profile> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~Profile() override { }

    protocol::Array<protocol::Profiler::ProfileNode>* getNodes() { return m_nodes.get(); }
    void setNodes(std::unique_ptr<protocol::Array<protocol::Profiler::ProfileNode>> value) { m_nodes = std::move(value); }

    double getStartTime() { return m_startTime; }
    void setStartTime(double value) { m_startTime = value; }

    double getEndTime() { return m_endTime; }
    void setEndTime(double value) { m_endTime = value; }

    bool hasSamples() { return m_samples.isJust(); }
    protocol::Array<int>* getSamples(protocol::Array<int>* defaultValue) { return m_samples.isJust() ? m_samples.fromJust() : defaultValue; }
    void setSamples(std::unique_ptr<protocol::Array<int>> value) { m_samples = std::move(value); }

    bool hasTimeDeltas() { return m_timeDeltas.isJust(); }
    protocol::Array<int>* getTimeDeltas(protocol::Array<int>* defaultValue) { return m_timeDeltas.isJust() ? m_timeDeltas.fromJust() : defaultValue; }
    void setTimeDeltas(std::unique_ptr<protocol::Array<int>> value) { m_timeDeltas = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<Profile> clone() const;

    template<int STATE>
    class ProfileBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodesSet = 1 << 1,
            StartTimeSet = 1 << 2,
            EndTimeSet = 1 << 3,
            AllFieldsSet = (NodesSet | StartTimeSet | EndTimeSet | 0)};


        ProfileBuilder<STATE | NodesSet>& setNodes(std::unique_ptr<protocol::Array<protocol::Profiler::ProfileNode>> value)
        {
            static_assert(!(STATE & NodesSet), "property nodes should not be set yet");
            m_result->setNodes(std::move(value));
            return castState<NodesSet>();
        }

        ProfileBuilder<STATE | StartTimeSet>& setStartTime(double value)
        {
            static_assert(!(STATE & StartTimeSet), "property startTime should not be set yet");
            m_result->setStartTime(value);
            return castState<StartTimeSet>();
        }

        ProfileBuilder<STATE | EndTimeSet>& setEndTime(double value)
        {
            static_assert(!(STATE & EndTimeSet), "property endTime should not be set yet");
            m_result->setEndTime(value);
            return castState<EndTimeSet>();
        }

        ProfileBuilder<STATE>& setSamples(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setSamples(std::move(value));
            return *this;
        }

        ProfileBuilder<STATE>& setTimeDeltas(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setTimeDeltas(std::move(value));
            return *this;
        }

        std::unique_ptr<Profile> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class Profile;
        ProfileBuilder() : m_result(new Profile()) { }

        template<int STEP> ProfileBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ProfileBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::Profile> m_result;
    };

    static ProfileBuilder<0> create()
    {
        return ProfileBuilder<0>();
    }

private:
    Profile()
    {
          m_startTime = 0;
          m_endTime = 0;
    }

    std::unique_ptr<protocol::Array<protocol::Profiler::ProfileNode>> m_nodes;
    double m_startTime;
    double m_endTime;
    Maybe<protocol::Array<int>> m_samples;
    Maybe<protocol::Array<int>> m_timeDeltas;
};


class  PositionTickInfo : public Serializable{
    PROTOCOL_DISALLOW_COPY(PositionTickInfo);
public:
    static std::unique_ptr<PositionTickInfo> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~PositionTickInfo() override { }

    int getLine() { return m_line; }
    void setLine(int value) { m_line = value; }

    int getTicks() { return m_ticks; }
    void setTicks(int value) { m_ticks = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<PositionTickInfo> clone() const;

    template<int STATE>
    class PositionTickInfoBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            LineSet = 1 << 1,
            TicksSet = 1 << 2,
            AllFieldsSet = (LineSet | TicksSet | 0)};


        PositionTickInfoBuilder<STATE | LineSet>& setLine(int value)
        {
            static_assert(!(STATE & LineSet), "property line should not be set yet");
            m_result->setLine(value);
            return castState<LineSet>();
        }

        PositionTickInfoBuilder<STATE | TicksSet>& setTicks(int value)
        {
            static_assert(!(STATE & TicksSet), "property ticks should not be set yet");
            m_result->setTicks(value);
            return castState<TicksSet>();
        }

        std::unique_ptr<PositionTickInfo> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class PositionTickInfo;
        PositionTickInfoBuilder() : m_result(new PositionTickInfo()) { }

        template<int STEP> PositionTickInfoBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<PositionTickInfoBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::PositionTickInfo> m_result;
    };

    static PositionTickInfoBuilder<0> create()
    {
        return PositionTickInfoBuilder<0>();
    }

private:
    PositionTickInfo()
    {
          m_line = 0;
          m_ticks = 0;
    }

    int m_line;
    int m_ticks;
};


class  CoverageRange : public Serializable{
    PROTOCOL_DISALLOW_COPY(CoverageRange);
public:
    static std::unique_ptr<CoverageRange> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~CoverageRange() override { }

    int getStartOffset() { return m_startOffset; }
    void setStartOffset(int value) { m_startOffset = value; }

    int getEndOffset() { return m_endOffset; }
    void setEndOffset(int value) { m_endOffset = value; }

    int getCount() { return m_count; }
    void setCount(int value) { m_count = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<CoverageRange> clone() const;

    template<int STATE>
    class CoverageRangeBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            StartOffsetSet = 1 << 1,
            EndOffsetSet = 1 << 2,
            CountSet = 1 << 3,
            AllFieldsSet = (StartOffsetSet | EndOffsetSet | CountSet | 0)};


        CoverageRangeBuilder<STATE | StartOffsetSet>& setStartOffset(int value)
        {
            static_assert(!(STATE & StartOffsetSet), "property startOffset should not be set yet");
            m_result->setStartOffset(value);
            return castState<StartOffsetSet>();
        }

        CoverageRangeBuilder<STATE | EndOffsetSet>& setEndOffset(int value)
        {
            static_assert(!(STATE & EndOffsetSet), "property endOffset should not be set yet");
            m_result->setEndOffset(value);
            return castState<EndOffsetSet>();
        }

        CoverageRangeBuilder<STATE | CountSet>& setCount(int value)
        {
            static_assert(!(STATE & CountSet), "property count should not be set yet");
            m_result->setCount(value);
            return castState<CountSet>();
        }

        std::unique_ptr<CoverageRange> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class CoverageRange;
        CoverageRangeBuilder() : m_result(new CoverageRange()) { }

        template<int STEP> CoverageRangeBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<CoverageRangeBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::CoverageRange> m_result;
    };

    static CoverageRangeBuilder<0> create()
    {
        return CoverageRangeBuilder<0>();
    }

private:
    CoverageRange()
    {
          m_startOffset = 0;
          m_endOffset = 0;
          m_count = 0;
    }

    int m_startOffset;
    int m_endOffset;
    int m_count;
};


class  FunctionCoverage : public Serializable{
    PROTOCOL_DISALLOW_COPY(FunctionCoverage);
public:
    static std::unique_ptr<FunctionCoverage> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~FunctionCoverage() override { }

    String getFunctionName() { return m_functionName; }
    void setFunctionName(const String& value) { m_functionName = value; }

    protocol::Array<protocol::Profiler::CoverageRange>* getRanges() { return m_ranges.get(); }
    void setRanges(std::unique_ptr<protocol::Array<protocol::Profiler::CoverageRange>> value) { m_ranges = std::move(value); }

    bool getIsBlockCoverage() { return m_isBlockCoverage; }
    void setIsBlockCoverage(bool value) { m_isBlockCoverage = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<FunctionCoverage> clone() const;

    template<int STATE>
    class FunctionCoverageBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            FunctionNameSet = 1 << 1,
            RangesSet = 1 << 2,
            IsBlockCoverageSet = 1 << 3,
            AllFieldsSet = (FunctionNameSet | RangesSet | IsBlockCoverageSet | 0)};


        FunctionCoverageBuilder<STATE | FunctionNameSet>& setFunctionName(const String& value)
        {
            static_assert(!(STATE & FunctionNameSet), "property functionName should not be set yet");
            m_result->setFunctionName(value);
            return castState<FunctionNameSet>();
        }

        FunctionCoverageBuilder<STATE | RangesSet>& setRanges(std::unique_ptr<protocol::Array<protocol::Profiler::CoverageRange>> value)
        {
            static_assert(!(STATE & RangesSet), "property ranges should not be set yet");
            m_result->setRanges(std::move(value));
            return castState<RangesSet>();
        }

        FunctionCoverageBuilder<STATE | IsBlockCoverageSet>& setIsBlockCoverage(bool value)
        {
            static_assert(!(STATE & IsBlockCoverageSet), "property isBlockCoverage should not be set yet");
            m_result->setIsBlockCoverage(value);
            return castState<IsBlockCoverageSet>();
        }

        std::unique_ptr<FunctionCoverage> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class FunctionCoverage;
        FunctionCoverageBuilder() : m_result(new FunctionCoverage()) { }

        template<int STEP> FunctionCoverageBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<FunctionCoverageBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::FunctionCoverage> m_result;
    };

    static FunctionCoverageBuilder<0> create()
    {
        return FunctionCoverageBuilder<0>();
    }

private:
    FunctionCoverage()
    {
          m_isBlockCoverage = false;
    }

    String m_functionName;
    std::unique_ptr<protocol::Array<protocol::Profiler::CoverageRange>> m_ranges;
    bool m_isBlockCoverage;
};


class  ScriptCoverage : public Serializable{
    PROTOCOL_DISALLOW_COPY(ScriptCoverage);
public:
    static std::unique_ptr<ScriptCoverage> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ScriptCoverage() override { }

    String getScriptId() { return m_scriptId; }
    void setScriptId(const String& value) { m_scriptId = value; }

    String getUrl() { return m_url; }
    void setUrl(const String& value) { m_url = value; }

    protocol::Array<protocol::Profiler::FunctionCoverage>* getFunctions() { return m_functions.get(); }
    void setFunctions(std::unique_ptr<protocol::Array<protocol::Profiler::FunctionCoverage>> value) { m_functions = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ScriptCoverage> clone() const;

    template<int STATE>
    class ScriptCoverageBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ScriptIdSet = 1 << 1,
            UrlSet = 1 << 2,
            FunctionsSet = 1 << 3,
            AllFieldsSet = (ScriptIdSet | UrlSet | FunctionsSet | 0)};


        ScriptCoverageBuilder<STATE | ScriptIdSet>& setScriptId(const String& value)
        {
            static_assert(!(STATE & ScriptIdSet), "property scriptId should not be set yet");
            m_result->setScriptId(value);
            return castState<ScriptIdSet>();
        }

        ScriptCoverageBuilder<STATE | UrlSet>& setUrl(const String& value)
        {
            static_assert(!(STATE & UrlSet), "property url should not be set yet");
            m_result->setUrl(value);
            return castState<UrlSet>();
        }

        ScriptCoverageBuilder<STATE | FunctionsSet>& setFunctions(std::unique_ptr<protocol::Array<protocol::Profiler::FunctionCoverage>> value)
        {
            static_assert(!(STATE & FunctionsSet), "property functions should not be set yet");
            m_result->setFunctions(std::move(value));
            return castState<FunctionsSet>();
        }

        std::unique_ptr<ScriptCoverage> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ScriptCoverage;
        ScriptCoverageBuilder() : m_result(new ScriptCoverage()) { }

        template<int STEP> ScriptCoverageBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ScriptCoverageBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::ScriptCoverage> m_result;
    };

    static ScriptCoverageBuilder<0> create()
    {
        return ScriptCoverageBuilder<0>();
    }

private:
    ScriptCoverage()
    {
    }

    String m_scriptId;
    String m_url;
    std::unique_ptr<protocol::Array<protocol::Profiler::FunctionCoverage>> m_functions;
};


class  TypeObject : public Serializable{
    PROTOCOL_DISALLOW_COPY(TypeObject);
public:
    static std::unique_ptr<TypeObject> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~TypeObject() override { }

    String getName() { return m_name; }
    void setName(const String& value) { m_name = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<TypeObject> clone() const;

    template<int STATE>
    class TypeObjectBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NameSet = 1 << 1,
            AllFieldsSet = (NameSet | 0)};


        TypeObjectBuilder<STATE | NameSet>& setName(const String& value)
        {
            static_assert(!(STATE & NameSet), "property name should not be set yet");
            m_result->setName(value);
            return castState<NameSet>();
        }

        std::unique_ptr<TypeObject> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class TypeObject;
        TypeObjectBuilder() : m_result(new TypeObject()) { }

        template<int STEP> TypeObjectBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<TypeObjectBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::TypeObject> m_result;
    };

    static TypeObjectBuilder<0> create()
    {
        return TypeObjectBuilder<0>();
    }

private:
    TypeObject()
    {
    }

    String m_name;
};


class  TypeProfileEntry : public Serializable{
    PROTOCOL_DISALLOW_COPY(TypeProfileEntry);
public:
    static std::unique_ptr<TypeProfileEntry> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~TypeProfileEntry() override { }

    int getOffset() { return m_offset; }
    void setOffset(int value) { m_offset = value; }

    protocol::Array<protocol::Profiler::TypeObject>* getTypes() { return m_types.get(); }
    void setTypes(std::unique_ptr<protocol::Array<protocol::Profiler::TypeObject>> value) { m_types = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<TypeProfileEntry> clone() const;

    template<int STATE>
    class TypeProfileEntryBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            OffsetSet = 1 << 1,
            TypesSet = 1 << 2,
            AllFieldsSet = (OffsetSet | TypesSet | 0)};


        TypeProfileEntryBuilder<STATE | OffsetSet>& setOffset(int value)
        {
            static_assert(!(STATE & OffsetSet), "property offset should not be set yet");
            m_result->setOffset(value);
            return castState<OffsetSet>();
        }

        TypeProfileEntryBuilder<STATE | TypesSet>& setTypes(std::unique_ptr<protocol::Array<protocol::Profiler::TypeObject>> value)
        {
            static_assert(!(STATE & TypesSet), "property types should not be set yet");
            m_result->setTypes(std::move(value));
            return castState<TypesSet>();
        }

        std::unique_ptr<TypeProfileEntry> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class TypeProfileEntry;
        TypeProfileEntryBuilder() : m_result(new TypeProfileEntry()) { }

        template<int STEP> TypeProfileEntryBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<TypeProfileEntryBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::TypeProfileEntry> m_result;
    };

    static TypeProfileEntryBuilder<0> create()
    {
        return TypeProfileEntryBuilder<0>();
    }

private:
    TypeProfileEntry()
    {
          m_offset = 0;
    }

    int m_offset;
    std::unique_ptr<protocol::Array<protocol::Profiler::TypeObject>> m_types;
};


class  ScriptTypeProfile : public Serializable{
    PROTOCOL_DISALLOW_COPY(ScriptTypeProfile);
public:
    static std::unique_ptr<ScriptTypeProfile> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ScriptTypeProfile() override { }

    String getScriptId() { return m_scriptId; }
    void setScriptId(const String& value) { m_scriptId = value; }

    String getUrl() { return m_url; }
    void setUrl(const String& value) { m_url = value; }

    protocol::Array<protocol::Profiler::TypeProfileEntry>* getEntries() { return m_entries.get(); }
    void setEntries(std::unique_ptr<protocol::Array<protocol::Profiler::TypeProfileEntry>> value) { m_entries = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ScriptTypeProfile> clone() const;

    template<int STATE>
    class ScriptTypeProfileBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            ScriptIdSet = 1 << 1,
            UrlSet = 1 << 2,
            EntriesSet = 1 << 3,
            AllFieldsSet = (ScriptIdSet | UrlSet | EntriesSet | 0)};


        ScriptTypeProfileBuilder<STATE | ScriptIdSet>& setScriptId(const String& value)
        {
            static_assert(!(STATE & ScriptIdSet), "property scriptId should not be set yet");
            m_result->setScriptId(value);
            return castState<ScriptIdSet>();
        }

        ScriptTypeProfileBuilder<STATE | UrlSet>& setUrl(const String& value)
        {
            static_assert(!(STATE & UrlSet), "property url should not be set yet");
            m_result->setUrl(value);
            return castState<UrlSet>();
        }

        ScriptTypeProfileBuilder<STATE | EntriesSet>& setEntries(std::unique_ptr<protocol::Array<protocol::Profiler::TypeProfileEntry>> value)
        {
            static_assert(!(STATE & EntriesSet), "property entries should not be set yet");
            m_result->setEntries(std::move(value));
            return castState<EntriesSet>();
        }

        std::unique_ptr<ScriptTypeProfile> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ScriptTypeProfile;
        ScriptTypeProfileBuilder() : m_result(new ScriptTypeProfile()) { }

        template<int STEP> ScriptTypeProfileBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ScriptTypeProfileBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::ScriptTypeProfile> m_result;
    };

    static ScriptTypeProfileBuilder<0> create()
    {
        return ScriptTypeProfileBuilder<0>();
    }

private:
    ScriptTypeProfile()
    {
    }

    String m_scriptId;
    String m_url;
    std::unique_ptr<protocol::Array<protocol::Profiler::TypeProfileEntry>> m_entries;
};


class  ConsoleProfileFinishedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ConsoleProfileFinishedNotification);
public:
    static std::unique_ptr<ConsoleProfileFinishedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ConsoleProfileFinishedNotification() override { }

    String getId() { return m_id; }
    void setId(const String& value) { m_id = value; }

    protocol::Debugger::Location* getLocation() { return m_location.get(); }
    void setLocation(std::unique_ptr<protocol::Debugger::Location> value) { m_location = std::move(value); }

    protocol::Profiler::Profile* getProfile() { return m_profile.get(); }
    void setProfile(std::unique_ptr<protocol::Profiler::Profile> value) { m_profile = std::move(value); }

    bool hasTitle() { return m_title.isJust(); }
    String getTitle(const String& defaultValue) { return m_title.isJust() ? m_title.fromJust() : defaultValue; }
    void setTitle(const String& value) { m_title = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ConsoleProfileFinishedNotification> clone() const;

    template<int STATE>
    class ConsoleProfileFinishedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            IdSet = 1 << 1,
            LocationSet = 1 << 2,
            ProfileSet = 1 << 3,
            AllFieldsSet = (IdSet | LocationSet | ProfileSet | 0)};


        ConsoleProfileFinishedNotificationBuilder<STATE | IdSet>& setId(const String& value)
        {
            static_assert(!(STATE & IdSet), "property id should not be set yet");
            m_result->setId(value);
            return castState<IdSet>();
        }

        ConsoleProfileFinishedNotificationBuilder<STATE | LocationSet>& setLocation(std::unique_ptr<protocol::Debugger::Location> value)
        {
            static_assert(!(STATE & LocationSet), "property location should not be set yet");
            m_result->setLocation(std::move(value));
            return castState<LocationSet>();
        }

        ConsoleProfileFinishedNotificationBuilder<STATE | ProfileSet>& setProfile(std::unique_ptr<protocol::Profiler::Profile> value)
        {
            static_assert(!(STATE & ProfileSet), "property profile should not be set yet");
            m_result->setProfile(std::move(value));
            return castState<ProfileSet>();
        }

        ConsoleProfileFinishedNotificationBuilder<STATE>& setTitle(const String& value)
        {
            m_result->setTitle(value);
            return *this;
        }

        std::unique_ptr<ConsoleProfileFinishedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ConsoleProfileFinishedNotification;
        ConsoleProfileFinishedNotificationBuilder() : m_result(new ConsoleProfileFinishedNotification()) { }

        template<int STEP> ConsoleProfileFinishedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ConsoleProfileFinishedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::ConsoleProfileFinishedNotification> m_result;
    };

    static ConsoleProfileFinishedNotificationBuilder<0> create()
    {
        return ConsoleProfileFinishedNotificationBuilder<0>();
    }

private:
    ConsoleProfileFinishedNotification()
    {
    }

    String m_id;
    std::unique_ptr<protocol::Debugger::Location> m_location;
    std::unique_ptr<protocol::Profiler::Profile> m_profile;
    Maybe<String> m_title;
};


class  ConsoleProfileStartedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(ConsoleProfileStartedNotification);
public:
    static std::unique_ptr<ConsoleProfileStartedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~ConsoleProfileStartedNotification() override { }

    String getId() { return m_id; }
    void setId(const String& value) { m_id = value; }

    protocol::Debugger::Location* getLocation() { return m_location.get(); }
    void setLocation(std::unique_ptr<protocol::Debugger::Location> value) { m_location = std::move(value); }

    bool hasTitle() { return m_title.isJust(); }
    String getTitle(const String& defaultValue) { return m_title.isJust() ? m_title.fromJust() : defaultValue; }
    void setTitle(const String& value) { m_title = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<ConsoleProfileStartedNotification> clone() const;

    template<int STATE>
    class ConsoleProfileStartedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            IdSet = 1 << 1,
            LocationSet = 1 << 2,
            AllFieldsSet = (IdSet | LocationSet | 0)};


        ConsoleProfileStartedNotificationBuilder<STATE | IdSet>& setId(const String& value)
        {
            static_assert(!(STATE & IdSet), "property id should not be set yet");
            m_result->setId(value);
            return castState<IdSet>();
        }

        ConsoleProfileStartedNotificationBuilder<STATE | LocationSet>& setLocation(std::unique_ptr<protocol::Debugger::Location> value)
        {
            static_assert(!(STATE & LocationSet), "property location should not be set yet");
            m_result->setLocation(std::move(value));
            return castState<LocationSet>();
        }

        ConsoleProfileStartedNotificationBuilder<STATE>& setTitle(const String& value)
        {
            m_result->setTitle(value);
            return *this;
        }

        std::unique_ptr<ConsoleProfileStartedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class ConsoleProfileStartedNotification;
        ConsoleProfileStartedNotificationBuilder() : m_result(new ConsoleProfileStartedNotification()) { }

        template<int STEP> ConsoleProfileStartedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<ConsoleProfileStartedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::ConsoleProfileStartedNotification> m_result;
    };

    static ConsoleProfileStartedNotificationBuilder<0> create()
    {
        return ConsoleProfileStartedNotificationBuilder<0>();
    }

private:
    ConsoleProfileStartedNotification()
    {
    }

    String m_id;
    std::unique_ptr<protocol::Debugger::Location> m_location;
    Maybe<String> m_title;
};


// ------------- Backend interface.

class  Backend {
public:
    virtual ~Backend() { }

    virtual DispatchResponse disable() = 0;
    virtual DispatchResponse enable() = 0;
    virtual DispatchResponse getBestEffortCoverage(std::unique_ptr<protocol::Array<protocol::Profiler::ScriptCoverage>>* out_result) = 0;
    virtual DispatchResponse setSamplingInterval(int in_interval) = 0;
    virtual DispatchResponse start() = 0;
    virtual DispatchResponse startPreciseCoverage(Maybe<bool> in_callCount, Maybe<bool> in_detailed) = 0;
    virtual DispatchResponse startTypeProfile() = 0;
    virtual DispatchResponse stop(std::unique_ptr<protocol::Profiler::Profile>* out_profile) = 0;
    virtual DispatchResponse stopPreciseCoverage() = 0;
    virtual DispatchResponse stopTypeProfile() = 0;
    virtual DispatchResponse takePreciseCoverage(std::unique_ptr<protocol::Array<protocol::Profiler::ScriptCoverage>>* out_result) = 0;
    virtual DispatchResponse takeTypeProfile(std::unique_ptr<protocol::Array<protocol::Profiler::ScriptTypeProfile>>* out_result) = 0;

};

// ------------- Frontend interface.

class  Frontend {
public:
    explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
    void consoleProfileFinished(const String& id, std::unique_ptr<protocol::Debugger::Location> location, std::unique_ptr<protocol::Profiler::Profile> profile, Maybe<String> title = Maybe<String>());
    void consoleProfileStarted(const String& id, std::unique_ptr<protocol::Debugger::Location> location, Maybe<String> title = Maybe<String>());

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

} // namespace Profiler
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Profiler_h)

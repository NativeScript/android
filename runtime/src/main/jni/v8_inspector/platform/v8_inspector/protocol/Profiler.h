// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef protocol_Profiler_h
#define protocol_Profiler_h

#include "platform/PlatformExport.h"
#include "platform/inspector_protocol/InspectorProtocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "platform/v8_inspector/protocol/Runtime.h"
#include "platform/v8_inspector/protocol/Debugger.h"

namespace blink {
namespace protocol {
namespace Profiler {

// ------------- Forward and enum declarations.
// CPU Profile node. Holds callsite information, execution statistics and child nodes.
class CPUProfileNode;
// Profile.
class CPUProfile;
// Specifies a number of samples attributed to a certain source position.
class PositionTickInfo;

// ------------- Type and builder declarations.

// CPU Profile node. Holds callsite information, execution statistics and child nodes.
class PLATFORM_EXPORT CPUProfileNode {
    PROTOCOL_DISALLOW_COPY(CPUProfileNode);
public:
    static std::unique_ptr<CPUProfileNode> parse(protocol::Value* value, ErrorSupport* errors);

    ~CPUProfileNode() { }

    int getId() { return m_id; }
    void setId(int value) { m_id = value; }

    protocol::Runtime::CallFrame* getCallFrame() { return m_callFrame.get(); }
    void setCallFrame(std::unique_ptr<protocol::Runtime::CallFrame> value) { m_callFrame = std::move(value); }

    int getHitCount() { return m_hitCount; }
    void setHitCount(int value) { m_hitCount = value; }

    bool hasChildren() { return m_children.isJust(); }
    protocol::Array<int>* getChildren(protocol::Array<int>* defaultValue) { return m_children.isJust() ? m_children.fromJust() : defaultValue; }
    void setChildren(std::unique_ptr<protocol::Array<int>> value) { m_children = std::move(value); }

    bool hasDeoptReason() { return m_deoptReason.isJust(); }
    String16 getDeoptReason(const String16& defaultValue) { return m_deoptReason.isJust() ? m_deoptReason.fromJust() : defaultValue; }
    void setDeoptReason(const String16& value) { m_deoptReason = value; }

    bool hasPositionTicks() { return m_positionTicks.isJust(); }
    protocol::Array<protocol::Profiler::PositionTickInfo>* getPositionTicks(protocol::Array<protocol::Profiler::PositionTickInfo>* defaultValue) { return m_positionTicks.isJust() ? m_positionTicks.fromJust() : defaultValue; }
    void setPositionTicks(std::unique_ptr<protocol::Array<protocol::Profiler::PositionTickInfo>> value) { m_positionTicks = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<CPUProfileNode> clone() const;

    template<int STATE>
    class CPUProfileNodeBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          IdSet = 1 << 1,
          CallFrameSet = 1 << 2,
          HitCountSet = 1 << 3,
            AllFieldsSet = (IdSet | CallFrameSet | HitCountSet | 0)};


        CPUProfileNodeBuilder<STATE | IdSet>& setId(int value)
        {
            static_assert(!(STATE & IdSet), "property id should not be set yet");
            m_result->setId(value);
            return castState<IdSet>();
        }

        CPUProfileNodeBuilder<STATE | CallFrameSet>& setCallFrame(std::unique_ptr<protocol::Runtime::CallFrame> value)
        {
            static_assert(!(STATE & CallFrameSet), "property callFrame should not be set yet");
            m_result->setCallFrame(std::move(value));
            return castState<CallFrameSet>();
        }

        CPUProfileNodeBuilder<STATE | HitCountSet>& setHitCount(int value)
        {
            static_assert(!(STATE & HitCountSet), "property hitCount should not be set yet");
            m_result->setHitCount(value);
            return castState<HitCountSet>();
        }

        CPUProfileNodeBuilder<STATE>& setChildren(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setChildren(std::move(value));
            return *this;
        }

        CPUProfileNodeBuilder<STATE>& setDeoptReason(const String16& value)
        {
            m_result->setDeoptReason(value);
            return *this;
        }

        CPUProfileNodeBuilder<STATE>& setPositionTicks(std::unique_ptr<protocol::Array<protocol::Profiler::PositionTickInfo>> value)
        {
            m_result->setPositionTicks(std::move(value));
            return *this;
        }

        std::unique_ptr<CPUProfileNode> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class CPUProfileNode;
        CPUProfileNodeBuilder() : m_result(new CPUProfileNode()) { }

        template<int STEP> CPUProfileNodeBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<CPUProfileNodeBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::CPUProfileNode> m_result;
    };

    static CPUProfileNodeBuilder<0> create()
    {
        return CPUProfileNodeBuilder<0>();
    }

private:
    CPUProfileNode()
    {
          m_id = 0;
          m_hitCount = 0;
    }

    int m_id;
    std::unique_ptr<protocol::Runtime::CallFrame> m_callFrame;
    int m_hitCount;
    Maybe<protocol::Array<int>> m_children;
    Maybe<String16> m_deoptReason;
    Maybe<protocol::Array<protocol::Profiler::PositionTickInfo>> m_positionTicks;
};


// Profile.
class PLATFORM_EXPORT CPUProfile {
    PROTOCOL_DISALLOW_COPY(CPUProfile);
public:
    static std::unique_ptr<CPUProfile> parse(protocol::Value* value, ErrorSupport* errors);

    ~CPUProfile() { }

    protocol::Array<protocol::Profiler::CPUProfileNode>* getNodes() { return m_nodes.get(); }
    void setNodes(std::unique_ptr<protocol::Array<protocol::Profiler::CPUProfileNode>> value) { m_nodes = std::move(value); }

    double getStartTime() { return m_startTime; }
    void setStartTime(double value) { m_startTime = value; }

    double getEndTime() { return m_endTime; }
    void setEndTime(double value) { m_endTime = value; }

    bool hasSamples() { return m_samples.isJust(); }
    protocol::Array<int>* getSamples(protocol::Array<int>* defaultValue) { return m_samples.isJust() ? m_samples.fromJust() : defaultValue; }
    void setSamples(std::unique_ptr<protocol::Array<int>> value) { m_samples = std::move(value); }

    bool hasTimestampDeltas() { return m_timestampDeltas.isJust(); }
    protocol::Array<int>* getTimestampDeltas(protocol::Array<int>* defaultValue) { return m_timestampDeltas.isJust() ? m_timestampDeltas.fromJust() : defaultValue; }
    void setTimestampDeltas(std::unique_ptr<protocol::Array<int>> value) { m_timestampDeltas = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<CPUProfile> clone() const;

    template<int STATE>
    class CPUProfileBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          NodesSet = 1 << 1,
          StartTimeSet = 1 << 2,
          EndTimeSet = 1 << 3,
            AllFieldsSet = (NodesSet | StartTimeSet | EndTimeSet | 0)};


        CPUProfileBuilder<STATE | NodesSet>& setNodes(std::unique_ptr<protocol::Array<protocol::Profiler::CPUProfileNode>> value)
        {
            static_assert(!(STATE & NodesSet), "property nodes should not be set yet");
            m_result->setNodes(std::move(value));
            return castState<NodesSet>();
        }

        CPUProfileBuilder<STATE | StartTimeSet>& setStartTime(double value)
        {
            static_assert(!(STATE & StartTimeSet), "property startTime should not be set yet");
            m_result->setStartTime(value);
            return castState<StartTimeSet>();
        }

        CPUProfileBuilder<STATE | EndTimeSet>& setEndTime(double value)
        {
            static_assert(!(STATE & EndTimeSet), "property endTime should not be set yet");
            m_result->setEndTime(value);
            return castState<EndTimeSet>();
        }

        CPUProfileBuilder<STATE>& setSamples(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setSamples(std::move(value));
            return *this;
        }

        CPUProfileBuilder<STATE>& setTimestampDeltas(std::unique_ptr<protocol::Array<int>> value)
        {
            m_result->setTimestampDeltas(std::move(value));
            return *this;
        }

        std::unique_ptr<CPUProfile> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class CPUProfile;
        CPUProfileBuilder() : m_result(new CPUProfile()) { }

        template<int STEP> CPUProfileBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<CPUProfileBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Profiler::CPUProfile> m_result;
    };

    static CPUProfileBuilder<0> create()
    {
        return CPUProfileBuilder<0>();
    }

private:
    CPUProfile()
    {
          m_startTime = 0;
          m_endTime = 0;
    }

    std::unique_ptr<protocol::Array<protocol::Profiler::CPUProfileNode>> m_nodes;
    double m_startTime;
    double m_endTime;
    Maybe<protocol::Array<int>> m_samples;
    Maybe<protocol::Array<int>> m_timestampDeltas;
};


// Specifies a number of samples attributed to a certain source position.
class PLATFORM_EXPORT PositionTickInfo {
    PROTOCOL_DISALLOW_COPY(PositionTickInfo);
public:
    static std::unique_ptr<PositionTickInfo> parse(protocol::Value* value, ErrorSupport* errors);

    ~PositionTickInfo() { }

    int getLine() { return m_line; }
    void setLine(int value) { m_line = value; }

    int getTicks() { return m_ticks; }
    void setTicks(int value) { m_ticks = value; }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
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


// ------------- Backend interface.

class PLATFORM_EXPORT Backend {
public:
    virtual void enable(ErrorString*) = 0;
    virtual void disable(ErrorString*) = 0;
    virtual void setSamplingInterval(ErrorString*, int in_interval) = 0;
    virtual void start(ErrorString*) = 0;
    virtual void stop(ErrorString*, std::unique_ptr<protocol::Profiler::CPUProfile>* out_profile) = 0;


protected:
    virtual ~Backend() { }
};

// ------------- Frontend interface.

class PLATFORM_EXPORT Frontend {
public:
    Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
    void consoleProfileStarted(const String16& id, std::unique_ptr<protocol::Debugger::Location> location, const Maybe<String16>& title = Maybe<String16>());
    void consoleProfileFinished(const String16& id, std::unique_ptr<protocol::Debugger::Location> location, std::unique_ptr<protocol::Profiler::CPUProfile> profile, const Maybe<String16>& title = Maybe<String16>());

    void flush() { m_frontendChannel->flushProtocolNotifications(); }
private:
    FrontendChannel* m_frontendChannel;
};

// ------------- Dispatcher.

class PLATFORM_EXPORT Dispatcher {
public:
    static void wire(UberDispatcher*, blink::protocol::Profiler::Backend*);

private:
    Dispatcher() { }
};

// ------------- Metainfo.

class PLATFORM_EXPORT Metainfo {
public:
    using BackendClass = Backend;
    using FrontendClass = Frontend;
    using DispatcherClass = Dispatcher;
    static const char domainName[];
    static const char commandPrefix[];
    static const char version[];
};

} // namespace Profiler
} // namespace protocol
} // namespace blink

#endif // !defined(protocol_Profiler_h)

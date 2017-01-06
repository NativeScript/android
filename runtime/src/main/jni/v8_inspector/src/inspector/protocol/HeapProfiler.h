// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_HeapProfiler_h
#define v8_inspector_protocol_HeapProfiler_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "src/inspector/protocol/Runtime.h"

namespace v8_inspector {
namespace protocol {
namespace HeapProfiler {

// ------------- Forward and enum declarations.
// Heap snapshot object id.
using HeapSnapshotObjectId = String;
// Sampling Heap Profile node. Holds callsite information, allocation statistics and child nodes.
class SamplingHeapProfileNode;
// Profile.
class SamplingHeapProfile;

// ------------- Type and builder declarations.

// Sampling Heap Profile node. Holds callsite information, allocation statistics and child nodes.
class  SamplingHeapProfileNode {
    PROTOCOL_DISALLOW_COPY(SamplingHeapProfileNode);
public:
    static std::unique_ptr<SamplingHeapProfileNode> parse(protocol::Value* value, ErrorSupport* errors);

    ~SamplingHeapProfileNode() { }

    protocol::Runtime::CallFrame* getCallFrame() { return m_callFrame.get(); }
    void setCallFrame(std::unique_ptr<protocol::Runtime::CallFrame> value) { m_callFrame = std::move(value); }

    double getSelfSize() { return m_selfSize; }
    void setSelfSize(double value) { m_selfSize = value; }

    protocol::Array<protocol::HeapProfiler::SamplingHeapProfileNode>* getChildren() { return m_children.get(); }
    void setChildren(std::unique_ptr<protocol::Array<protocol::HeapProfiler::SamplingHeapProfileNode>> value) { m_children = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<SamplingHeapProfileNode> clone() const;

    template<int STATE>
    class SamplingHeapProfileNodeBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          CallFrameSet = 1 << 1,
          SelfSizeSet = 1 << 2,
          ChildrenSet = 1 << 3,
            AllFieldsSet = (CallFrameSet | SelfSizeSet | ChildrenSet | 0)};


        SamplingHeapProfileNodeBuilder<STATE | CallFrameSet>& setCallFrame(std::unique_ptr<protocol::Runtime::CallFrame> value)
        {
            static_assert(!(STATE & CallFrameSet), "property callFrame should not be set yet");
            m_result->setCallFrame(std::move(value));
            return castState<CallFrameSet>();
        }

        SamplingHeapProfileNodeBuilder<STATE | SelfSizeSet>& setSelfSize(double value)
        {
            static_assert(!(STATE & SelfSizeSet), "property selfSize should not be set yet");
            m_result->setSelfSize(value);
            return castState<SelfSizeSet>();
        }

        SamplingHeapProfileNodeBuilder<STATE | ChildrenSet>& setChildren(std::unique_ptr<protocol::Array<protocol::HeapProfiler::SamplingHeapProfileNode>> value)
        {
            static_assert(!(STATE & ChildrenSet), "property children should not be set yet");
            m_result->setChildren(std::move(value));
            return castState<ChildrenSet>();
        }

        std::unique_ptr<SamplingHeapProfileNode> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class SamplingHeapProfileNode;
        SamplingHeapProfileNodeBuilder() : m_result(new SamplingHeapProfileNode()) { }

        template<int STEP> SamplingHeapProfileNodeBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<SamplingHeapProfileNodeBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::HeapProfiler::SamplingHeapProfileNode> m_result;
    };

    static SamplingHeapProfileNodeBuilder<0> create()
    {
        return SamplingHeapProfileNodeBuilder<0>();
    }

private:
    SamplingHeapProfileNode()
    {
          m_selfSize = 0;
    }

    std::unique_ptr<protocol::Runtime::CallFrame> m_callFrame;
    double m_selfSize;
    std::unique_ptr<protocol::Array<protocol::HeapProfiler::SamplingHeapProfileNode>> m_children;
};


// Profile.
class  SamplingHeapProfile {
    PROTOCOL_DISALLOW_COPY(SamplingHeapProfile);
public:
    static std::unique_ptr<SamplingHeapProfile> parse(protocol::Value* value, ErrorSupport* errors);

    ~SamplingHeapProfile() { }

    protocol::HeapProfiler::SamplingHeapProfileNode* getHead() { return m_head.get(); }
    void setHead(std::unique_ptr<protocol::HeapProfiler::SamplingHeapProfileNode> value) { m_head = std::move(value); }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<SamplingHeapProfile> clone() const;

    template<int STATE>
    class SamplingHeapProfileBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          HeadSet = 1 << 1,
            AllFieldsSet = (HeadSet | 0)};


        SamplingHeapProfileBuilder<STATE | HeadSet>& setHead(std::unique_ptr<protocol::HeapProfiler::SamplingHeapProfileNode> value)
        {
            static_assert(!(STATE & HeadSet), "property head should not be set yet");
            m_result->setHead(std::move(value));
            return castState<HeadSet>();
        }

        std::unique_ptr<SamplingHeapProfile> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class SamplingHeapProfile;
        SamplingHeapProfileBuilder() : m_result(new SamplingHeapProfile()) { }

        template<int STEP> SamplingHeapProfileBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<SamplingHeapProfileBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::HeapProfiler::SamplingHeapProfile> m_result;
    };

    static SamplingHeapProfileBuilder<0> create()
    {
        return SamplingHeapProfileBuilder<0>();
    }

private:
    SamplingHeapProfile()
    {
    }

    std::unique_ptr<protocol::HeapProfiler::SamplingHeapProfileNode> m_head;
};


// ------------- Backend interface.

class  Backend {
public:
    virtual ~Backend() { }

    virtual void enable(ErrorString*) = 0;
    virtual void disable(ErrorString*) = 0;
    virtual void startTrackingHeapObjects(ErrorString*, const Maybe<bool>& in_trackAllocations) = 0;
    virtual void stopTrackingHeapObjects(ErrorString*, const Maybe<bool>& in_reportProgress) = 0;
    virtual void takeHeapSnapshot(ErrorString*, const Maybe<bool>& in_reportProgress) = 0;
    virtual void collectGarbage(ErrorString*) = 0;
    virtual void getObjectByHeapObjectId(ErrorString*, const String& in_objectId, const Maybe<String>& in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_result) = 0;
    virtual void addInspectedHeapObject(ErrorString*, const String& in_heapObjectId) = 0;
    virtual void getHeapObjectId(ErrorString*, const String& in_objectId, String* out_heapSnapshotObjectId) = 0;
    virtual void startSampling(ErrorString*, const Maybe<double>& in_samplingInterval) = 0;
    virtual void stopSampling(ErrorString*, std::unique_ptr<protocol::HeapProfiler::SamplingHeapProfile>* out_profile) = 0;

};

// ------------- Frontend interface.

class  Frontend {
public:
    Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
    void addHeapSnapshotChunk(const String& chunk);
    void resetProfiles();
    void reportHeapSnapshotProgress(int done, int total, const Maybe<bool>& finished = Maybe<bool>());
    void lastSeenObjectId(int lastSeenObjectId, double timestamp);
    void heapStatsUpdate(std::unique_ptr<protocol::Array<int>> statsUpdate);

    void flush();
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

} // namespace HeapProfiler
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_HeapProfiler_h)

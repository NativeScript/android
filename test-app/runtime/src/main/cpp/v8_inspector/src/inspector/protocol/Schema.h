// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Schema_h
#define v8_inspector_protocol_Schema_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "include/inspector/Schema.h"

namespace v8_inspector {
namespace protocol {
namespace Schema {

// ------------- Forward and enum declarations.
class Domain;

// ------------- Type and builder declarations.

class  Domain : public Serializable, public API::Domain{
    PROTOCOL_DISALLOW_COPY(Domain);
public:
    static std::unique_ptr<Domain> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~Domain() override { }

    String getName() { return m_name; }
    void setName(const String& value) { m_name = value; }

    String getVersion() { return m_version; }
    void setVersion(const String& value) { m_version = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serialize() override { return toValue()->serialize(); }
    std::unique_ptr<Domain> clone() const;
    std::unique_ptr<StringBuffer> toJSONString() const override;

    template<int STATE>
    class DomainBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NameSet = 1 << 1,
            VersionSet = 1 << 2,
            AllFieldsSet = (NameSet | VersionSet | 0)};


        DomainBuilder<STATE | NameSet>& setName(const String& value)
        {
            static_assert(!(STATE & NameSet), "property name should not be set yet");
            m_result->setName(value);
            return castState<NameSet>();
        }

        DomainBuilder<STATE | VersionSet>& setVersion(const String& value)
        {
            static_assert(!(STATE & VersionSet), "property version should not be set yet");
            m_result->setVersion(value);
            return castState<VersionSet>();
        }

        std::unique_ptr<Domain> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class Domain;
        DomainBuilder() : m_result(new Domain()) { }

        template<int STEP> DomainBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<DomainBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Schema::Domain> m_result;
    };

    static DomainBuilder<0> create()
    {
        return DomainBuilder<0>();
    }

private:
    Domain()
    {
    }

    String m_name;
    String m_version;
};


// ------------- Backend interface.

class  Backend {
public:
    virtual ~Backend() { }

    virtual DispatchResponse getDomains(std::unique_ptr<protocol::Array<protocol::Schema::Domain>>* out_domains) = 0;

    virtual DispatchResponse disable()
    {
        return DispatchResponse::OK();
    }
};

// ------------- Frontend interface.

class  Frontend {
public:
    explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }

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

} // namespace Schema
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Schema_h)

// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef protocol_Schema_h
#define protocol_Schema_h

#include "platform/PlatformExport.h"
#include "platform/inspector_protocol/InspectorProtocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "platform/v8_inspector/public/protocol/Schema.h"

namespace blink {
namespace protocol {
namespace Schema {

// ------------- Forward and enum declarations.
// Description of the protocol domain.
class Domain;

// ------------- Type and builder declarations.

// Description of the protocol domain.
class PLATFORM_EXPORT Domain : public API::Domain {
    PROTOCOL_DISALLOW_COPY(Domain);
public:
    static std::unique_ptr<Domain> parse(protocol::Value* value, ErrorSupport* errors);

    ~Domain() { }

    String16 getName() { return m_name; }
    void setName(const String16& value) { m_name = value; }

    String16 getVersion() { return m_version; }
    void setVersion(const String16& value) { m_version = value; }

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<Domain> clone() const;
    String16 toJSONString() const override;

    template<int STATE>
    class DomainBuilder {
    public:
        enum {
            NoFieldsSet = 0,
          NameSet = 1 << 1,
          VersionSet = 1 << 2,
            AllFieldsSet = (NameSet | VersionSet | 0)};


        DomainBuilder<STATE | NameSet>& setName(const String16& value)
        {
            static_assert(!(STATE & NameSet), "property name should not be set yet");
            m_result->setName(value);
            return castState<NameSet>();
        }

        DomainBuilder<STATE | VersionSet>& setVersion(const String16& value)
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

    String16 m_name;
    String16 m_version;
};


// ------------- Backend interface.

class PLATFORM_EXPORT Backend {
public:
    virtual void getDomains(ErrorString*, std::unique_ptr<protocol::Array<protocol::Schema::Domain>>* out_domains) = 0;

    virtual void disable(ErrorString*) { }

protected:
    virtual ~Backend() { }
};

// ------------- Frontend interface.

class PLATFORM_EXPORT Frontend {
public:
    Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }

    void flush() { m_frontendChannel->flushProtocolNotifications(); }
private:
    FrontendChannel* m_frontendChannel;
};

// ------------- Dispatcher.

class PLATFORM_EXPORT Dispatcher {
public:
    static void wire(UberDispatcher*, blink::protocol::Schema::Backend*);

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

} // namespace Schema
} // namespace protocol
} // namespace blink

#endif // !defined(protocol_Schema_h)

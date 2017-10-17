// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_GenericTypes_h
#define v8_inspector_protocol_GenericTypes_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.

namespace v8_inspector {
namespace protocol {
namespace GenericTypes {

// ------------- Forward and enum declarations.
// Search match in a resource.
class SearchMatch;

// ------------- Type and builder declarations.

// Search match in a resource.
class  SearchMatch {
        PROTOCOL_DISALLOW_COPY(SearchMatch);
    public:
        static std::unique_ptr<SearchMatch> parse(protocol::Value* value, ErrorSupport* errors);

        ~SearchMatch() { }

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

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<SearchMatch> clone() const;

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

                std::unique_ptr<protocol::GenericTypes::SearchMatch> m_result;
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


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }


        virtual void disable(ErrorString*) { }
};

// ------------- Frontend interface.

class  Frontend {
    public:
        Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }

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

} // namespace GenericTypes
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_GenericTypes_h)

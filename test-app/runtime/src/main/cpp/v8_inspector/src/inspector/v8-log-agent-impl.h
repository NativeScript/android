//
// Created by pkanev on 12/5/2017.
//

#ifndef V8_LOG_AGENT_IMPL_H
#define V8_LOG_AGENT_IMPL_H

#include <v8_inspector/src/inspector/protocol/Log.h>
#include <Util.h>
#include <chrono>


namespace v8_inspector {

class V8InspectorSessionImpl;

using protocol::ErrorString;
using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;


class V8LogAgentImpl : public protocol::Log::Backend {
    public:
        V8LogAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                       protocol::DictionaryValue* state);

        ~V8LogAgentImpl() override;
        void enable(ErrorString*) override;
        void disable(ErrorString*) override;
        void clear(ErrorString*) override;
        void startViolationsReport(ErrorString*, std::unique_ptr<protocol::Array<protocol::Log::ViolationSetting>> in_config) override;
        void stopViolationsReport(ErrorString*) override;

        static void EntryAdded(const std::string& text, std::string verbosityLevel, std::string url, int lineNumber);

        static V8LogAgentImpl* Instance;
        protocol::Log::Frontend m_frontend;

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        bool m_enabled;
};

}

#endif //V8_LOG_AGENT_IMPL_H

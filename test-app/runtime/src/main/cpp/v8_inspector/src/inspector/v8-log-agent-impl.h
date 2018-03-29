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

using v8_inspector::protocol::Maybe;
using String = v8_inspector::String16;
using v8_inspector::protocol::DispatchResponse;


class V8LogAgentImpl : public protocol::Log::Backend {
    public:
        V8LogAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                       protocol::DictionaryValue* state);

        ~V8LogAgentImpl() override;
        DispatchResponse enable() override;
        DispatchResponse disable() override;
        DispatchResponse clear() override;
        DispatchResponse startViolationsReport(std::unique_ptr<protocol::Array<protocol::Log::ViolationSetting>> in_config) override;
        DispatchResponse stopViolationsReport() override;

        static void EntryAdded(const std::string& text, std::string verbosityLevel, std::string url, int lineNumber);

    private:
        V8InspectorSessionImpl* m_session;
        protocol::DictionaryValue* m_state;
        protocol::Log::Frontend m_frontend;
        bool m_enabled;

        static V8LogAgentImpl* Instance;
};

}

#endif //V8_LOG_AGENT_IMPL_H

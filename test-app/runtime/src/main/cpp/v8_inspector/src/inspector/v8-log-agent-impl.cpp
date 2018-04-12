//
// Created by pkanev on 12/5/2017.
//

#include <NativeScriptAssert.h>
#include <ArgConverter.h>
#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
#include "v8-log-agent-impl.h"

namespace v8_inspector {

using tns::ArgConverter;

namespace LogAgentState {
static const char logEnabled[] = "logEnabled";
}

V8LogAgentImpl::V8LogAgentImpl(V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
                               protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

V8LogAgentImpl::~V8LogAgentImpl() { }

DispatchResponse V8LogAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::Error("Log Agent already enabled!");
    }

    m_state->setBoolean(LogAgentState::logEnabled, true);
    m_enabled = true;

    return DispatchResponse::OK();
}

DispatchResponse V8LogAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::OK();
    }

    m_state->setBoolean(LogAgentState::logEnabled, false);

    m_enabled = false;

    return DispatchResponse::OK();
}

DispatchResponse V8LogAgentImpl::startViolationsReport(std::unique_ptr<protocol::Array<protocol::Log::ViolationSetting>> in_config) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8LogAgentImpl::stopViolationsReport() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8LogAgentImpl::clear() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

void V8LogAgentImpl::EntryAdded(const std::string& text, std::string verbosityLevel, std::string url, int lineNumber) {
    auto logAgentInstance = V8LogAgentImpl::Instance;

    if (!logAgentInstance) {
        return;
    }

    auto nano = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    double timestamp = nano.time_since_epoch().count();

    auto textString16 = String16(text.data());

    auto logEntry = protocol::Log::LogEntry::create()
                    .setSource(protocol::Log::LogEntry::SourceEnum::Javascript)
                    .setText(textString16)
                    .setLevel(verbosityLevel.c_str())
                    .setTimestamp(timestamp)
                    .setUrl(url.c_str())
                    .setLineNumber(lineNumber - 1)
                    .build();

    logAgentInstance->m_frontend.entryAdded(std::move(logEntry));
}

V8LogAgentImpl* V8LogAgentImpl::Instance = nullptr;
}


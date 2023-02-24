//
// Created by pkanev on 12/5/2017.
//

#include <NativeScriptAssert.h>
#include <ArgConverter.h>

#include "LogAgentImpl.h"
#include "utils/InspectorCommon.h"

namespace tns {

namespace LogAgentState {
static const char logEnabled[] = "logEnabled";
}

LogAgentImpl::LogAgentImpl(V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
                           protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

LogAgentImpl::~LogAgentImpl() { }

DispatchResponse LogAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::ServerError("Log Agent already enabled!");
    }

    m_state->setBoolean(LogAgentState::logEnabled, true);
    m_enabled = true;

    return DispatchResponse::Success();
}

DispatchResponse LogAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::Success();
    }

    m_state->setBoolean(LogAgentState::logEnabled, false);

    m_enabled = false;

    return DispatchResponse::Success();
}

DispatchResponse LogAgentImpl::startViolationsReport(std::unique_ptr<protocol::Array<protocol::Log::ViolationSetting>> in_config) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse LogAgentImpl::stopViolationsReport() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse LogAgentImpl::clear() {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

void LogAgentImpl::EntryAdded(const std::string& text, std::string verbosityLevel, std::string url, int lineNumber) {
    auto logAgentInstance = LogAgentImpl::Instance;

    if (!logAgentInstance) {
        return;
    }

    auto nano = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    double timestamp = nano.time_since_epoch().count();

    auto vector = utils::Common::toVector(text);
    auto textString16 = protocol::String(vector.data(), vector.size());

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

LogAgentImpl* LogAgentImpl::Instance = nullptr;
}


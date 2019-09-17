// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "NSV8DebuggerAgentImpl.h"

namespace v8_inspector {

NSV8DebuggerAgentImpl::NSV8DebuggerAgentImpl(
    V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
    protocol::DictionaryValue* state)
    : V8DebuggerAgentImpl(session, frontendChannel, state),
    m_env(tns::JEnv()){

    auto runtimeClass = m_env.FindClass("com/tns/Runtime");
    assert(runtimeClass != nullptr);

    auto getLineBreakpointsEnabledMethodID = m_env.GetStaticMethodID(runtimeClass, "getLineBreakpointsEnabled", "()Z");
    assert(getLineBreakpointsEnabledMethodID != nullptr);

    auto lineBreakpointsEnabled = m_env.CallStaticBooleanMethod(runtimeClass, getLineBreakpointsEnabledMethodID);
    m_lineBreakpointsEnabled = lineBreakpointsEnabled == JNI_TRUE;
}

Response NSV8DebuggerAgentImpl::getPossibleBreakpoints(
    std::unique_ptr<protocol::Debugger::Location> start,
    Maybe<protocol::Debugger::Location> end, Maybe<bool> restrictToFunction,
    std::unique_ptr<protocol::Array<protocol::Debugger::BreakLocation>>*
        locations) {
    if(m_lineBreakpointsEnabled) {
        return V8DebuggerAgentImpl::getPossibleBreakpoints(std::move(start), std::move(end), std::move(restrictToFunction), locations);
    } else {
        *locations = std::make_unique<protocol::Array<protocol::Debugger::BreakLocation>>();
        return Response::OK();
    }
}

}  // namespace v8_inspector

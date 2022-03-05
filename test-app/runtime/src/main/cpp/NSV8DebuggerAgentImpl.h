// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_INSPECTOR_NS_V8_DEBUGGER_AGENT_IMPL_H_
#define V8_INSPECTOR_NS_V8_DEBUGGER_AGENT_IMPL_H_

#include <JEnv.h>
#include "src/inspector/v8-debugger-agent-impl.h"

namespace v8_inspector {

    class NSV8DebuggerAgentImpl : public V8DebuggerAgentImpl {
    public:
        NSV8DebuggerAgentImpl(V8InspectorSessionImpl *, protocol::FrontendChannel *,
                              protocol::DictionaryValue *state);

        Response getPossibleBreakpoints(
                std::unique_ptr<protocol::Debugger::Location> start,
                Maybe<protocol::Debugger::Location> end, Maybe<bool> restrictToFunction,
                std::unique_ptr<protocol::Array<protocol::Debugger::BreakLocation>> *
                locations) override;
        DISALLOW_COPY_AND_ASSIGN(NSV8DebuggerAgentImpl);
    private:
        tns::JEnv m_env;
        bool m_lineBreakpointsEnabled;
    };
}
#endif  // V8_INSPECTOR_NS_V8_DEBUGGER_AGENT_IMPL_H_

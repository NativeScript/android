#ifndef JSDEBUGGER_H_
#define JSDEBUGGER_H_

#include <string>
#include "JEnv.h"

namespace tns
{
	class JsDebugger
	{
	public:
		static void Init(const std::string& packageName, int port);

		static void ProcessDebugMessages();

		static bool EnableAgent(const std::string& name, int port, bool waitForConnection);

		static void DisableAgent();

		static int GetDebuggerPort();

		static int GetCurrentDebuggerPort();

		static std::string GetPackageName();

	private:
		JsDebugger();

		static void DispatchMessagesDebugAgentCallback();

		static int s_port;
		static int s_currentPort;
		static std::string s_packageName;
		static jclass s_JsDebuggerClass;
		static jmethodID s_DispatchMessagesDebugAgentCallback;

		static const int INVALID_PORT = -1;
	};
}


#endif /* JSDEBUGGER_H_ */

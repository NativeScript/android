#ifndef JSDEBUGGER_H_
#define JSDEBUGGER_H_

#include <string>
#include "v8.h"
#include "v8-debug.h"
#include "JEnv.h"

namespace tns
{
	class JsDebugger
	{
	public:
		static void Init(v8::Isolate *isolate, const std::string& packageName);

		static void ProcessDebugMessages();

		static void Enable();

		static void Disable();

		static void DebugBreak();

		static int GetDebuggerPort();

		static std::string GetPackageName();

		static void SendCommand(uint16_t *cmd, int length);

	private:
		JsDebugger();

		static void MyMessageHandler(const v8::Debug::Message& message);

		static int s_port;
		static std::string s_packageName;
		static jclass s_JsDebuggerClass;
		static jmethodID s_EnqueueMessage;
		static v8::Isolate *s_isolate;

		static const int INVALID_PORT = -1;
	};
}


#endif /* JSDEBUGGER_H_ */

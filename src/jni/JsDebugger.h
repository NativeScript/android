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
		static void Init(v8::Isolate *isolate, const std::string& packageName, jobject jsDebugger);

		static void ProcessDebugMessages();

		static void Enable();

		static void Disable();

		static void DebugBreak();

		static std::string GetPackageName();

		static void SendCommand(uint16_t *cmd, int length);

		static void DebugBreakCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

	private:
		JsDebugger();

		static void MyMessageHandler(const v8::Debug::Message& message);

		static std::string s_packageName;
		static jclass s_JsDebuggerClass;
		static jobject s_jsDebugger;
		static jmethodID s_EnqueueMessage;
		static jmethodID s_EnableAgent;
		static v8::Isolate *s_isolate;

		static const int INVALID_PORT = -1;
	};
}


#endif /* JSDEBUGGER_H_ */

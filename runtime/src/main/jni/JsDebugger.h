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

			static bool IsDebuggerActive();

			static void SendCommand(JNIEnv *_env, jobject obj, jbyteArray command, jint length);

			static void DebugBreakCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void ConsoleMessageCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void ConsoleMessage(const std::string& message, const std::string& level = "log", const std::string& srcFileName = "",
					const int lineNumber = 0, const int columnNumber = 0);

		private:
			JsDebugger();

			static void MyMessageHandler(const v8::Debug::Message& message);

			static void SendCommandToV8(uint16_t *cmd, int length);

			static void ConsoleMessage(const v8::FunctionCallbackInfo<v8::Value>& args);

			static bool enabled;
			static jclass s_JsDebuggerClass;
			static jobject s_jsDebugger;
			static jmethodID s_EnqueueMessage;
			static jmethodID s_EnqueueConsoleMessage;
			static jmethodID s_EnableAgent;
			static v8::Isolate *s_isolate;
			static bool s_processedCommands;
	};
}

#endif /* JSDEBUGGER_H_ */

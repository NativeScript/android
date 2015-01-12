#include "JsDebugger.h"
#include "v8-debug.h"
#include <assert.h>
#include <android/log.h>

using namespace std;
using namespace tns;

JsDebugger::JsDebugger()
{
}

void JsDebugger::Init(const string& packageName, int port)
{
	s_packageName = packageName;
	s_port = port;

	JEnv env;
	s_JsDebuggerClass = env.FindClass("com/tns/JsDebugger");
	assert(s_JsDebuggerClass != nullptr);
	s_DispatchMessagesDebugAgentCallback = env.GetStaticMethodID(s_JsDebuggerClass, "dispatchMessagesDebugAgentCallback", "()Z");
	assert(s_DispatchMessagesDebugAgentCallback != nullptr);

	v8::Debug::SetDebugMessageDispatchHandler(DispatchMessagesDebugAgentCallback);
}

int JsDebugger::GetDebuggerPort()
{
	return s_port;
}

int JsDebugger::GetCurrentDebuggerPort()
{
	return s_currentPort;
}

string JsDebugger::GetPackageName()
{
	return s_packageName;
}

void JsDebugger::DispatchMessagesDebugAgentCallback()
{
	JEnv env;
	jboolean success = env.CallStaticBooleanMethod(s_JsDebuggerClass, s_DispatchMessagesDebugAgentCallback);
	assert(JNI_TRUE == success);
}

void JsDebugger::ProcessDebugMessages()
{
	v8::Debug::ProcessDebugMessages();
}

bool JsDebugger::EnableAgent(const std::string& name, int port, bool waitForConnection)
{
	bool success = v8::Debug::EnableAgent(name.c_str(), port, waitForConnection);
	__android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "Enable V8 debugger (app=%s, port=%d, waitForConnection=%d, success=%d)", name.c_str(), port, waitForConnection, success);
	s_currentPort = success ? port : INVALID_PORT;
	return success;
}

void JsDebugger::DisableAgent()
{
	v8::Debug::DisableAgent();
	s_currentPort = INVALID_PORT;
}

int JsDebugger::s_port = JsDebugger::INVALID_PORT;
int JsDebugger::s_currentPort = JsDebugger::INVALID_PORT;
string JsDebugger::s_packageName = "";
jclass JsDebugger::s_JsDebuggerClass = nullptr;
jmethodID JsDebugger::s_DispatchMessagesDebugAgentCallback = nullptr;


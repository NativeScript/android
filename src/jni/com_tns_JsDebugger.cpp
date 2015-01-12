#include "JEnv.h"
#include "JsDebugger.h"
#include "ArgConverter.h"
#include <string>

extern "C" void Java_com_tns_JsDebugger_processDebugMessages(JNIEnv *env, jobject obj)
{
	tns::JsDebugger::ProcessDebugMessages();
}

extern "C" jboolean Java_com_tns_JsDebugger_enableAgentNative(JNIEnv *env, jobject obj, jstring packageName, jint port, jboolean waitForConnetion)
{
	bool wait = JNI_TRUE == waitForConnetion;
	std::string pckName = tns::ArgConverter::jstringToString(packageName);
	bool ret = tns::JsDebugger::EnableAgent(pckName, port, wait);
	return ret ? JNI_TRUE : JNI_FALSE;
}

extern "C" void Java_com_tns_JsDebugger_disableAgentNative(JNIEnv *env, jobject obj)
{
	tns::JsDebugger::DisableAgent();
}

extern "C" jint Java_com_tns_JsDebugger_getCurrentDebuggerPort(JNIEnv *env, jobject obj)
{
	int port = tns::JsDebugger::GetCurrentDebuggerPort();
	return port;
}

#include "JEnv.h"
#include "JsDebugger.h"
#include "ArgConverter.h"
#include <string>
#include <android/log.h>

extern "C" void Java_com_tns_JsDebugger_processDebugMessages(JNIEnv *env, jobject obj)
{
	tns::JsDebugger::ProcessDebugMessages();
}

extern "C" void Java_com_tns_JsDebugger_enable(JNIEnv *env, jobject obj)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.JsDebugger", "Java_com_tns_JsDebugger_enable called");
	tns::JsDebugger::Enable();
}

extern "C" void Java_com_tns_JsDebugger_disable(JNIEnv *env, jobject obj)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.JsDebugger", "Java_com_tns_JsDebugger_disable called");
	tns::JsDebugger::Disable();
}

extern "C" void Java_com_tns_JsDebugger_debugBreak(JNIEnv *env, jobject obj)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.JsDebugger", "Java_com_tns_JsDebugger_debugBreak called");
	tns::JsDebugger::DebugBreak();
}


extern "C" void Java_com_tns_JsDebugger_sendCommand(JNIEnv *_env, jobject obj, jbyteArray command, jint length)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.JsDebugger", "Java_com_tns_JsDebugger_sendCommand called");
	tns::JEnv env(_env);
	auto buf = new jbyte[length];

	env.GetByteArrayRegion(command, 0, length, buf);

	int len = length / sizeof(uint16_t);
	tns::JsDebugger::SendCommand(reinterpret_cast<uint16_t*>(buf), len);

	delete[] buf;
}


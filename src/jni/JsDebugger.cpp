#include "JsDebugger.h"
#include "V8GlobalHelpers.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include <assert.h>

using namespace std;
using namespace tns;

JsDebugger::JsDebugger()
{
}

void JsDebugger::Init(v8::Isolate *isolate, const string& packageName)
{
	s_isolate = isolate;
	s_packageName = packageName;

	JEnv env;
	s_JsDebuggerClass = env.FindClass("com/tns/JsDebugger");
	assert(s_JsDebuggerClass != nullptr);

	s_EnqueueMessage = env.GetStaticMethodID(s_JsDebuggerClass, "enqueueMessage", "(Ljava/lang/String;)V");
	assert(s_EnqueueMessage != nullptr);

	s_EnableAgent = env.GetStaticMethodID(s_JsDebuggerClass, "enableAgent", "(Ljava/lang/String;IZ)V");
	assert(s_EnableAgent != nullptr);
}

string JsDebugger::GetPackageName()
{
	return s_packageName;
}

/* *
 * sets who will handle the messages when they start comming from v8
 */
void JsDebugger::Enable()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::SetMessageHandler(MyMessageHandler);
}

/* *
 * the message that come from v8 will not be handled anymore
 */
void JsDebugger::Disable()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::SetMessageHandler(nullptr);
}

/* *
 * schedule a debugger break to happen when JavaScript code is run in the given isolate
 * (cli command: tns debug android --debug-brk) ?
 */
void JsDebugger::DebugBreak()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::DebugBreak(isolate);
}

void JsDebugger::ProcessDebugMessages()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::ProcessDebugMessages();
}

void JsDebugger::SendCommand(JNIEnv *_env, jobject obj, jbyteArray command, jint length) {
	tns::JEnv env(_env);
	auto buf = new jbyte[length];

	env.GetByteArrayRegion(command, 0, length, buf);

	int len = length / sizeof(uint16_t);
	SendCommandToV8(reinterpret_cast<uint16_t*>(buf), len);

	delete[] buf;
}

void JsDebugger::DebugBreakCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	JEnv env;
	JniLocalRef packageName(env.NewStringUTF(s_packageName.c_str()));

	jint port = 8181;
	if ((args.Length() > 0) && args[0]->IsInt32())
	{
		port = args[0]->Int32Value();
	}
	jboolean jniFalse = JNI_FALSE;

	env.CallStaticVoidMethod(s_JsDebuggerClass, s_EnableAgent, (jstring)packageName, port, jniFalse);

	DebugBreak();
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void JsDebugger::SendCommandToV8(uint16_t *cmd, int length)
{
	auto isolate = s_isolate;

	v8::Debug::SendCommand(isolate, cmd, length, nullptr);
}

/* *
 * private method that takes debug message as json from v8
 * after it gets the message the message handler passes it to enqueueMessage method in java
 */
void JsDebugger::MyMessageHandler(const v8::Debug::Message& message)
{
	auto json = message.GetJSON();
	auto str = ConvertToString(json);

	JEnv env;
	JniLocalRef s(env.NewStringUTF(str.c_str()));

	env.CallStaticVoidMethod(s_JsDebuggerClass, s_EnqueueMessage, (jstring)s);
}

v8::Isolate* JsDebugger::s_isolate = nullptr;
string JsDebugger::s_packageName = "";
jclass JsDebugger::s_JsDebuggerClass = nullptr;
jmethodID JsDebugger::s_EnqueueMessage = nullptr;
jmethodID JsDebugger::s_EnableAgent = nullptr;


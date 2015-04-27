#include "JsDebugger.h"
#include "V8GlobalHelpers.h"
#include "JniLocalRef.h"
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
	assert(s_EnqueueMessage != nullptr);
}

string JsDebugger::GetPackageName()
{
	return s_packageName;
}

void JsDebugger::MyMessageHandler(const v8::Debug::Message& message)
{
	auto json = message.GetJSON();
	auto str = ConvertToString(json);

	JEnv env;
	JniLocalRef s(env.NewStringUTF(str.c_str()));

	env.CallStaticVoidMethod(s_JsDebuggerClass, s_EnqueueMessage, (jstring)s);
}

void JsDebugger::Enable()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::SetMessageHandler(MyMessageHandler);
}

void JsDebugger::Disable()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::SetMessageHandler(nullptr);
}

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

void JsDebugger::SendCommand(uint16_t *cmd, int length)
{
	auto isolate = s_isolate;

	v8::Debug::SendCommand(isolate, cmd, length, nullptr);
}

void JsDebugger::DebugBreakCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
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
}


v8::Isolate* JsDebugger::s_isolate = nullptr;
string JsDebugger::s_packageName = "";
jclass JsDebugger::s_JsDebuggerClass = nullptr;
jmethodID JsDebugger::s_EnqueueMessage = nullptr;
jmethodID JsDebugger::s_EnableAgent = nullptr;


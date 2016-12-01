#include "JsDebugger.h"
#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include <sstream>

using namespace std;
using namespace tns;
using namespace v8;

JsDebugger::JsDebugger()
{
}

void JsDebugger::Init(v8::Isolate *isolate, const string& packageName, jobject jsDebugger)
{
	s_isolate = isolate;

	JEnv env;
	s_jsDebugger = env.NewGlobalRef(jsDebugger);

	s_JsDebuggerClass = env.FindClass("com/tns/JsDebugger");
	assert(s_JsDebuggerClass != nullptr);

	s_EnqueueMessage = env.GetMethodID(s_JsDebuggerClass, "enqueueMessage", "(Ljava/lang/String;)V");
	assert(s_EnqueueMessage != nullptr);

	s_EnqueueConsoleMessage = env.GetMethodID(s_JsDebuggerClass, "enqueueConsoleMessage", "(Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;)V");
	assert(s_EnqueueConsoleMessage != nullptr);


	s_EnableAgent = env.GetMethodID(s_JsDebuggerClass, "enableAgent", "()V");
	assert(s_EnableAgent != nullptr);
}

/* *
 * sets who will handle the messages when they start comming from v8
 */
void JsDebugger::Enable()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::SetMessageHandler(isolate, MyMessageHandler);
	enabled = true;
}

/* *
 * the message that come from v8 will not be handled anymore
 */
void JsDebugger::Disable()
{
	enabled = false;
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::SetMessageHandler(isolate, nullptr);
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

bool JsDebugger::IsDebuggerActive()
{
	return s_processedCommands;
}

void JsDebugger::ProcessDebugMessages()
{
	auto isolate = s_isolate;
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Debug::ProcessDebugMessages(isolate);
}

void JsDebugger::SendCommand(JNIEnv *_env, jobject obj, jbyteArray command, jint length)
{
	tns::JEnv env(_env);
	auto buf = new jbyte[length];

	env.GetByteArrayRegion(command, 0, length, buf);

	int len = length / sizeof(uint16_t);
	SendCommandToV8(reinterpret_cast<uint16_t*>(buf), len);

	delete[] buf;
}

void JsDebugger::DebugBreakCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (s_jsDebugger == nullptr)
	{
		return;
	}

	try
	{
		JEnv env;
		env.CallStaticVoidMethod(s_JsDebuggerClass, s_EnableAgent);

		DebugBreak();
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void JsDebugger::ConsoleMessageCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (s_jsDebugger == nullptr || !enabled)
	{
		return;
	}

	auto isolate = s_isolate;
	Isolate::Scope isolate_scope(isolate);
	HandleScope handleScope(isolate);

	try
	{
		ConsoleMessage(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void JsDebugger::ConsoleMessage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if ((args.Length() > 0) && args[0]->IsString())
	{
		std::string message = ArgConverter::ConvertToString(args[0]->ToString());

		std:string level = "log";
		if (args.Length() > 1  && args[1]->IsString())
		{
			level = ArgConverter::ConvertToString(args[1]->ToString());
		}

		string srcFileName = "";
		int lineNumber = 0;
		int columnNumber = 0;

		auto stackTrace = StackTrace::CurrentStackTrace(args.GetIsolate(), 2, StackTrace::kOverview);
		if (!stackTrace.IsEmpty())
		{
			auto frame = stackTrace->GetFrame(1);
			if (!frame.IsEmpty())
			{
				auto scriptName = frame->GetScriptName();
				if (!scriptName.IsEmpty())
				{
					srcFileName = ArgConverter::ConvertToString(scriptName);
				}

				lineNumber = frame->GetLineNumber();
				columnNumber = frame->GetColumn();
			}
		}

		JEnv env;
		JniLocalRef jniText(env.NewStringUTF(message.c_str()));
		JniLocalRef jniLevel(env.NewStringUTF(level.c_str()));
		JniLocalRef jniSrcFileName(env.NewStringUTF(srcFileName.c_str()));

		env.CallVoidMethod(s_jsDebugger, s_EnqueueConsoleMessage, (jstring) jniText, (jstring)jniLevel, lineNumber, columnNumber, (jstring)jniSrcFileName);
	}
}

void JsDebugger::ConsoleMessage(const string& message, const string& level, const string& srcFileName, int lineNumber, int columnNumber)
{
	if (s_jsDebugger == nullptr || !enabled)
	{
		return;
	}

	try
	{
		JEnv env;
		JniLocalRef jniText(env.NewStringUTF(message.c_str()));
		JniLocalRef jniLevel(env.NewStringUTF(level.c_str()));
		JniLocalRef jniSrcFileName(env.NewStringUTF(srcFileName.c_str()));

		env.CallVoidMethod(s_jsDebugger, s_EnqueueConsoleMessage, (jstring) jniText, (jstring) jniLevel, lineNumber, columnNumber, (jstring) jniSrcFileName);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e)
	{
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...)
	{
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void JsDebugger::SendCommandToV8(uint16_t *cmd, int length)
{
	auto isolate = s_isolate;

	v8::Debug::SendCommand(isolate, cmd, length, nullptr);

	s_processedCommands = true;
}

/* *
 * private method that takes debug message as json from v8
 * after it gets the message the message handler passes it to enqueueMessage method in java
 */
void JsDebugger::MyMessageHandler(const v8::Debug::Message& message)
{
	if (s_jsDebugger == nullptr)
	{
		return;
	}

	auto json = message.GetJSON();
	auto str = ArgConverter::ConvertToString(json);

	JEnv env;
	JniLocalRef s(env.NewStringUTF(str.c_str()));
	env.CallVoidMethod(s_jsDebugger, s_EnqueueMessage, (jstring) s);
}

bool JsDebugger::enabled = false;
v8::Isolate* JsDebugger::s_isolate = nullptr;
jobject JsDebugger::s_jsDebugger = nullptr;
jclass JsDebugger::s_JsDebuggerClass = nullptr;
jmethodID JsDebugger::s_EnqueueMessage = nullptr;
jmethodID JsDebugger::s_EnqueueConsoleMessage = nullptr;
jmethodID JsDebugger::s_EnableAgent = nullptr;
bool JsDebugger::s_processedCommands = false;

#include "JsDebugger.h"
#include "V8GlobalHelpers.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include <assert.h>
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
	s_packageName = packageName;

	JEnv env;
	s_jsDebugger = env.NewGlobalRef(jsDebugger);

	s_JsDebuggerClass = env.FindClass("com/tns/JsDebugger");
	assert(s_JsDebuggerClass != nullptr);

	s_EnqueueMessage = env.GetMethodID(s_JsDebuggerClass, "enqueueMessage", "(Ljava/lang/String;)V");
	assert(s_EnqueueMessage != nullptr);

	s_EnableAgent = env.GetMethodID(s_JsDebuggerClass, "enableAgent", "()V");
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
		std::string message = ConvertToString(args[0]->ToString());
		//jboolean isError = (jboolean) = args[1]->ToBoolean()->BooleanValue();


		std:string level = "log";
		if (args.Length() > 1  && args[1]->IsString())
		{
			level = ConvertToString(args[1]->ToString());
		}

		string srcFileName = "";
		int lineNumber = 0;
		int columnNumber = 0;

		auto stackTrace = StackTrace::CurrentStackTrace(Isolate::GetCurrent(), 2, StackTrace::kOverview);
		if (!stackTrace.IsEmpty())
		{
			auto frame = stackTrace->GetFrame(1);
			if (!frame.IsEmpty())
			{
				auto scriptName = frame->GetScriptName();
				if (!scriptName.IsEmpty())
				{
					srcFileName = ConvertToString(scriptName);
				}

				lineNumber = frame->GetLineNumber();
				columnNumber = frame->GetColumn();
			}
		}




		//    			var consoleEvent = {
		//	    			"seq":0,
		//	    			"type":"event",
		//	    			"event":"messageAdded",
		//	    			"success":true,
		//	    			"body":
		//	    			{
		//	    				"message":
		//	    				{
		//	    			        "source":"console-api",
		//	    			        "type": "log",
		//	    			        "level": 'error',
		//	    			        "line": 0,
		//	    			        "column": 0,
		//	    			        "url": "",
		//	    			        "groupLevel": 7,
		//	    			        "repeatCount": 1,
		//	    			        "text": "My message"
		//	    			    }
		//	    			}
		//    			};

		stringstream consoleEventSS;
		consoleEventSS << "{\"seq\":0, \"type\":\"event\", \"event\":\"messageAdded\", \"success\":true, \"body\": { \"message\": { \"source\":\"console-api\", "
				<< " \"type\": \"log\","
				<< " \"level\": \"" << level << "\", "
				<< " \"line\": " << lineNumber << ","
				<< " \"column\": " << columnNumber << ","
				<< " \"url\" : \"" << srcFileName << "\","
				<< " \"groupLevel\": 7, \"repeatCount\": 1, "
				<< " \"text\": \"" << message << "\" } } }";


		JEnv env;
		JniLocalRef s(env.NewStringUTF(consoleEventSS.str().c_str()));
		env.CallVoidMethod(s_jsDebugger, s_EnqueueMessage, (jstring) s);
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
	if (s_jsDebugger == nullptr)
	{
		return;
	}

	auto json = message.GetJSON();
	auto str = ConvertToString(json);

	JEnv env;
	JniLocalRef s(env.NewStringUTF(str.c_str()));
	env.CallVoidMethod(s_jsDebugger, s_EnqueueMessage, (jstring) s);
}

bool JsDebugger::enabled = false;
v8::Isolate* JsDebugger::s_isolate = nullptr;
jobject JsDebugger::s_jsDebugger = nullptr;
string JsDebugger::s_packageName = "";
jclass JsDebugger::s_JsDebuggerClass = nullptr;
jmethodID JsDebugger::s_EnqueueMessage = nullptr;
jmethodID JsDebugger::s_EnableAgent = nullptr;


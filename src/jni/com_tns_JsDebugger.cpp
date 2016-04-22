#include "JEnv.h"
#include "JsDebugger.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include <sstream>
#include <string>

using namespace tns;
using namespace std;

extern "C" void Java_com_tns_JsDebugger_processDebugMessages(JNIEnv *env, jobject obj)
{
	try
	{
		JsDebugger::ProcessDebugMessages();
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToJava();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToJava();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToJava();
	}
}

extern "C" void Java_com_tns_JsDebugger_enable(JNIEnv *env, jobject obj)
{
	try
	{
		JsDebugger::Enable();
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToJava();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToJava();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToJava();
	}
}

extern "C" void Java_com_tns_JsDebugger_disable(JNIEnv *env, jobject obj)
{
	try
	{
		JsDebugger::Disable();
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToJava();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToJava();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToJava();
	}
}

extern "C" void Java_com_tns_JsDebugger_debugBreak(JNIEnv *env, jobject obj)
{
	try
	{
		JsDebugger::DebugBreak();
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToJava();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToJava();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToJava();
	}
}

extern "C" jboolean Java_com_tns_JsDebugger_isDebuggerActive(JNIEnv *env, jobject obj)
{
	try
	{
		return (jboolean)JsDebugger::IsDebuggerActive();
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToJava();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToJava();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToJava();
	}
}

extern "C" void Java_com_tns_JsDebugger_sendCommand(JNIEnv *_env, jobject obj, jbyteArray command, jint length)
{
	try
	{
		JsDebugger::SendCommand(_env, obj, command, length);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToJava();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToJava();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToJava();
	}
}


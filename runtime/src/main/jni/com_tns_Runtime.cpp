#include "V8StringConstants.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace std;
using namespace tns;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	try
	{
		Runtime::Init(vm, reserved);
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

	return JNI_VERSION_1_6;
}

extern "C" void Java_com_tns_Runtime_initNativeScript(JNIEnv *_env, jobject obj, jint runtimeId, jstring filesPath, jboolean verboseLoggingEnabled, jstring packageName, jobjectArray args, jobject jsDebugger)
{
	try
	{
		Runtime::Init(_env, obj, runtimeId, filesPath, verboseLoggingEnabled, packageName, args, jsDebugger);
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

Runtime* TryGetRuntime(int runtimeId)
{
	Runtime *runtime = nullptr;
	try
	{
		runtime = Runtime::GetRuntime(runtimeId);
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
	return runtime;
}

extern "C" void Java_com_tns_Runtime_runModule(JNIEnv *_env, jobject obj, jint runtimeId, jstring scriptFile)
{
	auto runtime = TryGetRuntime(runtimeId);
	if (runtime == nullptr)
	{
		return;
	}

	auto isolate = runtime->GetIsolate();
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	try
	{
		runtime->RunModule(_env, obj, scriptFile);
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

extern "C" jobject Java_com_tns_Runtime_runScript(JNIEnv *_env, jobject obj, jint runtimeId, jstring scriptFile)
{
	jobject result = nullptr;

	auto runtime = TryGetRuntime(runtimeId);
	if (runtime == nullptr)
	{
		return result;
	}

	auto isolate = runtime->GetIsolate();
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	try
	{
		result = runtime->RunScript(_env, obj, scriptFile);
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
	return result;
}

extern "C" jobject Java_com_tns_Runtime_callJSMethodNative(JNIEnv *_env, jobject obj, jint runtimeId, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs)
{
	jobject result = nullptr;

	auto runtime = TryGetRuntime(runtimeId);
	if (runtime == nullptr)
	{
		return result;
	}

	auto isolate = runtime->GetIsolate();
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	try
	{
		result = runtime->CallJSMethodNative(_env, obj, javaObjectID, methodName, retType, isConstructor, packagedArgs);
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
	return result;
}

extern "C" void Java_com_tns_Runtime_createJSInstanceNative(JNIEnv *_env, jobject obj, jint runtimeId, jobject javaObject, jint javaObjectID, jstring className)
{
	auto runtime = TryGetRuntime(runtimeId);
	if (runtime == nullptr)
	{
		return;
	}

	auto isolate = runtime->GetIsolate();
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	try
	{
		runtime->CreateJSInstanceNative(_env, obj, javaObject, javaObjectID, className);
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

extern "C" jint Java_com_tns_Runtime_generateNewObjectId(JNIEnv *env, jobject obj, jint runtimeId)
{
	try
	{
		auto runtime = TryGetRuntime(runtimeId);
		if (runtime == nullptr)
		{
			return 0;
		}
		return runtime->GenerateNewObjectId(env, obj);
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

extern "C" void Java_com_tns_Runtime_adjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jint runtimeId, jlong usedMemory)
{
	auto runtime = TryGetRuntime(runtimeId);
	if (runtime == nullptr)
	{
		return;
	}

	auto isolate = runtime->GetIsolate();
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	try
	{
		runtime->AdjustAmountOfExternalAllocatedMemoryNative(env, obj, usedMemory);
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

extern "C" void Java_com_tns_Runtime_passUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jint runtimeId, jthrowable exception, jstring stackTrace)
{
	auto runtime = TryGetRuntime(runtimeId);
	if (runtime == nullptr)
	{
		return;
	}

	auto isolate = runtime->GetIsolate();
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handleScope(isolate);

	try
	{
		runtime->PassUncaughtExceptionToJsNative(env, obj, exception, stackTrace);
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

extern "C" void Java_com_tns_Runtime_clearStartupData(JNIEnv *env, jobject obj, jint runtimeId)
{
	auto runtime = TryGetRuntime(runtimeId);
	if (runtime == nullptr)
	{
		return;
	}

	runtime->ClearStartupData(env, obj);
}

extern "C" jint Java_com_tns_Runtime_getPointerSize(JNIEnv *env, jobject obj)
{
	return sizeof(void*);
}

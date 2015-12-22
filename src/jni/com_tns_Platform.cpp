#include "V8StringConstants.h"
#include "NativePlatform.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"

using namespace std;
using namespace tns;

v8::Isolate *g_isolate = nullptr;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	try
	{
		return NativePlatform::JNI_ON_LOAD(vm, reserved);
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

extern "C" void Java_com_tns_Platform_initNativeScript(JNIEnv *_env, jobject obj, jstring filesPath, jint appJavaObjectId, jboolean verboseLoggingEnabled, jstring packageName, jobjectArray args)
{
	try
	{
		g_isolate = NativePlatform::InitNativeScript(_env, obj, filesPath, appJavaObjectId, verboseLoggingEnabled, packageName, args);
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

extern "C" void Java_com_tns_Platform_runModule(JNIEnv *_env, jobject obj, jstring scriptFile)
{
	v8::Isolate::Scope isolate_scope(g_isolate);
	v8::HandleScope handleScope(g_isolate);

	try
	{
		NativePlatform::RunModule(_env, obj, scriptFile);
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

extern "C" jobject Java_com_tns_Platform_runScript(JNIEnv *_env, jobject obj, jstring scriptFile)
{
	v8::Isolate::Scope isolate_scope(g_isolate);
	v8::HandleScope handleScope(g_isolate);

	jobject o = nullptr;
	try
	{
		o = NativePlatform::RunScript(_env, obj, scriptFile);
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
	return o;
}

extern "C" jobject Java_com_tns_Platform_callJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs)
{
	v8::Isolate::Scope isolate_scope(g_isolate);
	v8::HandleScope handleScope(g_isolate);

	jobject o = nullptr;
	try
	{
		o = NativePlatform::CallJSMethodNative(_env, obj, javaObjectID, methodName, retType, isConstructor, packagedArgs);
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
	return o;
}

extern "C" void Java_com_tns_Platform_createJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className)
{
	v8::Isolate::Scope isolate_scope(g_isolate);
	v8::HandleScope handleScope(g_isolate);

	try
	{
		NativePlatform::CreateJSInstanceNative(_env, obj, javaObject, javaObjectID, className);
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

extern "C" jint Java_com_tns_Platform_generateNewObjectId(JNIEnv *env, jobject obj)
{
	try
	{
		return NativePlatform::GenerateNewObjectId(env, obj);
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

extern "C" void Java_com_tns_Platform_adjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jlong usedMemory)
{
	v8::Isolate::Scope isolate_scope(g_isolate);
	v8::HandleScope handleScope(g_isolate);

	try
	{
		NativePlatform::AdjustAmountOfExternalAllocatedMemoryNative(env, obj, usedMemory);
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

extern "C" void Java_com_tns_Platform_passUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace)
{
	v8::Isolate::Scope isolate_scope(g_isolate);
	v8::HandleScope handleScope(g_isolate);

	try
	{
		NativePlatform::PassUncaughtExceptionToJsNative(env, obj, exception, stackTrace);
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

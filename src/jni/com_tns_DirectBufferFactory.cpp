#include "jni.h"
#include "NativePlatform.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace std;
using namespace tns;

extern "C" jobject Java_com_tns_DirectBufferFactory_allocateByteBuffer(JNIEnv *env, jobject obj, jint capacity, jlongArray address)
{
	jobject buffer = nullptr;
	try
	{
		buffer = NativePlatform::AllocateByteBuffer(env, capacity, address);
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
	return buffer;
}

extern "C" void Java_com_tns_DirectBufferFactory_freeByteBuffer(JNIEnv *env, jobject obj, jlong address)
{
	try
	{
		NativePlatform::FreeByteBuffer(env, address);
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

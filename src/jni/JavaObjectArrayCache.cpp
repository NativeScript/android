#include "JavaObjectArrayCache.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace tns;


JavaObjectArrayCache::JavaObjectArrayCache()
{
}


void JavaObjectArrayCache::Init(JavaVM *jvm)
{
	s_jvm = jvm;
}


jobjectArray JavaObjectArrayCache::GetJavaObjectArray(int length)
{
	bool supportedLength = length < MAX_JAVA_PARAMS_COUNT;

	if(!supportedLength) {
		stringstream ss;
		ss << "You are trying to override more than the MAX_JAVA_PARAMS_COUNT: " << MAX_JAVA_PARAMS_COUNT;
		throw NativeScriptException(ss.str());
	}

	jobjectArray arr = s_objArr[length];

	if (arr == nullptr)
	{
		JEnv env;
		jclass objectClass = env.FindClass("java/lang/Object");
		JniLocalRef tmpArr(env.NewObjectArray(length, objectClass, nullptr));
		s_objArr[length] = arr = (jobjectArray)env.NewGlobalRef(tmpArr);
	}

	return arr;
}


jobjectArray JavaObjectArrayCache::GetJavaStringArray(int length)
{
	bool supportedLength = length < MAX_OVERWRITE_METHOD_COUNT;

	if(!supportedLength) {
		stringstream ss;
		ss << "You are trying to override more than the MAX_OVERWRITE_METHOD_COUNT: " << MAX_OVERWRITE_METHOD_COUNT;
		throw NativeScriptException(ss.str());
	}

	jobjectArray arr = s_strArr[length];

	if (arr == nullptr)
	{
		JEnv env;
		jclass stringClass = env.FindClass("java/lang/String");
		JniLocalRef tmpArr(env.NewObjectArray(length, stringClass, nullptr));
		s_strArr[length] = arr = (jobjectArray)env.NewGlobalRef(tmpArr);
	}

	return arr;
}


JavaVM* JavaObjectArrayCache::s_jvm = nullptr;
__thread jobjectArray JavaObjectArrayCache::s_objArr[MAX_JAVA_PARAMS_COUNT] = { nullptr };
__thread jobjectArray JavaObjectArrayCache::s_strArr[MAX_OVERWRITE_METHOD_COUNT]  = { nullptr };

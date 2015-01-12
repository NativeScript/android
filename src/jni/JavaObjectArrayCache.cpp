#include "JavaObjectArrayCache.h"
#include "JniLocalRef.h"
#include <assert.h>

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

	assert(supportedLength);

	jobjectArray arr;

	if (supportedLength)
	{
		arr = s_objArr[length];

		if (arr == nullptr)
		{
			JEnv env;
			jclass objectClass = env.FindClass("java/lang/Object");
			JniLocalRef tmpArr(env.NewObjectArray(length, objectClass, nullptr));
			s_objArr[length] = arr = (jobjectArray)env.NewGlobalRef(tmpArr);
		}
	}
	else
	{
		arr = nullptr;
	}

	return arr;
}


jobjectArray JavaObjectArrayCache::GetJavaStringArray(int length)
{
	bool supportedLength = length < MAX_OVERWRITE_METHOD_COUNT;

	assert(supportedLength);

	jobjectArray arr;

	if (supportedLength)
	{
		arr = s_strArr[length];

		if (arr == nullptr)
		{
			JEnv env;
			jclass stringClass = env.FindClass("java/lang/String");
			JniLocalRef tmpArr(env.NewObjectArray(length, stringClass, nullptr));
			s_strArr[length] = arr = (jobjectArray)env.NewGlobalRef(tmpArr);
		}
	}
	else
	{
		arr = nullptr;
	}

	return arr;
}


JavaVM* JavaObjectArrayCache::s_jvm = nullptr;
__thread jobjectArray JavaObjectArrayCache::s_objArr[MAX_JAVA_PARAMS_COUNT] = { nullptr };
__thread jobjectArray JavaObjectArrayCache::s_strArr[MAX_OVERWRITE_METHOD_COUNT]  = { nullptr };

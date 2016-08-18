#include "JavaObjectArrayCache.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace tns;
using namespace std;

JavaObjectArrayCache::JavaObjectArrayCache()
{
}

jobjectArray JavaObjectArrayCache::GetJavaStringArray(int length)
{
	bool supportedLength = length < MAX_OVERWRITE_METHOD_COUNT;

	if (!supportedLength)
	{
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
		s_strArr[length] = arr = (jobjectArray) env.NewGlobalRef(tmpArr);
	}

	return arr;
}

__thread jobjectArray JavaObjectArrayCache::s_strArr[MAX_OVERWRITE_METHOD_COUNT] =
{
		nullptr };

#ifndef JAVAOBJECTARRAYCACHE_H_
#define JAVAOBJECTARRAYCACHE_H_

#include "JEnv.h"

namespace tns
{
	class JavaObjectArrayCache
	{
	public:
		static void Init(JavaVM *jvm);

		static jobjectArray GetJavaObjectArray(int length);

		static jobjectArray GetJavaStringArray(int length);

	private:
		JavaObjectArrayCache();

		static const int MAX_JAVA_PARAMS_COUNT = 256;

		static const int MAX_OVERWRITE_METHOD_COUNT = 100;

		static JavaVM *s_jvm;

		static __thread jobjectArray s_objArr[MAX_JAVA_PARAMS_COUNT];

		static __thread jobjectArray s_strArr[MAX_OVERWRITE_METHOD_COUNT];
	};
}


#endif /* JAVAOBJECTARRAYCACHE_H_ */

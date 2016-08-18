#ifndef JAVAOBJECTARRAYCACHE_H_
#define JAVAOBJECTARRAYCACHE_H_

#include "JEnv.h"

namespace tns
{
	class JavaObjectArrayCache
	{
		public:
			static jobjectArray GetJavaStringArray(int length);

		private:
			JavaObjectArrayCache();

			static const int MAX_OVERWRITE_METHOD_COUNT = 100;

			static __thread jobjectArray s_strArr[MAX_OVERWRITE_METHOD_COUNT];
	};
}

#endif /* JAVAOBJECTARRAYCACHE_H_ */

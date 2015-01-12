#ifndef JSINSTANCEINFO_H_
#define JSINSTANCEINFO_H_

#include "jni.h"

namespace tns
{
	class JSInstanceInfo
	{
	public:
		JSInstanceInfo()
		{
			IsJavaObjectWeak = false;
			JavaObjectID = -1;
		}

		bool IsJavaObjectWeak;
		uint32_t JavaObjectID;
		jclass clazz;
	};
}

#endif /* JSINSTANCEINFO_H_ */

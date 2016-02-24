#ifndef ARRAYBUFFERHELPER_H_
#define ARRAYBUFFERHELPER_H_

#include "v8.h"
#include "ObjectManager.h"

namespace tns
{
	class ArrayBufferHelper
	{
		public:
			static void CreateConvertFunctions(ObjectManager *objectManager, const v8::Local<v8::Object>& global);

		private:
			ArrayBufferHelper();

			static void CreateFromCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

			static void CreateFromCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& info);

			static ObjectManager *s_objectManager;
	};
}


#endif /* ARRAYBUFFERHELPER_H_ */

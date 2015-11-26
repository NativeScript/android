#ifndef ARRAYELEMENTACCESSOR_H_
#define ARRAYELEMENTACCESSOR_H_

#include "JEnv.h"
#include "v8.h"
#include <string>
#include "ObjectManager.h"

namespace tns
{
	class ArrayElementAccessor
	{
	public:
		void Init(JavaVM *jvm, ObjectManager *objectManager);

		v8::Local<v8::Value> GetArrayElement(const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature);

		void SetArrayElement(const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature, v8::Local<v8::Value>& value);

	private:
		v8::Local<v8::Value> ConvertToJsValue(JEnv& env, const std::string& elementSignature, const void *value);

		JavaVM *jvm;

		ObjectManager *objectManager;
	};
}


#endif /* ARRAYELEMENTACCESSOR_H_ */

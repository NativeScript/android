#ifndef FIELDACCESSOR_H_
#define FIELDACCESSOR_H_

#include "v8.h"
#include "JEnv.h"
#include <string>
#include "ObjectManager.h"
#include "FieldCallbackData.h"

namespace tns
{
	class FieldAccessor
	{
	public:
		void Init(JavaVM *jvm, ObjectManager *objectManager);

		v8::Local<v8::Value> GetJavaField(const v8::Local<v8::Object>& target, FieldCallbackData *fieldData);

		void SetJavaField(const v8::Local<v8::Object>& target, const v8::Local<v8::Value>& value, FieldCallbackData *fieldData);

	private:

		JavaVM *jvm;

		ObjectManager *objectManager;
	};
}


#endif /* FIELDACCESSOR_H_ */

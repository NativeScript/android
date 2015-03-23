#ifndef FIELDACCESSOR_H_
#define FIELDACCESSOR_H_

#include "v8.h"
#include "JEnv.h"
#include <string>
#include "ObjectManager.h"

namespace tns
{
	class FieldAccessor
	{
	public:
		void Init(JavaVM *jvm, ObjectManager *objectManager);

		v8::Handle<v8::Value> GetJavaField(const v8::Handle<v8::Object>& caller, const std::string& declaringClassName, const std::string& fieldName, const std::string& fieldTypeName, const bool isStatic);

		void SetJavaField(const v8::Handle<v8::Object>& target, const v8::Handle<v8::Value>& value, const std::string& declaringTypeName, const std::string& fieldName, const std::string& fieldTypeName, bool isStatic);

	private:

		JavaVM *jvm;

		ObjectManager *objectManager;
	};
}


#endif /* FIELDACCESSOR_H_ */

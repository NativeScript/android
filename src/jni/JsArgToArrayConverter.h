#ifndef JSARGTOARRAYCONVERTER_H_
#define JSARGTOARRAYCONVERTER_H_

#include "JEnv.h"
#include "v8.h"
#include <vector>
#include <string>

namespace tns
{
	class JsArgToArrayConverter
	{
	public:
		JsArgToArrayConverter(const v8::FunctionCallbackInfo<v8::Value>& args, bool hasImplementationObject, const v8::Local<v8::Object>& outerThis = v8::Local<v8::Object>());

		JsArgToArrayConverter(const v8::Local<v8::Value>& arg, bool isImplementationObject, int classReturnType);

		~JsArgToArrayConverter();

		jobjectArray ToJavaArray();

		jobject GetConvertedArg();

		int Length() const;

		bool IsValid() const;

		struct Error;

		Error GetError() const;

		struct Error
		{
			Error() : index(-1), msg(std::string())
			{}

			int index;
			std::string msg;
		};

	private:
		bool ConvertArg(const v8::Local<v8::Value>& arg, int index);

		void SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobalRef = false);

		int m_argsLen;

		int m_return_type;

		bool m_isValid;

		Error m_error;

		std::vector<int> m_storedIndexes;

		jobject *m_argsAsObject;

		jobjectArray m_arr;
	};
}


#endif /* JSARGTOARRAYCONVERTER_H_ */

#ifndef JSARGCONVERTER_H_
#define JSARGCONVERTER_H_


#include <string>
#include <vector>
#include "JEnv.h"
#include "v8.h"

namespace tns
{
	class JsArgConverter
	{
	public:
		JsArgConverter(const v8::FunctionCallbackInfo<v8::Value>& args, bool hasImplementationObject, const std::string& methodSignature);

		~JsArgConverter();

		jvalue* ToArgs() const;

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

		bool ConvertArg(const v8::Handle<v8::Value>& arg, int index);

		bool ConvertJavaScriptArray(JEnv& env, const v8::Handle<v8::Array>& jsArr, int index);

		bool ConvertJavaScriptNumber(JEnv& env, const v8::Handle<v8::Value>& jsValue, int index);

		bool ConvertJavaScriptBoolean(JEnv& env, const v8::Handle<v8::Value>& jsValue, int index);

		bool ConvertJavaScriptString(JEnv& env, const v8::Handle<v8::Value>& jsValue, int index);

		void SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobalRef = false);

		template<typename T>
		bool ConvertFromCastFunctionObject(T value, int index);

		int m_argsLen;

		bool m_isValid;

		jvalue *m_args;

		std::string m_methodSignature;

		std::vector<std::string> m_tokens;

		std::vector<int> m_storedObjects;

		Error m_error;
	};
}

#endif /* JSARGCONVERTER_H_ */

#ifndef JSARGCONVERTER_H_
#define JSARGCONVERTER_H_


#include <string>
#include <vector>
#include "JEnv.h"
#include "v8.h"
#include "MetadataEntry.h"

namespace tns
{
	class JsArgConverter
	{
	public:
		JsArgConverter(const v8::FunctionCallbackInfo<v8::Value>& args, bool hasImplementationObject, const std::string& methodSignature, MetadataEntry *entry);

		~JsArgConverter();

		jvalue* ToArgs();

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

		bool ConvertJavaScriptArray(const v8::Local<v8::Array>& jsArr, int index);

		bool ConvertJavaScriptNumber(const v8::Local<v8::Value>& jsValue, int index);

		bool ConvertJavaScriptBoolean(const v8::Local<v8::Value>& jsValue, int index);

		bool ConvertJavaScriptString(const v8::Local<v8::Value>& jsValue, int index);

		void SetConvertedObject(int index, jobject obj, bool isGlobalRef = false);

		template<typename T>
		bool ConvertFromCastFunctionObject(T value, int index);

		JEnv m_env;

		int m_argsLen;

		bool m_isValid;

		jvalue m_args[255];

		std::string m_methodSignature;

		std::vector<std::string> *m_tokens;
		std::vector<std::string> m_tokens2;

		std::vector<int> m_storedObjects;

		Error m_error;
	};
}

#endif /* JSARGCONVERTER_H_ */

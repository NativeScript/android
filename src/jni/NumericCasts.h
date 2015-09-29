#ifndef NUMERICCASTS_H_
#define NUMERICCASTS_H_

#include "v8.h"
#include <string>

namespace tns
{
	class NumericCasts
	{
	public:
			void CreateGlobalCastFunctions(const v8::Local<v8::ObjectTemplate>& globalTemplate);

	private:
			void MarkAsLongCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			void MarkAsByteCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			void MarkAsShortCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			void MarkAsCharCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			void MarkAsFloatCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			void MarkAsDoubleCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			v8::Persistent<v8::Object>* MarkJsObject(const v8::Local<v8::Object>& object, const std::string& mark, const v8::Local<v8::Value>& value);

			static void MarkAsLongCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void MarkAsByteCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void MarkAsShortCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void MarkAsCharCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void MarkAsFloatCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void MarkAsDoubleCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void MarkedJsObjectWeakCallback(const v8::WeakCallbackData<v8::Object, v8::Persistent<v8::Object> >& data);

			static NumericCasts* GetThis(const v8::FunctionCallbackInfo<v8::Value>& args);
	};
}


#endif /* NUMERICCASTS_H_ */

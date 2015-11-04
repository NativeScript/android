/*
 * Module.h
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#ifndef JNI_MODULE_H_
#define JNI_MODULE_H_

#include "JEnv.h"
#include "v8.h"

#include <string>
#include <map>

namespace tns
{
	class Module
	{
		public:
			static void Init(v8::Isolate *isolate);

			static v8::Local<v8::String> WrapModuleContent(const std::string& path);

			static v8::Local<v8::Object> CompileAndRun(const std::string& modulePath, bool& hasError);

		private:
			Module() = default;

			static void RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static v8::Local<v8::Function> GetRequireFunction(v8::Isolate *isolate, const std::string& dirName);

			static jclass MODULE_CLASS;
			static jmethodID GET_MODULE_PATH_METHOD_ID;

			static const char* MODULE_PART_1;
			static const char* MODULE_PART_2;

			static v8::Persistent<v8::Function> *s_poRequireFunction;

			static v8::Persistent<v8::Function> *s_poRequireFactoryFunction;

			static std::map<std::string, v8::Persistent<v8::Function>*> s_requireCache;

			static std::map<std::string, v8::Persistent<v8::Object>*> s_loadedModules;
	};
}


#endif /* JNI_MODULE_H_ */

/*
 * Require.h
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#ifndef JNI_REQUIRE_H_
#define JNI_REQUIRE_H_

#include "v8.h"
#include "JEnv.h"
#include <string>

namespace tns
{
	class Require
	{
		public:
			static v8::Local<v8::Object> CompileAndRun(const std::string& path, bool& hasError);
			static void Callback(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Init();

		private:
			static v8::Local<v8::String> LoadScriptText(const std::string& path);
			static v8::ScriptCompiler::CachedData* TryLoadScriptCache(const std::string& path);
			static void SaveScriptCache(const v8::ScriptCompiler::Source& source, const std::string& path);

			// fields
			static jmethodID GET_MODULE_PATH_METHOD_ID;
			static std::map<std::string, v8::Persistent<v8::Object>*> loadedModules;
			static jclass RequireClass;

			static const char* MODULE_PART_1;
			static const char* MODULE_PART_2;
			static const char* MODULE_PART_3;
			static const char* MODULE_PART_4;
	};
}


#endif /* JNI_REQUIRE_H_ */

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

			static v8::Local<v8::Object> Load(const std::string& path, bool& isData);

		private:
			Module() = default;

			static v8::Local<v8::String> WrapModuleContent(const std::string& path);

			static v8::Local<v8::Object> LoadModule(v8::Isolate *isolate, const std::string& path);

			static v8::Local<v8::Object> LoadData(v8::Isolate *isolate, const std::string& path);

			static v8::Local<v8::Script> LoadScript(v8::Isolate *isolate, const std::string& modulePath, const v8::Local<v8::String>& fullRequiredModulePath);

			static void RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void RequireNativeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static v8::Local<v8::Function> GetRequireFunction(v8::Isolate *isolate, const std::string& dirName);

			static v8::ScriptCompiler::CachedData* TryLoadScriptCache(const std::string& path);
			static void SaveScriptCache(const v8::ScriptCompiler::Source& source, const std::string& path);

			static jclass MODULE_CLASS;
			static jmethodID RESOLVE_PATH_METHOD_ID;

			static const char* MODULE_PART_1;
			static const char* MODULE_PART_2;

			static v8::Persistent<v8::Function> *s_poRequireFunction;

			static v8::Persistent<v8::Function> *s_poRequireFactoryFunction;

			static std::map<std::string, v8::Persistent<v8::Function>*> s_requireCache;

			static std::map<std::string, v8::Persistent<v8::Object>*> s_loadedModules;

			class TempModule
			{
				public:
				TempModule(const std::string& modulePath, v8::Persistent<v8::Object> *poModuleObj)
				:m_dispose(true), m_modulePath(modulePath), m_poModuleObj(poModuleObj)
				{
					s_loadedModules.insert(make_pair(m_modulePath, m_poModuleObj));
				}

				~TempModule()
				{
					if (m_dispose)
					{
						s_loadedModules.erase(m_modulePath);
					}
				}

				void SaveToCache()
				{
					m_dispose = false;
				}

				private:
				bool m_dispose;
				std::string m_modulePath;
				v8::Persistent<v8::Object> *m_poModuleObj;
			};
		};
	}

#endif /* JNI_MODULE_H_ */

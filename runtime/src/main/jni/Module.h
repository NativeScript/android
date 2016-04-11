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

			static void Load(const std::string& path);

			static void RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		private:
			struct Cache;

			Module() = default;

			static Cache* GetCache(v8::Isolate *isolate);

			static v8::Local<v8::String> WrapModuleContent(const std::string& path);

			static v8::Local<v8::Object> LoadImpl(v8::Isolate *isolate, const std::string& path, bool& isData);

			static v8::Local<v8::Object> LoadModule(v8::Isolate *isolate, const std::string& path);

			static v8::Local<v8::Object> LoadData(v8::Isolate *isolate, const std::string& path);

			static v8::Local<v8::Script> LoadScript(v8::Isolate *isolate, const std::string& modulePath, const v8::Local<v8::String>& fullRequiredModulePath);

			static void RequireNativeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static v8::Local<v8::Function> GetRequireFunction(v8::Isolate *isolate, const std::string& dirName);

			static v8::ScriptCompiler::CachedData* TryLoadScriptCache(const std::string& path);
			static void SaveScriptCache(const v8::ScriptCompiler::Source& source, const std::string& path);

			static jclass MODULE_CLASS;
			static jmethodID RESOLVE_PATH_METHOD_ID;

			static const char* MODULE_PROLOGUE;
			static const char* MODULE_EPILOGUE;
			static std::map<v8::Isolate*, Cache*> s_cache;

			struct Cache
			{
				v8::Persistent<v8::Function> *RequireFunction;

				v8::Persistent<v8::Function> *RequireFactoryFunction;

				std::map<std::string, v8::Persistent<v8::Function>*> RequireCache;

				std::map<std::string, v8::Persistent<v8::Object>*> LoadedModules;
			};

			class TempModule
			{
				public:
				TempModule(v8::Isolate *isolate, const std::string& modulePath, v8::Persistent<v8::Object> *poModuleObj)
				:m_isolate(isolate), m_dispose(true), m_modulePath(modulePath), m_poModuleObj(poModuleObj)
				{
					auto cache = GetCache(isolate);
					cache->LoadedModules.insert(make_pair(m_modulePath, m_poModuleObj));
				}

				~TempModule()
				{
					if (m_dispose)
					{
						auto cache = GetCache(m_isolate);
						cache->LoadedModules.erase(m_modulePath);
					}
				}

				void SaveToCache()
				{
					m_dispose = false;
				}

				private:
				bool m_dispose;
				v8::Isolate *m_isolate;
				std::string m_modulePath;
				v8::Persistent<v8::Object> *m_poModuleObj;
			};
		};
	}

#endif /* JNI_MODULE_H_ */

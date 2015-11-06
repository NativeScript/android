/*
 * Require.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */
#include "Require.h"
#include "File.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptAssert.h"
#include "SimpleProfiler.h"
#include "ExceptionUtil.h"
#include "JniLocalRef.h"
#include "ArgConverter.h"

#include <sstream>

using namespace v8;
using namespace std;

namespace tns
{
	void Require::Init()
	{
		JEnv env;

		RequireClass = env.FindClass("com/tns/Require");
		assert(RequireClass != nullptr);

		GET_MODULE_PATH_METHOD_ID = env.GetStaticMethodID(RequireClass, "getModulePath", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
		assert(GET_MODULE_PATH_METHOD_ID != nullptr);
	}
	Local<String> Require::LoadScriptText(const string& path)
	{
		string content = File::ReadText(path);

		auto separatorIndex = path.find_last_of("/");
		string dirName = path.substr(0, separatorIndex);

		// TODO: Use statically allocated buffer for better performance
		string result(MODULE_PART_1);
		result.reserve(content.length() + 1024);
		result += dirName;
		result += MODULE_PART_2;
		result += path;
		result += MODULE_PART_3;
		result += content;
		result += MODULE_PART_4;

		return ConvertToV8String(result);
	}

	void Require::Callback(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		SET_PROFILER_FRAME();

		auto isolate = Isolate::GetCurrent();
		if (args.Length() != 2)
		{
			isolate->ThrowException(ConvertToV8String("require should be called with two parameters"));
			return;
		}
		if (!args[0]->IsString())
		{
			isolate->ThrowException(ConvertToV8String("require's first parameter should be string"));
			return;
		}
		if (!args[1]->IsString())
		{
			isolate->ThrowException(ConvertToV8String("require's second parameter should be string"));
			return;
		}

		string moduleName = ConvertToString(args[0].As<String>());
		string callingModuleDirName = ConvertToString(args[1].As<String>());

		JEnv env;
		JniLocalRef jsModulename(env.NewStringUTF(moduleName.c_str()));
		JniLocalRef jsCallingModuleDirName(env.NewStringUTF(callingModuleDirName.c_str()));
		JniLocalRef jsModulePath(env.CallStaticObjectMethod(RequireClass, GET_MODULE_PATH_METHOD_ID, (jstring) jsModulename, (jstring) jsCallingModuleDirName));

		// cache the required modules by full path, not name only, since there might be some collisions with relative paths and names
		string modulePath = ArgConverter::jstringToString((jstring) jsModulePath);

		if(modulePath == ""){
			// module not found
			stringstream ss;
			ss << "Module \"" << moduleName << "\" not found";
			string exception = ss.str();
			ExceptionUtil::GetInstance()->ThrowExceptionToJs(exception);
			return;
		}
		if (modulePath == "EXTERNAL_FILE_ERROR")
		{
			// module not found
			stringstream ss;
			ss << "Module \"" << moduleName << "\" is located on the external storage. Modules can be private application files ONLY";
			string exception = ss.str();
			ExceptionUtil::GetInstance()->ThrowExceptionToJs(exception);
			return;
		}

		auto it = loadedModules.find(modulePath);

		Local<Object> moduleObj;
		bool hasError = false;

		if (it == loadedModules.end())
		{
			moduleObj = CompileAndRun(modulePath, hasError);
		}
		else
		{
			moduleObj = Local<Object>::New(isolate, *((*it).second));
		}

		if(!hasError){
			args.GetReturnValue().Set(moduleObj);
		}
	}

	Local<Object> Require::CompileAndRun(const string& path, bool& hasError)
	{
		auto isolate = Isolate::GetCurrent();

		Local<Value> exportObj = Object::New(isolate);
		Persistent<Object>* persistentExportObj = new Persistent<Object>(isolate, exportObj.As<Object>());
		loadedModules.insert(make_pair(path, persistentExportObj));

		TryCatch tc;
		Local<Object> moduleObj;

		auto scriptText = LoadScriptText(path);

		DEBUG_WRITE("Compiling script (module %s)", path.c_str());
		auto v8Path = ConvertToV8String(path);

		auto cacheData = TryLoadScriptCache(path);

		ScriptOrigin origin(v8Path);
		ScriptCompiler::Source source(scriptText, origin, cacheData);
		ScriptCompiler::CompileOptions option = ScriptCompiler::kNoCompileOptions;
		Local<Script> script;

		if(cacheData != nullptr)
		{
			option = ScriptCompiler::kConsumeCodeCache;
			script = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option).ToLocalChecked();
		}
		else
		{
			option = ScriptCompiler::kProduceCodeCache;
			script = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option).ToLocalChecked();
			SaveScriptCache(source, path);
		}

		DEBUG_WRITE("Compiled script (module %s)", path.c_str());

		if (ExceptionUtil::GetInstance()->HandleTryCatch(tc, "Script " + path + " contains compilation errors!"))
		{
			hasError = true;
		}
		else if (script.IsEmpty())
		{
			//think about more descriptive message -> [script_name] was empty
			DEBUG_WRITE("%s was empty", path.c_str());
		}
		else
		{
			DEBUG_WRITE("Running script (module %s)", path.c_str());

			auto f = script->Run().As<Function>();
			if (ExceptionUtil::GetInstance()->HandleTryCatch(tc, "Error running script " + path))
			{
				hasError = true;
			}
			else
			{
				auto result = f->Call(Object::New(isolate), 1, &exportObj);
				if(ExceptionUtil::GetInstance()->HandleTryCatch(tc, "Error calling module function "))
				{
					hasError = true;
				}
				else
				{
					moduleObj = result.As<Object>();
					if (moduleObj.IsEmpty())
					{
						auto objectTemplate = ObjectTemplate::New();
						moduleObj = objectTemplate->NewInstance();
					}

					DEBUG_WRITE("Script completed (module %s)", path.c_str());

					if (!moduleObj->StrictEquals(exportObj))
					{
						loadedModules.erase(path);
						persistentExportObj->Reset();
						delete persistentExportObj;

						auto persistentModuleObject = new Persistent<Object>(isolate, moduleObj);
						loadedModules.insert(make_pair(path, persistentModuleObject));
					}
				}
			}
		}

		if(hasError)
		{
			loadedModules.erase(path);
			persistentExportObj->Reset();
			delete persistentExportObj;

			// this handles recursive require calls
			tc.ReThrow();
		}
		else
		{
			return moduleObj;
		}
	}

	ScriptCompiler::CachedData* Require::TryLoadScriptCache(const std::string& path)
	{
		auto cachePath = path + ".cache";
		if(!File::Exists(cachePath))
		{
			return nullptr;
		}

		auto file = fopen(cachePath.c_str(), "rb" /* read binary */);
		fseek(file, 0, SEEK_END);
		auto size = ftell(file);
		rewind(file);

		uint8_t* data = new uint8_t[size];
		fread(data, sizeof(uint8_t), size, file);

		auto cache = new ScriptCompiler::CachedData(data, size, ScriptCompiler::CachedData::BufferOwned);

		fclose(file);

		return cache;
	}

	void Require::SaveScriptCache(const ScriptCompiler::Source& source, const std::string& path)
	{
		int length = source.GetCachedData()->length;
		auto cachePath = path + ".cache";
		auto file = fopen(cachePath.c_str(), "wb" /* write binary */);
		fwrite(source.GetCachedData()->data, sizeof(uint8_t), length, file);
		fclose(file);
	}

	const char* Require::MODULE_PART_1 = "(function(){\n var module = {}; module.exports = arguments[0]; var exports = module.exports; var __dirname = \"";
	const char* Require::MODULE_PART_2 = "\"; var __filename = \"";
	const char* Require::MODULE_PART_3 = "\"; function require(moduleName){ return __global.require(moduleName, __dirname); } module.filename = __filename; this.__extends = __global.__extends; \n";
	const char* Require::MODULE_PART_4 = "\n return module.exports; \n})";

	map<string, Persistent<Object>*> Require::loadedModules;
	jmethodID Require::GET_MODULE_PATH_METHOD_ID = nullptr;
	jclass Require::RequireClass = nullptr;
}

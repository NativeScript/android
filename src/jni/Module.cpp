/*
 * Module.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */
#include "Module.h"
#include "File.h"
#include "JniLocalRef.h"
#include "ArgConverter.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptAssert.h"
#include "Constants.h"
#include "NativeScriptException.h"
#include "Util.h"

#include <sstream>
#include <assert.h>
#include <libgen.h>
#include <dlfcn.h>

using namespace v8;
using namespace std;
using namespace tns;

void Module::Init(Isolate *isolate)
{
	JEnv env;

	MODULE_CLASS = env.FindClass("com/tns/Module");
	assert(MODULE_CLASS != nullptr);

	RESOLVE_PATH_METHOD_ID = env.GetStaticMethodID(MODULE_CLASS, "resolvePath", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	assert(RESOLVE_PATH_METHOD_ID != nullptr);

	string requireFactoryScript =
	"(function () { "
	"	function require_factory(requireInternal, dirName) { "
	"		return function require(modulePath) { "
	"			return requireInternal(modulePath, dirName); "
	"		} "
	"	} "
	"	return require_factory; "
	"})()";

	auto source = ConvertToV8String(requireFactoryScript);
	auto context = isolate->GetCurrentContext();

	Local<Script> script;
	auto maybeScript = Script::Compile(context, source).ToLocal(&script);

	assert(!script.IsEmpty());

	Local<Value> result;
	auto maybeResult = script->Run(context).ToLocal(&result);

	assert(!result.IsEmpty() && result->IsFunction());

	auto requireFactoryFunction = result.As<Function>();

	s_poRequireFactoryFunction = new Persistent<Function>(isolate, requireFactoryFunction);

	auto requireFuncTemplate = FunctionTemplate::New(isolate, RequireCallback);
	auto requireFunc = requireFuncTemplate->GetFunction();
	s_poRequireFunction = new Persistent<Function>(isolate, requireFunc);
}

Local<Function> Module::GetRequireFunction(Isolate *isolate, const string& dirName)
{
	Local<Function> requireFunc;

	auto itFound = s_requireCache.find(dirName);

	if (itFound != s_requireCache.end())
	{
		requireFunc = Local<Function>::New(isolate, *itFound->second);
	}
	else
	{
		auto requireFuncFactory = Local<Function>::New(isolate, *s_poRequireFactoryFunction);

		auto context = isolate->GetCurrentContext();

		auto requireInternalFunc = Local<Function>::New(isolate, *s_poRequireFunction);

		Local<Value> args[2] { requireInternalFunc, ConvertToV8String(dirName) };
		Local<Value> result;
		auto thiz = Object::New(isolate);
		auto success = requireFuncFactory->Call(context, thiz, 2, args).ToLocal(&result);

		assert(success && !result.IsEmpty() && result->IsFunction());

		requireFunc = result.As<Function>();

		auto poFunc = new Persistent<Function>(isolate, requireFunc);

		s_requireCache.insert(make_pair(dirName, poFunc));
	}

	return requireFunc;
}

void Module::RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
		auto isolate = Isolate::GetCurrent();

		if (args.Length() != 2)
		{
			throw NativeScriptException(string("require should be called with two parameters"));
		}
		if (!args[0]->IsString())
		{
			throw NativeScriptException(string("require's first parameter should be string"));
		}
		if (!args[1]->IsString())
		{
			throw NativeScriptException(string("require's second parameter should be string"));
		}

		string moduleName = ConvertToString(args[0].As<String>());
		string callingModuleDirName = ConvertToString(args[1].As<String>());

		JEnv env;
		JniLocalRef jsModulename(env.NewStringUTF(moduleName.c_str()));
		JniLocalRef jsCallingModuleDirName(env.NewStringUTF(callingModuleDirName.c_str()));
		JniLocalRef jsModulePath(env.CallStaticObjectMethod(MODULE_CLASS, RESOLVE_PATH_METHOD_ID, (jstring) jsModulename, (jstring) jsCallingModuleDirName));

		// cache the required modules by full path, not name only, since there might be some collisions with relative paths and names
		string modulePath = ArgConverter::jstringToString((jstring) jsModulePath);

		auto it = s_loadedModules.find(modulePath);

		Local<Object> moduleObj;

		auto isData = false;

		if (it == s_loadedModules.end())
		{
			moduleObj = Load(modulePath, isData);
		}
		else
		{
			moduleObj = Local<Object>::New(isolate, *it->second);
		}

		if (isData)
		{
			assert(!moduleObj.IsEmpty());

			args.GetReturnValue().Set(moduleObj);
		}
		else
		{
			auto exportsObj = moduleObj->Get(ConvertToV8String("exports"));

			assert(!exportsObj.IsEmpty());

			args.GetReturnValue().Set(exportsObj);
		}
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void Module::RequireNativeCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	auto ext = args.Data().As<External>();
	auto funcPtr = reinterpret_cast<FunctionCallback>(ext->Value());
	funcPtr(args);
}

Local<Object> Module::Load(const string& path, bool& isData)
{
	Local<Object> result;

	auto isolate = Isolate::GetCurrent();

	if (Util::EndsWith(path, ".js") || Util::EndsWith(path, ".so"))
	{
		isData = false;
		result = LoadModule(isolate, path);
	}
	else if (Util::EndsWith(path, ".json"))
	{
		isData = true;
		result = LoadData(isolate, path);
	}
	else
	{
		string errMsg = "Unsupported file extension: " + path;
		throw NativeScriptException(errMsg);
	}

	return result;
}

Local<Object> Module::LoadModule(Isolate *isolate, const string& modulePath)
{
	Local<Object> result;

	auto moduleObj = Object::New(isolate);
	auto exportsObj = Object::New(isolate);
	auto exportsPropName = ConvertToV8String("exports");
	moduleObj->Set(exportsPropName, exportsObj);
	auto fullRequiredModulePath = ConvertToV8String(modulePath);
	moduleObj->Set(ConvertToV8String("filename"), fullRequiredModulePath);

	auto poModuleObj = new Persistent<Object>(isolate, moduleObj);
	s_loadedModules.insert(make_pair(modulePath, poModuleObj));

	TryCatch tc;

	Local<Function> moduleFunc;

	if (Util::EndsWith(modulePath, ".js"))
	{
		auto script = LoadScript(isolate, modulePath, fullRequiredModulePath);

		moduleFunc = script->Run().As<Function>();
		if (tc.HasCaught())
		{
			throw NativeScriptException(tc, "Error running script " + modulePath);
		}
	}
	else if (Util::EndsWith(modulePath, ".so"))
	{
		auto handle = dlopen (modulePath.c_str(), RTLD_LAZY);
		if (handle == nullptr)
		{
			auto error = dlerror();
			string errMsg(error);
			throw NativeScriptException(errMsg);
		}
		auto func = dlsym(handle, "NSMain");
		if (func == nullptr)
		{
			string errMsg("Cannot find 'NSMain' in " + modulePath);
			throw NativeScriptException(errMsg);
		}
		auto extFunc = External::New(isolate, func);
		auto ft = FunctionTemplate::New(isolate, RequireNativeCallback, extFunc);
		auto maybeFunc = ft->GetFunction(isolate->GetCurrentContext());
		if (maybeFunc.IsEmpty() || tc.HasCaught())
		{
			throw NativeScriptException(tc, "Cannot create native module function callback");
		}
		moduleFunc = maybeFunc.ToLocalChecked();
	}
	else
	{
		string errMsg = "Unsupported file extension: " + modulePath;
		throw NativeScriptException(errMsg);
	}

	auto fileName = ConvertToV8String(modulePath);
	char pathcopy[1024];
	strcpy(pathcopy, modulePath.c_str());
	string strDirName(dirname(pathcopy));
	auto dirName = ConvertToV8String(strDirName);
	auto require = GetRequireFunction(isolate, strDirName);
	Local<Value> requireArgs[5] { moduleObj, exportsObj, require, fileName, dirName };

	auto thiz = Object::New(isolate);
	moduleFunc->Call(thiz, sizeof(requireArgs) / sizeof(Local<Value>), requireArgs);

	if(tc.HasCaught()) {
		throw NativeScriptException(tc, "Error calling module function ");
	}

	result = moduleObj;

	return result;
}

Local<Script> Module::LoadScript(Isolate *isolate, const string& path, const Local<String>& fullRequiredModulePath)
{
	Local<Script> script;

	TryCatch tc;

	auto scriptText = Module::WrapModuleContent(path);

	DEBUG_WRITE("Compiling script (module %s)", path.c_str());
	//
	auto cacheData = TryLoadScriptCache(path);

	ScriptOrigin origin(fullRequiredModulePath);
	ScriptCompiler::Source source(scriptText, origin, cacheData);
	ScriptCompiler::CompileOptions option = ScriptCompiler::kNoCompileOptions;

	if(cacheData != nullptr)
	{
		option = ScriptCompiler::kConsumeCodeCache;
		auto maybeScript = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option);
		if (maybeScript.IsEmpty() || tc.HasCaught()) {
			throw NativeScriptException(tc, "Cannot compile " + path);
		}
		script = maybeScript.ToLocalChecked();
	}
	else
	{
		if(Constants::V8_CACHE_COMPILED_CODE)
		{
			option = ScriptCompiler::kProduceCodeCache;
		}
		auto maybeScript = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option);
		if (maybeScript.IsEmpty() || tc.HasCaught()) {
			throw NativeScriptException(tc, "Cannot compile " + path);
		}
		script = maybeScript.ToLocalChecked();
		SaveScriptCache(source, path);
	}

	DEBUG_WRITE("Compiled script (module %s)", path.c_str());

	return script;
}

Local<Object> Module::LoadData(Isolate *isolate, const string& path)
{
	Local<Object> json;

	auto jsonData = File::ReadText(path);

	TryCatch tc;

	auto jsonStr = ConvertToV8String(jsonData);

	auto maybeValue = JSON::Parse(isolate, jsonStr);

	if (maybeValue.IsEmpty() || tc.HasCaught())
	{
		string errMsg = "Cannot parse JSON file " + path;
		throw NativeScriptException(tc, errMsg);
	}

	auto value = maybeValue.ToLocalChecked();

	if (!value->IsObject())
	{
		string errMsg = "JSON is not valid, file=" + path;
		throw NativeScriptException(errMsg);
	}

	json = value.As<Object>();

	return json;
}

Local<String> Module::WrapModuleContent(const string& path)
{
	string content = File::ReadText(path);

	auto separatorIndex = path.find_last_of("/");
	string dirName = path.substr(0, separatorIndex);

	// TODO: Use statically allocated buffer for better performance
	string result(MODULE_PART_1);
	result.reserve(content.length() + 1024);
	result += content;
	result += MODULE_PART_2;

	return ConvertToV8String(result);
}

ScriptCompiler::CachedData* Module::TryLoadScriptCache(const std::string& path)
{
	if(!Constants::V8_CACHE_COMPILED_CODE)
	{
		return nullptr;
	}

	auto cachePath = path + ".cache";
	int length = 0;
	auto data = File::ReadBinary(cachePath, length);
	if(!data)
	{
		return nullptr;
	}

	return new ScriptCompiler::CachedData(reinterpret_cast<uint8_t*>(data), length, ScriptCompiler::CachedData::BufferOwned);
}

void Module::SaveScriptCache(const ScriptCompiler::Source& source, const std::string& path)
{
	if(!Constants::V8_CACHE_COMPILED_CODE)
	{
		return;
	}

	int length = source.GetCachedData()->length;
	auto cachePath = path + ".cache";
	File::WriteBinary(cachePath, source.GetCachedData()->data, length);
}


jclass Module::MODULE_CLASS = nullptr;
jmethodID Module::RESOLVE_PATH_METHOD_ID = nullptr;

const char* Module::MODULE_PART_1 = "(function(module, exports, require, __filename, __dirname){ ";
const char* Module::MODULE_PART_2 = "\n})";
Persistent<Function>* Module::s_poRequireFunction = nullptr;
Persistent<Function>* Module::s_poRequireFactoryFunction = nullptr;
map<string, Persistent<Function>*> Module::s_requireCache;
map<string, Persistent<Object>*> Module::s_loadedModules;

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
#include "SimpleProfiler.h"
#include "include/v8.h"
#include "CallbackHandlers.h"
#include <sstream>
#include <libgen.h>
#include <dlfcn.h>

using namespace v8;
using namespace std;
using namespace tns;

Module::Module()
	: m_isolate(nullptr), m_requireFunction(nullptr), m_requireFactoryFunction(nullptr)
{
}

void Module::Init(Isolate *isolate)
{
	JEnv env;

	if (MODULE_CLASS == nullptr)
	{
		MODULE_CLASS = env.FindClass("com/tns/Module");
		assert(MODULE_CLASS != nullptr);

		RESOLVE_PATH_METHOD_ID = env.GetStaticMethodID(MODULE_CLASS, "resolvePath", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
		assert(RESOLVE_PATH_METHOD_ID != nullptr);
	}

	m_isolate = isolate;

	string requireFactoryScript =
	"(function () { "
	"	function require_factory(requireInternal, dirName) { "
	"		return function require(modulePath) { "
	"			if(global.__requireOverride) { "
	"				var result = global.__requireOverride(modulePath, dirName); "
	"				if(result) { "
	"					return result; "
	"				} "
	"			} "
	"			return requireInternal(modulePath, dirName); "
	"		} "
	"	} "
	"	return require_factory; "
	"})()";

	auto source = ArgConverter::ConvertToV8String(isolate, requireFactoryScript);
	auto context = isolate->GetCurrentContext();

	auto global = context->Global();

	Local<Script> script;
	auto maybeScript = Script::Compile(context, source).ToLocal(&script);

	assert(!script.IsEmpty());

	Local<Value> result;
	auto maybeResult = script->Run(context).ToLocal(&result);

	assert(!result.IsEmpty() && result->IsFunction());

	auto requireFactoryFunction = result.As<Function>();

	m_requireFactoryFunction = new Persistent<Function>(isolate, requireFactoryFunction);

	auto requireFuncTemplate = FunctionTemplate::New(isolate, RequireCallback, External::New(isolate, this));
	auto requireFunc = requireFuncTemplate->GetFunction();
	global->Set(ArgConverter::ConvertToV8String(isolate, "__nativeRequire"), requireFunc);
	m_requireFunction = new Persistent<Function>(isolate, requireFunc);

	auto globalRequire = GetRequireFunction(isolate, Constants::APP_ROOT_FOLDER_PATH);
	global->Set(ArgConverter::ConvertToV8String(isolate, "require"), globalRequire);
}

Local<Function> Module::GetRequireFunction(Isolate *isolate, const string& dirName)
{
	Local<Function> requireFunc;

	auto itFound = m_requireCache.find(dirName);

	if (itFound != m_requireCache.end())
	{
		requireFunc = Local<Function>::New(isolate, *itFound->second);
	}
	else
	{
		auto requireFuncFactory = Local<Function>::New(isolate, *m_requireFactoryFunction);

		auto context = isolate->GetCurrentContext();

		auto requireInternalFunc = Local<Function>::New(isolate, *m_requireFunction);

		Local<Value> args[2]
		{
			requireInternalFunc, ArgConverter::ConvertToV8String(isolate, dirName)
		};
		Local<Value> result;
		auto thiz = Object::New(isolate);
		auto success = requireFuncFactory->Call(context, thiz, 2, args).ToLocal(&result);

		assert(success && !result.IsEmpty() && result->IsFunction());

		requireFunc = result.As<Function>();

		auto poFunc = new Persistent<Function>(isolate, requireFunc);

		m_requireCache.insert(make_pair(dirName, poFunc));
	}

	return requireFunc;
}

void Module::RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try
	{
		auto thiz = static_cast<Module*>(args.Data().As<External>()->Value());
		thiz->RequireCallbackImpl(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
				stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void Module::RequireCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	auto isolate = args.GetIsolate();

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

	string moduleName = ArgConverter::ConvertToString(args[0].As<String>());
	string callingModuleDirName = ArgConverter::ConvertToString(args[1].As<String>());
	auto isData = false;

	auto moduleObj = LoadImpl(isolate, moduleName, callingModuleDirName, isData);

	if (isData)
	{
		assert(!moduleObj.IsEmpty());

		args.GetReturnValue().Set(moduleObj);
	}
	else
	{
		auto exportsObj = moduleObj->Get(ArgConverter::ConvertToV8String(isolate, "exports"));

		assert(!exportsObj.IsEmpty());

		args.GetReturnValue().Set(exportsObj);
	}
}

void Module::RequireNativeCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	auto ext = args.Data().As<External>();
	auto funcPtr = reinterpret_cast<FunctionCallback>(ext->Value());
	funcPtr(args);
}

void Module::Load(const string& path)
{
	auto isolate = m_isolate;
	auto context = isolate->GetCurrentContext();
	auto globalObject = context->Global();
	auto require = globalObject->Get(context, ArgConverter::ConvertToV8String(isolate, "require")).ToLocalChecked().As<Function>();
	Local<Value> args[] = { ArgConverter::ConvertToV8String(isolate, path) };
	require->Call(context, globalObject, 1, args);
}

void Module::LoadWorker(const string& path)
{
	auto isolate = m_isolate;
	TryCatch tc;

	Load(path);

	if(tc.HasCaught()) {
		// This will handle any errors that occur when first loading a script (new worker)
		// exceptions include: syntax errors, throw statements, access of properties of undefined objects
		// Check if `onerror` handle is implemented
		// Web behaviour - if onerror handle comes before exception throw - execute it, else - bubble up to main's worker object
		CallbackHandlers::CallWorkerScopeOnErrorHandle(isolate, tc);
	}
}

void Module::CheckFileExists(Isolate* isolate, const std::string &path, const std::string &baseDir) {
	JEnv env;
	JniLocalRef jsModulename(env.NewStringUTF(path.c_str()));
	JniLocalRef jsBaseDir(env.NewStringUTF(baseDir.c_str()));

	env.CallStaticObjectMethod(MODULE_CLASS, RESOLVE_PATH_METHOD_ID, (jstring) jsModulename, (jstring) jsBaseDir);
}

Local<Object> Module::LoadImpl(Isolate *isolate, const string& moduleName, const string& baseDir, bool& isData)
{
	auto pathKind = GetModulePathKind(moduleName);
	auto cachePathKey = (pathKind == ModulePathKind::Global) ? moduleName : (baseDir + "*" + moduleName);

	Local<Object> result;

	DEBUG_WRITE(">>LoadImpl cachePathKey=%s", cachePathKey.c_str());

	auto it = m_loadedModules.find(cachePathKey);

	if (it == m_loadedModules.end())
	{
		JEnv env;
		JniLocalRef jsModulename(env.NewStringUTF(moduleName.c_str()));
		JniLocalRef jsBaseDir(env.NewStringUTF(baseDir.c_str()));
		JniLocalRef jsModulePath(env.CallStaticObjectMethod(MODULE_CLASS, RESOLVE_PATH_METHOD_ID, (jstring) jsModulename, (jstring) jsBaseDir));

		auto path = ArgConverter::jstringToString((jstring) jsModulePath);

		auto it2 = m_loadedModules.find(path);

		if (it2 == m_loadedModules.end())
		{
			if (Util::EndsWith(path, ".js") || Util::EndsWith(path, ".so"))
			{
				isData = false;
				result = LoadModule(isolate, path, cachePathKey);
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
		}
		else
		{
			auto& cacheEntry = it2->second;
			isData = cacheEntry.isData;
			result = Local<Object>::New(isolate, *cacheEntry.obj);
		}
	}
	else
	{
		auto& cacheEntry = it->second;
		isData = cacheEntry.isData;
		result = Local<Object>::New(isolate, *cacheEntry.obj);
	}

	return result;
}

Local<Object> Module::LoadModule(Isolate *isolate, const string& modulePath, const string& moduleCacheKey)
{
	Local<Object> result;

	auto moduleObj = Object::New(isolate);
	auto exportsObj = Object::New(isolate);
	auto exportsPropName = ArgConverter::ConvertToV8String(isolate, "exports");
	moduleObj->Set(exportsPropName, exportsObj);
	auto fullRequiredModulePath = ArgConverter::ConvertToV8String(isolate, modulePath);
	moduleObj->Set(ArgConverter::ConvertToV8String(isolate, "filename"), fullRequiredModulePath);

	auto poModuleObj = new Persistent<Object>(isolate, moduleObj);
	TempModule tempModule(this, modulePath, moduleCacheKey, poModuleObj);

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
		auto handle = dlopen(modulePath.c_str(), RTLD_LAZY);
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

	SET_PROFILER_FRAME();

	auto fileName = ArgConverter::ConvertToV8String(isolate, modulePath);
	char pathcopy[1024];
	strcpy(pathcopy, modulePath.c_str());
	string strDirName(dirname(pathcopy));
	auto dirName = ArgConverter::ConvertToV8String(isolate, strDirName);
	auto require = GetRequireFunction(isolate, strDirName);
	Local<Value> requireArgs[5]
	{
		moduleObj, exportsObj, require, fileName, dirName
	};

	moduleObj->Set(ArgConverter::ConvertToV8String(isolate, "require"), require);

	auto moduleIdProp = ArgConverter::ConvertToV8String(isolate, "id");
	const auto readOnlyFlags = static_cast<PropertyAttribute>(PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);
	Maybe<bool> success = moduleObj->DefineOwnProperty(isolate->GetCurrentContext(), moduleIdProp, fileName, readOnlyFlags);
	if(success.IsNothing()) {
		throw NativeScriptException(string("Couldn't execute method 'DefineOwnProperty' on 'moduleObj' in 'Module::LoadModule'."));
	}

	auto thiz = Object::New(isolate);
	auto extendsName = ArgConverter::ConvertToV8String(isolate, "__extends");
	thiz->Set(extendsName, isolate->GetCurrentContext()->Global()->Get(extendsName));
	moduleFunc->Call(thiz, sizeof(requireArgs) / sizeof(Local<Value> ), requireArgs);

	if (tc.HasCaught())
	{
		throw NativeScriptException(tc, "Error calling module function ");
	}

	tempModule.SaveToCache();
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

	if (cacheData != nullptr)
	{
		option = ScriptCompiler::kConsumeCodeCache;
		auto maybeScript = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option);
		if (maybeScript.IsEmpty() || tc.HasCaught())
		{
			throw NativeScriptException(tc, "Cannot compile " + path);
		}
		script = maybeScript.ToLocalChecked();
	}
	else
	{
		if (Constants::V8_CACHE_COMPILED_CODE)
		{
			option = ScriptCompiler::kProduceCodeCache;
		}
		auto maybeScript = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option);
		if (maybeScript.IsEmpty() || tc.HasCaught())
		{
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

	auto jsonStr = ArgConverter::ConvertToV8String(isolate, jsonData);

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

	// TODO: Use statically allocated buffer for better performance
	string result(MODULE_PROLOGUE);
	result.reserve(content.length() + 1024);
	result += content;
	result += MODULE_EPILOGUE;

	return ArgConverter::ConvertToV8String(m_isolate, result);
}

ScriptCompiler::CachedData* Module::TryLoadScriptCache(const std::string& path)
{
	if (!Constants::V8_CACHE_COMPILED_CODE)
	{
		return nullptr;
	}

	auto cachePath = path + ".cache";
	int length = 0;
	auto data = File::ReadBinary(cachePath, length);
	if (!data)
	{
		return nullptr;
	}

	return new ScriptCompiler::CachedData(reinterpret_cast<uint8_t*>(data), length, ScriptCompiler::CachedData::BufferOwned);
}

void Module::SaveScriptCache(const ScriptCompiler::Source& source, const std::string& path)
{
	if (!Constants::V8_CACHE_COMPILED_CODE)
	{
		return;
	}

	int length = source.GetCachedData()->length;
	auto cachePath = path + ".cache";
	File::WriteBinary(cachePath, source.GetCachedData()->data, length);
}

Module::ModulePathKind Module::GetModulePathKind(const std::string& path)
{
	ModulePathKind kind;
	switch (path[0])
	{
		case '.': kind = ModulePathKind::Relative; break;
		case '/': kind = ModulePathKind::Absolute; break;
		default: kind = ModulePathKind::Global; break;
	}
	return kind;
}

jclass Module::MODULE_CLASS = nullptr;
jmethodID Module::RESOLVE_PATH_METHOD_ID = nullptr;

const char* Module::MODULE_PROLOGUE = "(function(module, exports, require, __filename, __dirname){ ";
const char* Module::MODULE_EPILOGUE = "\n})";

#include "NativeScriptRuntime.h"
#include "MetadataNode.h"
#include "JniLocalRef.h"
#include "JsArgConverter.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "Constants.h"
#include "v8.h"
#include "libplatform/libplatform.h"
#include "Version.h"
#include "JEnv.h"
#include "WeakRef.h"
#include "Profiler.h"
#include "NativeScriptAssert.h"
#include "JsDebugger.h"
#include "SimpleProfiler.h"
#include "SimpleAllocator.h"
#include "File.h"
#include "JType.h"
#include "Module.h"
#include "NativeScriptException.h"
#include "NativePlatform.h"
#include <sstream>
#include <android/log.h>
#include <string>

using namespace v8;
using namespace std;
using namespace tns;

//TODO: Lubo: properly release this jni global ref on shutdown
JavaVM *g_jvm = nullptr;
Persistent<Context> *PrimaryContext = nullptr;
Context::Scope *context_scope = nullptr;
Isolate *g_isolate = nullptr;
ObjectManager *g_objectManager = nullptr;
bool tns::LogEnabled = true;
int AppJavaObjectID = -1;
int count = 0;
SimpleAllocator g_allocator;

jobject ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType);
void AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
void static PrepareExtendFunction(Isolate *isolate, jstring filesPath);
void static PrepareV8Runtime(Isolate *isolate, JEnv& env, jstring filesPath, jstring packageName);

jint NativePlatform::JNI_ON_LOAD(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.Native", "NativeScript Runtime Version %s, commit %s", NATIVE_SCRIPT_RUNTIME_VERSION, NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);
	DEBUG_WRITE("JNI_ONLoad");

	g_jvm = vm;

	JEnv::Init(g_jvm);

	g_objectManager = new ObjectManager();

	DEBUG_WRITE("JNI_ONLoad END");

	return JNI_VERSION_1_6;
}

void NativePlatform::InitNativeScript(JNIEnv *_env, jobject obj, jstring filesPath, jint appJavaObjectId, jboolean verboseLoggingEnabled, jstring packageName, jobjectArray args)
{
	AppJavaObjectID = appJavaObjectId;
	tns::LogEnabled = verboseLoggingEnabled;

	JEnv env(_env);

	auto filesRoot = ArgConverter::jstringToString(filesPath);
	Constants::APP_ROOT_FOLDER_PATH = filesRoot + "/app/";
	// read config options passed from Java
	JniLocalRef v8Flags(env.GetObjectArrayElement(args, 0));
	Constants::V8_STARTUP_FLAGS = ArgConverter::jstringToString(v8Flags);
	JniLocalRef cacheCode(env.GetObjectArrayElement(args, 1));
	Constants::V8_CACHE_COMPILED_CODE = (bool)cacheCode;
	JniLocalRef snapshot(env.GetObjectArrayElement(args, 2));
	Constants::V8_HEAP_SNAPSHOT = (bool)snapshot;

	DEBUG_WRITE("Initializing Telerik NativeScript: app instance id:%d", appJavaObjectId);

	NativeScriptException::Init(g_jvm, g_objectManager);
	PrepareV8Runtime(env, filesRoot, packageName);

}

void NativePlatform::RunModule(JNIEnv *_env, jobject obj, jstring scriptFile)
{
	JEnv env(_env);
	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);

	HandleScope handleScope(isolate);

	string filePath = ArgConverter::jstringToString(scriptFile);

	bool isData = false;

	auto moduleObj = Module::Load(filePath, isData);

	assert(!isData);
}

jobject NativePlatform::RunScript(JNIEnv *_env, jobject obj, jstring scriptFile)
{
	JEnv env(_env);
	jobject res = nullptr;

	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);
	HandleScope handleScope(isolate);
	auto context = isolate->GetCurrentContext();

	auto filename = ArgConverter::jstringToString(scriptFile);
	auto src = File::ReadText(filename);
	auto source = ConvertToV8String(src);

	TryCatch tc;

	Local<Script> script;
	ScriptOrigin origin(ConvertToV8String(filename));
	auto maybeScript = Script::Compile(context, source, &origin).ToLocal(&script);

	if(tc.HasCaught()) {
		throw NativeScriptException(tc, "Script " + filename + " contains compilation errors!");
	}

	if (!script.IsEmpty())
	{
		Local<Value> result;
		auto maybeResult = script->Run(context).ToLocal(&result);

		if(tc.HasCaught()) {
			throw NativeScriptException(tc, "Error running script " + filename);
		}
		if (!result.IsEmpty())
		{
			res = ConvertJsValueToJavaObject(env, result, static_cast<int>(Type::Null));
		}
		else
		{
			DEBUG_WRITE(">>runScript maybeResult is empty");
		}
	}
	else
	{
		DEBUG_WRITE(">>runScript maybeScript is empty");
	}

	return res;
}

jobject NativePlatform::CallJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs)
{
	SET_PROFILER_FRAME();

	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);

	JEnv env(_env);

	DEBUG_WRITE("CallJSMethodNative called javaObjectID=%d", javaObjectID);

	HandleScope handleScope(isolate);

	auto jsObject = g_objectManager->GetJsObjectByJavaObject(javaObjectID);
	if (jsObject.IsEmpty())
	{
		stringstream ss;
		ss << "JavaScript object for Java ID " << javaObjectID << " not found." << endl;
		ss << "Attempting to call method " << ArgConverter::jstringToString(methodName) << endl;

		throw NativeScriptException(ss.str());
	}

	if (isConstructor)
	{
		DEBUG_WRITE("CallJSMethodNative: Updating linked instance with its real class");
		jclass instanceClass = env.GetObjectClass(obj);
		g_objectManager->SetJavaClass(jsObject, instanceClass);
	}

	DEBUG_WRITE("CallJSMethodNative called jsObject=%d", jsObject->GetIdentityHash());

	string method_name = ArgConverter::jstringToString(methodName);
	auto jsResult = NativeScriptRuntime::CallJSMethod(env, jsObject, method_name, packagedArgs);

	int classReturnType = retType;
	jobject javaObject = ConvertJsValueToJavaObject(env, jsResult, classReturnType);
	return javaObject;
}

void NativePlatform::CreateJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className)
{
	SET_PROFILER_FRAME();

	DEBUG_WRITE("createJSInstanceNative called");

	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);

	JEnv env(_env);

	HandleScope handleScope(isolate);
	// TODO: Do we need a TryCatch here? It is currently not used anywhere
	 TryCatch tc;

	string existingClassName = ArgConverter::jstringToString(className);
	string jniName = Util::ConvertFromCanonicalToJniName(existingClassName);
	Local<Object> jsInstance;
	Local<Object> implementationObject;

	auto proxyClassName = g_objectManager->GetClassName(javaObject);
	//
	if (proxyClassName == "com/tns/NativeScriptActivity")
	{
		return;
	}
	//
	DEBUG_WRITE("createJSInstanceNative class %s", proxyClassName.c_str());
	jsInstance = MetadataNode::CreateExtendedJSWrapper(isolate, proxyClassName);
	if (jsInstance.IsEmpty())
	{
		throw NativeScriptException(string("NativeScript application not initialized correctly. Cannot create extended JS wrapper."));
	}

	implementationObject = MetadataNode::GetImplementationObject(jsInstance);
	if (implementationObject.IsEmpty())
	{
		string msg("createJSInstanceNative: implementationObject is empty");
		throw NativeScriptException(msg);
	}
	DEBUG_WRITE("createJSInstanceNative: implementationObject :%d", implementationObject->GetIdentityHash());

	jclass clazz = env.FindClass(jniName);
	g_objectManager->Link(jsInstance, javaObjectID, clazz);
}

jint NativePlatform::GenerateNewObjectId(JNIEnv *env, jobject obj)
{
	int objectId = g_objectManager->GenerateNewObjectID();

	return objectId;
}

void NativePlatform::AdjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jlong usedMemory)
{
	Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(usedMemory);
}

void NativePlatform::PassUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace)
{
	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);
	HandleScope handleScope(isolate);

	//create error message
	string errMsg = "The application crashed because of an uncaught exception. You can look at \"stackTrace\" or \"nativeException\" for more detailed information about the exception.";
	auto errObj = Exception::Error(ConvertToV8String(errMsg)).As<Object>();

	//create a new native exception js object
	jint javaObjectID = g_objectManager->GetOrCreateObjectId((jobject) exception);
	auto nativeExceptionObject = g_objectManager->GetJsObjectByJavaObject(javaObjectID);

	if (nativeExceptionObject.IsEmpty())
	{
		string className = g_objectManager->GetClassName((jobject)exception);
		//create proxy object that wraps the java err
		nativeExceptionObject = g_objectManager->CreateJSWrapper(javaObjectID, className);
	}

	//create a JS error object
	errObj->Set(V8StringConstants::GetNativeException(), nativeExceptionObject);
	errObj->Set(V8StringConstants::GetStackTrace(), ArgConverter::jstringToV8String(stackTrace));

	//pass err to JS
	NativeScriptException::CallJsFuncWithErr(errObj);
}

void NativePlatform::AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		throw NativeScriptException(string("Application should be initialized with single parameter"));
	}
	if (!args[0]->IsObject())
	{
		throw NativeScriptException(string("Application should be initialized with single object parameter containing overridden methods"));
	}

	// TODO: find another way to get "com/tns/NativeScriptApplication" metadata (move it to more appropriate place)
	auto node = MetadataNode::GetOrCreate("com/tns/NativeScriptApplication");
	auto appInstance = node->CreateJSWrapper(isolate);
	DEBUG_WRITE("Application object created id: %d", appInstance->GetIdentityHash());

	auto implementationObject = args[0]->ToObject();
	implementationObject->SetHiddenValue(V8StringConstants::GetClassImplementationObject(), External::New(isolate, node));
	DEBUG_WRITE("Application object implementation object is with id: %d", implementationObject->GetIdentityHash());
	implementationObject->SetPrototype(appInstance->GetPrototype());
	bool appSuccess = appInstance->SetPrototype(implementationObject);
	if (!appSuccess)
	{
		throw NativeScriptException(string("Application could not be initialized correctly"));
	}

	jweak applicationObject = g_objectManager->GetJavaObjectByID(AppJavaObjectID);

	JEnv env;
	jclass appClass = env.FindClass("com/tns/NativeScriptApplication");
	g_objectManager->Link(appInstance, AppJavaObjectID, appClass);

	JniLocalRef applicationClass(env.GetObjectClass(applicationObject));
	jmethodID setNativeScriptOverridesMethodId = env.GetMethodID((jclass)applicationClass, "setNativeScriptOverrides", "([Ljava/lang/String;)V");
	jobjectArray methodOverrides = NativeScriptRuntime::GetMethodOverrides(env, implementationObject);
	env.CallVoidMethod(applicationObject, setNativeScriptOverridesMethodId, methodOverrides);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void NativePlatform::PrepareV8Runtime(JEnv& env, string filesPath, jstring packageName)
{
	Platform* platform = v8::platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();

	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = &g_allocator;

	// prepare the snapshot blob
	if(Constants::V8_HEAP_SNAPSHOT)
	{
		auto snapshotPath = filesPath + "/internal/snapshot.dat";
		StartupData startup_data;
		if( File::Exists(snapshotPath))
		{
			int length;
			startup_data.data = reinterpret_cast<char*>(File::ReadBinary(snapshotPath, length));
			startup_data.raw_size = length;
		}
		else
		{
			startup_data = V8::CreateSnapshotDataBlob();
			File::WriteBinary(snapshotPath, startup_data.data, startup_data.raw_size);
		}

		create_params.snapshot_blob = &startup_data;
	}

	g_isolate = Isolate::New(create_params);
	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);
	HandleScope handleScope(isolate);

	V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(), Constants::V8_STARTUP_FLAGS.size());
	V8::SetCaptureStackTraceForUncaughtExceptions(true, 100, StackTrace::kOverview);
	V8::AddMessageListener(NativeScriptException::OnUncaughtError);

	auto globalTemplate = ObjectTemplate::New();

	const auto readOnlyFlags = static_cast<PropertyAttribute>(PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);

	globalTemplate->Set(ConvertToV8String("__startNDKProfiler"), FunctionTemplate::New(isolate, Profiler::StartNDKProfilerCallback));
	globalTemplate->Set(ConvertToV8String("__stopNDKProfiler"), FunctionTemplate::New(isolate, Profiler::StopNDKProfilerCallback));
	globalTemplate->Set(ConvertToV8String("__startCPUProfiler"), FunctionTemplate::New(isolate, Profiler::StartCPUProfilerCallback));
	globalTemplate->Set(ConvertToV8String("__stopCPUProfiler"), FunctionTemplate::New(isolate, Profiler::StopCPUProfilerCallback));
	globalTemplate->Set(ConvertToV8String("__heapSnapshot"), FunctionTemplate::New(isolate, Profiler::HeapSnapshotMethodCallback));
	globalTemplate->Set(ConvertToV8String("__log"), FunctionTemplate::New(isolate, NativeScriptRuntime::LogMethodCallback));
	globalTemplate->Set(ConvertToV8String("__dumpReferenceTables"), FunctionTemplate::New(isolate, NativeScriptRuntime::DumpReferenceTablesMethodCallback));
	globalTemplate->Set(ConvertToV8String("__debugbreak"), FunctionTemplate::New(isolate, JsDebugger::DebugBreakCallback));
	globalTemplate->Set(ConvertToV8String("__enableVerboseLogging"), FunctionTemplate::New(isolate, NativeScriptRuntime::EnableVerboseLoggingMethodCallback));
	globalTemplate->Set(ConvertToV8String("__disableVerboseLogging"), FunctionTemplate::New(isolate, NativeScriptRuntime::DisableVerboseLoggingMethodCallback));
	globalTemplate->Set(ConvertToV8String("__exit"), FunctionTemplate::New(isolate, NativeScriptRuntime::ExitMethodCallback));

	WeakRef::Init(isolate, globalTemplate, g_objectManager);

	SimpleProfiler::Init(isolate, globalTemplate);

	NativeScriptRuntime::CreateGlobalCastFunctions(globalTemplate);

	Local<Context> context = Context::New(isolate, nullptr, globalTemplate);
	PrimaryContext = new Persistent<Context>(isolate, context);

	if(Constants::V8_HEAP_SNAPSHOT)
	{
		// we own the snapshot buffer, delete it
		delete[] create_params.snapshot_blob->data;
	}

	context_scope = new Context::Scope(context);

	Module::Init(isolate);

	g_objectManager->Init(isolate);

	auto global = context->Global();

	auto appTemplate = ObjectTemplate::New();
	appTemplate->Set(ConvertToV8String("init"), FunctionTemplate::New(isolate, AppInitCallback));
	auto appInstance = appTemplate->NewInstance();
	global->ForceSet(ConvertToV8String("app"), appInstance, readOnlyFlags);

	global->ForceSet(ConvertToV8String("global"), global, readOnlyFlags);
	global->ForceSet(ConvertToV8String("__global"), global, readOnlyFlags);

	ArgConverter::Init(g_jvm);

	NativeScriptRuntime::Init(g_jvm, g_objectManager);

	string pckName = ArgConverter::jstringToString(packageName);
	Profiler::Init(pckName);
	JsDebugger::Init(isolate, pckName);

	NativeScriptRuntime::BuildMetadata(env, filesPath);

	NativeScriptRuntime::CreateTopLevelNamespaces(global);
}

jobject NativePlatform::ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType)
{
	JsArgToArrayConverter argConverter(value, false/*is implementation object*/, classReturnType);
	jobject jr = argConverter.GetConvertedArg();
	jobject javaResult = nullptr;
	if (jr != nullptr)
	{
		javaResult = env.NewLocalRef(jr);
	}

	return javaResult;
}

void NativePlatform::PrepareExtendFunction(Isolate *isolate, jstring filesPath)
{
	string fullPath = ArgConverter::jstringToString(filesPath);
	fullPath.append("/internal/prepareExtend.js");

	int length;
	bool isNew;
	const char* content = File::ReadText(fullPath, length, isNew);

	TryCatch tc;
	auto cmd = ConvertToV8String(content, length);

	if(isNew)
	{
		delete[] content;
	}

	auto origin = ConvertToV8String(fullPath);
	DEBUG_WRITE("Compiling prepareExtend.js script");

	auto script = Script::Compile(cmd, origin);
	DEBUG_WRITE("Compile prepareExtend.js script");

	if (script.IsEmpty() || tc.HasCaught())
	{
		DEBUG_WRITE("Cannot compile prepareExtend.js script");
		return;
	}

	DEBUG_WRITE("Compiled prepareExtend.js script");

	script->Run();

	if(tc.HasCaught()) {
		throw NativeScriptException(tc);
	}

	DEBUG_WRITE("Executed prepareExtend.js script");
}

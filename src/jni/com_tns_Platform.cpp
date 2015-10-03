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
#include "ExceptionUtil.h"
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
#include <sstream>
#include <android/log.h>
#include <assert.h>
#include <string>

using namespace v8;
using namespace std;
using namespace tns;

jobject ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType);

//TODO: Lubo: properly release this jni global ref on shutdown
JavaVM *g_jvm = nullptr;
Persistent<Context> *PrimaryContext = nullptr;
int count = 0;
Context::Scope *context_scope = nullptr;
bool tns::LogEnabled = true;
Isolate *g_isolate = nullptr;
std::string Constants::APP_ROOT_FOLDER_PATH = "";

ObjectManager *g_objectManager = nullptr;
SimpleAllocator g_allocator;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.Native", "NativeScript Runtime Version %s, commit %s", NATIVE_SCRIPT_RUNTIME_VERSION, NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);
	DEBUG_WRITE("JNI_ONLoad");

	g_jvm = vm;

	JEnv::Init(g_jvm);

	g_objectManager = new ObjectManager();

	DEBUG_WRITE("JNI_ONLoad END");

	return JNI_VERSION_1_6;
}

void PrepareExtendFunction(Isolate *isolate, jstring filesPath)
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

	ExceptionUtil::GetInstance()->HandleTryCatch(tc);

	DEBUG_WRITE("Executed prepareExtend.js script");
}

void PrepareV8Runtime(Isolate *isolate, JEnv& env, jstring filesPath, jstring packageName, jstring jsoptions)
{
	string v8flags = ArgConverter::jstringToString(jsoptions);
	V8::SetFlagsFromString(v8flags.c_str(), v8flags.size());
	V8::SetCaptureStackTraceForUncaughtExceptions(true, 100, StackTrace::kOverview);
	V8::AddMessageListener(ExceptionUtil::OnUncaughtError);

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

	context_scope = new Context::Scope(context);

	Module::Init(isolate);

	g_objectManager->Init(isolate);

	auto global = context->Global();

	global->ForceSet(ConvertToV8String("global"), global, readOnlyFlags);
	global->ForceSet(ConvertToV8String("__global"), global, readOnlyFlags);

	ArgConverter::Init(g_jvm);

	NativeScriptRuntime::Init(g_jvm, g_objectManager);

	string pckName = ArgConverter::jstringToString(packageName);
	Profiler::Init(pckName);
	JsDebugger::Init(isolate, pckName);

	//PrepareExtendFunction(isolate, filesPath);

	NativeScriptRuntime::BuildMetadata(env, filesPath);

	NativeScriptRuntime::CreateTopLevelNamespaces(global);
}

extern "C" void Java_com_tns_Platform_initNativeScript(JNIEnv *_env, jobject obj, jstring filesPath, jboolean verboseLoggingEnabled, jstring packageName, jstring jsoptions)
{
	tns::LogEnabled = verboseLoggingEnabled;

	DEBUG_WRITE("Initializing Telerik NativeScript");

	Platform* platform = v8::platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();

	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = &g_allocator;
	g_isolate = Isolate::New(create_params);
	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);
	HandleScope handleScope(isolate);

	ExceptionUtil::GetInstance()->Init(g_jvm, g_objectManager);

	JEnv env(_env);
	PrepareV8Runtime(isolate, env, filesPath, packageName, jsoptions);

	NativeScriptRuntime::APP_FILES_DIR = ArgConverter::jstringToString(filesPath);
	Constants::APP_ROOT_FOLDER_PATH = NativeScriptRuntime::APP_FILES_DIR + "/app/";
}

extern "C" void Java_com_tns_Platform_runModule(JNIEnv *_env, jobject obj, jstring scriptFile)
{
	JEnv env(_env);
	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);

	HandleScope handleScope(isolate);

	string filePath = ArgConverter::jstringToString(scriptFile);
	bool hasError = false;

	auto moduleObj = Module::CompileAndRun(filePath, hasError);
}

extern "C" jobject Java_com_tns_Platform_runScript(JNIEnv *_env, jobject obj, jstring scriptFile)
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

	if (!ExceptionUtil::GetInstance()->HandleTryCatch(tc, "Script " + filename + " contains compilation errors!"))
	{
		if (!script.IsEmpty())
		{
			Local<Value> result;
			auto maybeResult = script->Run(context).ToLocal(&result);

			if (!ExceptionUtil::GetInstance()->HandleTryCatch(tc, "Error running script " + filename))
			{
				if (!result.IsEmpty())
				{
					res = ConvertJsValueToJavaObject(env, result, static_cast<int>(Type::Null));
				}
				else
				{
					DEBUG_WRITE(">>runScript maybeResult is empty");
				}
			}
		}
		else
		{
			DEBUG_WRITE(">>runScript maybeScript is empty");
		}
	}

	return res;
}


jobject ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType)
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

extern "C" jobject Java_com_tns_Platform_callJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs)
{
	SET_PROFILER_FRAME();

	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);

	JEnv env(_env);
	TryCatch tc;

	DEBUG_WRITE("CallJSMethodNative called javaObjectID=%d", javaObjectID);

	HandleScope handleScope(isolate);

	auto jsObject = g_objectManager->GetJsObjectByJavaObject(javaObjectID);
	if (jsObject.IsEmpty())
	{
		stringstream ss;
		ss << "JavaScript object for Java ID " << javaObjectID << " not found." << endl;
		ss << "Attempting to call method " << ArgConverter::jstringToString(methodName) << endl;

		// TODO: Should we kill the Java VM here?
		ExceptionUtil::GetInstance()->ThrowExceptionToJava(tc, ss.str());
		return nullptr;
	}

	if (isConstructor)
	{
		DEBUG_WRITE("CallJSMethodNative: Updating linked instance with its real class");
		jclass instanceClass = env.GetObjectClass(obj);
		g_objectManager->SetJavaClass(jsObject, instanceClass);
	}

	DEBUG_WRITE("CallJSMethodNative called jsObject=%d", jsObject->GetIdentityHash());

	string method_name = ArgConverter::jstringToString(methodName);

	auto jsResult = NativeScriptRuntime::CallJSMethod(env, jsObject, method_name, packagedArgs, tc);

	stringstream ss;
	ss << "Calling js method " << method_name << " failed";
	string exceptionMessage = ss.str();
	if (ExceptionUtil::GetInstance()->HandleTryCatch(tc, exceptionMessage))
	{
		DEBUG_WRITE("%s", exceptionMessage.c_str());
	}

	int classReturnType = retType;
	jobject javaObject = ConvertJsValueToJavaObject(env, jsResult, classReturnType);
	return javaObject;
}

extern "C" void Java_com_tns_Platform_createJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className)
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

	DEBUG_WRITE("createJSInstanceNative class %s", proxyClassName.c_str());
	jsInstance = MetadataNode::CreateExtendedJSWrapper(isolate, proxyClassName);
	if (jsInstance.IsEmpty())
	{
		ExceptionUtil::GetInstance()->ThrowExceptionToJava(tc, "NativeScript application not initialized correctly. Cannot create extended JS wrapper.");
		return;
	}

	implementationObject = MetadataNode::GetImplementationObject(jsInstance);
	if (implementationObject.IsEmpty())
	{
		NativeScriptRuntime::AppFail(nullptr, "createJSInstanceNative: implementationObject is empty");
		return;
	}
	DEBUG_WRITE("createJSInstanceNative: implementationObject :%d", implementationObject->GetIdentityHash());

	jclass clazz = env.FindClass(jniName);
	g_objectManager->Link(jsInstance, javaObjectID, clazz);
}

extern "C" jint Java_com_tns_Platform_generateNewObjectId(JNIEnv *env, jobject obj)
{
	int objectId = g_objectManager->GenerateNewObjectID();

	return objectId;
}


extern "C" void Java_com_tns_Platform_enableVerboseLoggingNative(JNIEnv *env, jobject obj)
{
	tns::LogEnabled = true;
}

extern "C" void Java_com_tns_Platform_disableVerboseLoggingNative(JNIEnv *env, jobject obj)
{
	tns::LogEnabled = false;
}

extern "C" void Java_com_tns_Platform_adjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jlong usedMemory)
{
	Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(usedMemory);
}

extern "C" void Java_com_tns_Platform_passUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace)
{
	auto isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

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
	ExceptionUtil::CallJsFuncWithErr(errObj);
}

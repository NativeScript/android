#include "CallbackHandlers.h"
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
#include "NativeScriptAssert.h"
#include "JsDebugger.h"
#include "SimpleProfiler.h"
#include "SimpleAllocator.h"
#include "JType.h"
#include "Module.h"
#include "NativeScriptException.h"
#include "V8NativeScriptExtension.h"
#include "Runtime.h"
#include "ArrayHelper.h"
#include <sstream>
#include <android/log.h>
#include <string>

using namespace v8;
using namespace std;
using namespace tns;

//TODO: Lubo: properly release this jni global ref on shutdown

Persistent<Context> *PrimaryContext = nullptr;
Context::Scope *context_scope = nullptr;
bool tns::LogEnabled = true;
SimpleAllocator g_allocator;

void Runtime::Init(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.Native", "NativeScript Runtime Version %s, commit %s", NATIVE_SCRIPT_RUNTIME_VERSION, NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);
	DEBUG_WRITE("JNI_ONLoad");

	if (Runtime::s_jvm == nullptr)
	{
		s_jvm = vm;

		JEnv::Init(s_jvm);
	}

	DEBUG_WRITE("JNI_ONLoad END");
}

Runtime::Runtime(JNIEnv *env, jobject runtime, int id)
	: m_env(env), m_id(id), m_isolate(nullptr)
{
	m_runtime = m_env.NewGlobalRef(runtime);
	m_objectManager = new ObjectManager(m_runtime);
	s_id2RuntimeCache.insert(make_pair(id, this));
}

Runtime* Runtime::GetRuntime(int runtimeId)
{
	auto itFound = s_id2RuntimeCache.find(runtimeId);
	auto runtime = (itFound != s_id2RuntimeCache.end())
			? itFound->second
			: nullptr;

	if (runtime == nullptr)
	{
		stringstream ss;
		ss << "Cannot find runtime for id:" << runtimeId;
		throw NativeScriptException(ss.str());
	}

	return runtime;
}

Runtime* Runtime::GetRuntime(v8::Isolate *isolate)
{
	auto itFound = s_isolate2RuntimesCache.find(isolate);
	auto runtime = (itFound != s_isolate2RuntimesCache.end())
			? itFound->second
			: nullptr;

	if (runtime == nullptr)
	{
		stringstream ss;
		ss << "Cannot find runtime for isolate: " << isolate;
		throw NativeScriptException(ss.str());
	}

	return runtime;
}

Isolate* Runtime::GetIsolate() const
{
	return m_isolate;
}

jobject Runtime::GetJavaRuntime() const
{
	return m_runtime;
}

ObjectManager* Runtime::GetObjectManager() const
{
	return m_objectManager;
}

void Runtime::Init(JNIEnv *_env, jobject obj, int runtimeId, jstring filesPath, jboolean verboseLoggingEnabled, jstring packageName, jobjectArray args, jobject jsDebugger)
{
	auto runtime = new Runtime(_env, obj, runtimeId);

	auto enableLog = verboseLoggingEnabled == JNI_TRUE;

	runtime->Init(filesPath, enableLog, packageName, args, jsDebugger);
}

void Runtime::Init(jstring filesPath, bool verboseLoggingEnabled, jstring packageName, jobjectArray args, jobject jsDebugger)
{
	LogEnabled = m_logEnabled = verboseLoggingEnabled;

	auto filesRoot = ArgConverter::jstringToString(filesPath);
	Constants::APP_ROOT_FOLDER_PATH = filesRoot + "/app/";
	// read config options passed from Java
	JniLocalRef v8Flags(m_env.GetObjectArrayElement(args, 0));
	Constants::V8_STARTUP_FLAGS = ArgConverter::jstringToString(v8Flags);
	JniLocalRef cacheCode(m_env.GetObjectArrayElement(args, 1));
	Constants::V8_CACHE_COMPILED_CODE = (bool) cacheCode;
	JniLocalRef snapshotScript(m_env.GetObjectArrayElement(args, 2));
	Constants::V8_HEAP_SNAPSHOT_SCRIPT = ArgConverter::jstringToString(snapshotScript);
	JniLocalRef snapshotBlob(m_env.GetObjectArrayElement(args, 3));
	Constants::V8_HEAP_SNAPSHOT_BLOB = ArgConverter::jstringToString(snapshotBlob);
	JniLocalRef profilerOutputDir(m_env.GetObjectArrayElement(args, 4));

	DEBUG_WRITE("Initializing Telerik NativeScript");

	NativeScriptException::Init(m_objectManager);
	m_isolate = PrepareV8Runtime(filesRoot, packageName, jsDebugger, profilerOutputDir);

	s_isolate2RuntimesCache.insert(make_pair(m_isolate, this));
}

void Runtime::RunModule(JNIEnv *_env, jobject obj, jstring scriptFile)
{
	JEnv env(_env);

	string filePath = ArgConverter::jstringToString(scriptFile);
	m_module.Load(filePath);
}

jobject Runtime::RunScript(JNIEnv *_env, jobject obj, jstring scriptFile)
{
	JEnv env(_env);
	jobject res = nullptr;

	auto isolate = m_isolate;
	auto context = isolate->GetCurrentContext();

	auto filename = ArgConverter::jstringToString(scriptFile);
	auto src = File::ReadText(filename);
	auto source = ConvertToV8String(src);

	TryCatch tc;

	Local<Script> script;
	ScriptOrigin origin(ConvertToV8String(filename));
	auto maybeScript = Script::Compile(context, source, &origin).ToLocal(&script);

	if (tc.HasCaught())
	{
		throw NativeScriptException(tc, "Script " + filename + " contains compilation errors!");
	}

	if (!script.IsEmpty())
	{
		Local<Value> result;
		auto maybeResult = script->Run(context).ToLocal(&result);

		if (tc.HasCaught())
		{
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

jobject Runtime::CallJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs)
{
	SET_PROFILER_FRAME();

	auto isolate = m_isolate;

	JEnv env(_env);

	DEBUG_WRITE("CallJSMethodNative called javaObjectID=%d", javaObjectID);

	auto jsObject = m_objectManager->GetJsObjectByJavaObject(javaObjectID);
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
		m_objectManager->SetJavaClass(jsObject, instanceClass);
	}

	DEBUG_WRITE("CallJSMethodNative called jsObject=%d", jsObject->GetIdentityHash());

	string method_name = ArgConverter::jstringToString(methodName);
	auto jsResult = CallbackHandlers::CallJSMethod(m_isolate, env, jsObject, method_name, packagedArgs);

	int classReturnType = retType;
	jobject javaObject = ConvertJsValueToJavaObject(env, jsResult, classReturnType);
	return javaObject;
}

void Runtime::CreateJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className)
{
	SET_PROFILER_FRAME();

	DEBUG_WRITE("createJSInstanceNative called");

	auto isolate = m_isolate;

	JEnv env(_env);

	string existingClassName = ArgConverter::jstringToString(className);
	string jniName = Util::ConvertFromCanonicalToJniName(existingClassName);
	Local<Object> jsInstance;
	Local<Object> implementationObject;

	auto proxyClassName = m_objectManager->GetClassName(javaObject);
	DEBUG_WRITE("createJSInstanceNative class %s", proxyClassName.c_str());
	jsInstance = MetadataNode::CreateExtendedJSWrapper(isolate, m_objectManager, proxyClassName);

	if (jsInstance.IsEmpty())
	{
		throw NativeScriptException(string("Failed to create JavaScript extend wrapper for class '" + proxyClassName + "'"));
	}

	implementationObject = MetadataNode::GetImplementationObject(isolate, jsInstance);
	if (implementationObject.IsEmpty())
	{
		string msg("createJSInstanceNative: implementationObject is empty");
		throw NativeScriptException(msg);
	}
	DEBUG_WRITE("createJSInstanceNative: implementationObject :%d", implementationObject->GetIdentityHash());

	jclass clazz = env.FindClass(jniName);
	m_objectManager->Link(jsInstance, javaObjectID, clazz);
}

jint Runtime::GenerateNewObjectId(JNIEnv *env, jobject obj)
{
	int objectId = m_objectManager->GenerateNewObjectID();

	return objectId;
}

void Runtime::AdjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jlong usedMemory)
{
	Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(usedMemory);
}

void Runtime::PassUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace)
{
	auto isolate = m_isolate;

	//create error message
	string errMsg = "The application crashed because of an uncaught exception. You can look at \"stackTrace\" or \"nativeException\" for more detailed information about the exception.";
	auto errObj = Exception::Error(ConvertToV8String(errMsg)).As<Object>();

	//create a new native exception js object
	jint javaObjectID = m_objectManager->GetOrCreateObjectId((jobject) exception);
	auto nativeExceptionObject = m_objectManager->GetJsObjectByJavaObject(javaObjectID);

	if (nativeExceptionObject.IsEmpty())
	{
		string className = m_objectManager->GetClassName((jobject) exception);
		//create proxy object that wraps the java err
		nativeExceptionObject = m_objectManager->CreateJSWrapper(javaObjectID, className);
		if (nativeExceptionObject.IsEmpty())
		{
			nativeExceptionObject = Object::New(isolate);
		}
	}

	string stackTraceText = ArgConverter::jstringToString(stackTrace);
	errMsg += "\n" + stackTraceText;

	//create a JS error object
	errObj->Set(V8StringConstants::GetNativeException(), nativeExceptionObject);
	errObj->Set(V8StringConstants::GetStackTrace(), ArgConverter::jstringToV8String(stackTrace));


	if (JsDebugger::IsDebuggerActive())
	{
		__android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "debuger active throwing exception with message %s", errMsg.c_str());

		JsDebugger::ConsoleMessage(errMsg, "error");
		//throwing unhandled error to debugger so catch all and catch unhandled exceptions setting on client can be triggered
		DEBUG_WRITE_FORCE("throwing unhandled error to debugger");
		NativeScriptException(errMsg).ReThrowToV8();
	}
	else
	{
		//pass err to JS
		NativeScriptException::CallJsFuncWithErr(errObj);
	}
}

void Runtime::ClearStartupData(JNIEnv *env, jobject obj) {
	delete m_heapSnapshotBlob;
	delete m_startupData;
}

static void InitializeV8() {
	auto platform = v8::platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();
}

Isolate* Runtime::PrepareV8Runtime(const string& filesPath, jstring packageName, jobject jsDebugger, jstring profilerOutputDir)
{
	Isolate::CreateParams create_params;
	bool didInitializeV8 = false;

	create_params.array_buffer_allocator = &g_allocator;
	// prepare the snapshot blob
	if (!Constants::V8_HEAP_SNAPSHOT_BLOB.empty() || !Constants::V8_HEAP_SNAPSHOT_SCRIPT.empty())
	{
		DEBUG_WRITE_FORCE("Snapshot enabled.");

		m_startupData = new StartupData();

		string snapshotPath;
		bool saveSnapshot = true;
		// we have a precompiled snapshot blob provided - try to load it directly
		if (Constants::V8_HEAP_SNAPSHOT_BLOB.size() > 0)
		{
			snapshotPath = Constants::V8_HEAP_SNAPSHOT_BLOB;
			saveSnapshot = false;
		}
		else
		{
			snapshotPath = filesPath + "/internal/snapshot.blob";
		}

		if (File::Exists(snapshotPath))
		{
			m_heapSnapshotBlob = new MemoryMappedFile(MemoryMappedFile::Open(snapshotPath.c_str()));
			m_startupData->data = static_cast<const char*>(m_heapSnapshotBlob->memory);
			m_startupData->raw_size = m_heapSnapshotBlob->size;

			DEBUG_WRITE_FORCE("Snapshot read %s (%dB).", snapshotPath.c_str(), m_heapSnapshotBlob->size);
		}
		else if (!saveSnapshot)
		{
			DEBUG_WRITE_FORCE("No snapshot file found at %s", snapshotPath.c_str());

		}
		else
		{
			// This should be executed before V8::Initialize, which calls it with false.
			NativeScriptExtension::CpuFeaturesProbe(true);
			InitializeV8();
			didInitializeV8 = true;

			string customScript;

			// check for custom script to include in the snapshot
			if (Constants::V8_HEAP_SNAPSHOT_SCRIPT.size() > 0 && File::Exists(Constants::V8_HEAP_SNAPSHOT_SCRIPT))
			{
				customScript = File::ReadText(Constants::V8_HEAP_SNAPSHOT_SCRIPT);
			}

			DEBUG_WRITE_FORCE("Creating heap snapshot");
			*m_startupData = V8::CreateSnapshotDataBlob(customScript.c_str());

			if (m_startupData->raw_size == 0)
			{
				DEBUG_WRITE_FORCE("Failed to create heap snapshot.");
			}
			else
			{
				bool writeSuccess = File::WriteBinary(snapshotPath, m_startupData->data, m_startupData->raw_size);

				if (!writeSuccess)
				{
					DEBUG_WRITE_FORCE("Failed to save created snapshot.");
				}
				else
				{
					DEBUG_WRITE_FORCE("Saved snapshot of %s (%dB) in %s (%dB)",
							Constants::V8_HEAP_SNAPSHOT_SCRIPT.c_str(), customScript.size(),
							snapshotPath.c_str(), m_startupData->raw_size);
				}
			}
		}

		create_params.snapshot_blob = m_startupData;
	}

	if (!didInitializeV8) {
		InitializeV8();
	}
	auto isolate = Isolate::New(create_params);
	Isolate::Scope isolate_scope(isolate);
	HandleScope handleScope(isolate);

	V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(), Constants::V8_STARTUP_FLAGS.size());
	V8::SetCaptureStackTraceForUncaughtExceptions(true, 100, StackTrace::kOverview);
	V8::AddMessageListener(NativeScriptException::OnUncaughtError);
	__android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "V8 version %s", V8::GetVersion());

	auto globalTemplate = ObjectTemplate::New();

	const auto readOnlyFlags = static_cast<PropertyAttribute>(PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);

	globalTemplate->Set(ConvertToV8String("__log"), FunctionTemplate::New(isolate, CallbackHandlers::LogMethodCallback));
	globalTemplate->Set(ConvertToV8String("__dumpReferenceTables"), FunctionTemplate::New(isolate, CallbackHandlers::DumpReferenceTablesMethodCallback));
	globalTemplate->Set(ConvertToV8String("__debugbreak"), FunctionTemplate::New(isolate, JsDebugger::DebugBreakCallback));
	globalTemplate->Set(ConvertToV8String("__consoleMessage"), FunctionTemplate::New(isolate, JsDebugger::ConsoleMessageCallback));
	globalTemplate->Set(ConvertToV8String("__enableVerboseLogging"), FunctionTemplate::New(isolate, CallbackHandlers::EnableVerboseLoggingMethodCallback));
	globalTemplate->Set(ConvertToV8String("__disableVerboseLogging"), FunctionTemplate::New(isolate, CallbackHandlers::DisableVerboseLoggingMethodCallback));
	globalTemplate->Set(ConvertToV8String("__exit"), FunctionTemplate::New(isolate, CallbackHandlers::ExitMethodCallback));
	globalTemplate->Set(ConvertToV8String("__runtimeVersion"), ConvertToV8String(NATIVE_SCRIPT_RUNTIME_VERSION), readOnlyFlags);

	m_weakRef.Init(isolate, globalTemplate, m_objectManager);

	SimpleProfiler::Init(isolate, globalTemplate);

	CallbackHandlers::CreateGlobalCastFunctions(globalTemplate);

	Local<Context> context = Context::New(isolate, nullptr, globalTemplate);
	PrimaryContext = new Persistent<Context>(isolate, context);

	context_scope = new Context::Scope(context);

	m_objectManager->Init(isolate);

	m_module.Init(isolate);

	auto global = context->Global();

	global->ForceSet(ConvertToV8String("global"), global, readOnlyFlags);
	global->ForceSet(ConvertToV8String("__global"), global, readOnlyFlags);

	ArgConverter::Init(isolate);

	CallbackHandlers::Init(isolate, m_objectManager);

	auto pckName = ArgConverter::jstringToString(packageName);
	auto outputDir = ArgConverter::jstringToString(profilerOutputDir);
	m_profiler.Init(isolate, global, pckName, outputDir);
	JsDebugger::Init(isolate, pckName, jsDebugger);

	MetadataNode::BuildMetadata(filesPath);

	auto enableProfiler = !outputDir.empty();
	MetadataNode::EnableProfiler(enableProfiler);

	MetadataNode::CreateTopLevelNamespaces(isolate, global);

	ArrayHelper::Init(context);

	m_arrayBufferHelper.CreateConvertFunctions(isolate, global, m_objectManager);

	return isolate;
}

jobject Runtime::ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType)
{
	JsArgToArrayConverter argConverter(m_isolate, value, false/*is implementation object*/, classReturnType);
	jobject jr = argConverter.GetConvertedArg();
	jobject javaResult = nullptr;
	if (jr != nullptr)
	{
		javaResult = env.NewLocalRef(jr);
	}

	return javaResult;
}

void Runtime::PrepareExtendFunction(Isolate *isolate, jstring filesPath)
{
	string fullPath = ArgConverter::jstringToString(filesPath);
	fullPath.append("/internal/prepareExtend.js");

	int length;
	bool isNew;
	const char* content = File::ReadText(fullPath, length, isNew);

	TryCatch tc;
	auto cmd = ConvertToV8String(content, length);

	if (isNew)
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

	if (tc.HasCaught())
	{
		throw NativeScriptException(tc);
	}

	DEBUG_WRITE("Executed prepareExtend.js script");
}

JavaVM* Runtime::s_jvm = nullptr;
map<int, Runtime*> Runtime::s_id2RuntimeCache;
map<Isolate*, Runtime*> Runtime::s_isolate2RuntimesCache;

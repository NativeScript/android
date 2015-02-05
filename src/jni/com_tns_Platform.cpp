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
#include "Version.h"
#include "JEnv.h"
#include "WeakRef.h"
#include "NativeScriptAssert.h"
#include "JsDebugger.h"
#include <sstream>
#include <android/log.h>
#include <assert.h>
#include <string>


using namespace v8;
using namespace std;
using namespace tns;

void AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

//TODO: Lubo: properly release this jni global ref on shutdown
JavaVM *g_jvm = nullptr;
Persistent<Context> *PrimaryContext = nullptr;
int AppJavaObjectID = -1;
int count = 0;
Context::Scope *context_scope = nullptr;
bool tns::LogEnabled = true;

ObjectManager *objectManager = nullptr;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_INFO, "TNS.Native", "NativeScript Runtime Version %s, commit %s", NATIVE_SCRIPT_RUNTIME_VERSION, NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);
	DEBUG_WRITE("JNI_ONLoad");

	g_jvm = vm;

	JEnv::Init(g_jvm);
	JsArgConverter::Init(g_jvm);
	JsArgToArrayConverter::Init(g_jvm);

	objectManager = new ObjectManager();
	ExceptionUtil::GetInstance()->Init(g_jvm, objectManager);

	NativeScriptRuntime::Init(g_jvm, objectManager);

	DEBUG_WRITE("JNI_ONLoad END");

	return JNI_VERSION_1_6;
}

void PrepareExtendFunction(Isolate *isolate, jstring filesPath)
{
	string fullPath = ArgConverter::jstringToString(filesPath);
	fullPath.append("/internal/prepareExtend.js");

	FILE *f = fopen(fullPath.c_str(), "rb");
	assert(f != nullptr);
	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	char *content = new char[len];
	rewind(f);
	fread(content, 1, len, f);
	fclose(f);

	string s(content, len);
	delete[] content;

	TryCatch tc;

	auto cmd = ConvertToV8String(s);
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

void PrepareV8Runtime(Isolate *isolate, JEnv& env, jstring filesPath, jstring packageName, int debuggerPort)
{
	const char v8flags[] = "--expose_gc";
	V8::SetFlagsFromString(v8flags, sizeof(v8flags) - 1);
	V8::SetCaptureStackTraceForUncaughtExceptions(true, 100, StackTrace::kOverview);
	V8::AddMessageListener(ExceptionUtil::OnUncaughtError);

	auto globalTemplate = ObjectTemplate::New();

	globalTemplate->Set(ConvertToV8String("__startNativeScriptProfiler"), FunctionTemplate::New(isolate, NativeScriptRuntime::StartProfilerCallback));
	globalTemplate->Set(ConvertToV8String("__stopNativeScriptProfiler"), FunctionTemplate::New(isolate, NativeScriptRuntime::StopProfilerCallback));
	globalTemplate->Set(ConvertToV8String("Log"), FunctionTemplate::New(isolate, NativeScriptRuntime::LogMethodCallback));
	globalTemplate->Set(ConvertToV8String("dumpReferenceTables"), FunctionTemplate::New(isolate, NativeScriptRuntime::DumpReferenceTablesMethodCallback));
	globalTemplate->Set(ConvertToV8String("waitForDebugger"), FunctionTemplate::New(isolate, NativeScriptRuntime::WaitForDebuggerMethodCallback));
	globalTemplate->Set(ConvertToV8String("enableVerboseLogging"), FunctionTemplate::New(isolate, NativeScriptRuntime::EnableVerboseLoggingMethodCallback));
	globalTemplate->Set(ConvertToV8String("disableVerboseLogging"), FunctionTemplate::New(isolate, NativeScriptRuntime::DisableVerboseLoggingMethodCallback));
	globalTemplate->Set(ConvertToV8String("heapSnapshot"), FunctionTemplate::New(isolate, NativeScriptRuntime::HeapSnapshotMethodCallback));
	globalTemplate->Set(ConvertToV8String("fail"), FunctionTemplate::New(isolate, NativeScriptRuntime::FailMethodCallback));
	globalTemplate->Set(ConvertToV8String("require"), FunctionTemplate::New(isolate, NativeScriptRuntime::RequireCallback));
	globalTemplate->Set(ConvertToV8String("WeakRef"), FunctionTemplate::New(isolate, WeakRef::ConstructorCallback));

	NativeScriptRuntime::CreateGlobalCastFunctions(globalTemplate);

	Local<Context> context = Context::New(isolate, nullptr, globalTemplate);
	PrimaryContext = new Persistent<Context>(isolate, context);

	context_scope = new Context::Scope(context);

	auto global = context->Global();

	auto appTemplate = ObjectTemplate::New();
	appTemplate->Set(ConvertToV8String("init"), FunctionTemplate::New(isolate, AppInitCallback));
	auto appInstance = appTemplate->NewInstance();
	global->Set(ConvertToV8String("app"), appInstance, PropertyAttribute(ReadOnly | DontDelete));

	global->Set(ConvertToV8String("global"), global, PropertyAttribute(ReadOnly | DontDelete));

	ArgConverter::Init(g_jvm);

	string pckName = ArgConverter::jstringToString(packageName);
	JsDebugger::Init(pckName, debuggerPort);

	PrepareExtendFunction(isolate, filesPath);

	NativeScriptRuntime::BuildMetadata(env, filesPath);

	NativeScriptRuntime::CreateTopLevelNamespaces(global);
}

extern "C" void Java_com_tns_Platform_initNativeScript(JNIEnv *_env, jobject obj, jstring filesPath, jint appJavaObjectId, jboolean verboseLoggingEnabled, jstring packageName, jint debuggerPort)
{
	AppJavaObjectID = appJavaObjectId;
	tns::LogEnabled = verboseLoggingEnabled;

	DEBUG_WRITE("Initializing Telerik NativeScript: app instance id:%d", appJavaObjectId);

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	JEnv env(_env);
	PrepareV8Runtime(isolate, env, filesPath, packageName, debuggerPort);

	NativeScriptRuntime::APP_FILES_DIR = ArgConverter::jstringToString(filesPath);
}

extern "C" void Java_com_tns_Platform_runNativeScript(JNIEnv *_env, jobject obj, jstring appModuleName, jstring appCode)
{
	JEnv env(_env);

	Isolate *isolate = nullptr;
	isolate = Isolate::GetCurrent();

	TryCatch tc;

	HandleScope handleScope(isolate);
	auto context = Local<Context>::New(isolate, *PrimaryContext);

	jstring retval;
	jboolean isCopy;

	const char* code = env.GetStringUTFChars(appCode, &isCopy);

	auto cmd = ConvertToV8String(code);

	env.ReleaseStringUTFChars(appCode, code);

	DEBUG_WRITE("Compiling script");

	auto moduleName = ArgConverter::jstringToV8String(appModuleName);

	auto script = Script::Compile(cmd, moduleName);

	DEBUG_WRITE("Compile script");

	if (ExceptionUtil::GetInstance()->HandleTryCatch(tc))
	{
		ExceptionUtil::GetInstance()->HandleInvalidState("Bootstrap script has error(s).", true);
	}
	else if (script.IsEmpty())
	{
		ExceptionUtil::GetInstance()->HandleInvalidState("Bootstrap script is empty.", true);
	}
	else
	{
		DEBUG_WRITE("Running script");

		int debuggerPort = JsDebugger::GetDebuggerPort();
		if (debuggerPort > 0)
		{
			string packageName = JsDebugger::GetPackageName();
			JsDebugger::EnableAgent(packageName, debuggerPort, true);
		}

		auto appModuleObj = script->Run();
		if (ExceptionUtil::GetInstance()->HandleTryCatch(tc))
		{
			// TODO: Fail?
		}
		else if (!appModuleObj.IsEmpty() && appModuleObj->IsFunction())
		{
			auto moduleFunc = appModuleObj.As<Function>();
			Handle<Value> exportsObj = Object::New(isolate);
			auto thiz = Object::New(isolate);
			auto res = moduleFunc->Call(thiz, 1, &exportsObj);

			ExceptionUtil::GetInstance()->HandleTryCatch(tc);
		}
		else
		{
			ExceptionUtil::GetInstance()->HandleInvalidState("Error running NativeScript bootstrap code.", true);
		}

		// TODO: Why do we need this line?
		auto persistentAppModuleObject = new Persistent<Object>(Isolate::GetCurrent(), appModuleObj.As<Object>());
	}

	//NativeScriptRuntime::loadedModules.insert(make_pair(appModuleName, persistentAppModuleObject));

	//DEBUG_WRITE("Forcing V8 garbage collection");
	//while (!V8::IdleNotification());
}

void AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	ASSERT_MESSAGE(args.Length() == 1, "Application should be initialized with single parameter");
	ASSERT_MESSAGE(args[0]->IsObject(), "Application should be initialized with single object parameter containing overridden methods");

	auto isolate = Isolate::GetCurrent();

	// TODO: find another way to get "com/tns/NativeScriptApplication" metadata (move it to more appropriate place)
	auto node = MetadataNode::GetOrCreate("com/tns/NativeScriptApplication");
	auto appInstance = node->CreateInstanceProxy(isolate);
	DEBUG_WRITE("Application object created id: %d", appInstance->GetIdentityHash());

	auto implementationObject = args[0]->ToObject();
	implementationObject->SetHiddenValue(V8StringConstants::GetClassImplementationObject(), External::New(isolate, node));
	DEBUG_WRITE("Application object implementation object is with id: %d", implementationObject->GetIdentityHash());
	bool appSuccess = appInstance->SetPrototype(implementationObject);
	ASSERT_MESSAGE(appSuccess == true, "Application could not be initialized correctly");

	jweak applicationObject = objectManager->GetJavaObjectByID(AppJavaObjectID);

	JEnv env;
	jclass appClass = env.FindClass("com/tns/NativeScriptApplication");
	objectManager->Link(appInstance, AppJavaObjectID, appClass);

	JniLocalRef applicationClass(env.GetObjectClass(applicationObject));
	jmethodID setNativeScriptOverridesMethodId = env.GetMethodID((jclass)applicationClass, "setNativeScriptOverrides", "([Ljava/lang/String;)V");
	jobjectArray methodOverrides = NativeScriptRuntime::GetMethodOverrides(env, implementationObject);
	env.CallVoidMethod(applicationObject, setNativeScriptOverridesMethodId, methodOverrides);

	ExceptionUtil::GetInstance()->CheckForJavaException(env);
}

jobject ConvertJsValueToJavaObject(JEnv& env, const Handle<Value>& value)
{
	JsArgToArrayConverter argConverter(value, false);
	jobject jr = argConverter.GetConvertedArg();
	jobject javaResult = nullptr;
	if (jr != nullptr)
	{
		javaResult = env.NewLocalRef(jr);
	}

	return javaResult;
}

extern "C" jobject Java_com_tns_Platform_callJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jboolean isConstructor, jobjectArray packagedArgs)
{
	JEnv env(_env);

	DEBUG_WRITE("CallJSMethodNative called javaObjectID=%d", javaObjectID);

	auto isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	auto jsObject = objectManager->GetJsObjectByJavaObject(javaObjectID);
	if (jsObject.IsEmpty())
	{
		stringstream ss;
		ss << "JavaScript object for Java ID " << javaObjectID << " not found." << endl;
		ss << "Attempting to call method " << ArgConverter::jstringToString(methodName) << endl;

		// TODO: Should we kill the Java VM here?
		ExceptionUtil::GetInstance()->HandleInvalidState(ss.str(), true);
		return nullptr;
	}

	if (isConstructor)
	{
		DEBUG_WRITE("CallJSMethodNative: Updating linked instance with its real class");
		jclass instanceClass = env.GetObjectClass(obj);
		objectManager->SetJavaClass(jsObject, instanceClass);
	}

	DEBUG_WRITE("CallJSMethodNative called jsObject=%d", jsObject->GetIdentityHash());

	string method_name = ArgConverter::jstringToString(methodName);

	TryCatch tc;
	auto jsResult = NativeScriptRuntime::CallJSMethod(env, jsObject, methodName, packagedArgs, tc);

	if (tc.HasCaught())
	{
		ExceptionUtil::GetInstance()->CheckForException(isolate, method_name, tc);
	}

	jobject javaObject = ConvertJsValueToJavaObject(env, jsResult);
	return javaObject;
}

extern "C" jobjectArray Java_com_tns_Platform_createJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring canonicalName, jboolean createActivity, jobjectArray packagedCreationArgs)
{
	DEBUG_WRITE("createJSInstanceNative called");

	JEnv env(_env);

	auto isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);
	// TODO: Do we need a TryCatch here? It is currently not used anywhere
	// TryCatch tc;

	string existingClassCanonicalName = ArgConverter::jstringToString(canonicalName);
	string jniName = Util::ConvertFromCanonicalToJniName(existingClassCanonicalName);
	Handle<Object> jsInstance;
	Handle<Object> implementationObject;
	Handle<Object> classProxy;

	bool isActivity = createActivity == JNI_TRUE;

	if (isActivity)
	{
		auto appInstance = objectManager->GetJsObjectByJavaObject(AppJavaObjectID);
		if (appInstance.IsEmpty())
		{
			ExceptionUtil::GetInstance()->HandleInvalidState("NativeScript application not initialized correctly. Missing the global app object initialization.", true);
			return nullptr;
		}

		string createActivityMethodName = "getActivity";
		auto createActivityFunction = appInstance->Get(ConvertToV8String(createActivityMethodName.c_str()));

		if (createActivityFunction.IsEmpty() || !createActivityFunction->IsFunction())
		{
			ExceptionUtil::GetInstance()->HandleInvalidState("NativeScript application not initialized correctly. No function 'createActivity' found on the application object.", true);
			return nullptr;
		}

		JniLocalRef jMethodName(env.NewStringUTF(createActivityMethodName.c_str()));
		TryCatch tc;
		auto jsResult = NativeScriptRuntime::CallJSMethod(env, appInstance, jMethodName, packagedCreationArgs, tc);
		jsInstance = jsResult.As<Object>();
		if (jsInstance.IsEmpty() || jsInstance->IsNull() || jsInstance->IsUndefined())
		{
			ExceptionUtil::GetInstance()->HandleInvalidState("NativeScript application not initialized correctly. getActivity method returned invalid value.", true);
			return nullptr;
		}

		auto node = MetadataNode::GetNodeFromHandle(jsInstance);
		string nodeName = node->GetName();

		if (nodeName != jniName)
		{
			ExceptionUtil::GetInstance()->HandleInvalidState("NativeScript application not initialized correctly. createActivity returned wrong type.", true);
			return nullptr;
		}

		classProxy = jsInstance;
	}
	else
	{
		DEBUG_WRITE("createJSInstanceNative class %s", jniName.c_str());
		classProxy = MetadataNode::GetExistingClassProxy(jniName);
		if (classProxy.IsEmpty())
		{
			string nativeScriptBindingPrefix("com/tns/");
			if (Util::StartsWith(jniName, nativeScriptBindingPrefix))
			{
					jniName = jniName.substr(nativeScriptBindingPrefix.length());
					classProxy = MetadataNode::GetExistingClassProxy(jniName);
			}
			if (classProxy.IsEmpty())
			{
				//TODO: support creating js instances after java instances with no implementations defined
				//implementationObject = CreateJSProxyInstance(javaObjectID, jstringToString(env, canonicalName));
				NativeScriptRuntime::APP_FAIL("createJSInstanceNative: classProxy not found when js instance should be created after java instance");
			}
		}
	}

	implementationObject = MetadataNode::GetImplementationObject(classProxy);
	if (implementationObject.IsEmpty())
	{
		NativeScriptRuntime::APP_FAIL("createJSInstanceNative: implementationObject is empty");
		return nullptr;
	}
	DEBUG_WRITE("createJSInstanceNative: implementationObject :%d", implementationObject->GetIdentityHash());

	if (implementationObject.IsEmpty())
	{
		//TODO: support creating js instances after java instances with no implementations defined
		//implementationObject = CreateJSProxyInstance(javaObjectID, jstringToString(env, canonicalName));
		NativeScriptRuntime::APP_FAIL("createJSInstanceNative: classProxy.implementationObject not found when js instance should be created after java instance");
	}

	auto node = MetadataNode::GetNodeFromHandle(classProxy);
	string name;

	bool createTypeScriptActivity = isActivity && classProxy->HasOwnProperty(V8StringConstants::GetIsPrototypeImplementationObject());

	if (isActivity && !createTypeScriptActivity)
	{
		DEBUG_WRITE("createJSInstanceNative: creating js activity with classProxy: %d", classProxy->GetIdentityHash());
	}

	if (createTypeScriptActivity)
	{
		DEBUG_WRITE("createJSInstanceNative: creating typescript activity with classProxy: %d", classProxy->GetIdentityHash());

		//call extends function on the prototype which must the be Parent function
		auto parent = classProxy->GetPrototype().As<Function>();
		auto extendsFunc = parent->Get(V8StringConstants::GetExtends()).As<Function>();
		ASSERT_MESSAGE(!extendsFunc.IsEmpty(), "extends not found on activity parent");


		Handle<Value> arguments[1];
		arguments[0] = implementationObject;
		auto extended =  extendsFunc->Call(parent, 1, arguments).As<Function>();
		ASSERT_MESSAGE(!extended.IsEmpty(), "extends result is not an function");

		extended->SetHiddenValue(ConvertToV8String("t::TypescriptActivity::DonNotRegisterInstance"), Boolean::New(isolate, true));
		auto extendedObject = extended->CallAsConstructor(0, nullptr).As<Object>();
		DEBUG_WRITE("createJSInstanceNative: typescript activity extendedObject's prototype : %d", extendedObject->GetPrototype().As<Object>()->GetIdentityHash());

		extendedObject->SetPrototype(implementationObject);
		extendedObject->SetHiddenValue(ConvertToV8String(MetadataNode::METADATA_NODE_KEY_NAME), External::New(isolate, node));

		auto objectTemplate = ObjectTemplate::New();
		auto instance = objectTemplate->NewInstance();
		instance->SetPrototype(extendedObject);
		bool success = extendedObject->SetHiddenValue(ConvertToV8String("t::ActivityImplementationObject"), implementationObject);
		ASSERT_MESSAGE(success == true, "Setting up the typescript activity implementation object failed");
		DEBUG_WRITE("createJSInstanceNative: typescript activity instance: %d", instance->GetIdentityHash());
		DEBUG_WRITE("createJSInstanceNative: typescript activity extendedObject: %d", extendedObject->GetIdentityHash());

		//This will cause the Link to link the extendedObject not the empty object instance. This is on par with all typescript objects created through JS
		jsInstance = extendedObject;
	}
	else
	{
		jsInstance = MetadataNode::CreateJSInstance(classProxy, node, implementationObject, name);
	}

	jclass clazz = env.FindClass(jniName);
	objectManager->Link(jsInstance, javaObjectID, clazz);

	vector<jstring> methods;

	auto propNames = implementationObject->GetPropertyNames();
	for (int i = 0; i < propNames->Length(); i++)
	{
		auto name = propNames->Get(i).As<String>();
		auto method = implementationObject->Get(name);

		if (method->IsFunction())
		{
			String::Utf8Value stringValue(name);
			jstring value = env.NewStringUTF(*stringValue);

			methods.push_back(value);
		}
	}

	jclass stringClass = env.FindClass("java/lang/String");
	jobjectArray methodOverrides = env.NewObjectArray(methods.size(), stringClass, nullptr);
	for (int i = 0; i < methods.size(); i++)
	{
		env.SetObjectArrayElement(methodOverrides, i, methods[i]);
	}

	return methodOverrides;
}

extern "C" jint Java_com_tns_Platform_generateNewObjectId(JNIEnv *env, jobject obj)
{
	int objectId = objectManager->GenerateNewObjectID();

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
	jint javaObjectID = objectManager->GetOrCreateObjectId((jobject) exception);
	auto nativeExceptionObject = objectManager->GetJsObjectByJavaObject(javaObjectID);

	if (nativeExceptionObject.IsEmpty())
	{
		string className = objectManager->GetClassName((jobject)exception);
		//create proxy object that wraps the java err
		nativeExceptionObject = objectManager->CreateJSProxyInstance(javaObjectID, className);
	}

	//create a JS error object
	errObj->Set(V8StringConstants::GetNativeException(), nativeExceptionObject);
	errObj->Set(V8StringConstants::GetStackTrace(), ArgConverter::jstringToV8String(stackTrace));

	//pass err to JS
	ExceptionUtil::CallJFuncWithErr(errObj);
}

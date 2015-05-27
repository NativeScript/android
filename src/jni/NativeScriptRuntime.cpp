#include "NativeScriptRuntime.h"
#include "NativeScriptAssert.h"
#include "MetadataNode.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "JniLocalRef.h"
#include "JsArgConverter.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "v8-profiler.h"
#include "Constants.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <cstdio>
#include "JavaObjectArrayCache.h"
#include "MethodCache.h"
#include "JsDebugger.h"
#include "SimpleProfiler.h"

using namespace v8;
using namespace std;
using namespace tns;

// init
void NativeScriptRuntime::Init(JavaVM *jvm, ObjectManager *objectManager)
{
	NativeScriptRuntime::jvm = jvm;

	JEnv env;

	JAVA_LANG_STRING = env.FindClass("java/lang/String");
	assert(JAVA_LANG_STRING != nullptr);

	PlatformClass = env.FindClass("com/tns/Platform");
	assert(PlatformClass != nullptr);

	RequireClass = env.FindClass("com/tns/Require");
	assert(RequireClass != nullptr);

	MAKE_CLASS_INSTANCE_OF_TYPE_STRONG = env.GetStaticMethodID(PlatformClass, "makeClassInstanceOfTypeStrong", "(Ljava/lang/String;)I");
	assert(MAKE_CLASS_INSTANCE_OF_TYPE_STRONG != nullptr);

	CREATE_INSTANCE_METHOD_ID = env.GetStaticMethodID(PlatformClass, "createInstance", "([Ljava/lang/Object;II)Ljava/lang/Object;");
	assert(CREATE_INSTANCE_METHOD_ID != nullptr);

	CACHE_CONSTRUCTOR_METHOD_ID = env.GetStaticMethodID(PlatformClass, "cacheConstructor", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/Object;[Ljava/lang/String;)I");
	assert(CACHE_CONSTRUCTOR_METHOD_ID != nullptr);

	GET_TYPE_METADATA = env.GetStaticMethodID(PlatformClass, "getTypeMetadata", "(Ljava/lang/String;I)[Ljava/lang/String;");
	assert(GET_TYPE_METADATA != nullptr);

	APP_FAIL_METHOD_ID = env.GetStaticMethodID(PlatformClass, "APP_FAIL", "(Ljava/lang/String;)V");
	assert(APP_FAIL_METHOD_ID != nullptr);

	GET_MODULE_CONTENT_METHOD_ID = env.GetStaticMethodID(RequireClass, "getModuleContent", "(Ljava/lang/String;)Ljava/lang/String;");
	assert(GET_MODULE_CONTENT_METHOD_ID != nullptr);

	GET_MODULE_PATH_METHOD_ID = env.GetStaticMethodID(RequireClass, "getModulePath", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	assert(GET_MODULE_PATH_METHOD_ID != nullptr);

	ENABLE_VERBOSE_LOGGING_METHOD_ID = env.GetStaticMethodID(PlatformClass, "enableVerboseLogging", "()V");
	assert(ENABLE_VERBOSE_LOGGING_METHOD_ID != nullptr);

	DISABLE_VERBOSE_LOGGING_METHOD_ID = env.GetStaticMethodID(PlatformClass, "disableVerboseLogging", "()V");
	assert(ENABLE_VERBOSE_LOGGING_METHOD_ID != nullptr);

	GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID = env.GetStaticMethodID(PlatformClass, "getChangeInBytesOfUsedMemory", "()J");
	assert(GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID != nullptr);

	MetadataNode::SubscribeCallbacks(objectManager,
									NativeScriptRuntime::GetJavaField,
									NativeScriptRuntime::SetJavaField,
									NativeScriptRuntime::GetArrayElement,
									NativeScriptRuntime::SetArrayElement,
									NativeScriptRuntime::CallJavaMethod,
									NativeScriptRuntime::RegisterInstance,
									NativeScriptRuntime::GetTypeMetadata,
									NativeScriptRuntime::FindClass,
									NativeScriptRuntime::GetArrayLength);

	NativeScriptRuntime::objectManager = objectManager;

	fieldAccessor.Init(jvm, objectManager);

	arrayElementAccessor.Init(jvm, objectManager);

	JavaObjectArrayCache::Init(jvm);

	MethodCache::Init();
}

bool NativeScriptRuntime::RegisterInstance(const Handle<Object>& jsObject, const std::string& name, const string& className, const ArgsWrapper& argWrapper, const Handle<Object>& implementationObject, bool isInterface)
{
	bool success;

	DEBUG_WRITE("RegisterInstance called for '%s-%s'", className.c_str(), name.c_str());

	int javaObjectID = objectManager->GenerateNewObjectID();

	JEnv env;
	DEBUG_WRITE("RegisterInstance: Linking new instance");
	objectManager->Link(jsObject, javaObjectID, nullptr);

	jobject instance = CreateJavaInstance(javaObjectID, name, className, argWrapper, implementationObject, isInterface);
	JniLocalRef localInstance(instance);
	success = !localInstance.IsNull();

	if (success)
	{
		DEBUG_WRITE("RegisterInstance: Updating linked instance with its real class");
		jclass clazz = env.GetObjectClass(instance);
		JniLocalRef clazzLocalRef(clazz);
		string instanceClassName = objectManager->GetClassName(clazz);
		jclass instanceClass = env.FindClass(instanceClassName);
		objectManager->SetJavaClass(jsObject, instanceClass);
	}
	else
	{
		DEBUG_WRITE("RegisterInstance failed with null new instance");
	}

	return success;
}

void NativeScriptRuntime::MakeClassInstanceOfTypeStrong(const string& className, const Handle<Object>& classObj)
{
	JEnv env;

	JniLocalRef param(env.NewStringUTF(className.c_str()));
	jint javaObjectID = env.CallStaticIntMethod(PlatformClass, MAKE_CLASS_INSTANCE_OF_TYPE_STRONG, (jstring)param);

	jclass clazz = env.FindClass(className);
	objectManager->Link(classObj, javaObjectID, clazz);
}

Handle<Value> NativeScriptRuntime::GetArrayElement(const Handle<Object>& array, uint32_t index, const string& arraySignature)
{
	return arrayElementAccessor.GetArrayElement(array, index, arraySignature);
}

void NativeScriptRuntime::SetArrayElement(const Handle<Object>& array, uint32_t index, const string& arraySignature, Handle<Value>& value)
{
	JEnv env;

	arrayElementAccessor.SetArrayElement(array, index, arraySignature, value);

	ExceptionUtil::GetInstance()->CheckForJavaException(env);
}

Handle<Value> NativeScriptRuntime::GetJavaField(const Handle<Object>& caller, FieldCallbackData *fieldData)
{
	return fieldAccessor.GetJavaField(caller, fieldData);
}

void NativeScriptRuntime::SetJavaField(const Handle<Object>& target, const Handle<Value>& value, FieldCallbackData *fieldData)
{
	fieldAccessor.SetJavaField(target, value, fieldData);
}


void NativeScriptRuntime::CallJavaMethod(const Handle<Object>& caller, const string& className, const string& methodName, MetadataEntry *entry, bool isStatic, bool isSuper, const v8::FunctionCallbackInfo<v8::Value>& args)
{
	SET_PROFILER_FRAME();

	JEnv env;

	jclass clazz;
	jmethodID mid;
	string sig;

	if ((entry != nullptr) && entry->isResolved)
	{
		isStatic = entry->isStatic;
		if (entry->memberId == nullptr)
		{
			entry->clazz = env.FindClass(className);
			entry->memberId = isStatic
								? env.GetStaticMethodID(entry->clazz, methodName, entry->sig)
								: env.GetMethodID(entry->clazz, methodName, entry->sig);
		}
		clazz = entry->clazz;
		mid = reinterpret_cast<jmethodID>(entry->memberId);
		sig = entry->sig;
	}
	else
	{
		auto mi = MethodCache::ResolveMethodSignature(className, methodName, args, isStatic);
		if (mi.mid == nullptr)
		{
			DEBUG_WRITE("Cannot resolve class=%s, method=%s, isStatic=%d, isSuper=%d", className.c_str(), methodName.c_str(), isStatic, isSuper);
			return;
		}
		clazz = mi.clazz;
		mid = mi.mid;
		sig = mi.signature;
	}

	if (!isStatic)
	{
		DEBUG_WRITE("CallJavaMethod called %s.%s. Instance id: %d, isSuper=%d", className.c_str(), methodName.c_str(), caller.IsEmpty() ? -42 : caller->GetIdentityHash(), isSuper);
	}
	else
	{
		DEBUG_WRITE("CallJavaMethod called %s.%s. static method", className.c_str(), methodName.c_str());
	}

	JsArgConverter argConverter(args, false, sig);

	if (!argConverter.IsValid())
	{
		JsArgConverter::Error err = argConverter.GetError();
		ExceptionUtil::GetInstance()->ThrowExceptionToJs(err.msg);
		return;
	}

	auto isolate = Isolate::GetCurrent();

	jweak callerJavaObject;

	jvalue* javaArgs = argConverter.ToArgs();

	if (!isStatic)
	{
		callerJavaObject = objectManager->GetJavaObjectByJsObject(caller);
		if(callerJavaObject == nullptr)
		{
			stringstream ss;
			ss << "No java object found on which to call \"" << methodName << "\" method. It is possible your Javascript object is not linked with the corresponding Java class. Try passing context(this) to the constructor function.";
			string exceptionMessage = ss.str();
			isolate->ThrowException(v8::Exception::ReferenceError(ConvertToV8String(exceptionMessage)));
			return;
		}
	}

	auto returnType = GetReturnType(sig);

	switch (returnType[0])
	{
		case 'V': //void
		{
			if (isStatic)
			{
				env.CallStaticVoidMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				env.CallNonvirtualVoidMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				env.CallVoidMethodA(callerJavaObject, mid, javaArgs);
			}
			break;
		}
		case 'Z': //bool
		{
			jboolean result;
			if (isStatic)
			{
				result = env.CallStaticBooleanMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualBooleanMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallBooleanMethodA(callerJavaObject, mid, javaArgs);
			}
			args.GetReturnValue().Set(result != 0 ? True(isolate) : False(isolate));
			break;
		}
		case 'B': //byte
		{
			jbyte result;
			if (isStatic)
			{
				result = env.CallStaticByteMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualByteMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallByteMethodA(callerJavaObject, mid, javaArgs);
			}
			args.GetReturnValue().Set(result);
			break;
		}
		case 'C': //char
		{
			jchar result;
			if (isStatic)
			{
				result = env.CallStaticCharMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualCharMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallCharMethodA(callerJavaObject, mid, javaArgs);
			}

			JniLocalRef str(env.NewString(&result, 1));
			jboolean bol = true;
			const char* resP = env.GetStringUTFChars(str, &bol);
			env.ReleaseStringUTFChars(str, resP);
			args.GetReturnValue().Set(ConvertToV8String(resP, 1));
			break;
		}
		case 'S': //short
		{
			jshort result;
			if (isStatic)
			{
				result = env.CallStaticShortMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualShortMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallShortMethodA(callerJavaObject, mid, javaArgs);
			}
			args.GetReturnValue().Set(result);
			break;
		}
		case 'I': //int
		{
			jint result;
			if (isStatic)
			{
				result = env.CallStaticIntMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualIntMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallIntMethodA(callerJavaObject, mid, javaArgs);
			}
			args.GetReturnValue().Set(result);
			break;

		}
		case 'J': //long
		{
			jlong result;
			if (isStatic)
			{
				result = env.CallStaticLongMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualLongMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallLongMethodA(callerJavaObject, mid, javaArgs);
			}
			auto jsLong = ArgConverter::ConvertFromJavaLong(result);
			args.GetReturnValue().Set(jsLong);
			break;
		}
		case 'F': //float
		{
			jfloat result;
			if (isStatic)
			{
				result = env.CallStaticFloatMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualFloatMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallFloatMethodA(callerJavaObject, mid, javaArgs);
			}
			args.GetReturnValue().Set((double) result); //TODO: handle float value here correctly.
			break;
		}
		case 'D': //double
		{
			jdouble result;
			if (isStatic)
			{
				result = env.CallStaticDoubleMethodA(clazz, mid, javaArgs);
			}
			else if (isSuper)
			{
				result = env.CallNonvirtualDoubleMethodA(callerJavaObject, clazz, mid, javaArgs);
			}
			else
			{
				result = env.CallDoubleMethodA(callerJavaObject, mid, javaArgs);
			}
			args.GetReturnValue().Set(result);
			break;

		}
		case '[':
		case 'L':
		{
			bool isString = returnType == "Ljava/lang/String;";

			jobject result = nullptr;
			bool exceptionOccurred;

			if (isString)
			{
				if (isStatic)
				{
					result = env.CallStaticObjectMethodA(clazz, mid, javaArgs);
				}
				else if (isSuper)
				{
					result = env.CallNonvirtualObjectMethodA(callerJavaObject, clazz, mid, javaArgs);
				}
				else
				{
					result = env.CallObjectMethodA(callerJavaObject, mid, javaArgs);
				}

				exceptionOccurred = env.ExceptionCheck() == JNI_TRUE;

				if (!exceptionOccurred)
				{
					auto resultV8String = ArgConverter::jstringToV8String((jstring) result);
					args.GetReturnValue().Set(resultV8String);
				}
			}
			else
			{
				if (isStatic)
				{
					result = env.CallStaticObjectMethodA(clazz, mid, javaArgs);
				}
				else if (isSuper)
				{
					result = env.CallNonvirtualObjectMethodA(callerJavaObject, clazz, mid, javaArgs);
				}
				else
				{
					result = env.CallObjectMethodA(callerJavaObject, mid, javaArgs);
				}

				exceptionOccurred = env.ExceptionCheck() == JNI_TRUE;

				if (!exceptionOccurred)
				{
					if (result != nullptr)
					{
						isString = env.IsInstanceOf(result, JAVA_LANG_STRING);

						Handle<Value> objectResult;
						if (isString)
						{
							objectResult = ArgConverter::jstringToV8String((jstring)result);
						}
						else
						{
							jint javaObjectID = objectManager->GetOrCreateObjectId(result);
							objectResult = objectManager->GetJsObjectByJavaObject(javaObjectID);

							if (objectResult.IsEmpty())
							{
								objectResult = objectManager->CreateJSWrapper(javaObjectID, returnType, result);
							}
						}

						args.GetReturnValue().Set(objectResult);
					}
					else
					{
						args.GetReturnValue().Set(Null(isolate));
					}
				}
			}

			if (!exceptionOccurred)
			{
				env.DeleteLocalRef(result);
			}

			break;
		}
		default:
		{
			// TODO:
			ASSERT_FAIL("Unknown return type");
			break;
		}
	}

	if (!ExceptionUtil::GetInstance()->CheckForJavaException(env))
	{
		AdjustAmountOfExternalAllocatedMemory(env, isolate);
	}
}

int64_t NativeScriptRuntime::AdjustAmountOfExternalAllocatedMemory(JEnv& env, Isolate *isolate)
{
	int64_t changeInBytes = env.CallStaticLongMethod(PlatformClass, GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID);

	int64_t adjustedValue = (changeInBytes != 0)
							? isolate->AdjustAmountOfExternalAllocatedMemory(changeInBytes)
							: 0;

	return adjustedValue;
}


Handle<Object> NativeScriptRuntime::CreateJSWrapper(jint javaObjectID, const string& typeName)
{
	return objectManager->CreateJSWrapper(javaObjectID, typeName);
}


string NativeScriptRuntime::GetReturnType(const string& methodSignature)
{
	int idx = methodSignature.find(')'); //TODO: throw error if not found

	string jniReturnType = methodSignature.substr(idx + 1);

	return jniReturnType;
}

jobject NativeScriptRuntime::CreateJavaInstance(int objectID, const std::string& name, const string& className, const ArgsWrapper& argWrapper, const Handle<Object>& implementationObject, bool isInterface)
{
	SET_PROFILER_FRAME();

	jobject instance = nullptr;
	DEBUG_WRITE("CreateJavaInstance:  %s-%s", className.c_str(), (!name.empty() ? name.c_str() : "0"));

	JEnv env;
	auto& args = argWrapper.args;

	JsArgToArrayConverter argConverter(args, isInterface, argWrapper.outerThis);

	if (argConverter.IsValid())
	{
		jobjectArray javaArgs = argConverter.ToJavaArray();

		int ctorId = GetCachedConstructorId(env, args, name, className, javaArgs, implementationObject);

		jobject obj = env.CallStaticObjectMethod(PlatformClass,
				CREATE_INSTANCE_METHOD_ID,
				javaArgs,
				(jint) objectID,
				ctorId);

		bool exceptionFound = ExceptionUtil::GetInstance()->CheckForJavaException(env);

		if (!exceptionFound)
		{
			instance = obj;
			objectManager->UpdateCache(objectID, obj);
		}
	}
	else
	{
		JsArgToArrayConverter::Error err = argConverter.GetError();
		ExceptionUtil::GetInstance()->ThrowExceptionToJs(err.msg);
	}

	return instance;
}

int NativeScriptRuntime::GetCachedConstructorId(JEnv& env, const FunctionCallbackInfo<Value>& args, const string& name, const string& className, jobjectArray javaArgs, const Handle<Object>& implementationObject)
{
	int ctorId = -1;
	string fullClassName = className + '-' + name;
	string encodedCtorArgs = MethodCache::EncodeSignature(fullClassName, "<init>", args, false);
	auto itFound = s_constructorCache.find(encodedCtorArgs);


	if (itFound != s_constructorCache.end())
	{
		ctorId = itFound->second;
	}
	else
	{
		JniLocalRef javaName(env.NewStringUTF(name.c_str()));
		JniLocalRef javaClassName(env.NewStringUTF(className.c_str()));
		jobjectArray methodOverrides = GetMethodOverrides(env, implementationObject);

		jint id = env.CallStaticIntMethod(PlatformClass, CACHE_CONSTRUCTOR_METHOD_ID, (jstring)javaName, (jstring)javaClassName, javaArgs, methodOverrides);

		if (env.ExceptionCheck() == JNI_FALSE)
		{
			ctorId = id;
			s_constructorCache.insert(make_pair(encodedCtorArgs, ctorId));
		}
	}

	DEBUG_WRITE("GetCachedConstructorId: encodedCtorArgs=%s, ctorId=%d", encodedCtorArgs.c_str(), ctorId);
	return ctorId;
}


//delete the returned local reference after use
jobjectArray NativeScriptRuntime::GetMethodOverrides(JEnv& env, const Handle<Object>& implementationObject)
{
	if (implementationObject.IsEmpty())
	{
		return JavaObjectArrayCache::GetJavaStringArray(0);
	}

	vector<jstring> methodNames;
	auto propNames = implementationObject->GetOwnPropertyNames();
	for (int i = 0; i < propNames->Length(); i++)
	{
		auto name = propNames->Get(i).As<String>();
		auto method = implementationObject->Get(name);

		bool methodFound = !method.IsEmpty() && method->IsFunction();

		if (methodFound)
		{
			String::Utf8Value stringValue(name);
			jstring value = env.NewStringUTF(*stringValue);
			methodNames.push_back(value);
		}
	}

	int methodCount = methodNames.size();

	jobjectArray methodOverrides = JavaObjectArrayCache::GetJavaStringArray(methodCount);
	for (int i = 0; i < methodCount; i++)
	{
		env.SetObjectArrayElement(methodOverrides, i, methodNames[i]);
	}

	for (int i = 0; i < methodCount; i++)
	{
		env.DeleteLocalRef(methodNames[i]);
	}

	return methodOverrides;
}

void NativeScriptRuntime::LogMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	ASSERT_MESSAGE(args.Length() == 1, "Log should be called with string parameter");
	ASSERT_MESSAGE(!args[0]->IsUndefined() && !args[0]->IsNull(), "Log called with undefined");
	ASSERT_MESSAGE(args[0]->IsString(), "Log should be called with string parameter");

	String::Utf8Value message(args[0]->ToString());
	DEBUG_WRITE("%s", *message);
}

void NativeScriptRuntime::DumpReferenceTablesMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	JEnv env;
	jclass vmDbgClass = env.FindClass("dalvik/system/VMDebug");
	if (vmDbgClass != nullptr)
	{
		jmethodID mid = env.GetStaticMethodID(vmDbgClass, "dumpReferenceTables", "()V");
		if (mid != 0)
		{
			env.CallStaticVoidMethod(vmDbgClass, mid);
		}
	}
}

void NativeScriptRuntime::EnableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	JEnv env;
	env.CallStaticVoidMethod(PlatformClass, ENABLE_VERBOSE_LOGGING_METHOD_ID);
}

void NativeScriptRuntime::DisableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	JEnv env;
	env.CallStaticVoidMethod(PlatformClass, DISABLE_VERBOSE_LOGGING_METHOD_ID);
}

void NativeScriptRuntime::ExitMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	auto msg = ConvertToString(args[0].As<String>());
	ASSERT_MESSAGE(false, "%s", msg.c_str());
	exit(-1);
}

void NativeScriptRuntime::APP_FAIL(const char *message)
{
	//ASSERT_FAIL(message);

	JEnv env;
	jstring msg = env.NewStringUTF(message);
	env.CallStaticVoidMethod(PlatformClass, APP_FAIL_METHOD_ID, msg);
}


void NativeScriptRuntime::AddApplicationModule(const string& appName, v8::Persistent<v8::Object>* applicationModule)
{
	loadedModules.insert(make_pair(appName, applicationModule));
}

void NativeScriptRuntime::CreateGlobalCastFunctions(const Handle<ObjectTemplate>& globalTemplate)
{
	castFunctions.CreateGlobalCastFunctions(globalTemplate);
}


void NativeScriptRuntime::RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	SET_PROFILER_FRAME();

	ASSERT_MESSAGE(args.Length() == 2, "require should be called with two parameters");
	ASSERT_MESSAGE(!args[0]->IsUndefined() && !args[0]->IsNull(), "require called with undefined moduleName parameter");
	ASSERT_MESSAGE(!args[1]->IsUndefined() && !args[1]->IsNull(), "require called with undefined callingModulePath parameter");
	ASSERT_MESSAGE(args[0]->IsString(), "require should be called with string parameter");
	ASSERT_MESSAGE(args[1]->IsString(), "require should be called with string parameter");

	string moduleName = ConvertToString(args[0].As<String>());
	string callingModuleName = ConvertToString(args[1].As<String>());

	JEnv env;
	JniLocalRef jsModulename(env.NewStringUTF(moduleName.c_str()));
	JniLocalRef jsCallingModuleName(env.NewStringUTF(callingModuleName.c_str()));
	JniLocalRef jsModulePath(env.CallStaticObjectMethod(RequireClass, GET_MODULE_PATH_METHOD_ID, (jstring) jsModulename, (jstring) jsCallingModuleName));

	auto isolate = Isolate::GetCurrent();

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

	Handle<Object> moduleObj;
	bool hasError = false;

	if (it == loadedModules.end())
	{
		Local<Value> exportObj = Object::New(isolate);
		auto tmpExportObj = new Persistent<Object>(isolate, exportObj.As<Object>());
		loadedModules.insert(make_pair(modulePath, tmpExportObj));

		JniLocalRef moduleContent(env.CallStaticObjectMethod(RequireClass, GET_MODULE_CONTENT_METHOD_ID, (jstring) jsModulePath));
		TryCatch tc;

		auto scriptText = ArgConverter::jstringToV8String(moduleContent);
		DEBUG_WRITE("Compiling script (module %s)", moduleName.c_str());
		auto script = Script::Compile(scriptText, args[0].As<String>());
		DEBUG_WRITE("Compiled script (module %s)", moduleName.c_str());

		if(ExceptionUtil::GetInstance()->HandleTryCatch(tc)){
			loadedModules.erase(modulePath);
			tmpExportObj->Reset();
			delete tmpExportObj;
			hasError = true;
		}
		else {
			DEBUG_WRITE("Running script (module %s)", moduleName.c_str());

			TryCatch tcRequire;

			Local<Function> f = script->Run().As<Function>();
			auto result = f->Call(Object::New(isolate), 1, &exportObj);

			moduleObj = result.As<Object>();

			DEBUG_WRITE("After Running script (module %s)", moduleName.c_str());

			if(ExceptionUtil::GetInstance()->HandleTryCatch(tcRequire)){
				loadedModules.erase(modulePath);
				tmpExportObj->Reset();
				delete tmpExportObj;
				hasError = true;
				tcRequire.ReThrow();
			}
			else {
				if (moduleObj.IsEmpty())
				{
					auto objectTemplate = ObjectTemplate::New();
					moduleObj = objectTemplate->NewInstance();
				}

				DEBUG_WRITE("Script completed (module %s)", moduleName.c_str());

				if (!moduleObj->StrictEquals(exportObj))
				{
					loadedModules.erase(modulePath);
					tmpExportObj->Reset();
					delete tmpExportObj;

					auto persistentModuleObject = new Persistent<Object>(isolate, moduleObj.As<Object>());

					loadedModules.insert(make_pair(modulePath, persistentModuleObject));
				}
			}
		}
		if (tc.HasCaught())
		{
			tc.ReThrow();
		}
	}
	else
	{
		moduleObj = Local<Object>::New(isolate, *((*it).second));
	}

	if(!hasError){
		args.GetReturnValue().Set(moduleObj);
	}
}

vector<string> NativeScriptRuntime::GetTypeMetadata(const string& name, int index)
{
	JEnv env;

	string canonicalName = Util::ConvertFromJniToCanonicalName(name);

	JniLocalRef className(env.NewStringUTF(canonicalName.c_str()));
	jint idx = index;

	JniLocalRef pubApi(env.CallStaticObjectMethod(PlatformClass, GET_TYPE_METADATA, (jstring) className, idx));

	jsize length = env.GetArrayLength(pubApi);

	assert(length > 0);

	vector<string> result;

	for (jsize i=0; i<length; i++)
	{
		JniLocalRef s(env.GetObjectArrayElement(pubApi, i));
		const char *pc = env.GetStringUTFChars(s, nullptr);
		result.push_back(string(pc));
		env.ReleaseStringUTFChars(s, pc);
	}

	return result;
}

void NativeScriptRuntime::BuildMetadata(JEnv& env, jstring filesPath)
{
	timeval time1;
	gettimeofday(&time1, nullptr);

	string baseDir = ArgConverter::jstringToString(filesPath);
	baseDir.append("/metadata");
	string nodesFile = baseDir + "/treeNodeStream.dat";
	string namesFile = baseDir + "/treeStringsStream.dat";
	string valuesFile = baseDir + "/treeValueStream.dat";

	FILE *f = fopen(nodesFile.c_str(), "rb");
	assert(f != nullptr);
	fseek(f, 0, SEEK_END);
	int lenNodes = ftell(f);
	assert((lenNodes % sizeof(MetadataTreeNodeRawData)) == 0);
	char *nodes = new char[lenNodes];
	rewind(f);
	fread(nodes, 1, lenNodes, f);
	fclose(f);

	const int _512KB = 524288;

	f = fopen(namesFile.c_str(), "rb");
	assert(f != nullptr);
	fseek(f, 0, SEEK_END);
	int lenNames = ftell(f);
	char *names = new char[lenNames + _512KB];
	rewind(f);
	fread(names, 1, lenNames, f);
	fclose(f);

	f = fopen(valuesFile.c_str(), "rb");
	assert(f != nullptr);
	fseek(f, 0, SEEK_END);
	int lenValues = ftell(f);
	char *values = new char[lenValues + _512KB];
	rewind(f);
	fread(values, 1, lenValues, f);
	fclose(f);

	timeval time2;
	gettimeofday(&time2, nullptr);

	__android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "lenNodes=%d, lenNames=%d, lenValues=%d", lenNodes, lenNames, lenValues);

	long millis1 = (time1.tv_sec * 1000) + (time1.tv_usec / 1000);
	long millis2 = (time2.tv_sec * 1000) + (time2.tv_usec / 1000);

	__android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "time=%ld", (millis2 - millis1));

	MetadataNode::BuildMetadata(lenNodes, reinterpret_cast<uint8_t*>(nodes), lenNames, reinterpret_cast<uint8_t*>(names), lenValues, reinterpret_cast<uint8_t*>(values));

	delete[] nodes;
	//delete[] names;
	//delete[] values;
}


void NativeScriptRuntime::CreateTopLevelNamespaces(const Handle<Object>& global)
{
	MetadataNode::CreateTopLevelNamespaces(global);
}

Handle<Value> NativeScriptRuntime::CallJSMethod(JNIEnv *_env, const Handle<Object>& jsObject, jstring methodName, jobjectArray args, TryCatch& tc)
{
	SET_PROFILER_FRAME();

	JEnv env(_env);
	Handle<Value> result;

	auto isolate = Isolate::GetCurrent();

	auto exceptionUtil = ExceptionUtil::GetInstance();

	auto jsMethodName = ArgConverter::jstringToV8String(methodName);

	//auto method = MetadataNode::GetPropertyFromImplementationObject(jsObject, jsMethodName);
	string name2 = ConvertToString(jsMethodName);
	auto method = jsObject->Get(ConvertToV8String(name2));

	if (method.IsEmpty() || method->IsUndefined())
	{
		string name = ConvertToString(jsMethodName);

		stringstream ss;
		ss << "Cannot find method '" << name << "' implementation";

		ExceptionUtil::GetInstance()->ThrowExceptionToJs(ss.str());

		result = Undefined(isolate);
	}
	else if (!method->IsFunction())
	{
		string name = ConvertToString(jsMethodName);

		stringstream ss;
		ss << "Property '" << name << "' is not a function";

		ExceptionUtil::GetInstance()->ThrowExceptionToJs(ss.str());

		result = Undefined(isolate);
	}
	else
	{
		EscapableHandleScope handleScope(isolate);

		auto jsMethod = method.As<Function>();
		auto jsArgs = ArgConverter::ConvertJavaArgsToJsArgs(args);
		int argc = jsArgs->Length();

		Handle<Value> arguments[argc];
		for (int i = 0; i < argc; i++)
		{
			arguments[i] = jsArgs->Get(i);
		}

		DEBUG_WRITE("implementationObject->GetIdentityHash()=%d", jsObject->GetIdentityHash());

		Local<Value> jsResult;
		{
			SET_PROFILER_FRAME();
			jsResult = jsMethod->Call(jsObject, argc, argc == 0 ? nullptr : arguments);
		}

		//TODO: if javaResult is a pure js object create a java object that represents this object in java land

		if (tc.HasCaught())
		{
			jsResult = Undefined(isolate);
		}

		result = handleScope.Escape(jsResult);
	}

	return result;
}

Handle<Object> NativeScriptRuntime::FindClass(const string& className)
{
	Handle<Object> clazz;
	JEnv env;

	jmethodID mid = env.GetStaticMethodID(PlatformClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	assert(mid != nullptr);
	JniLocalRef name(env.NewStringUTF(className.c_str()));
	jclass c = (jclass)env.CallStaticObjectMethod(PlatformClass, mid, (jstring)name);
	if (env.ExceptionCheck() == JNI_FALSE)
	{
		jint javaObjectID = objectManager->GetOrCreateObjectId(c);
		clazz = objectManager->GetJsObjectByJavaObject(javaObjectID);

		if (clazz.IsEmpty())
		{
			clazz = objectManager->CreateJSWrapper(javaObjectID, "Ljava/lang/Class;", c);
		}

		env.DeleteLocalRef(c);
	}
	return clazz;
}

int NativeScriptRuntime::GetArrayLength(const Handle<Object>& arr)
{
	JEnv env;

	auto javaArr = reinterpret_cast<jarray>(objectManager->GetJavaObjectByJsObject(arr));

	auto length = env.GetArrayLength(javaArr);

	return length;
}


JavaVM* NativeScriptRuntime::jvm = nullptr;
jclass NativeScriptRuntime::PlatformClass = nullptr;
jclass NativeScriptRuntime::RequireClass = nullptr;
jclass NativeScriptRuntime::JAVA_LANG_STRING = nullptr;
jmethodID NativeScriptRuntime::MAKE_CLASS_INSTANCE_OF_TYPE_STRONG = nullptr;
jmethodID NativeScriptRuntime::CREATE_INSTANCE_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::CACHE_CONSTRUCTOR_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::APP_FAIL_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::GET_MODULE_CONTENT_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::GET_MODULE_PATH_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::GET_TYPE_METADATA = nullptr;
jmethodID NativeScriptRuntime::ENABLE_VERBOSE_LOGGING_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::DISABLE_VERBOSE_LOGGING_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID = nullptr;
map<string, Persistent<Object>*> NativeScriptRuntime::loadedModules;
MetadataTreeNode* NativeScriptRuntime::metadataRoot = nullptr;
ObjectManager* NativeScriptRuntime::objectManager = nullptr;
NumericCasts NativeScriptRuntime::castFunctions;
ArrayElementAccessor NativeScriptRuntime::arrayElementAccessor;
FieldAccessor NativeScriptRuntime::fieldAccessor;
string NativeScriptRuntime::APP_FILES_DIR;
map<string, int> NativeScriptRuntime::s_constructorCache;

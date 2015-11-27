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
#include "NativeScriptException.h"
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

void NativeScriptRuntime::Init(JavaVM *jvm, ObjectManager *objectManager)
{
	NativeScriptRuntime::jvm = jvm;

	JEnv env;

	JAVA_LANG_STRING = env.FindClass("java/lang/String");
	assert(JAVA_LANG_STRING != nullptr);

	PlatformClass = env.FindClass("com/tns/Platform");
	assert(PlatformClass != nullptr);

	RESOLVE_CLASS_METHOD_ID = env.GetStaticMethodID(PlatformClass, "resolveClass", "(Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/Class;");
	assert(RESOLVE_CLASS_METHOD_ID != nullptr);

	CREATE_INSTANCE_METHOD_ID = env.GetStaticMethodID(PlatformClass, "createInstance", "([Ljava/lang/Object;II)Ljava/lang/Object;");
	assert(CREATE_INSTANCE_METHOD_ID != nullptr);

	CACHE_CONSTRUCTOR_METHOD_ID = env.GetStaticMethodID(PlatformClass, "cacheConstructor", "(Ljava/lang/Class;[Ljava/lang/Object;)I");
	assert(CACHE_CONSTRUCTOR_METHOD_ID != nullptr);

	GET_TYPE_METADATA = env.GetStaticMethodID(PlatformClass, "getTypeMetadata", "(Ljava/lang/String;I)[Ljava/lang/String;");
	assert(GET_TYPE_METADATA != nullptr);

	ENABLE_VERBOSE_LOGGING_METHOD_ID = env.GetStaticMethodID(PlatformClass, "enableVerboseLogging", "()V");
	assert(ENABLE_VERBOSE_LOGGING_METHOD_ID != nullptr);

	DISABLE_VERBOSE_LOGGING_METHOD_ID = env.GetStaticMethodID(PlatformClass, "disableVerboseLogging", "()V");
	assert(ENABLE_VERBOSE_LOGGING_METHOD_ID != nullptr);

	GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID = env.GetStaticMethodID(PlatformClass, "getChangeInBytesOfUsedMemory", "()J");
	assert(GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID != nullptr);

	MetadataNode::Init(objectManager);

	NativeScriptRuntime::objectManager = objectManager;

	fieldAccessor.Init(jvm, objectManager);

	arrayElementAccessor.Init(jvm, objectManager);

	JavaObjectArrayCache::Init(jvm);

	MethodCache::Init();
}

bool NativeScriptRuntime::RegisterInstance(const Local<Object>& jsObject, const std::string& fullClassName, const ArgsWrapper& argWrapper, const Local<Object>& implementationObject, bool isInterface)
{
	bool success;

	DEBUG_WRITE("RegisterInstance called for '%s'", fullClassName.c_str());

	JEnv env;

	jclass generatedJavaClass = ResolveClass(fullClassName, implementationObject);

	int javaObjectID = objectManager->GenerateNewObjectID();

	DEBUG_WRITE("RegisterInstance: Linking new instance");
	objectManager->Link(jsObject, javaObjectID, nullptr);

	jobject instance = CreateJavaInstance(javaObjectID, fullClassName, argWrapper, generatedJavaClass, isInterface);

	JniLocalRef localInstance(instance);
	success = !localInstance.IsNull();

	if (success)
	{
		DEBUG_WRITE("RegisterInstance: Updating linked instance with its real class");
		jclass instanceClass = env.FindClass(fullClassName);
		objectManager->SetJavaClass(jsObject, instanceClass);
	}
	else
	{
		DEBUG_WRITE("RegisterInstance failed with null new instance");
	}

	return success;
}

jclass NativeScriptRuntime::ResolveClass(const std::string& fullClassname, const Local<Object>& implementationObject) {

	auto itFound = s_classCache.find(fullClassname);

	jclass globalRefToGeneratedClass;

	if (itFound != s_classCache.end())
	{
		globalRefToGeneratedClass = itFound->second;
	}
	else
	{
		JEnv env;

		//get needed arguments in order to load binding
		JniLocalRef javaFullClassName(env.NewStringUTF(fullClassname.c_str()));

		jobjectArray methodOverrides = GetMethodOverrides(env, implementationObject);

		//create or load generated binding (java class)
		JniLocalRef generatedClass(env.CallStaticObjectMethod(PlatformClass, RESOLVE_CLASS_METHOD_ID,  (jstring)javaFullClassName, methodOverrides));
		globalRefToGeneratedClass = reinterpret_cast<jclass>(env.NewGlobalRef(generatedClass));

		s_classCache.insert(make_pair(fullClassname, globalRefToGeneratedClass));
	}

	return globalRefToGeneratedClass;
}

Local<Value> NativeScriptRuntime::GetArrayElement(const Local<Object>& array, uint32_t index, const string& arraySignature)
{
	return arrayElementAccessor.GetArrayElement(array, index, arraySignature);
}

void NativeScriptRuntime::SetArrayElement(const Local<Object>& array, uint32_t index, const string& arraySignature, Local<Value>& value)
{
	JEnv env;

	arrayElementAccessor.SetArrayElement(array, index, arraySignature, value);
}

Local<Value> NativeScriptRuntime::GetJavaField(const Local<Object>& caller, FieldCallbackData *fieldData)
{
	return fieldAccessor.GetJavaField(caller, fieldData);
}

void NativeScriptRuntime::SetJavaField(const Local<Object>& target, const Local<Value>& value, FieldCallbackData *fieldData)
{
	fieldAccessor.SetJavaField(target, value, fieldData);
}


void NativeScriptRuntime::CallJavaMethod(const Local<Object>& caller, const string& className, const string& methodName, MetadataEntry *entry, bool isStatic, bool isSuper, const v8::FunctionCallbackInfo<v8::Value>& args)
{
	SET_PROFILER_FRAME();

	JEnv env;

	jclass clazz;
	jmethodID mid;
	string *sig = nullptr;
	string *returnType = nullptr;
	auto retType = MethodReturnType::Unknown;
	MethodCache::CacheMethodInfo mi;

	if ((entry != nullptr) && entry->isResolved)
	{
		isStatic = entry->isStatic;

		if (entry->memberId == nullptr)
		{
			clazz = env.FindClass(className);
			if (clazz == nullptr)
			{
				MetadataNode* callerNode = MetadataNode::GetNodeFromHandle(caller);
				const string callerClassName = callerNode->GetName();

				DEBUG_WRITE("Cannot resolve class: %s while calling method: %s callerClassName: %s", className.c_str(), methodName.c_str(), callerClassName.c_str());
				clazz = env.FindClass(callerClassName);
				if (clazz == nullptr)
				{
					//todo: plamen5kov: throw exception here
					DEBUG_WRITE("Cannot resolve caller's class name: %s", callerClassName.c_str());
					return;
				}

				entry->memberId = isStatic ?
						env.GetStaticMethodID(clazz, methodName, entry->sig) :
						env.GetMethodID(clazz, methodName, entry->sig);

				if (entry->memberId == nullptr)
				{
					//todo: plamen5kov: throw exception here
					DEBUG_WRITE("Cannot resolve a method %s on caller class: %s", methodName.c_str(), callerClassName.c_str());
					return;
				}
			}
			else
			{
				entry->memberId = isStatic ?
									env.GetStaticMethodID(clazz, methodName, entry->sig) :
									env.GetMethodID(clazz, methodName, entry->sig);

				if (entry->memberId == nullptr)
				{
					//todo: plamen5kov: throw exception here
					DEBUG_WRITE("Cannot resolve a method %s on class: %s", methodName.c_str(), className.c_str());
					return;
				}
			}
			entry->clazz = clazz;
		}

		mid = reinterpret_cast<jmethodID>(entry->memberId);
		clazz = entry->clazz;
		sig = &entry->sig;
		returnType = &entry->returnType;
		retType = entry->retType;
	}
	else
	{
		DEBUG_WRITE("Resolving method: %s on className %s", methodName.c_str(), className.c_str());

		clazz = env.FindClass(className);
		if (clazz != nullptr)
		{
			mi = MethodCache::ResolveMethodSignature(className, methodName, args, isStatic);
			if (mi.mid == nullptr)
			{
				DEBUG_WRITE("Cannot resolve class=%s, method=%s, isStatic=%d, isSuper=%d", className.c_str(), methodName.c_str(), isStatic, isSuper);
				return;
			}
		}
		else
		{
			MetadataNode* callerNode = MetadataNode::GetNodeFromHandle(caller);
			const string callerClassName = callerNode->GetName();
			DEBUG_WRITE("Resolving method on caller class: %s.%s on className %s", callerClassName.c_str(), methodName.c_str(), className.c_str());
			mi = MethodCache::ResolveMethodSignature(callerClassName, methodName, args, isStatic);
			if (mi.mid == nullptr)
			{
				DEBUG_WRITE("Cannot resolve class=%s, method=%s, isStatic=%d, isSuper=%d, callerClass=%s", className.c_str(), methodName.c_str(), isStatic, isSuper, callerClassName.c_str());
				return;
			}
		}

		clazz = mi.clazz;
		mid = mi.mid;
		sig = &mi.signature;
		returnType = &mi.returnType;
		retType = mi.retType;
	}


	if (!isStatic)
	{
		DEBUG_WRITE("CallJavaMethod called %s.%s. Instance id: %d, isSuper=%d", className.c_str(), methodName.c_str(), caller.IsEmpty() ? -42 : caller->GetIdentityHash(), isSuper);
	}
	else
	{
		DEBUG_WRITE("CallJavaMethod called %s.%s. static method", className.c_str(), methodName.c_str());
	}

	JsArgConverter argConverter(args, false, *sig, entry);

	if (!argConverter.IsValid())
	{
		JsArgConverter::Error err = argConverter.GetError();
		throw NativeScriptException(err.msg);
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
			throw NativeScriptException(ss.str());
		}
	}

	switch (retType)
	{
		case MethodReturnType::Void:
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
		case MethodReturnType::Boolean:
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
		case MethodReturnType::Byte:
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
		case MethodReturnType::Char:
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
			args.GetReturnValue().Set(ConvertToV8String(resP, 1));
			env.ReleaseStringUTFChars(str, resP);
			break;
		}
		case MethodReturnType::Short:
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
		case MethodReturnType::Int:
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
		case MethodReturnType::Long:
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
		case MethodReturnType::Float:
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
		case MethodReturnType::Double:
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
		case MethodReturnType::String:
		{
			jobject result = nullptr;
			bool exceptionOccurred;

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

			if (result != nullptr)
			{
				auto objectResult = ArgConverter::jstringToV8String(static_cast<jstring>(result));
				args.GetReturnValue().Set(objectResult);
				env.DeleteLocalRef(result);
			}
			else
			{
				args.GetReturnValue().Set(Null(isolate));
			}

			break;
		}
		case MethodReturnType::Object:
		{
			jobject result = nullptr;
			bool exceptionOccurred;

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

			if (result != nullptr)
			{
				auto isString = env.IsInstanceOf(result, JAVA_LANG_STRING);

				Local<Value> objectResult;
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
						objectResult = objectManager->CreateJSWrapper(javaObjectID, *returnType, result);
					}
				}

				args.GetReturnValue().Set(objectResult);
				env.DeleteLocalRef(result);
			}
			else
			{
				args.GetReturnValue().Set(Null(isolate));
			}

			break;
		}
		default:
		{
			assert(false);
			break;
		}
	}

	static uint32_t adjustMemCount = 0;

	if ((++adjustMemCount % 2) == 0)
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


Local<Object> NativeScriptRuntime::CreateJSWrapper(jint javaObjectID, const string& typeName)
{
	return objectManager->CreateJSWrapper(javaObjectID, typeName);
}


jobject NativeScriptRuntime::CreateJavaInstance(int objectID, const std::string& fullClassName, const ArgsWrapper& argWrapper, jclass javaClass, bool isInterface)
{
	SET_PROFILER_FRAME();

	jobject instance = nullptr;
	DEBUG_WRITE("CreateJavaInstance:  %s", fullClassName.c_str());

	JEnv env;
	auto& args = argWrapper.args;

	JsArgToArrayConverter argConverter(args, isInterface, argWrapper.outerThis);
	if (argConverter.IsValid())
	{
		jobjectArray javaArgs = argConverter.ToJavaArray();

		int ctorId = GetCachedConstructorId(env, args, fullClassName, javaArgs, javaClass);

		jobject obj = env.CallStaticObjectMethod(PlatformClass,
				CREATE_INSTANCE_METHOD_ID,
				javaArgs,
				(jint) objectID,
				ctorId);


		instance = obj;
	}
	else
	{
		JsArgToArrayConverter::Error err = argConverter.GetError();
		throw NativeScriptException(err.msg);
	}

	return instance;
}

int NativeScriptRuntime::GetCachedConstructorId(JEnv& env, const FunctionCallbackInfo<Value>& args, const string& fullClassName, jobjectArray javaArgs, jclass javaClass)
{
	int ctorId = -1;
	string encodedCtorArgs = MethodCache::EncodeSignature(fullClassName, "<init>", args, false);
	auto itFound = s_constructorCache.find(encodedCtorArgs);


	if (itFound != s_constructorCache.end())
	{
		ctorId = itFound->second;
	}
	else
	{
		jint id = env.CallStaticIntMethod(PlatformClass, CACHE_CONSTRUCTOR_METHOD_ID, javaClass, javaArgs);

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
jobjectArray NativeScriptRuntime::GetMethodOverrides(JEnv& env, const Local<Object>& implementationObject)
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
	try {
	if ((args.Length() > 0) && args[0]->IsString())
	{
		String::Utf8Value message(args[0]->ToString());
		DEBUG_WRITE("%s", *message);
	}
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

void NativeScriptRuntime::DumpReferenceTablesMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
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

void NativeScriptRuntime::EnableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	tns::LogEnabled = true;
	JEnv env;
	env.CallStaticVoidMethod(PlatformClass, ENABLE_VERBOSE_LOGGING_METHOD_ID);
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

void NativeScriptRuntime::DisableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	try {
	tns::LogEnabled = false;
	JEnv env;
	env.CallStaticVoidMethod(PlatformClass, DISABLE_VERBOSE_LOGGING_METHOD_ID);
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

void NativeScriptRuntime::ExitMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	auto msg = ConvertToString(args[0].As<String>());
	DEBUG_WRITE_FATAL("FORCE EXIT: %s", msg.c_str());
	exit(-1);
}

void NativeScriptRuntime::CreateGlobalCastFunctions(const Local<ObjectTemplate>& globalTemplate)
{
	castFunctions.CreateGlobalCastFunctions(globalTemplate);
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

void NativeScriptRuntime::BuildMetadata(JEnv& env, string filesPath)
{
	timeval time1;
	gettimeofday(&time1, nullptr);

	string baseDir = filesPath;
	baseDir.append("/metadata");
	string nodesFile = baseDir + "/treeNodeStream.dat";
	string namesFile = baseDir + "/treeStringsStream.dat";
	string valuesFile = baseDir + "/treeValueStream.dat";

	FILE *f = fopen(nodesFile.c_str(), "rb");
	if(f == nullptr) {
		throw NativeScriptException(string("metadata file (treeNodeStream.dat) couldn't be opened!"));
	}
	fseek(f, 0, SEEK_END);
	int lenNodes = ftell(f);
	assert((lenNodes % sizeof(MetadataTreeNodeRawData)) == 0);
	char *nodes = new char[lenNodes];
	rewind(f);
	fread(nodes, 1, lenNodes, f);
	fclose(f);

	const int _512KB = 524288;

	f = fopen(namesFile.c_str(), "rb");
	if(f == nullptr) {
		throw NativeScriptException(string("metadata file (treeStringsStream.dat) couldn't be opened!"));
	}
	fseek(f, 0, SEEK_END);
	int lenNames = ftell(f);
	char *names = new char[lenNames + _512KB];
	rewind(f);
	fread(names, 1, lenNames, f);
	fclose(f);

	f = fopen(valuesFile.c_str(), "rb");
	if(f == nullptr) {
		throw NativeScriptException(string("metadata file (treeValueStream.dat) couldn't be opened!"));
	}
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

void NativeScriptRuntime::CreateTopLevelNamespaces(const Local<Object>& global)
{
	MetadataNode::CreateTopLevelNamespaces(global);
}

Local<Value> NativeScriptRuntime::CallJSMethod(JNIEnv *_env, const Local<Object>& jsObject, const string& methodName, jobjectArray args)
{
	SET_PROFILER_FRAME();

	JEnv env(_env);
	Local<Value> result;

	auto isolate = Isolate::GetCurrent();

	//auto method = MetadataNode::GetPropertyFromImplementationObject(jsObject, jsMethodName);
	auto method = jsObject->Get(ConvertToV8String(methodName));


	if (method.IsEmpty() || method->IsUndefined())
	{
		stringstream ss;
		ss << "Cannot find method '" << methodName << "' implementation";
		throw NativeScriptException(ss.str());
	}
	else if (!method->IsFunction())
	{
		stringstream ss;
		ss << "Property '" << methodName << "' is not a function";
		throw NativeScriptException(ss.str());
	}
	else
	{
		EscapableHandleScope handleScope(isolate);

		auto jsMethod = method.As<Function>();
		auto jsArgs = ArgConverter::ConvertJavaArgsToJsArgs(args);
		int argc = jsArgs->Length();

		Local<Value> arguments[argc];
		for (int i = 0; i < argc; i++)
		{
			arguments[i] = jsArgs->Get(i);
		}

		DEBUG_WRITE("implementationObject->GetIdentityHash()=%d", jsObject->GetIdentityHash());

		TryCatch tc;
		Local<Value> jsResult;
		{
			SET_PROFILER_FRAME();
			jsResult = jsMethod->Call(jsObject, argc, argc == 0 ? nullptr : arguments);
		}

		//TODO: if javaResult is a pure js object create a java object that represents this object in java land

		if(tc.HasCaught()) {
			stringstream ss;
			ss << "Calling js method " << methodName << " failed";
			string exceptionMessage = ss.str();
			throw NativeScriptException(tc, ss.str());
		}

		result = handleScope.Escape(jsResult);
	}

	return result;
}

Local<Object> NativeScriptRuntime::FindClass(const string& className)
{
	Local<Object> clazz;
	JEnv env;

	jclass c = env.FindClass(className);
	if (env.ExceptionCheck() == JNI_FALSE)
	{
		jint javaObjectID = objectManager->GetOrCreateObjectId(c);
		clazz = objectManager->GetJsObjectByJavaObject(javaObjectID);

		if (clazz.IsEmpty())
		{
			clazz = objectManager->CreateJSWrapper(javaObjectID, "Ljava/lang/Class;", c);
		}
	}
	return clazz;
}

int NativeScriptRuntime::GetArrayLength(const Local<Object>& arr)
{
	JEnv env;

	auto javaArr = reinterpret_cast<jarray>(objectManager->GetJavaObjectByJsObject(arr));

	auto length = env.GetArrayLength(javaArr);

	return length;
}


JavaVM* NativeScriptRuntime::jvm = nullptr;
jclass NativeScriptRuntime::PlatformClass = nullptr;
jclass NativeScriptRuntime::JAVA_LANG_STRING = nullptr;
jmethodID NativeScriptRuntime::RESOLVE_CLASS_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::CREATE_INSTANCE_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::CACHE_CONSTRUCTOR_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::GET_TYPE_METADATA = nullptr;
jmethodID NativeScriptRuntime::ENABLE_VERBOSE_LOGGING_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::DISABLE_VERBOSE_LOGGING_METHOD_ID = nullptr;
jmethodID NativeScriptRuntime::GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID = nullptr;
MetadataTreeNode* NativeScriptRuntime::metadataRoot = nullptr;
ObjectManager* NativeScriptRuntime::objectManager = nullptr;
NumericCasts NativeScriptRuntime::castFunctions;
ArrayElementAccessor NativeScriptRuntime::arrayElementAccessor;
FieldAccessor NativeScriptRuntime::fieldAccessor;
map<string, int> NativeScriptRuntime::s_constructorCache;
map<std::string, jclass> NativeScriptRuntime::s_classCache;

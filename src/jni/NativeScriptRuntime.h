#ifndef NATIVESCRIPTRUNTIME_H_
#define NATIVESCRIPTRUNTIME_H_

#include <string>
#include <map>
#include <vector>
#include "v8.h"
#include "v8-debug.h"
#include "JEnv.h"
#include "JSInstanceInfo.h"
#include "ArgsWrapper.h"
#include "MetadataEntry.h"
#include "FieldCallbackData.h"
#include "MetadataTreeNode.h"
#include "NumericCasts.h"
#include "FieldAccessor.h"
#include "ArrayElementAccessor.h"
#include "ObjectManager.h"

namespace tns
{
	class NativeScriptRuntime
	{
	public:

		static void Init(JavaVM *jvm, ObjectManager *objectManager);


		static v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID, const std::string& typeName);

		static jobject CreateJavaInstance(int objectID, const std::string& fullClassName, const ArgsWrapper& argWrapper, jclass javaClass, bool isInterface);

		static bool RegisterInstance(const v8::Local<v8::Object>& jsObject, const std::string& fullClassName, const ArgsWrapper& argWrapper, const v8::Local<v8::Object>& implementationObject, bool isInterface);

		static jclass ResolveClass(const std::string& fullClassname, const v8::Local<v8::Object>& implementationObject);
		//

		static v8::Local<v8::Value> GetArrayElement(const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature);

		static void SetArrayElement(const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature, v8::Local<v8::Value>& value);

		static int GetArrayLength(const v8::Local<v8::Object>& arr);

		//

		static void CallJavaMethod(const v8::Local<v8::Object>& caller, const std::string& className, const std::string& methodName, MetadataEntry *entry, bool isStatic, bool isSuper, const v8::FunctionCallbackInfo<v8::Value>& args);

		static v8::Local<v8::Value> CallJSMethod(JNIEnv *_env, const v8::Local<v8::Object>& jsObject, const std::string& methodName, jobjectArray args);

		//

		static v8::Local<v8::Value> GetJavaField(const v8::Local<v8::Object>& caller, FieldCallbackData *fieldData);

		static void SetJavaField(const v8::Local<v8::Object>& target, const v8::Local<v8::Value>& value, FieldCallbackData *fieldData);

		//

		static void OverridesMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void LogMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void DumpReferenceTablesMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void ExitMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void OverridesWeakCallback(v8::Isolate* isolate, v8::Persistent<v8::Object>* target, void* arg);

		static v8::Local<v8::Object> GetImplementationObjectFromArg(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void CreateGlobalCastFunctions(const v8::Local<v8::ObjectTemplate>& globalTemplate);

		static std::vector<std::string> GetTypeMetadata(const std::string& name, int index);

		static jobjectArray GetMethodOverrides(JEnv& env, const v8::Local<v8::Object>& implementationObject);

		static std::string LogExceptionStackTrace(const v8::TryCatch& tryCatch);

		static void EnableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void DisableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void BuildMetadata(JEnv& env, std::string filesPath);

		static void CreateTopLevelNamespaces(const v8::Local<v8::Object>& global);

		static v8::Local<v8::Object> FindClass(const std::string& className);

		static MetadataTreeNode *metadataRoot;

		static jclass PlatformClass;

		static jclass JAVA_LANG_STRING;
		//

	private:
		NativeScriptRuntime() {}

		static int GetCachedConstructorId(JEnv& env, const v8::FunctionCallbackInfo<v8::Value>& args, const std::string& fullClassName, jobjectArray javaArgs, jclass javaClass);

		static int64_t AdjustAmountOfExternalAllocatedMemory(JEnv& env, v8::Isolate *isolate);

		static v8::Persistent<v8::Object>* MarkJsObject(const v8::Local<v8::Object>& object, std::string mark, const v8::Local<v8::Value>& value);

		static JavaVM *jvm;

		static jmethodID RESOLVE_CLASS_METHOD_ID;

		static jmethodID CREATE_INSTANCE_METHOD_ID;

		static jmethodID CACHE_CONSTRUCTOR_METHOD_ID;

		static jmethodID GET_TYPE_METADATA;

		static jmethodID ENABLE_VERBOSE_LOGGING_METHOD_ID;

		static jmethodID DISABLE_VERBOSE_LOGGING_METHOD_ID;

		static jmethodID GET_CHANGE_IN_BYTES_OF_USED_MEMORY_METHOD_ID;

		static NumericCasts castFunctions;

		static ArrayElementAccessor arrayElementAccessor;

		static FieldAccessor fieldAccessor;
	
		static ObjectManager *objectManager;

		static std::map<std::string, int> s_constructorCache;

		static std::map<std::string, jclass> s_classCache;
	};
}

#endif /* NATIVESCRIPTRUNTIME_H_ */

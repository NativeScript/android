#include "NativeScriptRuntime.h"
#include "MetadataNode.h"
#include "JniLocalRef.h"
#include "JsArgConverter.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "ExceptionUtil.h"
#include "v8.h"
#include "JEnv.h"
#include "NativeScriptAssert.h"
#include <sstream>
#include <android/log.h>
#include <assert.h>
#include <string>

using namespace v8;
using namespace std;
using namespace tns;

extern Isolate *g_isolate;
extern ObjectManager *g_objectManager;
extern int AppJavaObjectID;


jobject ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value);

extern "C" jobjectArray Java_com_tns_NativeScriptActivity_getMethodOverrides(JNIEnv *_env, jobject obj, jint objectId, jobjectArray packagedArgs)
{
	DEBUG_WRITE("getMethodOverrides called");

	auto isolate = g_isolate;
	Isolate::Scope isolate_scope(isolate);

	JEnv env(_env);

	HandleScope handleScope(isolate);

	TryCatch tc;

	auto appInstance = g_objectManager->GetJsObjectByJavaObject(AppJavaObjectID);
	if (appInstance.IsEmpty())
	{
		ExceptionUtil::GetInstance()->ThrowExceptionToJava(tc, "NativeScript application not initialized correctly. Missing the global app object initialization.");
		return nullptr;
	}

	string createActivityMethodName = "getActivity";
	auto createActivityFunction = appInstance->Get(ConvertToV8String(createActivityMethodName.c_str()));

	if (createActivityFunction.IsEmpty() || !createActivityFunction->IsFunction())
	{
		ExceptionUtil::GetInstance()->ThrowExceptionToJava(tc, "NativeScript application not initialized correctly. No function 'createActivity' found on the application object.");
		return nullptr;
	}

	auto jsResult = NativeScriptRuntime::CallJSMethod(env, appInstance, createActivityMethodName, packagedArgs, tc);
	auto jsInstance = jsResult.As<Object>();
	if (jsInstance.IsEmpty() || jsInstance->IsNull() || jsInstance->IsUndefined())
	{
		ExceptionUtil::GetInstance()->ThrowExceptionToJava(tc, "NativeScript application not initialized correctly. getActivity method returned invalid value.");
		return nullptr;
	}

	auto jsActivity = g_objectManager->GetJsObjectByJavaObject(objectId);

	MetadataNode::InjectPrototype(jsActivity, jsInstance);

	jobjectArray methodOverrides = NativeScriptRuntime::GetMethodOverrides(env, jsInstance);
	return methodOverrides;
}

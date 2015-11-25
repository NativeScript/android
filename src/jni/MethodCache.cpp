#include "MethodCache.h"
#include "JniLocalRef.h"
#include "JsArgToArrayConverter.h"
#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include <sstream>
#include <assert.h>

using namespace v8;
using namespace std;
using namespace tns;

void MethodCache::Init()
{
	JEnv env;

	PLATFORM_CLASS = env.FindClass("com/tns/Platform");
	assert(PLATFORM_CLASS != nullptr);

	RESOLVE_METHOD_OVERLOAD_METHOD_ID = env.GetStaticMethodID(PLATFORM_CLASS, "resolveMethodOverload", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;");
	assert(RESOLVE_METHOD_OVERLOAD_METHOD_ID != nullptr);
}

MethodCache::CacheMethodInfo MethodCache::ResolveMethodSignature(const string& className, const string& methodName, const FunctionCallbackInfo<Value>& args, bool isStatic)
{
	CacheMethodInfo mi;

	// TODO: Possible improvement
	auto key = EncodeSignature(className, methodName, args, isStatic);

	auto it = s_cache.find(key);

	if (it == s_cache.end())
	{
		auto signature = ResolveJavaMethod(args, className, methodName);

		DEBUG_WRITE("ResolveMethodSignature %s='%s'", key.c_str(), signature.c_str());

		if (!signature.empty())
		{
			JEnv env;
			auto clazz = env.FindClass(className);
			assert(clazz != nullptr);
			mi.clazz = clazz;
			mi.signature = signature;
			mi.returnType = MetadataReader::ParseReturnType(mi.signature);
			mi.retType = MetadataReader::GetReturnType(mi.returnType);
			mi.isStatic = isStatic;
			mi.mid = isStatic
						? env.GetStaticMethodID(clazz, methodName, signature)
						: env.GetMethodID(clazz, methodName, signature);

			s_cache.insert(make_pair(key, mi));
		}
	}
	else
	{
		mi = (*it).second;
	}

	return mi;
}


string MethodCache::EncodeSignature(const string& className, const string& methodName, const FunctionCallbackInfo<Value>& args, bool isStatic)
{
	string sig(className);
	sig.append(".");
	if (isStatic)
	{
		sig.append("S.");
	}
	else
	{
		sig.append("I.");
	}
	sig.append(methodName);
	sig.append(".");
	int len = args.Length();
	stringstream s;
	s << len;
	sig.append(s.str());

	for (int i=0; i<len; i++)
	{
		sig.append(".");
		sig.append(GetType(args[i]));
	}

	return sig;
}


string MethodCache::GetType(const v8::Local<v8::Value>& value)
{
	string type;

	if (value->IsArray() || value->IsArrayBuffer() || value->IsArrayBufferView() || value->IsTypedArray()
			|| value->IsFloat32Array() || value->IsFloat64Array()
			|| value->IsInt8Array() || value->IsInt16Array() || value->IsInt32Array()
			|| value->IsUint8Array() || value->IsUint16Array() || value->IsUint32Array())
	{
		type = "array";
	}
	else if (value->IsBoolean() || value->IsBooleanObject() || value->IsFalse() || value->IsTrue())
	{
		type = "bool";
	}
	else if (value->IsDataView())
	{
		type = "view";
	}
	else if (value->IsDate())
	{
		type = "date";
	}
	else if (value->IsFunction())
	{
		type = "function";
	}
	else if (value->IsInt32() || value->IsUint32())
	{
		type = "int";
	}
	else if (value->IsNull() || value->IsUndefined())
	{
		type = "null";
	}
	else if (value->IsString() || value->IsStringObject())
	{
		type = "string";
	}
	else if (value->IsNumber() || value->IsNumberObject())
	{
		double d = value->NumberValue();
		int64_t i = (int64_t)d;
		bool isInteger = d == i;

		type = isInteger ? "intnumber" : "doublenumber";
	}
	else if (value->IsObject())
	{
		auto object = value->ToObject();

		if (!object->GetHiddenValue(V8StringConstants::GetMarkedAsByte()).IsEmpty())
		{
			type = "byte";
		}
		else if ((!object->GetHiddenValue(V8StringConstants::GetMarkedAsLong()).IsEmpty())
				|| (!object->GetHiddenValue(V8StringConstants::GetJavaLong()).IsEmpty()))
		{
			type = "long";
		}
		else if (!object->GetHiddenValue(V8StringConstants::GetMarkedAsShort()).IsEmpty())
		{
			type = "short";
		}
		else if (!object->GetHiddenValue(V8StringConstants::GetMarkedAsChar()).IsEmpty())
		{
			type = "char";
		}
		else if (!object->GetHiddenValue(V8StringConstants::GetMarkedAsFloat()).IsEmpty())
		{
			type = "float";
		}
		else if (!object->GetHiddenValue(V8StringConstants::GetMarkedAsDouble()).IsEmpty())
		{
			type = "double";
		}
		else
		{
			MetadataNode *node = MetadataNode::GetNodeFromHandle(object);
			type = (node != nullptr) ? node->GetName() : "<unknown>";
		}
	}
	return type;
}


string MethodCache::ResolveJavaMethod(const FunctionCallbackInfo<Value>& args, const string& className, const string& methodName)
{
	JEnv env;

	JsArgToArrayConverter argConverter(args, false);

	auto canonicalClassName = Util::ConvertFromJniToCanonicalName(className);
	JniLocalRef jsClassName(env.NewStringUTF(canonicalClassName.c_str()));
	JniLocalRef jsMethodName(env.NewStringUTF(methodName.c_str()));

	jobjectArray arrArgs = argConverter.ToJavaArray();

	jstring signature = (jstring) env.CallStaticObjectMethod(PLATFORM_CLASS, RESOLVE_METHOD_OVERLOAD_METHOD_ID, (jstring) jsClassName, (jstring) jsMethodName, arrArgs);

	string resolvedSignature;

	const char* str = env.GetStringUTFChars(signature, nullptr);
	resolvedSignature = string(str);
	env.ReleaseStringUTFChars(signature, str);

	env.DeleteLocalRef(signature);

	return resolvedSignature;
}


map<string, MethodCache::CacheMethodInfo> MethodCache::s_cache;
jclass MethodCache::PLATFORM_CLASS = nullptr;
jmethodID MethodCache::RESOLVE_METHOD_OVERLOAD_METHOD_ID = nullptr;

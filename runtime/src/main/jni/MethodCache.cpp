#include "MethodCache.h"
#include "JniLocalRef.h"
#include "JsArgToArrayConverter.h"
#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "NumericCasts.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include <sstream>
#include <assert.h>

using namespace v8;
using namespace std;
using namespace tns;

void MethodCache::Init()
{
	JEnv env;

	RUNTIME_CLASS = env.FindClass("com/tns/Runtime");
	assert(RUNTIME_CLASS != nullptr);

	RESOLVE_METHOD_OVERLOAD_METHOD_ID = env.GetMethodID(RUNTIME_CLASS, "resolveMethodOverload", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;");
	assert(RESOLVE_METHOD_OVERLOAD_METHOD_ID != nullptr);

	RESOLVE_CONSTRUCTOR_SIGNATURE_ID = env.GetMethodID(RUNTIME_CLASS, "resolveConstructorSignature", "(Ljava/lang/Class;[Ljava/lang/Object;)Ljava/lang/String;");
	assert(RESOLVE_CONSTRUCTOR_SIGNATURE_ID != nullptr);
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
							:
							env.GetMethodID(clazz, methodName, signature);

			s_cache.insert(make_pair(key, mi));
		}
	}
	else
	{
		mi = (*it).second;
	}

	return mi;
}

MethodCache::CacheMethodInfo MethodCache::ResolveConstructorSignature(const ArgsWrapper& argWrapper, const string& fullClassName, jclass javaClass, bool isInterface)
{
	auto& args = argWrapper.args;

	CacheMethodInfo mi;

	auto key = EncodeSignature(fullClassName, "<init>", args, false);

	auto it = s_cache.find(key);

	if (it == s_cache.end())
	{
		auto signature = ResolveConstructor(args, javaClass, isInterface, argWrapper.outerThis);

		DEBUG_WRITE("ResolveConstructorSignature %s='%s'", key.c_str(), signature.c_str());

		if (!signature.empty())
		{
			JEnv env;
			mi.clazz = javaClass;
			mi.signature = signature;
			mi.mid = env.GetMethodID(javaClass, "<init>", signature);

			s_cache.insert(make_pair(key, mi));
		}
	}
	else
	{
		mi = (*it).second;
	}

	return mi;
}

// Encoded signature <className>.S/I.<methodName>.<argsCount>.<arg1class>.<...>
string MethodCache::EncodeSignature(const string& className, const string& methodName, const FunctionCallbackInfo<Value>& args, bool isStatic)
{
	string sig(className);
	auto isolate = args.GetIsolate();

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

	for (int i = 0; i < len; i++)
	{
		sig.append(".");
		sig.append(GetType(isolate, args[i]));
	}

	return sig;
}

string MethodCache::GetType(Isolate *isolate, const v8::Local<v8::Value>& value)
{
	string type;

	if(value->IsObject())
	{
		auto objVal = value->ToObject();

		auto maybeNullNode = objVal->GetPrivate(isolate->GetCurrentContext(), Private::New(isolate, V8StringConstants::GetNullNodeName()));

		if(!maybeNullNode.IsEmpty()) {
			Local<Value> nullNode = maybeNullNode.ToLocalChecked();

			auto treeNode = reinterpret_cast<MetadataNode*>(nullNode.As<External>()->Value());

			type = (treeNode != nullptr) ? treeNode->GetName() : "<unknown>";

			DEBUG_WRITE("Parameter of type %s with NULL value is passed to the method.", type.c_str());
			return type;
		}
	}

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
		int64_t i = (int64_t) d;
		bool isInteger = d == i;

		type = isInteger ? "intnumber" : "doublenumber";
	}
	else if (value->IsObject())
	{
		auto object = value->ToObject();
		auto castType = NumericCasts::GetCastType(isolate, object);
		MetadataNode *node;

		switch (castType)
		{
			case CastType::Char:
				type = "char";
				break;

			case CastType::Byte:
				type = "byte";
				break;

			case CastType::Short:
				type = "short";
				break;

			case CastType::Long:
				type = "long";
				break;

			case CastType::Float:
				type = "float";
				break;

			case CastType::Double:
				type = "double";
				break;

			case CastType::None:
				node = MetadataNode::GetNodeFromHandle(object);
				type = (node != nullptr) ? node->GetName() : "<unknown>";
				break;

			default:
				throw NativeScriptException("Unsupported cast type");
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

	auto runtime = Runtime::GetRuntime(args.GetIsolate());

	jstring signature = (jstring) env.CallObjectMethod(runtime->GetJavaRuntime(), RESOLVE_METHOD_OVERLOAD_METHOD_ID, (jstring) jsClassName, (jstring) jsMethodName, arrArgs);

	string resolvedSignature;

	const char* str = env.GetStringUTFChars(signature, nullptr);
	resolvedSignature = string(str);
	env.ReleaseStringUTFChars(signature, str);

	env.DeleteLocalRef(signature);

	return resolvedSignature;
}

string MethodCache::ResolveConstructor(const FunctionCallbackInfo<Value>& args, jclass javaClass, bool isInterface, Local<Object> outerThis)
{
	JEnv env;
	string resolvedSignature;

	JsArgToArrayConverter argConverter(args, isInterface, outerThis);
	if (argConverter.IsValid())
	{
		jobjectArray javaArgs = argConverter.ToJavaArray();

		auto runtime = Runtime::GetRuntime(args.GetIsolate());

		jstring signature = (jstring) env.CallObjectMethod(runtime->GetJavaRuntime(), RESOLVE_CONSTRUCTOR_SIGNATURE_ID, javaClass, javaArgs);

		const char* str = env.GetStringUTFChars(signature, nullptr);
		resolvedSignature = string(str);
		env.ReleaseStringUTFChars(signature, str);
		env.DeleteLocalRef(signature);
	}
	else
	{
		JsArgToArrayConverter::Error err = argConverter.GetError();
		throw NativeScriptException(err.msg);
	}

	return resolvedSignature;
}

map<string, MethodCache::CacheMethodInfo> MethodCache::s_cache;
jclass MethodCache::RUNTIME_CLASS = nullptr;
jmethodID MethodCache::RESOLVE_METHOD_OVERLOAD_METHOD_ID = nullptr;
jmethodID MethodCache::RESOLVE_CONSTRUCTOR_SIGNATURE_ID = nullptr;

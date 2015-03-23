#include "MethodCache.h"
#include "NativeScriptRuntime.h"
#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

string MethodCache::ResolveMethodSignature(const string& className, const string& methodName, const FunctionCallbackInfo<Value>& args)
{
	string signature;

	// TODO: Possible improvement
	string key = EncodeSignature(className, methodName, args);

	auto it = s_cache.find(key);

	if (it == s_cache.end())
	{
		signature = NativeScriptRuntime::ResolveJavaMethod(args, className, methodName);

		DEBUG_WRITE("ResolveMethodSignature %s='%s'", key.c_str(), signature.c_str());

		if (!signature.empty())
		{
			s_cache.insert(make_pair(key, signature));
		}
	}
	else
	{
		signature = (*it).second;
	}

	return signature;
}


string MethodCache::EncodeSignature(const string& className, const string& methodName, const FunctionCallbackInfo<Value>& args)
{
	string sig(className);
	sig.append(".");
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


string MethodCache::GetType(const v8::Handle<v8::Value>& value)
{
	int x = 1;
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
		else if (!object->GetHiddenValue(V8StringConstants::GetMarkedAsLong()).IsEmpty())
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


map<string, string> MethodCache::s_cache;

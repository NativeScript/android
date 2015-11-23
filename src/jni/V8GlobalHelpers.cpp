#include "V8GlobalHelpers.h"
#include "JEnv.h"

using namespace v8;
using namespace std;

string tns::ConvertToString(const v8::Local<String>& s)
{
	if (s.IsEmpty())
	{
		return string();
	}
	else
	{
		String::Utf8Value str(s);
		return string(*str);
	}
}

jstring tns::ConvertToJavaString(const Local<Value>& value)
{
	JEnv env;
	String::Value stringValue(value);
	return env.NewString((const jchar*)*stringValue, stringValue.length());
}

Local<String> tns::ConvertToV8String(const jchar* data, int length)
{
	auto isolate = Isolate::GetCurrent();
	return String::NewFromTwoByte(isolate, (const uint16_t*)data, String::kNormalString, length);
}

Local<String> tns::ConvertToV8String(const string& s)
{
	auto isolate = Isolate::GetCurrent();
	Local<String> str;
	String::NewFromUtf8(isolate, s.c_str(), NewStringType::kNormal, s.length()).ToLocal(&str);
	return str;
}

Local<String> tns::ConvertToV8String(const char *data, int length)
{
	auto isolate = Isolate::GetCurrent();
	return String::NewFromUtf8(isolate, (const char *)data, String::kNormalString, length);
}

Local<Value> tns::V8GetHiddenValue(const Local<Object>& obj, const string& propName)
{
	auto s = tns::ConvertToV8String(propName);
	return obj->GetHiddenValue(s);
}

bool tns::V8SetHiddenValue(const Local<Object>& obj, const string& propName, const Local<Value>& value)
{
	auto s = tns::ConvertToV8String(propName);
	return obj->SetHiddenValue(s, value);
}

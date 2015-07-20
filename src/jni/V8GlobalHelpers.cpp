#include "V8GlobalHelpers.h"
#include "JEnv.h"

using namespace v8;
using namespace std;

string tns::ConvertToString(const v8::Handle<String>& s)
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

jstring tns::ConvertToJavaString(const Handle<Value>& value)
{
	Handle<String> valueAsString;
	if (value->IsStringObject())
	{
		auto stringObject = Handle<StringObject>::Cast(value);
		valueAsString = stringObject->ValueOf();
	}
	else
	{
		valueAsString = value->ToString();
	}

	JEnv env;
	String::Value stringValue(valueAsString);
	return env.NewString((const jchar*)*stringValue, valueAsString->Length());
}

Local<String> tns::ConvertToV8String(const jchar* data, int length)
{
	auto isolate = Isolate::GetCurrent();
	return String::NewFromTwoByte(isolate, (const uint16_t*)data, String::kNormalString, length);
}

Local<String> tns::ConvertToV8String(const string& s)
{
	auto isolate = Isolate::GetCurrent();
	return String::NewFromUtf8(isolate, s.c_str());
}

Local<String> tns::ConvertToV8String(const char *data, int length)
{
	auto isolate = Isolate::GetCurrent();
	return String::NewFromUtf8(isolate, (const char *)data, String::kNormalString, length);
}

Local<Value> tns::V8GetHiddenValue(const Handle<Object>& obj, const string& propName)
{
	auto s = tns::ConvertToV8String(propName);
	return obj->GetHiddenValue(s);
}

bool tns::V8SetHiddenValue(const Handle<Object>& obj, const string& propName, const Handle<Value>& value)
{
	auto s = tns::ConvertToV8String(propName);
	return obj->SetHiddenValue(s, value);
}

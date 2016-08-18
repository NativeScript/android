#include "V8GlobalHelpers.h"
#include "ArgConverter.h"

using namespace v8;
using namespace std;

Local<Value> tns::V8GetHiddenValue(const Local<Object>& obj, const string& propName)
{
	auto s = ArgConverter::ConvertToV8String(propName);
	return obj->GetHiddenValue(s);
}

bool tns::V8SetHiddenValue(const Local<Object>& obj, const string& propName, const Local<Value>& value)
{
	auto s = ArgConverter::ConvertToV8String(propName);
	return obj->SetHiddenValue(s, value);
}

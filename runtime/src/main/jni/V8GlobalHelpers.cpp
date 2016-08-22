#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "include/v8.h"

using namespace v8;
using namespace std;

Local<Value> tns::V8GetHiddenValue(const Local<Object>& obj, const string& propName)
{
	// TODO: Pete: Temporary - this function is changed in the new v8 api refactoring branch
	auto isolate = Isolate::GetCurrent();
	auto s = ArgConverter::ConvertToV8String(isolate, propName);
	return obj->GetHiddenValue(s);
}

bool tns::V8SetHiddenValue(const Local<Object>& obj, const string& propName, const Local<Value>& value)
{
// TODO: Pete: Temporary - this function is changed in the new v8 api refactoring branch
	auto isolate = Isolate::GetCurrent();
	auto s = ArgConverter::ConvertToV8String(isolate, propName);
	return obj->SetHiddenValue(s, value);
}

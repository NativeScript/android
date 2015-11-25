#include "NumericCasts.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"

using namespace v8;
using namespace std;
using namespace tns;

void NumericCasts::CreateGlobalCastFunctions(const Local<ObjectTemplate>& globalTemplate)
{
	auto isolate = Isolate::GetCurrent();
	auto ext = External::New(isolate, this);

	globalTemplate->Set(ConvertToV8String("long"), FunctionTemplate::New(isolate, NumericCasts::MarkAsLongCallbackStatic, ext));
	globalTemplate->Set(ConvertToV8String("byte"), FunctionTemplate::New(isolate, NumericCasts::MarkAsByteCallbackStatic, ext));
	globalTemplate->Set(ConvertToV8String("short"), FunctionTemplate::New(isolate, NumericCasts::MarkAsShortCallbackStatic, ext));
	globalTemplate->Set(ConvertToV8String("double"), FunctionTemplate::New(isolate, NumericCasts::MarkAsDoubleCallbackStatic, ext));
	globalTemplate->Set(ConvertToV8String("float"), FunctionTemplate::New(isolate, NumericCasts::MarkAsFloatCallbackStatic, ext));
	globalTemplate->Set(ConvertToV8String("char"), FunctionTemplate::New(isolate, NumericCasts::MarkAsCharCallbackStatic, ext));
}

NumericCasts* NumericCasts::GetThis(const v8::FunctionCallbackInfo<Value>& args)
{
	auto ext = args.Data().As<External>();

	auto thisPtr = reinterpret_cast<NumericCasts*>(ext->Value());

	return thisPtr;
}

void NumericCasts::MarkAsLongCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsLongCallback(args);
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

void NumericCasts::MarkAsByteCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsByteCallback(args);
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

void NumericCasts::MarkAsShortCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsShortCallback(args);
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

void NumericCasts::MarkAsCharCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsCharCallback(args);
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

void NumericCasts::MarkAsFloatCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsFloatCallback(args);
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

void NumericCasts::MarkAsDoubleCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsDoubleCallback(args);
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

void NumericCasts::MarkedJsObjectWeakCallback(const v8::WeakCallbackData<Object, Persistent<Object> >& data)
{
	//todo: plamen5kov: re-think using try catch here
	try {
	data.GetParameter()->Reset();
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

void NumericCasts::MarkAsLongCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		throw NativeScriptException(string("long(x) should be called with single parameter"));
	}
	if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject())
	{
		throw NativeScriptException(string("long(x) should be called with single parameter containing a long number representation"));
	}

	Local<Value> value;
	if (args[0]->IsInt32())
	{
		value = args[0]->ToInt32();
	}
	else
	{
		value = args[0]->ToString();
	}

	auto cast = Object::New(isolate);

	auto markedObject = MarkJsObject(cast, V8StringConstants::MARKED_AS_LONG, value);
	if (markedObject != nullptr)
	{
		args.GetReturnValue().Set(*markedObject);
	}
	else
	{
		args.GetReturnValue().Set(cast);
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

void NumericCasts::MarkAsByteCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		throw NativeScriptException(string("byte(x) should be called with single parameter"));
	}
	if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject())
	{
		throw NativeScriptException(string("byte(x) should be called with single parameter containing a byte number representation"));
	}

	Local<Value> value;
	if (args[0]->IsInt32())
	{
		value = args[0]->ToInt32();
	}
	else
	{
		value = args[0]->ToString();
	}

	auto cast = Object::New(isolate);

	auto markedObject = MarkJsObject(cast, V8StringConstants::MARKED_AS_BYTE, value);
	if (markedObject != nullptr)
	{
		args.GetReturnValue().Set(*markedObject);
	}
	else
	{
		args.GetReturnValue().Set(cast);
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

void NumericCasts::MarkAsShortCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		throw NativeScriptException(string("short(x) should be called with single parameter"));
		return;
	}
	if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject())
	{
		throw NativeScriptException(string("short(x) should be called with single parameter containing a short number representation"));
	}

	Local<Value> value;
	if (args[0]->IsInt32())
	{
		value = args[0]->ToInt32();
	}
	else
	{
		value = args[0]->ToString();
	}

	auto cast = Object::New(isolate);

	auto markedObject = MarkJsObject(cast, V8StringConstants::MARKED_AS_SHORT, value);
	if (markedObject != nullptr)
	{
		args.GetReturnValue().Set(*markedObject);
	}
	else
	{
		args.GetReturnValue().Set(cast);
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

void NumericCasts::MarkAsCharCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		throw NativeScriptException(string("char(x) should be called with single parameter"));
	}
	if (!args[0]->IsString())
	{
		throw NativeScriptException(string("char(x) should be called with single parameter containing a char representation"));
	}

	auto value = args[0]->ToString();
	if (value->Length() != 1)
	{
		throw NativeScriptException(string("char(x) should be called with single parameter containing a single char"));
	}

	auto cast = Object::New(isolate);

	auto markedObject = MarkJsObject(cast, V8StringConstants::MARKED_AS_CHAR, value);
	if (markedObject != nullptr)
	{
		args.GetReturnValue().Set(*markedObject);
	}
	else
	{
		args.GetReturnValue().Set(cast);
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

void NumericCasts::MarkAsFloatCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		throw NativeScriptException(string("float(x) should be called with single parameter"));
	}
	if (!args[0]->IsNumber())
	{
		throw NativeScriptException(string("float(x) should be called with single parameter containing a float number representation"));
	}

	auto value = args[0]->ToNumber();
	auto cast = Object::New(isolate);

	auto markedObject = MarkJsObject(cast, V8StringConstants::MARKED_AS_FLOAT, value);
	if (markedObject != nullptr)
	{
		args.GetReturnValue().Set(*markedObject);
	}
	else
	{
		args.GetReturnValue().Set(cast);
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

void NumericCasts::MarkAsDoubleCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try {
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		throw NativeScriptException(string("double(x) should be called with single parameter"));
	}
	if (!args[0]->IsNumber())
	{
		throw NativeScriptException(string("double(x) should be called with single parameter containing a double number representation"));
	}

	auto value = args[0]->ToNumber();
	auto cast = Object::New(isolate);

	auto markedObject = MarkJsObject(cast, V8StringConstants::MARKED_AS_DOUBLE, value);
	if (markedObject != nullptr)
	{
		args.GetReturnValue().Set(*markedObject);
	}
	else
	{
		args.GetReturnValue().Set(cast);
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

Persistent<Object>* NumericCasts::MarkJsObject(const Local<Object>& object, const string& mark, const Local<Value>& value)
{
	auto isolate = Isolate::GetCurrent();

	auto key = ConvertToV8String(mark);
	auto marked = object->GetHiddenValue(key);

	//Allow marking again with different value, but don't oversubscribe for weak event for second time
	if (!marked.IsEmpty())
	{
		object->SetHiddenValue(key, value);
		return nullptr;
	}

	object->SetHiddenValue(key, value);
	DEBUG_WRITE("MarkJsObject: Marking js object: %d with %s", object->GetIdentityHash(), mark.c_str());

	auto objectHandle = new Persistent<Object>(isolate, object);
	objectHandle->SetWeak(objectHandle, MarkedJsObjectWeakCallback);
	return objectHandle;
}

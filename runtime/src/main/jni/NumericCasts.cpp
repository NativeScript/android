#include "NumericCasts.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

void NumericCasts::CreateGlobalCastFunctions(Isolate *isolate, const Local<ObjectTemplate>& globalTemplate)
{
	auto ext = External::New(isolate, this);

	globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "long"), FunctionTemplate::New(isolate, NumericCasts::MarkAsLongCallbackStatic, ext));
	globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "byte"), FunctionTemplate::New(isolate, NumericCasts::MarkAsByteCallbackStatic, ext));
	globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "short"), FunctionTemplate::New(isolate, NumericCasts::MarkAsShortCallbackStatic, ext));
	globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "double"), FunctionTemplate::New(isolate, NumericCasts::MarkAsDoubleCallbackStatic, ext));
	globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "float"), FunctionTemplate::New(isolate, NumericCasts::MarkAsFloatCallbackStatic, ext));
	globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "char"), FunctionTemplate::New(isolate, NumericCasts::MarkAsCharCallbackStatic, ext));

	s_castMarker = new Persistent<String>(isolate, ArgConverter::ConvertToV8String(isolate, "t::cast"));
}

CastType NumericCasts::GetCastType(const Local<Object>& object)
{
	auto ret = CastType::None;
	auto isolate = object->GetIsolate();
	auto key = Local<String>::New(isolate, *s_castMarker);
	auto hidden = object->GetHiddenValue(key);
	if (!hidden.IsEmpty())
	{
		ret = static_cast<CastType>(hidden->Int32Value());
	}

	return ret;
}

Local<Value> NumericCasts::GetCastValue(const Local<Object>& object)
{
	auto isolate = object->GetIsolate();
	auto value = object->Get(ArgConverter::ConvertToV8String(isolate, "value"));
	return value;
}

void NumericCasts::MarkAsLong(const v8::Local<v8::Object>& object, const v8::Local<v8::Value>& value)
{
	MarkJsObject(object, CastType::Long, value);
}

NumericCasts* NumericCasts::GetThis(const v8::FunctionCallbackInfo<Value>& args)
{
	auto ext = args.Data().As<External>();

	auto thisPtr = reinterpret_cast<NumericCasts*>(ext->Value());

	return thisPtr;
}

void NumericCasts::MarkAsLongCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
		auto thisPtr = GetThis(args);

		thisPtr->MarkAsLongCallback(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsByteCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
		auto thisPtr = GetThis(args);

		thisPtr->MarkAsByteCallback(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsShortCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
		auto thisPtr = GetThis(args);

		thisPtr->MarkAsShortCallback(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsCharCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
		auto thisPtr = GetThis(args);

		thisPtr->MarkAsCharCallback(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsFloatCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
		auto thisPtr = GetThis(args);

		thisPtr->MarkAsFloatCallback(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsDoubleCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
		auto thisPtr = GetThis(args);

		thisPtr->MarkAsDoubleCallback(args);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsLongCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
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
		MarkJsObject(cast, CastType::Long, value);
		args.GetReturnValue().Set(cast);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsByteCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
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
		MarkJsObject(cast, CastType::Byte, value);
		args.GetReturnValue().Set(cast);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsShortCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
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
		MarkJsObject(cast, CastType::Short, value);
		args.GetReturnValue().Set(cast);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsCharCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
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
		MarkJsObject(cast, CastType::Char, value);
		args.GetReturnValue().Set(cast);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsFloatCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
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
		MarkJsObject(cast, CastType::Float, value);
		args.GetReturnValue().Set(cast);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkAsDoubleCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	try
	{
		auto isolate = args.GetIsolate();

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
		MarkJsObject(cast, CastType::Double, value);
		args.GetReturnValue().Set(cast);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		stringstream ss;
		ss << "Error: c++ exception: " << e.what() << endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void NumericCasts::MarkJsObject(const Local<Object>& object, CastType castType, const Local<Value>& value)
{
	auto isolate = object->GetIsolate();
	auto key = Local<String>::New(isolate, *s_castMarker);
	auto type = Integer::New(isolate, static_cast<int>(castType));

	object->SetHiddenValue(key, type);
	object->Set(ArgConverter::ConvertToV8String(isolate, "value"), value);
	DEBUG_WRITE("MarkJsObject: Marking js object: %d with cast type: %d", object->GetIdentityHash(), castType);
}

Persistent<String> *NumericCasts::s_castMarker = nullptr;

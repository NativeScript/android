#include "NumericCasts.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"

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
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsLongCallback(args);
}

void NumericCasts::MarkAsByteCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsByteCallback(args);
}

void NumericCasts::MarkAsShortCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsShortCallback(args);
}

void NumericCasts::MarkAsCharCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsCharCallback(args);
}

void NumericCasts::MarkAsFloatCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsFloatCallback(args);
}

void NumericCasts::MarkAsDoubleCallbackStatic(const v8::FunctionCallbackInfo<Value>& args)
{
	auto thisPtr = GetThis(args);

	thisPtr->MarkAsDoubleCallback(args);
}

void NumericCasts::MarkedJsObjectWeakCallback(const v8::WeakCallbackData<Object, Persistent<Object> >& data)
{
	data.GetParameter()->Reset();
}

void NumericCasts::MarkAsLongCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		isolate->ThrowException(ConvertToV8String("long(x) should be called with single parameter"));
		return;
	}
	if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject())
	{
		isolate->ThrowException(ConvertToV8String("long(x) should be called with single parameter containing a long number representation"));
		return;
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
}

void NumericCasts::MarkAsByteCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		isolate->ThrowException(ConvertToV8String("byte(x) should be called with single parameter"));
		return;
	}
	if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject())
	{
		isolate->ThrowException(ConvertToV8String("byte(x) should be called with single parameter containing a byte number representation"));
		return;
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
}

void NumericCasts::MarkAsShortCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		isolate->ThrowException(ConvertToV8String("short(x) should be called with single parameter"));
		return;
	}
	if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject())
	{
		isolate->ThrowException(ConvertToV8String("short(x) should be called with single parameter containing a short number representation"));
		return;
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
}

void NumericCasts::MarkAsCharCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		isolate->ThrowException(ConvertToV8String("char(x) should be called with single parameter"));
		return;
	}
	if (!args[0]->IsString())
	{
		isolate->ThrowException(ConvertToV8String("char(x) should be called with single parameter containing a char representation"));
		return;
	}

	auto value = args[0]->ToString();
	if (value->Length() != 1)
	{
		isolate->ThrowException(ConvertToV8String("char(x) should be called with single parameter containing a single char"));
		return;
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
}

void NumericCasts::MarkAsFloatCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		isolate->ThrowException(ConvertToV8String("float(x) should be called with single parameter"));
		return;
	}
	if (!args[0]->IsNumber())
	{
		isolate->ThrowException(ConvertToV8String("float(x) should be called with single parameter containing a float number representation"));
		return;
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
}

void NumericCasts::MarkAsDoubleCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();

	if (args.Length() != 1)
	{
		isolate->ThrowException(ConvertToV8String("double(x) should be called with single parameter"));
		return;
	}
	if (!args[0]->IsNumber())
	{
		isolate->ThrowException(ConvertToV8String("double(x) should be called with single parameter containing a double number representation"));
		return;
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

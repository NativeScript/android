#include "V8StringConstants.h"

using namespace v8;
using namespace std;

namespace tns
{
	V8StringConstants::PerIsolateV8Constants *V8StringConstants::GetConstantsForIsolate(v8::Isolate *isolate) {
		// TODO: Pete: Create enumeration - 1 is the slot at which we store our constants structure for each isolate
		auto data = isolate->GetData(1);
		auto consts = (struct V8StringConstants::PerIsolateV8Constants*) data;

		// TODO: Pete: Assert that consts object is not null
		return consts;
	}

	Local<String> V8StringConstants::GetClassImplementationObject(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->CLASS_IMPLEMENTATION_OBJECT_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, CLASS_IMPLEMENTATION_OBJECT.c_str());
			consts->CLASS_IMPLEMENTATION_OBJECT_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->CLASS_IMPLEMENTATION_OBJECT_PERSISTENT);
	}

	Local<String> V8StringConstants::GetExtend(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->EXTEND_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, EXTEND.c_str());
			consts->EXTEND_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->EXTEND_PERSISTENT);
	}

	Local<String> V8StringConstants::GetNullObject(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->NULL_OBJECT_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, NULL_OBJECT.c_str());
			consts->NULL_OBJECT_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->NULL_OBJECT_PERSISTENT);
	}

	Local<String> V8StringConstants::GetNullNodeName(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->NULL_NODE_NAME_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, NULL_NODE_NAME.c_str());
			consts->NULL_NODE_NAME_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->NULL_NODE_NAME_PERSISTENT);
	}

	Local<String> V8StringConstants::GetIsPrototypeImplementationObject(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, IS_PROTOTYPE_IMPLEMENTATION_OBJECT.c_str());
			consts->IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT);
	}

	Local<String> V8StringConstants::GetNativeException(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->NATIVE_EXCEPTION_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, NATIVE_EXCEPTION.c_str());
			consts->NATIVE_EXCEPTION_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->NATIVE_EXCEPTION_PERSISTENT);
	}

	Local<String> V8StringConstants::GetStackTrace(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->STACK_TRACE_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, STACK_TRACE.c_str());
			consts->STACK_TRACE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->STACK_TRACE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetLongNumber(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->LONG_NUMBER_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, LONG_NUMBER.c_str());
			consts->LONG_NUMBER_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->LONG_NUMBER_PERSISTENT);
	}

	Local<String> V8StringConstants::GetPrototype(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->PROTOTYPE_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, PROTOTYPE.c_str());
			consts->PROTOTYPE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->PROTOTYPE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetSuper(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->SUPER_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, SUPER.c_str());
			consts->SUPER_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->SUPER_PERSISTENT);
	}

	Local<String> V8StringConstants::GetTSuper(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->T_SUPER_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, T_SUPER.c_str());
			consts->T_SUPER_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->T_SUPER_PERSISTENT);
	}

	Local<String> V8StringConstants::GetTarget(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->TARGET_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, TARGET.c_str());
			consts->TARGET_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->TARGET_PERSISTENT);
	}

	Local<String> V8StringConstants::GetToString(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->TO_STRING_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, TO_STRING.c_str());
			consts->TO_STRING_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->TO_STRING_PERSISTENT);
	}

	Local<String> V8StringConstants::GetHiddenJSInstance(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->HIDDEN_JS_INSTANCE_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, HIDDEN_JS_INSTANCE.c_str());
			consts->HIDDEN_JS_INSTANCE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->HIDDEN_JS_INSTANCE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetJavaLong(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->JAVA_LONG_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, JAVA_LONG.c_str());
			consts->JAVA_LONG_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->JAVA_LONG_PERSISTENT);
	}

	Local<String> V8StringConstants::GetValue(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->VALUE_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, VALUE.c_str());
			consts->VALUE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->VALUE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetValueOf(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->VALUE_OF_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, VALUE_OF.c_str());
			consts->VALUE_OF_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->VALUE_OF_PERSISTENT);
	}

	Local<String> V8StringConstants::GetUncaughtError(Isolate *isolate)
	{
		auto consts = GetConstantsForIsolate(isolate);

		if (consts->UNCAUGHT_ERROR_PERSISTENT == nullptr)
		{
			auto str = String::NewFromUtf8(isolate, UNCAUGHT_ERROR.c_str());
			consts->UNCAUGHT_ERROR_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}

		return Local<String>::New(isolate, *consts->UNCAUGHT_ERROR_PERSISTENT);
	}

	const string V8StringConstants::CLASS_IMPLEMENTATION_OBJECT = "t::ClassImplementationObject";
	const string V8StringConstants::EXTEND = "extend";
	const string V8StringConstants::NULL_OBJECT = "null";
	const string V8StringConstants::NULL_NODE_NAME = "nullNode";
	const string V8StringConstants::IS_PROTOTYPE_IMPLEMENTATION_OBJECT = "__isPrototypeImplementationObject";
	const string V8StringConstants::NATIVE_EXCEPTION = "nativeException";
	const string V8StringConstants::STACK_TRACE = "stackTrace";
	const string V8StringConstants::LONG_NUMBER = "NativeScriptLongNumber";
	const string V8StringConstants::PROTOTYPE = "prototype";
	const string V8StringConstants::SUPER = "super";
	const string V8StringConstants::T_SUPER = "t:super";
	const string V8StringConstants::TARGET = "target";
	const string V8StringConstants::TO_STRING = "toString";
	const string V8StringConstants::HIDDEN_JS_INSTANCE = "t::HiddenJSInstanceInfo";
	const string V8StringConstants::JAVA_LONG = "t::JavaLong";
	const string V8StringConstants::MARKED_AS_BYTE = "t::MarkedAsByte";
	const string V8StringConstants::MARKED_AS_CHAR = "t::MarkedAsChar";
	const string V8StringConstants::MARKED_AS_DOUBLE = "t::MarkedAsDouble";
	const string V8StringConstants::MARKED_AS_FLOAT = "t::MarkedAsFloat";
	const string V8StringConstants::MARKED_AS_LONG = "t::MarkedAsLong";
	const string V8StringConstants::MARKED_AS_SHORT = "t::MarkedAsShort";
	const string V8StringConstants::VALUE = "value";
	const string V8StringConstants::VALUE_OF = "valueOf";
	const string V8StringConstants::UNCAUGHT_ERROR = "__onUncaughtError";
}

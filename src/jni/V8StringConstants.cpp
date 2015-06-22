#include "V8StringConstants.h"
#include "NativeScriptAssert.h"

using namespace v8;
using namespace std;

namespace tns {
	Local<String> V8StringConstants::GetClassImplementationObject(){
		auto isolate = Isolate::GetCurrent();
		if(CLASS_IMPLEMENTATION_OBJECT_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, CLASS_IMPLEMENTATION_OBJECT.c_str());
			CLASS_IMPLEMENTATION_OBJECT_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *CLASS_IMPLEMENTATION_OBJECT_PERSISTENT);
	}

	Local<String> V8StringConstants::GetExtend(){
		auto isolate = Isolate::GetCurrent();
		if(EXTEND_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, EXTEND.c_str());
			EXTEND_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *EXTEND_PERSISTENT);
	}

	Local<String> V8StringConstants::GetIsPrototypeImplementationObject(){
		auto isolate = Isolate::GetCurrent();
		if(IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, IS_PROTOTYPE_IMPLEMENTATION_OBJECT.c_str());
			IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT);
	}

	Local<String> V8StringConstants::GetNativeException(){
		auto isolate = Isolate::GetCurrent();
		if(NATIVE_EXCEPTION_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, NATIVE_EXCEPTION.c_str());
			NATIVE_EXCEPTION_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *NATIVE_EXCEPTION_PERSISTENT);
	}

	Local<String> V8StringConstants::GetStackTrace(){
		auto isolate = Isolate::GetCurrent();
		if(STACK_TRACE_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, STACK_TRACE.c_str());
			STACK_TRACE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *STACK_TRACE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetLongNumber(){
		auto isolate = Isolate::GetCurrent();
		if(LONG_NUMBER_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, LONG_NUMBER.c_str());
			LONG_NUMBER_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *LONG_NUMBER_PERSISTENT);
	}

	Local<String> V8StringConstants::GetPrototype(){
		auto isolate = Isolate::GetCurrent();
		if(PROTOTYPE_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, PROTOTYPE.c_str());
			PROTOTYPE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *PROTOTYPE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetSuper(){
		auto isolate = Isolate::GetCurrent();
		if(SUPER_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, SUPER.c_str());
			SUPER_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *SUPER_PERSISTENT);
	}

	Local<String> V8StringConstants::GetTSuper(){
		auto isolate = Isolate::GetCurrent();
		if(T_SUPER_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, T_SUPER.c_str());
			T_SUPER_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *T_SUPER_PERSISTENT);
	}

	Local<String> V8StringConstants::GetTarget(){
		auto isolate = Isolate::GetCurrent();
		if(TARGET_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, TARGET.c_str());
			TARGET_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *TARGET_PERSISTENT);
	}

	Local<String> V8StringConstants::GetToString(){
		auto isolate = Isolate::GetCurrent();
		if(TO_STRING_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, TO_STRING.c_str());
			TO_STRING_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *TO_STRING_PERSISTENT);
	}

	Local<String> V8StringConstants::GetHiddenJSInstance(){
		auto isolate = Isolate::GetCurrent();
		if(HIDDEN_JS_INSTANCE_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, HIDDEN_JS_INSTANCE.c_str());
			HIDDEN_JS_INSTANCE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *HIDDEN_JS_INSTANCE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetJavaLong(){
		auto isolate = Isolate::GetCurrent();
		if(JAVA_LONG_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, JAVA_LONG.c_str());
			JAVA_LONG_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *JAVA_LONG_PERSISTENT);
	}

	Local<String> V8StringConstants::GetMarkedAsByte(){
		auto isolate = Isolate::GetCurrent();
		if(MARKED_AS_BYTE_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, MARKED_AS_BYTE.c_str());
			MARKED_AS_BYTE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *MARKED_AS_BYTE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetMarkedAsChar(){
		auto isolate = Isolate::GetCurrent();
		if(MARKED_AS_CHAR_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, MARKED_AS_CHAR.c_str());
			MARKED_AS_CHAR_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *MARKED_AS_CHAR_PERSISTENT);
	}

	Local<String> V8StringConstants::GetMarkedAsDouble(){
		auto isolate = Isolate::GetCurrent();
		if(MARKED_AS_DOUBLE_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, MARKED_AS_DOUBLE.c_str());
			MARKED_AS_DOUBLE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *MARKED_AS_DOUBLE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetMarkedAsFloat(){
		auto isolate = Isolate::GetCurrent();
		if(MARKED_AS_FLOAT_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, MARKED_AS_FLOAT.c_str());
			MARKED_AS_FLOAT_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *MARKED_AS_FLOAT_PERSISTENT);
	}

	Local<String> V8StringConstants::GetMarkedAsLong(){
		auto isolate = Isolate::GetCurrent();
		if(MARKED_AS_LONG_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, MARKED_AS_LONG.c_str());
			MARKED_AS_LONG_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *MARKED_AS_LONG_PERSISTENT);
	}

	Local<String> V8StringConstants::GetMarkedAsShort(){
		auto isolate = Isolate::GetCurrent();
		if(MARKED_AS_SHORT_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, MARKED_AS_SHORT.c_str());
			MARKED_AS_SHORT_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *MARKED_AS_SHORT_PERSISTENT);
	}

	Local<String> V8StringConstants::GetValue(){
		auto isolate = Isolate::GetCurrent();
		if(VALUE_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, VALUE.c_str());
			VALUE_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *VALUE_PERSISTENT);
	}

	Local<String> V8StringConstants::GetValueOf(){
		auto isolate = Isolate::GetCurrent();
		if(VALUE_OF_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, VALUE_OF.c_str());
			VALUE_OF_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *VALUE_OF_PERSISTENT);
	}

	Local<String> V8StringConstants::GetUncaughtError(){
		auto isolate = Isolate::GetCurrent();
		if(UNCAUGHT_ERROR_PERSISTENT == nullptr) {
			auto str = String::NewFromUtf8(isolate, UNCAUGHT_ERROR.c_str());
			UNCAUGHT_ERROR_PERSISTENT = new Persistent<String>(Isolate::GetCurrent(), str);
		}
		return Local<String>::New(isolate, *UNCAUGHT_ERROR_PERSISTENT);
	}

	const string V8StringConstants::CLASS_IMPLEMENTATION_OBJECT = "t::ClassImplementationObject";
	const string V8StringConstants::EXTEND = "extend";
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

	// TODO: Release these objects when we implement V8 reset
	// TODO: These are not thread-safe!
	Persistent<String> *V8StringConstants::CLASS_IMPLEMENTATION_OBJECT_PERSISTENT;
	Persistent<String> *V8StringConstants::EXTEND_PERSISTENT;
	Persistent<String> *V8StringConstants::IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT;
	Persistent<String> *V8StringConstants::NATIVE_EXCEPTION_PERSISTENT;
	Persistent<String> *V8StringConstants::STACK_TRACE_PERSISTENT;
	Persistent<String> *V8StringConstants::LONG_NUMBER_PERSISTENT;
	Persistent<String> *V8StringConstants::PROTOTYPE_PERSISTENT;
	Persistent<String> *V8StringConstants::SUPER_PERSISTENT;
	Persistent<String> *V8StringConstants::T_SUPER_PERSISTENT;
	Persistent<String> *V8StringConstants::TARGET_PERSISTENT;
	Persistent<String> *V8StringConstants::TO_STRING_PERSISTENT;
	Persistent<String> *V8StringConstants::HIDDEN_JS_INSTANCE_PERSISTENT;
	Persistent<String> *V8StringConstants::JAVA_LONG_PERSISTENT;
	Persistent<String> *V8StringConstants::MARKED_AS_BYTE_PERSISTENT;
	Persistent<String> *V8StringConstants::MARKED_AS_CHAR_PERSISTENT;
	Persistent<String> *V8StringConstants::MARKED_AS_DOUBLE_PERSISTENT;
	Persistent<String> *V8StringConstants::MARKED_AS_FLOAT_PERSISTENT;
	Persistent<String> *V8StringConstants::MARKED_AS_LONG_PERSISTENT;
	Persistent<String> *V8StringConstants::MARKED_AS_SHORT_PERSISTENT;
	Persistent<String> *V8StringConstants::VALUE_PERSISTENT;
	Persistent<String> *V8StringConstants::VALUE_OF_PERSISTENT;
	Persistent<String> *V8StringConstants::UNCAUGHT_ERROR_PERSISTENT;
}

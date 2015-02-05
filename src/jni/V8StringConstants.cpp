#include "V8StringConstants.h"
#include "NativeScriptAssert.h"

using namespace v8;
using namespace std;

namespace tns {
	Local<String> V8StringConstants::GetClassImplementationObject(){
		return String::NewFromUtf8(Isolate::GetCurrent(), CLASS_IMPLEMENTATION_OBJECT.c_str());
	}

	Local<String> V8StringConstants::GetDebugName(){
		return String::NewFromUtf8(Isolate::GetCurrent(), DEBUG_NAME.c_str());
	}

	Local<String> V8StringConstants::GetExtends(){
		return String::NewFromUtf8(Isolate::GetCurrent(), EXTENDS.c_str());
	}

	Local<String> V8StringConstants::GetIsPrototypeImplementationObject(){
		return String::NewFromUtf8(Isolate::GetCurrent(), IS_PROTOTYPE_IMPLEMENTATION_OBJECT.c_str());
	}

	Local<String> V8StringConstants::GetNativeException(){
		return String::NewFromUtf8(Isolate::GetCurrent(), NATIVE_EXCEPTION.c_str());
	}

	Local<String> V8StringConstants::GetStackTrace(){
		return String::NewFromUtf8(Isolate::GetCurrent(), STACK_TRACE.c_str());
	}

	Local<String> V8StringConstants::GetLongNumber(){
		return String::NewFromUtf8(Isolate::GetCurrent(), LONG_NUMBER.c_str());
	}

	Local<String> V8StringConstants::GetPrototype(){
		return String::NewFromUtf8(Isolate::GetCurrent(), PROTOTYPE.c_str());
	}

	Local<String> V8StringConstants::GetSuper(){
		return String::NewFromUtf8(Isolate::GetCurrent(), SUPER.c_str());
	}

	Local<String> V8StringConstants::GetTSuper(){
		return String::NewFromUtf8(Isolate::GetCurrent(), T_SUPER.c_str());
	}

	Local<String> V8StringConstants::GetTarget(){
		return String::NewFromUtf8(Isolate::GetCurrent(), TARGET.c_str());
	}

	Local<String> V8StringConstants::GetToString(){
		return String::NewFromUtf8(Isolate::GetCurrent(), TO_STRING.c_str());
	}

	Local<String> V8StringConstants::GetHiddenJSInstance(){
		return String::NewFromUtf8(Isolate::GetCurrent(), HIDDEN_JS_INSTANCE.c_str());
	}

	Local<String> V8StringConstants::GetJavaLong(){
		return String::NewFromUtf8(Isolate::GetCurrent(), JAVA_LONG.c_str());
	}

	Local<String> V8StringConstants::GetMarkedAsByte(){
		return String::NewFromUtf8(Isolate::GetCurrent(), MARKED_AS_BYTE.c_str());
	}

	Local<String> V8StringConstants::GetMarkedAsChar(){
		return String::NewFromUtf8(Isolate::GetCurrent(), MARKED_AS_CHAR.c_str());
	}

	Local<String> V8StringConstants::GetMarkedAsDouble(){
		return String::NewFromUtf8(Isolate::GetCurrent(), MARKED_AS_DOUBLE.c_str());
	}

	Local<String> V8StringConstants::GetMarkedAsFloat(){
		return String::NewFromUtf8(Isolate::GetCurrent(), MARKED_AS_FLOAT.c_str());
	}

	Local<String> V8StringConstants::GetMarkedAsLong(){
		return String::NewFromUtf8(Isolate::GetCurrent(), MARKED_AS_LONG.c_str());
	}

	Local<String> V8StringConstants::GetMarkedAsShort(){
		return String::NewFromUtf8(Isolate::GetCurrent(), MARKED_AS_SHORT.c_str());
	}

	Local<String> V8StringConstants::GetValue(){
		return String::NewFromUtf8(Isolate::GetCurrent(), VALUE.c_str());
	}

	Local<String> V8StringConstants::GetValueOf(){
		return String::NewFromUtf8(Isolate::GetCurrent(), VALUE_OF.c_str());
	}

	Local<String> V8StringConstants::GetUncaughtError(){
			return String::NewFromUtf8(Isolate::GetCurrent(), UNCAUGHT_ERROR.c_str());
		}

	const string V8StringConstants::CLASS_IMPLEMENTATION_OBJECT = "t::ClassImplementationObject";
	const string V8StringConstants::DEBUG_NAME = "__debugName__";
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
}

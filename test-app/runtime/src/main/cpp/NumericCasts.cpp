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

void NumericCasts::CreateGlobalCastFunctions(Isolate* isolate, const Local<ObjectTemplate>& globalTemplate) {
    auto ext = External::New(isolate, this);

    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "long"), FunctionTemplate::New(isolate, NumericCasts::MarkAsLongCallbackStatic, ext));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "byte"), FunctionTemplate::New(isolate, NumericCasts::MarkAsByteCallbackStatic, ext));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "short"), FunctionTemplate::New(isolate, NumericCasts::MarkAsShortCallbackStatic, ext));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "double"), FunctionTemplate::New(isolate, NumericCasts::MarkAsDoubleCallbackStatic, ext));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "float"), FunctionTemplate::New(isolate, NumericCasts::MarkAsFloatCallbackStatic, ext));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "char"), FunctionTemplate::New(isolate, NumericCasts::MarkAsCharCallbackStatic, ext));
}

CastType NumericCasts::GetCastType(Isolate* isolate, const Local<Object>& object) {
    auto ret = CastType::None;
    auto key = ArgConverter::ConvertToV8String(isolate, s_castMarker);
    Local<Value> hidden;
    V8GetPrivateValue(isolate, object, key, hidden);
    if (!hidden.IsEmpty()) {
        auto context = isolate->GetCurrentContext();
        ret = static_cast<CastType>(hidden->Int32Value(context).ToChecked());
    }

    return ret;
}

Local<Value> NumericCasts::GetCastValue(const Local<Object>& object) {
    auto isolate = object->GetIsolate();
    auto value = object->Get(V8StringConstants::GetValue(isolate));
    return value;
}

void NumericCasts::MarkAsLong(Isolate* isolate, const v8::Local<v8::Object>& object, const v8::Local<v8::Value>& value) {
    MarkJsObject(isolate, object, CastType::Long, value);
}

NumericCasts* NumericCasts::GetThis(const v8::FunctionCallbackInfo<Value>& args) {
    auto ext = args.Data().As<External>();

    auto thisPtr = reinterpret_cast<NumericCasts*>(ext->Value());

    return thisPtr;
}

void NumericCasts::MarkAsLongCallbackStatic(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto thisPtr = GetThis(args);

        thisPtr->MarkAsLongCallback(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsByteCallbackStatic(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto thisPtr = GetThis(args);

        thisPtr->MarkAsByteCallback(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsShortCallbackStatic(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto thisPtr = GetThis(args);

        thisPtr->MarkAsShortCallback(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsCharCallbackStatic(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto thisPtr = GetThis(args);

        thisPtr->MarkAsCharCallback(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsFloatCallbackStatic(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto thisPtr = GetThis(args);

        thisPtr->MarkAsFloatCallback(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsDoubleCallbackStatic(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto thisPtr = GetThis(args);

        thisPtr->MarkAsDoubleCallback(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsLongCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();

        if (args.Length() != 1) {
            throw NativeScriptException(string("long(x) should be called with single parameter"));
        }
        if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject()) {
            throw NativeScriptException(string("long(x) should be called with single parameter containing a long number representation"));
        }

        Local<Value> value;
        if (args[0]->IsInt32()) {
            value = args[0]->ToInt32(isolate);
        } else {
            value = args[0]->ToString(isolate);
        }

        auto cast = Object::New(isolate);
        MarkJsObject(isolate, cast, CastType::Long, value);
        args.GetReturnValue().Set(cast);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsByteCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();

        if (args.Length() != 1) {
            throw NativeScriptException(string("byte(x) should be called with single parameter"));
        }
        if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject()) {
            throw NativeScriptException(string("byte(x) should be called with single parameter containing a byte number representation"));
        }

        Local<Value> value;
        if (args[0]->IsInt32()) {
            value = args[0]->ToInt32(isolate);
        } else {
            value = args[0]->ToString(isolate);
        }

        auto cast = Object::New(isolate);
        MarkJsObject(isolate, cast, CastType::Byte, value);
        args.GetReturnValue().Set(cast);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsShortCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();

        if (args.Length() != 1) {
            throw NativeScriptException(string("short(x) should be called with single parameter"));
            return;
        }
        if (!args[0]->IsString() && !args[0]->IsStringObject() && !args[0]->IsNumber() && !args[0]->IsNumberObject()) {
            throw NativeScriptException(string("short(x) should be called with single parameter containing a short number representation"));
        }

        Local<Value> value;
        if (args[0]->IsInt32()) {
            value = args[0]->ToInt32(isolate);
        } else {
            value = args[0]->ToString(isolate);
        }

        auto cast = Object::New(isolate);
        MarkJsObject(isolate, cast, CastType::Short, value);
        args.GetReturnValue().Set(cast);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsCharCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();

        if (args.Length() != 1) {
            throw NativeScriptException(string("char(x) should be called with single parameter"));
        }
        if (!args[0]->IsString()) {
            throw NativeScriptException(string("char(x) should be called with single parameter containing a char representation"));
        }

        auto value = args[0]->ToString(isolate);
        if (value->Length() != 1) {
            throw NativeScriptException(string("char(x) should be called with single parameter containing a single char"));
        }

        auto cast = Object::New(isolate);
        MarkJsObject(isolate, cast, CastType::Char, value);
        args.GetReturnValue().Set(cast);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsFloatCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();

        if (args.Length() != 1) {
            throw NativeScriptException(string("float(x) should be called with single parameter"));
        }
        if (!args[0]->IsNumber()) {
            throw NativeScriptException(string("float(x) should be called with single parameter containing a float number representation"));
        }

        auto value = args[0]->ToNumber(isolate);
        auto cast = Object::New(isolate);
        MarkJsObject(isolate, cast, CastType::Float, value);
        args.GetReturnValue().Set(cast);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkAsDoubleCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();

        if (args.Length() != 1) {
            throw NativeScriptException(string("double(x) should be called with single parameter"));
        }
        if (!args[0]->IsNumber()) {
            throw NativeScriptException(string("double(x) should be called with single parameter containing a double number representation"));
        }

        auto value = args[0]->ToNumber(isolate);
        auto cast = Object::New(isolate);
        MarkJsObject(isolate, cast, CastType::Double, value);
        args.GetReturnValue().Set(cast);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void NumericCasts::MarkJsObject(Isolate* isolate, const Local<Object>& object, CastType castType, const Local<Value>& value) {
    auto key = ArgConverter::ConvertToV8String(isolate, s_castMarker);
    auto type = Integer::New(isolate, static_cast<int>(castType));
    V8SetPrivateValue(isolate, object, key, type);
    object->Set(V8StringConstants::GetValue(isolate), value);

    DEBUG_WRITE("MarkJsObject: Marking js object: %d with cast type: %d", object->GetIdentityHash(), castType);
}

std::string NumericCasts::s_castMarker = "t::cast";

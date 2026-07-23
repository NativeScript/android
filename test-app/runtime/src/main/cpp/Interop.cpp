#include "Interop.h"

#include <string>

#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"
#include "ObjectManager.h"
#include "Runtime.h"
#include "V8StringConstants.h"

using namespace std;
using namespace tns;
using namespace v8;

/*
 * The brand is a per-isolate private symbol: invisible to JS property
 * enumeration, so app code can neither observe nor forge it. Its value is the
 * escape payload - `{nativeException}` when the branded error carries an
 * original Java throwable, `{name, message, stack}` synthesis info otherwise
 * (mirroring the iOS runtime's payload shape).
 */
static Local<Private> GetBrand(Isolate* isolate) {
    return Private::ForApi(
            isolate,
            ArgConverter::ConvertToV8String(isolate, "tns::escapedExceptionPayload"));
}

/*
 * Resolves the JS wrapper of a Java throwable carried by `value` - `value`
 * itself when it wraps a java.lang.Throwable, or its `nativeException` when
 * `value` is an Error carrying one. Empty handle otherwise.
 */
static Local<Value> GetWrappedJavaThrowable(Local<Context> context, Local<Value> value) {
    auto isolate = context->GetIsolate();

    auto isWrappedThrowable = [&](Local<Value> v) -> bool {
        if (v.IsEmpty() || !v->IsObject()) {
            return false;
        }
        auto objectManager = Runtime::GetObjectManager(isolate);
        auto javaObj = objectManager->GetJavaObjectByJsObject(v.As<Object>());
        if (javaObj.IsNull()) {
            return false;
        }
        JEnv env;
        JniLocalRef objClass(env.GetObjectClass(javaObj));
        jclass throwableClass = env.FindClass("java/lang/Throwable");
        return env.IsAssignableFrom(objClass, throwableClass) == JNI_TRUE;
    };

    if (isWrappedThrowable(value)) {
        return value;
    }
    if (value->IsObject()) {
        Local<Value> nativeExc;
        if (value.As<Object>()
                    ->Get(context, V8StringConstants::GetNativeException(isolate))
                    .ToLocal(&nativeExc) &&
            isWrappedThrowable(nativeExc)) {
            return nativeExc;
        }
    }
    return Local<Value>();
}

static void EscapeExceptionCallback(const FunctionCallbackInfo<Value>& info) {
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();

    if (info.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(
                isolate,
                "interop.escapeException requires 1 argument, but only 0 present.")));
        return;
    }

    Local<Value> x = info[0];
    Local<Private> brand = GetBrand(isolate);

    // Idempotent: an already-branded value is returned unchanged.
    if (x->IsObject() &&
        x.As<Object>()->HasPrivate(context, brand).FromMaybe(false)) {
        info.GetReturnValue().Set(x);
        return;
    }

    // Derive the message string, preferring x.message when x is an Error.
    string message;
    bool xIsObject = x->IsObject();
    Local<String> strVal;
    if (xIsObject) {
        Local<Value> msgVal;
        if (x.As<Object>()
                    ->Get(context, ArgConverter::ConvertToV8String(isolate, "message"))
                    .ToLocal(&msgVal) &&
            !msgVal->IsNullOrUndefined() && msgVal->ToString(context).ToLocal(&strVal)) {
            message = ArgConverter::ConvertToString(strVal);
        } else if (x->ToString(context).ToLocal(&strVal)) {
            message = ArgConverter::ConvertToString(strVal);
        }
    } else if (x->ToString(context).ToLocal(&strVal)) {
        message = ArgConverter::ConvertToString(strVal);
    }

    // The returned value is a real JS Error so `throw interop.escapeException(x)`
    // behaves like a normal throw in pure-JS paths.
    auto errObj = Exception::Error(ArgConverter::ConvertToV8String(isolate, message))
                          .As<Object>();

    // Copy stack from x when it is an Error carrying one.
    string stack;
    if (xIsObject) {
        Local<Value> stackVal;
        if (x.As<Object>()
                    ->Get(context, V8StringConstants::GetStack(isolate))
                    .ToLocal(&stackVal) &&
            stackVal->IsString()) {
            stack = ArgConverter::ConvertToString(stackVal.As<String>());
            errObj->Set(context, V8StringConstants::GetStack(isolate), stackVal)
                    .FromMaybe(false);
        }
    }

    // Build the branded payload: the original Java throwable when x carries
    // one, otherwise synthesis info (name/message/stack).
    auto payload = Object::New(isolate);
    Local<Value> nativeExc = GetWrappedJavaThrowable(context, x);
    if (!nativeExc.IsEmpty()) {
        payload->Set(context, V8StringConstants::GetNativeException(isolate), nativeExc)
                .FromMaybe(false);
    } else {
        string name = "Error";
        if (xIsObject) {
            Local<Value> nameVal;
            if (x.As<Object>()
                        ->Get(context, ArgConverter::ConvertToV8String(isolate, "name"))
                        .ToLocal(&nameVal) &&
                nameVal->IsString()) {
                name = ArgConverter::ConvertToString(nameVal.As<String>());
            }
        }
        payload->Set(context, ArgConverter::ConvertToV8String(isolate, "name"),
                     ArgConverter::ConvertToV8String(isolate, name))
                .FromMaybe(false);
        payload->Set(context, ArgConverter::ConvertToV8String(isolate, "message"),
                     ArgConverter::ConvertToV8String(isolate, message))
                .FromMaybe(false);
        payload->Set(context, V8StringConstants::GetStack(isolate),
                     ArgConverter::ConvertToV8String(isolate, stack))
                .FromMaybe(false);
    }

    errObj->SetPrivate(context, brand, payload).FromMaybe(false);

    info.GetReturnValue().Set(errObj);
}

void Interop::Init(Local<Context> context) {
    auto isolate = context->GetIsolate();
    auto global = context->Global();

    auto interop = Object::New(isolate);

    Local<Function> escapeException;
    if (!Function::New(context, EscapeExceptionCallback).ToLocal(&escapeException)) {
        throw NativeScriptException("Interop::Init: failed to create escapeException");
    }

    if (!interop->Set(context, ArgConverter::ConvertToV8String(isolate, "escapeException"),
                      escapeException)
                 .FromMaybe(false) ||
        !global->Set(context, ArgConverter::ConvertToV8String(isolate, "interop"), interop)
                 .FromMaybe(false)) {
        throw NativeScriptException("Interop::Init: failed to install the interop global");
    }
}

jthrowable Interop::ExtractEscapedJavaException(JEnv& env,
                                                const Local<Object>& errObj) {
    auto isolate = Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    if (context.IsEmpty()) {
        return nullptr;
    }

    Local<Private> brand = GetBrand(isolate);
    Local<Value> payload;
    if (!errObj->HasPrivate(context, brand).FromMaybe(false) ||
        !errObj->GetPrivate(context, brand).ToLocal(&payload) ||
        !payload->IsObject()) {
        return nullptr;
    }

    Local<Value> nativeExc;
    if (!payload.As<Object>()
                 ->Get(context, V8StringConstants::GetNativeException(isolate))
                 .ToLocal(&nativeExc) ||
        nativeExc.IsEmpty() || !nativeExc->IsObject()) {
        return nullptr;
    }

    auto objectManager = Runtime::GetObjectManager(isolate);
    auto javaObj = objectManager->GetJavaObjectByJsObject(nativeExc.As<Object>());
    if (javaObj.IsNull()) {
        return nullptr;
    }

    JniLocalRef objClass(env.GetObjectClass(javaObj));
    jclass throwableClass = env.FindClass("java/lang/Throwable");
    if (env.IsAssignableFrom(objClass, throwableClass) != JNI_TRUE) {
        return nullptr;
    }

    return static_cast<jthrowable>(env.NewLocalRef(javaObj));
}

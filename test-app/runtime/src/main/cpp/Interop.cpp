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

    // The freshly-created Error's own stack IS the escapeException() call
    // site - capture it before the origin stack overwrites it below. It is
    // the only stack available for non-Error values, and often the more
    // useful frame when debugging where an error was forwarded from.
    string escapeSiteStack;
    {
        Local<Value> ownStack;
        if (errObj->Get(context, V8StringConstants::GetStack(isolate)).ToLocal(&ownStack) &&
            ownStack->IsString()) {
            escapeSiteStack = ArgConverter::ConvertToString(ownStack.As<String>());
        }
    }

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

    // Build the branded payload: name/message and both stacks always (so the
    // boundary can carry the JS journey over to Java), plus the original Java
    // throwable when x carries one.
    auto payload = Object::New(isolate);
    payload->Set(context, ArgConverter::ConvertToV8String(isolate, "name"),
                 ArgConverter::ConvertToV8String(isolate, name))
            .FromMaybe(false);
    payload->Set(context, ArgConverter::ConvertToV8String(isolate, "message"),
                 ArgConverter::ConvertToV8String(isolate, message))
            .FromMaybe(false);
    payload->Set(context, V8StringConstants::GetStack(isolate),
                 ArgConverter::ConvertToV8String(isolate, stack))
            .FromMaybe(false);
    payload->Set(context, ArgConverter::ConvertToV8String(isolate, "escapeSiteStack"),
                 ArgConverter::ConvertToV8String(isolate, escapeSiteStack))
            .FromMaybe(false);
    Local<Value> nativeExc = GetWrappedJavaThrowable(context, x);
    if (!nativeExc.IsEmpty()) {
        payload->Set(context, V8StringConstants::GetNativeException(isolate), nativeExc)
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

static string GetPayloadString(Local<Context> context, Local<Object> payload,
                               Local<v8::String> key) {
    Local<Value> val;
    if (payload->Get(context, key).ToLocal(&val) && val->IsString()) {
        return ArgConverter::ConvertToString(val.As<String>());
    }
    return "";
}

bool Interop::GetEscapedExceptionInfo(JEnv& env, const Local<Object>& errObj,
                                      EscapedExceptionInfo& out) {
    auto isolate = Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    if (context.IsEmpty()) {
        return false;
    }

    Local<Private> brand = GetBrand(isolate);
    Local<Value> payloadVal;
    if (!errObj->HasPrivate(context, brand).FromMaybe(false) ||
        !errObj->GetPrivate(context, brand).ToLocal(&payloadVal) ||
        !payloadVal->IsObject()) {
        return false;
    }
    auto payload = payloadVal.As<Object>();

    out.branded = true;
    out.name = GetPayloadString(context, payload,
                                ArgConverter::ConvertToV8String(isolate, "name"));
    out.message = GetPayloadString(context, payload,
                                   ArgConverter::ConvertToV8String(isolate, "message"));
    out.stack = GetPayloadString(context, payload, V8StringConstants::GetStack(isolate));
    out.escapeSiteStack = GetPayloadString(
            context, payload, ArgConverter::ConvertToV8String(isolate, "escapeSiteStack"));

    Local<Value> nativeExc;
    if (payload->Get(context, V8StringConstants::GetNativeException(isolate))
                 .ToLocal(&nativeExc) &&
        !nativeExc.IsEmpty() && nativeExc->IsObject()) {
        auto objectManager = Runtime::GetObjectManager(isolate);
        auto javaObj = objectManager->GetJavaObjectByJsObject(nativeExc.As<Object>());
        if (!javaObj.IsNull()) {
            JniLocalRef objClass(env.GetObjectClass(javaObj));
            jclass throwableClass = env.FindClass("java/lang/Throwable");
            if (env.IsAssignableFrom(objClass, throwableClass) == JNI_TRUE) {
                out.original = static_cast<jthrowable>(env.NewLocalRef(javaObj));
            }
        }
    }

    return true;
}

/*
 * com.tns.JavaScriptStackTrace bridge. JEnv::FindClass returns cached global
 * refs, so the jclass/jmethodIDs are safe to keep in statics (resolved once,
 * on the first escape that needs them).
 */
static jclass GetCarrierClass(JEnv& env) {
    static jclass clazz = env.FindClass("com/tns/JavaScriptStackTrace");
    return clazz;
}

void Interop::AttachJavaScriptStackTrace(JEnv& env, jthrowable target,
                                         const EscapedExceptionInfo& info) {
    if (target == nullptr) {
        return;
    }
    jclass carrierClass = GetCarrierClass(env);
    static jmethodID attachMethod = env.GetStaticMethodID(
            carrierClass, "attach",
            "(Ljava/lang/Throwable;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

    string header = info.name.empty()
                            ? info.message
                            : (info.message.empty() ? info.name
                                                    : info.name + ": " + info.message);
    JniLocalRef headerRef(env.NewStringUTF(header.c_str()));
    JniLocalRef stackRef(env.NewStringUTF(info.stack.c_str()));
    JniLocalRef escapeRef(env.NewStringUTF(info.escapeSiteStack.c_str()));
    env.CallStaticVoidMethod(carrierClass, attachMethod, target, (jstring) headerRef,
                             (jstring) stackRef, (jstring) escapeRef);
}

void Interop::ApplyJavaScriptFrames(JEnv& env, jthrowable target,
                                    const EscapedExceptionInfo& info) {
    if (target == nullptr) {
        return;
    }
    jclass carrierClass = GetCarrierClass(env);
    static jmethodID applyMethod = env.GetStaticMethodID(
            carrierClass, "applyFrames",
            "(Ljava/lang/Throwable;Ljava/lang/String;Ljava/lang/String;)V");

    JniLocalRef stackRef(env.NewStringUTF(info.stack.c_str()));
    JniLocalRef escapeRef(env.NewStringUTF(info.escapeSiteStack.c_str()));
    env.CallStaticVoidMethod(carrierClass, applyMethod, target, (jstring) stackRef,
                             (jstring) escapeRef);
}

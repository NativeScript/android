#include "Util.h"
#include "NativeScriptException.h"
#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "NativeScriptAssert.h"
#include "V8StringConstants.h"
#include "Runtime.h"
#include <sstream>

using namespace std;
using namespace tns;
using namespace v8;

NativeScriptException::NativeScriptException(JEnv& env)
    :
    m_javascriptException(nullptr) {
    m_javaException = JniLocalRef(env.ExceptionOccurred());
    env.ExceptionClear();
}

NativeScriptException::NativeScriptException(const string& message)
    :
    m_javascriptException(nullptr), m_javaException(JniLocalRef()), m_message(message) {
}

NativeScriptException::NativeScriptException(const string& message, const string& stackTrace)
    :
    m_javascriptException(nullptr), m_javaException(JniLocalRef()), m_message(message), m_stackTrace(stackTrace) {
}

NativeScriptException::NativeScriptException(TryCatch& tc, const string& message)
    :
    m_javaException(JniLocalRef()) {
    auto isolate = Isolate::GetCurrent();
    m_javascriptException = new Persistent<Value>(isolate, tc.Exception());
    auto ex = tc.Exception();
    m_message = GetErrorMessage(tc.Message(), ex, message);
    m_stackTrace = GetErrorStackTrace(tc.Message()->GetStackTrace());
    m_fullMessage = GetFullMessage(tc, m_message);
    tc.Reset();
}

void NativeScriptException::ReThrowToV8() {
    auto isolate = Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    Local<Value> errObj;

    if (m_javascriptException != nullptr) {
        errObj = Local<Value>::New(isolate, *m_javascriptException);
        if (errObj->IsObject()) {
            if (!m_fullMessage.empty()) {
                errObj.As<Object>()->Set(context, ArgConverter::ConvertToV8String(isolate, "fullMessage"), ArgConverter::ConvertToV8String(isolate, m_fullMessage));
            } else if (!m_message.empty()) {
                errObj.As<Object>()->Set(context, ArgConverter::ConvertToV8String(isolate, "fullMessage"), ArgConverter::ConvertToV8String(isolate, m_message));
            }
        }
    } else if (!m_fullMessage.empty()) {
        errObj = Exception::Error(ArgConverter::ConvertToV8String(isolate, m_fullMessage));
    } else if (!m_message.empty()) {
        errObj = Exception::Error(ArgConverter::ConvertToV8String(isolate, m_message));
    } else if (!m_javaException.IsNull()) {
        errObj = WrapJavaToJsException();
    } else {
        errObj = Exception::Error(ArgConverter::ConvertToV8String(isolate, "No javascript exception or message provided."));
    }

    isolate->ThrowException(errObj);
}

void NativeScriptException::ReThrowToJava() {
    jthrowable ex = nullptr;
    JEnv env;

    auto isolate = Isolate::GetCurrent();


    if (!m_javaException.IsNull()) {
        std::string excClassName{env.GetClassName((jobject) m_javaException)};
        if (excClassName == "com/tns/NativeScriptException") {
            ex = m_javaException;
        } else {
            JniLocalRef msg(env.NewStringUTF("Java Error!"));
            ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID, (jstring) msg, (jobject) m_javaException));
        }
    } else if (m_javascriptException != nullptr) {
        /*
         * If exception is an object, then check if it has 'nativeException' property and it if does then use it.
         * If the found 'nativeException' is of type different than com.tns.NativeScript then we should wrap it and keep
         * the JavaScript callstack as a message.
         * Otherwise create we have to create new exception object.
         */
        auto isolate = Isolate::GetCurrent();
        auto errObj = Local<Value>::New(isolate, *m_javascriptException);
        if (errObj->IsObject()) {
            auto exObj = TryGetJavaThrowableObject(env, errObj.As<Object>());
            ex = (jthrowable) exObj.Move();
        }

        JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
        JniLocalRef stackTrace(env.NewStringUTF(m_stackTrace.c_str()));

        if (ex == nullptr) {
            ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID, (jstring) msg, (jstring)stackTrace, reinterpret_cast<jlong>(m_javascriptException)));
        } else {
            std::string excClassName{env.GetClassName(ex)};
            if (excClassName != "com/tns/NativeScriptException") {
                ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_THROWABLE_WITH_STACK_CTOR_ID, (jstring) msg, (jstring)stackTrace, ex));
            }
        }
    } else if (!m_message.empty()) {
        JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
        JniLocalRef stackTrace(env.NewStringUTF(m_stackTrace.c_str()));
        ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID, (jstring) msg, (jstring)stackTrace, (jlong) 0));
    } else {
        JniLocalRef msg(env.NewStringUTF("No java exception or message provided."));
        ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID, (jstring) msg, (jstring) nullptr, (jlong) 0));
    }
    env.Throw(ex);
}

void NativeScriptException::Init() {
    JEnv env;

    RUNTIME_CLASS = env.FindClass("com/tns/Runtime");
    assert(RUNTIME_CLASS != nullptr);

    THROWABLE_CLASS = env.FindClass("java/lang/Throwable");
    assert(THROWABLE_CLASS != nullptr);

    NATIVESCRIPTEXCEPTION_CLASS = env.FindClass("com/tns/NativeScriptException");
    assert(NATIVESCRIPTEXCEPTION_CLASS != nullptr);

    NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID = env.GetMethodID(NATIVESCRIPTEXCEPTION_CLASS, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
    assert(NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID != nullptr);

    NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID = env.GetMethodID(NATIVESCRIPTEXCEPTION_CLASS, "<init>", "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    assert(NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID != nullptr);

    NATIVESCRIPTEXCEPTION_THROWABLE_WITH_STACK_CTOR_ID = env.GetMethodID(NATIVESCRIPTEXCEPTION_CLASS, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Throwable;)V");
    assert(NATIVESCRIPTEXCEPTION_THROWABLE_WITH_STACK_CTOR_ID != nullptr);

    NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID = env.GetStaticMethodID(NATIVESCRIPTEXCEPTION_CLASS, "getStackTraceAsString", "(Ljava/lang/Throwable;)Ljava/lang/String;");
    assert(NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID != nullptr);

    NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID = env.GetStaticMethodID(NATIVESCRIPTEXCEPTION_CLASS, "getMessage", "(Ljava/lang/Throwable;)Ljava/lang/String;");
    assert(NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID != nullptr);
}

// ON V8 UNCAUGHT EXCEPTION
void NativeScriptException::OnUncaughtError(Local<Message> message, Local<Value> error) {
    string errorMessage = GetErrorMessage(message, error);
    string stackTrace = GetErrorStackTrace(message->GetStackTrace());

    NativeScriptException e(errorMessage, stackTrace);
    e.ReThrowToJava();
}

void NativeScriptException::CallJsFuncWithErr(Local<Value> errObj, jboolean isDiscarded) {
    auto isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    auto context = isolate->GetCurrentContext();
    auto globalHandle = context->Global();

    Local<Value> handler;
    if (isDiscarded) {
        globalHandle->Get(context, V8StringConstants::GetDiscardedError(isolate)).ToLocal(&handler);
    } else {
        globalHandle->Get(context, V8StringConstants::GetUncaughtError(isolate)).ToLocal(&handler);
    }
    auto isEmpty = handler.IsEmpty();
    auto isFunction = handler->IsFunction();

    if (!isEmpty && isFunction) {
        auto thiz = Object::New(isolate);
        auto func = handler.As<Function>();

        func->Call(context, thiz, 1, &errObj);
    }
}

Local<Value> NativeScriptException::WrapJavaToJsException() {
    Local<Value> errObj;

    JEnv env;

    auto isolate = Isolate::GetCurrent();

    string excClassName{env.GetClassName((jobject) m_javaException)};
    if (excClassName == "com/tns/NativeScriptException") {
        jfieldID fieldID = env.GetFieldID(env.GetObjectClass(m_javaException), "jsValueAddress", "J");
        jlong addr = env.GetLongField(m_javaException, fieldID);

        if (addr != 0) {
            auto pv = (Persistent<Value>*) addr;
            errObj = Local<Value>::New(isolate, *pv);
            pv->Reset();
        } else {
            errObj = GetJavaExceptionFromEnv(m_javaException, env);
        }
    } else {
        errObj = GetJavaExceptionFromEnv(m_javaException, env);
    }

    return errObj;
}

Local<Value> NativeScriptException::GetJavaExceptionFromEnv(const JniLocalRef& exc, JEnv& env) {
    auto errMsg = GetExceptionMessage(env, exc);
    auto stackTrace = GetExceptionStackTrace(env, exc);
    DEBUG_WRITE("Error during java interop errorMessage: %s\n stackTrace:\n %s", errMsg.c_str(), stackTrace.c_str());

    auto isolate = Isolate::GetCurrent();
    auto objectManager = Runtime::GetObjectManager(isolate);

    auto msg = ArgConverter::ConvertToV8String(isolate, errMsg);
    auto errObj = Exception::Error(msg).As<Object>();

    jint javaObjectID = objectManager->GetOrCreateObjectId((jobject) exc);
    auto nativeExceptionObject = objectManager->GetJsObjectByJavaObject(javaObjectID);

    if (nativeExceptionObject.IsEmpty()) {
        string className{env.GetClassName((jobject) exc)};
        nativeExceptionObject = objectManager->CreateJSWrapper(javaObjectID, className);
    }

    auto context = isolate->GetCurrentContext();
    errObj->Set(context, V8StringConstants::GetNativeException(isolate), nativeExceptionObject);

    string jsStackTraceMessage = GetErrorStackTrace(Exception::GetStackTrace(errObj));
    errObj->Set(context, V8StringConstants::GetStack(isolate), ArgConverter::ConvertToV8String(isolate, jsStackTraceMessage));
    errObj->Set(context, V8StringConstants::GetStackTrace(isolate), ArgConverter::ConvertToV8String(isolate, jsStackTraceMessage + stackTrace));

    return errObj;
}

string NativeScriptException::GetFullMessage(const TryCatch& tc, const string& jsExceptionMessage) {
    auto ex = tc.Exception();

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();

    auto message = tc.Message();

    stringstream ss;
    ss << jsExceptionMessage;

    //get script name
    auto scriptResName = message->GetScriptResourceName();

    //get stack trace
    string stackTraceMessage = GetErrorStackTrace(message->GetStackTrace());

    if (!scriptResName.IsEmpty() && scriptResName->IsString()) {
        ss << endl <<"File: (" << ArgConverter::ConvertToString(scriptResName.As<String>());
    } else {
        ss << endl <<"File: (<unknown>";
    }
    ss << ":" << message->GetLineNumber(context).ToChecked() << ":" << message->GetStartColumn() << ")" << endl << endl;
    ss << "StackTrace: " << endl << stackTraceMessage << endl;

    string loggedMessage = ss.str();

    PrintErrorMessage(loggedMessage);

    if (!tc.CanContinue()) {
        stringstream errM;
        errM << endl << "An uncaught error has occurred and V8's TryCatch block CAN'T be continued. ";
        loggedMessage = errM.str() + loggedMessage;
    }

    return loggedMessage;
}

JniLocalRef NativeScriptException::TryGetJavaThrowableObject(JEnv& env, const Local<Object>& jsObj) {
    JniLocalRef javaThrowableObject;

    auto isolate = Isolate::GetCurrent();
    auto objectManager = Runtime::GetObjectManager(isolate);

    auto javaObj = objectManager->GetJavaObjectByJsObject(jsObj);
    JniLocalRef objClass;

    if (!javaObj.IsNull()) {
        objClass = JniLocalRef(env.GetObjectClass(javaObj));
    } else {
        auto isolate = jsObj->GetIsolate();
        auto context = isolate->GetCurrentContext();
        Local<Value> nativeEx;
        jsObj->Get(context, V8StringConstants::GetNativeException(isolate)).ToLocal(&nativeEx);
        if (!nativeEx.IsEmpty() && nativeEx->IsObject()) {
            javaObj = objectManager->GetJavaObjectByJsObject(nativeEx.As<Object>());
            objClass = JniLocalRef(env.GetObjectClass(javaObj));
        }
    }

    auto isThrowable = !objClass.IsNull() ? env.IsAssignableFrom(objClass, THROWABLE_CLASS) : JNI_FALSE;

    if (isThrowable == JNI_TRUE) {
        javaThrowableObject = JniLocalRef(env.NewLocalRef(javaObj));
    }

    return javaThrowableObject;
}

void NativeScriptException::PrintErrorMessage(const string& errorMessage) {
    // split the message by new lines to workaround the LogCat's maximum characters in a single message
    stringstream ss(errorMessage);
    string line;
    while (getline(ss, line, '\n')) {
        // TODO: Log in the V8's Console as well?
        DEBUG_WRITE("%s", line.c_str());
    }
}

string NativeScriptException::GetErrorMessage(const Local<Message>& message, Local<Value>& error, const string& prependMessage) {

    Local<String> message_text_string = message->Get();
    auto mes = ArgConverter::ConvertToString(message_text_string);

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();

    //get whole error message from previous stack
    stringstream ss;

    if (prependMessage != "") {
        ss << prependMessage << endl;
    }

    string errMessage;
    bool hasFullErrorMessage = false;
    auto v8FullMessage = ArgConverter::ConvertToV8String(isolate, "fullMessage");
    if (error->IsObject() && error.As<Object>()->Has(context, v8FullMessage).ToChecked()) {
        hasFullErrorMessage = true;
        Local<Value> errMsgVal;
        error.As<Object>()->Get(context, v8FullMessage).ToLocal(&errMsgVal);
        if (!errMsgVal.IsEmpty()) {
            errMessage = ArgConverter::ConvertToString(errMsgVal.As<String>());
        } else {
            errMessage = "";
        }
        ss << errMessage;
    }

    auto str = error->ToDetailString(context);
    if (!str.IsEmpty()) {
        String::Utf8Value utfError(isolate, str.FromMaybe(Local<String>()));
        if(hasFullErrorMessage) {
            ss << endl;
        }
        ss << *utfError;
    }

    return ss.str();
}

string NativeScriptException::GetErrorStackTrace(const Local<StackTrace>& stackTrace) {
    stringstream ss;

    auto isolate = Isolate::GetCurrent();
    HandleScope handleScope(isolate);

    int frameCount = stackTrace->GetFrameCount();

    for (int i = 0; i < frameCount; i++) {
        auto frame = stackTrace->GetFrame(isolate, i);
        auto funcName = ArgConverter::ConvertToString(frame->GetFunctionName());
        auto srcName = ArgConverter::ConvertToString(frame->GetScriptName());
        auto lineNumber = frame->GetLineNumber();
        auto column = frame->GetColumn();

        auto startString = i == 0 ? "" : "\t";

        ss << startString << (i > 0 ? "at " : "") << funcName.c_str() << "(" << srcName.c_str() << ":" << lineNumber << ":" << column << ")" << endl;
    }

    return ss.str();
}

string NativeScriptException::GetExceptionMessage(JEnv& env, jthrowable exception) {
    string errMsg;
    JniLocalRef msg(env.CallStaticObjectMethod(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID, exception));

    const char* msgStr = env.GetStringUTFChars(msg, nullptr);

    errMsg.append(msgStr);

    env.ReleaseStringUTFChars(msg, msgStr);

    return errMsg;
}

string NativeScriptException::GetExceptionStackTrace(JEnv& env, jthrowable exception) {
    string errStackTrace;
    JniLocalRef msg(env.CallStaticObjectMethod(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID, exception));

    const char* msgStr = env.GetStringUTFChars(msg, nullptr);

    errStackTrace.append(msgStr);

    env.ReleaseStringUTFChars(msg, msgStr);

    return errStackTrace;
}

jclass NativeScriptException::RUNTIME_CLASS = nullptr;
jclass NativeScriptException::THROWABLE_CLASS = nullptr;
jclass NativeScriptException::NATIVESCRIPTEXCEPTION_CLASS = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_THROWABLE_WITH_STACK_CTOR_ID = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID = nullptr;
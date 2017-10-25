#include "CallbackHandlers.h"
#include "NativeScriptAssert.h"
#include "MetadataNode.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "JsArgConverter.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "v8-profiler.h"
#include "NativeScriptException.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include "MethodCache.h"
#include "SimpleProfiler.h"
#include "Runtime.h"

using namespace v8;
using namespace std;
using namespace tns;

void CallbackHandlers::Init(Isolate* isolate) {
    JEnv env;

    JAVA_LANG_STRING = env.FindClass("java/lang/String");
    assert(JAVA_LANG_STRING != nullptr);

    RUNTIME_CLASS = env.FindClass("com/tns/Runtime");
    assert(RUNTIME_CLASS != nullptr);

    RESOLVE_CLASS_METHOD_ID = env.GetMethodID(RUNTIME_CLASS, "resolveClass",
                              "(Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;Z)Ljava/lang/Class;");
    assert(RESOLVE_CLASS_METHOD_ID != nullptr);

    CURRENT_OBJECTID_FIELD_ID = env.GetFieldID(RUNTIME_CLASS, "currentObjectId", "I");
    assert(CURRENT_OBJECTID_FIELD_ID != nullptr);

    MAKE_INSTANCE_STRONG_ID = env.GetMethodID(RUNTIME_CLASS, "makeInstanceStrong",
                              "(Ljava/lang/Object;I)V");
    assert(MAKE_INSTANCE_STRONG_ID != nullptr);

    GET_TYPE_METADATA = env.GetStaticMethodID(RUNTIME_CLASS, "getTypeMetadata",
                        "(Ljava/lang/String;I)[Ljava/lang/String;");
    assert(GET_TYPE_METADATA != nullptr);

    ENABLE_VERBOSE_LOGGING_METHOD_ID = env.GetMethodID(RUNTIME_CLASS, "enableVerboseLogging",
                                       "()V");
    assert(ENABLE_VERBOSE_LOGGING_METHOD_ID != nullptr);

    DISABLE_VERBOSE_LOGGING_METHOD_ID = env.GetMethodID(RUNTIME_CLASS, "disableVerboseLogging",
                                        "()V");
    assert(ENABLE_VERBOSE_LOGGING_METHOD_ID != nullptr);

    INIT_WORKER_METHOD_ID = env.GetStaticMethodID(RUNTIME_CLASS, "initWorker", "(Ljava/lang/String;Ljava/lang/String;I)V");

    assert(INIT_WORKER_METHOD_ID != nullptr);

    Local<Object> json = isolate->GetCurrentContext()->Global()->Get(String::NewFromUtf8(isolate, "JSON"))->ToObject();
    Local<Function> stringify = json->Get(String::NewFromUtf8(isolate, "stringify")).As<Function>();

    auto persistentStringify = new Persistent<Function>(isolate, stringify);

    isolateToJsonStringify.insert({isolate, persistentStringify});

    MetadataNode::Init(isolate);

    MethodCache::Init();
}

bool CallbackHandlers::RegisterInstance(Isolate* isolate, const Local<Object>& jsObject,
                                        const std::string& fullClassName,
                                        const ArgsWrapper& argWrapper,
                                        const Local<Object>& implementationObject,
                                        bool isInterface) {
    bool success;

    DEBUG_WRITE("RegisterInstance called for '%s'", fullClassName.c_str());

    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    JEnv env;

    jclass generatedJavaClass = ResolveClass(isolate, fullClassName, implementationObject,
                                isInterface);

    int javaObjectID = objectManager->GenerateNewObjectID();

    objectManager->Link(jsObject, javaObjectID, nullptr);

    // resolve constructor
    auto mi = MethodCache::ResolveConstructorSignature(argWrapper, fullClassName,
              generatedJavaClass, isInterface);

    // while the "instance" is being created, if an exception is thrown during the construction
    // this scope will guarantee the "javaObjectID" will be set to -1 and won't have an invalid value
    jobject instance;
    {
        JavaObjectIdScope objIdScope(env, CURRENT_OBJECTID_FIELD_ID, runtime->GetJavaRuntime(),
                                     javaObjectID);

        if (argWrapper.type == ArgType::Interface) {
            instance = env.NewObject(generatedJavaClass, mi.mid);
        } else {
            // resolve arguments before passing them on to the constructor
            JsArgConverter argConverter(argWrapper.args, mi.signature);
            auto ctorArgs = argConverter.ToArgs();

            instance = env.NewObjectA(generatedJavaClass, mi.mid, ctorArgs);
        }
    }

    env.CallVoidMethod(runtime->GetJavaRuntime(), MAKE_INSTANCE_STRONG_ID, instance, javaObjectID);

    AdjustAmountOfExternalAllocatedMemory(env, isolate);

    JniLocalRef localInstance(instance);
    success = !localInstance.IsNull();

    if (success) {
        jclass instanceClass = env.FindClass(fullClassName);
        objectManager->SetJavaClass(jsObject, instanceClass);
    } else {
        DEBUG_WRITE_FORCE("RegisterInstance failed with null new instance class: %s", fullClassName.c_str());
    }

    return success;
}

jclass CallbackHandlers::ResolveClass(Isolate* isolate, const string& fullClassname,
                                      const Local<Object>& implementationObject, bool isInterface) {
    JEnv env;
    jclass globalRefToGeneratedClass = env.CheckForClassInCache(fullClassname);

    if (globalRefToGeneratedClass == nullptr) {

        // get needed arguments in order to load binding
        JniLocalRef javaFullClassName(env.NewStringUTF(fullClassname.c_str()));

        jobjectArray methodOverrides = GetMethodOverrides(env, implementationObject);

        jobjectArray implementedInterfaces = GetImplementedInterfaces(env, implementationObject);

        auto runtime = Runtime::GetRuntime(isolate);

        // create or load generated binding (java class)
        jclass generatedClass = (jclass)env.CallObjectMethod(runtime->GetJavaRuntime(),
                                RESOLVE_CLASS_METHOD_ID,
                                (jstring) javaFullClassName,
                                methodOverrides,
                                implementedInterfaces,
                                isInterface);

        globalRefToGeneratedClass = env.InsertClassIntoCache(fullClassname, generatedClass);

        env.DeleteGlobalRef(methodOverrides);
        env.DeleteGlobalRef(implementedInterfaces);
    }

    return globalRefToGeneratedClass;
}

// Called by ExtendMethodCallback when extending a class
string CallbackHandlers::ResolveClassName(Isolate* isolate, jclass& clazz) {
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();
    auto className = objectManager->GetClassName(clazz);
    return className;
}

Local<Value> CallbackHandlers::GetArrayElement(Isolate* isolate, const Local<Object>& array,
        uint32_t index, const string& arraySignature) {
    return arrayElementAccessor.GetArrayElement(isolate, array, index, arraySignature);
}

void CallbackHandlers::SetArrayElement(Isolate* isolate, const Local<Object>& array, uint32_t index,
                                       const string& arraySignature, Local<Value>& value) {

    arrayElementAccessor.SetArrayElement(isolate, array, index, arraySignature, value);
}

Local<Value> CallbackHandlers::GetJavaField(Isolate* isolate, const Local<Object>& caller,
        FieldCallbackData* fieldData) {
    return fieldAccessor.GetJavaField(isolate, caller, fieldData);
}

void CallbackHandlers::SetJavaField(Isolate* isolate, const Local<Object>& target,
                                    const Local<Value>& value, FieldCallbackData* fieldData) {
    fieldAccessor.SetJavaField(isolate, target, value, fieldData);
}

void CallbackHandlers::CallJavaMethod(const Local<Object>& caller, const string& className,
                                      const string& methodName, MetadataEntry* entry, bool isStatic,
                                      bool isSuper,
                                      const v8::FunctionCallbackInfo<v8::Value>& args) {
    SET_PROFILER_FRAME();

    JEnv env;

    jclass clazz;
    jmethodID mid;
    string* sig = nullptr;
    string* returnType = nullptr;
    auto retType = MethodReturnType::Unknown;
    MethodCache::CacheMethodInfo mi;

    if ((entry != nullptr) && entry->isResolved) {
        isStatic = entry->isStatic;

        if (entry->memberId == nullptr) {
            clazz = env.FindClass(className);
            if (clazz == nullptr) {
                MetadataNode* callerNode = MetadataNode::GetNodeFromHandle(caller);
                const string callerClassName = callerNode->GetName();

                DEBUG_WRITE("Cannot resolve class: %s while calling method: %s callerClassName: %s",
                            className.c_str(), methodName.c_str(), callerClassName.c_str());
                clazz = env.FindClass(callerClassName);
                if (clazz == nullptr) {
                    //todo: plamen5kov: throw exception here
                    DEBUG_WRITE("Cannot resolve caller's class name: %s", callerClassName.c_str());
                    return;
                }

                entry->memberId = isStatic ?
                                  env.GetStaticMethodID(clazz, methodName, entry->sig) :
                                  env.GetMethodID(clazz, methodName, entry->sig);

                if (entry->memberId == nullptr) {
                    //todo: plamen5kov: throw exception here
                    DEBUG_WRITE("Cannot resolve a method %s on caller class: %s",
                                methodName.c_str(), callerClassName.c_str());
                    return;
                }
            } else {
                entry->memberId = isStatic ?
                                  env.GetStaticMethodID(clazz, methodName, entry->sig) :
                                  env.GetMethodID(clazz, methodName, entry->sig);

                if (entry->memberId == nullptr) {
                    //todo: plamen5kov: throw exception here
                    DEBUG_WRITE("Cannot resolve a method %s on class: %s", methodName.c_str(),
                                className.c_str());
                    return;
                }
            }
            entry->clazz = clazz;
        }

        mid = reinterpret_cast<jmethodID>(entry->memberId);
        clazz = entry->clazz;
        sig = &entry->sig;
        returnType = &entry->returnType;
        retType = entry->retType;
    } else {
        DEBUG_WRITE("Resolving method: %s on className %s", methodName.c_str(), className.c_str());

        clazz = env.FindClass(className);
        if (clazz != nullptr) {
            mi = MethodCache::ResolveMethodSignature(className, methodName, args, isStatic);
            if (mi.mid == nullptr) {
                DEBUG_WRITE("Cannot resolve class=%s, method=%s, isStatic=%d, isSuper=%d",
                            className.c_str(), methodName.c_str(), isStatic, isSuper);
                return;
            }
        } else {
            MetadataNode* callerNode = MetadataNode::GetNodeFromHandle(caller);
            const string callerClassName = callerNode->GetName();
            DEBUG_WRITE("Resolving method on caller class: %s.%s on className %s",
                        callerClassName.c_str(), methodName.c_str(), className.c_str());
            mi = MethodCache::ResolveMethodSignature(callerClassName, methodName, args, isStatic);
            if (mi.mid == nullptr) {
                DEBUG_WRITE(
                    "Cannot resolve class=%s, method=%s, isStatic=%d, isSuper=%d, callerClass=%s",
                    className.c_str(), methodName.c_str(), isStatic, isSuper,
                    callerClassName.c_str());
                return;
            }
        }

        clazz = mi.clazz;
        mid = mi.mid;
        sig = &mi.signature;
        returnType = &mi.returnType;
        retType = mi.retType;
    }

    if (!isStatic) {
        DEBUG_WRITE("CallJavaMethod called %s.%s. Instance id: %d, isSuper=%d", className.c_str(),
                    methodName.c_str(), caller.IsEmpty() ? -42 : caller->GetIdentityHash(),
                    isSuper);
    } else {
        DEBUG_WRITE("CallJavaMethod called %s.%s. static method", className.c_str(),
                    methodName.c_str());
    }

    JsArgConverter argConverter(args, false, *sig, entry);

    if (!argConverter.IsValid()) {
        JsArgConverter::Error err = argConverter.GetError();
        throw NativeScriptException(err.msg);
    }

    auto isolate = args.GetIsolate();

    JniLocalRef callerJavaObject;

    jvalue* javaArgs = argConverter.ToArgs();

    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    if (!isStatic) {
        callerJavaObject = objectManager->GetJavaObjectByJsObject(caller);
        if (callerJavaObject.IsNull()) {
            stringstream ss;
            if (args.IsConstructCall()) {
                ss << "No java object found on which to call \"" << methodName << "\" method. It is possible your Javascript object is not linked with the corresponding Java class. Try passing context(this) to the constructor function.";
            } else {
                ss << "Failed calling " << methodName << " on a " << className << " instance. The JavaScript instance no longer has available Java instance counterpart.";
            }
            throw NativeScriptException(ss.str());
        }
    }

    switch (retType) {
    case MethodReturnType::Void: {
        if (isStatic) {
            env.CallStaticVoidMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            env.CallNonvirtualVoidMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            env.CallVoidMethodA(callerJavaObject, mid, javaArgs);
        }
        break;
    }
    case MethodReturnType::Boolean: {
        jboolean result;
        if (isStatic) {
            result = env.CallStaticBooleanMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualBooleanMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallBooleanMethodA(callerJavaObject, mid, javaArgs);
        }
        args.GetReturnValue().Set(result != 0 ? True(isolate) : False(isolate));
        break;
    }
    case MethodReturnType::Byte: {
        jbyte result;
        if (isStatic) {
            result = env.CallStaticByteMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualByteMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallByteMethodA(callerJavaObject, mid, javaArgs);
        }
        args.GetReturnValue().Set(result);
        break;
    }
    case MethodReturnType::Char: {
        jchar result;
        if (isStatic) {
            result = env.CallStaticCharMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualCharMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallCharMethodA(callerJavaObject, mid, javaArgs);
        }

        JniLocalRef str(env.NewString(&result, 1));
        jboolean bol = true;
        const char* resP = env.GetStringUTFChars(str, &bol);
        args.GetReturnValue().Set(ArgConverter::ConvertToV8String(isolate, resP, 1));
        env.ReleaseStringUTFChars(str, resP);
        break;
    }
    case MethodReturnType::Short: {
        jshort result;
        if (isStatic) {
            result = env.CallStaticShortMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualShortMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallShortMethodA(callerJavaObject, mid, javaArgs);
        }
        args.GetReturnValue().Set(result);
        break;
    }
    case MethodReturnType::Int: {
        jint result;
        if (isStatic) {
            result = env.CallStaticIntMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualIntMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallIntMethodA(callerJavaObject, mid, javaArgs);
        }
        args.GetReturnValue().Set(result);
        break;

    }
    case MethodReturnType::Long: {
        jlong result;
        if (isStatic) {
            result = env.CallStaticLongMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualLongMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallLongMethodA(callerJavaObject, mid, javaArgs);
        }
        auto jsLong = ArgConverter::ConvertFromJavaLong(isolate, result);
        args.GetReturnValue().Set(jsLong);
        break;
    }
    case MethodReturnType::Float: {
        jfloat result;
        if (isStatic) {
            result = env.CallStaticFloatMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualFloatMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallFloatMethodA(callerJavaObject, mid, javaArgs);
        }
        args.GetReturnValue().Set((double) result); //TODO: handle float value here correctly.
        break;
    }
    case MethodReturnType::Double: {
        jdouble result;
        if (isStatic) {
            result = env.CallStaticDoubleMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualDoubleMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallDoubleMethodA(callerJavaObject, mid, javaArgs);
        }
        args.GetReturnValue().Set(result);
        break;
    }
    case MethodReturnType::String: {
        jobject result = nullptr;
        bool exceptionOccurred;

        if (isStatic) {
            result = env.CallStaticObjectMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualObjectMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallObjectMethodA(callerJavaObject, mid, javaArgs);
        }

        if (result != nullptr) {
            auto objectResult = ArgConverter::jstringToV8String(isolate, static_cast<jstring>(result));
            args.GetReturnValue().Set(objectResult);
            env.DeleteLocalRef(result);
        } else {
            args.GetReturnValue().Set(Null(isolate));
        }

        break;
    }
    case MethodReturnType::Object: {
        jobject result = nullptr;
        bool exceptionOccurred;

        if (isStatic) {
            result = env.CallStaticObjectMethodA(clazz, mid, javaArgs);
        } else if (isSuper) {
            result = env.CallNonvirtualObjectMethodA(callerJavaObject, clazz, mid, javaArgs);
        } else {
            result = env.CallObjectMethodA(callerJavaObject, mid, javaArgs);
        }

        if (result != nullptr) {
            auto isString = env.IsInstanceOf(result, JAVA_LANG_STRING);

            Local<Value> objectResult;
            if (isString) {
                objectResult = ArgConverter::jstringToV8String(isolate, (jstring) result);
            } else {
                jint javaObjectID = objectManager->GetOrCreateObjectId(result);
                objectResult = objectManager->GetJsObjectByJavaObject(javaObjectID);

                if (objectResult.IsEmpty()) {
                    objectResult = objectManager->CreateJSWrapper(javaObjectID, *returnType,
                                   result);
                }
            }

            args.GetReturnValue().Set(objectResult);
            env.DeleteLocalRef(result);
        } else {
            args.GetReturnValue().Set(Null(isolate));
        }

        break;
    }
    default: {
        assert(false);
        break;
    }
    }

    static uint32_t adjustMemCount = 0;

    if ((++adjustMemCount % 2) == 0) {
        AdjustAmountOfExternalAllocatedMemory(env, isolate);
    }
}

void CallbackHandlers::AdjustAmountOfExternalAllocatedMemory(JEnv& env, Isolate* isolate) {
    auto runtime = Runtime::GetRuntime(isolate);
    runtime->AdjustAmountOfExternalAllocatedMemory();
    runtime->TryCallGC();
}

Local<Object> CallbackHandlers::CreateJSWrapper(Isolate* isolate, jint javaObjectID,
        const string& typeName) {
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    return objectManager->CreateJSWrapper(javaObjectID, typeName);
}


jobjectArray CallbackHandlers::GetImplementedInterfaces(JEnv& env, const Local<Object>& implementationObject) {
    if (implementationObject.IsEmpty()) {
        return CallbackHandlers::GetJavaStringArray(env, 0);
    }

    vector <jstring> interfacesToImplement;
    auto propNames = implementationObject->GetOwnPropertyNames();
    for (int i = 0; i < propNames->Length(); i++) {
        auto name = propNames->Get(i).As<String>();
        auto prop = implementationObject->Get(name);

        bool arrFound = !prop.IsEmpty() && prop->IsArray();

        if (arrFound) {
            v8::String::Utf8Value propName(name);
            std::string arrNameC = std::string(*propName);
            if (arrNameC == "interfaces") {
                auto interfacesArr = prop->ToObject();

                auto isolate = implementationObject->GetIsolate();
                int length = interfacesArr->Get(v8::String::NewFromUtf8(isolate, "length"))->ToObject()->Uint32Value();

                if (length > 0) {
                    for (int i = 0; i < length; i++) {
                        auto element = interfacesArr->Get(i);

                        if (element->IsFunction()) {
                            auto node = MetadataNode::GetTypeMetadataName(isolate, element);

                            node = Util::ReplaceAll(node, std::string("/"), std::string("."));

                            jstring value = env.NewStringUTF(node.c_str());
                            interfacesToImplement.push_back(value);
                        }
                    }
                }
            }
        }
    }

    int interfacesCount = interfacesToImplement.size();

    jobjectArray implementedInterfaces = CallbackHandlers::GetJavaStringArray(env, interfacesCount);
    for (int i = 0; i < interfacesCount; i++) {
        env.SetObjectArrayElement(implementedInterfaces, i, interfacesToImplement[i]);
    }

    for (int i = 0; i < interfacesCount; i++) {
        env.DeleteLocalRef(interfacesToImplement[i]);
    }

    return implementedInterfaces;
}

jobjectArray CallbackHandlers::GetMethodOverrides(JEnv& env, const Local<Object>& implementationObject) {
    if (implementationObject.IsEmpty()) {
        return CallbackHandlers::GetJavaStringArray(env, 0);
    }

    vector <jstring> methodNames;
    auto propNames = implementationObject->GetOwnPropertyNames();
    for (int i = 0; i < propNames->Length(); i++) {
        auto name = propNames->Get(i).As<String>();
        auto method = implementationObject->Get(name);

        bool methodFound = !method.IsEmpty() && method->IsFunction();

        if (methodFound) {
            String::Utf8Value stringValue(name);
            jstring value = env.NewStringUTF(*stringValue);
            methodNames.push_back(value);
        }
    }

    int methodCount = methodNames.size();

    jobjectArray methodOverrides = CallbackHandlers::GetJavaStringArray(env, methodCount);
    for (int i = 0; i < methodCount; i++) {
        env.SetObjectArrayElement(methodOverrides, i, methodNames[i]);
    }

    for (int i = 0; i < methodCount; i++) {
        env.DeleteLocalRef(methodNames[i]);
    }

    return methodOverrides;
}

void CallbackHandlers::LogMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        if ((args.Length() > 0) && args[0]->IsString()) {
            String::Utf8Value message(args[0]->ToString());
            DEBUG_WRITE("%s", *message);
        }
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

void CallbackHandlers::TimeCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto nano = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    double duration = nano.time_since_epoch().count();
    args.GetReturnValue().Set(duration);
}

void CallbackHandlers::DumpReferenceTablesMethodCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    DumpReferenceTablesMethod();
}

void CallbackHandlers::DumpReferenceTablesMethod() {
    try {
        JEnv env;
        jclass vmDbgClass = env.FindClass("dalvik/system/VMDebug");
        if (vmDbgClass != nullptr) {
            jmethodID mid = env.GetStaticMethodID(vmDbgClass, "dumpReferenceTables", "()V");
            if (mid != 0) {
                env.CallStaticVoidMethod(vmDbgClass, mid);
            }
        }
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

void CallbackHandlers::EnableVerboseLoggingMethodCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto runtume = Runtime::GetRuntime(isolate);
        tns::LogEnabled = true;
        JEnv env;
        env.CallVoidMethod(runtume->GetJavaRuntime(), ENABLE_VERBOSE_LOGGING_METHOD_ID);
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

void CallbackHandlers::DisableVerboseLoggingMethodCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto runtume = Runtime::GetRuntime(isolate);
        tns::LogEnabled = false;
        JEnv env;
        env.CallVoidMethod(runtume->GetJavaRuntime(), DISABLE_VERBOSE_LOGGING_METHOD_ID);
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

void CallbackHandlers::ExitMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto msg = ArgConverter::ConvertToString(args[0].As<String>());
    DEBUG_WRITE_FATAL("FORCE EXIT: %s", msg.c_str());
    exit(-1);
}

void CallbackHandlers::CreateGlobalCastFunctions(Isolate* isolate, const Local<ObjectTemplate>& globalTemplate) {
    castFunctions.CreateGlobalCastFunctions(isolate, globalTemplate);
}

vector <string> CallbackHandlers::GetTypeMetadata(const string& name, int index) {
    JEnv env;

    string canonicalName = Util::ConvertFromJniToCanonicalName(name);

    JniLocalRef className(env.NewStringUTF(canonicalName.c_str()));
    jint idx = index;

    JniLocalRef pubApi(
        env.CallStaticObjectMethod(RUNTIME_CLASS, GET_TYPE_METADATA, (jstring) className, idx));

    jsize length = env.GetArrayLength(pubApi);

    assert(length > 0);

    vector <string> result;

    for (jsize i = 0; i < length; i++) {
        JniLocalRef s(env.GetObjectArrayElement(pubApi, i));
        const char* pc = env.GetStringUTFChars(s, nullptr);
        result.push_back(string(pc));
        env.ReleaseStringUTFChars(s, pc);
    }

    return result;
}

Local<Value> CallbackHandlers::CallJSMethod(Isolate* isolate, JNIEnv* _env,
        const Local<Object>& jsObject, const string& methodName,
        jobjectArray args) {
    SET_PROFILER_FRAME();

    JEnv env(_env);
    Local<Value> result;

    auto method = jsObject->Get(ArgConverter::ConvertToV8String(isolate, methodName));

    if (method.IsEmpty() || method->IsUndefined()) {
        stringstream ss;
        ss << "Cannot find method '" << methodName << "' implementation";
        throw NativeScriptException(ss.str());
    } else if (!method->IsFunction()) {
        stringstream ss;
        ss << "Property '" << methodName << "' is not a function";
        throw NativeScriptException(ss.str());
    } else {
        EscapableHandleScope handleScope(isolate);

        auto jsMethod = method.As<Function>();
        auto jsArgs = ArgConverter::ConvertJavaArgsToJsArgs(isolate, args);
        int argc = jsArgs->Length();

        std::vector<Local<Value>> arguments(argc);
        for (int i = 0; i < argc; i++) {
            arguments[i] = jsArgs->Get(i);
        }

        TryCatch tc;
        Local<Value> jsResult;
        {
            SET_PROFILER_FRAME();
            jsResult = jsMethod->Call(jsObject, argc, argc == 0 ? nullptr : arguments.data());
        }

        //TODO: if javaResult is a pure js object create a java object that represents this object in java land

        if (tc.HasCaught()) {
            stringstream ss;
            ss << "Calling js method " << methodName << " failed";
            throw NativeScriptException(tc, ss.str());
        }

        result = handleScope.Escape(jsResult);
    }

    return result;
}

Local<Object> CallbackHandlers::FindClass(Isolate* isolate, const string& className) {
    Local<Object> clazz;
    JEnv env;

    jclass c = env.FindClass(className);
    if (env.ExceptionCheck() == JNI_FALSE) {
        auto runtime = Runtime::GetRuntime(isolate);
        auto objectManager = runtime->GetObjectManager();

        jint javaObjectID = objectManager->GetOrCreateObjectId(c);
        clazz = objectManager->GetJsObjectByJavaObject(javaObjectID);

        if (clazz.IsEmpty()) {
            clazz = objectManager->CreateJSWrapper(javaObjectID, "Ljava/lang/Class;", c);
        }
    }
    return clazz;
}

int CallbackHandlers::GetArrayLength(Isolate* isolate, const Local<Object>& arr) {
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    JEnv env;

    auto javaArr = objectManager->GetJavaObjectByJsObject(arr);

    auto length = env.GetArrayLength(javaArr);

    return length;
}

jobjectArray CallbackHandlers::GetJavaStringArray(JEnv& env, int length) {
    if (length > CallbackHandlers::MAX_JAVA_STRING_ARRAY_LENGTH) {
        stringstream ss;
        ss << "You are trying to override more methods than the limit of " <<  CallbackHandlers::MAX_JAVA_STRING_ARRAY_LENGTH;
        throw NativeScriptException(ss.str());
    }

    JniLocalRef tmpArr(env.NewObjectArray(length, JAVA_LANG_STRING, nullptr));
    return (jobjectArray) env.NewGlobalRef(tmpArr);
}

void CallbackHandlers::NewThreadCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        if (!args.IsConstructCall()) {
            throw NativeScriptException("Worker should be called as a constructor!");
        }

        if (args.Length() > 1 || !args[0]->IsString()) {
            throw NativeScriptException("Worker should be called with one string parameter (name of file to run)!");
        }

        auto thiz = args.This();
        auto isolate = thiz->GetIsolate();

        auto currentExecutingScriptName = StackTrace::CurrentStackTrace(isolate, 1, StackTrace::kScriptName)->GetFrame(0)->GetScriptName();
        auto currentExecutingScriptNameStr = ArgConverter::ConvertToString(currentExecutingScriptName);
        auto lastForwardSlash = currentExecutingScriptNameStr.find_last_of("/");
        auto currentDir = currentExecutingScriptNameStr.substr(0, lastForwardSlash + 1);
        string fileSchema("file://");
        if (currentDir.compare(0, fileSchema.length(), fileSchema) == 0) {
            currentDir = currentDir.substr(fileSchema.length());
        }

        auto workerPath = ArgConverter::ConvertToString(args[0]->ToString(isolate));

        // Will throw if path is invalid or doesn't exist
        ModuleInternal::CheckFileExists(isolate, workerPath, currentDir);

        auto workerId = nextWorkerId++;
        V8SetPrivateValue(isolate, thiz, ArgConverter::ConvertToV8String(isolate, "workerId"), Number::New(isolate, workerId));

        auto persistentWorker = new Persistent<Object>(isolate, thiz);

        id2WorkerMap.insert(make_pair(workerId, persistentWorker));

        DEBUG_WRITE("Called Worker constructor id=%d", workerId);

        JEnv env;
        JniLocalRef filePath(ArgConverter::ConvertToJavaString(args[0]));
        JniLocalRef dirPath(env.NewStringUTF(currentDir.c_str()));

        env.CallStaticVoidMethod(RUNTIME_CLASS, INIT_WORKER_METHOD_ID, (jstring) filePath, (jstring) dirPath, workerId);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c exception!"));
        nsEx.ReThrowToV8();
    }
}

void CallbackHandlers::WorkerObjectPostMessageCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = args.GetIsolate();

    HandleScope scope(isolate);

    try {
        if (args.Length() != 1) {
            isolate->ThrowException(ArgConverter::ConvertToV8String(isolate,
                                    "Failed to execute 'postMessage' on 'Worker': 1 argument required."));
            return;
        }

        auto thiz = args.This(); // Worker instance

        Local<Value> jsId;

        auto maybejsId = V8GetPrivateValue(isolate, thiz,
                                           ArgConverter::ConvertToV8String(isolate, "workerId"),
                                           jsId);

        Local<String> msg = tns::JsonStringifyObject(isolate, args[0])->ToString();
        // get worker's ID that is associated on the other side - in Java
        auto id = jsId->Int32Value();

        JEnv env;
        auto mId = env.GetStaticMethodID(RUNTIME_CLASS, "sendMessageFromMainToWorker",
                                         "(ILjava/lang/String;)V");

        auto jmsg = ArgConverter::ConvertToJavaString(msg);
        JniLocalRef jmsgRef(jmsg);

        env.CallStaticVoidMethod(RUNTIME_CLASS, mId, id, (jstring) jmsgRef);

        DEBUG_WRITE("MAIN: WorkerObjectPostMessageCallback called postMessage on Worker object(id=%d)", id);
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::WorkerGlobalOnMessageCallback(Isolate* isolate, jstring message) {
    auto context = isolate->GetCurrentContext();

    try {
        auto globalObject = context->Global();

        TryCatch tc(isolate);

        auto callback = globalObject->Get(ArgConverter::ConvertToV8String(isolate, "onmessage"));
        auto isEmpty = callback.IsEmpty();
        auto isFunction = callback->IsFunction();

        if (!isEmpty && isFunction) {
            auto msgString = ArgConverter::jstringToV8String(isolate, message).As<String>();
            Local<Value> msg;
            JSON::Parse(isolate, msgString).ToLocal(&msg);

            auto obj = Object::New(isolate);
            obj->DefineOwnProperty(isolate->GetCurrentContext(),
                                   ArgConverter::ConvertToV8String(isolate, "data"), msg,
                                   PropertyAttribute::ReadOnly);
            Local<Value> args1[] = {obj};

            auto func = callback.As<Function>();

            func->Call(Undefined(isolate), 1, args1);
        } else {
            DEBUG_WRITE(
                "WORKER: WorkerGlobalOnMessageCallback couldn't fire a worker's `onmessage` callback because it isn't implemented!");
        }

        if (tc.HasCaught()) {
            // TODO: Pete: Will catch exceptions thrown artificially in postMessage callbacks inside of 'onmessage' implementation
            CallWorkerScopeOnErrorHandle(isolate, tc);
        }
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::WorkerGlobalPostMessageCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = args.GetIsolate();

    HandleScope scope(isolate);

    try {
        TryCatch tc;

        // TODO: Pete: Discuss whether this is the way to go
        if (args.Length() != 1) {
            isolate->ThrowException(ArgConverter::ConvertToV8String(isolate,
                                    "Failed to execute 'postMessage' on WorkerGlobalScope: 1 argument required."));
        }

        if (tc.HasCaught()) {
            CallWorkerScopeOnErrorHandle(isolate, tc);
            return;
        }

        Local<String> msg = tns::JsonStringifyObject(isolate, args[0])->ToString();

        JEnv env;
        auto mId = env.GetStaticMethodID(RUNTIME_CLASS, "sendMessageFromWorkerToMain",
                                         "(Ljava/lang/String;)V");

        auto jmsg = ArgConverter::ConvertToJavaString(msg);
        JniLocalRef jmsgRef(jmsg);

        env.CallStaticVoidMethod(RUNTIME_CLASS, mId, (jstring) jmsgRef);

        DEBUG_WRITE("WORKER: WorkerGlobalPostMessageCallback called.");
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::WorkerObjectOnMessageCallback(Isolate* isolate, jint workerId, jstring message) {
    try {
        auto workerFound = CallbackHandlers::id2WorkerMap.find(workerId);

        if (workerFound == CallbackHandlers::id2WorkerMap.end()) {
            // TODO: Pete: Throw exception
            DEBUG_WRITE(
                "MAIN: WorkerObjectOnMessageCallback no worker instance was found with workerId=%d.",
                workerId);
            return;
        }

        auto workerPersistent = workerFound->second;

        if (workerPersistent->IsEmpty()) {// Object has been collected
            DEBUG_WRITE(
                "MAIN: WorkerObjectOnMessageCallback couldn't fire a worker(id=%d) object's `onmessage` callback because the worker has been Garbage Collected.",
                workerId);
            CallbackHandlers::id2WorkerMap.erase(workerId);
            return;
        }

        auto worker = Local<Object>::New(isolate, *workerPersistent);

        auto callback = worker->Get(ArgConverter::ConvertToV8String(isolate, "onmessage"));
        auto isEmpty = callback.IsEmpty();
        auto isFunction = callback->IsFunction();

        if (!isEmpty && isFunction) {
            auto msgString = ArgConverter::jstringToV8String(isolate, message).As<String>();
            Local<Value> msg;
            JSON::Parse(isolate, msgString).ToLocal(&msg);

            auto obj = Object::New(isolate);
            obj->DefineOwnProperty(isolate->GetCurrentContext(),
                                   ArgConverter::ConvertToV8String(isolate, "data"), msg,
                                   PropertyAttribute::ReadOnly);
            Local<Value> args1[] = {obj};

            auto func = callback.As<Function>();

            func->Call(Undefined(isolate), 1, args1);
        } else {
            DEBUG_WRITE(
                "MAIN: WorkerObjectOnMessageCallback couldn't fire a worker(id=%d) object's `onmessage` callback because it isn't implemented.",
                workerId);
        }
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::WorkerObjectTerminateCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = args.GetIsolate();

    DEBUG_WRITE("WORKER: WorkerObjectTerminateCallback called.");

    try {
        HandleScope scope(isolate);

        auto thiz = args.This(); // Worker instance

        Local<Value> jsId;

        auto maybejsId = V8GetPrivateValue(isolate, thiz,
                                           ArgConverter::ConvertToV8String(isolate, "workerId"),
                                           jsId);

        // get worker's ID that is associated on the other side - in Java
        auto id = jsId->Int32Value();

        Local<Value> isTerminated;
        V8GetPrivateValue(isolate, thiz, ArgConverter::ConvertToV8String(isolate, "isTerminated"),
                          isTerminated);

        if (!isTerminated.IsEmpty() && isTerminated->BooleanValue()) {
            DEBUG_WRITE(
                "Main: WorkerObjectTerminateCallback - Worker(id=%d)'s terminate has already been called.",
                id);
            return;
        }

        V8SetPrivateValue(isolate, thiz, ArgConverter::ConvertToV8String(isolate, "isTerminated"),
                          Boolean::New(isolate, true));

        JEnv env;
        auto mId = env.GetStaticMethodID(RUNTIME_CLASS, "workerObjectTerminate",
                                         "(I)V");

        env.CallStaticVoidMethod(RUNTIME_CLASS, mId, id);

        // Remove persistent handle from id2WorkerMap
        CallbackHandlers::ClearWorkerPersistent(id);
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::WorkerGlobalCloseCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = args.GetIsolate();

    DEBUG_WRITE("WORKER: WorkerThreadCloseCallback called.");

    try {
        HandleScope scope(isolate);

        auto context = isolate->GetCurrentContext();
        auto globalObject = context->Global();

        auto isTerminating = globalObject->Get(
                                 ArgConverter::ConvertToV8String(isolate, "isTerminating"));

        if (!isTerminating.IsEmpty() && isTerminating->BooleanValue()) {
            DEBUG_WRITE("WORKER: WorkerThreadCloseCallback - Worker is currently terminating...");
            return;
        }

        globalObject->Set(ArgConverter::ConvertToV8String(isolate, "isTerminating"),
                          Boolean::New(isolate, true));

        // execute onclose handler if one is implemented
        auto callback = globalObject->Get(ArgConverter::ConvertToV8String(isolate, "onclose"));
        auto isEmpty = callback.IsEmpty();
        auto isFunction = callback->IsFunction();

        TryCatch tc(isolate);

        if (!isEmpty && isFunction) {
            Local<Value> args1[] = {};

            auto func = callback.As<Function>();

            DEBUG_WRITE("WORKER: WorketThreadCloseCallback onclose handle is being called.");
            func->Call(Undefined(isolate), 0, args1);
            DEBUG_WRITE("WORKER: WorketThreadCloseCallback onclose handle was called.");
        }

        if (tc.HasCaught()) {
            CallWorkerScopeOnErrorHandle(isolate, tc);
        }

        JEnv env;
        auto mId = env.GetStaticMethodID(RUNTIME_CLASS, "workerScopeClose",
                                         "()V");

        env.CallStaticVoidMethod(RUNTIME_CLASS, mId);
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::CallWorkerScopeOnErrorHandle(Isolate* isolate, TryCatch& tc) {
    try {
        TryCatch innerTc;

        // See if `onerror` handle is implemented
        auto context = isolate->GetCurrentContext();
        auto globalObject = context->Global();

        // execute onerror handle if one is implemented
        auto callback = globalObject->Get(ArgConverter::ConvertToV8String(isolate, "onerror"));
        auto isEmpty = callback.IsEmpty();
        auto isFunction = callback->IsFunction();

        if (!isEmpty && isFunction) {
            auto msg = tc.Message()->Get();
            Local<Value> args1[] = {msg};

            auto func = callback.As<Function>();

            auto result = func->Call(Undefined(isolate), 1, args1);

            // return 'true'-like value, don't bubble up to main Worker.onerror
            if (!result.IsEmpty() && result->BooleanValue()) {
                // Do nothing, exception has been handled
                return;
            }
        }

        // will account for exceptions thrown inside the error handler
        if (innerTc.HasCaught()) {
            auto lno = innerTc.Message()->GetLineNumber();
            auto msg = innerTc.Message()->Get();
            Local<Value> outStackTrace = innerTc.StackTrace(context).FromMaybe(Local<Value>());
            Local<String> stackTrace;
            if (!outStackTrace.IsEmpty()) {
                stackTrace = outStackTrace->ToDetailString();
            }
            auto source = innerTc.Message()->GetScriptResourceName()->ToString(isolate);

            auto runtime = Runtime::GetRuntime(isolate);
            runtime->PassUncaughtExceptionFromWorkerToMainHandler(msg, stackTrace, source, lno);
        }

        // throw so that it may bubble up to main
        auto lno = tc.Message()->GetLineNumber();
        auto msg = tc.Message()->Get();
        auto source = tc.Message()->GetScriptResourceName()->ToString(isolate);
        Local<Value> outStackTrace = tc.StackTrace(context).FromMaybe(Local<Value>());
        Local<String> stackTrace;
        if (!outStackTrace.IsEmpty()) {
            stackTrace = outStackTrace->ToDetailString();
        }

        auto runtime = Runtime::GetRuntime(isolate);
        runtime->PassUncaughtExceptionFromWorkerToMainHandler(msg, stackTrace, source, lno);
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::CallWorkerObjectOnErrorHandle(Isolate* isolate, jint workerId, jstring message, jstring stackTrace, jstring filename, jint lineno, jstring threadName) {
    try {
        auto workerFound = CallbackHandlers::id2WorkerMap.find(workerId);

        if (workerFound == CallbackHandlers::id2WorkerMap.end()) {
            // TODO: Pete: Throw exception
            DEBUG_WRITE(
                "MAIN: CallWorkerObjectOnErrorHandle no worker instance was found with workerId=%d.",
                workerId);
            return;
        }

        auto workerPersistent = workerFound->second;

        if (workerPersistent->IsEmpty()) {// Object has been collected
            DEBUG_WRITE(
                "MAIN: WorkerObjectOnMessageCallback couldn't fire a worker(id=%d) object's `onmessage` callback because the worker has been Garbage Collected.",
                workerId);
            CallbackHandlers::id2WorkerMap.erase(workerId);
            return;
        }

        auto worker = Local<Object>::New(isolate, *workerPersistent);

        auto callback = worker->Get(ArgConverter::ConvertToV8String(isolate, "onerror"));
        auto isEmpty = callback.IsEmpty();
        auto isFunction = callback->IsFunction();

        if (!isEmpty && isFunction) {
            auto errEvent = Object::New(isolate);
            errEvent->Set(ArgConverter::ConvertToV8String(isolate, "message"),
                          ArgConverter::jstringToV8String(isolate, message));
            errEvent->Set(ArgConverter::ConvertToV8String(isolate, "stackTrace"),
                          ArgConverter::jstringToV8String(isolate, stackTrace));
            errEvent->Set(ArgConverter::ConvertToV8String(isolate, "filename"),
                          ArgConverter::jstringToV8String(isolate, filename));
            errEvent->Set(ArgConverter::ConvertToV8String(isolate, "lineno"),
                          Number::New(isolate, lineno));

            Local<Value> args1[] = {errEvent};

            auto func = callback.As<Function>();

            // Handle exceptions thrown in onmessage with the worker.onerror handler, if present
            auto result = func->Call(Undefined(isolate), 1, args1);
            if (!result.IsEmpty() && result->BooleanValue()) {
                // Do nothing, exception is handled and does not need to be raised to application level
                return;
            }
        }

        // Exception wasn't handled, or is critical -> Throw exception
        auto strMessage = ArgConverter::jstringToString(message);
        auto strFilename = ArgConverter::jstringToString(filename);
        auto strThreadname = ArgConverter::jstringToString(threadName);
        auto strStackTrace = ArgConverter::jstringToString(stackTrace);

        DEBUG_WRITE("Unhandled exception in '%s' thread. file: %s, line %d\nStackTrace: %s",
                    strThreadname.c_str(), strFilename.c_str(), lineno, strMessage.c_str(), strStackTrace.c_str());

        // Do not throw exception?
//    stringstream ss;
//    ss << endl << "Unhandled exception in '" << strThreadname << "' thread. file: " << strFilename <<
//    ", line: " << lineno << endl << strMessage << endl;
//    NativeScriptException ex(ss.str());
//    throw ex;
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
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

void CallbackHandlers::ClearWorkerPersistent(int workerId) {
    DEBUG_WRITE("ClearWorkerPersistent called for workerId=%d", workerId);

    auto workerFound = CallbackHandlers::id2WorkerMap.find(workerId);

    if (workerFound == CallbackHandlers::id2WorkerMap.end()) {
        DEBUG_WRITE("MAIN | WORKER: ClearWorkerPersistent no worker instance was found with workerId=%d ! The worker may already be terminated.", workerId);
        return;
    }

    auto workerPersistent = workerFound->second;
    workerPersistent->Reset();

    id2WorkerMap.erase(workerId);
}

void CallbackHandlers::TerminateWorkerThread(Isolate* isolate) {
    auto context = isolate->GetCurrentContext();
    context->Exit();

    isolate->TerminateExecution();
}

int CallbackHandlers::nextWorkerId = 0;
std::map<int, Persistent<Object>*> CallbackHandlers::id2WorkerMap;

std::map<Isolate*, Persistent<Function>*> CallbackHandlers::isolateToJsonStringify;

short CallbackHandlers::MAX_JAVA_STRING_ARRAY_LENGTH = 100;
jclass CallbackHandlers::RUNTIME_CLASS = nullptr;
jclass CallbackHandlers::JAVA_LANG_STRING = nullptr;
jfieldID CallbackHandlers::CURRENT_OBJECTID_FIELD_ID = nullptr;
jmethodID CallbackHandlers::RESOLVE_CLASS_METHOD_ID = nullptr;
jmethodID CallbackHandlers::MAKE_INSTANCE_STRONG_ID = nullptr;
jmethodID CallbackHandlers::GET_TYPE_METADATA = nullptr;
jmethodID CallbackHandlers::ENABLE_VERBOSE_LOGGING_METHOD_ID = nullptr;
jmethodID CallbackHandlers::DISABLE_VERBOSE_LOGGING_METHOD_ID = nullptr;
jmethodID CallbackHandlers::INIT_WORKER_METHOD_ID = nullptr;
NumericCasts CallbackHandlers::castFunctions;
ArrayElementAccessor CallbackHandlers::arrayElementAccessor;
FieldAccessor CallbackHandlers::fieldAccessor;

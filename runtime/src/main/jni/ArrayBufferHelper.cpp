#include "ArrayBufferHelper.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"
#include <sstream>


using namespace v8;
using namespace tns;

ArrayBufferHelper::ArrayBufferHelper()
    : m_objectManager(nullptr), m_ByteBufferClass(nullptr), m_isDirectMethodID(nullptr) {
}

void ArrayBufferHelper::CreateConvertFunctions(Isolate* isolate, const Local<Object>& global, ObjectManager* objectManager) {
    m_objectManager = objectManager;
    auto extData = External::New(isolate, this);
    auto fromFunc = FunctionTemplate::New(isolate, CreateFromCallbackStatic, extData)->GetFunction();
    auto ctx = isolate->GetCurrentContext();
    auto arrBufferCtorFunc = global->Get(ArgConverter::ConvertToV8String(isolate, "ArrayBuffer")).As<Function>();
    arrBufferCtorFunc->Set(ctx, ArgConverter::ConvertToV8String(isolate, "from"), fromFunc);
}

void ArrayBufferHelper::CreateFromCallbackStatic(const FunctionCallbackInfo<Value>& info) {
    try {
        auto extData = info.Data().As<External>();
        auto thiz = reinterpret_cast<ArrayBufferHelper*>(extData->Value());
        thiz->CreateFromCallbackImpl(info);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void ArrayBufferHelper::CreateFromCallbackImpl(const FunctionCallbackInfo<Value>& info) {
    auto isolate = info.GetIsolate();
    auto len = info.Length();

    if (len != 1) {
        throw NativeScriptException("Wrong number of arguments (1 expected)");
    }

    auto arg = info[0];

    if (!arg->IsObject()) {
        throw NativeScriptException("Wrong type of argument (object expected)");
    }

    auto argObj = arg.As<Object>();

    auto obj = m_objectManager->GetJavaObjectByJsObject(argObj);

    if (obj.IsNull()) {
        throw NativeScriptException("Wrong type of argument (object expected)");
    }

    JEnv env;

    if (m_ByteBufferClass == nullptr) {
        m_ByteBufferClass = env.FindClass("java/nio/ByteBuffer");
        assert(m_ByteBufferClass != nullptr);
    }

    auto isByteBuffer = env.IsInstanceOf(obj, m_ByteBufferClass);

    if (!isByteBuffer) {
        throw NativeScriptException("Wrong type of argument (ByteBuffer expected)");
    }

    if (m_isDirectMethodID == nullptr) {
        m_isDirectMethodID = env.GetMethodID(m_ByteBufferClass, "isDirect", "()Z");
        assert(m_isDirectMethodID != nullptr);
    }

    auto ret = env.CallBooleanMethod(obj, m_isDirectMethodID);

    auto isDirectBuffer = ret == JNI_TRUE;

    if (!isDirectBuffer) {
        throw NativeScriptException("Direct ByteBuffer expected)");
    }

    auto data = env.GetDirectBufferAddress(obj);
    auto size = env.GetDirectBufferCapacity(obj);

    auto arrayBuffer = ArrayBuffer::New(isolate, data, size);
    auto ctx = isolate->GetCurrentContext();
    arrayBuffer->Set(ctx, ArgConverter::ConvertToV8String(isolate, "nativeObject"), argObj);

    info.GetReturnValue().Set(arrayBuffer);
}

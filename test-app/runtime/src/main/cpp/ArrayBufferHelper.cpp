#include "ArrayBufferHelper.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"
#include <sstream>


using namespace v8;
using namespace tns;

ArrayBufferHelper::ArrayBufferHelper()
        : m_objectManager(nullptr), m_ByteBufferClass(nullptr), m_isDirectMethodID(nullptr),
          m_remainingMethodID(nullptr), m_getMethodID(nullptr) {
}

void ArrayBufferHelper::CreateConvertFunctions(Local<Context> context, const Local<Object>& global, ObjectManager* objectManager) {
    m_objectManager = objectManager;
    Isolate* isolate = context->GetIsolate();
    auto extData = External::New(isolate, this);
    auto fromFunc = FunctionTemplate::New(isolate, CreateFromCallbackStatic, extData)->GetFunction(context).ToLocalChecked();
    auto arrBufferCtorFunc = global->Get(context, ArgConverter::ConvertToV8String(isolate, "ArrayBuffer")).ToLocalChecked().As<Function>();
    arrBufferCtorFunc->Set(context, ArgConverter::ConvertToV8String(isolate, "from"), fromFunc);
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

    Local<ArrayBuffer> arrayBuffer;

    if (isDirectBuffer) {
        auto data = env.GetDirectBufferAddress(obj);
        auto size = env.GetDirectBufferCapacity(obj);
        auto store = ArrayBuffer::NewBackingStore(data, size, [](void*, size_t, void*) { }, nullptr);
        arrayBuffer = ArrayBuffer::New(isolate, std::shared_ptr<v8::BackingStore>(std::move(store)));
    } else {
        if (m_remainingMethodID == nullptr) {
            m_remainingMethodID = env.GetMethodID(m_ByteBufferClass, "remaining", "()I");
            assert(m_remainingMethodID != nullptr);
        }

        int bufferRemainingSize = env.CallIntMethod(obj, m_remainingMethodID);

        if (m_getMethodID == nullptr) {
            m_getMethodID = env.GetMethodID(m_ByteBufferClass, "get", "([BII)Ljava/nio/ByteBuffer;");
            assert(m_getMethodID != nullptr);
        }

        jbyteArray byteArray = env.NewByteArray(bufferRemainingSize);
        env.CallObjectMethod(obj, m_getMethodID, byteArray, 0, bufferRemainingSize);

        auto buffer = env.GetByteArrayElements(byteArray, nullptr);
        arrayBuffer = ArrayBuffer::New(isolate, bufferRemainingSize);
        memcpy(arrayBuffer->GetBackingStore()->Data(), buffer, bufferRemainingSize);
    }

    auto ctx = isolate->GetCurrentContext();
    arrayBuffer->Set(ctx, ArgConverter::ConvertToV8String(isolate, "nativeObject"), argObj);

    info.GetReturnValue().Set(arrayBuffer);
}

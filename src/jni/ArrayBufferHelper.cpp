#include "ArrayBufferHelper.h"
#include "JEnv.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptException.h"
#include <sstream>


using namespace v8;
using namespace tns;

ArrayBufferHelper::ArrayBufferHelper()
{
}


void ArrayBufferHelper::CreateConvertFunctions(ObjectManager *objectManager, const Local<Object>& global)
{
	s_objectManager = objectManager;
	auto isolate = Isolate::GetCurrent();
	auto fromFunc = FunctionTemplate::New(isolate, CreateFromCallback)->GetFunction();
	auto ctx = isolate->GetCurrentContext();
	auto arrBufferCtorFunc = global->Get(ConvertToV8String("ArrayBuffer")).As<Function>();
	arrBufferCtorFunc->Set(ctx, ConvertToV8String("from"), fromFunc);
}

void ArrayBufferHelper::CreateFromCallback(const FunctionCallbackInfo<Value>& info)
{
	try
	{
		CreateFromCallbackImpl(info);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToV8();
	}
	catch (std::exception e) {
		std::stringstream ss;
		ss << "Error: c++ exception: " << e.what() << std::endl;
		NativeScriptException nsEx(ss.str());
		nsEx.ReThrowToV8();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToV8();
	}
}

void ArrayBufferHelper::CreateFromCallbackImpl(const FunctionCallbackInfo<Value>& info)
{
	auto isolate = info.GetIsolate();
	auto len = info.Length();

	if (len != 1)
	{
		throw NativeScriptException("Wrong number of arguments (1 expected)");
	}

	auto arg = info[0];

	if (!arg->IsObject())
	{
		throw NativeScriptException("Wrong type of argument (object expected)");
	}

	auto argObj = arg.As<Object>();

	auto obj = s_objectManager->GetJavaObjectByJsObject(argObj);

	if (obj == nullptr)
	{
		throw NativeScriptException("Wrong type of argument (object expected)");
	}

	JEnv env;

	auto clazz = env.FindClass("java/nio/MappedByteBuffer");

	auto isMappedBuffer = env.IsInstanceOf(obj, clazz);

	if (!isMappedBuffer)
	{
		throw NativeScriptException("Wrong type of argument (MappedBuffer expected)");
	}

	auto data = env.GetDirectBufferAddress(obj);
	auto size = env.GetDirectBufferCapacity(obj);

	auto arrayBuffer = ArrayBuffer::New(isolate, data, size);
	auto ctx = isolate->GetCurrentContext();
	arrayBuffer->Set(ctx, ConvertToV8String("nativeObject"), argObj);

	info.GetReturnValue().Set(arrayBuffer);
}

ObjectManager* ArrayBufferHelper::s_objectManager = nullptr;

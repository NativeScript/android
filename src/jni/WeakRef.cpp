#include "WeakRef.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "ExceptionUtil.h"

using namespace v8;
using namespace tns;
using namespace std;

WeakRef::WeakRef()
{
}

void WeakRef::Init(v8::Isolate *isolate, Local<ObjectTemplate>& globalObjectTemplate, ObjectManager *objectManager)
{
	s_objectManager = objectManager;

	globalObjectTemplate->Set(ConvertToV8String("WeakRef"), FunctionTemplate::New(isolate, ConstructorCallback));
}

void WeakRef::ConstructorCallback(const FunctionCallbackInfo<Value>& args)
{
	auto isolate = args.GetIsolate();

	if (args.IsConstructCall())
	{
		if (args.Length() == 1)
		{
			auto target = args[0];

			if (target->IsObject())
			{
				auto targetObj = target.As<Object>();

				auto weakRef = s_objectManager->GetEmptyObject(isolate);

				auto poTarget = new Persistent<Object>(isolate, targetObj);
				auto poHolder = new Persistent<Object>(isolate, weakRef);
				auto callbackState = new CallbackState(poTarget, poHolder);

				poTarget->SetWeak(callbackState, WeakTargetCallback);
				poHolder->SetWeak(callbackState, WeakHolderCallback);

				weakRef->Set(ConvertToV8String("get"), GetGetterFunction(isolate));
				weakRef->Set(ConvertToV8String("clear"), GetClearFunction(isolate));
				weakRef->SetHiddenValue(V8StringConstants::GetTarget(), External::New(isolate, poTarget));

				args.GetReturnValue().Set(weakRef);
			}
			else
			{
				ExceptionUtil::GetInstance()->ThrowExceptionToJs("The WeakRef constructor expects an object argument.");
			}
		}
		else
		{
			ExceptionUtil::GetInstance()->ThrowExceptionToJs("The WeakRef constructor expects single parameter.");
		}
	}
	else
	{
		ExceptionUtil::GetInstance()->ThrowExceptionToJs("WeakRef must be used as a construct call.");
	}
}

void WeakRef::WeakTargetCallback(const WeakCallbackData<Object, CallbackState>& data)
{
	auto callbackState = data.GetParameter();
	auto poTarget = callbackState->target;
	poTarget->Reset();
	delete poTarget;
	callbackState->target = nullptr;

	auto isolate = data.GetIsolate();
	auto poHolder = callbackState->holder;
	if (poHolder != nullptr)
	{
		auto holder = Local<Object>::New(isolate, *poHolder);
		holder->SetHiddenValue(V8StringConstants::GetTarget(), External::New(isolate, nullptr));
	}

	if (callbackState->holder == nullptr)
	{
		delete callbackState;
	}
}

void WeakRef::WeakHolderCallback(const WeakCallbackData<Object, CallbackState>& data)
{
	auto callbackState = data.GetParameter();
	auto poHolder = callbackState->holder;
	auto isolate = data.GetIsolate();
	auto holder = Local<Object>::New(isolate, *poHolder);

	auto poTarget = reinterpret_cast<Persistent<Object>*>(holder->GetHiddenValue(V8StringConstants::GetTarget()).As<External>()->Value());

	if (poTarget != nullptr)
	{
		poHolder->SetWeak(callbackState, WeakHolderCallback);
	}
	else
	{
		poHolder->Reset();
		delete poHolder;
		callbackState->holder = nullptr;
		if (callbackState->target == nullptr)
		{
			delete callbackState;
		}
	}
}

void WeakRef::ClearCallback(const FunctionCallbackInfo<Value>& args)
{
	auto holder = args.This();
	auto isolate = Isolate::GetCurrent();

	holder->SetHiddenValue(V8StringConstants::GetTarget(), External::New(isolate, nullptr));
}

void WeakRef::GettertCallback(const FunctionCallbackInfo<Value>& args)
{
	auto holder = args.This();
	auto poTarget = reinterpret_cast<Persistent<Object>*>(holder->GetHiddenValue(V8StringConstants::GetTarget()).As<External>()->Value());
	auto isolate = Isolate::GetCurrent();

	if (poTarget != nullptr)
	{
		auto target = Local<Object>::New(isolate, *poTarget);
		args.GetReturnValue().Set(target);
	}
	else
	{
		args.GetReturnValue().Set(Null(isolate));
	}
}

Local<Function> WeakRef::GetGetterFunction(Isolate *isolate)
{
	if (s_poGetterFunc != nullptr)
	{
		return Local<Function>::New(isolate, *s_poGetterFunc);
	}
	else
	{
		auto getterFunc = FunctionTemplate::New(isolate, GettertCallback)->GetFunction();
		s_poGetterFunc = new Persistent<Function>(isolate, getterFunc);
		return getterFunc;
	}
}

Local<Function> WeakRef::GetClearFunction(Isolate *isolate)
{
	if (s_poClearFunc != nullptr)
	{
		return Local<Function>::New(isolate, *s_poClearFunc);
	}
	else
	{
		auto clearFunc = FunctionTemplate::New(isolate, ClearCallback)->GetFunction();
		s_poClearFunc = new Persistent<Function>(isolate, clearFunc);
		return clearFunc;
	}
}


Persistent<Function>* WeakRef::s_poClearFunc = nullptr;
Persistent<Function>* WeakRef::s_poGetterFunc = nullptr;
ObjectManager* WeakRef::s_objectManager = nullptr;

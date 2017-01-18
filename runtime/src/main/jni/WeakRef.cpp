#include "WeakRef.h"
#include "ArgConverter.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"
#include "V8GlobalHelpers.h"
#include <sstream>

using namespace v8;
using namespace tns;
using namespace std;

WeakRef::WeakRef()
    : m_objectManager(nullptr), m_poClearFunc(nullptr), m_poGetterFunc(nullptr) {
}

void WeakRef::Init(v8::Isolate* isolate, Local<ObjectTemplate>& globalObjectTemplate, ObjectManager* objectManager) {
    m_objectManager = objectManager;
    auto extData = External::New(isolate, this);
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "WeakRef"), FunctionTemplate::New(isolate, ConstructorCallback, extData));
}

void WeakRef::ConstructorCallback(const FunctionCallbackInfo<Value>& args) {
    try {
        auto extData = args.Data().As<External>();
        auto thiz = reinterpret_cast<WeakRef*>(extData->Value());
        thiz->ConstructorCallbackImpl(args);
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

void WeakRef::ConstructorCallbackImpl(const FunctionCallbackInfo<Value>& args) {
    auto isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        if (args.Length() == 1) {
            auto target = args[0];

            if (target->IsObject()) {
                auto targetObj = target.As<Object>();

                auto weakRef = m_objectManager->GetEmptyObject(isolate);

                auto poTarget = new Persistent<Object>(isolate, targetObj);
                auto poHolder = new Persistent<Object>(isolate, weakRef);
                auto callbackState = new CallbackState(poTarget, poHolder);

                poTarget->SetWeak(callbackState, WeakTargetCallback, WeakCallbackType::kFinalizer);
                poHolder->SetWeak(callbackState, WeakHolderCallback, WeakCallbackType::kFinalizer);

                weakRef->Set(ArgConverter::ConvertToV8String(isolate, "get"), GetGetterFunction(isolate));
                weakRef->Set(ArgConverter::ConvertToV8String(isolate, "clear"), GetClearFunction(isolate));
                V8SetPrivateValue(isolate, weakRef, V8StringConstants::GetTarget(isolate), External::New(isolate, poTarget));

                args.GetReturnValue().Set(weakRef);
            } else {
                throw NativeScriptException(string("The WeakRef constructor expects an object argument."));
            }
        } else {
            throw NativeScriptException(string("The WeakRef constructor expects single parameter."));
        }
    } else {
        throw NativeScriptException(string("WeakRef must be used as a construct call."));
    }
}

void WeakRef::WeakTargetCallback(const WeakCallbackInfo<CallbackState>& data) {
    auto callbackState = data.GetParameter();
    auto poTarget = callbackState->target;
    poTarget->Reset();
    delete poTarget;
    callbackState->target = nullptr;

    auto isolate = data.GetIsolate();
    auto poHolder = callbackState->holder;
    if (poHolder != nullptr) {
        auto holder = Local<Object>::New(isolate, *poHolder);
        V8SetPrivateValue(isolate, holder, V8StringConstants::GetTarget(isolate), External::New(isolate, nullptr));
    }

    if (callbackState->holder == nullptr) {
        delete callbackState;
    }
}

void WeakRef::WeakHolderCallback(const WeakCallbackInfo<CallbackState>& data) {
    try {
        auto callbackState = data.GetParameter();
        auto poHolder = callbackState->holder;
        auto isolate = data.GetIsolate();
        auto holder = Local<Object>::New(isolate, *poHolder);

        Local<Value> hiddenVal;
        V8GetPrivateValue(isolate, holder, V8StringConstants::GetTarget(isolate), hiddenVal);
        auto poTarget = reinterpret_cast<Persistent<Object>*>(hiddenVal.As<External>()->Value());

        if (poTarget != nullptr) {
            poHolder->SetWeak(callbackState, WeakHolderCallback, WeakCallbackType::kFinalizer);
        } else {
            poHolder->Reset();
            delete poHolder;
            callbackState->holder = nullptr;
            if (callbackState->target == nullptr) {
                delete callbackState;
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

void WeakRef::ClearCallback(const FunctionCallbackInfo<Value>& args) {
    try {
        auto holder = args.This();
        auto isolate = args.GetIsolate();

        V8SetPrivateValue(isolate, holder, V8StringConstants::GetTarget(isolate), External::New(isolate, nullptr));
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

void WeakRef::GettertCallback(const FunctionCallbackInfo<Value>& args) {
    try {
        auto holder = args.This();
        Local<Value> hiddenVal;
        auto isolate = args.GetIsolate();
        V8GetPrivateValue(isolate, holder, V8StringConstants::GetTarget(isolate), hiddenVal);
        auto poTarget = reinterpret_cast<Persistent<Object>*>(hiddenVal.As<External>()->Value());

        if (poTarget != nullptr) {
            auto target = Local<Object>::New(isolate, *poTarget);
            args.GetReturnValue().Set(target);
        } else {
            args.GetReturnValue().Set(Null(isolate));
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

Local<Function> WeakRef::GetGetterFunction(Isolate* isolate) {
    try {
        if (m_poGetterFunc != nullptr) {
            return Local<Function>::New(isolate, *m_poGetterFunc);
        } else {
            auto extData = External::New(isolate, this);
            auto getterFunc = FunctionTemplate::New(isolate, GettertCallback, extData)->GetFunction();
            m_poGetterFunc = new Persistent<Function>(isolate, getterFunc);
            return getterFunc;
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

Local<Function> WeakRef::GetClearFunction(Isolate* isolate) {
    try {
        if (m_poClearFunc != nullptr) {
            return Local<Function>::New(isolate, *m_poClearFunc);
        } else {
            auto extData = External::New(isolate, this);
            auto clearFunc = FunctionTemplate::New(isolate, ClearCallback, extData)->GetFunction();
            m_poClearFunc = new Persistent<Function>(isolate, clearFunc);
            return clearFunc;
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

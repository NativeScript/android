#ifndef WEAKREF_H_
#define WEAKREF_H_

#include "v8.h"
#include <set>
#include "ObjectManager.h"

namespace tns {
class WeakRef {
    public:
        WeakRef();

        void Init(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate>& globalObjectTemplate, ObjectManager* objectManager);

    private:
        struct CallbackState {
            CallbackState(v8::Persistent<v8::Object>* _target, v8::Persistent<v8::Object>* _holder)
                :
                target(_target), holder(_holder) {
            }
            v8::Persistent<v8::Object>* target;
            v8::Persistent<v8::Object>* holder;
        };

        static void ConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void ConstructorCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void ClearCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void GettertCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        v8::Local<v8::Function> GetClearFunction(v8::Isolate* isolate);

        v8::Local<v8::Function> GetGetterFunction(v8::Isolate* isolate);

        static void WeakTargetCallback(const v8::WeakCallbackInfo<CallbackState>& data);

        static void WeakHolderCallback(const v8::WeakCallbackInfo<CallbackState>& data);

        ObjectManager* m_objectManager;

        v8::Persistent<v8::Function>* m_poClearFunc;

        v8::Persistent<v8::Function>* m_poGetterFunc;
};
}

#endif /* WEAKREF_H_ */

#ifndef ARRAYBUFFERHELPER_H_
#define ARRAYBUFFERHELPER_H_

#include "v8.h"
#include "ObjectManager.h"

namespace tns {
class ArrayBufferHelper {
    public:
        ArrayBufferHelper();

        void CreateConvertFunctions(v8::Isolate* isolate, const v8::Local<v8::Object>& global, ObjectManager* objectManager);

    private:

        static void CreateFromCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& info);

        void CreateFromCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& info);

        ObjectManager* m_objectManager;

        jclass m_ByteBufferClass;

        jmethodID m_isDirectMethodID;
};
}


#endif /* ARRAYBUFFERHELPER_H_ */

#ifndef V8STRINGCONSTANTS_H_
#define V8STRINGCONSTANTS_H_

#include <string>
#include "include/v8.h"

using namespace v8;

namespace tns {
class V8StringConstants {
    public:
        static v8::Local<v8::String> GetClassImplementationObject(v8::Isolate* isolate);

        static v8::Local<v8::String> GetExtend(v8::Isolate* isolate);

        static v8::Local<v8::String> GetNullObject(v8::Isolate* isolate);

        static v8::Local<v8::String> GetNullNodeName(v8::Isolate* isolate);

        static v8::Local<v8::String> GetIsPrototypeImplementationObject(v8::Isolate* isolate);

        static v8::Local<v8::String> GetNativeException(v8::Isolate* isolate);

        static v8::Local<v8::String> GetStackTrace(v8::Isolate* isolate);

        static v8::Local<v8::String> GetLongNumber(v8::Isolate* isolate);

        static v8::Local<v8::String> GetPrototype(v8::Isolate* isolate);

        static v8::Local<v8::String> GetSuper(v8::Isolate* isolate);

        static v8::Local<v8::String> GetTarget(v8::Isolate* isolate);

        static v8::Local<v8::String> GetToString(v8::Isolate* isolate);

        static v8::Local<v8::String> GetJavaLong(v8::Isolate* isolate);

        static v8::Local<v8::String> GetValueOf(v8::Isolate* isolate);

        static v8::Local<v8::String> GetValue(v8::Isolate* isolate);

        static v8::Local<v8::String> GetUncaughtError(v8::Isolate* isolate);

        static v8::Local<v8::String> GetImplementationObject(v8::Isolate* isolate);

        static const std::string CLASS_IMPLEMENTATION_OBJECT;
        static const std::string DEBUG_NAME;
        static const std::string EXTEND;
        static const std::string NULL_OBJECT;
        static const std::string NULL_NODE_NAME;
        static const std::string IS_PROTOTYPE_IMPLEMENTATION_OBJECT;
        static const std::string NATIVE_EXCEPTION;
        static const std::string STACK_TRACE;
        static const std::string LONG_NUMBER;
        static const std::string PROTOTYPE;
        static const std::string SUPER;
        static const std::string TARGET;
        static const std::string TO_STRING;
        static const std::string JAVA_LONG;
        static const std::string VALUE_OF;
        static const std::string VALUE;
        static const std::string UNCAUGHT_ERROR;
        static const std::string IMPLEMENTATION_OBJECT;

        /*
         * The structure contains Persistent handles to the most commonly used v8 String values;
         * Persistents are created eagerly, and then reused whenever requested
         * The same PerIsolateV8Constants struct should only be used on the thread the isolated is created in
         */
        struct PerIsolateV8Constants {
            PerIsolateV8Constants(v8::Isolate* isolate) {
                auto str = String::NewFromUtf8(isolate, CLASS_IMPLEMENTATION_OBJECT.c_str());
                CLASS_IMPLEMENTATION_OBJECT_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, EXTEND.c_str());
                EXTEND_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, NULL_OBJECT.c_str());
                NULL_OBJECT_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, NULL_NODE_NAME.c_str());
                NULL_NODE_NAME_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, IS_PROTOTYPE_IMPLEMENTATION_OBJECT.c_str());
                IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT = new Persistent<String>(isolate,
                        str);


                str = String::NewFromUtf8(isolate, NATIVE_EXCEPTION.c_str());
                NATIVE_EXCEPTION_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, STACK_TRACE.c_str());
                STACK_TRACE_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, LONG_NUMBER.c_str());
                LONG_NUMBER_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, PROTOTYPE.c_str());
                PROTOTYPE_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, SUPER.c_str());
                SUPER_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, TARGET.c_str());
                TARGET_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, TO_STRING.c_str());
                TO_STRING_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, JAVA_LONG.c_str());
                JAVA_LONG_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, VALUE.c_str());
                VALUE_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, VALUE_OF.c_str());
                VALUE_OF_PERSISTENT = new Persistent<String>(isolate, str);


                str = String::NewFromUtf8(isolate, UNCAUGHT_ERROR.c_str());
                UNCAUGHT_ERROR_PERSISTENT = new Persistent<String>(isolate, str);

                str = String::NewFromUtf8(isolate, IMPLEMENTATION_OBJECT.c_str());
                IMPLEMENTATION_OBJECT_PERSISTENT = new Persistent<String>(isolate, str);
            };

            ~PerIsolateV8Constants() {
                CLASS_IMPLEMENTATION_OBJECT_PERSISTENT->Reset();
                DEBUG_NAME_PERSISTENT->Reset();
                EXTEND_PERSISTENT->Reset();
                NULL_OBJECT_PERSISTENT->Reset();
                NULL_NODE_NAME_PERSISTENT->Reset();
                IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT->Reset();
                NATIVE_EXCEPTION_PERSISTENT->Reset();
                STACK_TRACE_PERSISTENT->Reset();
                LONG_NUMBER_PERSISTENT->Reset();
                PROTOTYPE_PERSISTENT->Reset();
                SUPER_PERSISTENT->Reset();
                TARGET_PERSISTENT->Reset();
                TO_STRING_PERSISTENT->Reset();
                JAVA_LONG_PERSISTENT->Reset();
                VALUE_OF_PERSISTENT->Reset();
                VALUE_PERSISTENT->Reset();
                UNCAUGHT_ERROR_PERSISTENT->Reset();
                IMPLEMENTATION_OBJECT_PERSISTENT->Reset();
            }

            v8::Persistent<v8::String>* CLASS_IMPLEMENTATION_OBJECT_PERSISTENT;
            v8::Persistent<v8::String>* DEBUG_NAME_PERSISTENT;
            v8::Persistent<v8::String>* EXTEND_PERSISTENT;
            v8::Persistent<v8::String>* NULL_OBJECT_PERSISTENT;
            v8::Persistent<v8::String>* NULL_NODE_NAME_PERSISTENT;
            v8::Persistent<v8::String>* IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT;
            v8::Persistent<v8::String>* NATIVE_EXCEPTION_PERSISTENT;
            v8::Persistent<v8::String>* STACK_TRACE_PERSISTENT;
            v8::Persistent<v8::String>* LONG_NUMBER_PERSISTENT;
            v8::Persistent<v8::String>* PROTOTYPE_PERSISTENT;
            v8::Persistent<v8::String>* SUPER_PERSISTENT;
            v8::Persistent<v8::String>* TARGET_PERSISTENT;
            v8::Persistent<v8::String>* TO_STRING_PERSISTENT;
            v8::Persistent<v8::String>* JAVA_LONG_PERSISTENT;
            v8::Persistent<v8::String>* VALUE_OF_PERSISTENT;
            v8::Persistent<v8::String>* VALUE_PERSISTENT;
            v8::Persistent<v8::String>* UNCAUGHT_ERROR_PERSISTENT;
            v8::Persistent<v8::String>* IMPLEMENTATION_OBJECT_PERSISTENT;
        };

    private:
        V8StringConstants() {
        };

        static PerIsolateV8Constants* GetConstantsForIsolate(v8::Isolate* isolate);


};
}

#endif /* V8STRINGCONSTANTS_H_ */

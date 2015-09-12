#include "v8.h"

namespace v8 {
	
	class NativeScriptExtension {
	public:
		static uint8_t* GetAddress(const v8::Local<v8::Object>& obj);

		static v8::Local<v8::Value>* GetClosureObjects(v8::Isolate *isolate, const v8::Local<v8::Function>& func, int *length);

		static void ReleaseClosureObjects(v8::Local<v8::Value>* closureObjects);
		
		static void GetAssessorPair(v8::Isolate *isolate, const v8::Local<v8::Object>& obj, const v8::Local<v8::String>& propName, v8::Local<v8::Value>& getter, v8::Local<v8::Value>& setter);

		static v8::Local<v8::Array> GetPropertyKeys(v8::Isolate *isolate, const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object, bool& success);

		static int GetInternalFieldCount(const v8::Local<v8::Object>& object);
	private:
		NativeScriptExtension();
	};
}


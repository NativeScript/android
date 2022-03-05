#include <vector>
#include "v8.h"

namespace v8 {

	class NativeScriptExtension {
	public:
		static unsigned long GetAddress(const v8::Local<v8::Object>& obj);

		static v8::Local<v8::Value>* GetClosureObjects(v8::Isolate* isolate, const v8::Local<v8::Function>& func, int* length);

		static void ReleaseClosureObjects(v8::Local<v8::Value>* closureObjects);

		static void GetAssessorPair(v8::Isolate* isolate, const v8::Local<v8::Object>& obj, const v8::Local<v8::String>& propName, v8::Local<v8::Value>& getter, v8::Local<v8::Value>& setter);

		static std::vector<v8::Local<v8::Value>> GetPropertyKeys(v8::Isolate* isolate, const v8::Local<v8::Object>& object);

		static void CpuFeaturesProbe(bool cross_compile);
	private:
		NativeScriptExtension();
	};
}

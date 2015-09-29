#ifndef METHODCACHE_H_
#define METHODCACHE_H_

#include <string>
#include <map>
#include "v8.h"
#include "JEnv.h"
#include "MetadataEntry.h"

namespace tns
{
	class MethodCache
	{
	public:
		struct CacheMethodInfo
		{
			CacheMethodInfo()
				: retType(MethodReturnType::Unknown), mid(nullptr), clazz(nullptr), isStatic(false)
			{
			}
			std::string signature;
			std::string returnType;
			MethodReturnType retType;
			jmethodID mid;
			jclass clazz;
			bool isStatic;
		};

		static void Init();

		//static std::string ResolveMethodSignature(const std::string& className, const std::string& methodName, const v8::FunctionCallbackInfo<v8::Value>& args);
		static CacheMethodInfo ResolveMethodSignature(const std::string& className, const std::string& methodName, const v8::FunctionCallbackInfo<v8::Value>& args, bool isStatic);

		static std::string EncodeSignature(const std::string& className, const std::string& methodName, const v8::FunctionCallbackInfo<v8::Value>& args, bool isStatic);

	private:
		MethodCache() {}

		static std::string GetType(const v8::Local<v8::Value>& value);

		static std::string ResolveJavaMethod(const v8::FunctionCallbackInfo<v8::Value>& args, const std::string& className, const std::string& methodName);

		static jclass PLATFORM_CLASS;

		static jmethodID RESOLVE_METHOD_OVERLOAD_METHOD_ID;

		//static std::map<std::string, std::string> s_cache;
		static std::map<std::string, CacheMethodInfo> s_cache;
	};
}



#endif /* METHODCACHE_H_ */

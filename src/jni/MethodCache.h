#ifndef METHODCACHE_H_
#define METHODCACHE_H_

#include <string>
#include <map>
#include "v8.h"

namespace tns
{
	class MethodCache
	{
	public:
		static std::string ResolveMethodSignature(const std::string& className, const std::string& methodName, const v8::FunctionCallbackInfo<v8::Value>& args);

		static std::string EncodeSignature(const std::string& className, const std::string& methodName, const v8::FunctionCallbackInfo<v8::Value>& args);

	private:
		MethodCache() {}

		static std::string GetType(const v8::Handle<v8::Value>& value);

		static std::map<std::string, std::string> s_cache;
	};
}



#endif /* METHODCACHE_H_ */

/*
 * ArgsWrapper.h
 *
 *  Created on: Dec 20, 2013
 *      Author: slavchev
 */

#ifndef ARGSWRAPPER_H_
#define ARGSWRAPPER_H_

#include <v8.h>

namespace tns
{
	enum class ArgType
	{
		Class,
		Interface
	};

	struct ArgsWrapper
	{
	public:
		ArgsWrapper(const v8::FunctionCallbackInfo<v8::Value>& a, ArgType t, v8::Local<v8::Object> _outerThis)
		 : args(a), type(t), outerThis(_outerThis)
		{
		}
		v8::FunctionCallbackInfo<v8::Value> args;
		ArgType type;
		v8::Local<v8::Object> outerThis;
	};
}

#endif /* ARGSWRAPPER_H_ */

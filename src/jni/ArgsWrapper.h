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
		ArgsWrapper(v8::FunctionCallbackInfo<v8::Value> a, ArgType t, bool _isInnerClass)
		 : args(a), type(t), isInnerClass(_isInnerClass)
		{
		}
		v8::FunctionCallbackInfo<v8::Value> args;
		ArgType type;
		bool isInnerClass;
	};
}

#endif /* ARGSWRAPPER_H_ */

/*
 * ArgsWrapper.h
 *
 *  Created on: Dec 20, 2013
 *      Author: slavchev
 */

#ifndef ARGSWRAPPER_H_
#define ARGSWRAPPER_H_

#include <v8.h>

namespace tns {
enum class ArgType {
    Class,
    Interface
};

struct ArgsWrapper {
    public:
        ArgsWrapper(const v8::FunctionCallbackInfo<v8::Value>& a, ArgType t)
            :
            args(a), type(t) {
        }
        v8::FunctionCallbackInfo<v8::Value> args;
        ArgType type;
};
}

#endif /* ARGSWRAPPER_H_ */

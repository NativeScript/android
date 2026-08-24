#ifndef BASE64_H_
#define BASE64_H_

#include "v8.h"

namespace tns {

/*
 * Native ops behind the base64 builtin (internal/base64.js): the WHATWG
 * forgiving-base64 codec backing the atob / btoa globals. Both ops answer
 * null instead of throwing, so the builtin owns the error shape.
 */
class Base64 {
public:
    static v8::Local<v8::Object> CreateBinding(v8::Local<v8::Context> context);
};

}  // namespace tns

#endif /* BASE64_H_ */

#ifndef WEAKREF_H_
#define WEAKREF_H_

#include "v8.h"
#include <set>
#include "ObjectManager.h"

namespace tns {
class WeakRef {
    public:
        WeakRef();

        void Init(v8::Isolate* isolate, v8::Local<v8::Context> context);

    private:
};
}

#endif /* WEAKREF_H_ */

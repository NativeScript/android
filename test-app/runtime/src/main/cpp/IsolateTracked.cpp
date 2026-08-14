#include "IsolateTracked.h"

#include "RuntimeState.h"
#include "robin_hood.h"

namespace tns {

namespace {
// The live instances of one runtime; see RuntimeState.h. Touched only on that
// runtime's own thread -- both the GC finalizer and the teardown sweep run
// there -- so it needs no synchronization.
struct TrackedInstances {
    robin_hood::unordered_set<IsolateTracked*> live;
};
}  // namespace

void IsolateTracked::BindFinalizer(v8::Isolate* isolate,
                                   const v8::Local<v8::Object>& object) {
    v8::HandleScope scopedHandle(isolate);
    weakHandle_.Reset(isolate, object);
    weakHandle_.SetWeak(this, Finalizer, v8::WeakCallbackType::kParameter);

    auto* tracked = RuntimeState::For<TrackedInstances>(isolate);
    if (tracked != nullptr) {
        tracked->live.insert(this);
    }
}

void IsolateTracked::Finalizer(const v8::WeakCallbackInfo<IsolateTracked>& data) {
    IsolateTracked* self = data.GetParameter();

    auto* tracked = RuntimeState::For<TrackedInstances>(data.GetIsolate());
    if (tracked != nullptr) {
        tracked->live.erase(self);
    }

    delete self;
}

void IsolateTracked::SweepAll(v8::Isolate* isolate) {
    auto* tracked = RuntimeState::For<TrackedInstances>(isolate);
    if (tracked == nullptr) {
        return;
    }

    // Detached first: a destructor that somehow reached back into the registry
    // must not mutate the set being walked.
    auto survivors = std::move(tracked->live);
    tracked->live.clear();

    for (IsolateTracked* instance : survivors) {
        delete instance;
    }
}

}  // namespace tns

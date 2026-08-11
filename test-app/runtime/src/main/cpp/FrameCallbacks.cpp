#include "FrameCallbacks.h"

#include <android/api-level.h>
#include <android/looper.h>
#include <dlfcn.h>
#include <time.h>

#include <atomic>
#include <cassert>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "ArgConverter.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"
#include "Performance.h"
#include "Runtime.h"
#include "V8GlobalHelpers.h"
#include "robin_hood.h"

using namespace v8;

namespace tns {

namespace {

// ---- AChoreographer (NDK, API 24+), resolved through dlsym.

struct AChoreographer;

typedef void (*AChoreographer_frameCallback)(long frameTimeNanos, void* data);
typedef void (*AChoreographer_frameCallback64)(int64_t frameTimeNanos,
                                               void* data);
typedef AChoreographer* (*func_getInstance)();
typedef void (*func_postFrameCallback)(AChoreographer*,
                                       AChoreographer_frameCallback, void*);
typedef void (*func_postFrameCallback64)(AChoreographer*,
                                         AChoreographer_frameCallback64, void*);
typedef void (*func_postFrameCallbackDelayed)(AChoreographer*,
                                              AChoreographer_frameCallback,
                                              void*, long);
typedef void (*func_postFrameCallbackDelayed64)(AChoreographer*,
                                                AChoreographer_frameCallback64,
                                                void*, uint32_t);

func_getInstance getInstance_ = nullptr;
func_postFrameCallback postFrameCallback_ = nullptr;
func_postFrameCallbackDelayed postFrameCallbackDelayed_ = nullptr;
func_postFrameCallback64 postFrameCallback64_ = nullptr;
func_postFrameCallbackDelayed64 postFrameCallbackDelayed64_ = nullptr;

void ResolveChoreographer() {
    static std::once_flag once;
    std::call_once(once, [] {
        if (android_get_device_api_level() < 24) {
            return;
        }

        void* lib = dlopen("libandroid.so", RTLD_NOW | RTLD_LOCAL);
        if (lib == nullptr) {
            return;
        }

        getInstance_ = reinterpret_cast<func_getInstance>(
                dlsym(lib, "AChoreographer_getInstance"));
        postFrameCallback_ = reinterpret_cast<func_postFrameCallback>(
                dlsym(lib, "AChoreographer_postFrameCallback"));
        postFrameCallbackDelayed_ = reinterpret_cast<func_postFrameCallbackDelayed>(
                dlsym(lib, "AChoreographer_postFrameCallbackDelayed"));

        if (android_get_device_api_level() >= 29) {
            postFrameCallback64_ = reinterpret_cast<func_postFrameCallback64>(
                    dlsym(lib, "AChoreographer_postFrameCallback64"));
            postFrameCallbackDelayed64_ =
                    reinterpret_cast<func_postFrameCallbackDelayed64>(
                            dlsym(lib, "AChoreographer_postFrameCallbackDelayed64"));
        }
    });
}

bool NativeChoreographerAvailable() {
    ResolveChoreographer();
    return getInstance_ != nullptr && postFrameCallback_ != nullptr &&
           postFrameCallbackDelayed_ != nullptr;
}

// ---- Implementation selection.

enum class Impl { kAuto, kNative, kJava };

Impl forcedImpl_ = Impl::kAuto;

bool UseNativeChoreographer() {
    if (forcedImpl_ == Impl::kJava) {
        return false;
    }
    return NativeChoreographerAvailable();
}

/*
 * AChoreographer_postFrameCallback hands the frame time as `long`, which is 32
 * bits on the 32-bit ABIs and so wraps about every 4.3 s -- the reason
 * postFrameCallback64 exists from API 29. The frame time is always within a
 * frame of now, so the high bits are recoverable from the monotonic clock.
 */
int64_t WidenFrameTimeNanos(long ts) {
    if constexpr (sizeof(long) == sizeof(int64_t)) {
        return (int64_t) ts;
    } else {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        int64_t nowNanos = (int64_t) now.tv_sec * 1000000000LL + now.tv_nsec;
        int64_t candidate =
                (nowNanos & ~0xFFFFFFFFLL) | (int64_t)(uint32_t) ts;
        if (candidate > nowNanos) {
            candidate -= 0x100000000LL;
        }
        return candidate;
    }
}

// ---- Scheduled callbacks.

jclass FRAME_CALLBACKS_CLASS = nullptr;
jmethodID FRAME_CALLBACKS_CTOR = nullptr;
jmethodID FRAME_CALLBACKS_POST = nullptr;
jmethodID FRAME_CALLBACKS_RELEASE = nullptr;

void ResolveFrameCallbacksClass(JEnv& env) {
    static std::once_flag once;
    std::call_once(once, [&env] {
        // JEnv::FindClass caches a global ref to the class
        FRAME_CALLBACKS_CLASS = env.FindClass("com/tns/FrameCallbacks");
        assert(FRAME_CALLBACKS_CLASS != nullptr);
        FRAME_CALLBACKS_CTOR = env.GetMethodID(FRAME_CALLBACKS_CLASS, "<init>", "(J)V");
        FRAME_CALLBACKS_POST = env.GetMethodID(FRAME_CALLBACKS_CLASS, "post", "(J)V");
        FRAME_CALLBACKS_RELEASE =
                env.GetMethodID(FRAME_CALLBACKS_CLASS, "release", "()V");
    });
}

/*
 * Entries are identified to the platform by id rather than by address: an id
 * that no longer resolves is simply a frame arriving after its entry went
 * away, where a stale pointer would be a use-after-free.
 */
using EntryId = uintptr_t;

struct FrameCallbackEntry {
    FrameCallbackEntry(Isolate* isolate, Local<Function> callback, EntryId id)
        : isolate_(isolate), callback_(isolate, callback), id_(id) {
    }

    ~FrameCallbackEntry() {
        callback_.Reset();
        ReleaseJavaCallback();
    }

    void ReleaseJavaCallback() {
        if (javaCallback_ == nullptr) {
            return;
        }
        JEnv env;
        env.CallVoidMethod(javaCallback_, FRAME_CALLBACKS_RELEASE);
        env.DeleteGlobalRef(javaCallback_);
        javaCallback_ = nullptr;
    }

    bool IsScheduled() const {
        return scheduled_;
    }

    void MarkScheduled() {
        scheduled_ = true;
        removed_ = false;
    }

    // A posted frame callback cannot be recalled, so removal only marks.
    void MarkRemoved() {
        removed_ = true;
    }

    void MarkUnscheduled() {
        scheduled_ = false;
        removed_ = true;
    }

    bool ShouldRemoveBeforeCall() const {
        return removed_;
    }

    bool ShouldRemoveAfterCall() const {
        return !scheduled_ && removed_;
    }

    Isolate* isolate_;
    Global<Function> callback_;
    EntryId id_;
    jobject javaCallback_ = nullptr;

private:
    bool removed_ = false;
    bool scheduled_ = false;
};

/*
 * One registry for every isolate in the process, so each of the two lookups
 * below takes the mutex. It must never be held across the JS call: a callback
 * that reschedules itself re-enters PostFrameCallback and would deadlock.
 * Creating, erasing and dispatching an entry all happen on the thread that
 * owns its isolate, so a pointer resolved under the mutex stays valid after
 * releasing it -- only this thread can retire it.
 */
robin_hood::unordered_map<EntryId, std::unique_ptr<FrameCallbackEntry>> entries_;
std::mutex entriesMutex_;
std::atomic<EntryId> entryCount_ = {0};

FrameCallbackEntry* FindEntryById(EntryId id) {
    std::lock_guard<std::mutex> lock(entriesMutex_);
    auto found = entries_.find(id);
    return found == entries_.end() ? nullptr : found->second.get();
}

/*
 * Detaches the entry from the registry and hands back its owner, so the
 * destructor -- which calls into Java -- runs with the mutex released.
 */
std::unique_ptr<FrameCallbackEntry> TakeEntry(EntryId id) {
    std::lock_guard<std::mutex> lock(entriesMutex_);
    auto found = entries_.find(id);
    if (found == entries_.end()) {
        return nullptr;
    }
    auto owner = std::move(found->second);
    entries_.erase(found);
    return owner;
}

/*
 * Never throws: on the NDK path this runs inside a C callback in libandroid,
 * which a C++ exception may not unwind through. A JS exception the runtime
 * still owns is handed to Java the way Timers::FireTimer does -- the frame
 * dispatch is driven by the thread's looper, so a pending Java exception is
 * picked up when control returns to Looper.loop().
 */
void Dispatch(EntryId id, int64_t frameTimeNanos) {
    FrameCallbackEntry* entry = FindEntryById(id);
    if (entry == nullptr) {
        return;  // the entry was retired before this frame arrived
    }
    if (entry->ShouldRemoveBeforeCall()) {
        TakeEntry(id);
        return;
    }

    Isolate* isolate = entry->isolate_;
    Runtime* runtime = static_cast<Runtime*>(
            isolate->GetData((uint32_t) Runtime::IsolateData::RUNTIME));
    if (runtime == nullptr) {
        return;
    }

    Locker locker(isolate);
    Isolate::Scope isolateScope(isolate);
    HandleScope handleScope(isolate);

    Local<Function> cb = entry->callback_.Get(isolate);
    Local<Context> context = runtime->GetContext();
    Context::Scope contextScope(context);

    entry->MarkUnscheduled();

    Local<Value> args[2] = {
            Number::New(isolate, (double) frameTimeNanos),
            Number::New(isolate, Performance::MonotonicNanosToTimelineMillis(
                                         isolate, frameTimeNanos)),
    };

    TryCatch tc(isolate);

    cb->Call(context, context->Global(), 2, args);  // ignore JS return value

    // Re-resolve: the callback may have rescheduled or removed itself.
    entry = FindEntryById(id);
    if (entry != nullptr && entry->ShouldRemoveAfterCall()) {
        TakeEntry(id);
    }

    if (tc.HasCaught() &&
        !NativeScriptException::ContainUncaughtCallbackException(isolate, tc)) {
        NativeScriptException(tc).ReThrowToJava();
    }
}

void OnNativeFrame32(long ts, void* data) {
    Dispatch((EntryId) (uintptr_t) data, WidenFrameTimeNanos(ts));
}

void OnNativeFrame64(int64_t ts, void* data) {
    Dispatch((EntryId) (uintptr_t) data, ts);
}

void PostNative(EntryId id, long delayMillis) {
    ALooper_prepare(0);
    AChoreographer* instance = getInstance_();
    void* data = reinterpret_cast<void*>((uintptr_t) id);

    if (postFrameCallback64_ != nullptr && postFrameCallbackDelayed64_ != nullptr) {
        if (delayMillis > 0) {
            postFrameCallbackDelayed64_(instance, &OnNativeFrame64, data,
                                        (uint32_t) delayMillis);
        } else {
            postFrameCallback64_(instance, &OnNativeFrame64, data);
        }
        return;
    }

    if (delayMillis > 0) {
        postFrameCallbackDelayed_(instance, &OnNativeFrame32, data, delayMillis);
    } else {
        postFrameCallback_(instance, &OnNativeFrame32, data);
    }
}

void PostJava(FrameCallbackEntry* entry, long delayMillis) {
    JEnv env;
    ResolveFrameCallbacksClass(env);

    if (entry->javaCallback_ == nullptr) {
        JniLocalRef instance(env.NewObject(FRAME_CALLBACKS_CLASS, FRAME_CALLBACKS_CTOR,
                                           (jlong) entry->id_));
        entry->javaCallback_ = env.NewGlobalRef(instance);
    }

    env.CallVoidMethod(entry->javaCallback_, FRAME_CALLBACKS_POST,
                       (jlong) delayMillis);
}

void Post(FrameCallbackEntry* entry, long delayMillis) {
    if (UseNativeChoreographer()) {
        PostNative(entry->id_, delayMillis);
    } else {
        PostJava(entry, delayMillis);
    }
}

long ReadDelay(const FunctionCallbackInfo<Value>& args, Local<Context> context) {
    if (args.Length() < 2 || !args[1]->IsNumber()) {
        return 0;
    }
    return (long) args[1]->IntegerValue(context).FromMaybe(0);
}

EntryId ReadEntryId(Isolate* isolate, Local<Function> func, Local<Context> context) {
    Local<Value> id;
    if (!V8GetPrivateValue(isolate, func,
                           ArgConverter::ConvertToV8String(isolate,
                                                          "_postFrameCallbackId"),
                           id) ||
        !id->IsNumber()) {
        return 0;
    }
    return (EntryId) id->IntegerValue(context).FromMaybe(0);
}

}  // namespace

void FrameCallbacks::PostFrameCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Locker locker(isolate);
    Isolate::Scope isolateScope(isolate);
    HandleScope handleScope(isolate);
    Local<Context> context = isolate->GetCurrentContext();
    Context::Scope contextScope(context);

    if (args.Length() < 1 || !args[0]->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8Literal(
                isolate, "Frame callback argument is not a function")));
        return;
    }

    Local<Function> func = args[0].As<Function>();
    long delayMillis = ReadDelay(args, context);

    FrameCallbackEntry* existing = FindEntryById(ReadEntryId(isolate, func, context));
    if (existing != nullptr) {
        // Always mark, which also clears a pending removal; only post when it
        // is not already waiting on a frame.
        bool shouldPost = !existing->IsScheduled();
        existing->MarkScheduled();
        if (shouldPost) {
            Post(existing, delayMillis);
        }
        return;
    }

    EntryId id = ++entryCount_;
    V8SetPrivateValue(isolate, func,
                      ArgConverter::ConvertToV8String(isolate, "_postFrameCallbackId"),
                      Number::New(isolate, (double) id));

    FrameCallbackEntry* entry;
    {
        std::lock_guard<std::mutex> lock(entriesMutex_);
        auto inserted = entries_.emplace(
                id, std::make_unique<FrameCallbackEntry>(isolate, func, id));
        assert(inserted.second && "Frame callback ID should not be duplicated");
        entry = inserted.first->second.get();
    }

    entry->MarkScheduled();
    Post(entry, delayMillis);
}

void FrameCallbacks::RemoveFrameCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Locker locker(isolate);
    Isolate::Scope isolateScope(isolate);
    HandleScope handleScope(isolate);
    Local<Context> context = isolate->GetCurrentContext();
    Context::Scope contextScope(context);

    if (args.Length() < 1 || !args[0]->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8Literal(
                isolate, "Frame callback argument is not a function")));
        return;
    }

    FrameCallbackEntry* entry =
            FindEntryById(ReadEntryId(isolate, args[0].As<Function>(), context));
    if (entry != nullptr) {
        entry->MarkRemoved();
    }
}

void FrameCallbacks::RemoveIsolateEntries(Isolate* isolate) {
    // Detached first, destroyed after the mutex is released: the destructors
    // call into Java.
    std::vector<std::unique_ptr<FrameCallbackEntry>> doomed;
    {
        std::lock_guard<std::mutex> lock(entriesMutex_);
        for (auto it = entries_.begin(); it != entries_.end();) {
            if (it->second->isolate_ == isolate) {
                doomed.push_back(std::move(it->second));
                it = entries_.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void FrameCallbacks::Init(Isolate* isolate, Local<ObjectTemplate> globalTemplate) {
    globalTemplate->Set(
            ArgConverter::ConvertToV8String(isolate, "__postFrameCallback"),
            FunctionTemplate::New(isolate, PostFrameCallback));
    globalTemplate->Set(
            ArgConverter::ConvertToV8String(isolate, "__removeFrameCallback"),
            FunctionTemplate::New(isolate, RemoveFrameCallback));

#ifdef APPLICATION_IN_DEBUG
    /*
     * Test-only override, absent from release runtimes: the NDK path is the
     * only one a modern device would ever select, so the Java bridge would
     * otherwise ship without device coverage. Takes "auto", "native" or
     * "java" and returns the implementation subsequent posts will use.
     */
    globalTemplate->Set(
            ArgConverter::ConvertToV8String(isolate, "__setFrameCallbackImpl"),
            FunctionTemplate::New(
                    isolate, [](const FunctionCallbackInfo<Value>& args) {
                        Isolate* isolate = args.GetIsolate();
                        std::string requested =
                                args.Length() > 0 && args[0]->IsString()
                                        ? ArgConverter::ConvertToString(
                                                  args[0].As<String>())
                                        : "auto";
                        if (requested == "java") {
                            forcedImpl_ = Impl::kJava;
                        } else if (requested == "native") {
                            forcedImpl_ = Impl::kNative;
                        } else {
                            forcedImpl_ = Impl::kAuto;
                        }
                        args.GetReturnValue().Set(ArgConverter::ConvertToV8String(
                                isolate,
                                UseNativeChoreographer() ? "native" : "java"));
                    }));
#endif
}

}  // namespace tns

extern "C" JNIEXPORT void JNICALL Java_com_tns_FrameCallbacks_nativeDoFrame(
        JNIEnv* env, jclass clazz, jlong entryId, jlong frameTimeNanos) {
    try {
        tns::Dispatch((tns::EntryId) entryId, (int64_t) frameTimeNanos);
    } catch (tns::NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception& e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        tns::NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        tns::NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}

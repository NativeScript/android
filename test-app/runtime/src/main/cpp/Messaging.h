#ifndef MESSAGING_H_
#define MESSAGING_H_

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <string>

#include "v8.h"

namespace tns {

class EventLoop;

namespace serialization {
class SerializedValue;
}

namespace messaging {

class NativeMessagePort;
class SiblingGroup;

// What a port's group did with a message handed to it.
enum class DispatchResult {
    // Queued on at least one destination.
    kDelivered,
    // The group has no other member; the message is dropped.
    kNoDestination,
    // Nothing was queued and the caller must not treat the send as done: the
    // port is not entangled, or the message carries transferables and the group
    // has more than one destination. The out-parameter says which.
    kFailed,
};

// Everything about a port that is not tied to an isolate, so it can be moved
// into a message and adopted on the receiving side.
//
// `mutex_` is the only lock a producer on a foreign thread ever takes. Lock
// order across the whole subsystem is SiblingGroup's lock FIRST, a port's
// mutex_ second; never the reverse. Every path that needs both — dispatch,
// entangle, disentangle — is entered through the group.
class PortData {
public:
    explicit PortData(NativeMessagePort* owner);
    ~PortData();

    PortData(const PortData&) = delete;
    PortData& operator=(const PortData&) = delete;

    // The one cross-thread entry point. Appends `message` and wakes the owning
    // port while STILL holding the mutex, so a port detaching concurrently
    // either takes the mutex first and is never woken, or waits and observes the
    // queued message.
    void AddToIncomingQueue(std::shared_ptr<serialization::SerializedValue> message);

    // Hands `message` to every other member of this port's group.
    DispatchResult Dispatch(std::shared_ptr<serialization::SerializedValue> message,
                            std::string* error);

    // Connects the two ends of a fresh channel. Neither end may already belong
    // to a group.
    static void Entangle(PortData* a, PortData* b);

    // Leaves the group, queueing a close sentinel on this port and — for an
    // anonymous pair — on the sibling left behind. Once this returns, no other
    // thread can reach this object through the group. Owner thread only.
    void Disentangle();

private:
    friend class NativeMessagePort;
    friend class SiblingGroup;

    std::mutex mutex_;
    std::deque<std::shared_ptr<serialization::SerializedValue>> incoming_;
    NativeMessagePort* owner_ = nullptr;
    std::shared_ptr<SiblingGroup> group_;
};

// The isolate-bound half of a port: the JS wrapper, the delivery callout and
// the drain that runs on the owning runtime's event loop. Home-thread only,
// TriggerAsync excepted.
class NativeMessagePort : public std::enable_shared_from_this<NativeMessagePort> {
public:
    ~NativeMessagePort();

    NativeMessagePort(const NativeMessagePort&) = delete;
    NativeMessagePort& operator=(const NativeMessagePort&) = delete;

    // Creates a port and its JS wrapper. With `data` the port adopts an
    // in-flight port — the group travels with the data — and schedules a drain
    // of whatever queued up while it was in transit; with `group` it joins that
    // named group; with neither it is one unentangled end of a new channel.
    // Null with an exception pending when the wrapper or the JS tier could not
    // be built.
    static std::shared_ptr<NativeMessagePort> New(v8::Local<v8::Context> context,
                                                  std::unique_ptr<PortData> data = nullptr,
                                                  std::shared_ptr<SiblingGroup> group = nullptr);

    // Schedules a drain. Any thread; the caller must hold this port's data
    // mutex, which is what keeps the port from detaching underneath the post.
    void TriggerAsync();

    // HTML's port message queue enable/disable. Starting a port with a backlog
    // schedules a drain for it.
    void Start();
    void Stop();

    // Detaches the data, sentinels the sibling, drops the JS wrapper and fires
    // the tier's close event on it. Safe to call on an already-closed port, and
    // safe to call from inside that event.
    void Close();

    // Drops the data out of the port and out of its group, so nothing can reach
    // it any more. What the teardown sweep does to a port app code never closed.
    void OrphanData();

    // Pops one message regardless of whether the port was started
    // (receiveMessageOnPort). Just(false) when the queue holds nothing
    // deliverable, Just(true) with `out` set otherwise, Nothing when the value
    // could not be read.
    v8::Maybe<bool> ReceiveOne(v8::Local<v8::Context> context, v8::Local<v8::Value>* out);

    // The [[Detached]] internal slot.
    bool IsDetached() const {
        return this->data_ == nullptr;
    }

    // Moves the data into a message. The handle side closes, but the data keeps
    // its group membership and its queue: senders keep queueing into it while it
    // is in flight, and with no owner nothing is woken.
    std::unique_ptr<PortData> TransferForMessaging();

    // Empty once the port has been closed.
    v8::Local<v8::Object> Wrapper(v8::Isolate* isolate) const;

    PortData* Data() const {
        return this->data_.get();
    }

private:
    NativeMessagePort(v8::Isolate* isolate, v8::Local<v8::Object> wrapper);

    std::unique_ptr<PortData> Detach();
    void CloseHandle();
    void EmitClose(v8::Local<v8::Object> wrapper);
    void Drain();
    std::shared_ptr<serialization::SerializedValue> TakeMessage(bool force);
    bool Emit(v8::Local<v8::Context> context, v8::Local<v8::Object> receiver,
              v8::Local<v8::Function> emitMessage, v8::Local<v8::Value> data,
              v8::Local<v8::Value> ports, const char* type);

    std::unique_ptr<PortData> data_;
    bool receiving_ = false;
    // Set while a drain is queued, so a burst of messages costs one post.
    // Atomic because producers flip it from their own threads.
    std::atomic<bool> scheduled_{false};
    // Strong on purpose: a port and its JS wrapper stay alive until the port is
    // closed, which is the lifetime model HTML and Node specify — reachability
    // plays no part in it.
    v8::Global<v8::Object> wrapper_;
    // Only ever dereferenced through Runtime::TryGetRuntime, which answers null
    // once the runtime behind it is gone; the raw pointer is never touched
    // otherwise.
    v8::Isolate* isolate_ = nullptr;
    // Held by shared_ptr so a drain posted from a foreign thread can never race
    // the loop's own teardown.
    std::shared_ptr<EventLoop> loop_;
};

// The port behind a JS wrapper, or null when `object` is not a port wrapper or
// its port has been closed.
NativeMessagePort* PortFromWrapper(v8::Isolate* isolate, v8::Local<v8::Object> object);

// Whether `object` is a MessagePort wrapper at all, closed or not. The
// serializer needs the distinction: a closed port in a transfer list is a
// different error from a value that was never transferable.
bool IsPortWrapper(v8::Isolate* isolate, v8::Local<v8::Object> object);

// Adopts an in-flight port on this isolate and returns its fresh wrapper.
v8::MaybeLocal<v8::Object> AdoptPort(v8::Local<v8::Context> context,
                                     std::unique_ptr<PortData> data);

// The markAsUntransferable / markAsUncloneable brands. Both answer Just(false)
// without creating anything when this isolate has never stamped one.
v8::Maybe<bool> IsMarkedUntransferable(v8::Isolate* isolate, v8::Local<v8::Object> object);
v8::Maybe<bool> IsMarkedUncloneable(v8::Isolate* isolate, v8::Local<v8::Object> object);

// The markAsUncloneable brand itself, empty when this isolate has never
// stamped one. For the serializer, which is asked about every object in a
// claimed graph and hoists the lookup out of that loop.
v8::Local<v8::Private> UncloneableBrandIfAny(v8::Isolate* isolate);

// The natives behind the message-channel builtin: channel and port
// primitives, the two registration hooks the JS tier calls once per isolate,
// and the transfer brands.
v8::MaybeLocal<v8::Object> CreateBinding(v8::Local<v8::Context> context);

// The two builtins' exports with that binding attached. GetExports consults
// the factory only on the run that populates the cache, so every call site for
// these builtins must go through here — a site passing a different factory
// would win or lose by init order.
v8::MaybeLocal<v8::Object> GetMessageChannelExports(v8::Local<v8::Context> context);
v8::MaybeLocal<v8::Object> GetBroadcastChannelExports(v8::Local<v8::Context> context);

// Force-closes every port this isolate still owns: the data is orphaned and
// disentangled, so siblings on other isolates get their close sentinels and
// nothing can reach this isolate's ports afterwards. Must run after the event
// loop has stopped and while the isolate is still locked.
void CloseAllPorts(v8::Isolate* isolate);

}  // namespace messaging
}  // namespace tns

#endif /* MESSAGING_H_ */

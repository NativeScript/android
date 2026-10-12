#include "Messaging.h"

#include <algorithm>
#include <initializer_list>
#include <set>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "EventLoop.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "RuntimeState.h"
#include "StructuredSerialization.h"
#include "Util.h"
#include "WorkerWrapper.h"

using namespace v8;

namespace tns {
namespace messaging {

using Message = serialization::SerializedValue;

namespace {

// Per-isolate state. `livePorts` is the strong reference that keeps a port and
// its wrapper alive until it is closed; everything else is registered once by
// the JS tier or built on first use.
struct MessagingState {
    ~MessagingState();

    std::unordered_set<std::shared_ptr<NativeMessagePort>> livePorts;
    Global<FunctionTemplate> portTemplate;
    Global<v8::Function> emitMessage;
    // The tier's per-wrapper setup, read off the builtin's exports. A wrapper is
    // built from a template, so no JS constructor ever ran on it.
    Global<v8::Function> adoptPort;
    Global<Private> untransferableBrand;
    Global<Private> uncloneableBrand;
};

// Null once the runtime has started tearing down — callers bail rather than
// recreate state that would never be destroyed. The state outlives the
// force-close sweep: DestroyRuntime releases it in its very last statement,
// long after CloseAllPorts has run.
MessagingState* State(Isolate* isolate) {
    return RuntimeState::For<MessagingState>(isolate);
}

MessagingState::~MessagingState() {
    // Detach the set first so a port's teardown cannot mutate it mid-walk.
    std::unordered_set<std::shared_ptr<NativeMessagePort>> survivors =
            std::move(this->livePorts);
    this->livePorts.clear();
}

// Values set with setEnvironmentData, shared by every isolate in the process.
// Cloned on the way in and read back per isolate, so nothing but bytes is
// shared. Documented deviation from Node: a write after a worker spawned is
// visible to it, because there is no per-thread snapshot.
std::mutex g_environmentDataMutex;
std::unordered_map<std::string, std::shared_ptr<serialization::SerializedValue>>
        g_environmentData;

void IllegalConstructorCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    isolate->ThrowException(Exception::TypeError(
            ArgConverter::ConvertToV8String(isolate, "Illegal constructor")));
}

// The template every port wrapper is built from. It doubles as the brand: a
// wrapper is recognised by HasInstance, and the port itself lives in the one
// internal field.
Local<FunctionTemplate> PortTemplate(Isolate* isolate) {
    MessagingState* state = State(isolate);
    if (state == nullptr) {
        return Local<FunctionTemplate>();
    }
    if (state->portTemplate.IsEmpty()) {
        Local<FunctionTemplate> tmpl =
                FunctionTemplate::New(isolate, IllegalConstructorCallback);
        tmpl->SetClassName(ArgConverter::ConvertToV8String(isolate, "MessagePort"));
        tmpl->InstanceTemplate()->SetInternalFieldCount(1);
        state->portTemplate.Reset(isolate, tmpl);
    }
    return state->portTemplate.Get(isolate);
}

// A port can be created on an isolate that never touched MessagePort — a
// worker receiving a transferred one — so the builtin that registers the
// delivery function and exports the wrapper setup is run on demand rather than
// assumed.
bool EnsureJsTier(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    MessagingState* state = State(isolate);
    if (state == nullptr) {
        return false;
    }
    if (!state->emitMessage.IsEmpty() && !state->adoptPort.IsEmpty()) {
        return true;
    }
    Local<Object> exports;
    Local<Value> adopt;
    if (!GetMessageChannelExports(context).ToLocal(&exports) ||
        !exports->Get(context, ArgConverter::ConvertToV8String(isolate, "adoptPort"))
                 .ToLocal(&adopt)) {
        return false;
    }
    if (!adopt->IsFunction() || state->emitMessage.IsEmpty()) {
        return false;
    }
    state->adoptPort.Reset(isolate, adopt.As<v8::Function>());
    return true;
}

Local<Private> UntransferableBrand(Isolate* isolate, bool create) {
    MessagingState* state = State(isolate);
    if (state == nullptr) {
        return Local<Private>();
    }
    if (state->untransferableBrand.IsEmpty()) {
        if (!create) {
            return Local<Private>();
        }
        state->untransferableBrand.Reset(
                isolate, Private::New(isolate, ArgConverter::ConvertToV8String(
                                                       isolate, "messagingUntransferable")));
    }
    return state->untransferableBrand.Get(isolate);
}

Local<Private> UncloneableBrand(Isolate* isolate, bool create) {
    MessagingState* state = State(isolate);
    if (state == nullptr) {
        return Local<Private>();
    }
    if (state->uncloneableBrand.IsEmpty()) {
        if (!create) {
            return Local<Private>();
        }
        state->uncloneableBrand.Reset(
                isolate, Private::New(isolate, ArgConverter::ConvertToV8String(
                                                       isolate, "messagingUncloneable")));
    }
    return state->uncloneableBrand.Get(isolate);
}

// Private, not a plain Symbol: app code can neither discover a brand nor forge
// one onto a value the sender never marked.
void StampBrand(const FunctionCallbackInfo<Value>& info,
                Local<Private> (*brandFor)(Isolate*, bool)) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() < 1 || !info[0]->IsObject()) {
        return;
    }
    Local<Private> brand = brandFor(isolate, true);
    if (brand.IsEmpty()) {
        return;
    }
    (void)info[0].As<Object>()->SetPrivate(isolate->GetCurrentContext(), brand,
                                           v8::True(isolate));
}

}  // namespace

// The process-wide set of ports that can reach each other. An anonymous group
// is one channel's two ends; a named one is every BroadcastChannel sharing a
// name, across every isolate in the process.
class SiblingGroup final : public std::enable_shared_from_this<SiblingGroup> {
public:
    static std::shared_ptr<SiblingGroup> Get(const std::string& name);

    SiblingGroup() = default;
    explicit SiblingGroup(std::string name) : name_(std::move(name)), named_(true) {}
    ~SiblingGroup();

    SiblingGroup(const SiblingGroup&) = delete;
    SiblingGroup& operator=(const SiblingGroup&) = delete;

    DispatchResult Dispatch(PortData* source, std::shared_ptr<Message> message,
                            std::string* error);
    void Entangle(std::initializer_list<PortData*> ports);
    void Entangle(PortData* port);
    void Disentangle(PortData* data);

private:
    const std::string name_;
    // A BroadcastChannel group, whatever its name ("" included); an anonymous
    // group is one channel's two ends.
    const bool named_ = false;
    std::shared_mutex mutex_;
    std::set<PortData*> ports_;
};

namespace {

std::mutex g_groupsMutex;
std::unordered_map<std::string, std::weak_ptr<SiblingGroup>> g_groups;

}  // namespace

std::shared_ptr<SiblingGroup> SiblingGroup::Get(const std::string& name) {
    std::lock_guard<std::mutex> lock(g_groupsMutex);
    auto entry = g_groups.find(name);
    if (entry != g_groups.end()) {
        std::shared_ptr<SiblingGroup> existing = entry->second.lock();
        if (existing != nullptr) {
            return existing;
        }
    }
    std::shared_ptr<SiblingGroup> group = std::make_shared<SiblingGroup>(name);
    g_groups[name] = group;
    return group;
}

SiblingGroup::~SiblingGroup() {
    if (!this->named_) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_groupsMutex);
    auto entry = g_groups.find(this->name_);
    if (entry != g_groups.end() && entry->second.expired()) {
        g_groups.erase(entry);
    }
}

DispatchResult SiblingGroup::Dispatch(PortData* source, std::shared_ptr<Message> message,
                                      std::string* error) {
    std::shared_lock<std::shared_mutex> lock(this->mutex_);

    if (this->ports_.find(source) == this->ports_.end()) {
        if (error != nullptr) {
            *error = "Source MessagePort is not entangled with this group.";
        }
        return DispatchResult::kFailed;
    }
    if (this->ports_.size() <= 1) {
        return DispatchResult::kNoDestination;
    }
    // Nothing that can only be handed over once may fan out.
    if (this->ports_.size() > 2 && message->HasTransferables()) {
        if (error != nullptr) {
            *error = "Transferables cannot be used with multiple destinations.";
        }
        return DispatchResult::kFailed;
    }

    for (PortData* port : this->ports_) {
        if (port == source) {
            continue;
        }
        // Only reachable with a single destination, since a fan-out message can
        // carry no transferables at all.
        if (message->TransfersPort(port)) {
            if (error != nullptr) {
                *error =
                        "The target port was posted to itself, and the communication "
                        "channel was lost";
            }
            return DispatchResult::kDelivered;
        }
        // One message object shared by every destination: legal only because a
        // fan-out carries nothing that a destination could consume.
        port->AddToIncomingQueue(message);
    }
    return DispatchResult::kDelivered;
}

void SiblingGroup::Entangle(PortData* port) {
    this->Entangle({port});
}

void SiblingGroup::Entangle(std::initializer_list<PortData*> ports) {
    std::unique_lock<std::shared_mutex> lock(this->mutex_);
    for (PortData* data : ports) {
        this->ports_.insert(data);
        // group_ is written under the port's own mutex, which is what lets
        // PortData::Dispatch read it without racing a disentangle. Taken here in
        // the only legal order: this group's lock is already held.
        std::lock_guard<std::mutex> dataLock(data->mutex_);
        NS_CHECK(data->group_ == nullptr);
        data->group_ = this->shared_from_this();
    }
}

void SiblingGroup::Disentangle(PortData* data) {
    // Keeps the group alive past the last member dropping its reference.
    std::shared_ptr<SiblingGroup> self = this->shared_from_this();
    std::unique_lock<std::shared_mutex> lock(this->mutex_);
    this->ports_.erase(data);
    {
        std::lock_guard<std::mutex> dataLock(data->mutex_);
        data->group_.reset();
    }

    // Queued rather than delivered: a close orders behind everything already
    // sent, on both ends.
    data->AddToIncomingQueue(std::make_shared<Message>());
    if (this->ports_.size() == 1 && !this->named_) {
        // A channel with one end left is a channel no more; a named group outlives
        // any number of members joining and leaving.
        (*this->ports_.begin())->AddToIncomingQueue(std::make_shared<Message>());
    }
}

PortData::PortData(NativeMessagePort* owner) : owner_(owner) {}

PortData::~PortData() {
    NS_CHECK(this->owner_ == nullptr);
    this->Disentangle();
}

void PortData::AddToIncomingQueue(std::shared_ptr<Message> message) {
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->incoming_.push_back(std::move(message));
    if (this->owner_ != nullptr) {
        // Still holding the mutex: an owner read outside it could be detached by
        // the time the wake reaches it.
        this->owner_->TriggerAsync();
    }
}

DispatchResult PortData::Dispatch(std::shared_ptr<Message> message, std::string* error) {
    std::shared_ptr<SiblingGroup> group;
    {
        std::lock_guard<std::mutex> lock(this->mutex_);
        group = this->group_;
    }
    // The group's lock is taken with this port's mutex released: the two are
    // always acquired group first.
    if (group == nullptr) {
        if (error != nullptr) {
            *error = "MessagePortData is not entangled.";
        }
        return DispatchResult::kFailed;
    }
    return group->Dispatch(this, std::move(message), error);
}

void PortData::Entangle(PortData* a, PortData* b) {
    std::make_shared<SiblingGroup>()->Entangle({a, b});
}

void PortData::Disentangle() {
    std::shared_ptr<SiblingGroup> group;
    {
        std::lock_guard<std::mutex> lock(this->mutex_);
        group = this->group_;
    }
    if (group != nullptr) {
        group->Disentangle(this);
    }
}

NativeMessagePort::NativeMessagePort(Isolate* isolate, Local<Object> wrapper)
    : wrapper_(isolate, wrapper), isolate_(isolate) {
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime != nullptr) {
        this->loop_ = runtime->GetEventLoop();
    }
}

NativeMessagePort::~NativeMessagePort() {
    this->OrphanData();
}

std::shared_ptr<NativeMessagePort> NativeMessagePort::New(Local<Context> context,
                                                          std::unique_ptr<PortData> data,
                                                          std::shared_ptr<SiblingGroup> group) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    MessagingState* state = State(isolate);
    if (state == nullptr || !EnsureJsTier(context)) {
        return nullptr;
    }
    Local<FunctionTemplate> tmpl = PortTemplate(isolate);
    Local<Object> wrapper;
    if (tmpl.IsEmpty() || !tmpl->InstanceTemplate()->NewInstance(context).ToLocal(&wrapper)) {
        return nullptr;
    }

    std::shared_ptr<NativeMessagePort> port(new NativeMessagePort(isolate, wrapper));
    wrapper->SetAlignedPointerInInternalField(0, port.get(), v8::kEmbedderDataTypeTagDefault);
    state->livePorts.insert(port);

    if (data != nullptr) {
        port->data_ = std::move(data);
        std::lock_guard<std::mutex> lock(port->data_->mutex_);
        port->data_->owner_ = port.get();
        // Whatever queued up while the port was in flight drains on a later turn,
        // never inside the read that produced this port.
        port->TriggerAsync();
    } else {
        port->data_ = std::make_unique<PortData>(port.get());
        if (group != nullptr) {
            group->Entangle(port->data_.get());
        }
    }

    // The tier installs whatever a MessagePort instance needs before the wrapper
    // is handed out. A failure leaves a live channel behind, so take it down —
    // without the close event, which would dispatch on a wrapper that never
    // became a MessagePort.
    Local<Value> arg = wrapper;
    if (state->adoptPort.Get(isolate)->Call(context, v8::Undefined(isolate), 1, &arg).IsEmpty()) {
        port->OrphanData();
        port->CloseHandle();
        return nullptr;
    }
    return port;
}

void NativeMessagePort::TriggerAsync() {
    // The caller holds this port's data mutex, which is what makes "the port is
    // still owned" and "a drain is posted" one indivisible step against a
    // concurrent detach. Never takes the receiving isolate's Locker: the posted
    // entry runs under the home loop's own ceremony.
    if (this->loop_ == nullptr || this->scheduled_.exchange(true)) {
        return;
    }
    std::shared_ptr<NativeMessagePort> self = this->shared_from_this();
    // A dropped post (the loop already stopped) leaves scheduled_ set on
    // purpose: nothing will ever run on that loop again, and the flag keeps
    // producers from posting into it.
    this->loop_->PostInternal([self]() {
        if (Runtime::TryGetRuntime(self->isolate_) == nullptr) {
            return;
        }
        self->Drain();
    });
}

void NativeMessagePort::Start() {
    if (this->data_ == nullptr) {
        return;
    }
    this->receiving_ = true;
    std::lock_guard<std::mutex> lock(this->data_->mutex_);
    if (!this->data_->incoming_.empty()) {
        this->TriggerAsync();
    }
}

void NativeMessagePort::Stop() {
    this->receiving_ = false;
}

std::unique_ptr<PortData> NativeMessagePort::Detach() {
    // owner_ drops under the data mutex, so a producer either wakes this port
    // before the detach or never sees an owner at all. Node carries a separate
    // "closing" flag because libuv tears its handle down asynchronously; here
    // the detach IS the close, so a null data_ is the whole [[Detached]] state.
    std::lock_guard<std::mutex> lock(this->data_->mutex_);
    this->data_->owner_ = nullptr;
    return std::move(this->data_);
}

void NativeMessagePort::CloseHandle() {
    Isolate* isolate = this->isolate_;
    if (!this->wrapper_.IsEmpty()) {
        HandleScope handleScope(isolate);
        // The wrapper outlives the port whenever JS still holds it; clearing the
        // field is what makes PortFromWrapper report a closed port instead of
        // handing out a pointer to freed memory.
        this->wrapper_.Get(isolate)->SetAlignedPointerInInternalField(
                0, nullptr, v8::kEmbedderDataTypeTagDefault);
        this->wrapper_.Reset();
    }
    MessagingState* state = State(isolate);
    if (state != nullptr) {
        state->livePorts.erase(this->shared_from_this());
    }
}

void NativeMessagePort::Close() {
    // Keeps this object alive across the registry erase in CloseHandle.
    std::shared_ptr<NativeMessagePort> self = this->shared_from_this();
    if (this->wrapper_.IsEmpty() && this->data_ == nullptr) {
        return;
    }
    Isolate* isolate = this->isolate_;
    HandleScope handleScope(isolate);
    Local<Object> wrapper = this->Wrapper(isolate);

    std::unique_ptr<PortData> data;
    if (this->data_ != nullptr) {
        data = this->Detach();
    }
    this->CloseHandle();
    if (data != nullptr) {
        // Sequential, never nested: Detach released the data mutex before the
        // group's lock is taken here.
        data->Disentangle();
        data.reset();
    }
    // Last, on the wrapper the port has just let go of, so a listener finds an
    // already-detached port and a close() from inside one is a no-op rather than
    // a recursion.
    if (!wrapper.IsEmpty()) {
        this->EmitClose(wrapper);
    }
}

void NativeMessagePort::EmitClose(Local<Object> wrapper) {
    Isolate* isolate = this->isolate_;
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return;
    }
    Local<Context> context = runtime->GetContext();
    if (context.IsEmpty()) {
        return;
    }
    Context::Scope contextScope(context);
    if (!EnsureJsTier(context)) {
        return;
    }
    Local<Value> undefined = v8::Undefined(isolate);
    this->Emit(context, wrapper, State(isolate)->emitMessage.Get(isolate), undefined, undefined,
               "close");
}

std::unique_ptr<PortData> NativeMessagePort::TransferForMessaging() {
    std::shared_ptr<NativeMessagePort> self = this->shared_from_this();
    std::unique_ptr<PortData> data = this->Detach();
    // Deliberately not disentangled: the group membership and the queue are
    // exactly what the receiving port adopts, and senders keep queueing into the
    // data while it is in flight.
    this->CloseHandle();
    return data;
}

void NativeMessagePort::OrphanData() {
    if (this->data_ == nullptr) {
        return;
    }
    std::unique_ptr<PortData> data = this->Detach();
    data->Disentangle();
}

Local<Object> NativeMessagePort::Wrapper(Isolate* isolate) const {
    if (this->wrapper_.IsEmpty()) {
        return Local<Object>();
    }
    return this->wrapper_.Get(isolate);
}

std::shared_ptr<Message> NativeMessagePort::TakeMessage(bool force) {
    std::lock_guard<std::mutex> lock(this->data_->mutex_);
    if (this->data_->incoming_.empty()) {
        return nullptr;
    }
    // A port that was never started still learns that its sibling died: the
    // close sentinel is honoured with the message queue disabled.
    if (!this->receiving_ && !force && !this->data_->incoming_.front()->IsCloseMessage()) {
        return nullptr;
    }
    std::shared_ptr<Message> message = std::move(this->data_->incoming_.front());
    this->data_->incoming_.pop_front();
    return message;
}

Maybe<bool> NativeMessagePort::ReceiveOne(Local<Context> context, Local<Value>* out) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    std::shared_ptr<Message> received = this->TakeMessage(true);
    if (received == nullptr) {
        return Just(false);
    }
    if (received->IsCloseMessage()) {
        this->Close();
        return Just(false);
    }
    return received->Deserialize(isolate, context).ToLocal(out) ? Just(true) : Nothing<bool>();
}

bool NativeMessagePort::Emit(Local<Context> context, Local<Object> receiver,
                             Local<v8::Function> emitMessage, Local<Value> data,
                             Local<Value> ports, const char* type) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    if (receiver.IsEmpty()) {
        return false;
    }
    Local<Value> argv[] = {data, ports, ArgConverter::ConvertToV8String(isolate, type)};
    TryCatch tc(isolate);
    if (!emitMessage->Call(context, receiver, 3, argv).IsEmpty()) {
        return true;
    }
    if (tc.HasTerminated() || !tc.CanContinue()) {
        return false;
    }
    // There is no event-loop frame to unwind into, so a listener that throws is
    // an uncaught error, reported where a timer callback's would be.
    if (!NativeScriptException::ContainUncaughtCallbackException(isolate, tc)) {
        if (EventLoop::IsPumping()) {
            // A pump keeps making JNI calls after this returns, so arming a
            // pending Java exception here is illegal; the loop reports it from
            // its next token dispatch instead.
            Runtime* runtime = Runtime::TryGetRuntime(isolate);
            std::shared_ptr<EventLoop> loop =
                    runtime == nullptr ? nullptr : runtime->GetEventLoop();
            if (loop != nullptr) {
                loop->DeferJavaThrow(std::make_shared<NativeScriptException>(tc));
            }
        } else {
            NativeScriptException(tc).ReThrowToJava();
        }
    }
    return false;
}

void NativeMessagePort::Drain() {
    // Cleared first: a message arriving from here on must schedule a fresh
    // drain rather than be left for this one, which may already be past its
    // queue read.
    this->scheduled_.store(false);
    if (this->data_ == nullptr) {
        return;
    }
    Isolate* isolate = this->isolate_;
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return;
    }
    HandleScope handleScope(isolate);
    Local<Context> context = runtime->GetContext();
    if (context.IsEmpty()) {
        return;
    }
    Context::Scope contextScope(context);
    if (!EnsureJsTier(context)) {
        return;
    }
    MessagingState* state = State(isolate);
    if (state == nullptr || state->emitMessage.IsEmpty()) {
        return;
    }
    Local<v8::Function> emitMessage = state->emitMessage.Get(isolate);
    Local<Object> wrapper = this->Wrapper(isolate);

    size_t budget;
    {
        std::lock_guard<std::mutex> lock(this->data_->mutex_);
        budget = std::max(this->data_->incoming_.size(), static_cast<size_t>(1000));
    }

    bool reschedule = false;
    // data_ is written only on this thread, but the callout below can transfer
    // or close this very port, so it is re-checked every iteration.
    while (this->data_ != nullptr) {
        if (budget-- == 0) {
            // Only messages that arrived after this drain began are deferred: the
            // budget is a floor, not a cap, so the backlog present at the trigger
            // always drains in one turn (Node's processing_limit semantics). The
            // repost carries the late arrivals.
            reschedule = true;
            break;
        }
        HandleScope messageScope(isolate);
        std::shared_ptr<Message> received = this->TakeMessage(false);
        if (received == nullptr) {
            break;
        }
        if (received->IsCloseMessage()) {
            this->Close();
            return;
        }

        Local<Value> payload;
        Local<Value> ports = v8::Undefined(isolate);
        bool read;
        {
            // Failures reading the value are the port's 'messageerror' event, not
            // the isolate's uncaught-error path. Never holds the data mutex: the
            // read runs arbitrary JS.
            TryCatch tc(isolate);
            read = received->Deserialize(isolate, context, &ports).ToLocal(&payload);
            if (!read) {
                if (tc.HasTerminated() || !tc.CanContinue()) {
                    return;
                }
                payload = tc.HasCaught() ? tc.Exception() : v8::Undefined(isolate).As<Value>();
                tc.Reset();
            }
        }
        if (!read) {
            this->Emit(context, wrapper, emitMessage, payload, v8::Undefined(isolate),
                       "messageerror");
            reschedule = true;
            break;
        }
        if (!this->Emit(context, wrapper, emitMessage, payload, ports, "message")) {
            reschedule = true;
            break;
        }
        // Per message, not per drain: a handler's microtasks run before the next
        // message arrives, which is what both browsers and Node observe.
        isolate->PerformMicrotaskCheckpoint();
    }

    if (reschedule && this->data_ != nullptr) {
        std::lock_guard<std::mutex> lock(this->data_->mutex_);
        this->TriggerAsync();
    }
}

NativeMessagePort* PortFromWrapper(Isolate* isolate, Local<Object> object) {
    if (!IsPortWrapper(isolate, object)) {
        return nullptr;
    }
    return static_cast<NativeMessagePort*>(
            object->GetAlignedPointerFromInternalField(0, v8::kEmbedderDataTypeTagDefault));
}

bool IsPortWrapper(Isolate* isolate, Local<Object> object) {
    MessagingState* state = State(isolate);
    if (state == nullptr || state->portTemplate.IsEmpty()) {
        return false;
    }
    return state->portTemplate.Get(isolate)->HasInstance(object);
}

MaybeLocal<Object> AdoptPort(Local<Context> context, std::unique_ptr<PortData> data) {
    std::shared_ptr<NativeMessagePort> port = NativeMessagePort::New(context, std::move(data));
    if (port == nullptr) {
        return MaybeLocal<Object>();
    }
    return port->Wrapper(v8::Isolate::GetCurrent());
}

Maybe<bool> IsMarkedUntransferable(Isolate* isolate, Local<Object> object) {
    Local<Private> brand = UntransferableBrand(isolate, false);
    if (brand.IsEmpty()) {
        return Just(false);
    }
    return object->HasPrivate(isolate->GetCurrentContext(), brand);
}

Maybe<bool> IsMarkedUncloneable(Isolate* isolate, Local<Object> object) {
    Local<Private> brand = UncloneableBrand(isolate, false);
    if (brand.IsEmpty()) {
        return Just(false);
    }
    return object->HasPrivate(isolate->GetCurrentContext(), brand);
}

Local<Private> UncloneableBrandIfAny(Isolate* isolate) {
    return UncloneableBrand(isolate, false);
}

void CloseAllPorts(Isolate* isolate) {
    MessagingState* state = State(isolate);
    if (state == nullptr) {
        return;
    }
    // Orphaning every port's data drops the owner — so nothing can be woken on a
    // loop that has stopped — and takes the data out of its group, which both
    // sentinels the siblings on other isolates and puts the data beyond the
    // reach of their sender threads. The ports themselves die with this
    // runtime's state, by which time their data is inert.
    for (const std::shared_ptr<NativeMessagePort>& port : state->livePorts) {
        port->OrphanData();
    }
}

namespace {

// The wrapper argument, or false after throwing. A closed port passes: its
// wrapper is still a MessagePort, and every native here tolerates one.
bool PortArg(const FunctionCallbackInfo<Value>& info, int index, Local<Object>* wrapper) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() <= index || !info[index]->IsObject() ||
        !IsPortWrapper(isolate, info[index].As<Object>())) {
        isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(
                isolate, "The \"port\" argument must be a MessagePort instance")));
        return false;
    }
    *wrapper = info[index].As<Object>();
    return true;
}

void CreateChannelCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    std::shared_ptr<NativeMessagePort> port1 = NativeMessagePort::New(context);
    if (port1 == nullptr) {
        return;
    }
    std::shared_ptr<NativeMessagePort> port2 = NativeMessagePort::New(context);
    if (port2 == nullptr) {
        port1->Close();
        return;
    }
    PortData::Entangle(port1->Data(), port2->Data());

    Local<v8::Array> pair = v8::Array::New(isolate, 2);
    if (!pair->Set(context, 0, port1->Wrapper(isolate)).FromMaybe(false) ||
        !pair->Set(context, 1, port2->Wrapper(isolate)).FromMaybe(false)) {
        return;
    }
    info.GetReturnValue().Set(pair);
}

void CreateBroadcastPortCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    if (info.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(
                isolate, "The \"name\" argument must be a string")));
        return;
    }
    std::shared_ptr<NativeMessagePort> port = NativeMessagePort::New(
            context, nullptr, SiblingGroup::Get(ArgConverter::ToString(isolate, info[0])));
    if (port == nullptr) {
        return;
    }
    // A BroadcastChannel has no port-enable step: it receives from the moment it
    // exists.
    port->Start();
    info.GetReturnValue().Set(port->Wrapper(isolate));
}

void PostMessageCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Object> wrapper;
    if (!PortArg(info, 0, &wrapper)) {
        return;
    }
    Local<Context> context = isolate->GetCurrentContext();
    Local<Value> value = info.Length() > 1 ? info[1] : v8::Undefined(isolate).As<Value>();
    Local<Value> transferList = info.Length() > 2 ? info[2] : v8::Undefined(isolate).As<Value>();

    // Serialization runs even for a port that can no longer deliver: the
    // transfer list's side effects, and its errors, do not depend on delivery.
    std::shared_ptr<Message> message = std::make_shared<Message>();
    if (message->Serialize(isolate, context, value, transferList,
                           serialization::HostObjectPolicy::kReject, wrapper)
                .IsNothing()) {
        return;
    }
    // Re-read: serializing runs user getters, which may have closed the port.
    NativeMessagePort* port = PortFromWrapper(isolate, wrapper);
    if (port == nullptr || port->IsDetached()) {
        return;
    }

    std::string error;
    port->Data()->Dispatch(std::move(message), &error);
    if (!error.empty()) {
        DEBUG_WRITE_FORCE("MessagePort: %s", error.c_str());
    }
}

void StartCallback(const FunctionCallbackInfo<Value>& info) {
    Local<Object> wrapper;
    if (!PortArg(info, 0, &wrapper)) {
        return;
    }
    NativeMessagePort* port = PortFromWrapper(info.GetIsolate(), wrapper);
    if (port != nullptr) {
        port->Start();
    }
}

void StopCallback(const FunctionCallbackInfo<Value>& info) {
    Local<Object> wrapper;
    if (!PortArg(info, 0, &wrapper)) {
        return;
    }
    NativeMessagePort* port = PortFromWrapper(info.GetIsolate(), wrapper);
    if (port != nullptr) {
        port->Stop();
    }
}

void CloseCallback(const FunctionCallbackInfo<Value>& info) {
    Local<Object> wrapper;
    if (!PortArg(info, 0, &wrapper)) {
        return;
    }
    NativeMessagePort* port = PortFromWrapper(info.GetIsolate(), wrapper);
    if (port != nullptr) {
        // The keepalive outlives the registry erase inside Close.
        std::shared_ptr<NativeMessagePort> self = port->shared_from_this();
        self->Close();
    }
}

void DrainOneCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Object> wrapper;
    if (!PortArg(info, 0, &wrapper)) {
        return;
    }
    // Null, not a sentinel: the box is what says a message was there at all, so
    // a message whose value is undefined stays distinguishable from none.
    info.GetReturnValue().SetNull();
    NativeMessagePort* port = PortFromWrapper(isolate, wrapper);
    if (port == nullptr || port->IsDetached()) {
        return;
    }
    Local<Context> context = isolate->GetCurrentContext();
    std::shared_ptr<NativeMessagePort> self = port->shared_from_this();
    Local<Value> message;
    bool received = false;
    if (!self->ReceiveOne(context, &message).To(&received) || !received) {
        return;
    }
    Local<Object> box = Object::New(isolate);
    if (box->Set(context, ArgConverter::ConvertToV8String(isolate, "message"), message)
                .FromMaybe(false)) {
        info.GetReturnValue().Set(box);
    }
}

void IsDetachedCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Object> wrapper;
    if (!PortArg(info, 0, &wrapper)) {
        return;
    }
    NativeMessagePort* port = PortFromWrapper(isolate, wrapper);
    info.GetReturnValue().Set(port == nullptr || port->IsDetached());
}

void SetEmitMessageCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    MessagingState* state = State(isolate);
    if (state == nullptr || info.Length() < 1 || !info[0]->IsFunction()) {
        return;
    }
    state->emitMessage.Reset(isolate, info[0].As<v8::Function>());
}

void SetEnvironmentDataCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() < 1) {
        return;
    }
    Local<Context> context = isolate->GetCurrentContext();
    std::string key = ArgConverter::ToString(isolate, info[0]);
    if (info.Length() < 2 || info[1]->IsUndefined()) {
        std::lock_guard<std::mutex> lock(g_environmentDataMutex);
        g_environmentData.erase(key);
        return;
    }
    // Cloned on the way in, so a later mutation of the value the caller kept is
    // not visible to the threads that read it.
    auto stored = std::make_shared<serialization::SerializedValue>();
    if (stored->Serialize(isolate, context, info[1], v8::Undefined(isolate),
                          serialization::HostObjectPolicy::kReject)
                .IsNothing()) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_environmentDataMutex);
    g_environmentData[key] = std::move(stored);
}

void GetEnvironmentDataCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() < 1) {
        return;
    }
    std::string key = ArgConverter::ToString(isolate, info[0]);
    std::shared_ptr<serialization::SerializedValue> stored;
    {
        std::lock_guard<std::mutex> lock(g_environmentDataMutex);
        auto entry = g_environmentData.find(key);
        if (entry == g_environmentData.end()) {
            return;
        }
        stored = entry->second;
    }
    // Read back outside the lock: the read runs JS, and a value stored without a
    // transfer list can be read any number of times, on any isolate.
    Local<Value> value;
    if (stored->Deserialize(isolate, isolate->GetCurrentContext()).ToLocal(&value)) {
        info.GetReturnValue().Set(value);
    }
}

void MarkAsUntransferableCallback(const FunctionCallbackInfo<Value>& info) {
    StampBrand(info, UntransferableBrand);
}

void MarkAsUncloneableCallback(const FunctionCallbackInfo<Value>& info) {
    StampBrand(info, UncloneableBrand);
}

void IsMarkedAsUntransferableCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() < 1 || !info[0]->IsObject()) {
        info.GetReturnValue().Set(false);
        return;
    }
    bool marked = false;
    if (IsMarkedUntransferable(isolate, info[0].As<Object>()).To(&marked)) {
        info.GetReturnValue().Set(marked);
    }
}

}  // namespace

MaybeLocal<Object> CreateBinding(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    if (State(isolate) == nullptr) {
        return MaybeLocal<Object>();
    }
    Local<Object> binding = Object::New(isolate);

    // Constant for the lifetime of the isolate, so they are values rather than
    // calls. Node numbers the main thread 0; this runtime numbers its workers
    // from 1 and leaves the main runtime's own id unset.
    WorkerWrapper* worker = WorkerWrapper::FromIsolate(isolate);
    bool isWorker = worker != nullptr;
    if (!binding
                 ->Set(context, ArgConverter::ConvertToV8String(isolate, "isMainThread"),
                       v8::Boolean::New(isolate, !isWorker))
                 .FromMaybe(false) ||
        !binding
                 ->Set(context, ArgConverter::ConvertToV8String(isolate, "threadId"),
                       v8::Integer::New(isolate, isWorker ? worker->WorkerId() : 0))
                 .FromMaybe(false)) {
        return MaybeLocal<Object>();
    }

    tns::SetMethod(context, binding, "createChannel", CreateChannelCallback);
    tns::SetMethod(context, binding, "createBroadcastPort", CreateBroadcastPortCallback);
    tns::SetMethod(context, binding, "postMessage", PostMessageCallback);
    tns::SetMethod(context, binding, "start", StartCallback);
    tns::SetMethod(context, binding, "stop", StopCallback);
    tns::SetMethod(context, binding, "close", CloseCallback);
    tns::SetMethod(context, binding, "drainOne", DrainOneCallback);
    tns::SetMethodNoSideEffect(context, binding, "isDetached", IsDetachedCallback);
    tns::SetMethod(context, binding, "setEmitMessage", SetEmitMessageCallback);
    tns::SetMethod(context, binding, "setEnvironmentData", SetEnvironmentDataCallback);
    tns::SetMethod(context, binding, "getEnvironmentData", GetEnvironmentDataCallback);
    tns::SetMethod(context, binding, "markAsUntransferable", MarkAsUntransferableCallback);
    tns::SetMethodNoSideEffect(context, binding, "isMarkedAsUntransferable",
                               IsMarkedAsUntransferableCallback);
    tns::SetMethod(context, binding, "markAsUncloneable", MarkAsUncloneableCallback);
    return binding;
}

MaybeLocal<Object> GetMessageChannelExports(Local<Context> context) {
    return BuiltinLoader::GetExports(context, BuiltinId::kMessageChannel, CreateBinding);
}

MaybeLocal<Object> GetBroadcastChannelExports(Local<Context> context) {
    return BuiltinLoader::GetExports(context, BuiltinId::kBroadcastChannel, CreateBinding);
}

}  // namespace messaging
}  // namespace tns

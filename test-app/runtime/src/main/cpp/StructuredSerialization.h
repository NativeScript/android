#ifndef STRUCTUREDSERIALIZATION_H_
#define STRUCTUREDSERIALIZATION_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "Messaging.h"
#include "v8.h"

namespace tns {
namespace serialization {

/*
 * What an entry point does with an object backed by native state — a Java
 * proxy, an interop wrapper. The two callers deliberately disagree, and this
 * enum is the only place that disagreement is encoded.
 */
enum class HostObjectPolicy {
    // structuredClone: a DataCloneError, as the HTML spec requires. A clone
    // whose native half was left behind would be a wrapper around nothing.
    kReject,
    // Worker postMessage: the value arrives as an empty object. This is what
    // the runtime has always shipped and what the cross-runtime worker suite
    // asserts; moving it to kReject is a breaking change both runtimes have to
    // make together.
    kDegrade,
};

/*
 * Throws a "DataCloneError" DOMException, the same class the JS half of
 * structuredClone raises. Falls back to a "DataCloneError"-named Error when
 * the dom-exception builtin cannot run.
 */
void ThrowDataCloneError(v8::Isolate* isolate, const std::string& message);

/*
 * The dom-exception builtin's native half: `markCloneable`, which stamps a
 * per-isolate private brand on every instance the constructor makes. The brand
 * is what the serialization delegates answer IsHostObject from, so DOMException
 * travels through structuredClone and worker postMessage (Web IDL
 * [Serializable]). Lives here, next to those delegates.
 */
v8::MaybeLocal<v8::Object> DomExceptionBinding(v8::Local<v8::Context> context);

/*
 * The dom-exception builtin's exports with its binding attached. GetExports
 * consults the factory only on the run that populates the cache, so every call
 * site for this builtin must go through here — a site passing a different
 * factory would win or lose by init order.
 */
v8::MaybeLocal<v8::Object> GetDomExceptionExports(v8::Local<v8::Context> context);

/*
 * A value serialized out of one isolate, plus the memory that travels with it.
 * Serializing and deserializing are separate halves because a worker message
 * is read back on a different isolate than it was written on, while
 * structuredClone round-trips on a single one.
 */
class SerializedValue {
   public:
    SerializedValue() = default;
    SerializedValue(SerializedValue&&) = default;
    SerializedValue& operator=(SerializedValue&&) = default;
    SerializedValue(const SerializedValue&) = delete;
    SerializedValue& operator=(const SerializedValue&) = delete;

    /*
     * Serializes `input`, moving out of this isolate every ArrayBuffer and
     * every MessagePort named by `transferList` (an Array, or undefined/null
     * for none). `sourcePort` is the port a message is being posted on, which
     * the spec forbids transferring with its own message. Returns Nothing with
     * an exception pending: a TypeError when the transfer list is not an
     * Array, a DataCloneError for anything wrong with its entries or with the
     * value.
     */
    v8::Maybe<bool> Serialize(
            v8::Isolate* isolate, v8::Local<v8::Context> context,
            v8::Local<v8::Value> input, v8::Local<v8::Value> transferList,
            HostObjectPolicy hostObjectPolicy,
            v8::Local<v8::Object> sourcePort = v8::Local<v8::Object>());

    /*
     * Reads the value back into `context`, filling `portList` (when given)
     * with the wrappers of the ports the message transferred. A value carrying
     * anything transferred can be read exactly once — the memory and the ports
     * change hands; one carrying only clones may be read any number of times,
     * which is what lets a BroadcastChannel fan one message out.
     */
    v8::MaybeLocal<v8::Value> Deserialize(
            v8::Isolate* isolate, v8::Local<v8::Context> context,
            v8::Local<v8::Value>* portList = nullptr);

    /*
     * The close sentinel a sibling group queues when a channel goes away: a
     * message with no payload at all.
     */
    bool IsCloseMessage() const { return this->buffer_ == nullptr; }

    /*
     * Whether anything in here can only be handed over once, which is what
     * makes a message undeliverable to more than one destination.
     */
    bool HasTransferables() const {
        return !this->transferredBuffers_.empty() || !this->transferredPorts_.empty();
    }

    /*
     * Whether `data` is one of the ports this message carries — a message
     * transferring its own destination destroys the channel it travels on.
     */
    bool TransfersPort(const messaging::PortData* data) const;

    /*
     * Web IDL's DOMException serialization steps (name, message) plus the
     * stack, matching Node. Kept out-of-band because V8 forbids JS while a
     * value is being read: Deserialize constructs every instance up front and
     * ReadHostObject only hands them out by index (Node's host_objects_
     * design).
     */
    struct DomExceptionPayload {
        std::string name;
        std::string message;
        std::string stack;
        bool hasStack = false;
    };

   private:
    struct FreeDeleter {
        void operator()(void* pointer) const { std::free(pointer); }
    };

    // The serializer grows this with realloc() through its delegate's default
    // allocator, so it is free()d rather than deleted.
    std::unique_ptr<uint8_t, FreeDeleter> buffer_;
    size_t bufferSize_ = 0;
    // Backing stores moved out of the sending isolate. Each is re-wrapped in a
    // fresh ArrayBuffer under the same transfer id on the receiving side.
    std::vector<std::shared_ptr<v8::BackingStore>> transferredBuffers_;
    // Backing stores shared with — not moved from — the sending isolate.
    std::vector<std::shared_ptr<v8::BackingStore>> sharedBuffers_;
    // Ports moved out of the sending isolate, in transfer-list order: the wire
    // carries the index, the port itself travels here. Each keeps its group and
    // its queue, so senders can go on queueing into it while it is in flight.
    std::vector<std::unique_ptr<messaging::PortData>> transferredPorts_;
    // Set by the first read of a message that had something to hand over, so a
    // second read is caught rather than handing out emptied slots.
    bool consumed_ = false;
    // One entry per distinct DOMException in the graph, in write order (a
    // repeated reference is an object id in the stream, not a second entry).
    std::vector<DomExceptionPayload> domExceptions_;
};

}  // namespace serialization
}  // namespace tns

#endif /* STRUCTUREDSERIALIZATION_H_ */

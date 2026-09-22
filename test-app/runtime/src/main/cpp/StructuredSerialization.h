#ifndef STRUCTUREDSERIALIZATION_H_
#define STRUCTUREDSERIALIZATION_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

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
     * Serializes `input`, moving out of this isolate every ArrayBuffer named
     * by `transferList` (an Array, or undefined/null for none). Returns
     * Nothing with an exception pending: a TypeError when the transfer list is
     * not an Array, a DataCloneError for anything wrong with its entries or
     * with the value.
     */
    v8::Maybe<bool> Serialize(v8::Isolate* isolate,
                              v8::Local<v8::Context> context,
                              v8::Local<v8::Value> input,
                              v8::Local<v8::Value> transferList,
                              HostObjectPolicy hostObjectPolicy);

    /*
     * Reads the value back into `context`. Transferred buffers are consumed,
     * so this runs once per serialized value.
     */
    v8::MaybeLocal<v8::Value> Deserialize(v8::Isolate* isolate,
                                          v8::Local<v8::Context> context);

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
    // One entry per distinct DOMException in the graph, in write order (a
    // repeated reference is an object id in the stream, not a second entry).
    std::vector<DomExceptionPayload> domExceptions_;
};

}  // namespace serialization
}  // namespace tns

#endif /* STRUCTUREDSERIALIZATION_H_ */

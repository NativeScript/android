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
 * Throws the runtime's DataCloneError. There is no DOMException here, so it is
 * an Error carrying that name — the shape the shared cross-runtime suite
 * detects clone failures by.
 */
void ThrowDataCloneError(v8::Isolate* isolate, const std::string& message);

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
};

}  // namespace serialization
}  // namespace tns

#endif /* STRUCTUREDSERIALIZATION_H_ */

#ifndef WORKERMESSAGE_H_
#define WORKERMESSAGE_H_

#include <cstdlib>
#include <memory>
#include <vector>
#include "v8.h"

namespace tns {

template <typename T>
inline T* Malloc(size_t n) {
  return static_cast<T*>(malloc(n));
}

template <typename T>
T* UncheckedRealloc(T* pointer, size_t n) {
  size_t full_size = sizeof(T) * n;

  if (full_size == 0) {
    free(pointer);
    return nullptr;
  }

  void* allocated = realloc(pointer, full_size);

  return static_cast<T*>(allocated);
}

template <typename T>
struct MallocedBuffer {
  T* data;
  size_t size;

  T* release() {
    T* ret = data;
    data = nullptr;
    return ret;
  }

  void Truncate(size_t new_size) {
    size = new_size;
  }

  void Realloc(size_t new_size) {
    Truncate(new_size);
    data = UncheckedRealloc(data, new_size);
  }

  bool is_empty() const { return data == nullptr; }

  MallocedBuffer() : data(nullptr), size(0) {}
  explicit MallocedBuffer(size_t size) : data(Malloc<T>(size)), size(size) {}
  MallocedBuffer(T* data, size_t size) : data(data), size(size) {}
  MallocedBuffer(MallocedBuffer&& other) : data(other.data), size(other.size) {
    other.data = nullptr;
  }
  MallocedBuffer& operator=(MallocedBuffer&& other) {
    this->~MallocedBuffer();
    return *new (this) MallocedBuffer(std::move(other));
  }
  ~MallocedBuffer() { free(data); }
  MallocedBuffer(const MallocedBuffer&) = delete;
  MallocedBuffer& operator=(const MallocedBuffer&) = delete;
};

namespace worker {

/*
 * A structured-clone payload that can cross isolate/thread boundaries.
 * Serialized with v8::ValueSerializer on the sending isolate and
 * deserialized with v8::ValueDeserializer on the receiving one.
 * SharedArrayBuffer contents are shared via their backing stores.
 */
class Message {
 public:
  Message(MallocedBuffer<char>&& payload = MallocedBuffer<char>());
  Message(Message&& other) = default;
  Message& operator=(Message&& other) = default;
  Message& operator=(const Message&) = delete;
  Message(const Message&) = delete;

  v8::Maybe<bool> Serialize(v8::Isolate* isolate,
                            v8::Local<v8::Context> context,
                            v8::Local<v8::Value> input);
  v8::MaybeLocal<v8::Value> Deserialize(v8::Isolate* isolate,
                                        v8::Local<v8::Context> context);

  // Called when a new SharedArrayBuffer object is encountered in the
  // incoming value's structure.
  void AddSharedArrayBuffer(std::shared_ptr<v8::BackingStore> backing_store);

 private:
  MallocedBuffer<char> main_message_buf_;
  std::vector<std::shared_ptr<v8::BackingStore>> array_buffers_;
  std::vector<std::shared_ptr<v8::BackingStore>> shared_array_buffers_;
};

}  // namespace worker
}  // namespace tns

#endif /* WORKERMESSAGE_H_ */

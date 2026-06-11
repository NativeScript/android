#include "WorkerMessage.h"

#include <cassert>

#include "ArgConverter.h"

using namespace v8;

namespace tns {
namespace worker {
namespace {

void ThrowDataCloneException(Local<Context> context, Local<v8::String> message) {
    Isolate* isolate = context->GetIsolate();
    std::string msg = "DataCloneError: " + ArgConverter::ConvertToString(message);
    isolate->ThrowException(
            Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
}

class SerializerDelegate : public ValueSerializer::Delegate {
public:
    SerializerDelegate(Isolate* isolate, Local<Context> context, Message* m)
            : isolate_(isolate), context_(context), msg_(m) {}

    void ThrowDataCloneError(Local<v8::String> message) override {
        ThrowDataCloneException(context_, message);
    }

    Maybe<bool> WriteHostObject(Isolate* isolate, Local<Object> object) override {
        // Host objects (e.g. Java proxies) carry no transferable native state;
        // they are recreated as plain objects on the receiving side.
        return Just(true);
    }

    Maybe<uint32_t> GetSharedArrayBufferId(
            Isolate* isolate, Local<SharedArrayBuffer> shared_array_buffer) override {
        uint32_t i;
        for (i = 0; i < seen_shared_array_buffers_.size(); ++i) {
            if (seen_shared_array_buffers_[i].Get(isolate) == shared_array_buffer) {
                return Just(i);
            }
        }

        seen_shared_array_buffers_.emplace_back(
                Global<SharedArrayBuffer>{isolate, shared_array_buffer});
        msg_->AddSharedArrayBuffer(shared_array_buffer->GetBackingStore());
        return Just(i);
    }

    ValueSerializer* serializer = nullptr;

private:
    Isolate* isolate_;
    Local<Context> context_;
    Message* msg_;
    std::vector<Global<SharedArrayBuffer>> seen_shared_array_buffers_;

    friend class tns::worker::Message;
};

class DeserializerDelegate : public ValueDeserializer::Delegate {
public:
    DeserializerDelegate(
            Message* m, Isolate* isolate,
            const std::vector<Local<SharedArrayBuffer>>& shared_array_buffers)
            : shared_array_buffers_(shared_array_buffers) {}

    MaybeLocal<Object> ReadHostObject(Isolate* isolate) override {
        EscapableHandleScope scope(isolate);
        Local<Object> object = Object::New(isolate);
        return scope.Escape(object).As<Object>();
    }

    MaybeLocal<SharedArrayBuffer> GetSharedArrayBufferFromId(
            Isolate* isolate, uint32_t clone_id) override {
        if (clone_id >= shared_array_buffers_.size()) {
            return MaybeLocal<SharedArrayBuffer>();
        }
        return shared_array_buffers_[clone_id];
    }

    ValueDeserializer* deserializer = nullptr;

private:
    const std::vector<Local<SharedArrayBuffer>>& shared_array_buffers_;
};

}  // namespace

Maybe<bool> Message::Serialize(Isolate* isolate, Local<Context> context,
                               Local<Value> input) {
    HandleScope handle_scope(isolate);
    Context::Scope context_scope(context);

    // Verify that we're not silently overwriting an existing message.
    assert(main_message_buf_.is_empty());

    SerializerDelegate delegate(isolate, context, this);
    ValueSerializer serializer(isolate, &delegate);
    delegate.serializer = &serializer;

    serializer.WriteHeader();
    if (serializer.WriteValue(context, input).IsNothing()) {
        return Nothing<bool>();
    }

    // The serializer gave us a buffer allocated using `malloc()`.
    std::pair<uint8_t*, size_t> data = serializer.Release();
    assert(data.first != nullptr);
    main_message_buf_ =
            MallocedBuffer<char>(reinterpret_cast<char*>(data.first), data.second);
    return Just(true);
}

MaybeLocal<Value> Message::Deserialize(Isolate* isolate, Local<Context> context) {
    Context::Scope context_scope(context);
    EscapableHandleScope handle_scope(isolate);

    // Attach all transferred SharedArrayBuffers to their new Isolate.
    std::vector<Local<SharedArrayBuffer>> shared_array_buffers;
    for (uint32_t i = 0; i < shared_array_buffers_.size(); ++i) {
        Local<SharedArrayBuffer> sab =
                SharedArrayBuffer::New(isolate, shared_array_buffers_[i]);
        shared_array_buffers.push_back(sab);
    }

    DeserializerDelegate delegate(this, isolate, shared_array_buffers);
    ValueDeserializer deserializer(
            isolate, reinterpret_cast<const uint8_t*>(main_message_buf_.data),
            main_message_buf_.size, &delegate);
    delegate.deserializer = &deserializer;

    // Attach all transferred ArrayBuffers to their new Isolate.
    for (uint32_t i = 0; i < array_buffers_.size(); ++i) {
        Local<ArrayBuffer> ab =
                ArrayBuffer::New(isolate, std::move(array_buffers_[i]));
        deserializer.TransferArrayBuffer(i, ab);
    }

    if (deserializer.ReadHeader(context).IsNothing()) {
        return MaybeLocal<Value>();
    }

    Local<Value> return_value;
    if (!deserializer.ReadValue(context).ToLocal(&return_value)) {
        return MaybeLocal<Value>();
    }

    return handle_scope.Escape(return_value);
}

void Message::AddSharedArrayBuffer(std::shared_ptr<BackingStore> backing_store) {
    shared_array_buffers_.emplace_back(std::move(backing_store));
}

Message::Message(MallocedBuffer<char>&& payload)
        : main_message_buf_(std::move(payload)) {}

}  // namespace worker
}  // namespace tns

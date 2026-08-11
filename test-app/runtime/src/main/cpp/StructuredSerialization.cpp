#include "StructuredSerialization.h"

#include <cassert>

#include "ArgConverter.h"

using namespace v8;

namespace tns {
namespace serialization {

void ThrowDataCloneError(Isolate* isolate, const std::string& message) {
    Local<Context> context = isolate->GetCurrentContext();
    Local<Value> error =
            Exception::Error(ArgConverter::ConvertToV8String(isolate, message));
    bool success =
            error.As<Object>()
                    ->Set(context, ArgConverter::ConvertToV8String(isolate, "name"),
                          ArgConverter::ConvertToV8String(isolate, "DataCloneError"))
                    .FromMaybe(false);
    assert(success);
    isolate->ThrowException(error);
}

namespace {

class SerializerDelegate : public ValueSerializer::Delegate {
public:
    SerializerDelegate(Isolate* isolate, HostObjectPolicy hostObjectPolicy,
                       std::vector<std::shared_ptr<BackingStore>>* sharedBuffers)
            : isolate_(isolate),
              hostObjectPolicy_(hostObjectPolicy),
              sharedBuffers_(sharedBuffers) {}

    void ThrowDataCloneError(Local<v8::String> message) override {
        serialization::ThrowDataCloneError(
                isolate_, ArgConverter::ConvertToString(message));
    }

    Maybe<bool> WriteHostObject(Isolate* isolate, Local<Object> object) override {
        if (hostObjectPolicy_ == HostObjectPolicy::kDegrade) {
            // V8 has already written the kHostObject tag; writing no payload is
            // what the zero-byte ReadHostObject below expects, and the value
            // surfaces as an empty object.
            return Just(true);
        }
        std::string name =
                ArgConverter::ConvertToString(object->GetConstructorName());
        serialization::ThrowDataCloneError(
                isolate, "#<" + name + "> could not be cloned.");
        return Nothing<bool>();
    }

    // Shared memory is shared, not copied: the receiving isolate builds a new
    // SharedArrayBuffer over this same backing store.
    Maybe<uint32_t> GetSharedArrayBufferId(
            Isolate* isolate, Local<SharedArrayBuffer> sharedArrayBuffer) override {
        std::shared_ptr<BackingStore> backingStore =
                sharedArrayBuffer->GetBackingStore();
        for (size_t i = 0; i < sharedBuffers_->size(); i++) {
            if ((*sharedBuffers_)[i] == backingStore) {
                return Just(static_cast<uint32_t>(i));
            }
        }
        uint32_t id = static_cast<uint32_t>(sharedBuffers_->size());
        sharedBuffers_->push_back(std::move(backingStore));
        return Just(id);
    }

    // Overridden only to keep the DataCloneError name: with a delegate
    // installed V8's default throws a plain Error straight onto the isolate.
    bool AdoptSharedValueConveyor(Isolate* isolate,
                                  SharedValueConveyor&& conveyor) override {
        serialization::ThrowDataCloneError(isolate,
                                           "shared value could not be cloned.");
        return false;
    }

private:
    Isolate* isolate_;
    HostObjectPolicy hostObjectPolicy_;
    std::vector<std::shared_ptr<BackingStore>>* sharedBuffers_;
};

class DeserializerDelegate : public ValueDeserializer::Delegate {
public:
    explicit DeserializerDelegate(
            const std::vector<Local<SharedArrayBuffer>>* sharedBuffers)
            : sharedBuffers_(sharedBuffers) {}

    // Counterpart of the kDegrade branch: consumes no bytes, so the stream
    // stays balanced. Unreachable for a value written under kReject.
    MaybeLocal<Object> ReadHostObject(Isolate* isolate) override {
        return Object::New(isolate);
    }

    MaybeLocal<SharedArrayBuffer> GetSharedArrayBufferFromId(
            Isolate* isolate, uint32_t cloneId) override {
        if (cloneId >= sharedBuffers_->size()) {
            return MaybeLocal<SharedArrayBuffer>();
        }
        return (*sharedBuffers_)[cloneId];
    }

private:
    const std::vector<Local<SharedArrayBuffer>>* sharedBuffers_;
};

/*
 * Validates the transfer list and collects it in registration order. The
 * detached and detachable checks are load-bearing rather than defensive:
 * ArrayBuffer::Detach() aborts the process on a non-detachable buffer instead
 * of reporting failure.
 */
bool CollectTransferList(Isolate* isolate, Local<Context> context,
                         Local<Value> transferList,
                         std::vector<Local<ArrayBuffer>>& transfers) {
    if (transferList.IsEmpty() || transferList->IsUndefined() ||
        transferList->IsNull()) {
        return true;
    }

    if (!transferList->IsArray()) {
        isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(
                isolate, "The transfer list must be an array of ArrayBuffers")));
        return false;
    }

    Local<v8::Array> list = transferList.As<v8::Array>();
    uint32_t length = list->Length();
    for (uint32_t i = 0; i < length; i++) {
        Local<Value> item;
        if (!list->Get(context, i).ToLocal(&item)) {
            return false;
        }
        if (!item->IsArrayBuffer()) {
            ThrowDataCloneError(isolate,
                                "A value in the transfer list is not transferable");
            return false;
        }

        Local<ArrayBuffer> buffer = item.As<ArrayBuffer>();
        for (const Local<ArrayBuffer>& existing : transfers) {
            if (existing == buffer) {
                ThrowDataCloneError(
                        isolate,
                        "The transfer list contains the same ArrayBuffer twice");
                return false;
            }
        }
        if (buffer->WasDetached() || !buffer->IsDetachable()) {
            ThrowDataCloneError(isolate,
                                "An ArrayBuffer in the transfer list is detached and "
                                "cannot be transferred");
            return false;
        }

        transfers.push_back(buffer);
    }
    return true;
}

}  // namespace

Maybe<bool> SerializedValue::Serialize(Isolate* isolate, Local<Context> context,
                                       Local<Value> input,
                                       Local<Value> transferList,
                                       HostObjectPolicy hostObjectPolicy) {
    HandleScope handleScope(isolate);
    Context::Scope contextScope(context);
    assert(buffer_ == nullptr);

    std::vector<Local<ArrayBuffer>> transfers;
    if (!CollectTransferList(isolate, context, transferList, transfers)) {
        return Nothing<bool>();
    }

    SerializerDelegate delegate(isolate, hostObjectPolicy, &sharedBuffers_);
    ValueSerializer serializer(isolate, &delegate);
    for (size_t i = 0; i < transfers.size(); i++) {
        serializer.TransferArrayBuffer(static_cast<uint32_t>(i), transfers[i]);
    }

    serializer.WriteHeader();
    bool written = serializer.WriteValue(context, input).FromMaybe(false);

    // Release() hands ownership over whether or not the write succeeded, so
    // the buffer is claimed either way rather than leaking with the
    // serializer.
    std::pair<uint8_t*, size_t> data = serializer.Release();
    std::unique_ptr<uint8_t, FreeDeleter> owned(data.first);
    if (!written) {
        return Nothing<bool>();
    }

    // Only once the value is safely written does the memory change hands:
    // claim each backing store before detaching, since detaching drops the
    // buffer's own reference to it.
    for (Local<ArrayBuffer> buffer : transfers) {
        std::shared_ptr<BackingStore> backingStore = buffer->GetBackingStore();
        // Detach rejects a null key only for a buffer carrying an
        // [[ArrayBufferDetachKey]]: script cannot set one, this runtime never
        // calls SetDetachKey, and the WebAssembly memory buffers that have one
        // are already turned away as non-detachable above. Unreachable, then —
        // but claiming success without moving the memory would hand the
        // receiver an empty buffer, so the failure propagates carrying V8's
        // TypeError, which names the key mismatch.
        if (buffer->Detach(Local<Value>()).IsNothing()) {
            return Nothing<bool>();
        }
        transferredBuffers_.push_back(std::move(backingStore));
    }

    buffer_ = std::move(owned);
    bufferSize_ = data.second;
    return Just(true);
}

MaybeLocal<Value> SerializedValue::Deserialize(Isolate* isolate,
                                               Local<Context> context) {
    Context::Scope contextScope(context);
    EscapableHandleScope handleScope(isolate);

    std::vector<Local<SharedArrayBuffer>> sharedBuffers;
    for (const std::shared_ptr<BackingStore>& backingStore : sharedBuffers_) {
        sharedBuffers.push_back(SharedArrayBuffer::New(isolate, backingStore));
    }

    DeserializerDelegate delegate(&sharedBuffers);
    ValueDeserializer deserializer(isolate, buffer_.get(), bufferSize_,
                                   &delegate);

    for (size_t i = 0; i < transferredBuffers_.size(); i++) {
        deserializer.TransferArrayBuffer(
                static_cast<uint32_t>(i),
                ArrayBuffer::New(isolate, std::move(transferredBuffers_[i])));
    }

    if (deserializer.ReadHeader(context).IsNothing()) {
        return MaybeLocal<Value>();
    }
    Local<Value> result;
    if (!deserializer.ReadValue(context).ToLocal(&result)) {
        return MaybeLocal<Value>();
    }
    return handleScope.Escape(result);
}

}  // namespace serialization
}  // namespace tns

#include "StructuredSerialization.h"

#include "NativeScriptAssert.h"

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "RuntimeState.h"

using namespace v8;

namespace tns {
namespace serialization {

namespace {

/*
 * The private symbol markCloneable stamps on every DOMException instance.
 * Private, so app code can neither forge the brand onto an impostor nor strip
 * it; per isolate because a worker's instances are branded and checked on its
 * own isolate, and only bytes cross between them. Every instance passes
 * through markCloneable — deserialization rebuilds via the constructor.
 */
struct DomExceptionBrandState {
    Persistent<Private> brand;
};

Local<Private> BrandOf(Isolate* isolate, DomExceptionBrandState* state) {
    if (state->brand.IsEmpty()) {
        state->brand.Reset(isolate,
                           Private::New(isolate, ArgConverter::ConvertToV8String(
                                                         isolate, "domExceptionCloneable")));
    }
    return state->brand.Get(isolate);
}

// Empty once teardown has begun — callers bail to their fallback.
Local<Private> DomExceptionBrand(Isolate* isolate) {
    auto* state = RuntimeState::For<DomExceptionBrandState>(isolate);
    if (state == nullptr) {
        return Local<Private>();
    }
    return BrandOf(isolate, state);
}

void MarkCloneableCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() < 1 || !info[0]->IsObject()) {
        return;
    }
    auto* state = RuntimeState::For<DomExceptionBrandState>(isolate);
    if (state == nullptr) {
        return;
    }
    Local<Private> brand = BrandOf(isolate, state);
    (void)info[0].As<Object>()->SetPrivate(isolate->GetCurrentContext(), brand,
                                           v8::True(isolate));
}

}  // namespace

MaybeLocal<Object> DomExceptionBinding(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    Local<Object> binding = Object::New(isolate);
    Local<v8::Function> markCloneable;
    if (!v8::Function::New(context, MarkCloneableCallback).ToLocal(&markCloneable) ||
        !binding->Set(context, ArgConverter::ConvertToV8String(isolate, "markCloneable"),
                      markCloneable)
                 .FromMaybe(false)) {
        return MaybeLocal<Object>();
    }
    return binding;
}

MaybeLocal<Object> GetDomExceptionExports(Local<Context> context) {
    return BuiltinLoader::GetExports(context, BuiltinId::kDomException, DomExceptionBinding);
}

void ThrowDataCloneError(Isolate* isolate, const std::string& message) {
    Local<Context> context = isolate->GetCurrentContext();

    /*
     * The spec's DataCloneError is a DOMException; build it through the
     * builtin's exports cache so native and JS throw sites produce the same
     * class. Delegates may call into JS here — V8 allows it, and Node's
     * serializer delegates do the same. The fallback covers a builtin that
     * can no longer run (isolate teardown, broken realm).
     */
    Local<Object> domException;
    {
        TryCatch tc(isolate);
        Local<Object> exports;
        Local<Value> ctor;
        if (GetDomExceptionExports(context).ToLocal(&exports) &&
            exports->Get(context, ArgConverter::ConvertToV8String(isolate, "DOMException"))
                    .ToLocal(&ctor) &&
            ctor->IsFunction()) {
            Local<Value> args[] = {ArgConverter::ConvertToV8String(isolate, message),
                                   ArgConverter::ConvertToV8String(isolate, "DataCloneError")};
            Local<Object> instance;
            if (ctor.As<v8::Function>()->NewInstance(context, 2, args).ToLocal(&instance)) {
                domException = instance;
            }
        }
    }
    if (!domException.IsEmpty()) {
        isolate->ThrowException(domException);
        return;
    }

    Local<Value> error =
            Exception::Error(ArgConverter::ConvertToV8String(isolate, message));
    bool success =
            error.As<Object>()
                    ->Set(context, ArgConverter::ConvertToV8String(isolate, "name"),
                          ArgConverter::ConvertToV8String(isolate, "DataCloneError"))
                    .FromMaybe(false);
    NS_DCHECK(success);
    isolate->ThrowException(error);
}

namespace {

/*
 * Every host object's payload starts with one of these, so the reader can
 * dispatch. kHostObjectDegraded carries nothing further; kHostObjectDomException
 * carries a uint32 index into the SerializedValue's out-of-band payload list.
 * The bytes never outlive the process (structuredClone round-trips in one
 * isolate, worker messages cross isolates in the same binary), so the format can
 * evolve freely with this file.
 */
constexpr uint32_t kHostObjectDegraded = 0;
constexpr uint32_t kHostObjectDomException = 1;

class SerializerDelegate : public ValueSerializer::Delegate {
public:
    SerializerDelegate(Isolate* isolate, HostObjectPolicy hostObjectPolicy,
                       std::vector<std::shared_ptr<BackingStore>>* sharedBuffers,
                       std::vector<SerializedValue::DomExceptionPayload>* domExceptions)
            : isolate_(isolate),
              hostObjectPolicy_(hostObjectPolicy),
              sharedBuffers_(sharedBuffers),
              domExceptions_(domExceptions),
              domExceptionBrand_(DomExceptionBrand(isolate)) {}

    void SetSerializer(ValueSerializer* serializer) { serializer_ = serializer; }

    void ThrowDataCloneError(Local<v8::String> message) override {
        serialization::ThrowDataCloneError(
                isolate_, ArgConverter::ConvertToString(message));
    }

    /*
     * Always claimed: V8 samples this once per ValueSerializer and never again,
     * so a gate on "this isolate holds a DOMException" would miss the isolate's
     * first instance when a getter constructs it during the very clone that
     * carries it. The price is one IsHostObject call per plain JS object in a
     * graph, the same Node pays for its JSTransferable protocol.
     */
    bool HasCustomHostObject(Isolate* isolate) override { return true; }

    Maybe<bool> IsHostObject(Isolate* isolate, Local<Object> object) override {
        // Claiming custom host objects REPLACES V8's own embedder-field
        // detection rather than adding to it, so anything with a native half
        // has to be claimed here too — otherwise a Java proxy would be written
        // out as a plain object, silently losing the half that mattered.
        if (object->InternalFieldCount() > 0) {
            return Just(true);
        }
        if (domExceptionBrand_.IsEmpty()) {
            return Just(false);
        }
        return object->HasPrivate(isolate->GetCurrentContext(), domExceptionBrand_);
    }

    Maybe<bool> WriteHostObject(Isolate* isolate, Local<Object> object) override {
        // DOMException serializes under both policies: it is [Serializable] in
        // the IDL, and it is a plain JS object with no native half to lose.
        bool isDomException = false;
        if (!domExceptionBrand_.IsEmpty() &&
            !object->HasPrivate(isolate->GetCurrentContext(), domExceptionBrand_)
                     .To(&isDomException)) {
            return Nothing<bool>();
        }
        if (isDomException) {
            return WriteDomException(isolate, object);
        }
        if (hostObjectPolicy_ == HostObjectPolicy::kDegrade) {
            // Tag only, no payload: the value surfaces as an empty object.
            serializer_->WriteUint32(kHostObjectDegraded);
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
    /*
     * Web IDL's DOMException serialization steps (name and message), plus the
     * stack, matching Node. The payload travels out-of-band and only an index
     * enters the stream: the receiving side must construct instances before
     * ReadValue runs, because V8 forbids JS execution during deserialization.
     */
    Maybe<bool> WriteDomException(Isolate* isolate, Local<Object> object) {
        Local<Context> context = isolate->GetCurrentContext();
        Local<Value> name, message, stack;
        if (!object->Get(context, ArgConverter::ConvertToV8String(isolate, "name"))
                     .ToLocal(&name) ||
            !object->Get(context, ArgConverter::ConvertToV8String(isolate, "message"))
                     .ToLocal(&message) ||
            !object->Get(context, ArgConverter::ConvertToV8String(isolate, "stack"))
                     .ToLocal(&stack)) {
            return Nothing<bool>();
        }
        SerializedValue::DomExceptionPayload payload;
        payload.name = ArgConverter::ToString(isolate, name);
        payload.message = ArgConverter::ToString(isolate, message);
        // The stack can legitimately be absent or tampered into a non-string;
        // carry it only when it is the string captureStackTrace left.
        payload.hasStack = stack->IsString();
        if (payload.hasStack) {
            payload.stack = ArgConverter::ToString(isolate, stack);
        }
        serializer_->WriteUint32(kHostObjectDomException);
        serializer_->WriteUint32(static_cast<uint32_t>(domExceptions_->size()));
        domExceptions_->push_back(std::move(payload));
        return Just(true);
    }

    Isolate* isolate_;
    HostObjectPolicy hostObjectPolicy_;
    std::vector<std::shared_ptr<BackingStore>>* sharedBuffers_;
    std::vector<SerializedValue::DomExceptionPayload>* domExceptions_;
    // Resolved once per serializer: V8 asks about every object in the graph,
    // and each lookup would otherwise re-resolve the state slot and push a
    // fresh handle into the caller's scope.
    Local<Private> domExceptionBrand_;
    ValueSerializer* serializer_ = nullptr;
};

class DeserializerDelegate : public ValueDeserializer::Delegate {
public:
    DeserializerDelegate(const std::vector<Local<SharedArrayBuffer>>* sharedBuffers,
                         const std::vector<Local<Object>>* domExceptions)
            : sharedBuffers_(sharedBuffers), domExceptions_(domExceptions) {}

    void SetDeserializer(ValueDeserializer* deserializer) {
        deserializer_ = deserializer;
    }

    // No JS may run in here (V8 forbids it during a read); DOMException
    // instances were constructed by Deserialize before ReadValue started, and
    // this only hands them out.
    MaybeLocal<Object> ReadHostObject(Isolate* isolate) override {
        uint32_t tag;
        if (!deserializer_->ReadUint32(&tag)) {
            return MaybeLocal<Object>();
        }
        switch (tag) {
            case kHostObjectDegraded:
                // Counterpart of the kDegrade branch: tag only, so the value
                // arrives as an empty object. Unreachable for a value written
                // under kReject.
                return Object::New(isolate);
            case kHostObjectDomException: {
                uint32_t index;
                if (!deserializer_->ReadUint32(&index) || index >= domExceptions_->size()) {
                    return MaybeLocal<Object>();
                }
                return (*domExceptions_)[index];
            }
            default:
                return MaybeLocal<Object>();
        }
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
    const std::vector<Local<Object>>* domExceptions_;
    ValueDeserializer* deserializer_ = nullptr;
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
    NS_DCHECK(buffer_ == nullptr);

    std::vector<Local<ArrayBuffer>> transfers;
    if (!CollectTransferList(isolate, context, transferList, transfers)) {
        return Nothing<bool>();
    }

    SerializerDelegate delegate(isolate, hostObjectPolicy, &sharedBuffers_, &domExceptions_);
    ValueSerializer serializer(isolate, &delegate);
    delegate.SetSerializer(&serializer);
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

    /*
     * Construct every DOMException the payload names before the read begins:
     * JS is allowed here and forbidden inside ReadHostObject. Construction goes
     * through the real constructor — on a worker isolate that never touched
     * DOMException this runs the builtin on demand — so each instance is
     * branded again and re-serializes on the next hop.
     */
    std::vector<Local<Object>> domExceptions;
    if (!domExceptions_.empty()) {
        Local<Object> exports;
        Local<Value> ctor;
        if (!GetDomExceptionExports(context).ToLocal(&exports) ||
            !exports->Get(context, ArgConverter::ConvertToV8String(isolate, "DOMException"))
                     .ToLocal(&ctor) ||
            !ctor->IsFunction()) {
            // Only reached with nothing pending (a builtin that cannot load
            // during teardown); the caller must see a failure, not an undefined
            // result.
            if (!isolate->HasPendingException()) {
                ThrowDataCloneError(isolate,
                                    "DOMException could not be rebuilt on this isolate.");
            }
            return MaybeLocal<Value>();
        }
        Local<v8::String> stackKey = ArgConverter::ConvertToV8String(isolate, "stack");
        for (const DomExceptionPayload& payload : domExceptions_) {
            Local<Value> args[] = {ArgConverter::ConvertToV8String(isolate, payload.message),
                                   ArgConverter::ConvertToV8String(isolate, payload.name)};
            Local<Object> exception;
            if (!ctor.As<v8::Function>()->NewInstance(context, 2, args).ToLocal(&exception)) {
                return MaybeLocal<Value>();
            }
            // The sender's stack replaces the one captured just now for the
            // receiving side's constructor frame, matching Node.
            if (payload.hasStack &&
                !exception->Set(context, stackKey,
                                ArgConverter::ConvertToV8String(isolate, payload.stack))
                         .FromMaybe(false)) {
                return MaybeLocal<Value>();
            }
            domExceptions.push_back(exception);
        }
    }

    DeserializerDelegate delegate(&sharedBuffers, &domExceptions);
    ValueDeserializer deserializer(isolate, buffer_.get(), bufferSize_,
                                   &delegate);
    delegate.SetDeserializer(&deserializer);

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

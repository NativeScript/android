//
// Created by Osei Fortune on 14/01/2024.
//

#include "URLSearchParamsImpl.h"

using namespace ada;

namespace tns {
    URLSearchParamsImpl::URLSearchParamsImpl(ada::url_search_params params) : params_(params) {}

    URLSearchParamsImpl *URLSearchParamsImpl::GetPointer(v8::Local<v8::Object> object) {
        // A foreign receiver (e.g. entries.call({})) has no internal field, and
        // reading one that is not there is undefined behavior, so check first
        // and report not-a-URLSearchParams instead.
        if (object->InternalFieldCount() < 1) {
            return nullptr;
        }
        auto ptr = object->GetAlignedPointerFromInternalField(0);
        if (ptr == nullptr) {
            return nullptr;
        }
        return static_cast<URLSearchParamsImpl *>(ptr);
    }

    v8::Local<v8::FunctionTemplate> URLSearchParamsImpl::GetCtor(v8::Isolate *isolate) {
        v8::Local<v8::FunctionTemplate> ctorTmpl = v8::FunctionTemplate::New(isolate, Ctor);
        ctorTmpl->SetClassName(ArgConverter::ConvertToV8String(isolate, "URLSearchParams"));

        auto tmpl = ctorTmpl->InstanceTemplate();
        tmpl->SetInternalFieldCount(1);
        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "append"),
                v8::FunctionTemplate::New(isolate, Append));
        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "delete"),
                v8::FunctionTemplate::New(isolate, Delete));

        // Capture the entries template so it can double as the default @@iterator
        // (registered below), matching the browser identity
        // URLSearchParams.prototype[Symbol.iterator] === entries.
        auto entriesTmpl = v8::FunctionTemplate::New(isolate, Entries);
        tmpl->Set(ArgConverter::ConvertToV8String(isolate, "entries"), entriesTmpl);

        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "forEach"),
                v8::FunctionTemplate::New(isolate, ForEach));

        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "get"),
                v8::FunctionTemplate::New(isolate, Get));

        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "getAll"),
                v8::FunctionTemplate::New(isolate, GetAll));

        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "has"),
                v8::FunctionTemplate::New(isolate, Has));

        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "keys"),
                v8::FunctionTemplate::New(isolate, Keys));

        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "set"),
                v8::FunctionTemplate::New(isolate, Set));

        tmpl->SetAccessor(
                ArgConverter::ConvertToV8String(isolate, "size"),
                GetSize
        );


        tmpl->Set(
                ArgConverter::ConvertToV8String(isolate, "sort"),
                v8::FunctionTemplate::New(isolate, Sort));

        tmpl->Set(ArgConverter::ConvertToV8String(isolate, "toString"),
                  v8::FunctionTemplate::New(isolate, &ToString));


        tmpl->Set(ArgConverter::ConvertToV8String(isolate, "values"),
                  v8::FunctionTemplate::New(isolate, &Values));

        // Declare the type as iterable<USVString, USVString> by exposing
        // @@iterator. It aliases entries() (which returns a genuine ES iterator),
        // so `for..of`, spread, and `new URLSearchParams(otherParams)` (the spec
        // sequence/copy form) all work. Non-enumerable, to match the browser
        // prototype shape.
        tmpl->Set(v8::Symbol::GetIterator(isolate), entriesTmpl,
                  v8::PropertyAttribute::DontEnum);

        return ctorTmpl;
    }

    // Spec-compliant URLSearchParams init (record / sequence / primitive). The
    // dispatch mirrors the WebIDL union conversion to
    // `(sequence<sequence<USVString>> or record<USVString, USVString> or USVString)`
    // that browsers/Node apply (https://url.spec.whatwg.org/#interface-urlsearchparams):
    //   object with @@iterator   -> sequence    (Array, Map, Set, URLSearchParams, generator)
    //   object without @@iterator-> record      (own enumerable string keys, in order)
    //   other primitive          -> USVString   (number/boolean/bigint -> string; Symbol throws)
    //   null / undefined         -> empty
    namespace {
        void ThrowTypeError(v8::Isolate *isolate, const char *message) {
            isolate->ThrowException(
                    v8::Exception::TypeError(ArgConverter::ConvertToV8String(isolate, message)));
        }

        // --- Live iterators for entries()/keys()/values()/@@iterator. ---
        // Spec URLSearchParams iterators are *live*: they reflect append()/delete()
        // performed after the iterator is created (and mid-iteration). Each is backed
        // by a small hidden state object — { src: the URLSearchParams, idx: cursor,
        // knd: which projection } — captured as next()'s Data. No snapshot, and no
        // bespoke iterator class/finalizer: the source is kept alive by the state
        // reference and collected normally with the iterator. next() re-reads the
        // params on every call, so duplicate keys keep their own values too.
        enum IteratorKind { ITER_KEYS = 0, ITER_VALUES = 1, ITER_ENTRIES = 2 };

        const char *const kStateSource = "src";
        const char *const kStateIndex = "idx";
        const char *const kStateKind = "knd";
        const char *const kStateIterator = "itr";

        void IteratorSelf(const v8::FunctionCallbackInfo<v8::Value> &args) {
            // An iterator is itself iterable (iterator[@@iterator]() === iterator),
            // so `for..of params.entries()` works.
            args.GetReturnValue().Set(args.This());
        }

        void IteratorNext(const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            auto context = isolate->GetCurrentContext();
            auto state = args.Data().As<v8::Object>();

            // WebIDL brand check: next() must be invoked on the iterator it was
            // created on, so a detached next() throws instead of advancing the
            // captured state.
            v8::Local<v8::Value> iteratorValue;
            if (!state->Get(context, ArgConverter::ConvertToV8String(isolate, kStateIterator))
                    .ToLocal(&iteratorValue) ||
                !iteratorValue->StrictEquals(args.This())) {
                ThrowTypeError(isolate, "Illegal invocation");
                return;
            }

            auto indexKey = ArgConverter::ConvertToV8String(isolate, kStateIndex);
            v8::Local<v8::Value> sourceValue;
            v8::Local<v8::Value> indexValue;
            v8::Local<v8::Value> kindValue;
            if (!state->Get(context, ArgConverter::ConvertToV8String(isolate, kStateSource))
                    .ToLocal(&sourceValue) ||
                !state->Get(context, indexKey).ToLocal(&indexValue) ||
                !state->Get(context, ArgConverter::ConvertToV8String(isolate, kStateKind))
                    .ToLocal(&kindValue)) {
                return;
            }
            auto index = indexValue->Uint32Value(context).FromMaybe(0u);
            auto kind = kindValue->Int32Value(context).FromMaybe(ITER_ENTRIES);

            auto result = v8::Object::New(isolate);
            auto valueKey = ArgConverter::ConvertToV8String(isolate, "value");
            auto doneKey = ArgConverter::ConvertToV8String(isolate, "done");

            URLSearchParamsImpl *ptr = sourceValue->IsObject()
                    ? URLSearchParamsImpl::GetPointer(sourceValue.As<v8::Object>()) : nullptr;
            if (ptr == nullptr || index >= ptr->GetURLSearchParams()->size()) {
                result->Set(context, valueKey, v8::Undefined(isolate)).Check();
                result->Set(context, doneKey, v8::True(isolate)).Check();
                args.GetReturnValue().Set(result);
                return;
            }

            auto pair = (*ptr->GetURLSearchParams())[index];
            v8::Local<v8::Value> value;
            if (kind == ITER_KEYS) {
                value = ArgConverter::ConvertToV8String(isolate, pair.first);
            } else if (kind == ITER_VALUES) {
                value = ArgConverter::ConvertToV8String(isolate, pair.second);
            } else {
                v8::Local<v8::Value> kv[] = {
                        ArgConverter::ConvertToV8String(isolate, pair.first),
                        ArgConverter::ConvertToV8String(isolate, pair.second),
                };
                value = v8::Array::New(isolate, kv, 2);
            }
            state->Set(context, indexKey, v8::Integer::NewFromUnsigned(isolate, index + 1)).Check();
            result->Set(context, valueKey, value).Check();
            result->Set(context, doneKey, v8::False(isolate)).Check();
            args.GetReturnValue().Set(result);
        }

        // Sets `args`' return value to a fresh live iterator of `kind` over
        // `args.This()` (the URLSearchParams). The state object is never exposed to
        // JS — only the iterator (with next() and @@iterator) is returned.
        void ReturnLiveIterator(const v8::FunctionCallbackInfo<v8::Value> &args, int kind) {
            auto isolate = args.GetIsolate();
            auto context = isolate->GetCurrentContext();

            // WebIDL brand check: entries()/keys()/values() require a genuine
            // URLSearchParams receiver, so e.g. entries.call({}) throws.
            if (URLSearchParamsImpl::GetPointer(args.This()) == nullptr) {
                ThrowTypeError(isolate, "Illegal invocation");
                return;
            }

            auto state = v8::Object::New(isolate);
            state->Set(context, ArgConverter::ConvertToV8String(isolate, kStateSource),
                       args.This()).Check();
            state->Set(context, ArgConverter::ConvertToV8String(isolate, kStateIndex),
                       v8::Integer::NewFromUnsigned(isolate, 0)).Check();
            state->Set(context, ArgConverter::ConvertToV8String(isolate, kStateKind),
                       v8::Integer::New(isolate, kind)).Check();

            v8::Local<v8::Function> nextFn;
            v8::Local<v8::Function> selfFn;
            if (!v8::Function::New(context, IteratorNext, state).ToLocal(&nextFn) ||
                !v8::Function::New(context, IteratorSelf).ToLocal(&selfFn)) {
                return;
            }

            auto iterator = v8::Object::New(isolate);
            // Recorded in the hidden state so next() can brand-check its receiver.
            state->Set(context, ArgConverter::ConvertToV8String(isolate, kStateIterator),
                       iterator).Check();
            iterator->Set(context, ArgConverter::ConvertToV8String(isolate, "next"), nextFn).Check();
            iterator->Set(context, v8::Symbol::GetIterator(isolate), selfFn).Check();
            iterator->DefineOwnProperty(context, v8::Symbol::GetToStringTag(isolate),
                                        ArgConverter::ConvertToV8String(isolate,
                                                                        "URLSearchParams Iterator"),
                                        v8::PropertyAttribute::DontEnum).Check();
            args.GetReturnValue().Set(iterator);
        }

        // Coerce a JS value to a UTF-8 string via the USVString conversion the
        // URLSearchParams init algorithm applies to every name/value. Returns
        // false if the conversion threw — a Symbol value, or a user toString that
        // throws — leaving the JS exception pending so the caller can stop and
        // let it propagate (the spec throws here rather than dropping the entry).
        // No further V8 API may be called once that happens.
        bool ValueToString(v8::Local<v8::Context> context, v8::Local<v8::Value> value,
                           std::string &out) {
            v8::Local<v8::String> str;
            if (!value->ToString(context).ToLocal(&str)) {
                return false;
            }
            out = ArgConverter::ConvertToString(str);
            return true;
        }

        // Opens a JS sync iterator: invokes `iterMethod` (the resolved @@iterator)
        // with `receiver` as the `this` value, requires the result to be an object,
        // and reads its callable `next` method. On any deviation the matching JS
        // TypeError is thrown (or left pending, if a user callback threw) and the
        // function returns false.
        bool OpenIterator(v8::Local<v8::Context> context, v8::Local<v8::Value> receiver,
                          v8::Local<v8::Function> iterMethod, v8::Local<v8::Object> &outIterator,
                          v8::Local<v8::Function> &outNext) {
            auto isolate = context->GetIsolate();
            v8::Local<v8::Value> iteratorValue;
            if (!iterMethod->Call(context, receiver, 0, nullptr).ToLocal(&iteratorValue)) {
                return false;
            }
            if (!iteratorValue->IsObject()) {
                ThrowTypeError(isolate, "Result of the Symbol.iterator method is not an object");
                return false;
            }
            outIterator = iteratorValue.As<v8::Object>();
            v8::Local<v8::Value> nextValue;
            if (!outIterator->Get(context, ArgConverter::ConvertToV8String(isolate, "next"))
                    .ToLocal(&nextValue)) {
                return false;
            }
            if (!nextValue->IsFunction()) {
                ThrowTypeError(isolate, "Iterator has no 'next' method");
                return false;
            }
            outNext = nextValue.As<v8::Function>();
            return true;
        }

        // Advances `iterator` once. On a normal step writes the yielded value to
        // `outValue` and leaves `outDone` false; at completion sets `outDone` true.
        // Returns false (exception pending) if next() threw or yielded a non-object
        // result. `next` is captured once by the caller, per the iterator protocol.
        bool StepIterator(v8::Local<v8::Context> context, v8::Local<v8::Object> iterator,
                          v8::Local<v8::Function> next, v8::Local<v8::Value> &outValue,
                          bool &outDone) {
            auto isolate = context->GetIsolate();
            v8::Local<v8::Value> resultValue;
            if (!next->Call(context, iterator, 0, nullptr).ToLocal(&resultValue)) {
                return false;
            }
            if (!resultValue->IsObject()) {
                ThrowTypeError(isolate, "Iterator result is not an object");
                return false;
            }
            auto result = resultValue.As<v8::Object>();
            v8::Local<v8::Value> doneValue;
            if (!result->Get(context, ArgConverter::ConvertToV8String(isolate, "done"))
                    .ToLocal(&doneValue)) {
                return false;
            }
            outDone = doneValue->BooleanValue(isolate);
            if (outDone) {
                return true;
            }
            return result->Get(context, ArgConverter::ConvertToV8String(isolate, "value"))
                    .ToLocal(&outValue);
        }

        // ES IteratorClose for an abrupt (error) completion: call the iterator's
        // `return` method, if it has one, and discard both its result and any error
        // it raises — the triggering exception wins (spec discards `return`'s
        // completion on a throw). Generators with `finally` and resource-backed
        // iterables rely on this to release state when conversion fails partway.
        // Must be called with no pending JS exception: the caller captures and
        // re-throws the triggering exception around this call.
        void CloseIterator(v8::Local<v8::Context> context, v8::Local<v8::Object> iterator) {
            auto isolate = context->GetIsolate();
            v8::TryCatch tryCatch(isolate);
            v8::Local<v8::Value> returnMethod;
            if (iterator->Get(context, ArgConverter::ConvertToV8String(isolate, "return"))
                    .ToLocal(&returnMethod) && returnMethod->IsFunction()) {
                v8::Local<v8::Value> ignored;
                (void) returnMethod.As<v8::Function>()->Call(context, iterator, 0, nullptr)
                        .ToLocal(&ignored);
            }
            // tryCatch swallows anything return() throws when it goes out of scope.
        }

        // Materializes one inner element of a sequence init into its strings. Per
        // spec each element is itself converted to `sequence<USVString>`, whose first
        // step ("convert to sequence<T>") throws a TypeError when Type(V) is not
        // Object. So a non-Object (primitive) element — including a primitive string —
        // throws; it is NOT boxed (boxing would wrongly accept ["ab"] as the pair
        // ("a","b")). Iterable objects (Array, Set, a String *object*) are materialized
        // by driving their @@iterator. If an element fails to coerce, the inner
        // iterator is closed before the exception propagates.
        bool MaterializeInnerSequence(v8::Local<v8::Context> context, v8::Local<v8::Value> value,
                                      std::vector<std::string> &out) {
            auto isolate = context->GetIsolate();
            // Per WebIDL "convert to sequence<T>": if Type(V) is not Object, throw a
            // TypeError. Primitives (including strings) are NOT boxed here — boxing
            // would wrongly accept e.g. ["ab"] as the pair ("a","b") instead of
            // throwing. A String/Set/Array *object* element is fine (it is an Object).
            if (!value->IsObject()) {
                ThrowTypeError(isolate, "URLSearchParams init sequence element is not iterable");
                return false;
            }
            v8::Local<v8::Object> object = value.As<v8::Object>();
            v8::Local<v8::Value> iterMethod;
            if (!object->Get(context, v8::Symbol::GetIterator(isolate)).ToLocal(&iterMethod)) {
                return false;
            }
            if (!iterMethod->IsFunction()) {
                ThrowTypeError(isolate, "URLSearchParams init sequence element is not iterable");
                return false;
            }
            v8::Local<v8::Object> iterator;
            v8::Local<v8::Function> next;
            if (!OpenIterator(context, object, iterMethod.As<v8::Function>(), iterator, next)) {
                return false;
            }
            while (true) {
                v8::Local<v8::Value> element;
                bool done = false;
                if (!StepIterator(context, iterator, next, element, done)) {
                    return false;  // next() threw: the iterator is already finished
                }
                if (done) {
                    break;
                }
                // Capture a conversion failure, then close the iterator outside the
                // TryCatch's scope so the close runs with a clean isolate and the
                // original exception is re-thrown (not swallowed by this handler).
                v8::Local<v8::Value> caught;
                {
                    v8::TryCatch tryCatch(isolate);
                    std::string str;
                    if (ValueToString(context, element, str)) {
                        out.push_back(std::move(str));
                    } else {
                        caught = tryCatch.Exception();
                        tryCatch.Reset();
                    }
                }
                if (!caught.IsEmpty()) {
                    CloseIterator(context, iterator);
                    isolate->ThrowException(caught);
                    return false;
                }
            }
            return true;
        }

        // Sequence init form: iterate the value via its @@iterator (already resolved
        // by the caller) and append each [name, value] pair. Covers Array, Map, Set,
        // another URLSearchParams and generators uniformly. A pair whose length is
        // not exactly 2 throws a TypeError; a name/value that cannot be coerced to a
        // string aborts with the JS exception left pending. On either abrupt
        // completion the source iterator is closed first (ES IteratorClose).
        bool BuildFromSequence(v8::Local<v8::Context> context, v8::Local<v8::Object> object,
                               v8::Local<v8::Function> iterMethod, ada::url_search_params &params) {
            auto isolate = context->GetIsolate();
            v8::Local<v8::Object> iterator;
            v8::Local<v8::Function> next;
            if (!OpenIterator(context, object, iterMethod, iterator, next)) {
                return false;
            }
            while (true) {
                v8::Local<v8::Value> entry;
                bool done = false;
                if (!StepIterator(context, iterator, next, entry, done)) {
                    return false;  // next() threw: the iterator is already finished
                }
                if (done) {
                    break;
                }
                // Convert and append under a TryCatch; on failure capture the
                // exception, then close the source iterator outside the handler's
                // scope and re-throw (so the close runs clean and the error survives).
                v8::Local<v8::Value> caught;
                {
                    v8::TryCatch tryCatch(isolate);
                    std::vector<std::string> pair;
                    bool built = MaterializeInnerSequence(context, entry, pair);
                    if (built && pair.size() != 2) {
                        ThrowTypeError(isolate,
                                       "Failed to construct 'URLSearchParams': Sequence initializer "
                                       "must only contain pair elements");
                    }
                    if (tryCatch.HasCaught()) {
                        caught = tryCatch.Exception();
                        tryCatch.Reset();
                    } else {
                        params.append(pair[0], pair[1]);
                    }
                }
                if (!caught.IsEmpty()) {
                    CloseIterator(context, iterator);
                    isolate->ThrowException(caught);
                    return false;
                }
            }
            return true;
        }

        // Record init form: a plain object of name -> value, iterated in own
        // enumerable string-key order. A value that cannot be coerced to a string
        // aborts with the JS exception left pending.
        bool BuildFromRecord(v8::Local<v8::Context> context, v8::Local<v8::Object> object,
                             ada::url_search_params &params) {
            auto filter = static_cast<v8::PropertyFilter>(
                    v8::PropertyFilter::ONLY_ENUMERABLE | v8::PropertyFilter::SKIP_SYMBOLS);
            v8::Local<v8::Array> keys;
            if (!object->GetOwnPropertyNames(context, filter,
                                             v8::KeyConversionMode::kConvertToString).ToLocal(&keys)) {
                return false;
            }
            auto length = keys->Length();
            for (uint32_t i = 0; i < length; i++) {
                v8::Local<v8::Value> key;
                if (!keys->Get(context, i).ToLocal(&key)) {
                    return false;
                }
                v8::Local<v8::Value> value;
                if (!object->Get(context, key).ToLocal(&value)) {
                    return false;
                }
                std::string keyStr;
                std::string valueStr;
                if (!ValueToString(context, key, keyStr) ||
                    !ValueToString(context, value, valueStr)) {
                    return false;
                }
                params.append(keyStr, valueStr);
            }
            return true;
        }
    }

    void URLSearchParamsImpl::Ctor(const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto value = args[0];
        auto isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();

        auto ret = args.This();

        ada::url_search_params params;
        if (value->IsString()) {
            // ada strips a single leading '?' (constructor step), then runs the
            // urlencoded string parser.
            params = ada::url_search_params(ArgConverter::ConvertToString(value.As<v8::String>()));
        }
        // Dispatch the record/sequence/primitive init forms via the helpers above.
        // A failed coercion leaves a pending exception; return so V8 throws it.
        else if (value->IsObject()) {
            auto object = value.As<v8::Object>();
            v8::Local<v8::Value> iterMethod;
            if (!object->Get(context, v8::Symbol::GetIterator(isolate)).ToLocal(&iterMethod)) {
                return;  // the @@iterator getter threw
            }
            if (iterMethod->IsNullOrUndefined()) {
                if (!BuildFromRecord(context, object, params)) {
                    return;
                }
            } else if (iterMethod->IsFunction()) {
                if (!BuildFromSequence(context, object, iterMethod.As<v8::Function>(), params)) {
                    return;
                }
            } else {
                // Per WebIDL GetMethod, a present-but-non-callable @@iterator is a
                // TypeError rather than a silent fall-back to the record form.
                ThrowTypeError(isolate, "URLSearchParams init Symbol.iterator is not a function");
                return;
            }
        } else if (!value->IsNullOrUndefined()) {
            // Other primitive (number / boolean / bigint / symbol): coerce to a
            // USVString and run the urlencoded string parser. A Symbol cannot be
            // converted and throws here, matching the spec.
            std::string init;
            if (!ValueToString(context, value, init)) {
                return;
            }
            params = ada::url_search_params(init);
        }
        // null / undefined -> leave params empty

        auto searchParams = new URLSearchParamsImpl(params);

        ret->SetAlignedPointerInInternalField(0, searchParams);

        searchParams->BindFinalizer(isolate, ret);

        args.GetReturnValue().Set(ret);

    }

    void URLSearchParamsImpl::Append(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        if (ptr == nullptr) {
            return;
        }
        auto key = ArgConverter::ConvertToString(args[0].As<v8::String>());
        auto value = ArgConverter::ConvertToString(args[1].As<v8::String>());
        ptr->GetURLSearchParams()->append(key.c_str(), value.c_str());
    }

    void URLSearchParamsImpl::Delete(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        if (ptr == nullptr) {
            return;
        }
        // The name is a USVString (url.bs:3861): coerce it like the optional
        // value below instead of assuming a string (a Symbol or throwing
        // toString leaves the pending exception and aborts).
        std::string key;
        if (!ValueToString(args.GetIsolate()->GetCurrentContext(), args[0], key)) {
            return;
        }
        // Spec delete(name, value): when the value argument is given, remove only
        // tuples matching BOTH name and value (url.bs:4000). The value is a
        // USVString, so coerce via ValueToString (number/boolean -> string; a
        // Symbol or throwing toString leaves the pending exception and aborts).
        // An explicit `undefined` value is treated as omitted (name-only), matching
        // WPT "Two-argument delete() respects undefined as second arg"; only a
        // present, non-undefined value takes the value-filtered path.
        if (args.Length() > 1 && !args[1]->IsUndefined()) {
            std::string value;
            if (!ValueToString(args.GetIsolate()->GetCurrentContext(), args[1], value)) {
                return;  // coercion threw (Symbol / throwing toString)
            }
            ptr->GetURLSearchParams()->remove(key, value);
        } else {
            ptr->GetURLSearchParams()->remove(key.c_str());
        }
    }

    // entries()/keys()/values() return live ES iterators (per spec they are
    // iterators, not arrays). Reading the params by index per next() also keeps
    // each occurrence of a repeated key paired with its own value.
    void URLSearchParamsImpl::Entries(const v8::FunctionCallbackInfo<v8::Value> &args) {
        ReturnLiveIterator(args, ITER_ENTRIES);
    }

    void URLSearchParamsImpl::ForEach(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        auto isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();
        if (ptr == nullptr) {
            return;
        }
        auto callback = args[0].As<v8::Function>();
        auto searchParams = args.This();
        // Use thisArg if provided, otherwise undefined
        auto thisArg = args.Length() > 1 ? args[1] : v8::Undefined(isolate).As<v8::Value>();
        // Use get_entries() to correctly handle duplicate keys
        auto entries = ptr->GetURLSearchParams()->get_entries();
        while (entries.has_next()) {
            auto entry = entries.next();
            if (entry) {
                auto& [key, value] = entry.value();
                // Per spec, forEach callback receives (value, key, searchParams)
                v8::Local<v8::Value> callbackArgs[] = {
                        ArgConverter::ConvertToV8String(isolate, value.data()),
                        ArgConverter::ConvertToV8String(isolate, key.data()),
                        searchParams,
                };
                v8::Local<v8::Value> result;
                if (!callback->Call(context, thisArg, 3, callbackArgs).ToLocal(&result)) {
                    // If the callback throws an exception, stop iteration
                    return;
                }
            }
        }
    }

    void URLSearchParamsImpl::Get(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        auto isolate = args.GetIsolate();
        if (ptr == nullptr) {
            args.GetReturnValue().SetUndefined();
            return;
        }
        // The name is a USVString (url.bs:3862); a Symbol or throwing toString
        // leaves the pending exception and aborts.
        std::string key;
        if (!ValueToString(isolate->GetCurrentContext(), args[0], key)) {
            return;
        }
        auto value = ptr->GetURLSearchParams()->get(key);
        if (value.has_value()) {
            auto ret = ArgConverter::ConvertToV8String(isolate, std::string(value.value()));
            args.GetReturnValue().Set(ret);
        } else {
            // Spec: get() returns null for a missing name (url.bs:4016), not
            // undefined.
            args.GetReturnValue().SetNull();
        }
    }

    void URLSearchParamsImpl::GetAll(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        auto isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();
        if (ptr == nullptr) {
            args.GetReturnValue().Set(v8::Array::New(isolate));
            return;
        }
        // The name is a USVString (url.bs:3863); a Symbol or throwing toString
        // leaves the pending exception and aborts.
        std::string key;
        if (!ValueToString(context, args[0], key)) {
            return;
        }
        auto values = ptr->GetURLSearchParams()->get_all(key);
        auto ret = v8::Array::New(isolate, values.size());
        size_t i = 0;
        for (auto item: values) {
            ret->Set(context, i++, ArgConverter::ConvertToV8String(isolate, item));
        }
        args.GetReturnValue().Set(ret);
    }

    void URLSearchParamsImpl::Has(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        if (ptr == nullptr) {
            args.GetReturnValue().Set(false);
            return;
        }
        // The name is a USVString (url.bs:3864): coerce it like the optional
        // value below instead of assuming a string (a Symbol or throwing
        // toString leaves the pending exception and aborts).
        std::string key;
        if (!ValueToString(args.GetIsolate()->GetCurrentContext(), args[0], key)) {
            return;
        }
        // Spec has(name, value): when the value argument is given, return true
        // only for a tuple matching BOTH name and value (url.bs:4028). The value
        // is a USVString, so coerce via ValueToString (number/boolean -> string;
        // a Symbol or throwing toString leaves the pending exception and aborts).
        bool value;
        // An explicit `undefined` value is treated as omitted (name-only), matching
        // WPT "Two-argument has() respects undefined as second arg"; only a present,
        // non-undefined value takes the value-filtered path.
        if (args.Length() > 1 && !args[1]->IsUndefined()) {
            std::string filter;
            if (!ValueToString(args.GetIsolate()->GetCurrentContext(), args[1], filter)) {
                return;  // coercion threw (Symbol / throwing toString)
            }
            value = ptr->GetURLSearchParams()->has(key, filter);
        } else {
            value = ptr->GetURLSearchParams()->has(key);
        }

        args.GetReturnValue().Set(value);
    }

    // Live ES iterator (see Entries above).
    void URLSearchParamsImpl::Keys(const v8::FunctionCallbackInfo<v8::Value> &args) {
        ReturnLiveIterator(args, ITER_KEYS);
    }

    void URLSearchParamsImpl::Set(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        if (ptr == nullptr) {
            return;
        }
        auto key = args[0].As<v8::String>();
        auto value = args[1].As<v8::String>();

        ptr->GetURLSearchParams()->set(
                ArgConverter::ConvertToString(key),
                ArgConverter::ConvertToString(value)
        );
    }

    void URLSearchParamsImpl::GetSize(v8::Local<v8::String> property,
                                      const v8::PropertyCallbackInfo<v8::Value> &info) {
        URLSearchParamsImpl *ptr = GetPointer(info.This());
        if (ptr == nullptr) {
            info.GetReturnValue().Set(0);
            return;
        }

        auto value = ptr->GetURLSearchParams()->size();
        info.GetReturnValue().Set((int) value);

    }

    void URLSearchParamsImpl::Sort(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        if (ptr == nullptr) {
            return;
        }
        ptr->GetURLSearchParams()->sort();
    }

    void URLSearchParamsImpl::ToString(const v8::FunctionCallbackInfo<v8::Value> &args) {
        URLSearchParamsImpl *ptr = GetPointer(args.This());
        if (ptr == nullptr) {
            args.GetReturnValue().SetEmptyString();
            return;
        }
        auto isolate = args.GetIsolate();


        auto value = ptr->GetURLSearchParams()->to_string();

        auto ret = ArgConverter::ConvertToV8String(isolate, value);

        args.GetReturnValue().Set(ret);
    }

    // Live ES iterator (see Entries above).
    void URLSearchParamsImpl::Values(const v8::FunctionCallbackInfo<v8::Value> &args) {
        ReturnLiveIterator(args, ITER_VALUES);
    }


    url_search_params *URLSearchParamsImpl::GetURLSearchParams() {
        return &this->params_;
    }

} // tns
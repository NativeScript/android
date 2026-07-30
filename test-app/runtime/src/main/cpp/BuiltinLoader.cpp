#include "BuiltinLoader.h"

#include <mutex>
#include <vector>

#include "ArgConverter.h"

using namespace v8;

namespace tns {

namespace {

/*
 * Process-wide bytecode cache shared across isolates. Worker runtimes
 * initialize on their own threads, so every access is under the mutex.
 */
std::mutex builtinCacheMutex;
std::vector<uint8_t> builtinCache[static_cast<unsigned>(BuiltinId::kCount)];

/*
 * Every builtin is compiled as a function body receiving these fixed
 * parameters, mirroring Node's module wrapper: a file exports through
 * `module.exports`/`exports`, and natives arrive as properties of the
 * `binding` bag (Node's internalBinding idiom) for each file to destructure.
 */
constexpr const char* kExportsParamName = "exports";
constexpr const char* kModuleParamName = "module";
constexpr const char* kBindingParamName = "binding";
constexpr size_t kParamCount = 3;

MaybeLocal<v8::Function> CompileBuiltin(Local<Context> context, BuiltinId id) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    const BuiltinSource& builtin = GetBuiltinSource(id);
    const unsigned index = static_cast<unsigned>(id);

    // Copy the blob out so the shared slot can be refreshed concurrently while
    // this compile still reads from the copy.
    std::vector<uint8_t> blob;
    {
        std::lock_guard<std::mutex> lock(builtinCacheMutex);
        blob = builtinCache[index];
    }

    ScriptOrigin origin(ArgConverter::ConvertToV8String(isolate, builtin.name));
    Local<v8::String> sourceText = ArgConverter::ConvertToV8String(
            isolate, builtin.source, static_cast<int>(builtin.length));
    Local<v8::String> params[] = {
            ArgConverter::ConvertToV8String(isolate, kExportsParamName),
            ArgConverter::ConvertToV8String(isolate, kModuleParamName),
            ArgConverter::ConvertToV8String(isolate, kBindingParamName)};

    Local<v8::Function> fn;
    if (!blob.empty()) {
        // The Source owns and deletes the CachedData object; BufferNotOwned
        // keeps the underlying bytes (our copy) out of its hands.
        auto* cachedData = new ScriptCompiler::CachedData(
                blob.data(), static_cast<int>(blob.size()),
                ScriptCompiler::CachedData::BufferNotOwned);
        ScriptCompiler::Source source(sourceText, origin, cachedData);
        if (ScriptCompiler::CompileFunction(context, &source, kParamCount, params, 0, nullptr,
                                            ScriptCompiler::kConsumeCodeCache)
                    .ToLocal(&fn) &&
            !cachedData->rejected) {
            return fn;
        }
        // Rejected cache (e.g. produced under different flags): fall through
        // and recompile eagerly so the refreshed blob covers inner functions
        // again.
    }

    ScriptCompiler::Source source(sourceText, origin);
    if (!ScriptCompiler::CompileFunction(context, &source, kParamCount, params, 0, nullptr,
                                         ScriptCompiler::kEagerCompile)
                 .ToLocal(&fn)) {
        return MaybeLocal<v8::Function>();
    }

    std::unique_ptr<ScriptCompiler::CachedData> produced(
            ScriptCompiler::CreateCodeCacheForFunction(fn));
    if (produced != nullptr && produced->data != nullptr && produced->length > 0) {
        std::lock_guard<std::mutex> lock(builtinCacheMutex);
        builtinCache[index].assign(produced->data, produced->data + produced->length);
    }

    return fn;
}

}  // namespace

MaybeLocal<Value> BuiltinLoader::RunBuiltin(Local<Context> context, BuiltinId id,
                                            Local<Value> binding) {
    Isolate* isolate = v8::Isolate::GetCurrent();

    Local<v8::Function> fn;
    if (!CompileBuiltin(context, id).ToLocal(&fn)) {
        return MaybeLocal<Value>();
    }

    Local<Object> exportsObj = Object::New(isolate);
    Local<Object> moduleObj = Object::New(isolate);
    Local<v8::String> exportsKey = ArgConverter::ConvertToV8String(isolate, kExportsParamName);
    if (!moduleObj->Set(context, exportsKey, exportsObj).FromMaybe(false)) {
        return MaybeLocal<Value>();
    }

    Local<Value> args[] = {exportsObj, moduleObj,
                           binding.IsEmpty() ? Undefined(isolate).As<Value>() : binding};
    if (fn->Call(context, Undefined(isolate), static_cast<int>(kParamCount), args).IsEmpty()) {
        return MaybeLocal<Value>();
    }

    return moduleObj->Get(context, exportsKey);
}

}  // namespace tns

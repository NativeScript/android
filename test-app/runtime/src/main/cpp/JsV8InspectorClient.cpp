#include "JsV8InspectorClient.h"
#include <assert.h>
#include <algorithm>
#include <cstring>
#include <include/libplatform/libplatform.h>
#include <src/inspector/v8-console-message.h>
#include <src/inspector/v8-inspector-impl.h>
#include <src/inspector/v8-inspector-session-impl.h>
#include <src/inspector/v8-runtime-agent-impl.h>
#include <src/inspector/v8-stack-trace-impl.h>

#include "Runtime.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"

#include "ArgConverter.h"
#include "Constants.h"
#include "File.h"
#include "Util.h"
#include "Utils.h"

#include "ada/ada.h"
#include "third_party/json.hpp"

using namespace std;
using namespace tns;
using namespace v8;

using namespace v8_inspector;
using namespace inspector;

using json = nlohmann::json;

// Utility functions for converting between inspector StringView and UTF8 string

static inline v8_inspector::StringView stringToStringView(const std::string &str) {
    auto* chars = reinterpret_cast<const uint8_t*>(str.c_str());
    return { chars, str.length() };
}

static inline std::string stringViewToString(v8::Isolate* isolate, const v8_inspector::StringView& stringView) {
    int length = static_cast<int>(stringView.length());
    if (!length) {
        return "";
    }
    v8::Local<v8::String> message = (
        stringView.is8Bit() ?
            v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t*>(stringView.characters8()), v8::NewStringType::kNormal, length) :
            v8::String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t*>(stringView.characters16()), v8::NewStringType::kNormal, length)
    ) .ToLocalChecked();
    v8::String::Utf8Value result(isolate, message);
    return *result;
}

namespace {

// Scheme advertised to the frontend for source maps the runtime can serve.
// Chrome DevTools never loads `file:` (or `data:`/`devtools:`) resources
// through the target -- PageResourceLoader routes those to the frontend host
// machine, which cannot see files on the device. Any other scheme is fetched
// with Network.loadNetworkResource, which we answer from disk.
constexpr const char* kSourceMapScheme = "nsruntime://";

std::string JsonDump(const json& value) {
    // replace invalid UTF-8 instead of throwing across the JNI boundary
    return value.dump(-1, ' ', false, json::error_handler_t::replace);
}

std::string FinishResponse(json& response, const std::string& sessionId) {
    if (!sessionId.empty()) {
        response["sessionId"] = sessionId;
    }
    return JsonDump(response);
}

std::string Base64Encode(const char* data, size_t length) {
    static const char* kAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    encoded.reserve(((length + 2) / 3) * 4);
    size_t i = 0;
    for (; i + 2 < length; i += 3) {
        uint32_t chunk = (static_cast<uint8_t>(data[i]) << 16) |
                         (static_cast<uint8_t>(data[i + 1]) << 8) |
                         static_cast<uint8_t>(data[i + 2]);
        encoded.push_back(kAlphabet[(chunk >> 18) & 0x3F]);
        encoded.push_back(kAlphabet[(chunk >> 12) & 0x3F]);
        encoded.push_back(kAlphabet[(chunk >> 6) & 0x3F]);
        encoded.push_back(kAlphabet[chunk & 0x3F]);
    }
    if (i < length) {
        uint32_t chunk = static_cast<uint8_t>(data[i]) << 16;
        bool hasSecond = i + 1 < length;
        if (hasSecond) {
            chunk |= static_cast<uint8_t>(data[i + 1]) << 8;
        }
        encoded.push_back(kAlphabet[(chunk >> 18) & 0x3F]);
        encoded.push_back(kAlphabet[(chunk >> 12) & 0x3F]);
        encoded.push_back(hasSecond ? kAlphabet[(chunk >> 6) & 0x3F] : '=');
        encoded.push_back('=');
    }
    return encoded;
}

// Opt-out via nativescript.config.ts (serialized into the bundled
// package.json): `android: { disableSourceMapURLRewrite: true }`, or the same
// key at the top level.
bool ShouldRewriteSourceMapURLs() {
    static bool disabled = []() {
        int length = 0;
        auto* data = static_cast<uint8_t*>(File::ReadBinary(Constants::APP_ROOT_FOLDER_PATH + "package.json", length));
        if (data == nullptr) {
            return false;
        }
        std::string content(reinterpret_cast<const char*>(data), length);
        delete[] data;

        auto parsed = json::parse(content, nullptr, false);
        if (parsed.is_discarded() || !parsed.is_object()) {
            return false;
        }

        const json* value = nullptr;
        if (parsed.contains("android") && parsed["android"].contains("disableSourceMapURLRewrite")) {
            value = &parsed["android"]["disableSourceMapURLRewrite"];
        } else if (parsed.contains("disableSourceMapURLRewrite")) {
            value = &parsed["disableSourceMapURLRewrite"];
        }
        return value != nullptr && value->is_boolean() && value->get<bool>();
    }();
    return !disabled;
}

// Rewrites the sourceMapURL of outgoing Debugger.scriptParsed /
// Debugger.scriptFailedToParse events from a file url (or a url relative to
// the script's file url) to an absolute nsruntime:// url, so DevTools
// requests the map through the target instead of the frontend host.
std::string MaybeRewriteSourceMapURL(const std::string& message) {
    if (message.find("\"Debugger.scriptParsed\"") == std::string::npos &&
            message.find("\"Debugger.scriptFailedToParse\"") == std::string::npos) {
        return message;
    }

    if (!ShouldRewriteSourceMapURLs()) {
        return message;
    }

    auto parsed = json::parse(message, nullptr, false);
    if (parsed.is_discarded() || !parsed.contains("params") || !parsed["params"].is_object()) {
        return message;
    }

    auto& params = parsed["params"];
    std::string sourceMapURL = params.value("sourceMapURL", "");
    if (sourceMapURL.empty() || sourceMapURL.rfind("data:", 0) == 0 ||
            sourceMapURL.rfind("http:", 0) == 0 || sourceMapURL.rfind("https:", 0) == 0 ||
            sourceMapURL.rfind(kSourceMapScheme, 0) == 0) {
        return message;
    }

    std::string path;
    if (sourceMapURL.rfind("file://", 0) == 0) {
        path = sourceMapURL.substr(strlen("file://"));
    } else if (sourceMapURL[0] == '/') {
        path = sourceMapURL;
    } else {
        // Relative to the script url, e.g. "bundle.js.map".
        std::string scriptUrl = params.value("url", "");
        if (scriptUrl.rfind("file://", 0) != 0) {
            return message;
        }
        std::string scriptPath = scriptUrl.substr(strlen("file://"));
        auto lastSlash = scriptPath.rfind('/');
        if (lastSlash == std::string::npos) {
            return message;
        }
        path = scriptPath.substr(0, lastSlash + 1) + sourceMapURL;
    }

    if (path.empty()) {
        return message;
    }

    params["sourceMapURL"] = kSourceMapScheme + path;
    return JsonDump(parsed);
}

}  // namespace

JsV8InspectorClient::JsV8InspectorClient(v8::Isolate* isolate)
    : isolate_(isolate),
      inspector_(nullptr),
      session_(nullptr),
      connection_(nullptr),
      context_(),
      terminated_(true),
      running_nested_loop_(false),
      isConnected_(false) {
    JEnv env;

    inspectorClass_ = env.FindClass("com/tns/AndroidJsV8Inspector");
    assert(inspectorClass_ != nullptr);

    sendMethod_ = env.GetStaticMethodID(inspectorClass_, "send", "(Ljava/lang/Object;Ljava/lang/String;)V");
    assert(sendMethod_ != nullptr);

    sendToDevToolsConsoleMethod_ = env.GetStaticMethodID(inspectorClass_, "sendToDevToolsConsole", "(Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;)V");
    assert(sendToDevToolsConsoleMethod_ != nullptr);

    getInspectorMessageMethod_ = env.GetStaticMethodID(inspectorClass_, "getInspectorMessage", "(Ljava/lang/Object;)Ljava/lang/String;");
    assert(getInspectorMessageMethod_ != nullptr);
}

void JsV8InspectorClient::connect(jobject connection) {
    JEnv env;
    connection_ = env.NewGlobalRef(connection);
    isConnected_ = true;
}

void JsV8InspectorClient::scheduleBreak() {
    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);
    auto context = Runtime::GetRuntime(isolate_)->GetContext();
    Context::Scope context_scope(context);

    session_->schedulePauseOnNextStatement({}, {});
}

void JsV8InspectorClient::createInspectorSession() {
    session_ = inspector_->connect(JsV8InspectorClient::contextGroupId, this, {});
}

void JsV8InspectorClient::disconnect() {
    {
        // Streams only have meaning to the frontend that opened them; drop any
        // the old connection never read to the end (or never IO.close-d).
        std::lock_guard<std::mutex> lock(resourceStreamsMutex_);
        resourceStreams_.clear();
    }

    if (connection_ == nullptr) {
        return;
    }

    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);

    session_->resume();
    session_.reset();

    JEnv env;
    env.DeleteGlobalRef(connection_);
    connection_ = nullptr;
    isConnected_ = false;

    createInspectorSession();
}


void JsV8InspectorClient::dispatchMessage(const std::string& message) {
    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);
    auto context = Runtime::GetRuntime(isolate_)->GetContext();
    Context::Scope context_scope(context);

    std::vector<uint16_t> vector = tns::Util::ToVector(message);
    StringView messageView(vector.data(), vector.size());
    bool success;

    /*
    // livesync uses the inspector socket for HMR/LiveSync...
    if(message.find("Page.reload") != std::string::npos) {
        success = tns::LiveSync(this->isolate_);
        if (!success) {
            NSLog(@"LiveSync failed");
        }
        // todo: should we return here, or is it OK to pass onto a possible Page.reload domain handler?
    }
    */


    if(message.find("Tracing.start") != std::string::npos) {
        tracing_agent_->start();

        // echo back the request to notify frontend the action was a success
        // todo: send an empty response for the incoming message id instead.
        this->sendNotification(StringBuffer::create(messageView));
        return;
    }

    if(message.find("Tracing.end") != std::string::npos) {
        tracing_agent_->end();
        std::string res = tracing_agent_->getLastTrace();
        tracing_agent_->SendToDevtools(context, res);
        return;
    }


    // parse incoming message as JSON
    Local<Value> arg;
    success = v8::JSON::Parse(context, tns::ArgConverter::ConvertToV8String(isolate_, message)).ToLocal(&arg);

    // stop processing invalid messages
    if(!success) {

      //  NSLog(@"Inspector failed to parse incoming message: %s", message.c_str());
        // ignore failures to parse.
        return;
    }




    // Pass incoming message to a registerd domain handler if any
    if(!arg.IsEmpty() && arg->IsObject()) {
        Local<Object> domainDebugger;
        Local<Object> argObject = arg.As<Object>();
        Local<v8::Function> domainMethodFunc = GetDebuggerFunctionFromObject(context, argObject, domainDebugger);

        Local<Value> result;
        success = this->CallDomainHandlerFunction(context, domainMethodFunc, argObject, domainDebugger, result);

        if(success) {
            auto requestId = arg.As<Object>()->Get(context, tns::ArgConverter::ConvertToV8String(isolate_, "id")).ToLocalChecked();
            auto returnString = GetReturnMessageFromDomainHandlerResult(result, requestId);

            if(returnString.size() > 0) {
                std::vector<uint16_t> vector_ = tns::Util::ToVector(returnString);
                StringView messageView_(vector_.data(), vector_.size());
                auto msg = StringBuffer::create(messageView_);
                this->sendNotification(std::move(msg));
            }
            return;
        }
    }



    doDispatchMessage(message);
    // TODO: check why this is needed (it should trigger automatically when script depth is 0)
    isolate_->PerformMicrotaskCheckpoint();
}

void JsV8InspectorClient::runMessageLoopOnPause(int context_group_id) {
    if (running_nested_loop_) {
        return;
    }

    JEnv env;

    terminated_ = false;
    running_nested_loop_ = true;
    isPausedNestedLoop_.store(true, std::memory_order_release);
    while (!terminated_) {
        JniLocalRef msg(env.CallStaticObjectMethod(inspectorClass_, getInspectorMessageMethod_, connection_));
        if (!msg.IsNull()) {
            auto inspectorMessage = ArgConverter::jstringToString(msg);
            doDispatchMessage(inspectorMessage);
        }

        while (v8::platform::PumpMessageLoop(Runtime::platform, isolate_)) {
        }
    }
    isPausedNestedLoop_.store(false, std::memory_order_release);
    terminated_ = false;
    running_nested_loop_ = false;
}

void JsV8InspectorClient::quitMessageLoopOnPause() {
    terminated_ = true;
}

v8::Local<v8::Context> JsV8InspectorClient::ensureDefaultContextInGroup(int contextGroupId) {
    return context_.Get(isolate_);
}

void JsV8InspectorClient::doDispatchMessage(const std::string& message) {
    if (session_ == nullptr) {
        return;
    }

    v8_inspector::StringView message_view = stringToStringView(message);
    session_->dispatchProtocolMessage(message_view);
}

// Chrome DevTools fetches source maps through the target: it sends
// Network.loadNetworkResource for the resolved sourceMappingURL and reads the
// returned stream with IO.read/IO.close. Neither domain is implemented by
// V8's inspector, so they are served here, on the websocket read thread --
// the main thread may be blocked in the pause message loop or busy running
// JS. None of these handlers touch V8.
std::string JsV8InspectorClient::handleMessageOnSocketThread(const std::string& message) {
    auto parsed = json::parse(message, nullptr, false);
    if (parsed.is_discarded() || !parsed.is_object()) {
        return "";
    }

    std::string method = parsed.contains("method") && parsed["method"].is_string()
                         ? parsed["method"].get<std::string>()
                         : "";
    long long msgId = parsed.contains("id") && parsed["id"].is_number()
                      ? parsed["id"].get<long long>()
                      : -1;
    std::string sessionId = parsed.contains("sessionId") && parsed["sessionId"].is_string()
                            ? parsed["sessionId"].get<std::string>()
                            : "";

    if (method == "Network.loadNetworkResource") {
        std::string url;
        if (parsed.contains("params") && parsed["params"].contains("url") && parsed["params"]["url"].is_string()) {
            url = parsed["params"]["url"].get<std::string>();
        }
        return HandleLoadNetworkResource(msgId, url, sessionId);
    }

    if (method == "IO.read" || method == "IO.close") {
        std::string handle;
        int size = 0;
        if (parsed.contains("params")) {
            const auto& params = parsed["params"];
            if (params.contains("handle") && params["handle"].is_string()) {
                handle = params["handle"].get<std::string>();
            }
            if (params.contains("size") && params["size"].is_number_integer()) {
                size = params["size"].get<int>();
            }
        }
        return method == "IO.read"
               ? HandleIORead(msgId, handle, size, sessionId)
               : HandleIOClose(msgId, handle, sessionId);
    }

    // Debugger.pause needs to interrupt V8 even if the main thread is busy
    // executing JS; RequestInterrupt fires at the next safe bytecode boundary.
    // The message still flows through the dispatch queue so V8 answers the
    // request id. Skip it while paused in the nested message loop: no JS is
    // running, so the interrupt would only fire after the user resumes,
    // causing a spurious re-pause.
    if (method == "Debugger.pause" && !isPausedNestedLoop_.load(std::memory_order_acquire)) {
        isolate_->RequestInterrupt(
            [](v8::Isolate* isolate, void* data) {
                auto* client = static_cast<JsV8InspectorClient*>(data);
                if (client->session_ != nullptr) {
                    client->session_->schedulePauseOnNextStatement({}, {});
                }
            },
            this);
    }

    return "";
}

std::string JsV8InspectorClient::HandleLoadNetworkResource(long long msgId, const std::string& url, const std::string& sessionId) {
    std::string path;
    if (url.rfind(kSourceMapScheme, 0) == 0) {
        path = url.substr(strlen(kSourceMapScheme));
    } else if (url.rfind("file://", 0) == 0) {
        path = url.substr(strlen("file://"));
    } else {
        // Reply with a protocol error (not success:false) so DevTools falls
        // back to loading the resource from the frontend host, which is the
        // pre-existing behavior for http(s) urls.
        json error = {
            {"id", msgId},
            {"error", {{"code", -32000}, {"message", "Unsupported URL scheme"}}}
        };
        return FinishResponse(error, sessionId);
    }

    std::string content;
    bool loaded = false;

    if (!path.empty()) {
        std::vector<std::string> candidates;
        auto addCandidate = [&candidates](const std::string& candidate) {
            if (!candidate.empty() &&
                    std::find(candidates.begin(), candidates.end(), candidate) == candidates.end()) {
                candidates.push_back(candidate);
            }
        };
        auto addVariants = [&addCandidate](const std::string& p) {
            addCandidate(p);
            if (p[0] != '/') {
                addCandidate(Constants::APP_ROOT_FOLDER_PATH + p);
            }
            // /data/data/<pkg> and /data/user/0/<pkg> alias the same files;
            // urls may use either spelling depending on who built them.
            constexpr const char* kDataData = "/data/data/";
            constexpr const char* kDataUser0 = "/data/user/0/";
            if (p.rfind(kDataData, 0) == 0) {
                addCandidate(kDataUser0 + p.substr(strlen(kDataData)));
            } else if (p.rfind(kDataUser0, 0) == 0) {
                addCandidate(kDataData + p.substr(strlen(kDataUser0)));
            }
        };

        addVariants(path);
        // fall back to percent-decoded variants for urls the frontend encoded
        std::string decoded = ada::unicode::percent_decode(path, path.find('%'));
        if (decoded != path) {
            addVariants(decoded);
        }

        for (const auto& candidate : candidates) {
            int length = 0;
            auto* data = static_cast<uint8_t*>(File::ReadBinary(candidate, length));
            if (data != nullptr) {
                content.assign(reinterpret_cast<const char*>(data), length);
                delete[] data;
                loaded = true;
                break;
            }
        }
    }

    json resource;
    if (loaded) {
        std::string handle;
        {
            std::lock_guard<std::mutex> lock(resourceStreamsMutex_);
            handle = "ns-network-resource-" + std::to_string(++lastStreamId_);
            resourceStreams_[handle] = {std::move(content), 0};
        }
        resource = {{"success", true}, {"httpStatusCode", 200}, {"stream", handle}};
    } else {
        resource = {{"success", false},
                    {"netError", -6},
                    {"netErrorName", "net::ERR_FILE_NOT_FOUND"},
                    {"httpStatusCode", 404}};
    }

    json response = {{"id", msgId}, {"result", {{"resource", resource}}}};
    return FinishResponse(response, sessionId);
}

std::string JsV8InspectorClient::HandleIORead(long long msgId, const std::string& handle, int size, const std::string& sessionId) {
    std::lock_guard<std::mutex> lock(resourceStreamsMutex_);
    auto it = resourceStreams_.find(handle);
    if (it == resourceStreams_.end()) {
        json error = {
            {"id", msgId},
            {"error", {{"code", -32602}, {"message", "Invalid stream handle"}}}
        };
        return FinishResponse(error, sessionId);
    }

    ResourceStream& stream = it->second;
    constexpr size_t kDefaultChunkSize = 1024 * 1024;
    size_t chunkSize = size > 0 ? static_cast<size_t>(size) : kDefaultChunkSize;
    size_t remaining = stream.data.size() - stream.offset;
    chunkSize = std::min(chunkSize, remaining);

    json result;
    if (chunkSize == 0) {
        // DevTools ignores any data sent alongside eof, so only signal it once
        // the whole stream has been delivered.
        result = {{"data", ""}, {"eof", true}, {"base64Encoded", false}};
    } else {
        // Base64 keeps arbitrary file bytes intact through the JSON transport.
        std::string encoded = Base64Encode(stream.data.data() + stream.offset, chunkSize);
        stream.offset += chunkSize;
        result = {{"data", encoded}, {"eof", false}, {"base64Encoded", true}};
    }

    json response = {{"id", msgId}, {"result", result}};
    return FinishResponse(response, sessionId);
}

std::string JsV8InspectorClient::HandleIOClose(long long msgId, const std::string& handle, const std::string& sessionId) {
    {
        std::lock_guard<std::mutex> lock(resourceStreamsMutex_);
        resourceStreams_.erase(handle);
    }
    json response = {{"id", msgId}, {"result", json::object()}};
    return FinishResponse(response, sessionId);
}

void JsV8InspectorClient::sendResponse(int callId, std::unique_ptr<StringBuffer> message) {
    sendNotification(std::move(message));
}

void JsV8InspectorClient::sendNotification(std::unique_ptr<StringBuffer> message) {
    if (connection_ == nullptr) {
        return;
    }

    const std::string msg = MaybeRewriteSourceMapURL(stringViewToString(isolate_, message->string()));

    JEnv env;
    // TODO: Pete: Check if we can use a wide (utf 16) string here
    JniLocalRef str(env.NewStringUTF(msg.c_str()));
    env.CallStaticVoidMethod(inspectorClass_, sendMethod_, connection_, (jstring) str);
}

void JsV8InspectorClient::flushProtocolNotifications() {
}

void JsV8InspectorClient::init() {
    if (inspector_ != nullptr) {
        return;
    }

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);
    v8::Local<Context> context = Runtime::GetRuntime(isolate_)->GetContext();

    inspector_ = V8Inspector::create(isolate_, this);

    inspector_->contextCreated(v8_inspector::V8ContextInfo(context, JsV8InspectorClient::contextGroupId, {}));

    context_.Reset(isolate_, context);

    createInspectorSession();

    tracing_agent_.reset(new tns::inspector::TracingAgentImpl());

    try {
        this->registerModules();
    } catch (NativeScriptException& e) {
        // we don't want to throw if registering modules failed.
        // e.ReThrowToV8();
    }
}

JsV8InspectorClient* JsV8InspectorClient::GetInstance() {
    if (instance == nullptr) {
        instance = new JsV8InspectorClient(Runtime::GetRuntime(0)->GetIsolate());
    }

    return instance;
}

void JsV8InspectorClient::inspectorSendEventCallback(const FunctionCallbackInfo<Value>& args) {
    if ((instance == nullptr) || (instance->connection_ == nullptr)) {
        return;
    }
    Isolate* isolate = args.GetIsolate();

    Local<v8::String> arg = args[0].As<v8::String>();
    std::string message = ArgConverter::ConvertToString(arg);

    JEnv env;
    // TODO: Pete: Check if we can use a wide (utf 16) string here
    JniLocalRef str(env.NewStringUTF(message.c_str()));
    env.CallStaticVoidMethod(instance->inspectorClass_, instance->sendMethod_, instance->connection_, (jstring) str);

    // TODO: ios uses this method, but doesn't work on android
    // so I'm just sending directly to the socket (which seems to work)
    instance->dispatchMessage(message);
}

void JsV8InspectorClient::sendToFrontEndCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if ((instance == nullptr) || (instance->connection_ == nullptr)) {
        return;
    }

    try {
        auto isolate = args.GetIsolate();
        if ((args.Length() > 0) && args[0]->IsString()) {
            auto context = isolate->GetCurrentContext();
            std::string message = ArgConverter::ConvertToString(args[0]->ToString(context).ToLocalChecked());

            std::string level = "log";
            if (args.Length() > 1  && args[1]->IsString()) {
                level = ArgConverter::ConvertToString(args[1]->ToString(context).ToLocalChecked());
            }

            JEnv env;
            JniLocalRef str(env.NewStringUTF(message.c_str()));
            JniLocalRef lev(env.NewStringUTF(level.c_str()));
            env.CallStaticVoidMethod(instance->inspectorClass_, instance->sendToDevToolsConsoleMethod_, instance->connection_, (jstring) str, (jstring)lev);
        }
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void JsV8InspectorClient::consoleLogCallback(Isolate* isolate, ConsoleAPIType method, const std::vector<v8::Local<v8::Value>>& args) {
    if (!inspectorIsConnected()) {
        return;
    }

    // Note, here we access private V8 API
    auto* impl = reinterpret_cast<v8_inspector::V8InspectorImpl*>(instance->inspector_.get());
    auto* session = reinterpret_cast<v8_inspector::V8InspectorSessionImpl*>(instance->session_.get());

    std::unique_ptr<V8StackTraceImpl> stack = impl->debugger()->captureStackTrace(false);

    v8::Local<v8::Context> context = instance->context_.Get(instance->isolate_);
    const int contextId = V8ContextInfo::executionContextId(context);

    std::unique_ptr<v8_inspector::V8ConsoleMessage> msg =
        v8_inspector::V8ConsoleMessage::createForConsoleAPI(
            context, contextId, contextGroupId, impl, instance->currentTimeMS(),
            method, args, String16{}, std::move(stack));

    session->runtimeAgent()->messageAdded(msg.get());
}

void JsV8InspectorClient::registerDomainDispatcherCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    std::string domain = tns::ArgConverter::ToString(isolate, args[0].As<v8::String>());
    auto it = Domains.find(domain);
    if (it == Domains.end()) {
        Local<v8::Function> domainCtorFunc = args[1].As<v8::Function>();
        Local<Context> context = isolate->GetCurrentContext();
        Local<Value> ctorArgs[0];
        Local<Value> domainInstance;
        bool success = domainCtorFunc->CallAsConstructor(context, 0, ctorArgs).ToLocal(&domainInstance);
        assert(success && domainInstance->IsObject());

        Local<Object> domainObj = domainInstance.As<Object>();
        Persistent<Object>* poDomainObj = new Persistent<Object>(isolate, domainObj);
        Domains.emplace(domain, poDomainObj);
    }
}

void JsV8InspectorClient::inspectorTimestampCallback(const FunctionCallbackInfo<Value>& args) {
    double timestamp = std::chrono::seconds(std::chrono::seconds(std::time(NULL))).count();
    args.GetReturnValue().Set(timestamp);
}

void JsV8InspectorClient::registerModules() {
    DEBUG_WRITE("Registering inspector modules");
    Isolate* isolate = isolate_;
    auto rt = Runtime::GetRuntime(isolate);
    v8::Locker l(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto ctx = rt->GetContext();
    v8::Context::Scope context_scope(ctx);

    Local<Context> context = isolate->GetEnteredOrMicrotaskContext();
    Local<Object> global = context->Global();
    Local<Object> inspectorObject = Object::New(isolate);

    bool success;
    Local<v8::Function> func;

    // __inspector
    success = global->Set(context, ArgConverter::ConvertToV8String(isolate, "__inspector"), inspectorObject).FromMaybe(false);
    assert(success);

    // __registerDomainDispatcher
    success = v8::Function::New(context, registerDomainDispatcherCallback).ToLocal(&func);
    assert(success);
    success = global->Set(context, ArgConverter::ConvertToV8String(isolate, "__registerDomainDispatcher"), func).FromMaybe(false);
    assert(success);

    // __inspectorSendEvent
    Local<External> data = External::New(isolate, this);
    success = v8::Function::New(context, inspectorSendEventCallback, data).ToLocal(&func);
    assert(success);
    success = global->Set(context, ArgConverter::ConvertToV8String(isolate, "__inspectorSendEvent"), func).FromMaybe(false);
    assert(success);

    // __inspectorTimestamp
    success = v8::Function::New(context, inspectorTimestampCallback).ToLocal(&func);
    assert(success);
    success = global->Set(context, ArgConverter::ConvertToV8String(isolate, "__inspectorTimestamp"), func).FromMaybe(false);
    assert(success);

    TryCatch tc(isolate);
    Runtime::GetRuntime(isolate)->RunModule("inspector_modules");

    if(tc.HasCaught()) {
        throw NativeScriptException(tc, "Error loading inspector modules");
    }
}


void JsV8InspectorClient::InspectorIsConnectedGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    info.GetReturnValue().Set(JsV8InspectorClient::GetInstance()->isConnected_);
}

JsV8InspectorClient* JsV8InspectorClient::instance = nullptr;


bool JsV8InspectorClient::CallDomainHandlerFunction(Local<Context> context, Local<Function> domainMethodFunc, const Local<Object>& arg, Local<Object>& domainDebugger, Local<Value>& result) {
    if(domainMethodFunc.IsEmpty() || !domainMethodFunc->IsFunction()) {
        return false;
    }

    bool success;
    Isolate* isolate = this->isolate_;
    TryCatch tc(isolate);

    Local<Value> params;
    success = arg.As<Object>()->Get(context, tns::ArgConverter::ConvertToV8String(isolate, "params")).ToLocal(&params);

    if(!success) {
        return false;
    }

    Local<Value> args[2] = { params, arg };
    success = domainMethodFunc->Call(context, domainDebugger, 2, args).ToLocal(&result);

    if (tc.HasCaught()) {
        std::string error = tns::ArgConverter::ToString(isolate_, tc.Message()->Get());

        // backwards compatibility
        if(error.find("may be enabled at a time") != std::string::npos) {
            // not returning false here because we are catching bogus errors from core...
            // Uncaught Error: One XXX may be enabled at a time...
            result = v8::Boolean::New(isolate, true);
            return true;
        }

        // log any other errors - they are caught, but still make them visible to the user.
       // tns::LogError(isolate, tc);

        return false;
    }

    return success;
}

std::string JsV8InspectorClient::GetReturnMessageFromDomainHandlerResult(const Local<Value>& result, const Local<Value>& requestId) {
    if(result.IsEmpty() || !(result->IsBoolean() || result->IsObject() || result->IsNullOrUndefined())) {
        return "";
    }

    Isolate* isolate = this->isolate_;

    if(!result->IsObject()) {
        // if there return value is a "true" boolean or undefined/null we send back an "ack" response with an empty result object
        if(result->IsNullOrUndefined() || result->BooleanValue(isolate_)) {
            return "{ \"id\":" + tns::ArgConverter::ToString(isolate_, requestId) + ", \"result\": {} }";
        }

        return "";
    }

    v8::Local<Context> context = Runtime::GetRuntime(isolate_)->GetContext();
    Local<Object> resObject = result.As<v8::Object>();
    Local<Value> stringified;

    bool success = true;
    // already a { result: ... } object
    if(resObject->Has(context, tns::ArgConverter::ToV8String(isolate, "result")).ToChecked()) {
        success = JSON::Stringify(context, result).ToLocal(&stringified);
    } else {
        // backwards compatibility - we wrap the response in a new object with the { id, result } keys
        // since the returned response only contained the result part.
        Context::Scope context_scope(context);

        Local<Object> newResObject = v8::Object::New(isolate);
        success = success && newResObject->Set(context, tns::ArgConverter::ToV8String(isolate, "id"), requestId).ToChecked();
        success = success && newResObject->Set(context, tns::ArgConverter::ToV8String(isolate, "result"), resObject).ToChecked();
        success = success && JSON::Stringify(context, newResObject).ToLocal(&stringified);
    }

    if(!success) {
        return "";
    }

    return tns::ArgConverter::ToString(isolate, stringified);
}

std::map<std::string, v8::Persistent<v8::Object>*> JsV8InspectorClient::Domains;

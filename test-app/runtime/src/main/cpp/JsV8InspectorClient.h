#ifndef JSV8INSPECTORCLIENT_H_
#define JSV8INSPECTORCLIENT_H_

#include <atomic>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <src/inspector/v8-console-message.h>
#include "v8.h"
#include "JEnv.h"
#include "v8-inspector.h"
#include "v8_inspector/ns-v8-tracing-agent-impl.h"

using namespace v8_inspector;

namespace tns {
class WorkerInspectorClient;

class JsV8InspectorClient : V8InspectorClient, v8_inspector::V8Inspector::Channel {
    public:
        static JsV8InspectorClient* GetInstance();

        // Non-constructing variant for worker threads: returns nullptr until a
        // main-thread entry point has created the client.
        static JsV8InspectorClient* GetInstanceIfCreated();

        void init();
        void connect(jobject connection);
        void scheduleBreak();
        void disconnect();
        void dispatchMessage(const std::string& message);

        // Any thread. Rewrites source map urls and writes to the frontend
        // socket; serializes against connect/disconnect.
        void SendToFrontend(const std::string& message);

        // Worker target management (Target domain, flat-session protocol).
        // Register/Unregister run on the worker's own thread; SchedulePauseInWorker
        // runs on the worker thread from a V8 interrupt.
        void RegisterWorkerTarget(int workerId, WorkerInspectorClient* client);
        void UnregisterWorkerTarget(int workerId);
        void SchedulePauseInWorker(int workerId);

        // Runs on the websocket read thread. Returns true when the message was
        // handled there (response, possibly empty when replies were already
        // sent or none is needed, goes back to the socket); false routes the
        // message through the normal main-thread dispatch queue.
        bool handleMessageOnSocketThread(const std::string& message, std::string& response);

        void registerModules();

        // Overrides of V8Inspector::Channel
        void sendResponse(int callId, std::unique_ptr<StringBuffer> message) override;
        void sendNotification(const std::unique_ptr<StringBuffer> message) override;
        void flushProtocolNotifications() override;

        static void sendToFrontEndCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void consoleLogCallback(v8::Isolate* isolate, ConsoleAPIType method, const std::vector<v8::Local<v8::Value>>& args);
        static void inspectorSendEventCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void registerDomainDispatcherCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void inspectorTimestampCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        // Overrides of V8InspectorClient
        void runMessageLoopOnPause(int context_group_id) override;
        void quitMessageLoopOnPause() override;

        static bool inspectorIsConnected() {
            return JsV8InspectorClient::GetInstance()->isConnected_;
        }

        static std::map<std::string, v8::Persistent<v8::Object>*> Domains;

    private:
        JsV8InspectorClient(v8::Isolate* isolate);

        // Override of V8InspectorClient
        v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

        void createInspectorSession();
        void doDispatchMessage(const std::string& message);

        static void InspectorIsConnectedGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static std::atomic<JsV8InspectorClient*> instance;
        static constexpr int contextGroupId = 1;

        // Streams backing Network.loadNetworkResource responses, read by the
        // frontend through IO.read/IO.close (how Chrome DevTools fetches source
        // maps from the target). Accessed from the websocket read thread.
        struct ResourceStream {
            std::string data;
            size_t offset = 0;
        };

        // Live worker inspectors, keyed by their flat-protocol sessionId
        // ("NS_WORKER_<id>"). Entries are added/removed from worker threads and
        // read from the socket thread.
        struct WorkerTarget {
            int workerId;
            WorkerInspectorClient* client;
            bool announced = false;
        };

        void RouteToWorker(const std::string& sessionId, const std::string& method,
                           long long msgId, const std::string& message);
        void AnnounceWorkerTargets();

        std::map<std::string, WorkerTarget> workerTargets_;
        std::mutex workerTargetsMutex_;
        bool autoAttach_ = false;  // guarded by workerTargetsMutex_

        // Lock order: workerTargetsMutex_ -> connectionMutex_ (registry walks
        // send while holding the registry lock); never the other way around.
        std::mutex connectionMutex_;

        // Source map delivery to Chrome DevTools (Network.loadNetworkResource +
        // IO domain). V8's inspector doesn't implement these embedder domains.
        std::string HandleLoadNetworkResource(long long msgId, const std::string& url, const std::string& sessionId);
        std::string HandleIORead(long long msgId, const std::string& handle, int size, const std::string& sessionId);
        std::string HandleIOClose(long long msgId, const std::string& handle, const std::string& sessionId);

        std::map<std::string, ResourceStream> resourceStreams_;
        std::mutex resourceStreamsMutex_;
        int lastStreamId_ = 0;
        std::atomic<bool> isPausedNestedLoop_{false};

        std::unique_ptr<tns::inspector::TracingAgentImpl> tracing_agent_;
        v8::Isolate* isolate_;
        std::unique_ptr<V8Inspector> inspector_;
        v8::Persistent<v8::Context> context_;
        std::unique_ptr<V8InspectorSession> session_;
        jclass inspectorClass_;
        jmethodID sendMethod_;
        jmethodID getInspectorMessageMethod_;
        jmethodID sendToDevToolsConsoleMethod_;
        jobject connection_;
        bool running_nested_loop_ : 1;
        bool terminated_ : 1;
        // Read from worker threads (target announcements); must not share a
        // memory location with the bitfields above, which the main thread
        // writes.
        std::atomic<bool> isConnected_;


    // {N} specific helpers
    bool CallDomainHandlerFunction(v8::Local<v8::Context> context,
                                   v8::Local<v8::Function> domainMethodFunc,
                                   const v8::Local<v8::Object>& arg,
                                   v8::Local<v8::Object>& domainDebugger,
                                   v8::Local<v8::Value>& result);
    std::string GetReturnMessageFromDomainHandlerResult(const v8::Local<v8::Value>& result, const v8::Local<v8::Value>& requestId);
};
}

#endif /* JSV8INSPECTORCLIENT_H_ */

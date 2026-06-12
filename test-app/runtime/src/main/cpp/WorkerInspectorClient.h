#ifndef WORKERINSPECTORCLIENT_H_
#define WORKERINSPECTORCLIENT_H_

#include <android/looper.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include <src/inspector/v8-console-message.h>
#include "v8.h"
#include "v8-inspector.h"

namespace tns {

/*
 * V8 inspector for a single worker isolate, exposed to Chrome DevTools as a
 * child target ("Target.attachedToTarget") and addressed with flat-session
 * CDP messages (a top-level "sessionId" field). One instance per worker,
 * mirroring the iOS runtime's WorkerInspectorClient.
 *
 * Threading: constructed, dispatched into, and destroyed on the worker's own
 * thread (V8's inspector is not thread-safe). Other threads interact only
 * through PushMessage/NotifyTerminating/RequestPauseInterrupt. Incoming
 * messages are queued and drained via an eventfd registered on the worker's
 * ALooper (the same mechanism as the worker's message inbox); while paused, a
 * nested loop on the worker thread pumps the same queue (the looper is NOT
 * re-entered, so postMessage deliveries stay queued during a pause, matching
 * Chrome's semantics).
 */
class WorkerInspectorClient final : public v8_inspector::V8InspectorClient,
                                    public v8_inspector::V8Inspector::Channel {
    public:
        // Worker thread, with the worker isolate locked and its context created.
        WorkerInspectorClient(int workerId, v8::Isolate* isolate, ALooper* workerLooper,
                              const std::string& url);
        ~WorkerInspectorClient() override;

        int WorkerId() const {
            return workerId_;
        }
        const std::string& SessionId() const {
            return sessionId_;
        }
        const std::string& TargetId() const {
            return targetId_;
        }
        const std::string& Url() const {
            return url_;
        }

        // Any thread. Queues a CDP message (already stripped of routing concerns)
        // and wakes the worker looper / a nested pause loop.
        void PushMessage(const std::string& message);

        // Any thread. Unblocks a paused worker and makes it drop all inspector
        // work; used by WorkerWrapper::Terminate together with TerminateExecution.
        void NotifyTerminating();

        // Any thread (with the worker isolate guaranteed alive). Schedules a pause
        // at the next statement even if the worker is busy executing JS.
        void RequestPauseInterrupt();

        // Worker thread (from the interrupt requested above).
        void SchedulePauseFromInterrupt();

        // Any thread. True while the worker sits in its nested pause loop — used
        // by the root client to skip the pause interrupt (it would only fire
        // after resume, causing a spurious re-pause).
        bool IsRunningPauseLoop() const {
            return runningPauseLoop_.load(std::memory_order_acquire);
        }

        // Worker thread. Mirrors JsV8InspectorClient::consoleLogCallback for this
        // isolate.
        void consoleLog(v8_inspector::ConsoleAPIType method,
                        const std::vector<v8::Local<v8::Value>>& args);

        // Internal control message pushed by the root client on frontend
        // reconnect; resumes the worker if paused and recreates its session.
        static constexpr const char* kResetSessionMessage = "{\"__nsInternal\":\"resetSession\"}";

        // Overrides of V8Inspector::Channel
        void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) override;
        void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;
        void flushProtocolNotifications() override;

        // Overrides of V8InspectorClient
        void runMessageLoopOnPause(int contextGroupId) override;
        void quitMessageLoopOnPause() override;

    private:
        static constexpr int contextGroupId = 1;

        static int InspectorMessagesCallback(int fd, int events, void* data);

        void DrainIncoming();
        std::string PopMessage();
        void DispatchOne(const std::string& message);
        void HandleResetRequest();
        void DoResetSession();
        void MaybeResetSession();
        void SendWrapped(const std::string& message);

        v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

        int workerId_;
        std::string sessionId_;
        std::string targetId_;
        std::string url_;
        v8::Isolate* isolate_;
        ALooper* workerLooper_;
        int eventFd_ = -1;

        std::unique_ptr<v8_inspector::V8Inspector> inspector_;
        std::unique_ptr<v8_inspector::V8InspectorSession> session_;
        v8::Persistent<v8::Context> context_;

        std::queue<std::string> incoming_;
        std::mutex incomingMutex_;
        std::mutex messageArrivedMutex_;
        std::condition_variable messageArrived_;

        std::atomic<bool> dying_{false};
        std::atomic<bool> pauseTerminated_{false};
        std::atomic<bool> runningPauseLoop_{false};  // written on the worker thread only
        bool pendingReset_ = false;                  // worker thread only
};

}  // namespace tns

#endif /* WORKERINSPECTORCLIENT_H_ */

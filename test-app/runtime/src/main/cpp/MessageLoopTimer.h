#ifndef MESSAGELOOPTIMER_H
#define MESSAGELOOPTIMER_H

#include "v8.h"

namespace tns {

class MessageLoopTimer {
public:
    void Init(v8::Local<v8::Context> context);
private:
    bool m_isRunning;
    int m_fd[2];

    void RegisterStartStopFunctions(v8::Local<v8::Context> context);
    static void StartCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void StopCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
    static int PumpMessageLoopCallback(int fd, int events, void* data);
    static void WorkerThreadRun(MessageLoopTimer* timer);
};

}

#endif //MESSAGELOOPTIMER_H

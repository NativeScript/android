package com.tns;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

/**
 * Dedicated per-runtime Handler carrying the EventLoop's ordered lane: work
 * that must run as a macrotask on the runtime thread, strictly FIFO-ordered
 * with Handler.post runnables and JS timers on the same looper. One instance
 * per isolate, created and used exclusively from native code (EventLoop.cpp).
 *
 * Messages are anonymous "task due" tokens: native code owns the actual task
 * queue and runs at most one due task per token, so a token never names a
 * task and a leftover token is a cheap no-op.
 */
final class EventLoopHandler extends Handler {
    private static final int MSG_RUN_TASK = 1;

    private final long nativeRunnerPtr;
    private boolean released;

    // constructed from native code (EventLoop::BindToCurrentThread)
    EventLoopHandler(long nativeRunnerPtr) {
        super(requireLooper());
        this.nativeRunnerPtr = nativeRunnerPtr;
    }

    private static Looper requireLooper() {
        Looper looper = Looper.myLooper();
        if (looper == null) {
            throw new IllegalStateException(
                "EventLoopHandler requires a prepared Looper on the runtime thread");
        }
        return looper;
    }

    /**
     * Enqueues an anonymous "task due" token at an absolute uptimeMillis.
     * Immediate tasks pass the current uptime; delayed tasks pass their due
     * time. Callable from any thread.
     */
    @RuntimeCallable
    void post(long uptimeMillis) {
        sendMessageAtTime(obtainMessage(MSG_RUN_TASK), uptimeMillis);
    }

    /**
     * Called from EventLoop::Shutdown on this handler's own thread. After
     * this no token can fire into the (about to be freed) native loop.
     */
    @RuntimeCallable
    void release() {
        released = true;
        removeCallbacksAndMessages(null); // safe: this handler is tasks-only
    }

    @Override
    public void handleMessage(Message msg) {
        if (released || msg.what != MSG_RUN_TASK) {
            return;
        }
        nativeRunTask(nativeRunnerPtr);
    }

    private static native void nativeRunTask(long nativeRunnerPtr);
}

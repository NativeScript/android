package com.tns;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import java.util.concurrent.atomic.AtomicBoolean;

import dalvik.annotation.optimization.CriticalNative;

/**
 * Dedicated per-runtime Handler carrying the EventLoop's ordered lane: work
 * that must run as a macrotask on the runtime thread, strictly FIFO-ordered
 * with Handler.post runnables and JS timers on the same looper. One instance
 * per isolate, created and used exclusively from native code (EventLoop.cpp).
 *
 * Messages are anonymous "task due" tokens: native code owns the actual task
 * queue and runs at most one due item per token, so a token never names the
 * work it will run. Two optional cancellation carriers ride a token:
 *
 * - a native claim cell (arg1/arg2 = packed 64-bit cell word) for short
 *   timers: handleMessage claims it through a @CriticalNative CAS before
 *   entering the runtime, so a token whose timer was cleared dies here in
 *   nanoseconds without acquiring the isolate Locker;
 * - an AtomicBoolean peer (obj) for long timers: claimed here in Java, and
 *   usable as removeMessages identity so a cleared long timer produces no
 *   wakeup at all. The peer and its Message are GC-owned, which is what makes
 *   the remove-vs-in-flight race harmless.
 *
 * Exactly one of the two (or neither) is set per token.
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
     * Callable from any thread.
     */
    @RuntimeCallable
    void post(long uptimeMillis) {
        sendMessageAtTime(obtainMessage(MSG_RUN_TASK), uptimeMillis);
    }

    /**
     * Token carrying a native claim cell word (split across arg1/arg2; a
     * Message has no long field). Callable from any thread.
     */
    @RuntimeCallable
    void postToken(long uptimeMillis, int cellHi, int cellLo) {
        Message msg = obtainMessage(MSG_RUN_TASK);
        msg.arg1 = cellHi;
        msg.arg2 = cellLo;
        sendMessageAtTime(msg, uptimeMillis);
    }

    /**
     * Token carrying an AtomicBoolean claim peer, returned so native code can
     * cancel it later. Callable from any thread.
     */
    @RuntimeCallable
    Object postIdentified(long uptimeMillis) {
        AtomicBoolean peer = new AtomicBoolean(false);
        Message msg = obtainMessage(MSG_RUN_TASK, peer);
        sendMessageAtTime(msg, uptimeMillis);
        return peer;
    }

    /**
     * Cancels an identified token: the CAS atomically decides against a
     * concurrent dispatch claim, and on success the queued message (if it is
     * still queued - removal may race a dequeue, which the claim makes
     * harmless) is removed so no wakeup happens. Returns whether this call
     * won the token. Callable from any thread.
     */
    @RuntimeCallable
    boolean cancelIdentified(Object peer) {
        if (((AtomicBoolean) peer).compareAndSet(false, true)) {
            removeMessages(MSG_RUN_TASK, peer);
            return true;
        }
        return false;
    }

    /**
     * Called from EventLoop::Shutdown on this handler's own thread. After
     * this no token can fire into the (about to be freed) native loop.
     */
    @RuntimeCallable
    void release() {
        released = true;
        removeCallbacksAndMessages(null); // safe: this handler is tokens-only
    }

    @Override
    public void handleMessage(Message msg) {
        if (released || msg.what != MSG_RUN_TASK) {
            return;
        }
        Object peer = msg.obj;
        if (peer != null && !((AtomicBoolean) peer).compareAndSet(false, true)) {
            // cancelIdentified won this token
            return;
        }
        long cellWord = (((long) msg.arg1) << 32) | (msg.arg2 & 0xffffffffL);
        if (cellWord != 0 && !nativeClaimToken(nativeRunnerPtr, cellWord)) {
            // the timer this token was posted for was cleared; the token dies
            // without touching the isolate
            return;
        }
        nativeRunTask(nativeRunnerPtr);
    }

    private static native void nativeRunTask(long nativeRunnerPtr);

    /**
     * Registered via RegisterNatives from EventLoop::BindToCurrentThread.
     * Runs without a JNIEnv or thread state transition - a single atomic CAS
     * against the loop's claim-cell table.
     */
    @CriticalNative
    private static native boolean nativeClaimToken(long nativeRunnerPtr, long cellWord);
}

package com.tns;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

/**
 * Dedicated per-runtime Handler for JS timers. One instance per isolate, bound
 * to the runtime thread's Looper. Timer messages are ordinary Java Messages,
 * so setTimeout(0) is strictly FIFO-ordered with any other Handler.post on the
 * same looper. Created and used exclusively from native code (Timers.cpp).
 */
final class TimerHandler extends Handler {
    private static final int MSG_FIRE_TIMER = 1;

    private final long nativeTimersPtr;
    private boolean released;

    // constructed from native code (Timers::Init)
    TimerHandler(long nativeTimersPtr) {
        super(Looper.myLooper());
        this.nativeTimersPtr = nativeTimersPtr;
    }

    /**
     * Enqueues an anonymous "timer due" token at an absolute uptimeMillis.
     * Native code keeps the exact (sub-millisecond) timer order and fires the
     * earliest due timer per token, so the token doesn't carry a timer id.
     */
    @RuntimeCallable
    void post(long uptimeMillis) {
        sendMessageAtTime(obtainMessage(MSG_FIRE_TIMER), uptimeMillis);
    }

    /**
     * Called from Timers::Destroy on this handler's own thread. After this no
     * timer message can fire into the (about to be freed) native Timers object.
     */
    @RuntimeCallable
    void release() {
        released = true;
        removeCallbacksAndMessages(null); // safe: this handler is timers-only
    }

    @Override
    public void handleMessage(Message msg) {
        if (released || msg.what != MSG_FIRE_TIMER) {
            return;
        }
        nativeFireTimer(nativeTimersPtr);
    }

    private static native void nativeFireTimer(long nativeTimersPtr);
}

package com.tns;

import android.view.Choreographer;

/**
 * Frame callback bridge for API levels without the NDK's AChoreographer (which
 * arrived in API 24). One instance per scheduled JS callback, bound to the
 * Choreographer of the thread that scheduled it -- the thread that owns the
 * isolate -- so doFrame arrives on that thread. Created and used exclusively
 * from native code (FrameCallbacks.cpp).
 */
final class FrameCallbacks implements Choreographer.FrameCallback {
    private final long nativeEntryPtr;
    private final Choreographer choreographer;
    private boolean released;

    // constructed from native code (FrameCallbacks::PostJavaCallback)
    FrameCallbacks(long nativeEntryPtr) {
        this.nativeEntryPtr = nativeEntryPtr;
        this.choreographer = Choreographer.getInstance();
    }

    @RuntimeCallable
    void post(long delayMillis) {
        if (delayMillis > 0) {
            choreographer.postFrameCallbackDelayed(this, delayMillis);
        } else {
            choreographer.postFrameCallback(this);
        }
    }

    /**
     * Called from native when the entry is dropped, on this callback's own
     * thread. A frame already queued for this instance still arrives, so the
     * flag -- not removeFrameCallback -- is what keeps it from reaching the
     * freed native entry.
     */
    @RuntimeCallable
    void release() {
        released = true;
        choreographer.removeFrameCallback(this);
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (released) {
            return;
        }
        nativeDoFrame(nativeEntryPtr, frameTimeNanos);
    }

    private static native void nativeDoFrame(long nativeEntryPtr, long frameTimeNanos);
}

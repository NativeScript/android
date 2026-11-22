package com.tns.tests;

// finalize() calls a method a JS subclass overrides. Java clears its weak references to an
// object before finalizing it, so the runtime no longer knows the object by then.
public class FinalizeCallsOverride {
    public static volatile int finalized;
    public static volatile int lastResult = -1;
    public static volatile Throwable failure;

    public int onFinalize() {
        return -2;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            lastResult = onFinalize();
        } catch (Throwable t) {
            failure = t;
        }
        finalized++;
        super.finalize();
    }
}

package com.tns.tests;

public class EscapeExceptionTest {
    public static void throwIOException() throws java.io.IOException {
        throw new java.io.IOException("original-io-exception");
    }

    /*
     * Invokes a callback implemented in JS and returns whatever Throwable
     * escapes it (or null). Catching Throwable (rather than a concrete type)
     * lets the tests assert exactly which exception class crossed the
     * JS->Java boundary.
     */
    public static Throwable invokeCatchingThrowable(Runnable runnable) {
        try {
            runnable.run();
            return null;
        } catch (Throwable t) {
            return t;
        }
    }
}

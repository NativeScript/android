package com.tns.tests;

public class UncaughtErrorPolicyTest {
    public static volatile int lastCompareResult = -999;

    /*
     * Invokes the comparator from a posted looper message - a native-initiated
     * entry into JS with no JS frames below it - so an uncaught throw in the
     * JS implementation is contained and compare() returns the int default.
     */
    public static void compareOnLooper(final java.util.Comparator<Object> comparator, final Runnable done) {
        new android.os.Handler(android.os.Looper.myLooper()).post(new Runnable() {
            @Override
            public void run() {
                lastCompareResult = comparator.compare("a", "b");
                done.run();
            }
        });
    }

    /*
     * Direct pass-through with no catch: lets tests verify that a JS->Java->JS
     * chain propagates the JS exception back to the outer JS catch.
     */
    public static void invoke(Runnable runnable) {
        runnable.run();
    }
}

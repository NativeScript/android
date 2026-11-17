package com.tns.tests.interfaces.runtimecalldefaults;

import com.tns.tests.DefaultMethodFixtures;

// Java callers for the runtime-call specs of testInterfaceDefaultMethodOverrides.js.
public class RuntimeCallCaller {
    public static String all(RuntimeCallShapes<String> shapes) {
        return shapes.wide(7, 2.5, 3) + "|" + shapes.half(5) + "|" + shapes.odd(3) + "|" + shapes.same("v");
    }

    public static String hidden(RuntimeCallVisible instance) {
        try {
            return instance.hidden();
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    public static String visible(RuntimeCallVisible instance) {
        try {
            return instance.visible();
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    /*
     * Calls shapes.wide() on a new thread while the calling (JS) thread waits here, so a call
     * that needed the JS thread could not complete before the timeout.
     */
    public static String wideOnBackgroundThread(final RuntimeCallShapes<?> shapes, long timeoutMillis) throws InterruptedException {
        final String[] result = new String[1];
        Thread caller = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    result[0] = Thread.currentThread().getName() + "|" + shapes.wide(Long.MAX_VALUE / 1000, -0.5, 1);
                } catch (Throwable t) {
                    result[0] = t.getClass().getName();
                }
            }
        }, DefaultMethodFixtures.CALLER_THREAD);
        caller.start();
        caller.join(timeoutMillis);
        return caller.isAlive() ? "timeout" : result[0];
    }

    public static boolean isDesugared() throws NoSuchMethodException {
        return !RuntimeCallShapes.class.getMethod("half", long.class).isDefault();
    }
}

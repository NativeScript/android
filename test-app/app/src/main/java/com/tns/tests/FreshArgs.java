package com.tns.tests;

import java.lang.ref.WeakReference;
import java.util.concurrent.CountDownLatch;

// Java->JS calls with objects JS has never seen, which Java registers before taking the
// isolate lock.
public class FreshArgs {
    public interface Target {
        void take(Object o);
    }

    public static WeakReference<Object> callWithNewObject(Target target) {
        Object o = new Object();
        WeakReference<Object> ref = new WeakReference<>(o);
        try {
            target.take(o);
        } catch (Throwable e) {
            // The call is expected to fail before JS sees the argument.
        }
        return ref;
    }

    private static final java.util.concurrent.atomic.AtomicInteger finishedCalls = new java.util.concurrent.atomic.AtomicInteger();

    public static int finishedCalls() {
        return finishedCalls.get();
    }

    // Returns at once: the JS thread must go idle so the workers can take the isolate lock.
    public static void startFromThreadsAtOnce(final Target target, int threads) {
        finishedCalls.set(0);
        final Object shared = new Object();
        final CountDownLatch go = new CountDownLatch(1);
        for (int t = 0; t < threads; t++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        go.await();
                        target.take(shared);
                    } catch (InterruptedException e) {
                        return;
                    } finally {
                        finishedCalls.incrementAndGet();
                    }
                }
            }).start();
        }
        go.countDown();
    }
}

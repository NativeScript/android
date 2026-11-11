package com.tns.tests;

import java.util.concurrent.CountDownLatch;

/**
 * Several Java threads calling one JS implementation at once, the multithreaded-JS
 * case. start() returns immediately so the JS thread can go idle and release the
 * isolate; the caller polls result() for the elapsed nanoseconds.
 */
public class ContentionBench {
    public interface Sink {
        void call0();

        void call3(Object a, Object b, Object c);
    }

    public static final int NO_ARGS = 0;
    public static final int EXISTING_OBJECTS = 1;
    public static final int NEW_OBJECTS = 2;
    public static final int NEW_OBJECTS_MANY_CLASSES = 3;

    private static volatile long resultNanos = -1;

    public static long result() {
        return resultNanos;
    }

    public static void start(final Sink sink, final int threads, final int callsPerThread, final int mode) {
        resultNanos = -1;
        final Object[] shared = {new Object(), new Object(), new Object()};
        new Thread(new Runnable() {
            @Override
            public void run() {
                final CountDownLatch ready = new CountDownLatch(threads);
                final CountDownLatch go = new CountDownLatch(1);
                Thread[] workers = new Thread[threads];
                for (int t = 0; t < threads; t++) {
                    workers[t] = new Thread(new Runnable() {
                        @Override
                        public void run() {
                            ready.countDown();
                            try {
                                go.await();
                            } catch (InterruptedException e) {
                                return;
                            }
                            for (int i = 0; i < callsPerThread; i++) {
                                if (mode == NO_ARGS) {
                                    sink.call0();
                                } else if (mode == EXISTING_OBJECTS) {
                                    sink.call3(shared[0], shared[1], shared[2]);
                                } else if (mode == NEW_OBJECTS) {
                                    sink.call3(new Object(), new Object(), new Object());
                                } else {
                                    sink.call3(JniBench.newOfClass(i), JniBench.newOfClass(i + 5), JniBench.newOfClass(i + 11));
                                }
                            }
                        }
                    });
                    workers[t].start();
                }
                try {
                    ready.await();
                    long start = System.nanoTime();
                    go.countDown();
                    for (Thread worker : workers) {
                        worker.join();
                    }
                    resultNanos = System.nanoTime() - start;
                } catch (InterruptedException e) {
                    resultNanos = 0;
                }
            }
        }).start();
    }
}

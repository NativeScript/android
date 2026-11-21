package com.tns.tests;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Hands JS-implemented objects from one runtime to another, and invokes them
 * from plain Java so a spec controls which thread makes the Java-&gt;JS call.
 */
public final class RuntimeBoundHolder {
    private static final ConcurrentHashMap<String, Object> objects = new ConcurrentHashMap<>();

    public static void put(String key, Object value) {
        objects.put(key, value);
    }

    public static Object get(String key) {
        return objects.get(key);
    }

    public static void clear() {
        objects.clear();
    }

    public static void run(String key) {
        ((Runnable) objects.get(key)).run();
    }

    public static void runOnNewThread(final String key) throws InterruptedException {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                RuntimeBoundHolder.run(key);
            }
        }, "RuntimeBoundHolder-caller");
        thread.start();
        thread.join();
    }

    @SuppressWarnings("unchecked")
    public static int compare(String key) {
        return ((Comparable<Object>) objects.get(key)).compareTo("");
    }

    // Results stopComparing reports for a comparison that threw, and for a comparer
    // thread still running at its deadline.
    public static final int THREW = Integer.MIN_VALUE;
    public static final int STUCK = Integer.MAX_VALUE;

    private static final List<Thread> comparers = new ArrayList<>();
    private static final Set<Integer> compared = ConcurrentHashMap.newKeySet();
    private static volatile boolean comparing;

    public static boolean hasCompared(int result) {
        return compared.contains(result);
    }

    /*
     * Keeps calling compareTo on objects[key] from `threads` new threads, each with a new
     * object as the argument (so the call registers it from that thread), until
     * stopComparing() or until the thread has had `zerosToStop` results of 0 in a row.
     */
    public static synchronized void startComparing(final String key, int threads, final int zerosToStop) {
        compared.clear();
        comparing = true;
        for (int i = 0; i < threads; i++) {
            Thread thread = new Thread(new Runnable() {
                @Override
                @SuppressWarnings("unchecked")
                public void run() {
                    Comparable<Object> comparable = (Comparable<Object>) objects.get(key);
                    int zeros = 0;
                    while (comparing && zeros < zerosToStop) {
                        int result;
                        try {
                            result = comparable.compareTo(new Object());
                        } catch (Throwable t) {
                            result = THREW;
                        }
                        compared.add(result);
                        zeros = result == 0 ? zeros + 1 : 0;
                    }
                }
            }, "RuntimeBoundHolder-comparer-" + i);
            comparers.add(thread);
            thread.start();
        }
    }

    public static synchronized boolean comparersFinished() {
        for (Thread thread : comparers) {
            if (thread.isAlive()) {
                return false;
            }
        }
        return true;
    }

    // Stops the comparing threads, waiting up to timeoutMillis for them; returns every
    // distinct result, with THREW for a throw and STUCK if a thread is still running.
    public static synchronized int[] stopComparing(long timeoutMillis) throws InterruptedException {
        comparing = false;
        long deadline = System.currentTimeMillis() + timeoutMillis;
        for (Thread thread : comparers) {
            thread.join(Math.max(1, deadline - System.currentTimeMillis()));
            if (thread.isAlive()) {
                compared.add(STUCK);
            }
        }
        comparers.clear();
        // A thread still running can add results while this copies them.
        Integer[] snapshot = compared.toArray(new Integer[0]);
        int[] results = new int[snapshot.length];
        for (int i = 0; i < snapshot.length; i++) {
            results[i] = snapshot[i];
        }
        return results;
    }
}

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

    private static final List<Thread> comparers = new ArrayList<>();
    private static final Set<Integer> compared = ConcurrentHashMap.newKeySet();
    private static volatile boolean comparing;

    // Keeps calling compare(key) from `threads` new threads until stopComparing().
    public static synchronized void startComparing(final String key, int threads) {
        compared.clear();
        comparing = true;
        for (int i = 0; i < threads; i++) {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    while (comparing) {
                        try {
                            compared.add(compare(key));
                        } catch (Throwable t) {
                            compared.add(Integer.MIN_VALUE);
                        }
                    }
                }
            }, "RuntimeBoundHolder-comparer-" + i);
            comparers.add(thread);
            thread.start();
        }
    }

    // Stops the comparing threads; returns every distinct result, MIN_VALUE for a throw.
    public static synchronized int[] stopComparing() throws InterruptedException {
        comparing = false;
        for (Thread thread : comparers) {
            thread.join();
        }
        comparers.clear();
        int[] results = new int[compared.size()];
        int i = 0;
        for (int result : compared) {
            results[i++] = result;
        }
        return results;
    }
}

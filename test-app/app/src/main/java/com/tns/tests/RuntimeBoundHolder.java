package com.tns.tests;

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
}

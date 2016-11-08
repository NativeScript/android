package com.tns;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Tracer {
    public static class Descriptor {
        public static final int CLASS = 0;
        public static final int EXCEPTION = 1;
        public static final int DIAGNOSTIC = 2;
        public static final int WORKERS = 3;
        public static final int APPLICATION = 4;
        public static final int DEBUGGER = 5;
        public static final int INFO = 6;
        public static final int REQUIRE = 7;
        public static final int REQUIRE_COMPILATION = 8;
        public static final int REQUIRE_EVAL = 9;
        public static final int BENCHMARK = 10;
    }

    private static Map<Integer, TraceListener> listeners = new ConcurrentHashMap<>();
    private static boolean enabled = false;
    private static Map<Integer, String> presetDescriptors = new HashMap<>();
    static {
        presetDescriptors.put(Descriptor.CLASS, "CLASS");
        presetDescriptors.put(Descriptor.EXCEPTION, "EXCEPTION");
        presetDescriptors.put(Descriptor.DIAGNOSTIC, "DIAGNOSTIC");
        presetDescriptors.put(Descriptor.WORKERS, "WORKERS");
        presetDescriptors.put(Descriptor.APPLICATION, "APPLICATION");
        presetDescriptors.put(Descriptor.DEBUGGER, "DEBUGGER");
        presetDescriptors.put(Descriptor.INFO, "INFO");
        presetDescriptors.put(Descriptor.REQUIRE, "REQUIRE");
        presetDescriptors.put(Descriptor.REQUIRE_COMPILATION, "REQUIRE_COMPILATION");
        presetDescriptors.put(Descriptor.REQUIRE_EVAL, "REQUIRE_EVAL");
        presetDescriptors.put(Descriptor.BENCHMARK, "BENCHMARK");
    }

    public static void setEnabled(boolean enable) {
        enabled = enable;

        Runtime.enableTracer(enable);
    }

    public static void trace(int descriptor, String message) {
        if (!enabled) {
            return;
        }

        TraceListener listener = getListener(descriptor);

        if (listener == null) {
            return;
        }

        TraceMessage traceMessage = buildMessage(message);
        listener.trace(traceMessage);
    }

    public static void traceIf(int descriptor, String message, boolean evaluatedCondition) {
        if (evaluatedCondition) {
            trace(descriptor, message);
        }
    }

    @RuntimeCallable
    public static void dumpToFile(int descriptor) {
        TraceListener listener = getListener(descriptor);

        if (listener == null) {
            return;
        }

        listener.dumpToFile();
    }

    public static void addListener(int descriptor, TraceListener listener) {
        listeners.put(descriptor, listener);
    }

    public static void removeListener(int descriptor) {
        listeners.remove(descriptor);
    }

    public static Map<Integer, String> getDescriptors() {
        return presetDescriptors;
    }

    private static TraceMessage buildMessage(String messageStr) {
        long tid = android.os.Process.myTid();
        String tName = Thread.currentThread().getName();

        TraceMessage message = new TraceMessage(messageStr, tName, tid);

        return message;
    }

    private static TraceListener getListener(int descriptor) {
        return listeners.get(descriptor);
    }
}

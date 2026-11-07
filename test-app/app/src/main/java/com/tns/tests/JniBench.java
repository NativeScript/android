package com.tns.tests;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;

/**
 * Targets for the JNI micro-benchmarks in assets/app/tests/benchmarks/jniBenchmarks.js.
 * Every member is deliberately trivial so a measurement is dominated by the bridge, not the work.
 */
public class JniBench {
    public interface Callback {
        void noArgs();

        void twoInts(int a, int b);

        void oneString(String s);

        int intReturn(int a);

        void oneObject(Object o);
    }

    public int intField = 7;
    public Object objField = new Object();
    public String strField = "hello";

    private final Object other = new Object();

    public static boolean isEnabled() {
        try {
            Process p = new ProcessBuilder("getprop", "debug.ns.jnibench").start();
            try (BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()))) {
                String line = r.readLine();
                return line != null && line.trim().equals("1");
            }
        } catch (Exception e) {
            return false;
        }
    }

    public static void noop() {
    }

    public static int add(int a, int b) {
        return a + b;
    }

    public static long addLong(long a, long b) {
        return a + b;
    }

    public static double addDouble(double a, double b) {
        return a + b;
    }

    public int getInt() {
        return intField;
    }

    public boolean getBool() {
        return true;
    }

    public char getChar() {
        return 'x';
    }

    public String echo(String s) {
        return s;
    }

    public String getString() {
        return strField;
    }

    public Object self() {
        return this;
    }

    public Object getOther() {
        return other;
    }

    public void takeObject(Object o) {
    }

    public int takeObjects(Object a, Object b, Object c) {
        return 3;
    }

    public static int bufferCapacity(ByteBuffer b) {
        return b.capacity();
    }

    public static int sumInts(int[] values) {
        int sum = 0;
        for (int v : values) {
            sum += v;
        }
        return sum;
    }

    public static Object newObjectInJava() {
        return new Object();
    }

    /** Returns total elapsed nanoseconds for {@code n} Java -> JS calls of the given kind. */
    public static long runCallbacks(Callback cb, int kind, int n) {
        Object arg = new Object();
        long start = System.nanoTime();
        switch (kind) {
            case 0:
                for (int i = 0; i < n; i++) cb.noArgs();
                break;
            case 1:
                for (int i = 0; i < n; i++) cb.twoInts(i, 2);
                break;
            case 2:
                for (int i = 0; i < n; i++) cb.oneString("hello world");
                break;
            case 3:
                for (int i = 0; i < n; i++) cb.intReturn(i);
                break;
            case 4:
                for (int i = 0; i < n; i++) cb.oneObject(arg);
                break;
            default:
                throw new IllegalArgumentException("kind " + kind);
        }
        return System.nanoTime() - start;
    }
}

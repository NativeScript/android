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
        return getProp("debug.ns.jnibench").equals("1");
    }

    /** Substring a benchmark name must contain to run; empty runs all. */
    public static String getFilter() {
        return getProp("debug.ns.jnibench.filter");
    }

    /** Iteration multiplier, e.g. to keep one benchmark running long enough to profile. */
    public static int getScale() {
        try {
            return Math.max(1, Integer.parseInt(getProp("debug.ns.jnibench.scale")));
        } catch (NumberFormatException e) {
            return 1;
        }
    }

    static String getProp(String name) {
        try {
            Process p = new ProcessBuilder("getprop", name).start();
            try (BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()))) {
                String line = r.readLine();
                return line == null ? "" : line.trim();
            }
        } catch (Exception e) {
            return "";
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

    public void instanceNoop() {
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

    // Sixteen distinct classes, for call sites that see many runtime classes, as the
    // generic ones in an app do (View, Object, Drawable...).
    public static final class K0 {
    }

    public static final class K1 {
    }

    public static final class K2 {
    }

    public static final class K3 {
    }

    public static final class K4 {
    }

    public static final class K5 {
    }

    public static final class K6 {
    }

    public static final class K7 {
    }

    public static final class K8 {
    }

    public static final class K9 {
    }

    public static final class K10 {
    }

    public static final class K11 {
    }

    public static final class K12 {
    }

    public static final class K13 {
    }

    public static final class K14 {
    }

    public static final class K15 {
    }

    public static Object newOfClass(int i) {
        switch (i & 15) {
            case 0:
                return new K0();
            case 1:
                return new K1();
            case 2:
                return new K2();
            case 3:
                return new K3();
            case 4:
                return new K4();
            case 5:
                return new K5();
            case 6:
                return new K6();
            case 7:
                return new K7();
            case 8:
                return new K8();
            case 9:
                return new K9();
            case 10:
                return new K10();
            case 11:
                return new K11();
            case 12:
                return new K12();
            case 13:
                return new K13();
            case 14:
                return new K14();
            case 15:
                return new K15();
            default:
                throw new AssertionError();
        }
    }

    private static int nextClass;

    public static Object newObjectOfManyClasses() {
        return newOfClass(nextClass++);
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
            case 5:
                for (int i = 0; i < n; i++) cb.oneObject(new Object());
                break;
            case 6:
                for (int i = 0; i < n; i++) cb.oneObject(newOfClass(i));
                break;
            case 7:
                // The callback is itself a binding: the argument carries its object id.
                for (int i = 0; i < n; i++) cb.oneObject(cb);
                break;
            default:
                throw new IllegalArgumentException("kind " + kind);
        }
        return System.nanoTime() - start;
    }
}

package com.tns.tests;

import java.nio.Buffer;
import java.util.Arrays;

// Each method has a single overload of its arity, so calls skip overload resolution and
// convert straight to the declared parameter type.
public class TypedArrayArgs {
    public static byte[] echoBytes(byte[] a) {
        return a;
    }

    public static short[] echoShorts(short[] a) {
        return a;
    }

    public static char[] echoChars(char[] a) {
        return a;
    }

    public static int[] echoInts(int[] a) {
        return a;
    }

    public static long[] echoLongs(long[] a) {
        return a;
    }

    public static float[] echoFloats(float[] a) {
        return a;
    }

    public static double[] echoDoubles(double[] a) {
        return a;
    }

    public static boolean[] echoBooleans(boolean[] a) {
        return a;
    }

    public static long sumInts(int[] a) {
        long sum = 0;
        for (int v : a) {
            sum += v;
        }
        return sum;
    }

    public static String describeObjects(Object[] items) {
        return Arrays.toString(items);
    }

    public static int countRows(int[][] rows) {
        return rows.length;
    }

    public static int capacityOf(Object o) {
        return o instanceof Buffer ? ((Buffer) o).capacity() : -1;
    }

    // Elements of an Object[] go through the single-value converter.
    public static long totalCapacity(Object[] items) {
        long total = 0;
        for (Object o : items) {
            total += capacityOf(o);
        }
        return total;
    }
}

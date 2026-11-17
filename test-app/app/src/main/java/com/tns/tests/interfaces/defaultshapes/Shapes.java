package com.tns.tests.interfaces.defaultshapes;

// Default methods of every argument and return kind, for testInterfaceDefaultMethodOverrides.js.
public interface Shapes<T> {
    String name();

    default long wide(long a, double b, int c) {
        return a * 1000 + (long) (b * 10) + c;
    }

    default double half(long value) {
        return value / 2.0;
    }

    default float third(float value) {
        return value / 3f;
    }

    default char upper(char value) {
        return Character.toUpperCase(value);
    }

    default boolean odd(int value) {
        return (value & 1) != 0;
    }

    default byte negated(byte value) {
        return (byte) -value;
    }

    default short doubled(short value) {
        return (short) (value * 2);
    }

    default void record(StringBuilder out, long value, double fraction) {
        out.append(name()).append(':').append(value).append(':').append(fraction);
    }

    default T same(T value) {
        return value;
    }

    default String fail(String message) throws ShapesException {
        throw new ShapesException(message);
    }
}

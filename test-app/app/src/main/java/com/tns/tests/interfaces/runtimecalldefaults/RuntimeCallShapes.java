package com.tns.tests.interfaces.runtimecalldefaults;

// For the runtime-call stubs only: no spec lets this interface get the invoke-super class.
public interface RuntimeCallShapes<T> {
    String name();

    default long wide(long a, double b, int c) {
        return a * 1000 + (long) (b * 10) + c;
    }

    default double half(long value) {
        return value / 2.0;
    }

    default boolean odd(int value) {
        return (value & 1) != 0;
    }

    default T same(T value) {
        return value;
    }
}

package com.tns.tests;

// Field reads and writes reach Java without a method call, which the finalizer batch tests
// rely on.
public class WrapperHolder {
    public static Object held;
}

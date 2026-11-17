package com.tns.tests.interfaces.runtimecalldefaults;

// Narrows the generic default; javac adds the bridge same(Object) that forwards to it.
public interface RuntimeCallStringShapes extends RuntimeCallShapes<String> {
    @Override
    default String same(String value) {
        return "string " + value;
    }
}

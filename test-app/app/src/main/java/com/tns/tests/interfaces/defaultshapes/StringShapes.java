package com.tns.tests.interfaces.defaultshapes;

// Narrows the generic default; javac adds the bridge same(Object) that forwards to it.
public interface StringShapes extends Shapes<String> {
    @Override
    default String same(String value) {
        return "string " + value;
    }
}

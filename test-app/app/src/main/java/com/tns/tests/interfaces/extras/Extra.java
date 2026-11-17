package com.tns.tests.interfaces.extras;

public interface Extra {
    String extra();

    default String extraDefault() {
        return "java";
    }
}

package com.tns.tests.interfaces.extras;

public interface DefaultsBase {
    String base();

    default String baseDefault() {
        return "java";
    }
}

package com.tns.tests.interfaces.runtimecalldefaults;

public interface RuntimeCallVisible extends RuntimeCallHidden {
    String name();

    default String visible() {
        return "visible java";
    }
}

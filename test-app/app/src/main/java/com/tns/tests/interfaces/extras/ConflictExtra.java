package com.tns.tests.interfaces.extras;

public interface ConflictExtra {
    default String shared() {
        return "extra";
    }
}

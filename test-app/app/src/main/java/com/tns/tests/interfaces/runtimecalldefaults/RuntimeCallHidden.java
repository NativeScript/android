package com.tns.tests.interfaces.runtimecalldefaults;

// Package-private: a class generated in another package cannot name it.
interface RuntimeCallHidden {
    default String hidden() {
        return "hidden java";
    }
}

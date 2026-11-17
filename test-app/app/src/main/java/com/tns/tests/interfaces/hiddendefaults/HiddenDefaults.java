package com.tns.tests.interfaces.hiddendefaults;

// Package-private: a class generated in another package cannot name it.
interface HiddenDefaults {
    default String hidden() {
        return "hidden java";
    }

    default Object narrowed() {
        return "hidden narrowed";
    }
}

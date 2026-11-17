package com.tns.tests.interfaces.extras;

// shared() conflicts with ConflictExtra's: a class implementing both has to override it.
public interface ConflictBase {
    String base();

    default String shared() {
        return "base";
    }
}

package com.tns;

/**
 * Holds, during ns:runtime's findLeakCycles() heap dump only, the Java objects the analysis
 * maps to JS wrappers and states. The analysis finds this array through this class's static
 * field by name and does not count it as a reference.
 */
final class LeakProbe {
    static Object[] pinned;

    private LeakProbe() {
    }
}

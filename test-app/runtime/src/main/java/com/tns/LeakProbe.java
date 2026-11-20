package com.tns;

import java.io.File;

/**
 * Holds, during ns:runtime's findLeakCycles() heap dump only, the Java objects the analysis
 * maps to JS wrappers and states. The analysis finds this array through this class's static
 * field by name and does not count it as a reference.
 */
final class LeakProbe {
    static final String DUMP_PREFIX = "ns-leak-probe";

    static Object[] pinned;

    private LeakProbe() {
    }

    // Dumps a killed process left behind; the native side deletes its own once parsed.
    static void deleteStaleDumps() {
        String tmp = System.getProperty("java.io.tmpdir");
        File[] files = tmp != null ? new File(tmp).listFiles() : null;
        if (files == null) {
            return;
        }
        for (File file : files) {
            if (file.getName().startsWith(DUMP_PREFIX) && file.getName().endsWith(".hprof")) {
                file.delete();
            }
        }
    }
}

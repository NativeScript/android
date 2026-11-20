package com.tns;

import java.io.File;

/**
 * ns:runtime findLeakCycles()'s Java side, in debug runtimes only. During the heap dump,
 * {@link #pinned} holds the Java objects the analysis maps to JS wrappers and states; the
 * analysis finds that array through this static field by name and does not count it as a
 * reference.
 */
final class LeakProbe {
    static final String DUMP_PREFIX = "ns-leak-probe";

    // Older than any dump a probe of this process or another one may still be parsing.
    private static final long STALE_DUMP_AGE_MS = 10 * 60 * 1000;

    static Object[] pinned;

    private LeakProbe() {
    }

    // Before the probe takes strong references: objects only weak entries hold would
    // otherwise read as leaked until Java collects them.
    static void collectGarbage() {
        for (int i = 0; i < 2; i++) {
            java.lang.Runtime.getRuntime().gc();
        }
    }

    // Writes an HPROF dump with objects pinned in order; returns its path, or null if it
    // could not be written. The caller deletes the dump once parsed.
    static String dump(Object[] objects) {
        // pinned is shared by every runtime of the process.
        synchronized (LeakProbe.class) {
            deleteStaleDumps();
            File file = null;
            try {
                file = File.createTempFile(DUMP_PREFIX, ".hprof");
                pinned = objects;
                android.os.Debug.dumpHprofData(file.getAbsolutePath());
                return file.getAbsolutePath();
            } catch (Throwable t) {
                android.util.Log.w("JS", "findLeakCycles could not dump the Java heap", t);
                if (file != null) {
                    file.delete();
                }
                return null;
            } finally {
                pinned = null;
            }
        }
    }

    // Dumps a killed process left behind.
    private static void deleteStaleDumps() {
        String tmp = System.getProperty("java.io.tmpdir");
        File[] files = tmp != null ? new File(tmp).listFiles() : null;
        if (files == null) {
            return;
        }
        long staleBefore = System.currentTimeMillis() - STALE_DUMP_AGE_MS;
        for (File file : files) {
            String name = file.getName();
            if (name.startsWith(DUMP_PREFIX) && name.endsWith(".hprof")
                    && file.lastModified() < staleBefore) {
                file.delete();
            }
        }
    }
}

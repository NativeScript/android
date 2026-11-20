package com.tns;

import android.util.Log;

import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.util.Arrays;

/**
 * Reports the collection of Java objects whose JS state the canary wrapper lifetime mode
 * keeps (ObjectTable.WatchedRef) to the runtime that owns the state. One daemon thread for
 * the process, started with the first watched object; it only removes the cleared entries
 * and passes their ids on, never touching a runtime's V8 state.
 */
final class WrapperLifetimeWatcher {
    private static ReferenceQueue<Object> queue;

    private WrapperLifetimeWatcher() {
    }

    static synchronized ReferenceQueue<Object> queue() {
        if (queue == null) {
            queue = new ReferenceQueue<>();
            Thread thread = new Thread(WrapperLifetimeWatcher::run, "NativeScript lifetime watcher");
            thread.setDaemon(true);
            thread.start();
        }
        return queue;
    }

    private static void run() {
        ReferenceQueue<Object> watched = queue();
        int[] ids = new int[64];
        while (true) {
            Reference<?> next;
            try {
                next = watched.remove();
            } catch (InterruptedException e) {
                return;
            }
            try {
                ids = drain(watched, next, ids);
            } catch (Throwable t) {
                // The only watcher: losing it would leak every later collected object's state.
                Log.w("JS", "Wrapper lifetime watcher failed to report collected objects", t);
            }
        }
    }

    // Reports next and every reference already queued behind it; returns the id buffer to reuse.
    private static int[] drain(ReferenceQueue<Object> watched, Reference<?> next, int[] ids) {
        // Every reference collected so far, grouped by consecutive runtime.
        int runtimeId = -1;
        int count = 0;
        for (; next != null; next = watched.poll()) {
            if (!(next instanceof ObjectTable.WatchedRef)) {
                continue;
            }
            ObjectTable.WatchedRef ref = (ObjectTable.WatchedRef) next;
            if (ref.runtimeId != runtimeId) {
                notify(runtimeId, ids, count);
                runtimeId = ref.runtimeId;
                count = 0;
            }
            Runtime runtime = Runtime.runtimeForId(ref.runtimeId);
            if (runtime == null || !runtime.removeWatched(ref)) {
                continue;
            }
            if (count == ids.length) {
                ids = Arrays.copyOf(ids, count * 2);
            }
            ids[count++] = ref.id;
        }
        notify(runtimeId, ids, count);
        return ids;
    }

    private static void notify(int runtimeId, int[] ids, int count) {
        if (count == 0) {
            return;
        }
        try {
            Runtime.notifyCanaryObjectsCollected(runtimeId, ids, count);
        } catch (Throwable t) {
            Log.w("JS", "Failed to report collected objects to runtime " + runtimeId, t);
        }
    }
}

package com.tns;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;

/**
 * The ordered app file layers: the first layer that has a path serves it.
 */
public final class AppFileLayers {
    /** The files extracted to (or written by LiveSync into) &lt;files&gt;/app. */
    public static final String EXTRACTED = "extracted";
    /** Code served straight from the APK; present only with android.loadCodeFromApk. */
    public static final String APK = "apk";

    static final String CUSTOMIZER_META_DATA_VALUE = "org.nativescript.AppFileLayersCustomizer";

    private static final String LOG_TAG = "JS";

    private AppFileLayers() {
    }

    public static final class Builder {
        private final Context context;
        private final boolean debuggable;
        private final boolean appUpdated;
        private final boolean loadsCodeFromApk;
        // Top first; null marks a built-in layer.
        private final List<String> ids = new ArrayList<>();
        private final List<AppFileLayer> layers = new ArrayList<>();

        Builder(Context context, boolean debuggable, boolean appUpdated, boolean loadsCodeFromApk) {
            this.context = context;
            this.debuggable = debuggable;
            this.appUpdated = appUpdated;
            this.loadsCodeFromApk = loadsCodeFromApk;
            add(ids.size(), EXTRACTED, null);
            if (loadsCodeFromApk) {
                add(ids.size(), APK, null);
            }
        }

        public Context context() {
            return context;
        }

        public boolean isDebuggable() {
            return debuggable;
        }

        /** True on the first launch after an install or update of the app. */
        public boolean isAppUpdated() {
            return appUpdated;
        }

        public boolean loadsCodeFromApk() {
            return loadsCodeFromApk;
        }

        /** The layer ids, top first. */
        public List<String> ids() {
            return Collections.unmodifiableList(new ArrayList<>(ids));
        }

        public Builder insertAbove(String id, AppFileLayer layer) {
            add(indexOf(id), layer.id(), layer);
            return this;
        }

        public Builder insertBelow(String id, AppFileLayer layer) {
            add(indexOf(id) + 1, layer.id(), layer);
            return this;
        }

        public Builder remove(String id) {
            int index = indexOf(id);
            ids.remove(index);
            layers.remove(index);
            return this;
        }

        private int indexOf(String id) {
            int index = ids.indexOf(id);
            if (index < 0) {
                throw new IllegalArgumentException("No app file layer \"" + id + "\"; layers are " + ids);
            }
            return index;
        }

        private void add(int index, String id, AppFileLayer layer) {
            if (ids.contains(id)) {
                throw new IllegalArgumentException("Duplicate app file layer \"" + id + "\"");
            }
            ids.add(index, id);
            layers.add(index, layer);
        }

        /*
         * LiveSync writes into <files>/app, and what it writes must win over
         * every other layer, whatever the customizers did. Release builds have
         * no LiveSync, so there a customizer may deliberately put a layer
         * above the extracted files.
         */
        private void pinExtractedOnTop() {
            int index = ids.indexOf(EXTRACTED);
            if (index >= 0) {
                ids.remove(index);
                layers.remove(index);
            }
            ids.add(0, EXTRACTED);
            layers.add(0, null);
        }

        Snapshot snapshot() {
            Snapshot snapshot = new Snapshot(ids.size());
            List<String> mapKeys = new ArrayList<>();
            List<Integer> mapKinds = new ArrayList<>();
            List<String> mapTargets = new ArrayList<>();
            for (int i = 0; i < ids.size(); i++) {
                AppFileLayer layer = layers.get(i);
                snapshot.mapStart[i] = mapKeys.size();
                if (layer == null) {
                    snapshot.kinds[i] = EXTRACTED.equals(ids.get(i)) ? Snapshot.EXTRACTED : Snapshot.APK;
                    continue;
                }
                snapshot.roots[i] = layer.root;
                switch (layer.kind) {
                    case DIRECTORY:
                        snapshot.kinds[i] = Snapshot.DIRECTORY;
                        break;
                    case APK_ASSETS:
                        snapshot.kinds[i] = Snapshot.APK_ASSETS;
                        break;
                    case MAP:
                        snapshot.kinds[i] = Snapshot.MAP;
                        for (Map.Entry<String, AppFileLayer.Entry> entry : layer.entries.entrySet()) {
                            mapKeys.add(entry.getKey());
                            mapKinds.add(entry.getValue().kind.ordinal());
                            mapTargets.add(entry.getValue().target);
                        }
                        break;
                }
            }
            snapshot.mapStart[ids.size()] = mapKeys.size();
            snapshot.mapKeys = mapKeys.toArray(new String[0]);
            snapshot.mapKinds = new int[mapKinds.size()];
            for (int i = 0; i < mapKinds.size(); i++) {
                snapshot.mapKinds[i] = mapKinds.get(i);
            }
            snapshot.mapTargets = mapTargets.toArray(new String[0]);
            return snapshot;
        }
    }

    /*
     * The flat form native reads. Kind values and the map entry kinds
     * (AppFileLayer.Entry.Kind ordinals) must match AppFiles.cpp.
     */
    static final class Snapshot {
        static final int EXTRACTED = 0;
        static final int APK = 1;
        static final int DIRECTORY = 2;
        static final int APK_ASSETS = 3;
        static final int MAP = 4;

        final int[] kinds;
        final String[] roots;
        // Layer i's map entries are [mapStart[i], mapStart[i + 1]).
        final int[] mapStart;
        String[] mapKeys;
        int[] mapKinds;
        String[] mapTargets;

        Snapshot(int count) {
            kinds = new int[count];
            roots = new String[count];
            mapStart = new int[count + 1];
        }
    }

    static Snapshot build(Context context, boolean appUpdated, boolean loadsCodeFromApk) {
        boolean debuggable = (context.getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0;
        Builder builder = new Builder(context, debuggable, appUpdated, loadsCodeFromApk);
        for (AppFileLayersCustomizer customizer : discoverCustomizers(context)) {
            try {
                customizer.customize(builder);
            } catch (RuntimeException | LinkageError e) {
                Log.e(LOG_TAG, "App file layers customizer " + customizer.getClass().getName() + " failed", e);
            }
        }
        if (debuggable) {
            builder.pinExtractedOnTop();
        }
        return builder.snapshot();
    }

    // Bundle.get is the only accessor that reads a value of unknown type
    // without logging a ClassCastException for every non-string meta-data.
    @SuppressWarnings("deprecation")
    private static List<AppFileLayersCustomizer> discoverCustomizers(Context context) {
        List<AppFileLayersCustomizer> customizers = new ArrayList<>();
        Bundle metaData;
        try {
            metaData = context.getPackageManager()
                    .getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA).metaData;
        } catch (PackageManager.NameNotFoundException e) {
            return customizers;
        }
        if (metaData == null) {
            return customizers;
        }
        for (String key : metaData.keySet()) {
            if (!CUSTOMIZER_META_DATA_VALUE.equals(metaData.get(key))) {
                continue;
            }
            try {
                Class<?> type = Class.forName(key, true, context.getClassLoader());
                customizers.add((AppFileLayersCustomizer) type.getDeclaredConstructor().newInstance());
            } catch (Exception | LinkageError e) {
                Log.e(LOG_TAG, "Cannot create app file layers customizer " + key, e);
            }
        }
        Collections.sort(customizers, new Comparator<AppFileLayersCustomizer>() {
            @Override
            public int compare(AppFileLayersCustomizer a, AppFileLayersCustomizer b) {
                int byPriority = Integer.compare(a.priority(), b.priority());
                return byPriority != 0 ? byPriority : a.getClass().getName().compareTo(b.getClass().getName());
            }
        });
        return customizers;
    }
}

package com.tns;

import java.io.File;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * An immutable description of one place app files can come from. Paths are
 * relative to the app root ("main.js" is &lt;files&gt;/app/main.js); layers never
 * see the runtime's internal or metadata files.
 */
public final class AppFileLayer {
    enum Kind {
        DIRECTORY,
        APK_ASSETS,
        MAP,
    }

    /** What a {@link #map} layer answers for one path. */
    public static final class Entry {
        enum Kind {
            FILE,
            ASSET,
            WHITEOUT,
        }

        final Kind kind;
        final String target;

        private Entry(Kind kind, String target) {
            this.kind = kind;
            this.target = target;
        }

        /** Served from a file anywhere on disk. */
        public static Entry file(File file) {
            return new Entry(Kind.FILE, file.getAbsolutePath());
        }

        /** Served from an APK asset, e.g. "ota/app/main.js". */
        public static Entry asset(String assetPath) {
            return new Entry(Kind.ASSET, checkRelative(assetPath, "asset path"));
        }

        /** Not found, whatever the layers below hold. */
        public static Entry whiteout() {
            return new Entry(Kind.WHITEOUT, null);
        }
    }

    private final String id;
    final Kind kind;
    final String root;
    final Map<String, Entry> entries;

    private AppFileLayer(String id, Kind kind, String root, Map<String, Entry> entries) {
        if (id == null || id.isEmpty()) {
            throw new IllegalArgumentException("An app file layer needs an id");
        }
        this.id = id;
        this.kind = kind;
        this.root = root;
        this.entries = entries;
    }

    /** Files under {@code dir}, laid out like the app root. */
    public static AppFileLayer directory(String id, File dir) {
        return new AppFileLayer(id, Kind.DIRECTORY, dir.getAbsolutePath(), null);
    }

    /** APK assets under {@code assetBase} (e.g. "ota/app"), laid out like the app root. */
    public static AppFileLayer apkAssets(String id, String assetBase) {
        return new AppFileLayer(id, Kind.APK_ASSETS, checkRelative(assetBase, "asset base"), null);
    }

    /**
     * Exactly the listed paths; every other path falls through to the layers
     * below. So does a {@link Entry#file} or {@link Entry#asset} entry whose
     * target does not exist; only {@link Entry#whiteout} hides lower layers.
     */
    public static AppFileLayer map(String id, Map<String, Entry> entries) {
        Map<String, Entry> copy = new LinkedHashMap<>();
        for (Map.Entry<String, Entry> entry : entries.entrySet()) {
            if (entry.getValue() == null) {
                throw new IllegalArgumentException("No entry for " + entry.getKey());
            }
            copy.put(checkRelative(entry.getKey(), "path"), entry.getValue());
        }
        return new AppFileLayer(id, Kind.MAP, null, Collections.unmodifiableMap(copy));
    }

    public String id() {
        return id;
    }

    private static String checkRelative(String path, String what) {
        String trimmed = path;
        while (trimmed.startsWith("./")) {
            trimmed = trimmed.substring(2);
        }
        while (trimmed.endsWith("/")) {
            trimmed = trimmed.substring(0, trimmed.length() - 1);
        }
        String segments = "/" + trimmed + "/";
        if (trimmed.isEmpty() || trimmed.startsWith("/") || segments.contains("/../")
                || segments.contains("/./") || trimmed.contains("//")) {
            throw new IllegalArgumentException("Not a relative " + what + ": \"" + path + "\"");
        }
        return trimmed;
    }
}

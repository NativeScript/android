package com.tns;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

/**
 * The runtime's view of the app's files: the same sources and path rules the
 * native module loader uses. Usable once libNativeScript is loaded; it needs no
 * initialized Runtime.
 */
public final class AppFiles {
    private AppFiles() {
    }

    /**
     * Sets the files directory. Only the first call takes effect, so every
     * caller that may run first can make it.
     */
    public static void init(File filesDir) {
        String absolute = filesDir.getAbsolutePath();
        String canonical;
        try {
            canonical = filesDir.getCanonicalPath();
        } catch (IOException e) {
            canonical = absolute;
        }
        nativeInit(canonical, absolute);
    }

    /** UTF-8 contents, or null when the file cannot be read. */
    public static String readText(String path) {
        byte[] bytes = readBytes(path);
        return bytes != null ? new String(bytes, StandardCharsets.UTF_8) : null;
    }

    /** The canonical files directory, which every logical path starts with. */
    public static native String filesRoot();

    /** "&lt;files&gt;/app", no trailing slash. */
    public static native String appRoot();

    /**
     * The logical spelling of a path inside the files directory ("//", "." and
     * ".." collapsed, aliases of the directory mapped onto {@link #filesRoot()}),
     * or null when the path is outside it.
     */
    public static native String logicalPath(String path);

    public static native boolean isFile(String path);

    private static native byte[] readBytes(String path);

    private static native void nativeInit(String filesRoot, String alias);
}

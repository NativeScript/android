package com.tns;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

/**
 * The runtime's view of the app's files: the same sources and path rules the
 * native module loader uses. Usable once libNativeScript is loaded; it needs no
 * initialized Runtime.
 */
public final class AppFiles {
    private static final String LOG_TAG = "JS";

    private AppFiles() {
    }

    /**
     * Configures app file access from the APK's own app/package.json
     * ({@code android.loadCodeFromApk}, {@code android.alwaysExtract}). Must run
     * before assets are extracted, since the extractor skips what is served
     * from the APK.
     */
    public static void init(Context context) {
        AssetManager assets = context.getAssets();
        ApkFlags flags = ApkFlags.read(assets);
        init(context.getFilesDir(), context.getCodeCacheDir(), flags.loadCodeFromApk ? assets : null,
                flags.loadCodeFromApk, flags.alwaysExtract);
    }

    /**
     * Every app file read from the files directory. Only the first init takes
     * effect, so every caller that may run first can make it.
     */
    public static void init(File filesDir, File codeCacheDir) {
        init(filesDir, codeCacheDir, null, false, new String[0]);
    }

    private static void init(File filesDir, File codeCacheDir, AssetManager assets,
                             boolean loadCodeFromApk, String[] alwaysExtract) {
        String absolute = filesDir.getAbsolutePath();
        String canonical;
        try {
            canonical = filesDir.getCanonicalPath();
        } catch (IOException e) {
            canonical = absolute;
        }
        nativeInit(canonical, absolute, codeCacheDir.getAbsolutePath(), assets, loadCodeFromApk,
                alwaysExtract);
    }

    /** UTF-8 contents, or null when the file cannot be read. */
    public static String readText(String path) {
        byte[] bytes = readBytes(path);
        return bytes != null ? new String(bytes, StandardCharsets.UTF_8) : null;
    }

    /** The canonical files directory, which every logical path starts with. */
    public static native String filesRoot();

    /** Whether code is served from the APK instead of being extracted. */
    public static native boolean loadsCodeFromApk();

    /**
     * The logical spelling of a path inside the files directory ("//", "." and
     * ".." collapsed, aliases of the directory mapped onto {@link #filesRoot()}),
     * or null when the path is outside it.
     */
    public static native String logicalPath(String path);

    public static native boolean isFile(String path);

    private static native byte[] readBytes(String path);

    private static native void nativeInit(String filesRoot, String alias, String codeCacheDir,
                                          AssetManager assets, boolean loadCodeFromApk,
                                          String[] alwaysExtract);

    /*
     * Read from the APK's copy of app/package.json, never through the app file
     * layers: a package.json pushed by LiveSync or a plugin must not be able to
     * change where code comes from.
     */
    private static final class ApkFlags {
        boolean loadCodeFromApk;
        String[] alwaysExtract = new String[0];

        static ApkFlags read(AssetManager assets) {
            ApkFlags flags = new ApkFlags();
            try (InputStream in = assets.open("app/package.json")) {
                ByteArrayOutputStream bytes = new ByteArrayOutputStream();
                byte[] buffer = new byte[8192];
                int n;
                while ((n = in.read(buffer)) > 0) {
                    bytes.write(buffer, 0, n);
                }
                JSONObject android = new JSONObject(new String(bytes.toByteArray(), StandardCharsets.UTF_8))
                        .optJSONObject("android");
                if (android == null) {
                    return flags;
                }
                flags.loadCodeFromApk = android.optBoolean("loadCodeFromApk", false);
                JSONArray globs = android.optJSONArray("alwaysExtract");
                if (globs != null) {
                    flags.alwaysExtract = new String[globs.length()];
                    for (int i = 0; i < globs.length(); i++) {
                        flags.alwaysExtract[i] = globs.optString(i, "");
                    }
                }
            } catch (java.io.FileNotFoundException e) {
                flags = new ApkFlags();
            } catch (Exception e) {
                Log.w(LOG_TAG, "Could not read android.loadCodeFromApk from the APK's app/package.json; code will be extracted. " + e);
                flags = new ApkFlags();
            }
            return flags;
        }
    }
}

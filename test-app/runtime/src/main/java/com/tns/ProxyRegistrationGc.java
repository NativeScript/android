package com.tns;

import android.os.Build;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Locale;

/**
 * Whether DexFactory has to run a blocking collection before it registers a
 * generated dex with the app's long-lived class loader.
 *
 * ART's concurrent mark-compact collector can lose the DexCache location store
 * when a collection overlaps ClassLinker::RegisterDexFile, and the app then dies
 * with a SIGSEGV in RegisterDexFileLocked. That collector only reaches devices
 * through the ART module, which is updatable from API 31.
 *
 * The collection is skipped only when the process's heap mappings positively
 * identify the concurrent-copying collector. A collector that can't be
 * identified, including future ART builds with renamed mappings, is treated as
 * affected.
 */
public final class ProxyRegistrationGc {
    private static final int FIRST_UPDATABLE_ART_SDK = 31;

    private static volatile Boolean required;

    private ProxyRegistrationGc() {
    }

    public static boolean isRequired() {
        Boolean result = required;
        if (result == null) {
            result = detect();
            required = result;
        }
        return result;
    }

    private static boolean detect() {
        if (Build.VERSION.SDK_INT < FIRST_UPDATABLE_ART_SDK) {
            return false;
        }
        try (BufferedReader maps = new BufferedReader(new FileReader("/proc/self/maps"))) {
            return isRequired(Build.VERSION.SDK_INT, maps);
        } catch (IOException | RuntimeException e) {
            return true;
        }
    }

    /**
     * @param maps the contents of /proc/self/maps; ART names its heap mappings
     *             "[anon:dalvik-...]" on kernels that support anonymous VMA names
     */
    public static boolean isRequired(int sdkInt, BufferedReader maps) throws IOException {
        if (sdkInt < FIRST_UPDATABLE_ART_SDK) {
            return false;
        }
        boolean concurrentCopying = false;
        String line;
        while ((line = maps.readLine()) != null) {
            int nameStart = line.indexOf("[anon:dalvik-");
            if (nameStart < 0) {
                continue;
            }
            String name = line.substring(nameStart).toLowerCase(Locale.ROOT);
            if (name.contains("mark-compact") || name.contains("mark compact")) {
                return true;
            }
            if (name.contains("concurrent copying") || name.contains("(region space)")) {
                concurrentCopying = true;
            }
        }
        return !concurrentCopying;
    }
}

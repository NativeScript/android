package com.tns;

import java.io.*;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;

import androidx.core.content.pm.PackageInfoCompat;

import com.tns.internal.Plugin;

public final class Util {
    private Util() {
    }

    public static String getDexThumb(Context context) throws NameNotFoundException {
        PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
        long code = PackageInfoCompat.getLongVersionCode(packageInfo);
        long updateTime = packageInfo.lastUpdateTime;
        return updateTime + "-" + code;
    }

    public static boolean isDebuggableApp(Context context) {
        int flags;
        try {
            flags = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).applicationInfo.flags;
        } catch (NameNotFoundException e) {
            flags = 0;
            if (Util.isDebuggableApp(context)) {
                e.printStackTrace();
            }
        }

        boolean isDebuggableApp = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
        return isDebuggableApp;
    }

    static boolean runPlugin(Logger logger, Context context) {
        boolean success = false;
        String pluginClassName = "org.nativescript.livesync.LiveSyncPlugin";
        try {
            ApplicationInfo ai = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
            Bundle metadataBundle = ai.metaData;
            if (metadataBundle != null) {
                pluginClassName = metadataBundle.getString("com.tns.internal.Plugin");
            }
        } catch (Exception e) {
            if (Util.isDebuggableApp(context) && logger.isEnabled()) {
                e.printStackTrace();
            }
        }

        try {
            Class<?> liveSyncPluginClass = Class.forName(pluginClassName);
            Plugin p = (Plugin) liveSyncPluginClass.newInstance();
            success = p.execute(context);
        } catch (Exception e) {
            if (Util.isDebuggableApp(context) && logger.isEnabled()) {
                e.printStackTrace();
            }
        }
        return success;
    }

    public static String readSystemProperty(String name) {
        InputStreamReader in = null;
        BufferedReader reader = null;
        try {
            Process proc = java.lang.Runtime.getRuntime().exec(new String[] { "/system/bin/getprop", name });
            in = new InputStreamReader(proc.getInputStream());
            reader = new BufferedReader(in);
            return reader.readLine();
        } catch (IOException e) {
            return null;
        } finally {
            silentClose(in);
            silentClose(reader);
        }
    }

    private static void silentClose(Closeable closeable) {
        if (closeable == null) {
            return;
        }
        try {
            closeable.close();
        } catch (IOException ignored) {
        }
    }

    public static Boolean isPositive(String value) {
        if (value == null) {
            return false;
        }

        return (value.equals("true") || value.equals("TRUE") ||
                value.equals("yes") || value.equals("YES") ||
                value.equals("enabled") || value.equals("ENABLED"));
    }
}

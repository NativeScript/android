package com.tns;

import java.io.File;
import org.json.JSONObject;
import org.json.JSONArray;
import android.os.Build;
import android.util.Log;
import java.util.ArrayList;
import java.util.List;

class AppConfig {
    protected enum KnownKeys {
        V8FlagsKey("v8Flags", "--expose_gc"),
        CodeCacheKey("codeCache", false),
        ProfilerOutputDirKey("profilerOutputDir", ""),
        GcThrottleTime("gcThrottleTime", 0),
        MemoryCheckInterval("memoryCheckInterval", 0),
        FreeMemoryRatio("freeMemoryRatio", 0.0),
        Profiling("profiling", ""),
        MarkingMode("markingMode", com.tns.MarkingMode.none),
        HandleTimeZoneChanges("handleTimeZoneChanges", false),
        MaxLogcatObjectSize("maxLogcatObjectSize", 1024),
        ForceLog("forceLog", false),
        DiscardUncaughtJsExceptions("discardUncaughtJsExceptions", false),
        EnableLineBreakpoins("enableLineBreakpoints", false),
        EnableMultithreadedJavascript("enableMultithreadedJavascript", false),
        LogScriptLoading("logScriptLoading", false);

        private final String name;
        private final Object defaultValue;

        KnownKeys(String name, Object defaultValue) {
            this.name = name;
            this.defaultValue = defaultValue;
        }

        public String getName() {
            return name;
        }
        public Object getDefaultValue() {
            return defaultValue;
        }
    }

    private final static String AndroidKey = "android";
    private final static String SecurityKey = "security";

    private final Object[] values;
    
    // Security config
    private boolean allowRemoteModules = false;
    private List<String> remoteModuleAllowlist = new ArrayList<>();

    public AppConfig(File appDir) {
        values = makeDefaultOptions();
        File packageInfo = new File(appDir, "/app/package.json");
        if (!packageInfo.exists()) {
            return;
        }

        JSONObject rootObject;
        try {
            rootObject = FileSystem.readJSONFile(packageInfo);
            if (rootObject != null) {
                // Parse security configuration
                if (rootObject.has(SecurityKey)) {
                    JSONObject securityObject = rootObject.getJSONObject(SecurityKey);
                    if (securityObject.has("allowRemoteModules")) {
                        allowRemoteModules = securityObject.getBoolean("allowRemoteModules");
                    }
                    if (securityObject.has("remoteModuleAllowlist")) {
                        JSONArray allowlist = securityObject.getJSONArray("remoteModuleAllowlist");
                        for (int i = 0; i < allowlist.length(); i++) {
                            String url = allowlist.optString(i);
                            if (url != null && !url.isEmpty()) {
                                remoteModuleAllowlist.add(url);
                            }
                        }
                    }
                }
                
                if (rootObject.has(KnownKeys.Profiling.getName())) {
                    String profiling = rootObject.getString(KnownKeys.Profiling.getName());
                    values[KnownKeys.Profiling.ordinal()] = profiling;
                }
                if (rootObject.has(KnownKeys.LogScriptLoading.getName())) {
                    values[KnownKeys.LogScriptLoading.ordinal()] = rootObject.getBoolean(KnownKeys.LogScriptLoading.getName());
                }
                if (rootObject.has(KnownKeys.DiscardUncaughtJsExceptions.getName())) {
                    values[KnownKeys.DiscardUncaughtJsExceptions.ordinal()] = rootObject.getBoolean(KnownKeys.DiscardUncaughtJsExceptions.getName());
                }
                if (rootObject.has(AndroidKey)) {
                    JSONObject androidObject = rootObject.getJSONObject(AndroidKey);
                    if (androidObject.has(KnownKeys.V8FlagsKey.getName())) {
                        values[KnownKeys.V8FlagsKey.ordinal()] = androidObject.getString(KnownKeys.V8FlagsKey.getName());
                    }
                    if (androidObject.has(KnownKeys.CodeCacheKey.getName())) {
                        values[KnownKeys.CodeCacheKey.ordinal()] = androidObject.getBoolean(KnownKeys.CodeCacheKey.getName());
                    }
                    if (androidObject.has(KnownKeys.ProfilerOutputDirKey.getName())) {
                        values[KnownKeys.ProfilerOutputDirKey.ordinal()] = androidObject.getString(KnownKeys.ProfilerOutputDirKey.getName());
                    }
                    if (androidObject.has(KnownKeys.GcThrottleTime.getName())) {
                        values[KnownKeys.GcThrottleTime.ordinal()] = androidObject.getInt(KnownKeys.GcThrottleTime.getName());
                    }
                    if (androidObject.has(KnownKeys.MemoryCheckInterval.getName())) {
                        values[KnownKeys.MemoryCheckInterval.ordinal()] = androidObject.getInt(KnownKeys.MemoryCheckInterval.getName());
                    }
                    if (androidObject.has(KnownKeys.FreeMemoryRatio.getName())) {
                        values[KnownKeys.FreeMemoryRatio.ordinal()] = androidObject.getDouble(KnownKeys.FreeMemoryRatio.getName());
                    }
                    if (androidObject.has(KnownKeys.MarkingMode.getName())) {
                        try {
                            String markingModeString = androidObject.getString(KnownKeys.MarkingMode.getName());
                            MarkingMode markingMode = MarkingMode.valueOf(markingModeString);
                            values[KnownKeys.MarkingMode.ordinal()] = markingMode;
                        } catch (Exception e) {
                            Log.e("JS", String.format("Failed to parse marking mode: %s. The default %s will be used!", e.getMessage(), ((MarkingMode)KnownKeys.MarkingMode.getDefaultValue()).name()));
                            if (com.tns.Runtime.isDebuggable()) {
                                e.printStackTrace();
                            }
                        }
                    }
                    if (androidObject.has(KnownKeys.HandleTimeZoneChanges.getName())) {
                        values[KnownKeys.HandleTimeZoneChanges.ordinal()] = androidObject.getBoolean(KnownKeys.HandleTimeZoneChanges.getName());
                    }
                    if (androidObject.has(KnownKeys.MaxLogcatObjectSize.getName())) {
                        values[KnownKeys.MaxLogcatObjectSize.ordinal()] = androidObject.getInt(KnownKeys.MaxLogcatObjectSize.getName());
                    }
                    if (androidObject.has(KnownKeys.ForceLog.getName())) {
                        values[KnownKeys.ForceLog.ordinal()] = androidObject.getBoolean(KnownKeys.ForceLog.getName());
                    }
                    if (androidObject.has(KnownKeys.EnableLineBreakpoins.getName())) {
                        values[KnownKeys.EnableLineBreakpoins.ordinal()] = androidObject.getBoolean(KnownKeys.EnableLineBreakpoins.getName());
                    }
                    if (androidObject.has(KnownKeys.EnableMultithreadedJavascript.getName())) {
                        values[KnownKeys.EnableMultithreadedJavascript.ordinal()] = androidObject.getBoolean(KnownKeys.EnableMultithreadedJavascript.getName());
                    }
                }
            }
        } catch (Exception e) {
            if (com.tns.Runtime.isDebuggable()) {
                e.printStackTrace();
            }
        }
    }

    public Object[] getAsArray() {
        return values;
    }

    private static Object[] makeDefaultOptions() {
        Object[] result = new Object[KnownKeys.values().length];
        for (KnownKeys key: KnownKeys.values()) {
            result[key.ordinal()] = key.getDefaultValue();
        }
        return result;
    }

    public int getGcThrottleTime() {
        return (int)values[KnownKeys.GcThrottleTime.ordinal()];
    }

    public int getMemoryCheckInterval() {
        return (int)values[KnownKeys.MemoryCheckInterval.ordinal()];
    }

    public double getFreeMemoryRatio() {
        return (double)values[KnownKeys.FreeMemoryRatio.ordinal()];
    }

    public String getProfilingMode() {
        return (String)values[KnownKeys.Profiling.ordinal()];
    }

    public MarkingMode getMarkingMode() {
        return (MarkingMode)values[KnownKeys.MarkingMode.ordinal()];
    }

    public boolean handleTimeZoneChanges() {
        return (boolean)values[KnownKeys.HandleTimeZoneChanges.ordinal()];
    }

    public int getMaxLogcatObjectSize() {
        return (int)values[KnownKeys.MaxLogcatObjectSize.ordinal()];
    }

    public boolean getForceLog() {
        return (boolean)values[KnownKeys.ForceLog.ordinal()];
    }

    public boolean getLineBreakpointsEnabled() {
        return (boolean)values[KnownKeys.EnableLineBreakpoins.ordinal()];
    }

    public boolean getDiscardUncaughtJsExceptions() {
        return (boolean)values[KnownKeys.DiscardUncaughtJsExceptions.ordinal()];
    }

    public boolean getEnableMultithreadedJavascript() {
        return (boolean)values[KnownKeys.EnableMultithreadedJavascript.ordinal()];
    }

    public boolean getLogScriptLoading() {
    Object v = values[KnownKeys.LogScriptLoading.ordinal()];
    return (v instanceof Boolean) ? ((Boolean)v).booleanValue() : false;
    }

    // Security conf
    
    /**
     * Returns true if remote ES modules are allowed in production.
     * Default: false
     */
    public boolean getAllowRemoteModules() {
        return allowRemoteModules;
    }
    
    /**
     * Returns the list of allowed URL prefixes for remote module loading.
     * Only used when allowRemoteModules is true.
     */
    public List<String> getRemoteModuleAllowlist() {
        return remoteModuleAllowlist;
    }
    
    /**
     * Returns the allowlist
     */
    public String[] getRemoteModuleAllowlistArray() {
        return remoteModuleAllowlist.toArray(new String[0]);
    }
}

package com.tns;

import java.io.File;
import org.json.JSONObject;
import android.os.Build;
import android.util.Log;

class AppConfig {
    protected enum KnownKeys {
        V8FlagsKey("v8Flags", "--expose_gc"),
        CodeCacheKey("codeCache", false),
        HeapSnapshotScriptKey("heapSnapshotScript", ""),
        SnapshotFile("snapshot.blob", ""),
        ProfilerOutputDirKey("profilerOutputDir", ""),
        GcThrottleTime("gcThrottleTime", 0),
        MemoryCheckInterval("memoryCheckInterval", 0),
        FreeMemoryRatio("freeMemoryRatio", 0.0),
        Profiling("profiling", ""),
        MarkingMode("markingMode", com.tns.MarkingMode.full),
        HandleTimeZoneChanges("handleTimeZoneChanges", false),
        MaxLogcatObjectSize("maxLogcatObjectSize", 1024),
        ForceLog("forceLog", false),
        DiscardUncaughtJsExceptions("discardUncaughtJsExceptions", false),
        ExperimentalModules("experimentalModules", false);

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
    private final static String HeapSnapshotBlobKey = "heapSnapshotBlob";

    private final Object[] values;

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
                if (rootObject.has(KnownKeys.Profiling.getName())) {
                    String profiling = rootObject.getString(KnownKeys.Profiling.getName());
                    values[KnownKeys.Profiling.ordinal()] = profiling;
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
                    if (androidObject.has(KnownKeys.HeapSnapshotScriptKey.getName())) {
                        String value = androidObject.getString(KnownKeys.HeapSnapshotScriptKey.getName());
                        values[KnownKeys.HeapSnapshotScriptKey.ordinal()] = FileSystem.resolveRelativePath(appDir.getPath(), value, appDir + "/app/");
                    }
                    if (androidObject.has(HeapSnapshotBlobKey)) {
                        String value = androidObject.getString(HeapSnapshotBlobKey);
                        String path = FileSystem.resolveRelativePath(appDir.getPath(), value, appDir + "/app/");
                        File dir = new File(path);
                        if (dir.exists() && dir.isDirectory()) {
                            // this path is expected to be a directory, containing three sub-directories: armeabi-v7a, x86 and arm64-v8a
                            @SuppressWarnings("deprecation")
                            String cpu_abi = Build.CPU_ABI;
                            path = path + "/" + cpu_abi + "/" + KnownKeys.SnapshotFile.getName();
                            values[KnownKeys.SnapshotFile.ordinal()] = path;
                        }
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
                            e.printStackTrace();
                            Log.v("JS", "Failed to parse marking mode. The default " + ((MarkingMode)KnownKeys.MarkingMode.getDefaultValue()).name() + " will be used.");
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
                    if (androidObject.has(KnownKeys.ExperimentalModules.getName())) {
                        values[KnownKeys.ExperimentalModules.ordinal()] = androidObject.getBoolean(KnownKeys.ExperimentalModules.getName());
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
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

    public boolean getDiscardUncaughtJsExceptions() {
        return (boolean)values[KnownKeys.DiscardUncaughtJsExceptions.ordinal()];
    }

    public boolean isExperimentalModules() {
        return (boolean)values[KnownKeys.ExperimentalModules.ordinal()];
    }
}

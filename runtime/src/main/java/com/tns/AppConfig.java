package com.tns;

import java.io.File;

import org.json.JSONObject;

import android.os.Build;
import android.util.Log;

class AppConfig {
    private enum KnownKeys {
        V8FlagsKey("v8Flags", "--expose_gc"),
        CodeCacheKey("codeCache", false),
        HeapSnapshotScriptKey("heapSnapshotScript", ""),
        SnapshotFile("snapshot.blob", ""),
        ProfilerOutputDirKey("profilerOutputDir", ""),
        GcThrottleTime("gcThrottleTime", 0),
        MemoryCheckInterval("memoryCheckInterval", 0),
        FreeMemoryRatio("freeMemoryRatio", 0.0),
        Profiling("profiling", "");

        public static final KnownKeys[] asArray = {
            V8FlagsKey,
            CodeCacheKey,
            HeapSnapshotScriptKey,
            SnapshotFile,
            ProfilerOutputDirKey,
            GcThrottleTime,
            MemoryCheckInterval,
            FreeMemoryRatio,
            Profiling
        };

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

        int getIndex() {
            for (int i=0; i<asArray.length; i++) {
                if (asArray[i] == this) {
                    return i;
                }
            }
            return -1;
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
                    values[KnownKeys.Profiling.getIndex()] = profiling;
                }
                if (rootObject.has(AndroidKey)) {
                    JSONObject androidObject = rootObject.getJSONObject(AndroidKey);
                    if (androidObject.has(KnownKeys.V8FlagsKey.getName())) {
                        values[KnownKeys.V8FlagsKey.getIndex()] = androidObject.getString(KnownKeys.V8FlagsKey.getName());
                    }
                    if (androidObject.has(KnownKeys.CodeCacheKey.getName())) {
                        values[KnownKeys.CodeCacheKey.getIndex()] = androidObject.getBoolean(KnownKeys.CodeCacheKey.getName());
                    }
                    if (androidObject.has(KnownKeys.HeapSnapshotScriptKey.getName())) {
                        String value = androidObject.getString(KnownKeys.HeapSnapshotScriptKey.getName());
                        values[KnownKeys.HeapSnapshotScriptKey.getIndex()] = FileSystem.resolveRelativePath(appDir.getPath(), value, appDir + "/app/");
                    }
                    if (androidObject.has(HeapSnapshotBlobKey)) {
                        String value = androidObject.getString(HeapSnapshotBlobKey);
                        String path = FileSystem.resolveRelativePath(appDir.getPath(), value, appDir + "/app/");
                        File dir = new File(path);
                        if (dir.exists() && dir.isDirectory()) {
                            // this path is expected to be a directory, containing three sub-directories: armeabi-v7a, x86 and arm64-v8a
                            path = path + "/" + Build.CPU_ABI + "/" + KnownKeys.SnapshotFile.getName();
                            values[KnownKeys.SnapshotFile.getIndex()] = path;
                        }
                    }
                    if (androidObject.has(KnownKeys.ProfilerOutputDirKey.getName())) {
                        values[KnownKeys.ProfilerOutputDirKey.getIndex()] = androidObject.getString(KnownKeys.ProfilerOutputDirKey.getName());
                    }
                    if (androidObject.has(KnownKeys.GcThrottleTime.getName())) {
                        values[KnownKeys.GcThrottleTime.getIndex()] = androidObject.getInt(KnownKeys.GcThrottleTime.getName());
                    }
                    if (androidObject.has(KnownKeys.MemoryCheckInterval.getName())) {
                        values[KnownKeys.MemoryCheckInterval.getIndex()] = androidObject.getInt(KnownKeys.MemoryCheckInterval.getName());
                    }
                    if (androidObject.has(KnownKeys.FreeMemoryRatio.getName())) {
                        values[KnownKeys.FreeMemoryRatio.getIndex()] = androidObject.getDouble(KnownKeys.FreeMemoryRatio.getName());
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
        Object[] result = new Object[KnownKeys.asArray.length];
        int index = 0;
        for (KnownKeys key: KnownKeys.asArray) {
            result[index++] = key.getDefaultValue();
        }
        return result;
    }

    public int getGcThrottleTime() {
        return (int)values[KnownKeys.GcThrottleTime.getIndex()];
    }

    public int getMemoryCheckInterval() {
        return (int)values[KnownKeys.MemoryCheckInterval.getIndex()];
    }

    public double getFreeMemoryRatio() {
        return (double)values[KnownKeys.FreeMemoryRatio.getIndex()];
    }

    public String getProfilingMode() {
        return (String)values[KnownKeys.Profiling.getIndex()];
    }
}

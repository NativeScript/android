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
        HandleTimeZoneChanges("handleTimeZoneChanges", false),
        MaxLogcatObjectSize("maxLogcatObjectSize", 1024),
        ForceLog("forceLog", false),
        DiscardUncaughtJsExceptions("discardUncaughtJsExceptions", false),
        EnableLineBreakpoins("enableLineBreakpoints", false),
        EnableMultithreadedJavascript("enableMultithreadedJavascript", false),
        UncaughtErrorPolicy("uncaughtErrorPolicy", "report"),
        InterfaceProxies("interfaceProxies", false),
        WrapperLifetimeMode("mode", "resurrecting"),
        WrapperLifetimeScope("scope", "extended");

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
    // Top level, like iOS, so one app config selects the mode on both platforms.
    private final static String WrapperLifetimeKey = "wrapperLifetime";

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
                // First, and never throwing: a malformed key further down ends
                // the whole read.
                if (rootObject.has(WrapperLifetimeKey)) {
                    readWrapperLifetime(rootObject.opt(WrapperLifetimeKey));
                }

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
                if (rootObject.has(KnownKeys.DiscardUncaughtJsExceptions.getName())) {
                    boolean discard = rootObject.getBoolean(KnownKeys.DiscardUncaughtJsExceptions.getName());
                    if (discard) {
                        // Legacy routing (__onDiscardedError) is preserved for the transition.
                        values[KnownKeys.DiscardUncaughtJsExceptions.ordinal()] = true;
                        Log.w("JS", "discardUncaughtJsExceptions is deprecated: uncaught JS errors no longer crash the app by default. Remove the flag, or use uncaughtErrorPolicy.");
                    } else {
                        // `false` used to mean "crash on uncaught errors" (the old
                        // default). It is NOT mapped to uncaughtErrorPolicy: "throw" -
                        // set that explicitly if crashing is desired.
                        Log.w("JS", "discardUncaughtJsExceptions is deprecated and `false` is ignored: uncaught JS errors are reported without crashing by default. Set uncaughtErrorPolicy: \"throw\" to restore the old crashing behavior.");
                    }
                }
                if (rootObject.has(KnownKeys.UncaughtErrorPolicy.getName())) {
                    String policy = rootObject.getString(KnownKeys.UncaughtErrorPolicy.getName());
                    if ("report".equals(policy) || "throw".equals(policy)) {
                        values[KnownKeys.UncaughtErrorPolicy.ordinal()] = policy;
                    } else {
                        Log.w("JS", String.format("Unknown uncaughtErrorPolicy \"%s\" - expected \"report\" or \"throw\". The default \"report\" will be used.", policy));
                    }
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
                    String markingMode = androidObject.optString("markingMode", "none");
                    if (!markingMode.equals("none")) {
                        Log.w("JS", String.format("markingMode \"%s\" is no longer supported; \"none\" is used.", markingMode));
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
                    if (androidObject.has(KnownKeys.InterfaceProxies.getName())) {
                        values[KnownKeys.InterfaceProxies.ordinal()] = androidObject.getBoolean(KnownKeys.InterfaceProxies.getName());
                    }
                }
            }
        } catch (Exception e) {
            Log.w("JS", "Could not read the app config past a malformed value; the keys read after it keep their defaults: " + e);
            if (com.tns.Runtime.isDebuggable()) {
                e.printStackTrace();
            }
        }
    }

    private void readWrapperLifetime(Object config) {
        if (!(config instanceof JSONObject)) {
            Log.w("JS", "wrapperLifetime must be an object; using the defaults.");
            return;
        }
        JSONObject object = (JSONObject) config;
        Object mode = object.opt(KnownKeys.WrapperLifetimeMode.getName());
        if (mode != null) {
            if ("resurrecting".equals(mode) || "canary".equals(mode)) {
                values[KnownKeys.WrapperLifetimeMode.ordinal()] = mode;
            } else if ("rooted".equals(mode)) {
                Log.w("JS", "wrapperLifetime mode \"rooted\" reads native retain counts and is iOS-only; using \"resurrecting\".");
            } else {
                Log.w("JS", String.format("Unknown wrapperLifetime mode %s; using \"resurrecting\".", JSONObject.quote(String.valueOf(mode))));
            }
        }
        Object scope = object.opt(KnownKeys.WrapperLifetimeScope.getName());
        if (scope != null) {
            if ("extended".equals(scope) || "all".equals(scope)) {
                values[KnownKeys.WrapperLifetimeScope.ordinal()] = scope;
            } else {
                Log.w("JS", String.format("Unknown wrapperLifetime scope %s; using \"extended\".", JSONObject.quote(String.valueOf(scope))));
            }
        }
    }

    public String getWrapperLifetimeMode() {
        return (String)values[KnownKeys.WrapperLifetimeMode.ordinal()];
    }

    public String getWrapperLifetimeScope() {
        return (String)values[KnownKeys.WrapperLifetimeScope.ordinal()];
    }

    public String getV8Flags() {
        return (String)values[KnownKeys.V8FlagsKey.ordinal()];
    }

    public boolean getCodeCache() {
        return (boolean)values[KnownKeys.CodeCacheKey.ordinal()];
    }

    public String getProfilerOutputDir() {
        return (String)values[KnownKeys.ProfilerOutputDirKey.ordinal()];
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

    public String getUncaughtErrorPolicy() {
        return (String)values[KnownKeys.UncaughtErrorPolicy.ordinal()];
    }

    public boolean getEnableMultithreadedJavascript() {
        return (boolean)values[KnownKeys.EnableMultithreadedJavascript.ordinal()];
    }

    /**
     * Implement an interface that has no class in the APK with a java.lang.reflect.Proxy
     * instead of generating, writing and loading a dex for it.
     */
    public boolean getInterfaceProxies() {
        return (boolean)values[KnownKeys.InterfaceProxies.ordinal()];
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

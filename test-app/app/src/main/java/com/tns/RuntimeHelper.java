package com.tns;

import android.app.Application;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.preference.PreferenceManager;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.TimeZone;

public final class RuntimeHelper {
    private RuntimeHelper() {
    }

    private static AndroidJsV8Inspector v8Inspector;

    // hasErrorIntent tells you if there was an event (with an uncaught
    // exception) raised from ErrorReport
    private static boolean hasErrorIntent(Context context) {
        boolean hasErrorIntent = false;

        try {
            // empty file just to check if there was a raised uncaught error by
            // ErrorReport
            if (Util.isDebuggableApp(context)) {
                String fileName;

                try {
                    Class<?> ErrReport = Class.forName("com.tns.ErrorReport");
                    Field field = ErrReport.getDeclaredField("ERROR_FILE_NAME");
                    fileName = (String) field.get(null);
                } catch (Exception e) {
                    return false;
                }

                File errFile = new File(context.getFilesDir(), fileName);

                if (errFile.exists()) {
                    errFile.delete();
                    hasErrorIntent = true;
                }
            }
        } catch (Exception e) {
            Log.d(logTag, e.getMessage());
        }

        return hasErrorIntent;
    }

    public static Runtime initRuntime(Context context) {
        if (Runtime.isInitialized()) {
            return Runtime.getCurrentRuntime();
        }

        ManualInstrumentation.Frame frame = ManualInstrumentation.start("RuntimeHelper.initRuntime");
        try {
            ManualInstrumentation.Frame loadLibraryFrame = ManualInstrumentation.start("loadLibrary NativeScript");
            try {
                System.loadLibrary("NativeScript");
            } finally {
                loadLibraryFrame.close();
            }

            Logger logger = new LogcatLogger(context);

            Runtime.nativeLibraryLoaded = true;
            Runtime runtime = null;
            boolean showErrorIntent = hasErrorIntent(context);
            if (!showErrorIntent) {
                NativeScriptUncaughtExceptionHandler exHandler = new NativeScriptUncaughtExceptionHandler(logger, context);

                Thread.setDefaultUncaughtExceptionHandler(exHandler);

                DefaultExtractPolicy extractPolicy = new DefaultExtractPolicy(logger);
                boolean skipAssetExtraction = Util.runPlugin(logger, context);

                String appName = context.getPackageName();
                File rootDir = new File(context.getApplicationInfo().dataDir);
                File appDir = context.getFilesDir();

                try {
                    appDir = appDir.getCanonicalFile();
                } catch (IOException e1) {
                }

                if (!skipAssetExtraction) {
                    ManualInstrumentation.Frame extractionFrame = ManualInstrumentation.start("Extracting assets");
                    try {
                        if (logger.isEnabled()) {
                            logger.write("Extracting assets...");
                        }

                        AssetExtractor aE = new AssetExtractor(null, logger);

                        String outputDir = context.getFilesDir().getPath() + File.separator;

                        // will force deletion of previously extracted files in app/files directories
                        // see https://github.com/NativeScript/NativeScript/issues/4137 for reference
                        boolean removePreviouslyInstalledAssets = true;
                        aE.extractAssets(context, "app", outputDir, extractPolicy, removePreviouslyInstalledAssets);
                        aE.extractAssets(context, "internal", outputDir, extractPolicy, removePreviouslyInstalledAssets);
                        aE.extractAssets(context, "metadata", outputDir, extractPolicy, false);

                        boolean shouldExtractSnapshots = true;

                        // will extract snapshot of the device appropriate architecture
                        if (shouldExtractSnapshots) {
                            if (logger.isEnabled()) {
                                logger.write("Extracting snapshot blob");
                            }

                            @SuppressWarnings("deprecation")
                            String cpu_abi = Build.CPU_ABI;
                            aE.extractAssets(context, "snapshots/" + cpu_abi, outputDir, extractPolicy, removePreviouslyInstalledAssets);
                        }

                        extractPolicy.setAssetsThumb(context);
                    } finally {
                        extractionFrame.close();
                    }
                }

                AppConfig appConfig = new AppConfig(appDir);
                ManualInstrumentation.setMode(appConfig.getProfilingMode());

                ClassLoader classLoader = context.getClassLoader();
                File dexDir = new File(rootDir, "code_cache/secondary-dexes");
                String dexThumb = null;
                try {
                    dexThumb = Util.getDexThumb(context);
                } catch (NameNotFoundException e) {
                    if (logger.isEnabled()) {
                        logger.write("Error while getting current proxy thumb");
                    }
                    if (Util.isDebuggableApp(context)) {
                        e.printStackTrace();
                    }
                }

                String nativeLibDir = null;
                try {
                    nativeLibDir = context.getPackageManager().getApplicationInfo(appName, 0).nativeLibraryDir;
                } catch (NameNotFoundException e) {
                    if (Util.isDebuggableApp(context)) {
                        e.printStackTrace();
                    }
                }

                boolean isDebuggable = Util.isDebuggableApp(context);
                StaticConfiguration config = new StaticConfiguration(logger, appName, nativeLibDir, rootDir,
                        appDir, classLoader, dexDir, dexThumb, appConfig, isDebuggable);

                runtime = Runtime.initializeRuntimeWithConfiguration(config);
                if (isDebuggable) {
                    try {
                        v8Inspector = new AndroidJsV8Inspector(context.getFilesDir().getAbsolutePath(), context.getPackageName());
                        v8Inspector.start();

                        // the following snippet is used as means to notify the VSCode extension
                        // debugger that the debugger agent has started
                        File debuggerStartedFile = new File("/data/local/tmp", context.getPackageName() + "-debugger-started");
                        if (debuggerStartedFile.exists() && !debuggerStartedFile.isDirectory() && debuggerStartedFile.length() == 0) {
                            java.io.FileWriter fileWriter = new java.io.FileWriter(debuggerStartedFile);
                            fileWriter.write("started");
                            fileWriter.close();
                        }

                        // check if --debug-brk flag has been set. If positive:
                        // write to the file to invalidate the flag
                        // inform the v8Inspector to pause the main thread
                        File debugBreakFile = new File("/data/local/tmp", context.getPackageName() + "-debugbreak");
                        boolean shouldBreak = false;
                        if (debugBreakFile.exists() && !debugBreakFile.isDirectory() && debugBreakFile.length() == 0) {
                            java.io.FileWriter fileWriter = new java.io.FileWriter(debugBreakFile);
                            fileWriter.write("started");
                            fileWriter.close();

                            shouldBreak = true;
                        }

                        v8Inspector.waitForDebugger(shouldBreak);
                    } catch (IOException e) {
                        if (Util.isDebuggableApp(context)) {
                            e.printStackTrace();
                        }
                    }

                    // if app is in debuggable mode run livesync service
                    // runtime needs to be initialized before the NativeScriptSyncService is enabled because it uses runtime.runScript(...)
                    initLiveSync(runtime, logger, context);

                    waitForLiveSync(context);
                }

                runtime.runScript(new File(appDir, "internal/ts_helpers.js"));

                File javaClassesModule = new File(appDir, "app/tns-java-classes.js");
                if (javaClassesModule.exists()) {
                    runtime.runModule(javaClassesModule);
                }

                try {
                    // put this call in a try/catch block because with the latest changes in the modules it is not granted that NativeScriptApplication is extended through JavaScript.
                    JavaScriptImplementation jsImpl = context.getClass().getAnnotation(JavaScriptImplementation.class);
                    if (jsImpl != null && !(context instanceof android.app.Service)) {
                        Runtime.initInstance(context);
                    }
                } catch (Exception e) {
                    if (logger.isEnabled()) {
                        logger.write("Cannot initialize application instance.");
                    }
                    if (Util.isDebuggableApp(context)) {
                        e.printStackTrace();
                    }
                }

                if (appConfig.handleTimeZoneChanges()) {
                    // If the user sets this flag, we will register a broadcast receiver
                    // that will listen for the TIMEZONE_CHANGED event and update V8's cache
                    // so that subsequent calls to "new Date()" return the new timezone
                    registerTimezoneChangedListener(context, runtime);
                }
            }
            return runtime;
        } finally {
            frame.close();
        }
    }

    private static void waitForLiveSync(Context context) {
        boolean needToWait = false;

        // CLI will create this file when initial sync is needed and then will remove it after syncing the fails and restarting the app
        File liveSyncFile = new File("/data/local/tmp/" + context.getPackageName() + "-livesync-in-progress");
        if (liveSyncFile.exists()) {
            needToWait = true;
            Long lastModified = liveSyncFile.lastModified();
            // we check for lastModified == 0 as this might happen if we cannot get the actual modified date
            if (lastModified > 0) {
                Long fileCreatedBeforeMillis = System.currentTimeMillis() - lastModified;
                // if last modified date is more than a minute before the current time discard the file as most probably this is a leftover
                if (fileCreatedBeforeMillis > 60000) {
                    needToWait = false;
                }
            }
        }

        if (needToWait) {
            try {
                // wait for the livesync to complete and it should restart the app after deleting the livesync-in-progress file
                Thread.sleep(30000);
            } catch (Exception ex) {
            }
        }
    }

    private static void registerTimezoneChangedListener(Context context, final Runtime runtime) {
        IntentFilter timezoneFilter = new IntentFilter(Intent.ACTION_TIMEZONE_CHANGED);

        BroadcastReceiver timezoneReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (action == null || !action.equals(Intent.ACTION_TIMEZONE_CHANGED)) {
                    return;
                }

                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

                String oldTimezone = prefs.getString(PREF_TIMEZONE, null);
                String newTimezone = TimeZone.getDefault().getID();
                if (newTimezone == null) {
                    newTimezone = "";
                }

                if (oldTimezone == null) {
                    oldTimezone = "";
                }

                if (!oldTimezone.equals(newTimezone)) {
                    prefs.edit().putString(PREF_TIMEZONE, newTimezone).commit();
                    // Notify V8 for the timezone change
                    runtime.ResetDateTimeConfigurationCache();
                }
            }
        };

        context.registerReceiver(timezoneReceiver, timezoneFilter);
    }

    public static void initLiveSync(Application app) {
        Runtime currentRuntime = Runtime.getCurrentRuntime();
        if (!currentRuntime.getIsLiveSyncStarted()) {
            initLiveSync(currentRuntime, currentRuntime.getLogger(), app);
            currentRuntime.setIsLiveSyncStarted(true);
        }

    }

    public static void initLiveSync(Runtime runtime, Logger logger, Context context) {
        boolean isDebuggable = Util.isDebuggableApp(context);

        if (!isDebuggable) {
            return;
        }

        // if app is in debuggable mode run livesync service
        // runtime needs to be initialized before the NativeScriptSyncService is enabled because it uses runtime.runScript(...)
        try {
            @SuppressWarnings("unchecked")
            Class<?> NativeScriptSyncService = Class.forName("com.tns.NativeScriptSyncServiceSocketImpl");

            @SuppressWarnings("unchecked")
            Constructor<?> cons = NativeScriptSyncService.getConstructor(new Class<?>[]{Runtime.class, Logger.class, Context.class});
            Object syncService = cons.newInstance(runtime, logger, context);

            @SuppressWarnings("unchecked")
            Method startServerMethod = NativeScriptSyncService.getMethod("startServer");
            startServerMethod.invoke(syncService);
        } catch (ClassNotFoundException e) {
            if (Util.isDebuggableApp(context)) {
                e.printStackTrace();
            }
        } catch (NoSuchMethodException e) {
            if (Util.isDebuggableApp(context)) {
                e.printStackTrace();
            }
        } catch (IllegalAccessException e) {
            if (Util.isDebuggableApp(context)) {
                e.printStackTrace();
            }
        } catch (InvocationTargetException e) {
            if (Util.isDebuggableApp(context)) {
                e.printStackTrace();
            }
        } catch (InstantiationException e) {
            if (Util.isDebuggableApp(context)) {
                e.printStackTrace();
            }
        }
    }

    private static final String logTag = "MyApp";
    private static final String PREF_TIMEZONE = "_android_runtime_pref_timezone_";
}

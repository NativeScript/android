package com.tns;

import java.io.File;

import android.app.Application;
import android.content.Context;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.util.Log;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public final class RuntimeHelper {
    private RuntimeHelper() {
    }

    private static AndroidJsV8Inspector v8Inspector;

    // hasErrorIntent tells you if there was an event (with an uncaught
    // exception) raised from ErrorReport
    private static boolean hasErrorIntent(Application app) {
        boolean hasErrorIntent = false;

        try {
            // empty file just to check if there was a raised uncaught error by
            // ErrorReport
            if (Util.isDebuggableApp(app)) {
                String fileName = "";

                try {
                    Class ErrReport = Class.forName("com.tns.ErrorReport");
                    Field field = ErrReport.getDeclaredField("ERROR_FILE_NAME");
                    fileName = (String) field.get(null);
                } catch (Exception e) {
                    return false;
                }

                File errFile = new File(app.getFilesDir(), fileName);

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

    public static Runtime initRuntime(Application app) {
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

            Logger logger = new LogcatLogger(app);

            Runtime runtime = null;
            boolean showErrorIntent = hasErrorIntent(app);
            if (!showErrorIntent) {
                NativeScriptUncaughtExceptionHandler exHandler = new NativeScriptUncaughtExceptionHandler(logger, app);

                Thread.setDefaultUncaughtExceptionHandler(exHandler);

                DefaultExtractPolicy extractPolicy = new DefaultExtractPolicy(logger);
                boolean skipAssetExtraction = Util.runPlugin(logger, app);

                String appName = app.getPackageName();
                File rootDir = new File(app.getApplicationInfo().dataDir);
                File appDir = app.getFilesDir();

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

                        String outputDir = app.getFilesDir().getPath() + File.separator;

                        // will force deletion of previously extracted files in app/files directories
                        // see https://github.com/NativeScript/NativeScript/issues/4137 for reference
                        boolean removePreviouslyInstalledAssets = true;
                        aE.extractAssets(app, "app", outputDir, extractPolicy, removePreviouslyInstalledAssets);
                        aE.extractAssets(app, "internal", outputDir, extractPolicy, removePreviouslyInstalledAssets);
                        aE.extractAssets(app, "metadata", outputDir, extractPolicy, false);

                        boolean shouldExtractSnapshots = true;

                        // will extract snapshot of the device appropriate architecture
                        if (shouldExtractSnapshots) {
                            if (logger.isEnabled()) {
                                logger.write("Extracting snapshot blob");
                            }

                            aE.extractAssets(app, "snapshots/" + Build.CPU_ABI, outputDir, extractPolicy, removePreviouslyInstalledAssets);
                        }

                        extractPolicy.setAssetsThumb(app);
                    } finally {
                        extractionFrame.close();
                    }
                }

                AppConfig appConfig = new AppConfig(appDir);
                ManualInstrumentation.setMode(appConfig.getProfilingMode());

                ClassLoader classLoader = app.getClassLoader();
                File dexDir = new File(rootDir, "code_cache/secondary-dexes");
                String dexThumb = null;
                try {
                    dexThumb = Util.getDexThumb(app);
                } catch (NameNotFoundException e) {
                    if (logger.isEnabled()) {
                        logger.write("Error while getting current proxy thumb");
                    }
                    e.printStackTrace();
                }

                String nativeLibDir = null;
                try {
                    nativeLibDir = app.getPackageManager().getApplicationInfo(appName, 0).nativeLibraryDir;
                } catch (android.content.pm.PackageManager.NameNotFoundException e) {
                    e.printStackTrace();
                }

                boolean isDebuggable = Util.isDebuggableApp(app);
                StaticConfiguration config = new StaticConfiguration(logger, appName, nativeLibDir, rootDir,
                        appDir, classLoader, dexDir, dexThumb, appConfig, isDebuggable);

                runtime = Runtime.initializeRuntimeWithConfiguration(config);
                if (isDebuggable) {
                    try {
                        v8Inspector = new AndroidJsV8Inspector(app.getFilesDir().getAbsolutePath(), app.getPackageName());
                        v8Inspector.start();

                        // the following snippet is used as means to notify the VSCode extension
                        // debugger that the debugger agent has started
                        File debuggerStartedFile = new File("/data/local/tmp", app.getPackageName() + "-debugger-started");
                        if (debuggerStartedFile.exists() && !debuggerStartedFile.isDirectory() && debuggerStartedFile.length() == 0) {
                            java.io.FileWriter fileWriter = new java.io.FileWriter(debuggerStartedFile);
                            fileWriter.write("started");
                            fileWriter.close();
                        }

                        // check if --debug-brk flag has been set. If positive:
                        // write to the file to invalidate the flag
                        // inform the v8Inspector to pause the main thread
                        File debugBreakFile = new File("/data/local/tmp", app.getPackageName() + "-debugbreak");
                        boolean shouldBreak = false;
                        if (debugBreakFile.exists() && !debugBreakFile.isDirectory() && debugBreakFile.length() == 0) {
                            java.io.FileWriter fileWriter = new java.io.FileWriter(debugBreakFile);
                            fileWriter.write("started");
                            fileWriter.close();

                            shouldBreak = true;
                        }

                        v8Inspector.waitForDebugger(shouldBreak);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    // if app is in debuggable mode run livesync service
                    // runtime needs to be initialized before the NativeScriptSyncService is enabled because it uses runtime.runScript(...)
                    try {
                        Class NativeScriptSyncService = Class.forName("com.tns.NativeScriptSyncService");

                        Constructor cons = NativeScriptSyncService.getConstructor(new Class[] {Runtime.class, Logger.class, Context.class});
                        Object syncService = cons.newInstance(runtime, logger, app);

                        Method syncMethod = NativeScriptSyncService.getMethod("sync");
                        syncMethod.invoke(syncService);
                        Method startServerMethod = NativeScriptSyncService.getMethod("startServer");
                        startServerMethod.invoke(syncService);
                    } catch (ClassNotFoundException e) {
                        e.printStackTrace();
                    } catch (NoSuchMethodException e) {
                        e.printStackTrace();
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    } catch (InvocationTargetException e) {
                        e.printStackTrace();
                    } catch (InstantiationException e) {
                        e.printStackTrace();
                    }
                }

                runtime.runScript(new File(appDir, "internal/ts_helpers.js"));

                File javaClassesModule = new File(appDir, "app/tns-java-classes.js");
                if (javaClassesModule.exists()) {
                    runtime.runModule(javaClassesModule);
                }

                try {
                    // put this call in a try/catch block because with the latest changes in the modules it is not granted that NativeScriptApplication is extended through JavaScript.
                    JavaScriptImplementation jsImpl = app.getClass().getAnnotation(JavaScriptImplementation.class);
                    if (jsImpl != null) {
                        Runtime.initInstance(app);
                    }
                } catch (Exception e) {
                    if (logger.isEnabled()) {
                        logger.write("Cannot initialize application instance.");
                    }
                    e.printStackTrace();
                }
            }
            return runtime;
        } finally {
            frame.close();
        }
    }

    private static final String logTag = "MyApp";
}

package com.tns;

import java.io.File;

import android.app.Application;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.util.Log;
import java.io.IOException;

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
                    java.lang.Class ErrReport = java.lang.Class.forName("com.tns.ErrorReport");
                    java.lang.reflect.Field field = ErrReport.getDeclaredField("ERROR_FILE_NAME");
                    fileName = (String)field.get(null);
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

        System.loadLibrary("NativeScript");

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
                if (logger.isEnabled()) {
                    logger.write("Extracting assets...");
                }

                AssetExtractor aE = new AssetExtractor(null, logger);

                String outputDir = app.getFilesDir().getPath() + File.separator;

                aE.extractAssets(app, "app", outputDir, extractPolicy);
                aE.extractAssets(app, "internal", outputDir, extractPolicy);
                aE.extractAssets(app, "metadata", outputDir, extractPolicy);

                // enable with flags?
                boolean shouldExtractSnapshots = true;

                // will extract snapshot of the device appropriate architecture
                if (shouldExtractSnapshots) {
                    if (logger.isEnabled()) {
                        logger.write("Extracting snapshot blob");
                    }

                    aE.extractAssets(app,  "snapshots/" + Build.CPU_ABI, outputDir, extractPolicy);
                }

                extractPolicy.setAssetsThumb(app);
            }

            AppConfig appConfig = new AppConfig(appDir);

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
                    v8Inspector = new AndroidJsV8Inspector(app, logger);
                    v8Inspector.start();

                    // the following snippet is used as means to notify the VSCode extension
                    // debugger that the debugger agent has started
                    File debugBreakFile = new File("/data/local/tmp", app.getPackageName() + "-debugger-started");
                    if (debugBreakFile.exists() && !debugBreakFile.isDirectory() && debugBreakFile.length() == 0) {
                        java.io.FileWriter fileWriter = new java.io.FileWriter(debugBreakFile);
                        fileWriter.write("started");
                        fileWriter.close();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            // runtime needs to be initialized before the NativeScriptSyncService is enabled because it uses runtime.runScript(...)
            if (NativeScriptSyncService.isSyncEnabled(app)) {
                NativeScriptSyncService syncService = new NativeScriptSyncService(runtime, logger, app);

                syncService.sync();
                syncService.startServer();

                // preserve this instance as strong reference
                // do not preserve in NativeScriptApplication field inorder to
                // make the code more portable
                // @@@
                // Runtime.getOrCreateJavaObjectID(syncService);
            } else {
                if (logger.isEnabled()) {
                    logger.write("NativeScript LiveSync is not enabled.");
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
    }

    private static final String logTag = "MyApp";
}

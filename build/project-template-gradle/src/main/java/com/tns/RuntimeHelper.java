package com.tns;

import java.io.File;

import android.app.Application;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import java.io.IOException;

public class RuntimeHelper {
	private final Application app;

	public RuntimeHelper(Application app) {
		this.app = app;
	}

	// hasErrorIntent tells you if there was an event (with an uncaught
	// exception) raised from ErrorReport
	public boolean hasErrorIntent() {
		boolean hasErrorIntent = false;

		try {
			// empty file just to check if there was a raised uncaught error by
			// ErrorReport
			File errFile = new File(app.getFilesDir(), ErrorReport.ERROR_FILE_NAME);

			if (errFile.exists()) {
				errFile.delete();
				hasErrorIntent = true;
			}
		} catch (Exception e) {
			Log.d(logTag, e.getMessage());
		}

		return hasErrorIntent;
	}
	
	public void initRuntime()
	{
		if (Runtime.isInitialized()) {
			return;
		}
		
		System.loadLibrary("NativeScript");

		Logger logger = new LogcatLogger(app);
		Debugger debugger = new AndroidJsDebugger(app, logger);

		boolean showErrorIntent = hasErrorIntent();
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
				if(logger.isEnabled()) {
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
				if(shouldExtractSnapshots) {
					if(logger.isEnabled()) {
						logger.write("Extracting snapshot blob");
					}

					aE.extractAssets(app,  "snapshots/" + Build.CPU_ABI, outputDir, extractPolicy);
				}

				extractPolicy.setAssetsThumb(app);
			}

			Object[] v8Config = V8Config.fromPackageJSON(appDir);
			
			ClassLoader classLoader = app.getClassLoader();
			File dexDir = new File(rootDir, "code_cache/secondary-dexes");
			String dexThumb = null;
			try {
				dexThumb = Util.getDexThumb(app);
			} catch (NameNotFoundException e) {
				if (logger.isEnabled())
					logger.write("Error while getting current proxy thumb");
				e.printStackTrace();
			}
			ThreadScheduler workThreadScheduler = new WorkThreadScheduler(new Handler(Looper.getMainLooper()));
			Configuration config = new Configuration(workThreadScheduler, logger, debugger, appName, null, rootDir,
					appDir, classLoader, dexDir, dexThumb, v8Config);
			Runtime runtime = new Runtime(config);

			exHandler.setRuntime(runtime);

			if (NativeScriptSyncService.isSyncEnabled(this.app)) {
				NativeScriptSyncService syncService = new NativeScriptSyncService(runtime, logger, this.app);

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

			runtime.init();
			runtime.runScript(new File(appDir, "internal/ts_helpers.js"));
			try {
				// put this call in a try/catch block because with the latest changes in the modules it is not granted that NativeScriptApplication is extended through JavaScript.
				Runtime.initInstance(this.app);
			}
			catch (Exception e) {
				
			}
			runtime.run();
		}
	}

	private final String logTag = "MyApp";
}

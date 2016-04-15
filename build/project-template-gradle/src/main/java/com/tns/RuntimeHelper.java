package com.tns;

import java.io.File;

import android.app.Application;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import java.io.IOException;

public class RuntimeHelper
{
	private final Application app;
	
	public RuntimeHelper(Application app)
	{
		this.app = app;
	}
	
	// hasErrorIntent tells you if there was an event (with an uncaught exception) raised from ErrorReport
	public boolean hasErrorIntent()
	{
		boolean hasErrorIntent = false;
		
		try
		{
			 //empty file just to check if there was a raised uncaught error by ErrorReport
			File errFile = new File(app.getFilesDir(), ErrorReport.ERROR_FILE_NAME);
			
			if (errFile.exists())
			{
				errFile.delete();
				hasErrorIntent = true;
			}
		}
		catch (Exception e)
		{
			Log.d(logTag, e.getMessage());
		}
		
		return hasErrorIntent;
	}
	
	public void initRuntime()
	{
		System.loadLibrary("NativeScript");
		
		Logger logger = new LogcatLogger(false, app);
		
		boolean showErrorIntent = hasErrorIntent();
		if (!showErrorIntent)
		{
			NativeScriptUncaughtExceptionHandler exHandler = new NativeScriptUncaughtExceptionHandler(logger, app);

			Thread.setDefaultUncaughtExceptionHandler(exHandler);
			
			Async.Http.setApplicationContext(this.app);
			
			ExtractPolicy extractPolicy = new DefaultExtractPolicy(logger);
			boolean skipAssetExtraction = Util.runPlugin(logger, app);
			if (!skipAssetExtraction)
			{
				new AssetExtractor(null, logger).extractAssets(app, extractPolicy);
			}
			
			String appName = app.getPackageName();
			File rootDir = new File(app.getApplicationInfo().dataDir);
			File appDir = app.getFilesDir();
			
			try
			{
				appDir = appDir.getCanonicalFile();
			}
			catch (IOException e1)
			{
			}

			ClassLoader classLoader = app.getClassLoader();
			File dexDir = new File(rootDir, "code_cache/secondary-dexes");
			String dexThumb = null;
			try
			{
				dexThumb = Util.getDexThumb(app);
			}
			catch (NameNotFoundException e)
			{
				if (logger.isEnabled()) logger.write("Error while getting current proxy thumb");
				e.printStackTrace();
			}
			ThreadScheduler workThreadScheduler = new WorkThreadScheduler(new Handler(Looper.getMainLooper()));
			Configuration config = new Configuration(this.app, workThreadScheduler, logger, appName, null, rootDir, appDir, classLoader, dexDir, dexThumb);
			Runtime runtime = new Runtime(config);
			
			exHandler.setRuntime(runtime);
			
			if (NativeScriptSyncService.isSyncEnabled(this.app))
			{
				NativeScriptSyncService syncService = new NativeScriptSyncService(runtime, logger, this.app);

				syncService.sync();
				syncService.startServer();

				// preserve this instance as strong reference
				// do not preserve in NativeScriptApplication field inorder to make the code more portable
				// @@@
				//Runtime.getOrCreateJavaObjectID(syncService);
			}
			else
			{
				if (logger.isEnabled())
				{
					logger.write("NativeScript LiveSync is not enabled.");
				}
			}
			
			

			
			runtime.init();
			runtime.runScript(new File(appDir, "internal/ts_helpers.js"));

			File javaClassesModule = new File(appDir, "app/tns-java-classes.js");
			if (javaClassesModule.exists()) {
				runtime.runModule(javaClassesModule);
			}

			Runtime.initInstance(this.app);
			runtime.run();
		}
	}
	
	private final String logTag = "MyApp";
}

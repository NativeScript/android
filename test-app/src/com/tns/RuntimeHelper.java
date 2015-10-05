package com.tns;

import java.io.File;

import android.app.Application;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

class RuntimeHelper
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
		//Logger logger = new LogcatLogger(BuildConfig.DEBUG, this);
		Logger logger = new LogcatLogger(true, app);
		
		boolean showErrorIntent = hasErrorIntent();
		if (!showErrorIntent)
		{
			Thread.UncaughtExceptionHandler exHandler = new MyUncaughtExceptionHandler(logger, app);

			Thread.setDefaultUncaughtExceptionHandler(exHandler);
			
			boolean shouldEnableDebugging = Util.isDebuggableApp(app);
					
			if (shouldEnableDebugging)
			{
				JsDebugger.registerEnableDisableDebuggerReceiver(app);
				JsDebugger.registerGetDebuggerPortReceiver(app);
			}
			
			// TODO: refactor
			ExtractPolicy extractPolicy = new DefaultExtractPolicy(logger);
			boolean skipAssetExtraction = Util.runPlugin(logger, app);
			if (!skipAssetExtraction)
			{
				new AssetExtractor(null, logger).extractAssets(app, extractPolicy);
			}
			
			NativeScriptSyncHelper.sync(logger, app);

			String appName = app.getPackageName();
			File rootDir = new File(app.getApplicationInfo().dataDir);
			File appDir = app.getFilesDir();
			File debuggerSetupDir = Util.isDebuggableApp(app)
										? app.getExternalFilesDir(null)
										: null;
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
			Platform.init(workThreadScheduler, logger, appName, null, rootDir, appDir, debuggerSetupDir, classLoader, dexDir, dexThumb);
			Platform.runScript(new File(appDir, "internal/ts_helpers.js"));
		}
	}
	
	private final String logTag = "MyApp";
}

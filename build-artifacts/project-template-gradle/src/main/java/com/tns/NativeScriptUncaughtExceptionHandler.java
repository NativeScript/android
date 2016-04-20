package com.tns;

import java.lang.Thread.UncaughtExceptionHandler;

import android.content.Context;

public class NativeScriptUncaughtExceptionHandler implements UncaughtExceptionHandler
{
	private final Context context;

	private final UncaughtExceptionHandler defaultHandler;

	private final Logger logger;
	
	private Runtime runtime;
	
	public NativeScriptUncaughtExceptionHandler(Logger logger, Context context)
	{
		this.logger = logger;
		this.context = context;
		defaultHandler = Thread.getDefaultUncaughtExceptionHandler();
	}
	
	public void setRuntime(Runtime runtime)
	{
		this.runtime = runtime;
	}

	@Override
	public void uncaughtException(Thread thread, Throwable ex)
	{
		String errorMessage = ErrorReport.getErrorMessage(ex);
		
		if (Runtime.isInitialized())
		{
			try
			{
				ex.printStackTrace();
				if (runtime != null)
				{
					runtime.passUncaughtExceptionToJs(ex, errorMessage);
				}
				
				if (JsDebugger.isJsDebuggerActive())
				{
					return;
				}
			}
			catch (Throwable t)
			{
				t.printStackTrace();
			}
		}

		if (logger.isEnabled())
		{
			logger.write("Uncaught Exception Message=" + errorMessage);
		}

		if (!ErrorReport.startActivity(context, errorMessage) && defaultHandler != null)
		{
			defaultHandler.uncaughtException(thread, ex);
		}
	}
}

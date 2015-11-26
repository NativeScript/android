package com.tns;

import java.lang.Thread.UncaughtExceptionHandler;

import android.content.Context;

public class NativeScriptUncaughtExceptionHandler implements UncaughtExceptionHandler 
{
	private final Context context;
	
	private final UncaughtExceptionHandler defaultHandler;
	
	private final Logger logger; 
	
	public NativeScriptUncaughtExceptionHandler(Logger logger, Context context)
	{
		this.logger = logger;
		this.context = context;
		defaultHandler = Thread.getDefaultUncaughtExceptionHandler();
	}
	
	@Override
	public void uncaughtException(Thread thread, Throwable ex)
	{
		if(Platform.isInitialized()) {
			try {
				Platform.passUncaughtExceptionToJsNative(ex, ErrorReport.getErrorMessage(ex));
			} catch (Throwable t) {
				t.printStackTrace();
			}
		}
		
		String errorMessage = ErrorReport.getErrorMessage(ex);
		
		if (logger.isEnabled()) logger.write("Uncaught Exception Message=" + errorMessage);

		if(!ErrorReport.startActivity(context, errorMessage) && defaultHandler != null)
		{
			defaultHandler.uncaughtException(thread, ex);	
		}
	}
}

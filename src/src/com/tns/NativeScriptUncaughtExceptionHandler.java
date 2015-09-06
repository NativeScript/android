package com.tns;

import java.lang.Thread.UncaughtExceptionHandler;

import android.content.Context;
import android.util.Log;

public class NativeScriptUncaughtExceptionHandler implements UncaughtExceptionHandler 
{
	private final Context context;
	
	private final UncaughtExceptionHandler defaultHandler;
	
	public NativeScriptUncaughtExceptionHandler(Context context)
	{
		this.context = context;
		defaultHandler = Thread.getDefaultUncaughtExceptionHandler();
	}
	
	@Override
	public void uncaughtException(Thread thread, Throwable ex)
	{
		String errorMessage = ErrorReport.getErrorMessage(ex);
		
		if (Platform.IsLogEnabled) Log.e(Platform.DEFAULT_LOG_TAG, "Uncaught Exception Message=" + errorMessage);

		if(!ErrorReport.startActivity(context, errorMessage) && defaultHandler != null)
		{
			defaultHandler.uncaughtException(thread, ex);	
		}
	}
}

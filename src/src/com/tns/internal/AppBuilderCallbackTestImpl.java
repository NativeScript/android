package com.tns.internal;

import com.tns.Platform;

import android.util.Log;
import android.content.res.Configuration;

public class AppBuilderCallbackTestImpl implements AppBuilderCallback
{
	public AppBuilderCallbackTestImpl()
	{
	}

	@Override
	public void onConfigurationChanged(android.content.Context context, Configuration newConfig)
	{
		Log.d(logTag, "::onConfigurationChanged");
	}

	@Override
	public void onCreate(android.content.Context context)
	{
		Log.d(logTag, "::onCreate");
	}

	@Override
	public void onLowMemory(android.content.Context context)
	{
		Log.d(logTag, "::onLowMemory");
	}

	@Override
	public void onTerminate(android.content.Context context)
	{
		Log.d(logTag, "::onTerminate");
	}

	@Override
	public void onTrimMemory(android.content.Context context, int level)
	{
		Log.d(logTag, "::onTrimMemory");
	}
	
	@Override
	public Thread.UncaughtExceptionHandler getDefaultUncaughtExceptionHandler()
	{
		return null;
	}
	
	@Override
	public ExtractPolicy getExtractPolicy()
	{
		return null;
	}
	
	private final String logTag = Platform.DEFAULT_LOG_TAG;
}

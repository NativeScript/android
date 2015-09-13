package com.tns;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;

import com.tns.internal.Plugin;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.util.Log;

public final class Util
{
	private Util()
	{
	}
	
	public static String getDexThumb(Context context) throws NameNotFoundException
	{
		PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
		int code = packageInfo.versionCode;
		long updateTime = packageInfo.lastUpdateTime;
		return String.valueOf(updateTime) + "-" + String.valueOf(code);
	}

	public static boolean isDebuggableApp(Context context)
	{
		int flags;
		try
		{
			flags = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).applicationInfo.flags;
		}
		catch (NameNotFoundException e)
		{
			flags = 0;
			e.printStackTrace();
		}

		boolean isDebuggableApp = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
		return isDebuggableApp;
	}

    public static void initLogging(Logger logger, Context context)
    {
    	int flags;
		boolean verboseLogging = false;
		try
		{
			flags = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).applicationInfo.flags;
			verboseLogging = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
		}
		catch (NameNotFoundException e)
		{
			e.printStackTrace();
			return;
		}
		
		if (!verboseLogging)
		{
			return;
		}
		
    	String verboseLoggingProp = readSystemProperty("nativescript.verbose.logging");
    	if (verboseLoggingProp.equals("true") || verboseLoggingProp.equals("TRUE") ||
    		verboseLoggingProp.equals("yes") || verboseLoggingProp.equals("YES") ||
    		verboseLoggingProp.equals("enabled") || verboseLoggingProp.equals("ENABLED"))
    	{
    		logger.setEnabled(true);
    	}
    }
    
    
	private static String readSystemProperty(String name)
	{
		InputStreamReader in = null;
		BufferedReader reader = null;
		try
		{
			Process proc = Runtime.getRuntime().exec(new String[] { "/system/bin/getprop", name });
			in = new InputStreamReader(proc.getInputStream());
			reader = new BufferedReader(in);
			return reader.readLine();
		}
		catch (IOException e)
		{
			return null;
		}
		finally
		{
			silentClose(in);
			silentClose(reader);
		}
	}
	
	public static void silentClose(Closeable closeable)
	{
		if (closeable == null)
		{
			return;
		}
		try
		{
			closeable.close();
		}
		catch (IOException ignored)
		{
		}
	}
	
	static boolean runPlugin(Logger logger, Context context)
	{
		boolean success = false;
		String pluginClassName;
		try
		{
			ApplicationInfo ai = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
			pluginClassName = ai.metaData.getString("com.tns.internal.Plugin");
		}
		catch (Exception e)
		{
			pluginClassName = "org.nativescript.livesync.LiveSyncPlugin";
			if (logger.isEnabled()) e.printStackTrace();
		}
		try
		{
			Class<?> liveSyncPluginClass = Class.forName(pluginClassName);
			Plugin p = (Plugin)liveSyncPluginClass.newInstance();
			success = p.execute(context);
		}
		catch (Exception e)
		{
			if (logger.isEnabled()) e.printStackTrace();
		}
		return success;
	}
}

package com.tns;

import com.tns.internal.Plugin;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;

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
	
	static boolean runPlugin(Logger logger, Context context)
	{
		boolean success = false;
		String pluginClassName = "org.nativescript.livesync.LiveSyncPlugin";
		try
		{
			ApplicationInfo ai = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
			Bundle metadataBundle = ai.metaData;
			if (metadataBundle != null)
			{
				pluginClassName = metadataBundle.getString("com.tns.internal.Plugin");
			}
		}
		catch (Exception e)
		{
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

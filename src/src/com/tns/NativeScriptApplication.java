package com.tns;

import java.io.PrintWriter;
import java.io.StringWriter;

import android.app.Application;
import android.util.Log;

import com.tns.internal.AppBuilderCallback;
import com.tns.internal.ExtractPolicy;

public class NativeScriptApplication extends android.app.Application implements com.tns.NativeScriptHashCodeProvider
{
	public static class ActivityLifecycleCallbacks implements android.app.Application.ActivityLifecycleCallbacks, com.tns.NativeScriptHashCodeProvider
	{
		public void onActivityCreated(android.app.Activity param_0, android.os.Bundle param_1)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onActivityCreated", params);
		}

		public void onActivityDestroyed(android.app.Activity param_0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityDestroyed", params);
		}

		public void onActivityPaused(android.app.Activity param_0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityPaused", params);
		}

		public void onActivityResumed(android.app.Activity param_0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityResumed", params);
		}

		public void onActivitySaveInstanceState(android.app.Activity param_0, android.os.Bundle param_1)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onActivitySaveInstanceState", params);
		}

		public void onActivityStarted(android.app.Activity param_0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityStarted", params);
		}

		public void onActivityStopped(android.app.Activity param_0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityStopped", params);
		}

		public boolean equals__super(java.lang.Object other)
		{
			return super.equals(other);
		}

		public int hashCode__super()
		{
			return super.hashCode();
		}

		public void setNativeScriptOverrides(java.lang.String[] overrides)
		{
		}
	}

	public NativeScriptApplication()
	{
		super();
		if (__ctorOverridden)
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "init", true, params);
		}
	}

	static
	{
		System.loadLibrary("NativeScript");
		if (BuildConfig.DEBUG)
		{
			android.os.Debug.waitForDebugger();
		}
	}

	protected void attachBaseContext(android.content.Context param_0)
	{
		if ((__ho0 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "attachBaseContext", params);
		}
		else
		{
			super.attachBaseContext(param_0);
		}

		com.tns.Platform.installSecondaryDexes(this);
	}

	public boolean bindService(android.content.Intent param_0, android.content.ServiceConnection param_1, int param_2)
	{
		if ((__ho0 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean) com.tns.Platform.callJSMethod(this, "bindService", params);
		}
		else
		{
			return super.bindService(param_0, param_1, param_2);
		}
	}

	public int checkCallingOrSelfPermission(java.lang.String param_0)
	{
		if ((__ho0 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Integer) com.tns.Platform.callJSMethod(this, "checkCallingOrSelfPermission", params);
		}
		else
		{
			return super.checkCallingOrSelfPermission(param_0);
		}
	}

	public int checkCallingOrSelfUriPermission(android.net.Uri param_0, int param_1)
	{
		if ((__ho0 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Integer) com.tns.Platform.callJSMethod(this, "checkCallingOrSelfUriPermission", params);
		}
		else
		{
			return super.checkCallingOrSelfUriPermission(param_0, param_1);
		}
	}

	public int checkCallingPermission(java.lang.String param_0)
	{
		if ((__ho0 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Integer) com.tns.Platform.callJSMethod(this, "checkCallingPermission", params);
		}
		else
		{
			return super.checkCallingPermission(param_0);
		}
	}

	public int checkCallingUriPermission(android.net.Uri param_0, int param_1)
	{
		if ((__ho0 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Integer) com.tns.Platform.callJSMethod(this, "checkCallingUriPermission", params);
		}
		else
		{
			return super.checkCallingUriPermission(param_0, param_1);
		}
	}

	public int checkPermission(java.lang.String param_0, int param_1, int param_2)
	{
		if ((__ho0 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Integer) com.tns.Platform.callJSMethod(this, "checkPermission", params);
		}
		else
		{
			return super.checkPermission(param_0, param_1, param_2);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3)
	{
		if ((__ho0 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (Integer) com.tns.Platform.callJSMethod(this, "checkUriPermission", params);
		}
		else
		{
			return super.checkUriPermission(param_0, param_1, param_2, param_3);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5)
	{
		if ((__ho0 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			return (Integer) com.tns.Platform.callJSMethod(this, "checkUriPermission", params);
		}
		else
		{
			return super.checkUriPermission(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void clearWallpaper() throws java.io.IOException
	{
		if ((__ho1 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "clearWallpaper", params);
		}
		else
		{
			super.clearWallpaper();
		}
	}

	protected java.lang.Object clone() throws java.lang.CloneNotSupportedException
	{
		if ((__ho1 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.Object) com.tns.Platform.callJSMethod(this, "clone", params);
		}
		else
		{
			return super.clone();
		}
	}

	public android.content.Context createConfigurationContext(android.content.res.Configuration param_0)
	{
		if ((__ho1 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.Context) com.tns.Platform.callJSMethod(this, "createConfigurationContext", params);
		}
		else
		{
			return super.createConfigurationContext(param_0);
		}
	}

	public android.content.Context createDisplayContext(android.view.Display param_0)
	{
		if ((__ho1 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.Context) com.tns.Platform.callJSMethod(this, "createDisplayContext", params);
		}
		else
		{
			return super.createDisplayContext(param_0);
		}
	}

	public android.content.Context createPackageContext(java.lang.String param_0, int param_1) throws android.content.pm.PackageManager.NameNotFoundException
	{
		if ((__ho1 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.Context) com.tns.Platform.callJSMethod(this, "createPackageContext", params);
		}
		else
		{
			return super.createPackageContext(param_0, param_1);
		}
	}

	public java.lang.String[] databaseList()
	{
		if ((__ho1 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.String[]) com.tns.Platform.callJSMethod(this, "databaseList", params);
		}
		else
		{
			return super.databaseList();
		}
	}

	public boolean deleteDatabase(java.lang.String param_0)
	{
		if ((__ho1 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean) com.tns.Platform.callJSMethod(this, "deleteDatabase", params);
		}
		else
		{
			return super.deleteDatabase(param_0);
		}
	}

	public boolean deleteFile(java.lang.String param_0)
	{
		if ((__ho1 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean) com.tns.Platform.callJSMethod(this, "deleteFile", params);
		}
		else
		{
			return super.deleteFile(param_0);
		}
	}

	public void enforceCallingOrSelfPermission(java.lang.String param_0, java.lang.String param_1)
	{
		if ((__ho2 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfPermission", params);
		}
		else
		{
			super.enforceCallingOrSelfPermission(param_0, param_1);
		}
	}

	public void enforceCallingOrSelfUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2)
	{
		if ((__ho2 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfUriPermission", params);
		}
		else
		{
			super.enforceCallingOrSelfUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforceCallingPermission(java.lang.String param_0, java.lang.String param_1)
	{
		if ((__ho2 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingPermission", params);
		}
		else
		{
			super.enforceCallingPermission(param_0, param_1);
		}
	}

	public void enforceCallingUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2)
	{
		if ((__ho2 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingUriPermission", params);
		}
		else
		{
			super.enforceCallingUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforcePermission(java.lang.String param_0, int param_1, int param_2, java.lang.String param_3)
	{
		if ((__ho2 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			com.tns.Platform.callJSMethod(this, "enforcePermission", params);
		}
		else
		{
			super.enforcePermission(param_0, param_1, param_2, param_3);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3, java.lang.String param_4)
	{
		if ((__ho2 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[5];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", params);
		}
		else
		{
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5, java.lang.String param_6)
	{
		if ((__ho2 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", params);
		}
		else
		{
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public boolean equals(java.lang.Object param_0)
	{
		if ((__ho2 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean) com.tns.Platform.callJSMethod(this, "equals", params);
		}
		else
		{
			return super.equals(param_0);
		}
	}

	public java.lang.String[] fileList()
	{
		if ((__ho2 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.String[]) com.tns.Platform.callJSMethod(this, "fileList", params);
		}
		else
		{
			return super.fileList();
		}
	}

	protected void finalize() throws java.lang.Throwable
	{
		if ((__ho3 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "finalize", params);
		}
		else
		{
			super.finalize();
		}
	}

	public android.content.Context getApplicationContext()
	{
		if ((__ho3 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.Context) com.tns.Platform.callJSMethod(this, "getApplicationContext", params);
		}
		else
		{
			return super.getApplicationContext();
		}
	}

	public android.content.pm.ApplicationInfo getApplicationInfo()
	{
		if ((__ho3 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.pm.ApplicationInfo) com.tns.Platform.callJSMethod(this, "getApplicationInfo", params);
		}
		else
		{
			return super.getApplicationInfo();
		}
	}

	public android.content.res.AssetManager getAssets()
	{
		if ((__ho3 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.res.AssetManager) com.tns.Platform.callJSMethod(this, "getAssets", params);
		}
		else
		{
			return super.getAssets();
		}
	}

	public android.content.Context getBaseContext()
	{
		if ((__ho3 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.Context) com.tns.Platform.callJSMethod(this, "getBaseContext", params);
		}
		else
		{
			return super.getBaseContext();
		}
	}

	public java.io.File getCacheDir()
	{
		if ((__ho3 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getCacheDir", params);
		}
		else
		{
			return super.getCacheDir();
		}
	}

	public java.lang.ClassLoader getClassLoader()
	{
		if ((__ho3 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.ClassLoader) com.tns.Platform.callJSMethod(this, "getClassLoader", params);
		}
		else
		{
			return super.getClassLoader();
		}
	}

	public android.content.ContentResolver getContentResolver()
	{
		if ((__ho3 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.ContentResolver) com.tns.Platform.callJSMethod(this, "getContentResolver", params);
		}
		else
		{
			return super.getContentResolver();
		}
	}

	public java.io.File getDatabasePath(java.lang.String param_0)
	{
		if ((__ho4 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getDatabasePath", params);
		}
		else
		{
			return super.getDatabasePath(param_0);
		}
	}

	public java.io.File getDir(java.lang.String param_0, int param_1)
	{
		if ((__ho4 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getDir", params);
		}
		else
		{
			return super.getDir(param_0, param_1);
		}
	}

	public java.io.File getExternalCacheDir()
	{
		if ((__ho4 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getExternalCacheDir", params);
		}
		else
		{
			return super.getExternalCacheDir();
		}
	}

	public java.io.File getExternalFilesDir(java.lang.String param_0)
	{
		if ((__ho4 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getExternalFilesDir", params);
		}
		else
		{
			return super.getExternalFilesDir(param_0);
		}
	}

	public java.io.File getFileStreamPath(java.lang.String param_0)
	{
		if ((__ho4 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getFileStreamPath", params);
		}
		else
		{
			return super.getFileStreamPath(param_0);
		}
	}

	public java.io.File getFilesDir()
	{
		if ((__ho4 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getFilesDir", params);
		}
		else
		{
			return super.getFilesDir();
		}
	}

	public android.os.Looper getMainLooper()
	{
		if ((__ho4 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.os.Looper) com.tns.Platform.callJSMethod(this, "getMainLooper", params);
		}
		else
		{
			return super.getMainLooper();
		}
	}

	public java.io.File getObbDir()
	{
		if ((__ho4 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.io.File) com.tns.Platform.callJSMethod(this, "getObbDir", params);
		}
		else
		{
			return super.getObbDir();
		}
	}

	public java.lang.String getPackageCodePath()
	{
		if ((__ho5 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.String) com.tns.Platform.callJSMethod(this, "getPackageCodePath", params);
		}
		else
		{
			return super.getPackageCodePath();
		}
	}

	public android.content.pm.PackageManager getPackageManager()
	{
		if ((__ho5 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.pm.PackageManager) com.tns.Platform.callJSMethod(this, "getPackageManager", params);
		}
		else
		{
			return super.getPackageManager();
		}
	}

	public java.lang.String getPackageName()
	{
		if ((__ho5 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.String) com.tns.Platform.callJSMethod(this, "getPackageName", params);
		}
		else
		{
			return super.getPackageName();
		}
	}

	public java.lang.String getPackageResourcePath()
	{
		if ((__ho5 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.String) com.tns.Platform.callJSMethod(this, "getPackageResourcePath", params);
		}
		else
		{
			return super.getPackageResourcePath();
		}
	}

	public android.content.res.Resources getResources()
	{
		if ((__ho5 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.res.Resources) com.tns.Platform.callJSMethod(this, "getResources", params);
		}
		else
		{
			return super.getResources();
		}
	}

	public android.content.SharedPreferences getSharedPreferences(java.lang.String param_0, int param_1)
	{
		if ((__ho5 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.SharedPreferences) com.tns.Platform.callJSMethod(this, "getSharedPreferences", params);
		}
		else
		{
			return super.getSharedPreferences(param_0, param_1);
		}
	}

	public java.lang.Object getSystemService(java.lang.String param_0)
	{
		if ((__ho5 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.lang.Object) com.tns.Platform.callJSMethod(this, "getSystemService", params);
		}
		else
		{
			return super.getSystemService(param_0);
		}
	}

	public android.content.res.Resources.Theme getTheme()
	{
		if ((__ho5 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.content.res.Resources.Theme) com.tns.Platform.callJSMethod(this, "getTheme", params);
		}
		else
		{
			return super.getTheme();
		}
	}

	public android.graphics.drawable.Drawable getWallpaper()
	{
		if ((__ho6 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.graphics.drawable.Drawable) com.tns.Platform.callJSMethod(this, "getWallpaper", params);
		}
		else
		{
			return super.getWallpaper();
		}
	}

	public int getWallpaperDesiredMinimumHeight()
	{
		if ((__ho6 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = null;
			return (Integer) com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumHeight", params);
		}
		else
		{
			return super.getWallpaperDesiredMinimumHeight();
		}
	}

	public int getWallpaperDesiredMinimumWidth()
	{
		if ((__ho6 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = null;
			return (Integer) com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumWidth", params);
		}
		else
		{
			return super.getWallpaperDesiredMinimumWidth();
		}
	}

	public void grantUriPermission(java.lang.String param_0, android.net.Uri param_1, int param_2)
	{
		if ((__ho6 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "grantUriPermission", params);
		}
		else
		{
			super.grantUriPermission(param_0, param_1, param_2);
		}
	}

	public int hashCode()
	{
		if ((__ho6 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = null;
			return (Integer) com.tns.Platform.callJSMethod(this, "hashCode", params);
		}
		else
		{
			return super.hashCode();
		}
	}

	public boolean isRestricted()
	{
		if ((__ho6 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = null;
			return (Boolean) com.tns.Platform.callJSMethod(this, "isRestricted", params);
		}
		else
		{
			return super.isRestricted();
		}
	}

	public void onConfigurationChanged(android.content.res.Configuration param_0)
	{
		if (appBuilderCallbackImpl != null)
		{
			appBuilderCallbackImpl.onConfigurationChanged(this, param_0);
		}

		if ((__ho6 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onConfigurationChanged", params);
		}
		else
		{
			super.onConfigurationChanged(param_0);
		}
	}

	public void onCreateInternal()
	{
		if ((__ho6 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onCreate", params);
		}
		else
		{
			super.onCreate();
		}
	}

	private static Application appInstance;

	public static Application getInstance()
	{
		return appInstance;
	}

	public void onCreate()
	{
		appInstance = this;
		prepareAppBuilderCallbackImpl();

		if (appBuilderCallbackImpl != null)
		{
			appBuilderCallbackImpl.onCreate(this);
		}

		Platform.init(this);
		Platform.run(Platform.DefaultApplicationModuleName);

		onCreateInternal();
	}

	private void prepareAppBuilderCallbackImpl()
	{
		Class<?> appBuilderCallbackClass = null;

		String packageName = super.getPackageName();

		String className = packageName.equals("com.tns") ? "com.tns.internal.AppBuilderCallbackTestImpl" : "com.tns.internal.AppBuilderCallbackImpl";

		try
		{
			appBuilderCallbackClass = Class.forName(className);
		}
		catch (ClassNotFoundException e)
		{
			appBuilderCallbackClass = null;
			Log.d(logTag, "prepareAppBuilderCallbackImpl error: " + e.getMessage());
		}

		if (appBuilderCallbackClass != null)
		{
			try
			{
				appBuilderCallbackImpl = (AppBuilderCallback) appBuilderCallbackClass.newInstance();
			}
			catch (InstantiationException e)
			{
				appBuilderCallbackImpl = null;
				Log.d(logTag, "prepareAppBuilderCallbackImpl error: " + e.getMessage());
			}
			catch (IllegalAccessException e)
			{
				appBuilderCallbackImpl = null;
				Log.d(logTag, "prepareAppBuilderCallbackImpl error: " + e.getMessage());
			}
		}

		Thread.UncaughtExceptionHandler exHandler = (appBuilderCallbackImpl != null) ? appBuilderCallbackImpl.getDefaultUncaughtExceptionHandler() : null;

		Platform.setDefaultUncaughtExceptionHandler(exHandler);

		ExtractPolicy policy = (appBuilderCallbackImpl != null) ? appBuilderCallbackImpl.getExtractPolicy() : null;

		boolean shouldEnableDebugging = (appBuilderCallbackImpl != null) ? appBuilderCallbackImpl.shouldEnableDebugging(this) : JsDebugger.shouldEnableDebugging(this);

		if (shouldEnableDebugging)
		{
			JsDebugger.registerEnableDisableDebuggerReceiver(this);
			JsDebugger.registerGetDebuggerPortReceiver(this);
		}

		Platform.setExtractPolicy(policy);
	}

	public void onLowMemory()
	{
		if (appBuilderCallbackImpl != null)
		{
			appBuilderCallbackImpl.onLowMemory(this);
		}

		if ((__ho7 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onLowMemory", params);
		}
		else
		{
			super.onLowMemory();
		}
	}

	public void onTerminate()
	{
		if (appBuilderCallbackImpl != null)
		{
			appBuilderCallbackImpl.onTerminate(this);
		}

		if ((__ho7 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onTerminate", params);
		}
		else
		{
			super.onTerminate();
		}
	}

	public void onTrimMemory(int param_0)
	{
		if (appBuilderCallbackImpl != null)
		{
			appBuilderCallbackImpl.onTrimMemory(this, param_0);
		}

		if ((__ho7 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onTrimMemory", params);
		}
		else
		{
			super.onTrimMemory(param_0);
		}
	}

	public java.io.FileInputStream openFileInput(java.lang.String param_0) throws java.io.FileNotFoundException
	{
		if ((__ho7 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.FileInputStream) com.tns.Platform.callJSMethod(this, "openFileInput", params);
		}
		else
		{
			return super.openFileInput(param_0);
		}
	}

	public java.io.FileOutputStream openFileOutput(java.lang.String param_0, int param_1) throws java.io.FileNotFoundException
	{
		if ((__ho7 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (java.io.FileOutputStream) com.tns.Platform.callJSMethod(this, "openFileOutput", params);
		}
		else
		{
			return super.openFileOutput(param_0, param_1);
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2, android.database.DatabaseErrorHandler param_3)
	{
		if ((__ho7 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (android.database.sqlite.SQLiteDatabase) com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", params);
		}
		else
		{
			return super.openOrCreateDatabase(param_0, param_1, param_2, param_3);
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2)
	{
		if ((__ho7 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (android.database.sqlite.SQLiteDatabase) com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", params);
		}
		else
		{
			return super.openOrCreateDatabase(param_0, param_1, param_2);
		}
	}

	public android.graphics.drawable.Drawable peekWallpaper()
	{
		if ((__ho7 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = null;
			return (android.graphics.drawable.Drawable) com.tns.Platform.callJSMethod(this, "peekWallpaper", params);
		}
		else
		{
			return super.peekWallpaper();
		}
	}

	public void registerActivityLifecycleCallbacks(android.app.Application.ActivityLifecycleCallbacks param_0)
	{
		if ((__ho7 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerActivityLifecycleCallbacks", params);
		}
		else
		{
			super.registerActivityLifecycleCallbacks(param_0);
		}
	}

	public void registerComponentCallbacks(android.content.ComponentCallbacks param_0)
	{
		if ((__ho8 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerComponentCallbacks", params);
		}
		else
		{
			super.registerComponentCallbacks(param_0);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1, java.lang.String param_2, android.os.Handler param_3)
	{
		if ((__ho8 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (android.content.Intent) com.tns.Platform.callJSMethod(this, "registerReceiver", params);
		}
		else
		{
			return super.registerReceiver(param_0, param_1, param_2, param_3);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1)
	{
		if ((__ho8 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.Intent) com.tns.Platform.callJSMethod(this, "registerReceiver", params);
		}
		else
		{
			return super.registerReceiver(param_0, param_1);
		}
	}

	public void removeStickyBroadcast(android.content.Intent param_0)
	{
		if ((__ho8 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcast", params);
		}
		else
		{
			super.removeStickyBroadcast(param_0);
		}
	}

	public void removeStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1)
	{
		if ((__ho8 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcastAsUser", params);
		}
		else
		{
			super.removeStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void revokeUriPermission(android.net.Uri param_0, int param_1)
	{
		if ((__ho8 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "revokeUriPermission", params);
		}
		else
		{
			super.revokeUriPermission(param_0, param_1);
		}
	}

	public void sendBroadcast(android.content.Intent param_0, java.lang.String param_1)
	{
		if ((__ho8 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", params);
		}
		else
		{
			super.sendBroadcast(param_0, param_1);
		}
	}

	public void sendBroadcast(android.content.Intent param_0)
	{
		if ((__ho8 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", params);
		}
		else
		{
			super.sendBroadcast(param_0);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1)
	{
		if ((__ho8 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", params);
		}
		else
		{
			super.sendBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2)
	{
		if ((__ho8 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", params);
		}
		else
		{
			super.sendBroadcastAsUser(param_0, param_1, param_2);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1)
	{
		if ((__ho8 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", params);
		}
		else
		{
			super.sendOrderedBroadcast(param_0, param_1);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6)
	{
		if ((__ho8 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", params);
		}
		else
		{
			super.sendOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void sendOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2, android.content.BroadcastReceiver param_3, android.os.Handler param_4, int param_5, java.lang.String param_6, android.os.Bundle param_7)
	{
		if ((__ho9 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[8];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			params[7] = param_7;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcastAsUser", params);
		}
		else
		{
			super.sendOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7);
		}
	}

	public void sendStickyBroadcast(android.content.Intent param_0)
	{
		if ((__ho9 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcast", params);
		}
		else
		{
			super.sendStickyBroadcast(param_0);
		}
	}

	public void sendStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1)
	{
		if ((__ho9 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcastAsUser", params);
		}
		else
		{
			super.sendStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendStickyOrderedBroadcast(android.content.Intent param_0, android.content.BroadcastReceiver param_1, android.os.Handler param_2, int param_3, java.lang.String param_4, android.os.Bundle param_5)
	{
		if ((__ho9 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcast", params);
		}
		else
		{
			super.sendStickyOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void sendStickyOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6)
	{
		if ((__ho9 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcastAsUser", params);
		}
		else
		{
			super.sendStickyOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void setTheme(int param_0)
	{
		if ((__ho9 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTheme", params);
		}
		else
		{
			super.setTheme(param_0);
		}
	}

	public void setWallpaper(android.graphics.Bitmap param_0) throws java.io.IOException
	{
		if ((__ho9 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", params);
		}
		else
		{
			super.setWallpaper(param_0);
		}
	}

	public void setWallpaper(java.io.InputStream param_0) throws java.io.IOException
	{
		if ((__ho9 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", params);
		}
		else
		{
			super.setWallpaper(param_0);
		}
	}

	public void startActivities(android.content.Intent[] param_0, android.os.Bundle param_1)
	{
		if ((__ho9 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivities", params);
		}
		else
		{
			super.startActivities(param_0, param_1);
		}
	}

	public void startActivities(android.content.Intent[] param_0)
	{
		if ((__ho9 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivities", params);
		}
		else
		{
			super.startActivities(param_0);
		}
	}

	public void startActivity(android.content.Intent param_0)
	{
		if ((__ho10 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivity", params);
		}
		else
		{
			super.startActivity(param_0);
		}
	}

	public void startActivity(android.content.Intent param_0, android.os.Bundle param_1)
	{
		if ((__ho10 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivity", params);
		}
		else
		{
			super.startActivity(param_0, param_1);
		}
	}

	public boolean startInstrumentation(android.content.ComponentName param_0, java.lang.String param_1, android.os.Bundle param_2)
	{
		if ((__ho10 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean) com.tns.Platform.callJSMethod(this, "startInstrumentation", params);
		}
		else
		{
			return super.startInstrumentation(param_0, param_1, param_2);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4, android.os.Bundle param_5) throws android.content.IntentSender.SendIntentException
	{
		if ((__ho10 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			com.tns.Platform.callJSMethod(this, "startIntentSender", params);
		}
		else
		{
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4) throws android.content.IntentSender.SendIntentException
	{
		if ((__ho10 & (1 << 2)) > 0)
		{
			java.lang.Object[] params = new Object[5];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			com.tns.Platform.callJSMethod(this, "startIntentSender", params);
		}
		else
		{
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public android.content.ComponentName startService(android.content.Intent param_0)
	{
		if ((__ho10 & (1 << 3)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.ComponentName) com.tns.Platform.callJSMethod(this, "startService", params);
		}
		else
		{
			return super.startService(param_0);
		}
	}

	public boolean stopService(android.content.Intent param_0)
	{
		if ((__ho10 & (1 << 4)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean) com.tns.Platform.callJSMethod(this, "stopService", params);
		}
		else
		{
			return super.stopService(param_0);
		}
	}

	public java.lang.String toString()
	{
		if ((__ho10 & (1 << 5)) > 0)
		{
			java.lang.Object[] params = null;
			return (java.lang.String) com.tns.Platform.callJSMethod(this, "toString", params);
		}
		else
		{
			return super.toString();
		}
	}

	public void unbindService(android.content.ServiceConnection param_0)
	{
		if ((__ho10 & (1 << 6)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unbindService", params);
		}
		else
		{
			super.unbindService(param_0);
		}
	}

	public void unregisterActivityLifecycleCallbacks(android.app.Application.ActivityLifecycleCallbacks param_0)
	{
		if ((__ho10 & (1 << 7)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterActivityLifecycleCallbacks", params);
		}
		else
		{
			super.unregisterActivityLifecycleCallbacks(param_0);
		}
	}

	public void unregisterComponentCallbacks(android.content.ComponentCallbacks param_0)
	{
		if ((__ho11 & (1 << 0)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterComponentCallbacks", params);
		}
		else
		{
			super.unregisterComponentCallbacks(param_0);
		}
	}

	public void unregisterReceiver(android.content.BroadcastReceiver param_0)
	{
		if ((__ho11 & (1 << 1)) > 0)
		{
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterReceiver", params);
		}
		else
		{
			super.unregisterReceiver(param_0);
		}
	}

	public boolean equals__super(java.lang.Object other)
	{
		return super.equals(other);
	}

	public int hashCode__super()
	{
		return super.hashCode();
	}

	public void setNativeScriptOverrides(java.lang.String[] overrides)
	{
		for (java.lang.String name : overrides)
		{
			if (name.equals("init"))
			{
				__ctorOverridden = true;
			}
			if (name.equals("attachBaseContext"))
			{
				__ho0 |= (1 << 0);
			}
			else if (name.equals("bindService"))
			{
				__ho0 |= (1 << 1);
			}
			else if (name.equals("checkCallingOrSelfPermission"))
			{
				__ho0 |= (1 << 2);
			}
			else if (name.equals("checkCallingOrSelfUriPermission"))
			{
				__ho0 |= (1 << 3);
			}
			else if (name.equals("checkCallingPermission"))
			{
				__ho0 |= (1 << 4);
			}
			else if (name.equals("checkCallingUriPermission"))
			{
				__ho0 |= (1 << 5);
			}
			else if (name.equals("checkPermission"))
			{
				__ho0 |= (1 << 6);
			}
			else if (name.equals("checkUriPermission"))
			{
				__ho0 |= (1 << 7);
			}
			else if (name.equals("clearWallpaper"))
			{
				__ho1 |= (1 << 0);
			}
			else if (name.equals("clone"))
			{
				__ho1 |= (1 << 1);
			}
			else if (name.equals("createConfigurationContext"))
			{
				__ho1 |= (1 << 2);
			}
			else if (name.equals("createDisplayContext"))
			{
				__ho1 |= (1 << 3);
			}
			else if (name.equals("createPackageContext"))
			{
				__ho1 |= (1 << 4);
			}
			else if (name.equals("databaseList"))
			{
				__ho1 |= (1 << 5);
			}
			else if (name.equals("deleteDatabase"))
			{
				__ho1 |= (1 << 6);
			}
			else if (name.equals("deleteFile"))
			{
				__ho1 |= (1 << 7);
			}
			else if (name.equals("enforceCallingOrSelfPermission"))
			{
				__ho2 |= (1 << 0);
			}
			else if (name.equals("enforceCallingOrSelfUriPermission"))
			{
				__ho2 |= (1 << 1);
			}
			else if (name.equals("enforceCallingPermission"))
			{
				__ho2 |= (1 << 2);
			}
			else if (name.equals("enforceCallingUriPermission"))
			{
				__ho2 |= (1 << 3);
			}
			else if (name.equals("enforcePermission"))
			{
				__ho2 |= (1 << 4);
			}
			else if (name.equals("enforceUriPermission"))
			{
				__ho2 |= (1 << 5);
			}
			else if (name.equals("equals"))
			{
				__ho2 |= (1 << 6);
			}
			else if (name.equals("fileList"))
			{
				__ho2 |= (1 << 7);
			}
			else if (name.equals("finalize"))
			{
				__ho3 |= (1 << 0);
			}
			else if (name.equals("getApplicationContext"))
			{
				__ho3 |= (1 << 1);
			}
			else if (name.equals("getApplicationInfo"))
			{
				__ho3 |= (1 << 2);
			}
			else if (name.equals("getAssets"))
			{
				__ho3 |= (1 << 3);
			}
			else if (name.equals("getBaseContext"))
			{
				__ho3 |= (1 << 4);
			}
			else if (name.equals("getCacheDir"))
			{
				__ho3 |= (1 << 5);
			}
			else if (name.equals("getClassLoader"))
			{
				__ho3 |= (1 << 6);
			}
			else if (name.equals("getContentResolver"))
			{
				__ho3 |= (1 << 7);
			}
			else if (name.equals("getDatabasePath"))
			{
				__ho4 |= (1 << 0);
			}
			else if (name.equals("getDir"))
			{
				__ho4 |= (1 << 1);
			}
			else if (name.equals("getExternalCacheDir"))
			{
				__ho4 |= (1 << 2);
			}
			else if (name.equals("getExternalFilesDir"))
			{
				__ho4 |= (1 << 3);
			}
			else if (name.equals("getFileStreamPath"))
			{
				__ho4 |= (1 << 4);
			}
			else if (name.equals("getFilesDir"))
			{
				__ho4 |= (1 << 5);
			}
			else if (name.equals("getMainLooper"))
			{
				__ho4 |= (1 << 6);
			}
			else if (name.equals("getObbDir"))
			{
				__ho4 |= (1 << 7);
			}
			else if (name.equals("getPackageCodePath"))
			{
				__ho5 |= (1 << 0);
			}
			else if (name.equals("getPackageManager"))
			{
				__ho5 |= (1 << 1);
			}
			else if (name.equals("getPackageName"))
			{
				__ho5 |= (1 << 2);
			}
			else if (name.equals("getPackageResourcePath"))
			{
				__ho5 |= (1 << 3);
			}
			else if (name.equals("getResources"))
			{
				__ho5 |= (1 << 4);
			}
			else if (name.equals("getSharedPreferences"))
			{
				__ho5 |= (1 << 5);
			}
			else if (name.equals("getSystemService"))
			{
				__ho5 |= (1 << 6);
			}
			else if (name.equals("getTheme"))
			{
				__ho5 |= (1 << 7);
			}
			else if (name.equals("getWallpaper"))
			{
				__ho6 |= (1 << 0);
			}
			else if (name.equals("getWallpaperDesiredMinimumHeight"))
			{
				__ho6 |= (1 << 1);
			}
			else if (name.equals("getWallpaperDesiredMinimumWidth"))
			{
				__ho6 |= (1 << 2);
			}
			else if (name.equals("grantUriPermission"))
			{
				__ho6 |= (1 << 3);
			}
			else if (name.equals("hashCode"))
			{
				__ho6 |= (1 << 4);
			}
			else if (name.equals("isRestricted"))
			{
				__ho6 |= (1 << 5);
			}
			else if (name.equals("onConfigurationChanged"))
			{
				__ho6 |= (1 << 6);
			}
			else if (name.equals("onCreate"))
			{
				__ho6 |= (1 << 7);
			}
			else if (name.equals("onLowMemory"))
			{
				__ho7 |= (1 << 0);
			}
			else if (name.equals("onTerminate"))
			{
				__ho7 |= (1 << 1);
			}
			else if (name.equals("onTrimMemory"))
			{
				__ho7 |= (1 << 2);
			}
			else if (name.equals("openFileInput"))
			{
				__ho7 |= (1 << 3);
			}
			else if (name.equals("openFileOutput"))
			{
				__ho7 |= (1 << 4);
			}
			else if (name.equals("openOrCreateDatabase"))
			{
				__ho7 |= (1 << 5);
			}
			else if (name.equals("peekWallpaper"))
			{
				__ho7 |= (1 << 6);
			}
			else if (name.equals("registerActivityLifecycleCallbacks"))
			{
				__ho7 |= (1 << 7);
			}
			else if (name.equals("registerComponentCallbacks"))
			{
				__ho8 |= (1 << 0);
			}
			else if (name.equals("registerReceiver"))
			{
				__ho8 |= (1 << 1);
			}
			else if (name.equals("removeStickyBroadcast"))
			{
				__ho8 |= (1 << 2);
			}
			else if (name.equals("removeStickyBroadcastAsUser"))
			{
				__ho8 |= (1 << 3);
			}
			else if (name.equals("revokeUriPermission"))
			{
				__ho8 |= (1 << 4);
			}
			else if (name.equals("sendBroadcast"))
			{
				__ho8 |= (1 << 5);
			}
			else if (name.equals("sendBroadcastAsUser"))
			{
				__ho8 |= (1 << 6);
			}
			else if (name.equals("sendOrderedBroadcast"))
			{
				__ho8 |= (1 << 7);
			}
			else if (name.equals("sendOrderedBroadcastAsUser"))
			{
				__ho9 |= (1 << 0);
			}
			else if (name.equals("sendStickyBroadcast"))
			{
				__ho9 |= (1 << 1);
			}
			else if (name.equals("sendStickyBroadcastAsUser"))
			{
				__ho9 |= (1 << 2);
			}
			else if (name.equals("sendStickyOrderedBroadcast"))
			{
				__ho9 |= (1 << 3);
			}
			else if (name.equals("sendStickyOrderedBroadcastAsUser"))
			{
				__ho9 |= (1 << 4);
			}
			else if (name.equals("setTheme"))
			{
				__ho9 |= (1 << 5);
			}
			else if (name.equals("setWallpaper"))
			{
				__ho9 |= (1 << 6);
			}
			else if (name.equals("startActivities"))
			{
				__ho9 |= (1 << 7);
			}
			else if (name.equals("startActivity"))
			{
				__ho10 |= (1 << 0);
			}
			else if (name.equals("startInstrumentation"))
			{
				__ho10 |= (1 << 1);
			}
			else if (name.equals("startIntentSender"))
			{
				__ho10 |= (1 << 2);
			}
			else if (name.equals("startService"))
			{
				__ho10 |= (1 << 3);
			}
			else if (name.equals("stopService"))
			{
				__ho10 |= (1 << 4);
			}
			else if (name.equals("toString"))
			{
				__ho10 |= (1 << 5);
			}
			else if (name.equals("unbindService"))
			{
				__ho10 |= (1 << 6);
			}
			else if (name.equals("unregisterActivityLifecycleCallbacks"))
			{
				__ho10 |= (1 << 7);
			}
			else if (name.equals("unregisterComponentCallbacks"))
			{
				__ho11 |= (1 << 0);
			}
			else if (name.equals("unregisterReceiver"))
			{
				__ho11 |= (1 << 1);
			}
		}
	}

	private boolean __initialized = true;
	private boolean __ctorOverridden;
	private byte __ho0;
	private byte __ho1;
	private byte __ho2;
	private byte __ho3;
	private byte __ho4;
	private byte __ho5;
	private byte __ho6;
	private byte __ho7;
	private byte __ho8;
	private byte __ho9;
	private byte __ho10;
	private byte __ho11;
	private byte __ho12;

	private AppBuilderCallback appBuilderCallbackImpl;

	private final String logTag = Platform.DEFAULT_LOG_TAG;
}

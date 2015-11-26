package com.tns;

import java.io.File;
import java.lang.Thread.UncaughtExceptionHandler;

import android.app.Application;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.tns.internal.AppBuilderCallback;
import com.tns.internal.DefaultExtractPolicy;
import com.tns.internal.ExtractPolicy;

public class NativeScriptApplication extends android.app.Application implements com.tns.NativeScriptHashCodeProvider {
	public static class ActivityLifecycleCallbacks implements android.app.Application.ActivityLifecycleCallbacks, com.tns.NativeScriptHashCodeProvider {

		public void onActivityCreated(android.app.Activity param_0, android.os.Bundle param_1) {
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onActivityCreated", void.class, params);
		}

		public void onActivityDestroyed(android.app.Activity param_0) {
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityDestroyed", void.class, params);
		}
		
		public void onActivityPaused(android.app.Activity param_0) {
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityPaused", void.class, params);
		}

		public void onActivityResumed(android.app.Activity param_0) {
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityResumed", void.class, params);
		}

		public void onActivitySaveInstanceState(android.app.Activity param_0, android.os.Bundle param_1) {
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onActivitySaveInstanceState", void.class, params);
		}

		public void onActivityStarted(android.app.Activity param_0) {
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityStarted", void.class, params);
		}

		public void onActivityStopped(android.app.Activity param_0) {
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActivityStopped", void.class, params);
		}

		public boolean equals__super(java.lang.Object other) {
			return super.equals(other);
		}
		public int hashCode__super() {
			return super.hashCode();
		}
		public void setNativeScriptOverrides(java.lang.String[] overrides) {
		}
	}
	public NativeScriptApplication() {
		super();
		if (__ctorOverridden) {
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "init", void.class, true, params);
		}
	}

	static {
		if (BuildConfig.DEBUG) {
			android.os.Debug.waitForDebugger();
		}
	}
	
	protected void attachBaseContext(android.content.Context param_0) {
		if ((__ho0 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "attachBaseContext", void.class, params);
		} else {
			super.attachBaseContext(param_0);
		}
	}

	public boolean bindService(android.content.Intent param_0, android.content.ServiceConnection param_1, int param_2) {
		if ((__ho0 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "bindService", boolean.class, params);
		} else {
			return super.bindService(param_0, param_1, param_2);
		}
	}

	public int checkCallingOrSelfPermission(java.lang.String param_0) {
		if ((__ho0 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingOrSelfPermission", int.class, params);
		} else {
			return super.checkCallingOrSelfPermission(param_0);
		}
	}

	public int checkCallingOrSelfUriPermission(android.net.Uri param_0, int param_1) {
		if ((__ho0 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingOrSelfUriPermission", int.class, params);
		} else {
			return super.checkCallingOrSelfUriPermission(param_0, param_1);
		}
	}

	public int checkCallingPermission(java.lang.String param_0) {
		if ((__ho0 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingPermission", int.class, params);
		} else {
			return super.checkCallingPermission(param_0);
		}
	}

	public int checkCallingUriPermission(android.net.Uri param_0, int param_1) {
		if ((__ho0 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingUriPermission", int.class, params);
		} else {
			return super.checkCallingUriPermission(param_0, param_1);
		}
	}

	public int checkPermission(java.lang.String param_0, int param_1, int param_2) {
		if ((__ho0 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkPermission", int.class, params);
		} else {
			return super.checkPermission(param_0, param_1, param_2);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5) {
		if ((__ho0 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkUriPermission", int.class, params);
		} else {
			return super.checkUriPermission(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3) {
		if ((__ho0 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkUriPermission", int.class, params);
		} else {
			return super.checkUriPermission(param_0, param_1, param_2, param_3);
		}
	}

	public void clearWallpaper() throws java.io.IOException {
		if ((__ho1 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "clearWallpaper", void.class, params);
		} else {
			super.clearWallpaper();
		}
	}

	protected java.lang.Object clone() throws java.lang.CloneNotSupportedException {
		if ((__ho1 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "clone", java.lang.Object.class, params);
		} else {
			return super.clone();
		}
	}

	public android.content.Context createConfigurationContext(android.content.res.Configuration param_0) {
		if ((__ho1 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createConfigurationContext", android.content.Context.class, params);
		} else {
			return super.createConfigurationContext(param_0);
		}
	}

	public android.content.Context createDisplayContext(android.view.Display param_0) {
		if ((__ho1 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createDisplayContext", android.content.Context.class, params);
		} else {
			return super.createDisplayContext(param_0);
		}
	}

	public android.content.Context createPackageContext(java.lang.String param_0, int param_1) throws android.content.pm.PackageManager.NameNotFoundException {
		if ((__ho1 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createPackageContext", android.content.Context.class, params);
		} else {
			return super.createPackageContext(param_0, param_1);
		}
	}

	public java.lang.String[] databaseList() {
		if ((__ho1 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String[])com.tns.Platform.callJSMethod(this, "databaseList", java.lang.String[].class, params);
		} else {
			return super.databaseList();
		}
	}

	public boolean deleteDatabase(java.lang.String param_0) {
		if ((__ho1 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "deleteDatabase", boolean.class, params);
		} else {
			return super.deleteDatabase(param_0);
		}
	}

	public boolean deleteFile(java.lang.String param_0) {
		if ((__ho1 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "deleteFile", boolean.class, params);
		} else {
			return super.deleteFile(param_0);
		}
	}

	public void enforceCallingOrSelfPermission(java.lang.String param_0, java.lang.String param_1) {
		if ((__ho2 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfPermission", void.class, params);
		} else {
			super.enforceCallingOrSelfPermission(param_0, param_1);
		}
	}

	public void enforceCallingOrSelfUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2) {
		if ((__ho2 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfUriPermission", void.class, params);
		} else {
			super.enforceCallingOrSelfUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforceCallingPermission(java.lang.String param_0, java.lang.String param_1) {
		if ((__ho2 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingPermission", void.class, params);
		} else {
			super.enforceCallingPermission(param_0, param_1);
		}
	}

	public void enforceCallingUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2) {
		if ((__ho2 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingUriPermission", void.class, params);
		} else {
			super.enforceCallingUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforcePermission(java.lang.String param_0, int param_1, int param_2, java.lang.String param_3) {
		if ((__ho2 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			com.tns.Platform.callJSMethod(this, "enforcePermission", void.class, params);
		} else {
			super.enforcePermission(param_0, param_1, param_2, param_3);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3, java.lang.String param_4) {
		if ((__ho2 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[5];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", void.class, params);
		} else {
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5, java.lang.String param_6) {
		if ((__ho2 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", void.class, params);
		} else {
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public boolean equals(java.lang.Object param_0) {
		if ((__ho2 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "equals", boolean.class, params);
		} else {
			return super.equals(param_0);
		}
	}

	public java.lang.String[] fileList() {
		if ((__ho2 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String[])com.tns.Platform.callJSMethod(this, "fileList", java.lang.String[].class, params);
		} else {
			return super.fileList();
		}
	}

	protected void finalize() throws java.lang.Throwable {
		if ((__ho3 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "finalize", void.class, params);
		} else {
			super.finalize();
		}
	}

	public android.content.Context getApplicationContext() {
		if ((__ho3 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "getApplicationContext", android.content.Context.class, params);
		} else {
			return super.getApplicationContext();
		}
	}

	public android.content.pm.ApplicationInfo getApplicationInfo() {
		if ((__ho3 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.pm.ApplicationInfo)com.tns.Platform.callJSMethod(this, "getApplicationInfo", android.content.pm.ApplicationInfo.class, params);
		} else {
			return super.getApplicationInfo();
		}
	}

	public android.content.res.AssetManager getAssets() {
		if ((__ho3 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.res.AssetManager)com.tns.Platform.callJSMethod(this, "getAssets", android.content.res.AssetManager.class, params);
		} else {
			return super.getAssets();
		}
	}

	public android.content.Context getBaseContext() {
		if ((__ho3 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "getBaseContext", android.content.Context.class, params);
		} else {
			return super.getBaseContext();
		}
	}

	public java.io.File getCacheDir() {
		if ((__ho3 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getCacheDir", java.io.File.class, params);
		} else {
			return super.getCacheDir();
		}
	}

	public java.lang.ClassLoader getClassLoader() {
		if ((__ho3 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.ClassLoader)com.tns.Platform.callJSMethod(this, "getClassLoader", java.lang.ClassLoader.class, params);
		} else {
			return super.getClassLoader();
		}
	}

	public android.content.ContentResolver getContentResolver() {
		if ((__ho3 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.ContentResolver)com.tns.Platform.callJSMethod(this, "getContentResolver", android.content.ContentResolver.class, params);
		} else {
			return super.getContentResolver();
		}
	}

	public java.io.File getDatabasePath(java.lang.String param_0) {
		if ((__ho4 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getDatabasePath", java.io.File.class, params);
		} else {
			return super.getDatabasePath(param_0);
		}
	}

	public java.io.File getDir(java.lang.String param_0, int param_1) {
		if ((__ho4 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getDir", java.io.File.class, params);
		} else {
			return super.getDir(param_0, param_1);
		}
	}

	public java.io.File getExternalCacheDir() {
		if ((__ho4 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getExternalCacheDir", java.io.File.class, params);
		} else {
			return super.getExternalCacheDir();
		}
	}

	public java.io.File getExternalFilesDir(java.lang.String param_0) {
		if ((__ho4 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getExternalFilesDir", java.io.File.class, params);
		} else {
			return super.getExternalFilesDir(param_0);
		}
	}

	public java.io.File getFileStreamPath(java.lang.String param_0) {
		if ((__ho4 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getFileStreamPath", java.io.File.class, params);
		} else {
			return super.getFileStreamPath(param_0);
		}
	}

	public java.io.File getFilesDir() {
		if ((__ho4 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getFilesDir", java.io.File.class, params);
		} else {
			return super.getFilesDir();
		}
	}

	public android.os.Looper getMainLooper() {
		if ((__ho4 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (android.os.Looper)com.tns.Platform.callJSMethod(this, "getMainLooper", android.os.Looper.class, params);
		} else {
			return super.getMainLooper();
		}
	}

	public java.io.File getObbDir() {
		if ((__ho4 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getObbDir", java.io.File.class, params);
		} else {
			return super.getObbDir();
		}
	}

	public java.lang.String getPackageCodePath() {
		if ((__ho5 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageCodePath", java.lang.String.class, params);
		} else {
			return super.getPackageCodePath();
		}
	}

	public android.content.pm.PackageManager getPackageManager() {
		if ((__ho5 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.pm.PackageManager)com.tns.Platform.callJSMethod(this, "getPackageManager", android.content.pm.PackageManager.class, params);
		} else {
			return super.getPackageManager();
		}
	}

	public java.lang.String getPackageName() {
		if ((__ho5 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageName", java.lang.String.class, params);
		} else {
			return super.getPackageName();
		}
	}

	public java.lang.String getPackageResourcePath() {
		if ((__ho5 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageResourcePath", java.lang.String.class, params);
		} else {
			return super.getPackageResourcePath();
		}
	}

	public android.content.res.Resources getResources() {
		if ((__ho5 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.res.Resources)com.tns.Platform.callJSMethod(this, "getResources", android.content.res.Resources.class, params);
		} else {
			return super.getResources();
		}
	}

	public android.content.SharedPreferences getSharedPreferences(java.lang.String param_0, int param_1) {
		if ((__ho5 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.SharedPreferences)com.tns.Platform.callJSMethod(this, "getSharedPreferences", android.content.SharedPreferences.class, params);
		} else {
			return super.getSharedPreferences(param_0, param_1);
		}
	}

	public java.lang.Object getSystemService(java.lang.String param_0) {
		if ((__ho5 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "getSystemService", java.lang.Object.class, params);
		} else {
			return super.getSystemService(param_0);
		}
	}

	public android.content.res.Resources.Theme getTheme() {
		if ((__ho5 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.res.Resources.Theme)com.tns.Platform.callJSMethod(this, "getTheme", android.content.res.Resources.Theme.class, params);
		} else {
			return super.getTheme();
		}
	}

	public android.graphics.drawable.Drawable getWallpaper() {
		if ((__ho6 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			return (android.graphics.drawable.Drawable)com.tns.Platform.callJSMethod(this, "getWallpaper", android.graphics.drawable.Drawable.class, params);
		} else {
			return super.getWallpaper();
		}
	}

	public int getWallpaperDesiredMinimumHeight() {
		if ((__ho6 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumHeight", int.class, params);
		} else {
			return super.getWallpaperDesiredMinimumHeight();
		}
	}

	public int getWallpaperDesiredMinimumWidth() {
		if ((__ho6 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumWidth", int.class, params);
		} else {
			return super.getWallpaperDesiredMinimumWidth();
		}
	}

	public void grantUriPermission(java.lang.String param_0, android.net.Uri param_1, int param_2) {
		if ((__ho6 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "grantUriPermission", void.class, params);
		} else {
			super.grantUriPermission(param_0, param_1, param_2);
		}
	}

	public int hashCode() {
		if ((__ho6 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "hashCode", int.class, params);
		} else {
			return super.hashCode();
		}
	}

	public boolean isRestricted() {
		if ((__ho6 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isRestricted", boolean.class, params);
		} else {
			return super.isRestricted();
		}
	}

	public void onConfigurationChanged(android.content.res.Configuration param_0) {
		if (appBuilderCallbackImpl != null)
		{
		        appBuilderCallbackImpl.onConfigurationChanged(this, param_0);
		}
		
		if ((__ho6 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onConfigurationChanged", void.class, params);
		} else {
			super.onConfigurationChanged(param_0);
		}
	}

	public void onCreateInternal() {
		if ((__ho6 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onCreate", void.class, params);
		} else {
			super.onCreate();
		}
	}
	
	private static Application appInstance;
	public static Application getInstance(){
		return appInstance;
	}
	
	// hasErrorIntent tells you if there was an event (with an uncaught exception) raised from ErrorReport
	private boolean hasErrorIntent()
	{
		boolean hasErrorIntent = false;
		
		try
		{
			 //empty file just to check if there was a raised uncaught error by ErrorReport
			File errFile = new File(this.getFilesDir(), ErrorReport.ERROR_FILE_NAME);
			
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
	
	public void onCreate() {
		
		System.loadLibrary("NativeScript");
		
		Logger logger = new LogcatLogger(BuildConfig.DEBUG, this);		
		
		boolean showErrorIntent = hasErrorIntent();
		if (!showErrorIntent)
		{
			appInstance = this;
			
			Thread.UncaughtExceptionHandler exHandler = new NativeScriptUncaughtExceptionHandler(logger, this);
			
			prepareAppBuilderCallbackImpl(logger, exHandler);
			
			Thread.setDefaultUncaughtExceptionHandler(exHandler);
			
			// TODO: refactor
			ExtractPolicy extractPolicy = (appBuilderCallbackImpl != null)
					? appBuilderCallbackImpl.getExtractPolicy()
					: new DefaultExtractPolicy(logger);
			boolean skipAssetExtraction = Util.runPlugin(logger, this);
			if (!skipAssetExtraction)
			{
				new AssetExtractor(null, logger).extractAssets(this, extractPolicy);
			}
			
			if (appBuilderCallbackImpl != null)
			{
				appBuilderCallbackImpl.onCreate(this);
			}
			
			if (NativeScriptSyncService.isSyncEnabled(this))
			{
                NativeScriptSyncService syncService = new NativeScriptSyncService(logger, this);
                
                syncService.sync();
                syncService.startServer();

                //preserve this instance as strong reference
                //do not preserve in NativeScriptApplication field inorder to make the code more portable 
                Platform.getOrCreateJavaObjectID(syncService);
			}
			else
			{
				if (logger.isEnabled())
				{
					logger.write("NativeScript LiveSync is not enabled.");
				}
			}


			String appName = this.getPackageName();
			File rootDir = new File(this.getApplicationInfo().dataDir);
			File appDir = this.getFilesDir();
			File debuggerSetupDir = Util.isDebuggableApp(this)
										? getExternalFilesDir(null)
										: null;
			ClassLoader classLoader = this.getClassLoader();
			File dexDir = new File(rootDir, "code_cache/secondary-dexes");
			String dexThumb = null;
			try
			{
				dexThumb = Util.getDexThumb(this);
			}
			catch (NameNotFoundException e)
			{
				if (logger.isEnabled()) logger.write("Error while getting current proxy thumb");
				e.printStackTrace();
			}
			ThreadScheduler workThreadScheduler = new WorkThreadScheduler(new Handler(Looper.getMainLooper()));
			// TODO: Refactor these 11 method parameters!!! E.g. create Settings abstract object and add default implementation object 
			Platform.init(this, workThreadScheduler, logger, appName, null, rootDir, appDir, debuggerSetupDir, classLoader, dexDir, dexThumb);
			Platform.runScript(new File(appDir, "internal/prepareExtend.js"));
			Platform.run();
	
			onCreateInternal();
		}
	}
	
	private void prepareAppBuilderCallbackImpl(Logger logger, UncaughtExceptionHandler exHandler)
	{
		Class<?> appBuilderCallbackClass = null;
		
		try
		{
			String className = "com.tns.internal.AppBuilderCallbackImpl";
			appBuilderCallbackClass = Class.forName(className);
		}
		catch (ClassNotFoundException e)
		{
			appBuilderCallbackClass = null;
			if (logger.isEnabled())
			{
				logger.write(logTag, "prepareAppBuilderCallbackImpl error: " + e.getMessage());
			}
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
				if (logger.isEnabled())
				{
					logger.write(logTag, "prepareAppBuilderCallbackImpl error: " + e.getMessage());
				}
			}
			catch (IllegalAccessException e)
			{
				appBuilderCallbackImpl = null;
				if (logger.isEnabled())
				{
					logger.write(logTag, "prepareAppBuilderCallbackImpl error: " + e.getMessage());
				}
			}
		}

		if(appBuilderCallbackImpl != null) {
			exHandler = appBuilderCallbackImpl.getDefaultUncaughtExceptionHandler();	
		}
		
		boolean shouldEnableDebugging = (appBuilderCallbackImpl != null)
				? appBuilderCallbackImpl.shouldEnableDebugging(this)
				: Util.isDebuggableApp(this);
				
		if (shouldEnableDebugging)
		{
			JsDebugger.registerEnableDisableDebuggerReceiver(this);
			JsDebugger.registerGetDebuggerPortReceiver(this);
		}
	}

	public void onLowMemory() {
		if (appBuilderCallbackImpl != null)
		{
		        appBuilderCallbackImpl.onLowMemory(this);
		}
		
		if ((__ho7 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onLowMemory", void.class, params);
		} else {
			super.onLowMemory();
		}
	}

	public void onTerminate() {
		if (appBuilderCallbackImpl != null)
		{
		        appBuilderCallbackImpl.onTerminate(this);
		}
		
		if ((__ho7 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onTerminate", void.class, params);
		} else {
			super.onTerminate();
		}
	}

	public void onTrimMemory(int param_0) {
		if (appBuilderCallbackImpl != null)
		{
		        appBuilderCallbackImpl.onTrimMemory(this, param_0);
		}
		
		if ((__ho7 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onTrimMemory", void.class, params);
		} else {
			super.onTrimMemory(param_0);
		}
	}

	public java.io.FileInputStream openFileInput(java.lang.String param_0) throws java.io.FileNotFoundException {
		if ((__ho7 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.FileInputStream)com.tns.Platform.callJSMethod(this, "openFileInput", java.io.FileInputStream.class, params);
		} else {
			return super.openFileInput(param_0);
		}
	}

	public java.io.FileOutputStream openFileOutput(java.lang.String param_0, int param_1) throws java.io.FileNotFoundException {
		if ((__ho7 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (java.io.FileOutputStream)com.tns.Platform.callJSMethod(this, "openFileOutput", java.io.FileOutputStream.class, params);
		} else {
			return super.openFileOutput(param_0, param_1);
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2) {
		if ((__ho7 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (android.database.sqlite.SQLiteDatabase)com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", android.database.sqlite.SQLiteDatabase.class, params);
		} else {
			return super.openOrCreateDatabase(param_0, param_1, param_2);
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2, android.database.DatabaseErrorHandler param_3) {
		if ((__ho7 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (android.database.sqlite.SQLiteDatabase)com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", android.database.sqlite.SQLiteDatabase.class, params);
		} else {
			return super.openOrCreateDatabase(param_0, param_1, param_2, param_3);
		}
	}

	public android.graphics.drawable.Drawable peekWallpaper() {
		if ((__ho7 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (android.graphics.drawable.Drawable)com.tns.Platform.callJSMethod(this, "peekWallpaper", android.graphics.drawable.Drawable.class, params);
		} else {
			return super.peekWallpaper();
		}
	}

	public void registerActivityLifecycleCallbacks(android.app.Application.ActivityLifecycleCallbacks param_0) {
		if ((__ho7 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerActivityLifecycleCallbacks", void.class, params);
		} else {
			super.registerActivityLifecycleCallbacks(param_0);
		}
	}

	public void registerComponentCallbacks(android.content.ComponentCallbacks param_0) {
		if ((__ho8 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerComponentCallbacks", void.class, params);
		} else {
			super.registerComponentCallbacks(param_0);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1, java.lang.String param_2, android.os.Handler param_3) {
		if ((__ho8 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "registerReceiver", android.content.Intent.class, params);
		} else {
			return super.registerReceiver(param_0, param_1, param_2, param_3);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1) {
		if ((__ho8 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "registerReceiver", android.content.Intent.class, params);
		} else {
			return super.registerReceiver(param_0, param_1);
		}
	}

	public void removeStickyBroadcast(android.content.Intent param_0) {
		if ((__ho8 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcast", void.class, params);
		} else {
			super.removeStickyBroadcast(param_0);
		}
	}

	public void removeStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1) {
		if ((__ho8 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcastAsUser", void.class, params);
		} else {
			super.removeStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void revokeUriPermission(android.net.Uri param_0, int param_1) {
		if ((__ho8 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "revokeUriPermission", void.class, params);
		} else {
			super.revokeUriPermission(param_0, param_1);
		}
	}

	public void sendBroadcast(android.content.Intent param_0) {
		if ((__ho8 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", void.class, params);
		} else {
			super.sendBroadcast(param_0);
		}
	}
	public void sendBroadcast(android.content.Intent param_0, java.lang.String param_1) {
		if ((__ho8 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", void.class, params);
		} else {
			super.sendBroadcast(param_0, param_1);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2) {
		if ((__ho8 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", void.class, params);
		} else {
			super.sendBroadcastAsUser(param_0, param_1, param_2);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1) {
		if ((__ho8 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", void.class, params);
		} else {
			super.sendBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1) {
		if ((__ho8 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", void.class, params);
		} else {
			super.sendOrderedBroadcast(param_0, param_1);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6) {
		if ((__ho8 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", void.class, params);
		} else {
			super.sendOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void sendOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2, android.content.BroadcastReceiver param_3, android.os.Handler param_4, int param_5, java.lang.String param_6, android.os.Bundle param_7) {
		if ((__ho9 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[8];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			params[7] = param_7;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcastAsUser", void.class, params);
		} else {
			super.sendOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7);
		}
	}

	public void sendStickyBroadcast(android.content.Intent param_0) {
		if ((__ho9 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcast", void.class, params);
		} else {
			super.sendStickyBroadcast(param_0);
		}
	}

	public void sendStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1) {
		if ((__ho9 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcastAsUser", void.class, params);
		} else {
			super.sendStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendStickyOrderedBroadcast(android.content.Intent param_0, android.content.BroadcastReceiver param_1, android.os.Handler param_2, int param_3, java.lang.String param_4, android.os.Bundle param_5) {
		if ((__ho9 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcast", void.class, params);
		} else {
			super.sendStickyOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void sendStickyOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6) {
		if ((__ho9 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcastAsUser", void.class, params);
		} else {
			super.sendStickyOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void setTheme(int param_0) {
		if ((__ho9 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTheme", void.class, params);
		} else {
			super.setTheme(param_0);
		}
	}

	public void setWallpaper(java.io.InputStream param_0) throws java.io.IOException {
		if ((__ho9 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", void.class, params);
		} else {
			super.setWallpaper(param_0);
		}
	}

	public void setWallpaper(android.graphics.Bitmap param_0) throws java.io.IOException {
		if ((__ho9 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", void.class, params);
		} else {
			super.setWallpaper(param_0);
		}
	}

	public void startActivities(android.content.Intent[] param_0) {
		if ((__ho9 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivities", void.class, params);
		} else {
			super.startActivities(param_0);
		}
	}

	public void startActivities(android.content.Intent[] param_0, android.os.Bundle param_1) {
		if ((__ho9 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivities", void.class, params);
		} else {
			super.startActivities(param_0, param_1);
		}
	}

	public void startActivity(android.content.Intent param_0, android.os.Bundle param_1) {
		if ((__ho10 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivity", void.class, params);
		} else {
			super.startActivity(param_0, param_1);
		}
	}

	public void startActivity(android.content.Intent param_0) {
		if ((__ho10 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivity", void.class, params);
		} else {
			super.startActivity(param_0);
		}
	}

	public boolean startInstrumentation(android.content.ComponentName param_0, java.lang.String param_1, android.os.Bundle param_2) {
		if ((__ho10 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startInstrumentation", boolean.class, params);
		} else {
			return super.startInstrumentation(param_0, param_1, param_2);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4, android.os.Bundle param_5) throws android.content.IntentSender.SendIntentException {
		if ((__ho10 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			com.tns.Platform.callJSMethod(this, "startIntentSender", void.class, params);
		} else {
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4) throws android.content.IntentSender.SendIntentException {
		if ((__ho10 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[5];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			com.tns.Platform.callJSMethod(this, "startIntentSender", void.class, params);
		} else {
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public android.content.ComponentName startService(android.content.Intent param_0) {
		if ((__ho10 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.ComponentName)com.tns.Platform.callJSMethod(this, "startService", android.content.ComponentName.class, params);
		} else {
			return super.startService(param_0);
		}
	}

	public boolean stopService(android.content.Intent param_0) {
		if ((__ho10 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "stopService", boolean.class, params);
		} else {
			return super.stopService(param_0);
		}
	}

	public java.lang.String toString() {
		if ((__ho10 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "toString", java.lang.String.class, params);
		} else {
			return super.toString();
		}
	}

	public void unbindService(android.content.ServiceConnection param_0) {
		if ((__ho10 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unbindService", void.class, params);
		} else {
			super.unbindService(param_0);
		}
	}

	public void unregisterActivityLifecycleCallbacks(android.app.Application.ActivityLifecycleCallbacks param_0) {
		if ((__ho10 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterActivityLifecycleCallbacks", void.class, params);
		} else {
			super.unregisterActivityLifecycleCallbacks(param_0);
		}
	}

	public void unregisterComponentCallbacks(android.content.ComponentCallbacks param_0) {
		if ((__ho11 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterComponentCallbacks", void.class, params);
		} else {
			super.unregisterComponentCallbacks(param_0);
		}
	}

	public void unregisterReceiver(android.content.BroadcastReceiver param_0) {
		if ((__ho11 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterReceiver", void.class, params);
		} else {
			super.unregisterReceiver(param_0);
		}
	}

	public boolean equals__super(java.lang.Object other) {
		return super.equals(other);
	}
	public int hashCode__super() {
		return super.hashCode();
	}

	public void setNativeScriptOverrides(java.lang.String[] overrides) {
		for (java.lang.String name: overrides) {
			setNativeScriptOverride(name);
		}
	}
	
	public void setNativeScriptOverride(String name)
	{
			if (name.equals("init")) {
				__ctorOverridden = true;
			}
			if (name.equals("attachBaseContext")) {
				__ho0 |= (1 << 0);
			} else if (name.equals("bindService")) {
				__ho0 |= (1 << 1);
			} else if (name.equals("checkCallingOrSelfPermission")) {
				__ho0 |= (1 << 2);
			} else if (name.equals("checkCallingOrSelfUriPermission")) {
				__ho0 |= (1 << 3);
			} else if (name.equals("checkCallingPermission")) {
				__ho0 |= (1 << 4);
			} else if (name.equals("checkCallingUriPermission")) {
				__ho0 |= (1 << 5);
			} else if (name.equals("checkPermission")) {
				__ho0 |= (1 << 6);
			} else if (name.equals("checkUriPermission")) {
				__ho0 |= (1 << 7);
			} else if (name.equals("clearWallpaper")) {
				__ho1 |= (1 << 0);
			} else if (name.equals("clone")) {
				__ho1 |= (1 << 1);
			} else if (name.equals("createConfigurationContext")) {
				__ho1 |= (1 << 2);
			} else if (name.equals("createDisplayContext")) {
				__ho1 |= (1 << 3);
			} else if (name.equals("createPackageContext")) {
				__ho1 |= (1 << 4);
			} else if (name.equals("databaseList")) {
				__ho1 |= (1 << 5);
			} else if (name.equals("deleteDatabase")) {
				__ho1 |= (1 << 6);
			} else if (name.equals("deleteFile")) {
				__ho1 |= (1 << 7);
			} else if (name.equals("enforceCallingOrSelfPermission")) {
				__ho2 |= (1 << 0);
			} else if (name.equals("enforceCallingOrSelfUriPermission")) {
				__ho2 |= (1 << 1);
			} else if (name.equals("enforceCallingPermission")) {
				__ho2 |= (1 << 2);
			} else if (name.equals("enforceCallingUriPermission")) {
				__ho2 |= (1 << 3);
			} else if (name.equals("enforcePermission")) {
				__ho2 |= (1 << 4);
			} else if (name.equals("enforceUriPermission")) {
				__ho2 |= (1 << 5);
			} else if (name.equals("equals")) {
				__ho2 |= (1 << 6);
			} else if (name.equals("fileList")) {
				__ho2 |= (1 << 7);
			} else if (name.equals("finalize")) {
				__ho3 |= (1 << 0);
			} else if (name.equals("getApplicationContext")) {
				__ho3 |= (1 << 1);
			} else if (name.equals("getApplicationInfo")) {
				__ho3 |= (1 << 2);
			} else if (name.equals("getAssets")) {
				__ho3 |= (1 << 3);
			} else if (name.equals("getBaseContext")) {
				__ho3 |= (1 << 4);
			} else if (name.equals("getCacheDir")) {
				__ho3 |= (1 << 5);
			} else if (name.equals("getClassLoader")) {
				__ho3 |= (1 << 6);
			} else if (name.equals("getContentResolver")) {
				__ho3 |= (1 << 7);
			} else if (name.equals("getDatabasePath")) {
				__ho4 |= (1 << 0);
			} else if (name.equals("getDir")) {
				__ho4 |= (1 << 1);
			} else if (name.equals("getExternalCacheDir")) {
				__ho4 |= (1 << 2);
			} else if (name.equals("getExternalFilesDir")) {
				__ho4 |= (1 << 3);
			} else if (name.equals("getFileStreamPath")) {
				__ho4 |= (1 << 4);
			} else if (name.equals("getFilesDir")) {
				__ho4 |= (1 << 5);
			} else if (name.equals("getMainLooper")) {
				__ho4 |= (1 << 6);
			} else if (name.equals("getObbDir")) {
				__ho4 |= (1 << 7);
			} else if (name.equals("getPackageCodePath")) {
				__ho5 |= (1 << 0);
			} else if (name.equals("getPackageManager")) {
				__ho5 |= (1 << 1);
			} else if (name.equals("getPackageName")) {
				__ho5 |= (1 << 2);
			} else if (name.equals("getPackageResourcePath")) {
				__ho5 |= (1 << 3);
			} else if (name.equals("getResources")) {
				__ho5 |= (1 << 4);
			} else if (name.equals("getSharedPreferences")) {
				__ho5 |= (1 << 5);
			} else if (name.equals("getSystemService")) {
				__ho5 |= (1 << 6);
			} else if (name.equals("getTheme")) {
				__ho5 |= (1 << 7);
			} else if (name.equals("getWallpaper")) {
				__ho6 |= (1 << 0);
			} else if (name.equals("getWallpaperDesiredMinimumHeight")) {
				__ho6 |= (1 << 1);
			} else if (name.equals("getWallpaperDesiredMinimumWidth")) {
				__ho6 |= (1 << 2);
			} else if (name.equals("grantUriPermission")) {
				__ho6 |= (1 << 3);
			} else if (name.equals("hashCode")) {
				__ho6 |= (1 << 4);
			} else if (name.equals("isRestricted")) {
				__ho6 |= (1 << 5);
			} else if (name.equals("onConfigurationChanged")) {
				__ho6 |= (1 << 6);
			} else if (name.equals("onCreate")) {
				__ho6 |= (1 << 7);
			} else if (name.equals("onLowMemory")) {
				__ho7 |= (1 << 0);
			} else if (name.equals("onTerminate")) {
				__ho7 |= (1 << 1);
			} else if (name.equals("onTrimMemory")) {
				__ho7 |= (1 << 2);
			} else if (name.equals("openFileInput")) {
				__ho7 |= (1 << 3);
			} else if (name.equals("openFileOutput")) {
				__ho7 |= (1 << 4);
			} else if (name.equals("openOrCreateDatabase")) {
				__ho7 |= (1 << 5);
			} else if (name.equals("peekWallpaper")) {
				__ho7 |= (1 << 6);
			} else if (name.equals("registerActivityLifecycleCallbacks")) {
				__ho7 |= (1 << 7);
			} else if (name.equals("registerComponentCallbacks")) {
				__ho8 |= (1 << 0);
			} else if (name.equals("registerReceiver")) {
				__ho8 |= (1 << 1);
			} else if (name.equals("removeStickyBroadcast")) {
				__ho8 |= (1 << 2);
			} else if (name.equals("removeStickyBroadcastAsUser")) {
				__ho8 |= (1 << 3);
			} else if (name.equals("revokeUriPermission")) {
				__ho8 |= (1 << 4);
			} else if (name.equals("sendBroadcast")) {
				__ho8 |= (1 << 5);
			} else if (name.equals("sendBroadcastAsUser")) {
				__ho8 |= (1 << 6);
			} else if (name.equals("sendOrderedBroadcast")) {
				__ho8 |= (1 << 7);
			} else if (name.equals("sendOrderedBroadcastAsUser")) {
				__ho9 |= (1 << 0);
			} else if (name.equals("sendStickyBroadcast")) {
				__ho9 |= (1 << 1);
			} else if (name.equals("sendStickyBroadcastAsUser")) {
				__ho9 |= (1 << 2);
			} else if (name.equals("sendStickyOrderedBroadcast")) {
				__ho9 |= (1 << 3);
			} else if (name.equals("sendStickyOrderedBroadcastAsUser")) {
				__ho9 |= (1 << 4);
			} else if (name.equals("setTheme")) {
				__ho9 |= (1 << 5);
			} else if (name.equals("setWallpaper")) {
				__ho9 |= (1 << 6);
			} else if (name.equals("startActivities")) {
				__ho9 |= (1 << 7);
			} else if (name.equals("startActivity")) {
				__ho10 |= (1 << 0);
			} else if (name.equals("startInstrumentation")) {
				__ho10 |= (1 << 1);
			} else if (name.equals("startIntentSender")) {
				__ho10 |= (1 << 2);
			} else if (name.equals("startService")) {
				__ho10 |= (1 << 3);
			} else if (name.equals("stopService")) {
				__ho10 |= (1 << 4);
			} else if (name.equals("toString")) {
				__ho10 |= (1 << 5);
			} else if (name.equals("unbindService")) {
				__ho10 |= (1 << 6);
			} else if (name.equals("unregisterActivityLifecycleCallbacks")) {
				__ho10 |= (1 << 7);
			} else if (name.equals("unregisterComponentCallbacks")) {
				__ho11 |= (1 << 0);
			} else if (name.equals("unregisterReceiver")) {
				__ho11 |= (1 << 1);
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
	
	private final String logTag = "NativeScriptApplication";
}

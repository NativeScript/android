package com.tns;

import android.util.Log;

public class NativeScriptActivity extends android.app.Activity implements com.tns.NativeScriptHashCodeProvider {
	public NativeScriptActivity() {
		super();
		if (__ctorOverridden) {
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "init", true, params);
		}
	}

	public void addContentView(android.view.View param_0, android.view.ViewGroup.LayoutParams param_1) {
		if ((__ho0 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "addContentView", params);
		} else {
			super.addContentView(param_0, param_1);
		}
	}

	public void applyOverrideConfiguration(android.content.res.Configuration param_0) {
		if ((__ho0 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "applyOverrideConfiguration", params);
		} else {
			super.applyOverrideConfiguration(param_0);
		}
	}

	protected void attachBaseContext(android.content.Context param_0) {
		if ((__ho0 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "attachBaseContext", params);
		} else {
			super.attachBaseContext(param_0);
		}
	}

	public boolean bindService(android.content.Intent param_0, android.content.ServiceConnection param_1, int param_2) {
		if ((__ho0 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "bindService", params);
		} else {
			return super.bindService(param_0, param_1, param_2);
		}
	}

	public int checkCallingOrSelfPermission(java.lang.String param_0) {
		if ((__ho0 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingOrSelfPermission", params);
		} else {
			return super.checkCallingOrSelfPermission(param_0);
		}
	}

	public int checkCallingOrSelfUriPermission(android.net.Uri param_0, int param_1) {
		if ((__ho0 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingOrSelfUriPermission", params);
		} else {
			return super.checkCallingOrSelfUriPermission(param_0, param_1);
		}
	}

	public int checkCallingPermission(java.lang.String param_0) {
		if ((__ho0 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingPermission", params);
		} else {
			return super.checkCallingPermission(param_0);
		}
	}

	public int checkCallingUriPermission(android.net.Uri param_0, int param_1) {
		if ((__ho0 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingUriPermission", params);
		} else {
			return super.checkCallingUriPermission(param_0, param_1);
		}
	}

	public int checkPermission(java.lang.String param_0, int param_1, int param_2) {
		if ((__ho1 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkPermission", params);
		} else {
			return super.checkPermission(param_0, param_1, param_2);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3) {
		
		if ((__ho1 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkUriPermission", params);
		} else {
			return super.checkUriPermission(param_0, param_1, param_2, param_3);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5) {
		
		if ((__ho1 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkUriPermission", params);
		} else {
			return super.checkUriPermission(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void clearWallpaper() throws java.io.IOException {
		
		if ((__ho1 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "clearWallpaper", params);
		} else {
			super.clearWallpaper();
		}
	}

	protected java.lang.Object clone() throws java.lang.CloneNotSupportedException {
		
		if ((__ho1 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "clone", params);
		} else {
			return super.clone();
		}
	}

	public void closeContextMenu() {
		
		if ((__ho1 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "closeContextMenu", params);
		} else {
			super.closeContextMenu();
		}
	}

	public void closeOptionsMenu() {
		
		if ((__ho1 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "closeOptionsMenu", params);
		} else {
			super.closeOptionsMenu();
		}
	}

	public android.content.Context createConfigurationContext(android.content.res.Configuration param_0) {
		
		if ((__ho1 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createConfigurationContext", params);
		} else {
			return super.createConfigurationContext(param_0);
		}
	}

	public android.content.Context createDisplayContext(android.view.Display param_0) {
		
		if ((__ho1 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createDisplayContext", params);
		} else {
			return super.createDisplayContext(param_0);
		}
	}

	public android.content.Context createPackageContext(java.lang.String param_0, int param_1) throws android.content.pm.PackageManager.NameNotFoundException {
		
		if ((__ho2 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createPackageContext", params);
		} else {
			return super.createPackageContext(param_0, param_1);
		}
	}

	public android.app.PendingIntent createPendingResult(int param_0, android.content.Intent param_1, int param_2) {
		
		if ((__ho2 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (android.app.PendingIntent)com.tns.Platform.callJSMethod(this, "createPendingResult", params);
		} else {
			return super.createPendingResult(param_0, param_1, param_2);
		}
	}

	public java.lang.String[] databaseList() {
		
		if ((__ho2 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String[])com.tns.Platform.callJSMethod(this, "databaseList", params);
		} else {
			return super.databaseList();
		}
	}

	public boolean deleteDatabase(java.lang.String param_0) {
		
		if ((__ho2 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "deleteDatabase", params);
		} else {
			return super.deleteDatabase(param_0);
		}
	}

	public boolean deleteFile(java.lang.String param_0) {
		
		if ((__ho2 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "deleteFile", params);
		} else {
			return super.deleteFile(param_0);
		}
	}

	public boolean dispatchGenericMotionEvent(android.view.MotionEvent param_0) {
		
		if ((__ho2 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchGenericMotionEvent", params);
		} else {
			return super.dispatchGenericMotionEvent(param_0);
		}
	}

	public boolean dispatchKeyEvent(android.view.KeyEvent param_0) {
		
		if ((__ho2 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchKeyEvent", params);
		} else {
			return super.dispatchKeyEvent(param_0);
		}
	}

	public boolean dispatchKeyShortcutEvent(android.view.KeyEvent param_0) {
		
		if ((__ho2 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchKeyShortcutEvent", params);
		} else {
			return super.dispatchKeyShortcutEvent(param_0);
		}
	}

	public boolean dispatchPopulateAccessibilityEvent(android.view.accessibility.AccessibilityEvent param_0) {
		
		if ((__ho3 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchPopulateAccessibilityEvent", params);
		} else {
			return super.dispatchPopulateAccessibilityEvent(param_0);
		}
	}

	public boolean dispatchTouchEvent(android.view.MotionEvent param_0) {
		
		if ((__ho3 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchTouchEvent", params);
		} else {
			return super.dispatchTouchEvent(param_0);
		}
	}

	public boolean dispatchTrackballEvent(android.view.MotionEvent param_0) {
		
		if ((__ho3 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchTrackballEvent", params);
		} else {
			return super.dispatchTrackballEvent(param_0);
		}
	}

	public void dump(java.lang.String param_0, java.io.FileDescriptor param_1, java.io.PrintWriter param_2, java.lang.String[] param_3) {
		
		if ((__ho3 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			com.tns.Platform.callJSMethod(this, "dump", params);
		} else {
			super.dump(param_0, param_1, param_2, param_3);
		}
	}

	public void enforceCallingOrSelfPermission(java.lang.String param_0, java.lang.String param_1) {
		
		if ((__ho3 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfPermission", params);
		} else {
			super.enforceCallingOrSelfPermission(param_0, param_1);
		}
	}

	public void enforceCallingOrSelfUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2) {
		
		if ((__ho3 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfUriPermission", params);
		} else {
			super.enforceCallingOrSelfUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforceCallingPermission(java.lang.String param_0, java.lang.String param_1) {
		
		if ((__ho3 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingPermission", params);
		} else {
			super.enforceCallingPermission(param_0, param_1);
		}
	}

	public void enforceCallingUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2) {
		
		if ((__ho3 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingUriPermission", params);
		} else {
			super.enforceCallingUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforcePermission(java.lang.String param_0, int param_1, int param_2, java.lang.String param_3) {
		
		if ((__ho4 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			com.tns.Platform.callJSMethod(this, "enforcePermission", params);
		} else {
			super.enforcePermission(param_0, param_1, param_2, param_3);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3, java.lang.String param_4) {
		
		if ((__ho4 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[5];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", params);
		} else {
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5, java.lang.String param_6) {
		
		if ((__ho4 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", params);
		} else {
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public boolean equals(java.lang.Object param_0) {
		
		if ((__ho4 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "equals", params);
		} else {
			return super.equals(param_0);
		}
	}

	public java.lang.String[] fileList() {
		
		if ((__ho4 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String[])com.tns.Platform.callJSMethod(this, "fileList", params);
		} else {
			return super.fileList();
		}
	}

	protected void finalize() throws java.lang.Throwable {
		
		if ((__ho4 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "finalize", params);
		} else {
			super.finalize();
		}
	}

	public android.view.View findViewById(int param_0) {
		
		if ((__ho4 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "findViewById", params);
		} else {
			return super.findViewById(param_0);
		}
	}

	public void finish() {
		
		if ((__ho4 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "finish", params);
		} else {
			super.finish();
		}
	}

	public void finishActivity(int param_0) {
		
		if ((__ho4 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "finishActivity", params);
		} else {
			super.finishActivity(param_0);
		}
	}

	public void finishActivityFromChild(android.app.Activity param_0, int param_1) {
	
		if ((__ho5 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "finishActivityFromChild", params);
		} else {
			super.finishActivityFromChild(param_0, param_1);
		}
	}

	public void finishAffinity() {
		
		if ((__ho5 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "finishAffinity", params);
		} else {
			super.finishAffinity();
		}
	}

	public void finishFromChild(android.app.Activity param_0) {
		
		if ((__ho5 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "finishFromChild", params);
		} else {
			super.finishFromChild(param_0);
		}
	}

	public android.app.ActionBar getActionBar() {
		
		if ((__ho5 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (android.app.ActionBar)com.tns.Platform.callJSMethod(this, "getActionBar", params);
		} else {
			return super.getActionBar();
		}
	}

	public android.content.Context getApplicationContext() {
		
		if ((__ho5 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "getApplicationContext", params);
		} else {
			return super.getApplicationContext();
		}
	}

	public android.content.pm.ApplicationInfo getApplicationInfo() {
		if ((__ho5 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.pm.ApplicationInfo)com.tns.Platform.callJSMethod(this, "getApplicationInfo", params);
		} else {
			return super.getApplicationInfo();
		}
	}

	public android.content.res.AssetManager getAssets() {
		
		if ((__ho5 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.res.AssetManager)com.tns.Platform.callJSMethod(this, "getAssets", params);
		} else {
			return super.getAssets();
		}
	}

	public android.content.Context getBaseContext() {
		if ((__ho5 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "getBaseContext", params);
		} else {
			return super.getBaseContext();
		}
	}

	public java.io.File getCacheDir() {
	
		if ((__ho6 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getCacheDir", params);
		} else {
			return super.getCacheDir();
		}
	}

	public android.content.ComponentName getCallingActivity() {
		
		if ((__ho6 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.ComponentName)com.tns.Platform.callJSMethod(this, "getCallingActivity", params);
		} else {
			return super.getCallingActivity();
		}
	}

	public java.lang.String getCallingPackage() {
		
		if ((__ho6 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getCallingPackage", params);
		} else {
			return super.getCallingPackage();
		}
	}

	public int getChangingConfigurations() {
		
		if ((__ho6 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getChangingConfigurations", params);
		} else {
			return super.getChangingConfigurations();
		}
	}

	public java.lang.ClassLoader getClassLoader() {
		
		if ((__ho6 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.ClassLoader)com.tns.Platform.callJSMethod(this, "getClassLoader", params);
		} else {
			return super.getClassLoader();
		}
	}

	public android.content.ComponentName getComponentName() {
		
		if ((__ho6 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.ComponentName)com.tns.Platform.callJSMethod(this, "getComponentName", params);
		} else {
			return super.getComponentName();
		}
	}

	public android.content.ContentResolver getContentResolver() {
		
		if ((__ho6 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.ContentResolver)com.tns.Platform.callJSMethod(this, "getContentResolver", params);
		} else {
			return super.getContentResolver();
		}
	}

	public android.view.View getCurrentFocus() {
		
		if ((__ho6 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "getCurrentFocus", params);
		} else {
			return super.getCurrentFocus();
		}
	}

	public java.io.File getDatabasePath(java.lang.String param_0) {
		
		if ((__ho7 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getDatabasePath", params);
		} else {
			return super.getDatabasePath(param_0);
		}
	}

	public java.io.File getDir(java.lang.String param_0, int param_1) {
		
		if ((__ho7 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getDir", params);
		} else {
			return super.getDir(param_0, param_1);
		}
	}

	public java.io.File getExternalCacheDir() {
		
		if ((__ho7 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getExternalCacheDir", params);
		} else {
			return super.getExternalCacheDir();
		}
	}

	public java.io.File getExternalFilesDir(java.lang.String param_0) {
		
		if ((__ho7 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getExternalFilesDir", params);
		} else {
			return super.getExternalFilesDir(param_0);
		}
	}

	public java.io.File getFileStreamPath(java.lang.String param_0) {
		
		if ((__ho7 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getFileStreamPath", params);
		} else {
			return super.getFileStreamPath(param_0);
		}
	}

	public java.io.File getFilesDir() {
		
		if ((__ho7 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getFilesDir", params);
		} else {
			return super.getFilesDir();
		}
	}

	public android.app.FragmentManager getFragmentManager() {
		
		if ((__ho7 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (android.app.FragmentManager)com.tns.Platform.callJSMethod(this, "getFragmentManager", params);
		} else {
			return super.getFragmentManager();
		}
	}

	public android.content.Intent getIntent() {
		
		if ((__ho7 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "getIntent", params);
		} else {
			return super.getIntent();
		}
	}

	public java.lang.Object getLastNonConfigurationInstance() {
		
		if ((__ho8 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "getLastNonConfigurationInstance", params);
		} else {
			return super.getLastNonConfigurationInstance();
		}
	}

	public android.view.LayoutInflater getLayoutInflater() {
		
		if ((__ho8 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (android.view.LayoutInflater)com.tns.Platform.callJSMethod(this, "getLayoutInflater", params);
		} else {
			return super.getLayoutInflater();
		}
	}

	public android.app.LoaderManager getLoaderManager() {
		
		if ((__ho8 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (android.app.LoaderManager)com.tns.Platform.callJSMethod(this, "getLoaderManager", params);
		} else {
			return super.getLoaderManager();
		}
	}

	public java.lang.String getLocalClassName() {
		
		if ((__ho8 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getLocalClassName", params);
		} else {
			return super.getLocalClassName();
		}
	}

	public android.os.Looper getMainLooper() {
		
		if ((__ho8 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (android.os.Looper)com.tns.Platform.callJSMethod(this, "getMainLooper", params);
		} else {
			return super.getMainLooper();
		}
	}

	public android.view.MenuInflater getMenuInflater() {
		
		if ((__ho8 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (android.view.MenuInflater)com.tns.Platform.callJSMethod(this, "getMenuInflater", params);
		} else {
			return super.getMenuInflater();
		}
	}

	public java.io.File getObbDir() {
		
		if ((__ho8 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getObbDir", params);
		} else {
			return super.getObbDir();
		}
	}

	public java.lang.String getPackageCodePath() {
		
		if ((__ho8 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageCodePath", params);
		} else {
			return super.getPackageCodePath();
		}
	}

	public android.content.pm.PackageManager getPackageManager() {
		
		if ((__ho9 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.pm.PackageManager)com.tns.Platform.callJSMethod(this, "getPackageManager", params);
		} else {
			return super.getPackageManager();
		}
	}

	public java.lang.String getPackageName() {
		
		if ((__ho9 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageName", params);
		} else {
			return super.getPackageName();
		}
	}

	public java.lang.String getPackageResourcePath() {
		
		if ((__ho9 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageResourcePath", params);
		} else {
			return super.getPackageResourcePath();
		}
	}

	public android.content.Intent getParentActivityIntent() {
		
		if ((__ho9 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "getParentActivityIntent", params);
		} else {
			return super.getParentActivityIntent();
		}
	}

	public android.content.SharedPreferences getPreferences(int param_0) {
		
		if ((__ho9 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.SharedPreferences)com.tns.Platform.callJSMethod(this, "getPreferences", params);
		} else {
			return super.getPreferences(param_0);
		}
	}

	public int getRequestedOrientation() {
		
		if ((__ho9 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getRequestedOrientation", params);
		} else {
			return super.getRequestedOrientation();
		}
	}

	public android.content.res.Resources getResources() {
		if ((__ho9 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.res.Resources)com.tns.Platform.callJSMethod(this, "getResources", params);
		} else {
			return super.getResources();
		}
	}

	public android.content.SharedPreferences getSharedPreferences(java.lang.String param_0, int param_1) {
		
		if ((__ho9 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.SharedPreferences)com.tns.Platform.callJSMethod(this, "getSharedPreferences", params);
		} else {
			return super.getSharedPreferences(param_0, param_1);
		}
	}

	public java.lang.Object getSystemService(java.lang.String param_0) {
		if ((__ho10 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "getSystemService", params);
		} else {
			return super.getSystemService(param_0);
		}
	}

	public int getTaskId() {
		
		if ((__ho10 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getTaskId", params);
		} else {
			return super.getTaskId();
		}
	}

	public android.content.res.Resources.Theme getTheme() {
		
		if ((__ho10 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (android.content.res.Resources.Theme)com.tns.Platform.callJSMethod(this, "getTheme", params);
		} else {
			return super.getTheme();
		}
	}

	public android.graphics.drawable.Drawable getWallpaper() {
		
		if ((__ho10 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			return (android.graphics.drawable.Drawable)com.tns.Platform.callJSMethod(this, "getWallpaper", params);
		} else {
			return super.getWallpaper();
		}
	}

	public int getWallpaperDesiredMinimumHeight() {
		
		if ((__ho10 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumHeight", params);
		} else {
			return super.getWallpaperDesiredMinimumHeight();
		}
	}

	public int getWallpaperDesiredMinimumWidth() {
		
		if ((__ho10 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumWidth", params);
		} else {
			return super.getWallpaperDesiredMinimumWidth();
		}
	}

	public android.view.Window getWindow() {
		
		if ((__ho10 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (android.view.Window)com.tns.Platform.callJSMethod(this, "getWindow", params);
		} else {
			return super.getWindow();
		}
	}

	public android.view.WindowManager getWindowManager() {
		
		if ((__ho10 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (android.view.WindowManager)com.tns.Platform.callJSMethod(this, "getWindowManager", params);
		} else {
			return super.getWindowManager();
		}
	}

	public void grantUriPermission(java.lang.String param_0, android.net.Uri param_1, int param_2) {
		
		if ((__ho11 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "grantUriPermission", params);
		} else {
			super.grantUriPermission(param_0, param_1, param_2);
		}
	}

	public boolean hasWindowFocus() {
		
		if ((__ho11 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "hasWindowFocus", params);
		} else {
			return super.hasWindowFocus();
		}
	}

	public int hashCode() {
		
		if ((__ho11 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "hashCode", params);
		} else {
			return super.hashCode();
		}
	}

	public void invalidateOptionsMenu() {
		
		if ((__ho11 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "invalidateOptionsMenu", params);
		} else {
			super.invalidateOptionsMenu();
		}
	}

	public boolean isChangingConfigurations() {
		
		if ((__ho11 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isChangingConfigurations", params);
		} else {
			return super.isChangingConfigurations();
		}
	}

	public boolean isDestroyed() {
		
		if ((__ho11 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isDestroyed", params);
		} else {
			return super.isDestroyed();
		}
	}

	public boolean isFinishing() {
		
		if ((__ho11 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isFinishing", params);
		} else {
			return super.isFinishing();
		}
	}

	public boolean isRestricted() {
		
		if ((__ho11 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isRestricted", params);
		} else {
			return super.isRestricted();
		}
	}

	public boolean isTaskRoot() {
		
		if ((__ho12 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isTaskRoot", params);
		} else {
			return super.isTaskRoot();
		}
	}

	public boolean moveTaskToBack(boolean param_0) {
		
		if ((__ho12 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "moveTaskToBack", params);
		} else {
			return super.moveTaskToBack(param_0);
		}
	}

	public boolean navigateUpTo(android.content.Intent param_0) {
		
		if ((__ho12 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "navigateUpTo", params);
		} else {
			return super.navigateUpTo(param_0);
		}
	}

	public boolean navigateUpToFromChild(android.app.Activity param_0, android.content.Intent param_1) {
		
		if ((__ho12 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "navigateUpToFromChild", params);
		} else {
			return super.navigateUpToFromChild(param_0, param_1);
		}
	}

	public void onActionModeFinished(android.view.ActionMode param_0) {
		
		if ((__ho12 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActionModeFinished", params);
		} else {
			super.onActionModeFinished(param_0);
		}
	}

	public void onActionModeStarted(android.view.ActionMode param_0) {
		
		if ((__ho12 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActionModeStarted", params);
		} else {
			super.onActionModeStarted(param_0);
		}
	}

	protected void onActivityResult(int param_0, int param_1, android.content.Intent param_2) {
		
		if ((__ho12 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onActivityResult", params);
		} else {
			super.onActivityResult(param_0, param_1, param_2);
		}
	}

	protected void onApplyThemeResource(android.content.res.Resources.Theme param_0, int param_1, boolean param_2) {
		if ((__ho12 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onApplyThemeResource", params);
		} else {
			super.onApplyThemeResource(param_0, param_1, param_2);
		}
	}

	public void onAttachFragment(android.app.Fragment param_0) {
		
		if ((__ho13 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onAttachFragment", params);
		} else {
			super.onAttachFragment(param_0);
		}
	}

	public void onAttachedToWindow() {
		
		if ((__ho13 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onAttachedToWindow", params);
		} else {
			super.onAttachedToWindow();
		}
	}

	public void onBackPressed() {
		
		if ((__ho13 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onBackPressed", params);
		} else {
			super.onBackPressed();
		}
	}

	protected void onChildTitleChanged(android.app.Activity param_0, java.lang.CharSequence param_1) {
		
		if ((__ho13 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onChildTitleChanged", params);
		} else {
			super.onChildTitleChanged(param_0, param_1);
		}
	}

	public void onConfigurationChanged(android.content.res.Configuration param_0) {
		
		if ((__ho13 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onConfigurationChanged", params);
		} else {
			super.onConfigurationChanged(param_0);
		}
	}

	public void onContentChanged() {
		
		if ((__ho13 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onContentChanged", params);
		} else {
			super.onContentChanged();
		}
	}

	public boolean onContextItemSelected(android.view.MenuItem param_0) {
		
		if ((__ho13 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onContextItemSelected", params);
		} else {
			return super.onContextItemSelected(param_0);
		}
	}

	public void onContextMenuClosed(android.view.Menu param_0) {
		
		if ((__ho13 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onContextMenuClosed", params);
		} else {
			super.onContextMenuClosed(param_0);
		}
	}

	protected void onCreate(android.os.Bundle param_0) {
		if(ErrorReport.HasApplicationCreateError)
		{
			super.onCreate(param_0);
			this.finish();
			return;
		}
		
		if (!__initialized) {
			__initialized = true;
			
			Log.d(Platform.DEFAULT_LOG_TAG, "Creating Activity with intent: " + super.getIntent().toString() + " action:" + super.getIntent().getAction());
			
			if (BuildConfig.DEBUG && super.getIntent().getAction() == null)
			{
				super.getIntent().setAction(android.content.Intent.ACTION_MAIN);
			}
			com.tns.Platform.initInstance(this, super.getIntent());
		}
		if ((__ho14 & (1 << 0)) > 0) { 
			boolean isErrorReport = ErrorReport.hasIntent(this.getIntent());
			if (isErrorReport)
			{
				super.onCreate(param_0);
				new ErrorReport(this).buildUI();
			}
			else
			{
				java.lang.Object[] params = new Object[1];
				params[0] = param_0;
//				try
//				{
					com.tns.Platform.callJSMethod(this, "onCreate", params);	
//				}
//				catch(Throwable ex)
//				{
//					ErrorReport.HasApplicationCreateError = true;
//					ErrorReport.startActivity(this, ex);
//				}
			}
		} else {
			super.onCreate(param_0);
		}
		
	}

	public void onCreateContextMenu(android.view.ContextMenu param_0, android.view.View param_1, android.view.ContextMenu.ContextMenuInfo param_2) {
		
		if ((__ho14 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onCreateContextMenu", params);
		} else {
			super.onCreateContextMenu(param_0, param_1, param_2);
		}
	}

	public java.lang.CharSequence onCreateDescription() {
		
		if ((__ho14 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.CharSequence)com.tns.Platform.callJSMethod(this, "onCreateDescription", params);
		} else {
			return super.onCreateDescription();
		}
	}

	protected android.app.Dialog onCreateDialog(int param_0) {
		
		if ((__ho14 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.app.Dialog)com.tns.Platform.callJSMethod(this, "onCreateDialog", params);
		} else {
			return super.onCreateDialog(param_0);
		}
	}

	protected android.app.Dialog onCreateDialog(int param_0, android.os.Bundle param_1) {
		
		if ((__ho14 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.app.Dialog)com.tns.Platform.callJSMethod(this, "onCreateDialog", params);
		} else {
			return super.onCreateDialog(param_0, param_1);
		}
	}

	public void onCreateNavigateUpTaskStack(android.app.TaskStackBuilder param_0) {
		
		if ((__ho14 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onCreateNavigateUpTaskStack", params);
		} else {
			super.onCreateNavigateUpTaskStack(param_0);
		}
	}

	public boolean onCreateOptionsMenu(android.view.Menu param_0) {
		
		if ((__ho14 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onCreateOptionsMenu", params);
		} else {
			return super.onCreateOptionsMenu(param_0);
		}
	}

	public boolean onCreatePanelMenu(int param_0, android.view.Menu param_1) {
		
		if ((__ho14 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onCreatePanelMenu", params);
		} else {
			return super.onCreatePanelMenu(param_0, param_1);
		}
	}

	public android.view.View onCreatePanelView(int param_0) {
		
		if ((__ho14 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "onCreatePanelView", params);
		} else {
			return super.onCreatePanelView(param_0);
		}
	}

	public boolean onCreateThumbnail(android.graphics.Bitmap param_0, android.graphics.Canvas param_1) {
		
		if ((__ho15 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onCreateThumbnail", params);
		} else {
			return super.onCreateThumbnail(param_0, param_1);
		}
	}

	public android.view.View onCreateView(java.lang.String param_0, android.content.Context param_1, android.util.AttributeSet param_2) {
		
		if ((__ho15 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "onCreateView", params);
		} else {
			return super.onCreateView(param_0, param_1, param_2);
		}
	}

	public android.view.View onCreateView(android.view.View param_0, java.lang.String param_1, android.content.Context param_2, android.util.AttributeSet param_3) {
		
		if ((__ho15 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "onCreateView", params);
		} else {
			return super.onCreateView(param_0, param_1, param_2, param_3);
		}
	}

	protected void onDestroy() {
		
		if ((__ho15 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onDestroy", params);
		} else {
			super.onDestroy();
		}
	}

	public void onDetachedFromWindow() {
		
		if ((__ho15 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onDetachedFromWindow", params);
		} else {
			super.onDetachedFromWindow();
		}
	}

	public boolean onGenericMotionEvent(android.view.MotionEvent param_0) {
		
		if ((__ho15 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onGenericMotionEvent", params);
		} else {
			return super.onGenericMotionEvent(param_0);
		}
	}

	public boolean onKeyDown(int param_0, android.view.KeyEvent param_1) {
		
		if ((__ho15 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyDown", params);
		} else {
			return super.onKeyDown(param_0, param_1);
		}
	}

	public boolean onKeyLongPress(int param_0, android.view.KeyEvent param_1) {
		
		if ((__ho15 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyLongPress", params);
		} else {
			return super.onKeyLongPress(param_0, param_1);
		}
	}

	public boolean onKeyMultiple(int param_0, int param_1, android.view.KeyEvent param_2) {
		
		if ((__ho15 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyMultiple", params);
		} else {
			return super.onKeyMultiple(param_0, param_1, param_2);
		}
	}

	public boolean onKeyShortcut(int param_0, android.view.KeyEvent param_1) {
		
		if ((__ho16 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyShortcut", params);
		} else {
			return super.onKeyShortcut(param_0, param_1);
		}
	}

	public boolean onKeyUp(int param_0, android.view.KeyEvent param_1) {
		
		if ((__ho16 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyUp", params);
		} else {
			return super.onKeyUp(param_0, param_1);
		}
	}

	public void onLowMemory() {
		
		if ((__ho16 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onLowMemory", params);
		} else {
			super.onLowMemory();
		}
	}

	public boolean onMenuItemSelected(int param_0, android.view.MenuItem param_1) {
		
		if ((__ho16 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onMenuItemSelected", params);
		} else {
			return super.onMenuItemSelected(param_0, param_1);
		}
	}

	public boolean onMenuOpened(int param_0, android.view.Menu param_1) {
		
		if ((__ho16 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onMenuOpened", params);
		} else {
			return super.onMenuOpened(param_0, param_1);
		}
	}

	public boolean onNavigateUp() {
		
		if ((__ho16 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onNavigateUp", params);
		} else {
			return super.onNavigateUp();
		}
	}

	public boolean onNavigateUpFromChild(android.app.Activity param_0) {
		
		if ((__ho16 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onNavigateUpFromChild", params);
		} else {
			return super.onNavigateUpFromChild(param_0);
		}
	}

	protected void onNewIntent(android.content.Intent param_0) {
		
		if ((__ho16 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onNewIntent", params);
		} else {
			super.onNewIntent(param_0);
		}
	}

	public boolean onOptionsItemSelected(android.view.MenuItem param_0) {
		
		if ((__ho17 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onOptionsItemSelected", params);
		} else {
			return super.onOptionsItemSelected(param_0);
		}
	}

	public void onOptionsMenuClosed(android.view.Menu param_0) {
		
		if ((__ho17 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onOptionsMenuClosed", params);
		} else {
			super.onOptionsMenuClosed(param_0);
		}
	}

	public void onPanelClosed(int param_0, android.view.Menu param_1) {
		
		if ((__ho17 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onPanelClosed", params);
		} else {
			super.onPanelClosed(param_0, param_1);
		}
	}

	protected void onPause() {
		
		if ((__ho17 & (1 << 3)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onPause", params);
		} else {
			super.onPause();
		}
	}

	protected void onPostCreate(android.os.Bundle param_0) {
		
		if ((__ho17 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onPostCreate", params);
		} else {
			super.onPostCreate(param_0);
		}
	}

	protected void onPostResume() {
		
		if ((__ho17 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onPostResume", params);
		} else {
			super.onPostResume();
		}
	}

	protected void onPrepareDialog(int param_0, android.app.Dialog param_1, android.os.Bundle param_2) {
		
		if ((__ho17 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onPrepareDialog", params);
		} else {
			super.onPrepareDialog(param_0, param_1, param_2);
		}
	}

	protected void onPrepareDialog(int param_0, android.app.Dialog param_1) {
		
		if ((__ho17 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onPrepareDialog", params);
		} else {
			super.onPrepareDialog(param_0, param_1);
		}
	}

	public void onPrepareNavigateUpTaskStack(android.app.TaskStackBuilder param_0) {
		
		if ((__ho17 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onPrepareNavigateUpTaskStack", params);
		} else {
			super.onPrepareNavigateUpTaskStack(param_0);
		}
	}

	public boolean onPrepareOptionsMenu(android.view.Menu param_0) {
		
		if ((__ho18 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onPrepareOptionsMenu", params);
		} else {
			return super.onPrepareOptionsMenu(param_0);
		}
	}

	public boolean onPreparePanel(int param_0, android.view.View param_1, android.view.Menu param_2) {
		
		if ((__ho18 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onPreparePanel", params);
		} else {
			return super.onPreparePanel(param_0, param_1, param_2);
		}
	}

	protected void onRestart() {
		
		if ((__ho18 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onRestart", params);
		} else {
			super.onRestart();
		}
	}

	protected void onRestoreInstanceState(android.os.Bundle param_0) {
		
		if ((__ho18 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onRestoreInstanceState", params);
		} else {
			super.onRestoreInstanceState(param_0);
		}
	}

	protected void onResume() {
		
		if ((__ho18 & (1 << 4)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onResume", params);
		} else {
			super.onResume();
		}
	}

	public java.lang.Object onRetainNonConfigurationInstance() {
		
		if ((__ho18 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "onRetainNonConfigurationInstance", params);
		} else {
			return super.onRetainNonConfigurationInstance();
		}
	}

	protected void onSaveInstanceState(android.os.Bundle param_0) {
		
		if ((__ho18 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onSaveInstanceState", params);
		} else {
			super.onSaveInstanceState(param_0);
		}
	}

	public boolean onSearchRequested() {
		
		if ((__ho18 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onSearchRequested", params);
		} else {
			return super.onSearchRequested();
		}
	}

	protected void onStart() {
		
		if ((__ho19 & (1 << 0)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onStart", params);
		} else {
			super.onStart();
		}
	}

	protected void onStop() {
		
		if ((__ho19 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onStop", params);
		} else {
			super.onStop();
		}
	}

	protected void onTitleChanged(java.lang.CharSequence param_0, int param_1) {
		
		if ((__ho19 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onTitleChanged", params);
		} else {
			super.onTitleChanged(param_0, param_1);
		}
	}

	public boolean onTouchEvent(android.view.MotionEvent param_0) {
		
		if ((__ho19 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onTouchEvent", params);
		} else {
			return super.onTouchEvent(param_0);
		}
	}

	public boolean onTrackballEvent(android.view.MotionEvent param_0) {
		
		if ((__ho19 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onTrackballEvent", params);
		} else {
			return super.onTrackballEvent(param_0);
		}
	}

	public void onTrimMemory(int param_0) {
		
		if ((__ho19 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onTrimMemory", params);
		} else {
			super.onTrimMemory(param_0);
		}
	}

	public void onUserInteraction() {
		
		if ((__ho19 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onUserInteraction", params);
		} else {
			super.onUserInteraction();
		}
	}

	protected void onUserLeaveHint() {
		
		if ((__ho19 & (1 << 7)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "onUserLeaveHint", params);
		} else {
			super.onUserLeaveHint();
		}
	}

	public void onWindowAttributesChanged(android.view.WindowManager.LayoutParams param_0) {
		
		if ((__ho20 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onWindowAttributesChanged", params);
		} else {
			super.onWindowAttributesChanged(param_0);
		}
	}

	public void onWindowFocusChanged(boolean param_0) {
		
		if ((__ho20 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onWindowFocusChanged", params);
		} else {
			super.onWindowFocusChanged(param_0);
		}
	}

	public android.view.ActionMode onWindowStartingActionMode(android.view.ActionMode.Callback param_0) {
		
		if ((__ho20 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.view.ActionMode)com.tns.Platform.callJSMethod(this, "onWindowStartingActionMode", params);
		} else {
			return super.onWindowStartingActionMode(param_0);
		}
	}

	public void openContextMenu(android.view.View param_0) {
		
		if ((__ho20 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "openContextMenu", params);
		} else {
			super.openContextMenu(param_0);
		}
	}

	public java.io.FileInputStream openFileInput(java.lang.String param_0) throws java.io.FileNotFoundException {
		
		if ((__ho20 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (java.io.FileInputStream)com.tns.Platform.callJSMethod(this, "openFileInput", params);
		} else {
			return super.openFileInput(param_0);
		}
	}

	public java.io.FileOutputStream openFileOutput(java.lang.String param_0, int param_1) throws java.io.FileNotFoundException {
		
		if ((__ho20 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (java.io.FileOutputStream)com.tns.Platform.callJSMethod(this, "openFileOutput", params);
		} else {
			return super.openFileOutput(param_0, param_1);
		}
	}

	public void openOptionsMenu() {
		
		if ((__ho20 & (1 << 6)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "openOptionsMenu", params);
		} else {
			super.openOptionsMenu();
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2, android.database.DatabaseErrorHandler param_3) {
		
		if ((__ho20 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (android.database.sqlite.SQLiteDatabase)com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", params);
		} else {
			return super.openOrCreateDatabase(param_0, param_1, param_2, param_3);
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2) {
		
		if ((__ho20 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (android.database.sqlite.SQLiteDatabase)com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", params);
		} else {
			return super.openOrCreateDatabase(param_0, param_1, param_2);
		}
	}

	public void overridePendingTransition(int param_0, int param_1) {
		
		if ((__ho21 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "overridePendingTransition", params);
		} else {
			super.overridePendingTransition(param_0, param_1);
		}
	}

	public android.graphics.drawable.Drawable peekWallpaper() {
		
		if ((__ho21 & (1 << 1)) > 0) { 
			java.lang.Object[] params = null;
			return (android.graphics.drawable.Drawable)com.tns.Platform.callJSMethod(this, "peekWallpaper", params);
		} else {
			return super.peekWallpaper();
		}
	}

	public void recreate() {
		
		if ((__ho21 & (1 << 2)) > 0) { 
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "recreate", params);
		} else {
			super.recreate();
		}
	}

	public void registerComponentCallbacks(android.content.ComponentCallbacks param_0) {
		
		if ((__ho21 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerComponentCallbacks", params);
		} else {
			super.registerComponentCallbacks(param_0);
		}
	}

	public void registerForContextMenu(android.view.View param_0) {
		
		if ((__ho21 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerForContextMenu", params);
		} else {
			super.registerForContextMenu(param_0);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1, java.lang.String param_2, android.os.Handler param_3) {
		
		if ((__ho21 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "registerReceiver", params);
		} else {
			return super.registerReceiver(param_0, param_1, param_2, param_3);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1) {
		
		if ((__ho21 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "registerReceiver", params);
		} else {
			return super.registerReceiver(param_0, param_1);
		}
	}

	public void removeStickyBroadcast(android.content.Intent param_0) {
		
		if ((__ho21 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcast", params);
		} else {
			super.removeStickyBroadcast(param_0);
		}
	}

	public void removeStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1) {
		
		if ((__ho21 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcastAsUser", params);
		} else {
			super.removeStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void revokeUriPermission(android.net.Uri param_0, int param_1) {
		
		if ((__ho22 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "revokeUriPermission", params);
		} else {
			super.revokeUriPermission(param_0, param_1);
		}
	}

	public void sendBroadcast(android.content.Intent param_0, java.lang.String param_1) {
		
		if ((__ho22 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", params);
		} else {
			super.sendBroadcast(param_0, param_1);
		}
	}

	public void sendBroadcast(android.content.Intent param_0) {
		
		if ((__ho22 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", params);
		} else {
			super.sendBroadcast(param_0);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1) {
		
		if ((__ho22 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", params);
		} else {
			super.sendBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2) {
		
		if ((__ho22 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", params);
		} else {
			super.sendBroadcastAsUser(param_0, param_1, param_2);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1) {
		
		if ((__ho22 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", params);
		} else {
			super.sendOrderedBroadcast(param_0, param_1);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6) {
		
		if ((__ho22 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", params);
		} else {
			super.sendOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void sendOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2, android.content.BroadcastReceiver param_3, android.os.Handler param_4, int param_5, java.lang.String param_6, android.os.Bundle param_7) {
		
		if ((__ho22 & (1 << 4)) > 0) { 
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
		} else {
			super.sendOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7);
		}
	}

	public void sendStickyBroadcast(android.content.Intent param_0) {
		
		if ((__ho22 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcast", params);
		} else {
			super.sendStickyBroadcast(param_0);
		}
	}

	public void sendStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1) {
		
		if ((__ho22 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcastAsUser", params);
		} else {
			super.sendStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendStickyOrderedBroadcast(android.content.Intent param_0, android.content.BroadcastReceiver param_1, android.os.Handler param_2, int param_3, java.lang.String param_4, android.os.Bundle param_5) {
		
		if ((__ho22 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcast", params);
		} else {
			super.sendStickyOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void sendStickyOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6) {
		
		if ((__ho23 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcastAsUser", params);
		} else {
			super.sendStickyOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void setContentView(android.view.View param_0) {
		
		if ((__ho23 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setContentView", params);
		} else {
			super.setContentView(param_0);
		}
	}

	public void setContentView(android.view.View param_0, android.view.ViewGroup.LayoutParams param_1) {
		
		if ((__ho23 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "setContentView", params);
		} else {
			super.setContentView(param_0, param_1);
		}
	}

	public void setContentView(int param_0) {
		
		if ((__ho23 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setContentView", params);
		} else {
			super.setContentView(param_0);
		}
	}

	public void setFinishOnTouchOutside(boolean param_0) {
		
		if ((__ho23 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setFinishOnTouchOutside", params);
		} else {
			super.setFinishOnTouchOutside(param_0);
		}
	}

	public void setIntent(android.content.Intent param_0) {
		
		if ((__ho23 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setIntent", params);
		} else {
			super.setIntent(param_0);
		}
	}

	public void setRequestedOrientation(int param_0) {
		
		if ((__ho23 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setRequestedOrientation", params);
		} else {
			super.setRequestedOrientation(param_0);
		}
	}

	public void setTheme(int param_0) {
		if ((__ho23 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTheme", params);
		} else {
			super.setTheme(param_0);
		}
	}

	public void setTitle(java.lang.CharSequence param_0) {
		
		if ((__ho23 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTitle", params);
		} else {
			super.setTitle(param_0);
		}
	}

	public void setTitle(int param_0) {
		
		if ((__ho23 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTitle", params);
		} else {
			super.setTitle(param_0);
		}
	}

	public void setTitleColor(int param_0) {
		
		if ((__ho23 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTitleColor", params);
		} else {
			super.setTitleColor(param_0);
		}
	}

	public void setVisible(boolean param_0) {
		
		if ((__ho24 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setVisible", params);
		} else {
			super.setVisible(param_0);
		}
	}

	public void setWallpaper(android.graphics.Bitmap param_0) throws java.io.IOException {
		
		if ((__ho24 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", params);
		} else {
			super.setWallpaper(param_0);
		}
	}

	public void setWallpaper(java.io.InputStream param_0) throws java.io.IOException {
		
		if ((__ho24 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", params);
		} else {
			super.setWallpaper(param_0);
		}
	}

	public boolean shouldUpRecreateTask(android.content.Intent param_0) {
		
		if ((__ho24 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "shouldUpRecreateTask", params);
		} else {
			return super.shouldUpRecreateTask(param_0);
		}
	}

	public android.view.ActionMode startActionMode(android.view.ActionMode.Callback param_0) {
		
		if ((__ho24 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.view.ActionMode)com.tns.Platform.callJSMethod(this, "startActionMode", params);
		} else {
			return super.startActionMode(param_0);
		}
	}

	public void startActivities(android.content.Intent[] param_0, android.os.Bundle param_1) {
		
		if ((__ho24 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivities", params);
		} else {
			super.startActivities(param_0, param_1);
		}
	}

	public void startActivities(android.content.Intent[] param_0) {
		
		if ((__ho24 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivities", params);
		} else {
			super.startActivities(param_0);
		}
	}

	public void startActivity(android.content.Intent param_0) {
		
		if ((__ho24 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivity", params);
		} else {
			super.startActivity(param_0);
		}
	}

	public void startActivity(android.content.Intent param_0, android.os.Bundle param_1) {
		
		if ((__ho24 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivity", params);
		} else {
			super.startActivity(param_0, param_1);
		}
	}

	public void startActivityForResult(android.content.Intent param_0, int param_1, android.os.Bundle param_2) {
		
		if ((__ho24 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "startActivityForResult", params);
		} else {
			super.startActivityForResult(param_0, param_1, param_2);
		}
	}

	public void startActivityForResult(android.content.Intent param_0, int param_1) {
		
		if ((__ho24 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivityForResult", params);
		} else {
			super.startActivityForResult(param_0, param_1);
		}
	}

	public void startActivityFromChild(android.app.Activity param_0, android.content.Intent param_1, int param_2) {
		
		if ((__ho24 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "startActivityFromChild", params);
		} else {
			super.startActivityFromChild(param_0, param_1, param_2);
		}
	}

	public void startActivityFromChild(android.app.Activity param_0, android.content.Intent param_1, int param_2, android.os.Bundle param_3) {
		
		if ((__ho24 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			com.tns.Platform.callJSMethod(this, "startActivityFromChild", params);
		} else {
			super.startActivityFromChild(param_0, param_1, param_2, param_3);
		}
	}

	public void startActivityFromFragment(android.app.Fragment param_0, android.content.Intent param_1, int param_2) {
		
		if ((__ho25 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			com.tns.Platform.callJSMethod(this, "startActivityFromFragment", params);
		} else {
			super.startActivityFromFragment(param_0, param_1, param_2);
		}
	}

	public void startActivityFromFragment(android.app.Fragment param_0, android.content.Intent param_1, int param_2, android.os.Bundle param_3) {
		
		if ((__ho25 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			com.tns.Platform.callJSMethod(this, "startActivityFromFragment", params);
		} else {
			super.startActivityFromFragment(param_0, param_1, param_2, param_3);
		}
	}

	public boolean startActivityIfNeeded(android.content.Intent param_0, int param_1) {
		
		if ((__ho25 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startActivityIfNeeded", params);
		} else {
			return super.startActivityIfNeeded(param_0, param_1);
		}
	}

	public boolean startActivityIfNeeded(android.content.Intent param_0, int param_1, android.os.Bundle param_2) {
		
		if ((__ho25 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startActivityIfNeeded", params);
		} else {
			return super.startActivityIfNeeded(param_0, param_1, param_2);
		}
	}

	public boolean startInstrumentation(android.content.ComponentName param_0, java.lang.String param_1, android.os.Bundle param_2) {
		
		if ((__ho25 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[3];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startInstrumentation", params);
		} else {
			return super.startInstrumentation(param_0, param_1, param_2);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4) throws android.content.IntentSender.SendIntentException {
		
		if ((__ho25 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[5];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			com.tns.Platform.callJSMethod(this, "startIntentSender", params);
		} else {
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4, android.os.Bundle param_5) throws android.content.IntentSender.SendIntentException {
		
		if ((__ho25 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			com.tns.Platform.callJSMethod(this, "startIntentSender", params);
		} else {
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void startIntentSenderForResult(android.content.IntentSender param_0, int param_1, android.content.Intent param_2, int param_3, int param_4, int param_5, android.os.Bundle param_6) throws android.content.IntentSender.SendIntentException {
		
		if ((__ho25 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "startIntentSenderForResult", params);
		} else {
			super.startIntentSenderForResult(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void startIntentSenderForResult(android.content.IntentSender param_0, int param_1, android.content.Intent param_2, int param_3, int param_4, int param_5) throws android.content.IntentSender.SendIntentException {
		
		if ((__ho25 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[6];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			com.tns.Platform.callJSMethod(this, "startIntentSenderForResult", params);
		} else {
			super.startIntentSenderForResult(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void startIntentSenderFromChild(android.app.Activity param_0, android.content.IntentSender param_1, int param_2, android.content.Intent param_3, int param_4, int param_5, int param_6) throws android.content.IntentSender.SendIntentException {
		
		if ((__ho25 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[7];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			com.tns.Platform.callJSMethod(this, "startIntentSenderFromChild", params);
		} else {
			super.startIntentSenderFromChild(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void startIntentSenderFromChild(android.app.Activity param_0, android.content.IntentSender param_1, int param_2, android.content.Intent param_3, int param_4, int param_5, int param_6, android.os.Bundle param_7) throws android.content.IntentSender.SendIntentException {
		
		if ((__ho25 & (1 << 5)) > 0) { 
			java.lang.Object[] params = new Object[8];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			params[4] = param_4;
			params[5] = param_5;
			params[6] = param_6;
			params[7] = param_7;
			com.tns.Platform.callJSMethod(this, "startIntentSenderFromChild", params);
		} else {
			super.startIntentSenderFromChild(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7);
		}
	}

	public void startManagingCursor(android.database.Cursor param_0) {
		
		if ((__ho25 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startManagingCursor", params);
		} else {
			super.startManagingCursor(param_0);
		}
	}

	public boolean startNextMatchingActivity(android.content.Intent param_0) {
		
		if ((__ho25 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startNextMatchingActivity", params);
		} else {
			return super.startNextMatchingActivity(param_0);
		}
	}

	public boolean startNextMatchingActivity(android.content.Intent param_0, android.os.Bundle param_1) {
		
		if ((__ho25 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startNextMatchingActivity", params);
		} else {
			return super.startNextMatchingActivity(param_0, param_1);
		}
	}

	public void startSearch(java.lang.String param_0, boolean param_1, android.os.Bundle param_2, boolean param_3) {
		
		if ((__ho26 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[4];
			params[0] = param_0;
			params[1] = param_1;
			params[2] = param_2;
			params[3] = param_3;
			com.tns.Platform.callJSMethod(this, "startSearch", params);
		} else {
			super.startSearch(param_0, param_1, param_2, param_3);
		}
	}

	public android.content.ComponentName startService(android.content.Intent param_0) {
		
		if ((__ho26 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (android.content.ComponentName)com.tns.Platform.callJSMethod(this, "startService", params);
		} else {
			return super.startService(param_0);
		}
	}

	public void stopManagingCursor(android.database.Cursor param_0) {
		
		if ((__ho26 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "stopManagingCursor", params);
		} else {
			super.stopManagingCursor(param_0);
		}
	}

	public boolean stopService(android.content.Intent param_0) {
		
		if ((__ho26 & (1 << 3)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "stopService", params);
		} else {
			return super.stopService(param_0);
		}
	}

	public void takeKeyEvents(boolean param_0) {
		
		if ((__ho26 & (1 << 4)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "takeKeyEvents", params);
		} else {
			super.takeKeyEvents(param_0);
		}
	}

	public java.lang.String toString() {
		
		if ((__ho26 & (1 << 5)) > 0) { 
			java.lang.Object[] params = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "toString", params);
		} else {
			return super.toString();
		}
	}

	public void triggerSearch(java.lang.String param_0, android.os.Bundle param_1) {
		
		if ((__ho26 & (1 << 6)) > 0) { 
			java.lang.Object[] params = new Object[2];
			params[0] = param_0;
			params[1] = param_1;
			com.tns.Platform.callJSMethod(this, "triggerSearch", params);
		} else {
			super.triggerSearch(param_0, param_1);
		}
	}

	public void unbindService(android.content.ServiceConnection param_0) {
		
		if ((__ho26 & (1 << 7)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unbindService", params);
		} else {
			super.unbindService(param_0);
		}
	}

	public void unregisterComponentCallbacks(android.content.ComponentCallbacks param_0) {
		
		if ((__ho27 & (1 << 0)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterComponentCallbacks", params);
		} else {
			super.unregisterComponentCallbacks(param_0);
		}
	}

	public void unregisterForContextMenu(android.view.View param_0) {
		
		if ((__ho27 & (1 << 1)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterForContextMenu", params);
		} else {
			super.unregisterForContextMenu(param_0);
		}
	}

	public void unregisterReceiver(android.content.BroadcastReceiver param_0) {
		
		if ((__ho27 & (1 << 2)) > 0) { 
			java.lang.Object[] params = new Object[1];
			params[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterReceiver", params);
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
	
	@Override
	public void setNativeScriptOverride(String name)
	{
			if (name.equals("init")) {
				__ctorOverridden = true;
			}
			if (name.equals("addContentView")) {
				__ho0 |= (1 << 0);
			} else if (name.equals("applyOverrideConfiguration")) {
				__ho0 |= (1 << 1);
			} else if (name.equals("attachBaseContext")) {
				__ho0 |= (1 << 2);
			} else if (name.equals("bindService")) {
				__ho0 |= (1 << 3);
			} else if (name.equals("checkCallingOrSelfPermission")) {
				__ho0 |= (1 << 4);
			} else if (name.equals("checkCallingOrSelfUriPermission")) {
				__ho0 |= (1 << 5);
			} else if (name.equals("checkCallingPermission")) {
				__ho0 |= (1 << 6);
			} else if (name.equals("checkCallingUriPermission")) {
				__ho0 |= (1 << 7);
			} else if (name.equals("checkPermission")) {
				__ho1 |= (1 << 0);
			} else if (name.equals("checkUriPermission")) {
				__ho1 |= (1 << 1);
			} else if (name.equals("clearWallpaper")) {
				__ho1 |= (1 << 2);
			} else if (name.equals("clone")) {
				__ho1 |= (1 << 3);
			} else if (name.equals("closeContextMenu")) {
				__ho1 |= (1 << 4);
			} else if (name.equals("closeOptionsMenu")) {
				__ho1 |= (1 << 5);
			} else if (name.equals("createConfigurationContext")) {
				__ho1 |= (1 << 6);
			} else if (name.equals("createDisplayContext")) {
				__ho1 |= (1 << 7);
			} else if (name.equals("createPackageContext")) {
				__ho2 |= (1 << 0);
			} else if (name.equals("createPendingResult")) {
				__ho2 |= (1 << 1);
			} else if (name.equals("databaseList")) {
				__ho2 |= (1 << 2);
			} else if (name.equals("deleteDatabase")) {
				__ho2 |= (1 << 3);
			} else if (name.equals("deleteFile")) {
				__ho2 |= (1 << 4);
			} else if (name.equals("dispatchGenericMotionEvent")) {
				__ho2 |= (1 << 5);
			} else if (name.equals("dispatchKeyEvent")) {
				__ho2 |= (1 << 6);
			} else if (name.equals("dispatchKeyShortcutEvent")) {
				__ho2 |= (1 << 7);
			} else if (name.equals("dispatchPopulateAccessibilityEvent")) {
				__ho3 |= (1 << 0);
			} else if (name.equals("dispatchTouchEvent")) {
				__ho3 |= (1 << 1);
			} else if (name.equals("dispatchTrackballEvent")) {
				__ho3 |= (1 << 2);
			} else if (name.equals("dump")) {
				__ho3 |= (1 << 3);
			} else if (name.equals("enforceCallingOrSelfPermission")) {
				__ho3 |= (1 << 4);
			} else if (name.equals("enforceCallingOrSelfUriPermission")) {
				__ho3 |= (1 << 5);
			} else if (name.equals("enforceCallingPermission")) {
				__ho3 |= (1 << 6);
			} else if (name.equals("enforceCallingUriPermission")) {
				__ho3 |= (1 << 7);
			} else if (name.equals("enforcePermission")) {
				__ho4 |= (1 << 0);
			} else if (name.equals("enforceUriPermission")) {
				__ho4 |= (1 << 1);
			} else if (name.equals("equals")) {
				__ho4 |= (1 << 2);
			} else if (name.equals("fileList")) {
				__ho4 |= (1 << 3);
			} else if (name.equals("finalize")) {
				__ho4 |= (1 << 4);
			} else if (name.equals("findViewById")) {
				__ho4 |= (1 << 5);
			} else if (name.equals("finish")) {
				__ho4 |= (1 << 6);
			} else if (name.equals("finishActivity")) {
				__ho4 |= (1 << 7);
			} else if (name.equals("finishActivityFromChild")) {
				__ho5 |= (1 << 0);
			} else if (name.equals("finishAffinity")) {
				__ho5 |= (1 << 1);
			} else if (name.equals("finishFromChild")) {
				__ho5 |= (1 << 2);
			} else if (name.equals("getActionBar")) {
				__ho5 |= (1 << 3);
			} else if (name.equals("getApplicationContext")) {
				__ho5 |= (1 << 4);
			} else if (name.equals("getApplicationInfo")) {
				__ho5 |= (1 << 5);
			} else if (name.equals("getAssets")) {
				__ho5 |= (1 << 6);
			} else if (name.equals("getBaseContext")) {
				__ho5 |= (1 << 7);
			} else if (name.equals("getCacheDir")) {
				__ho6 |= (1 << 0);
			} else if (name.equals("getCallingActivity")) {
				__ho6 |= (1 << 1);
			} else if (name.equals("getCallingPackage")) {
				__ho6 |= (1 << 2);
			} else if (name.equals("getChangingConfigurations")) {
				__ho6 |= (1 << 3);
			} else if (name.equals("getClassLoader")) {
				__ho6 |= (1 << 4);
			} else if (name.equals("getComponentName")) {
				__ho6 |= (1 << 5);
			} else if (name.equals("getContentResolver")) {
				__ho6 |= (1 << 6);
			} else if (name.equals("getCurrentFocus")) {
				__ho6 |= (1 << 7);
			} else if (name.equals("getDatabasePath")) {
				__ho7 |= (1 << 0);
			} else if (name.equals("getDir")) {
				__ho7 |= (1 << 1);
			} else if (name.equals("getExternalCacheDir")) {
				__ho7 |= (1 << 2);
			} else if (name.equals("getExternalFilesDir")) {
				__ho7 |= (1 << 3);
			} else if (name.equals("getFileStreamPath")) {
				__ho7 |= (1 << 4);
			} else if (name.equals("getFilesDir")) {
				__ho7 |= (1 << 5);
			} else if (name.equals("getFragmentManager")) {
				__ho7 |= (1 << 6);
			} else if (name.equals("getIntent")) {
				__ho7 |= (1 << 7);
			} else if (name.equals("getLastNonConfigurationInstance")) {
				__ho8 |= (1 << 0);
			} else if (name.equals("getLayoutInflater")) {
				__ho8 |= (1 << 1);
			} else if (name.equals("getLoaderManager")) {
				__ho8 |= (1 << 2);
			} else if (name.equals("getLocalClassName")) {
				__ho8 |= (1 << 3);
			} else if (name.equals("getMainLooper")) {
				__ho8 |= (1 << 4);
			} else if (name.equals("getMenuInflater")) {
				__ho8 |= (1 << 5);
			} else if (name.equals("getObbDir")) {
				__ho8 |= (1 << 6);
			} else if (name.equals("getPackageCodePath")) {
				__ho8 |= (1 << 7);
			} else if (name.equals("getPackageManager")) {
				__ho9 |= (1 << 0);
			} else if (name.equals("getPackageName")) {
				__ho9 |= (1 << 1);
			} else if (name.equals("getPackageResourcePath")) {
				__ho9 |= (1 << 2);
			} else if (name.equals("getParentActivityIntent")) {
				__ho9 |= (1 << 3);
			} else if (name.equals("getPreferences")) {
				__ho9 |= (1 << 4);
			} else if (name.equals("getRequestedOrientation")) {
				__ho9 |= (1 << 5);
			} else if (name.equals("getResources")) {
				__ho9 |= (1 << 6);
			} else if (name.equals("getSharedPreferences")) {
				__ho9 |= (1 << 7);
			} else if (name.equals("getSystemService")) {
				__ho10 |= (1 << 0);
			} else if (name.equals("getTaskId")) {
				__ho10 |= (1 << 1);
			} else if (name.equals("getTheme")) {
				__ho10 |= (1 << 2);
			} else if (name.equals("getWallpaper")) {
				__ho10 |= (1 << 3);
			} else if (name.equals("getWallpaperDesiredMinimumHeight")) {
				__ho10 |= (1 << 4);
			} else if (name.equals("getWallpaperDesiredMinimumWidth")) {
				__ho10 |= (1 << 5);
			} else if (name.equals("getWindow")) {
				__ho10 |= (1 << 6);
			} else if (name.equals("getWindowManager")) {
				__ho10 |= (1 << 7);
			} else if (name.equals("grantUriPermission")) {
				__ho11 |= (1 << 0);
			} else if (name.equals("hasWindowFocus")) {
				__ho11 |= (1 << 1);
			} else if (name.equals("hashCode")) {
				__ho11 |= (1 << 2);
			} else if (name.equals("invalidateOptionsMenu")) {
				__ho11 |= (1 << 3);
			} else if (name.equals("isChangingConfigurations")) {
				__ho11 |= (1 << 4);
			} else if (name.equals("isDestroyed")) {
				__ho11 |= (1 << 5);
			} else if (name.equals("isFinishing")) {
				__ho11 |= (1 << 6);
			} else if (name.equals("isRestricted")) {
				__ho11 |= (1 << 7);
			} else if (name.equals("isTaskRoot")) {
				__ho12 |= (1 << 0);
			} else if (name.equals("moveTaskToBack")) {
				__ho12 |= (1 << 1);
			} else if (name.equals("navigateUpTo")) {
				__ho12 |= (1 << 2);
			} else if (name.equals("navigateUpToFromChild")) {
				__ho12 |= (1 << 3);
			} else if (name.equals("onActionModeFinished")) {
				__ho12 |= (1 << 4);
			} else if (name.equals("onActionModeStarted")) {
				__ho12 |= (1 << 5);
			} else if (name.equals("onActivityResult")) {
				__ho12 |= (1 << 6);
			} else if (name.equals("onApplyThemeResource")) {
				__ho12 |= (1 << 7);
			} else if (name.equals("onAttachFragment")) {
				__ho13 |= (1 << 0);
			} else if (name.equals("onAttachedToWindow")) {
				__ho13 |= (1 << 1);
			} else if (name.equals("onBackPressed")) {
				__ho13 |= (1 << 2);
			} else if (name.equals("onChildTitleChanged")) {
				__ho13 |= (1 << 3);
			} else if (name.equals("onConfigurationChanged")) {
				__ho13 |= (1 << 4);
			} else if (name.equals("onContentChanged")) {
				__ho13 |= (1 << 5);
			} else if (name.equals("onContextItemSelected")) {
				__ho13 |= (1 << 6);
			} else if (name.equals("onContextMenuClosed")) {
				__ho13 |= (1 << 7);
			} else if (name.equals("onCreate")) {
				__ho14 |= (1 << 0);
			} else if (name.equals("onCreateContextMenu")) {
				__ho14 |= (1 << 1);
			} else if (name.equals("onCreateDescription")) {
				__ho14 |= (1 << 2);
			} else if (name.equals("onCreateDialog")) {
				__ho14 |= (1 << 3);
			} else if (name.equals("onCreateNavigateUpTaskStack")) {
				__ho14 |= (1 << 4);
			} else if (name.equals("onCreateOptionsMenu")) {
				__ho14 |= (1 << 5);
			} else if (name.equals("onCreatePanelMenu")) {
				__ho14 |= (1 << 6);
			} else if (name.equals("onCreatePanelView")) {
				__ho14 |= (1 << 7);
			} else if (name.equals("onCreateThumbnail")) {
				__ho15 |= (1 << 0);
			} else if (name.equals("onCreateView")) {
				__ho15 |= (1 << 1);
			} else if (name.equals("onDestroy")) {
				__ho15 |= (1 << 2);
			} else if (name.equals("onDetachedFromWindow")) {
				__ho15 |= (1 << 3);
			} else if (name.equals("onGenericMotionEvent")) {
				__ho15 |= (1 << 4);
			} else if (name.equals("onKeyDown")) {
				__ho15 |= (1 << 5);
			} else if (name.equals("onKeyLongPress")) {
				__ho15 |= (1 << 6);
			} else if (name.equals("onKeyMultiple")) {
				__ho15 |= (1 << 7);
			} else if (name.equals("onKeyShortcut")) {
				__ho16 |= (1 << 0);
			} else if (name.equals("onKeyUp")) {
				__ho16 |= (1 << 1);
			} else if (name.equals("onLowMemory")) {
				__ho16 |= (1 << 2);
			} else if (name.equals("onMenuItemSelected")) {
				__ho16 |= (1 << 3);
			} else if (name.equals("onMenuOpened")) {
				__ho16 |= (1 << 4);
			} else if (name.equals("onNavigateUp")) {
				__ho16 |= (1 << 5);
			} else if (name.equals("onNavigateUpFromChild")) {
				__ho16 |= (1 << 6);
			} else if (name.equals("onNewIntent")) {
				__ho16 |= (1 << 7);
			} else if (name.equals("onOptionsItemSelected")) {
				__ho17 |= (1 << 0);
			} else if (name.equals("onOptionsMenuClosed")) {
				__ho17 |= (1 << 1);
			} else if (name.equals("onPanelClosed")) {
				__ho17 |= (1 << 2);
			} else if (name.equals("onPause")) {
				__ho17 |= (1 << 3);
			} else if (name.equals("onPostCreate")) {
				__ho17 |= (1 << 4);
			} else if (name.equals("onPostResume")) {
				__ho17 |= (1 << 5);
			} else if (name.equals("onPrepareDialog")) {
				__ho17 |= (1 << 6);
			} else if (name.equals("onPrepareNavigateUpTaskStack")) {
				__ho17 |= (1 << 7);
			} else if (name.equals("onPrepareOptionsMenu")) {
				__ho18 |= (1 << 0);
			} else if (name.equals("onPreparePanel")) {
				__ho18 |= (1 << 1);
			} else if (name.equals("onRestart")) {
				__ho18 |= (1 << 2);
			} else if (name.equals("onRestoreInstanceState")) {
				__ho18 |= (1 << 3);
			} else if (name.equals("onResume")) {
				__ho18 |= (1 << 4);
			} else if (name.equals("onRetainNonConfigurationInstance")) {
				__ho18 |= (1 << 5);
			} else if (name.equals("onSaveInstanceState")) {
				__ho18 |= (1 << 6);
			} else if (name.equals("onSearchRequested")) {
				__ho18 |= (1 << 7);
			} else if (name.equals("onStart")) {
				__ho19 |= (1 << 0);
			} else if (name.equals("onStop")) {
				__ho19 |= (1 << 1);
			} else if (name.equals("onTitleChanged")) {
				__ho19 |= (1 << 2);
			} else if (name.equals("onTouchEvent")) {
				__ho19 |= (1 << 3);
			} else if (name.equals("onTrackballEvent")) {
				__ho19 |= (1 << 4);
			} else if (name.equals("onTrimMemory")) {
				__ho19 |= (1 << 5);
			} else if (name.equals("onUserInteraction")) {
				__ho19 |= (1 << 6);
			} else if (name.equals("onUserLeaveHint")) {
				__ho19 |= (1 << 7);
			} else if (name.equals("onWindowAttributesChanged")) {
				__ho20 |= (1 << 0);
			} else if (name.equals("onWindowFocusChanged")) {
				__ho20 |= (1 << 1);
			} else if (name.equals("onWindowStartingActionMode")) {
				__ho20 |= (1 << 2);
			} else if (name.equals("openContextMenu")) {
				__ho20 |= (1 << 3);
			} else if (name.equals("openFileInput")) {
				__ho20 |= (1 << 4);
			} else if (name.equals("openFileOutput")) {
				__ho20 |= (1 << 5);
			} else if (name.equals("openOptionsMenu")) {
				__ho20 |= (1 << 6);
			} else if (name.equals("openOrCreateDatabase")) {
				__ho20 |= (1 << 7);
			} else if (name.equals("overridePendingTransition")) {
				__ho21 |= (1 << 0);
			} else if (name.equals("peekWallpaper")) {
				__ho21 |= (1 << 1);
			} else if (name.equals("recreate")) {
				__ho21 |= (1 << 2);
			} else if (name.equals("registerComponentCallbacks")) {
				__ho21 |= (1 << 3);
			} else if (name.equals("registerForContextMenu")) {
				__ho21 |= (1 << 4);
			} else if (name.equals("registerReceiver")) {
				__ho21 |= (1 << 5);
			} else if (name.equals("removeStickyBroadcast")) {
				__ho21 |= (1 << 6);
			} else if (name.equals("removeStickyBroadcastAsUser")) {
				__ho21 |= (1 << 7);
			} else if (name.equals("revokeUriPermission")) {
				__ho22 |= (1 << 0);
			} else if (name.equals("sendBroadcast")) {
				__ho22 |= (1 << 1);
			} else if (name.equals("sendBroadcastAsUser")) {
				__ho22 |= (1 << 2);
			} else if (name.equals("sendOrderedBroadcast")) {
				__ho22 |= (1 << 3);
			} else if (name.equals("sendOrderedBroadcastAsUser")) {
				__ho22 |= (1 << 4);
			} else if (name.equals("sendStickyBroadcast")) {
				__ho22 |= (1 << 5);
			} else if (name.equals("sendStickyBroadcastAsUser")) {
				__ho22 |= (1 << 6);
			} else if (name.equals("sendStickyOrderedBroadcast")) {
				__ho22 |= (1 << 7);
			} else if (name.equals("sendStickyOrderedBroadcastAsUser")) {
				__ho23 |= (1 << 0);
			} else if (name.equals("setContentView")) {
				__ho23 |= (1 << 1);
			} else if (name.equals("setFinishOnTouchOutside")) {
				__ho23 |= (1 << 2);
			} else if (name.equals("setIntent")) {
				__ho23 |= (1 << 3);
			} else if (name.equals("setRequestedOrientation")) {
				__ho23 |= (1 << 4);
			} else if (name.equals("setTheme")) {
				__ho23 |= (1 << 5);
			} else if (name.equals("setTitle")) {
				__ho23 |= (1 << 6);
			} else if (name.equals("setTitleColor")) {
				__ho23 |= (1 << 7);
			} else if (name.equals("setVisible")) {
				__ho24 |= (1 << 0);
			} else if (name.equals("setWallpaper")) {
				__ho24 |= (1 << 1);
			} else if (name.equals("shouldUpRecreateTask")) {
				__ho24 |= (1 << 2);
			} else if (name.equals("startActionMode")) {
				__ho24 |= (1 << 3);
			} else if (name.equals("startActivities")) {
				__ho24 |= (1 << 4);
			} else if (name.equals("startActivity")) {
				__ho24 |= (1 << 5);
			} else if (name.equals("startActivityForResult")) {
				__ho24 |= (1 << 6);
			} else if (name.equals("startActivityFromChild")) {
				__ho24 |= (1 << 7);
			} else if (name.equals("startActivityFromFragment")) {
				__ho25 |= (1 << 0);
			} else if (name.equals("startActivityIfNeeded")) {
				__ho25 |= (1 << 1);
			} else if (name.equals("startInstrumentation")) {
				__ho25 |= (1 << 2);
			} else if (name.equals("startIntentSender")) {
				__ho25 |= (1 << 3);
			} else if (name.equals("startIntentSenderForResult")) {
				__ho25 |= (1 << 4);
			} else if (name.equals("startIntentSenderFromChild")) {
				__ho25 |= (1 << 5);
			} else if (name.equals("startManagingCursor")) {
				__ho25 |= (1 << 6);
			} else if (name.equals("startNextMatchingActivity")) {
				__ho25 |= (1 << 7);
			} else if (name.equals("startSearch")) {
				__ho26 |= (1 << 0);
			} else if (name.equals("startService")) {
				__ho26 |= (1 << 1);
			} else if (name.equals("stopManagingCursor")) {
				__ho26 |= (1 << 2);
			} else if (name.equals("stopService")) {
				__ho26 |= (1 << 3);
			} else if (name.equals("takeKeyEvents")) {
				__ho26 |= (1 << 4);
			} else if (name.equals("toString")) {
				__ho26 |= (1 << 5);
			} else if (name.equals("triggerSearch")) {
				__ho26 |= (1 << 6);
			} else if (name.equals("unbindService")) {
				__ho26 |= (1 << 7);
			} else if (name.equals("unregisterComponentCallbacks")) {
				__ho27 |= (1 << 0);
			} else if (name.equals("unregisterForContextMenu")) {
				__ho27 |= (1 << 1);
			} else if (name.equals("unregisterReceiver")) {
				__ho27 |= (1 << 2);
			}
		
	}
	private boolean __initialized;
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
	private byte __ho13;
	private byte __ho14;
	private byte __ho15;
	private byte __ho16;
	private byte __ho17;
	private byte __ho18;
	private byte __ho19;
	private byte __ho20;
	private byte __ho21;
	private byte __ho22;
	private byte __ho23;
	private byte __ho24;
	private byte __ho25;
	private byte __ho26;
	private byte __ho27;
	private byte __ho28;
}

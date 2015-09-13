package com.tns;

public class NativeScriptActivity extends android.app.Activity
{
	private final int objectId;
	
	public NativeScriptActivity()
	{
		com.tns.Platform.initInstance(this);
		objectId = com.tns.Platform.getOrCreateJavaObjectID(this);
	}
	
	public void addContentView(android.view.View param_0, android.view.ViewGroup.LayoutParams param_1)
	{
		if (__ho0)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "addContentView", void.class, args);
		}
		else
		{
			super.addContentView(param_0, param_1);
		}
	}

	public void applyOverrideConfiguration(android.content.res.Configuration param_0)
	{
		if (__ho1)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "applyOverrideConfiguration", void.class, args);
		}
		else
		{
			super.applyOverrideConfiguration(param_0);
		}
	}

	public boolean bindService(android.content.Intent param_0, android.content.ServiceConnection param_1, int param_2)
	{
		if (__ho2)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "bindService", boolean.class, args);
		}
		else
		{
			return super.bindService(param_0, param_1, param_2);
		}
	}

	public int checkCallingOrSelfPermission(java.lang.String param_0)
	{
		if (__ho3)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingOrSelfPermission", int.class, args);
		}
		else
		{
			return super.checkCallingOrSelfPermission(param_0);
		}
	}

	public int checkCallingOrSelfUriPermission(android.net.Uri param_0, int param_1)
	{
		if (__ho4)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingOrSelfUriPermission", int.class, args);
		}
		else
		{
			return super.checkCallingOrSelfUriPermission(param_0, param_1);
		}
	}

	public int checkCallingPermission(java.lang.String param_0)
	{
		if (__ho5)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingPermission", int.class, args);
		}
		else
		{
			return super.checkCallingPermission(param_0);
		}
	}

	public int checkCallingUriPermission(android.net.Uri param_0, int param_1)
	{
		if (__ho6)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkCallingUriPermission", int.class, args);
		}
		else
		{
			return super.checkCallingUriPermission(param_0, param_1);
		}
	}

	public int checkPermission(java.lang.String param_0, int param_1, int param_2)
	{
		if (__ho7)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkPermission", int.class, args);
		}
		else
		{
			return super.checkPermission(param_0, param_1, param_2);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5)
	{
		if (__ho8)
		{
			Object[] args = new Object[6];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkUriPermission", int.class, args);
		}
		else
		{
			return super.checkUriPermission(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public int checkUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3)
	{
		if (__ho8)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			return (Integer)com.tns.Platform.callJSMethod(this, "checkUriPermission", int.class, args);
		}
		else
		{
			return super.checkUriPermission(param_0, param_1, param_2, param_3);
		}
	}

	public void clearWallpaper() throws java.io.IOException
	{
		if (__ho9)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "clearWallpaper", void.class, args);
		}
		else
		{
			super.clearWallpaper();
		}
	}

	public void closeContextMenu()
	{
		if (__ho10)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "closeContextMenu", void.class, args);
		}
		else
		{
			super.closeContextMenu();
		}
	}

	public void closeOptionsMenu()
	{
		if (__ho11)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "closeOptionsMenu", void.class, args);
		}
		else
		{
			super.closeOptionsMenu();
		}
	}

	public android.content.Context createConfigurationContext(android.content.res.Configuration param_0)
	{
		if (__ho12)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createConfigurationContext", android.content.Context.class, args);
		}
		else
		{
			return super.createConfigurationContext(param_0);
		}
	}

	public android.content.Context createDisplayContext(android.view.Display param_0)
	{
		if (__ho13)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createDisplayContext", android.content.Context.class, args);
		}
		else
		{
			return super.createDisplayContext(param_0);
		}
	}

	public android.content.Context createPackageContext(java.lang.String param_0, int param_1) throws android.content.pm.PackageManager.NameNotFoundException
	{
		if (__ho14)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "createPackageContext", android.content.Context.class, args);
		}
		else
		{
			return super.createPackageContext(param_0, param_1);
		}
	}

	public android.app.PendingIntent createPendingResult(int param_0, android.content.Intent param_1, int param_2)
	{
		if (__ho15)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (android.app.PendingIntent)com.tns.Platform.callJSMethod(this, "createPendingResult", android.app.PendingIntent.class, args);
		}
		else
		{
			return super.createPendingResult(param_0, param_1, param_2);
		}
	}

	public java.lang.String[] databaseList()
	{
		if (__ho16)
		{
			Object[] args = null;
			return (java.lang.String[])com.tns.Platform.callJSMethod(this, "databaseList", java.lang.String[].class, args);
		}
		else
		{
			return super.databaseList();
		}
	}

	public boolean deleteDatabase(java.lang.String param_0)
	{
		if (__ho17)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "deleteDatabase", boolean.class, args);
		}
		else
		{
			return super.deleteDatabase(param_0);
		}
	}

	public boolean deleteFile(java.lang.String param_0)
	{
		if (__ho18)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "deleteFile", boolean.class, args);
		}
		else
		{
			return super.deleteFile(param_0);
		}
	}

	public boolean dispatchGenericMotionEvent(android.view.MotionEvent param_0)
	{
		if (__ho19)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchGenericMotionEvent", boolean.class, args);
		}
		else
		{
			return super.dispatchGenericMotionEvent(param_0);
		}
	}

	public boolean dispatchKeyEvent(android.view.KeyEvent param_0)
	{
		if (__ho20)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchKeyEvent", boolean.class, args);
		}
		else
		{
			return super.dispatchKeyEvent(param_0);
		}
	}

	public boolean dispatchKeyShortcutEvent(android.view.KeyEvent param_0)
	{
		if (__ho21)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchKeyShortcutEvent", boolean.class, args);
		}
		else
		{
			return super.dispatchKeyShortcutEvent(param_0);
		}
	}

	public boolean dispatchPopulateAccessibilityEvent(android.view.accessibility.AccessibilityEvent param_0)
	{
		if (__ho22)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchPopulateAccessibilityEvent", boolean.class, args);
		}
		else
		{
			return super.dispatchPopulateAccessibilityEvent(param_0);
		}
	}

	public boolean dispatchTouchEvent(android.view.MotionEvent param_0)
	{
		if (__ho23)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchTouchEvent", boolean.class, args);
		}
		else
		{
			return super.dispatchTouchEvent(param_0);
		}
	}

	public boolean dispatchTrackballEvent(android.view.MotionEvent param_0)
	{
		if (__ho24)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "dispatchTrackballEvent", boolean.class, args);
		}
		else
		{
			return super.dispatchTrackballEvent(param_0);
		}
	}

	public void dump(java.lang.String param_0, java.io.FileDescriptor param_1, java.io.PrintWriter param_2, java.lang.String[] param_3)
	{
		if (__ho25)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			com.tns.Platform.callJSMethod(this, "dump", void.class, args);
		}
		else
		{
			super.dump(param_0, param_1, param_2, param_3);
		}
	}

	public void enforceCallingOrSelfPermission(java.lang.String param_0, java.lang.String param_1)
	{
		if (__ho26)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfPermission", void.class, args);
		}
		else
		{
			super.enforceCallingOrSelfPermission(param_0, param_1);
		}
	}

	public void enforceCallingOrSelfUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2)
	{
		if (__ho27)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingOrSelfUriPermission", void.class, args);
		}
		else
		{
			super.enforceCallingOrSelfUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforceCallingPermission(java.lang.String param_0, java.lang.String param_1)
	{
		if (__ho28)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "enforceCallingPermission", void.class, args);
		}
		else
		{
			super.enforceCallingPermission(param_0, param_1);
		}
	}

	public void enforceCallingUriPermission(android.net.Uri param_0, int param_1, java.lang.String param_2)
	{
		if (__ho29)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "enforceCallingUriPermission", void.class, args);
		}
		else
		{
			super.enforceCallingUriPermission(param_0, param_1, param_2);
		}
	}

	public void enforcePermission(java.lang.String param_0, int param_1, int param_2, java.lang.String param_3)
	{
		if (__ho30)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			com.tns.Platform.callJSMethod(this, "enforcePermission", void.class, args);
		}
		else
		{
			super.enforcePermission(param_0, param_1, param_2, param_3);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, java.lang.String param_1, java.lang.String param_2, int param_3, int param_4, int param_5, java.lang.String param_6)
	{
		if (__ho31)
		{
			Object[] args = new Object[7];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			args[6] = param_6;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", void.class, args);
		}
		else
		{
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void enforceUriPermission(android.net.Uri param_0, int param_1, int param_2, int param_3, java.lang.String param_4)
	{
		if (__ho31)
		{
			Object[] args = new Object[5];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			com.tns.Platform.callJSMethod(this, "enforceUriPermission", void.class, args);
		}
		else
		{
			super.enforceUriPermission(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public boolean equals(java.lang.Object param_0)
	{
		if (__ho32)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "equals", boolean.class, args);
		}
		else
		{
			return super.equals(param_0);
		}
	}

	public java.lang.String[] fileList()
	{
		if (__ho33)
		{
			Object[] args = null;
			return (java.lang.String[])com.tns.Platform.callJSMethod(this, "fileList", java.lang.String[].class, args);
		}
		else
		{
			return super.fileList();
		}
	}

	public android.view.View findViewById(int param_0)
	{
		if (__ho34)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "findViewById", android.view.View.class, args);
		}
		else
		{
			return super.findViewById(param_0);
		}
	}

	public void finish()
	{
		if (__ho35)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "finish", void.class, args);
		}
		else
		{
			super.finish();
		}
	}

	public void finishActivity(int param_0)
	{
		if (__ho36)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "finishActivity", void.class, args);
		}
		else
		{
			super.finishActivity(param_0);
		}
	}

	public void finishActivityFromChild(android.app.Activity param_0, int param_1)
	{
		if (__ho37)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "finishActivityFromChild", void.class, args);
		}
		else
		{
			super.finishActivityFromChild(param_0, param_1);
		}
	}

	public void finishAffinity()
	{
		if (__ho38)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "finishAffinity", void.class, args);
		}
		else
		{
			super.finishAffinity();
		}
	}

	public void finishFromChild(android.app.Activity param_0)
	{
		if (__ho39)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "finishFromChild", void.class, args);
		}
		else
		{
			super.finishFromChild(param_0);
		}
	}

	public android.app.ActionBar getActionBar()
	{
		if (__ho40)
		{
			Object[] args = null;
			return (android.app.ActionBar)com.tns.Platform.callJSMethod(this, "getActionBar", android.app.ActionBar.class, args);
		}
		else
		{
			return super.getActionBar();
		}
	}

	public android.content.Context getApplicationContext()
	{
		if (__ho41)
		{
			Object[] args = null;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "getApplicationContext", android.content.Context.class, args);
		}
		else
		{
			return super.getApplicationContext();
		}
	}

	public android.content.pm.ApplicationInfo getApplicationInfo()
	{
		if (__ho42)
		{
			Object[] args = null;
			return (android.content.pm.ApplicationInfo)com.tns.Platform.callJSMethod(this, "getApplicationInfo", android.content.pm.ApplicationInfo.class, args);
		}
		else
		{
			return super.getApplicationInfo();
		}
	}

	public android.content.res.AssetManager getAssets()
	{
		if (__ho43)
		{
			Object[] args = null;
			return (android.content.res.AssetManager)com.tns.Platform.callJSMethod(this, "getAssets", android.content.res.AssetManager.class, args);
		}
		else
		{
			return super.getAssets();
		}
	}

	public android.content.Context getBaseContext()
	{
		if (__ho44)
		{
			Object[] args = null;
			return (android.content.Context)com.tns.Platform.callJSMethod(this, "getBaseContext", android.content.Context.class, args);
		}
		else
		{
			return super.getBaseContext();
		}
	}

	public java.io.File getCacheDir()
	{
		if (__ho45)
		{
			Object[] args = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getCacheDir", java.io.File.class, args);
		}
		else
		{
			return super.getCacheDir();
		}
	}

	public android.content.ComponentName getCallingActivity()
	{
		if (__ho46)
		{
			Object[] args = null;
			return (android.content.ComponentName)com.tns.Platform.callJSMethod(this, "getCallingActivity", android.content.ComponentName.class, args);
		}
		else
		{
			return super.getCallingActivity();
		}
	}

	public java.lang.String getCallingPackage()
	{
		if (__ho47)
		{
			Object[] args = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getCallingPackage", java.lang.String.class, args);
		}
		else
		{
			return super.getCallingPackage();
		}
	}

	public int getChangingConfigurations()
	{
		if (__ho48)
		{
			Object[] args = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getChangingConfigurations", int.class, args);
		}
		else
		{
			return super.getChangingConfigurations();
		}
	}

	public java.lang.ClassLoader getClassLoader()
	{
		if (__ho49)
		{
			Object[] args = null;
			return (java.lang.ClassLoader)com.tns.Platform.callJSMethod(this, "getClassLoader", java.lang.ClassLoader.class, args);
		}
		else
		{
			return super.getClassLoader();
		}
	}

	public android.content.ComponentName getComponentName()
	{
		if (__ho50)
		{
			Object[] args = null;
			return (android.content.ComponentName)com.tns.Platform.callJSMethod(this, "getComponentName", android.content.ComponentName.class, args);
		}
		else
		{
			return super.getComponentName();
		}
	}

	public android.content.ContentResolver getContentResolver()
	{
		if (__ho51)
		{
			Object[] args = null;
			return (android.content.ContentResolver)com.tns.Platform.callJSMethod(this, "getContentResolver", android.content.ContentResolver.class, args);
		}
		else
		{
			return super.getContentResolver();
		}
	}

	public android.view.View getCurrentFocus()
	{
		if (__ho52)
		{
			Object[] args = null;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "getCurrentFocus", android.view.View.class, args);
		}
		else
		{
			return super.getCurrentFocus();
		}
	}

	public java.io.File getDatabasePath(java.lang.String param_0)
	{
		if (__ho53)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getDatabasePath", java.io.File.class, args);
		}
		else
		{
			return super.getDatabasePath(param_0);
		}
	}

	public java.io.File getDir(java.lang.String param_0, int param_1)
	{
		if (__ho54)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getDir", java.io.File.class, args);
		}
		else
		{
			return super.getDir(param_0, param_1);
		}
	}

	public java.io.File getExternalCacheDir()
	{
		if (__ho55)
		{
			Object[] args = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getExternalCacheDir", java.io.File.class, args);
		}
		else
		{
			return super.getExternalCacheDir();
		}
	}

	public java.io.File getExternalFilesDir(java.lang.String param_0)
	{
		if (__ho56)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getExternalFilesDir", java.io.File.class, args);
		}
		else
		{
			return super.getExternalFilesDir(param_0);
		}
	}

	public java.io.File getFileStreamPath(java.lang.String param_0)
	{
		if (__ho57)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getFileStreamPath", java.io.File.class, args);
		}
		else
		{
			return super.getFileStreamPath(param_0);
		}
	}

	public java.io.File getFilesDir()
	{
		if (__ho58)
		{
			Object[] args = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getFilesDir", java.io.File.class, args);
		}
		else
		{
			return super.getFilesDir();
		}
	}

	public android.app.FragmentManager getFragmentManager()
	{
		if (__ho59)
		{
			Object[] args = null;
			return (android.app.FragmentManager)com.tns.Platform.callJSMethod(this, "getFragmentManager", android.app.FragmentManager.class, args);
		}
		else
		{
			return super.getFragmentManager();
		}
	}

	public android.content.Intent getIntent()
	{
		if (__ho60)
		{
			Object[] args = null;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "getIntent", android.content.Intent.class, args);
		}
		else
		{
			return super.getIntent();
		}
	}

	public java.lang.Object getLastNonConfigurationInstance()
	{
		if (__ho61)
		{
			Object[] args = null;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "getLastNonConfigurationInstance", java.lang.Object.class, args);
		}
		else
		{
			return super.getLastNonConfigurationInstance();
		}
	}

	public android.view.LayoutInflater getLayoutInflater()
	{
		if (__ho62)
		{
			Object[] args = null;
			return (android.view.LayoutInflater)com.tns.Platform.callJSMethod(this, "getLayoutInflater", android.view.LayoutInflater.class, args);
		}
		else
		{
			return super.getLayoutInflater();
		}
	}

	public android.app.LoaderManager getLoaderManager()
	{
		if (__ho63)
		{
			Object[] args = null;
			return (android.app.LoaderManager)com.tns.Platform.callJSMethod(this, "getLoaderManager", android.app.LoaderManager.class, args);
		}
		else
		{
			return super.getLoaderManager();
		}
	}

	public java.lang.String getLocalClassName()
	{
		if (__ho64)
		{
			Object[] args = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getLocalClassName", java.lang.String.class, args);
		}
		else
		{
			return super.getLocalClassName();
		}
	}

	public android.os.Looper getMainLooper()
	{
		if (__ho65)
		{
			Object[] args = null;
			return (android.os.Looper)com.tns.Platform.callJSMethod(this, "getMainLooper", android.os.Looper.class, args);
		}
		else
		{
			return super.getMainLooper();
		}
	}

	public android.view.MenuInflater getMenuInflater()
	{
		if (__ho66)
		{
			Object[] args = null;
			return (android.view.MenuInflater)com.tns.Platform.callJSMethod(this, "getMenuInflater", android.view.MenuInflater.class, args);
		}
		else
		{
			return super.getMenuInflater();
		}
	}

	public java.io.File getObbDir()
	{
		if (__ho67)
		{
			Object[] args = null;
			return (java.io.File)com.tns.Platform.callJSMethod(this, "getObbDir", java.io.File.class, args);
		}
		else
		{
			return super.getObbDir();
		}
	}

	public java.lang.String getPackageCodePath()
	{
		if (__ho68)
		{
			Object[] args = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageCodePath", java.lang.String.class, args);
		}
		else
		{
			return super.getPackageCodePath();
		}
	}

	public android.content.pm.PackageManager getPackageManager()
	{
		if (__ho69)
		{
			Object[] args = null;
			return (android.content.pm.PackageManager)com.tns.Platform.callJSMethod(this, "getPackageManager", android.content.pm.PackageManager.class, args);
		}
		else
		{
			return super.getPackageManager();
		}
	}

	public java.lang.String getPackageName()
	{
		if (__ho70)
		{
			Object[] args = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageName", java.lang.String.class, args);
		}
		else
		{
			return super.getPackageName();
		}
	}

	public java.lang.String getPackageResourcePath()
	{
		if (__ho71)
		{
			Object[] args = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "getPackageResourcePath", java.lang.String.class, args);
		}
		else
		{
			return super.getPackageResourcePath();
		}
	}

	public android.content.Intent getParentActivityIntent()
	{
		if (__ho72)
		{
			Object[] args = null;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "getParentActivityIntent", android.content.Intent.class, args);
		}
		else
		{
			return super.getParentActivityIntent();
		}
	}

	public android.content.SharedPreferences getPreferences(int param_0)
	{
		if (__ho73)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.content.SharedPreferences)com.tns.Platform.callJSMethod(this, "getPreferences", android.content.SharedPreferences.class, args);
		}
		else
		{
			return super.getPreferences(param_0);
		}
	}

	public int getRequestedOrientation()
	{
		if (__ho74)
		{
			Object[] args = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getRequestedOrientation", int.class, args);
		}
		else
		{
			return super.getRequestedOrientation();
		}
	}

	public android.content.res.Resources getResources()
	{
		if (__ho75)
		{
			Object[] args = null;
			return (android.content.res.Resources)com.tns.Platform.callJSMethod(this, "getResources", android.content.res.Resources.class, args);
		}
		else
		{
			return super.getResources();
		}
	}

	public android.content.SharedPreferences getSharedPreferences(java.lang.String param_0, int param_1)
	{
		if (__ho76)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (android.content.SharedPreferences)com.tns.Platform.callJSMethod(this, "getSharedPreferences", android.content.SharedPreferences.class, args);
		}
		else
		{
			return super.getSharedPreferences(param_0, param_1);
		}
	}

	public java.lang.Object getSystemService(java.lang.String param_0)
	{
		if (__ho77)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "getSystemService", java.lang.Object.class, args);
		}
		else
		{
			return super.getSystemService(param_0);
		}
	}

	public int getTaskId()
	{
		if (__ho78)
		{
			Object[] args = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getTaskId", int.class, args);
		}
		else
		{
			return super.getTaskId();
		}
	}

	public android.content.res.Resources.Theme getTheme()
	{
		if (__ho79)
		{
			Object[] args = null;
			return (android.content.res.Resources.Theme)com.tns.Platform.callJSMethod(this, "getTheme", android.content.res.Resources.Theme.class, args);
		}
		else
		{
			return super.getTheme();
		}
	}

	public android.graphics.drawable.Drawable getWallpaper()
	{
		if (__ho80)
		{
			Object[] args = null;
			return (android.graphics.drawable.Drawable)com.tns.Platform.callJSMethod(this, "getWallpaper", android.graphics.drawable.Drawable.class, args);
		}
		else
		{
			return super.getWallpaper();
		}
	}

	public int getWallpaperDesiredMinimumHeight()
	{
		if (__ho81)
		{
			Object[] args = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumHeight", int.class, args);
		}
		else
		{
			return super.getWallpaperDesiredMinimumHeight();
		}
	}

	public int getWallpaperDesiredMinimumWidth()
	{
		if (__ho82)
		{
			Object[] args = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "getWallpaperDesiredMinimumWidth", int.class, args);
		}
		else
		{
			return super.getWallpaperDesiredMinimumWidth();
		}
	}

	public android.view.Window getWindow()
	{
		if (__ho83)
		{
			Object[] args = null;
			return (android.view.Window)com.tns.Platform.callJSMethod(this, "getWindow", android.view.Window.class, args);
		}
		else
		{
			return super.getWindow();
		}
	}

	public android.view.WindowManager getWindowManager()
	{
		if (__ho84)
		{
			Object[] args = null;
			return (android.view.WindowManager)com.tns.Platform.callJSMethod(this, "getWindowManager", android.view.WindowManager.class, args);
		}
		else
		{
			return super.getWindowManager();
		}
	}

	public void grantUriPermission(java.lang.String param_0, android.net.Uri param_1, int param_2)
	{
		if (__ho85)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "grantUriPermission", void.class, args);
		}
		else
		{
			super.grantUriPermission(param_0, param_1, param_2);
		}
	}

	public boolean hasWindowFocus()
	{
		if (__ho86)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "hasWindowFocus", boolean.class, args);
		}
		else
		{
			return super.hasWindowFocus();
		}
	}

	public int hashCode()
	{
		if (__ho87)
		{
			Object[] args = null;
			return (Integer)com.tns.Platform.callJSMethod(this, "hashCode", int.class, args);
		}
		else
		{
			return super.hashCode();
		}
	}

	public void invalidateOptionsMenu()
	{
		if (__ho88)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "invalidateOptionsMenu", void.class, args);
		}
		else
		{
			super.invalidateOptionsMenu();
		}
	}

	public boolean isChangingConfigurations()
	{
		if (__ho89)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isChangingConfigurations", boolean.class, args);
		}
		else
		{
			return super.isChangingConfigurations();
		}
	}

	public boolean isDestroyed()
	{
		if (__ho90)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isDestroyed", boolean.class, args);
		}
		else
		{
			return super.isDestroyed();
		}
	}

	public boolean isFinishing()
	{
		if (__ho91)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isFinishing", boolean.class, args);
		}
		else
		{
			return super.isFinishing();
		}
	}

	public boolean isRestricted()
	{
		if (__ho92)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isRestricted", boolean.class, args);
		}
		else
		{
			return super.isRestricted();
		}
	}

	public boolean isTaskRoot()
	{
		if (__ho93)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "isTaskRoot", boolean.class, args);
		}
		else
		{
			return super.isTaskRoot();
		}
	}

	public boolean moveTaskToBack(boolean param_0)
	{
		if (__ho94)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "moveTaskToBack", boolean.class, args);
		}
		else
		{
			return super.moveTaskToBack(param_0);
		}
	}

	public boolean navigateUpTo(android.content.Intent param_0)
	{
		if (__ho95)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "navigateUpTo", boolean.class, args);
		}
		else
		{
			return super.navigateUpTo(param_0);
		}
	}

	public boolean navigateUpToFromChild(android.app.Activity param_0, android.content.Intent param_1)
	{
		if (__ho96)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "navigateUpToFromChild", boolean.class, args);
		}
		else
		{
			return super.navigateUpToFromChild(param_0, param_1);
		}
	}

	public void onActionModeFinished(android.view.ActionMode param_0)
	{
		if (__ho97)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActionModeFinished", void.class, args);
		}
		else
		{
			super.onActionModeFinished(param_0);
		}
	}

	public void onActionModeStarted(android.view.ActionMode param_0)
	{
		if (__ho98)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onActionModeStarted", void.class, args);
		}
		else
		{
			super.onActionModeStarted(param_0);
		}
	}

	protected void onActivityResult(int param_0, int param_1, android.content.Intent param_2)
	{
		if (__ho99)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onActivityResult", void.class, args);
		}
		else
		{
			super.onActivityResult(param_0, param_1, param_2);
		}
	}

	protected void onApplyThemeResource(android.content.res.Resources.Theme param_0, int param_1, boolean param_2)
	{
		if (__ho100)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onApplyThemeResource", void.class, args);
		}
		else
		{
			super.onApplyThemeResource(param_0, param_1, param_2);
		}
	}

	public void onAttachFragment(android.app.Fragment param_0)
	{
		if (__ho101)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onAttachFragment", void.class, args);
		}
		else
		{
			super.onAttachFragment(param_0);
		}
	}

	public void onAttachedToWindow()
	{
		if (__ho102)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onAttachedToWindow", void.class, args);
		}
		else
		{
			super.onAttachedToWindow();
		}
	}

	public void onBackPressed()
	{
		if (__ho103)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onBackPressed", void.class, args);
		}
		else
		{
			super.onBackPressed();
		}
	}

	protected void onChildTitleChanged(android.app.Activity param_0, java.lang.CharSequence param_1)
	{
		if (__ho104)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onChildTitleChanged", void.class, args);
		}
		else
		{
			super.onChildTitleChanged(param_0, param_1);
		}
	}

	public void onConfigurationChanged(android.content.res.Configuration param_0)
	{
		if (__ho105)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onConfigurationChanged", void.class, args);
		}
		else
		{
			super.onConfigurationChanged(param_0);
		}
	}

	public void onContentChanged()
	{
		if (__ho106)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onContentChanged", void.class, args);
		}
		else
		{
			super.onContentChanged();
		}
	}

	public boolean onContextItemSelected(android.view.MenuItem param_0)
	{
		if (__ho107)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onContextItemSelected", boolean.class, args);
		}
		else
		{
			return super.onContextItemSelected(param_0);
		}
	}

	public void onContextMenuClosed(android.view.Menu param_0)
	{
		if (__ho108)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onContextMenuClosed", void.class, args);
		}
		else
		{
			super.onContextMenuClosed(param_0);
		}
	}
	
	private native String[] getMethodOverrides(int objectId, Object[] packagesArgs);

	protected void onCreate(android.os.Bundle param_0)
	{
		Object[] packagesArgs = Platform.packageArgs(this);
		String[] methodOverrides = getMethodOverrides(objectId, packagesArgs);
		setMethodOverrides(methodOverrides);
		
		if (__ho109)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onCreate", void.class, args);
			android.util.Log.d("NativeScriptActivity", "NativeScriptActivity.onCreate called");
		}
		else
		{
			super.onCreate(param_0);
		}
	}

	public void onCreateContextMenu(android.view.ContextMenu param_0, android.view.View param_1, android.view.ContextMenu.ContextMenuInfo param_2)
	{
		if (__ho110)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onCreateContextMenu", void.class, args);
		}
		else
		{
			super.onCreateContextMenu(param_0, param_1, param_2);
		}
	}

	public java.lang.CharSequence onCreateDescription()
	{
		if (__ho111)
		{
			Object[] args = null;
			return (java.lang.CharSequence)com.tns.Platform.callJSMethod(this, "onCreateDescription", java.lang.CharSequence.class, args);
		}
		else
		{
			return super.onCreateDescription();
		}
	}

	protected android.app.Dialog onCreateDialog(int param_0, android.os.Bundle param_1)
	{
		if (__ho112)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (android.app.Dialog)com.tns.Platform.callJSMethod(this, "onCreateDialog", android.app.Dialog.class, args);
		}
		else
		{
			return super.onCreateDialog(param_0, param_1);
		}
	}

	protected android.app.Dialog onCreateDialog(int param_0)
	{
		if (__ho112)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.app.Dialog)com.tns.Platform.callJSMethod(this, "onCreateDialog", android.app.Dialog.class, args);
		}
		else
		{
			return super.onCreateDialog(param_0);
		}
	}

	public void onCreateNavigateUpTaskStack(android.app.TaskStackBuilder param_0)
	{
		if (__ho113)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onCreateNavigateUpTaskStack", void.class, args);
		}
		else
		{
			super.onCreateNavigateUpTaskStack(param_0);
		}
	}

	public boolean onCreateOptionsMenu(android.view.Menu param_0)
	{
		if (__ho114)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onCreateOptionsMenu", boolean.class, args);
		}
		else
		{
			return super.onCreateOptionsMenu(param_0);
		}
	}

	public boolean onCreatePanelMenu(int param_0, android.view.Menu param_1)
	{
		if (__ho115)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onCreatePanelMenu", boolean.class, args);
		}
		else
		{
			return super.onCreatePanelMenu(param_0, param_1);
		}
	}

	public android.view.View onCreatePanelView(int param_0)
	{
		if (__ho116)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "onCreatePanelView", android.view.View.class, args);
		}
		else
		{
			return super.onCreatePanelView(param_0);
		}
	}

	public boolean onCreateThumbnail(android.graphics.Bitmap param_0, android.graphics.Canvas param_1)
	{
		if (__ho117)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onCreateThumbnail", boolean.class, args);
		}
		else
		{
			return super.onCreateThumbnail(param_0, param_1);
		}
	}

	public android.view.View onCreateView(android.view.View param_0, java.lang.String param_1, android.content.Context param_2, android.util.AttributeSet param_3)
	{
		if (__ho118)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "onCreateView", android.view.View.class, args);
		}
		else
		{
			return super.onCreateView(param_0, param_1, param_2, param_3);
		}
	}

	public android.view.View onCreateView(java.lang.String param_0, android.content.Context param_1, android.util.AttributeSet param_2)
	{
		if (__ho118)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (android.view.View)com.tns.Platform.callJSMethod(this, "onCreateView", android.view.View.class, args);
		}
		else
		{
			return super.onCreateView(param_0, param_1, param_2);
		}
	}

	protected void onDestroy()
	{
		if (__ho119)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onDestroy", void.class, args);
		}
		else
		{
			super.onDestroy();
		}
		// TODO: remove from com.tns.Platform.strongInstances
	}

	public void onDetachedFromWindow()
	{
		if (__ho120)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onDetachedFromWindow", void.class, args);
		}
		else
		{
			super.onDetachedFromWindow();
		}
	}

	public boolean onGenericMotionEvent(android.view.MotionEvent param_0)
	{
		if (__ho121)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onGenericMotionEvent", boolean.class, args);
		}
		else
		{
			return super.onGenericMotionEvent(param_0);
		}
	}

	public boolean onKeyDown(int param_0, android.view.KeyEvent param_1)
	{
		if (__ho122)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyDown", boolean.class, args);
		}
		else
		{
			return super.onKeyDown(param_0, param_1);
		}
	}

	public boolean onKeyLongPress(int param_0, android.view.KeyEvent param_1)
	{
		if (__ho123)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyLongPress", boolean.class, args);
		}
		else
		{
			return super.onKeyLongPress(param_0, param_1);
		}
	}

	public boolean onKeyMultiple(int param_0, int param_1, android.view.KeyEvent param_2)
	{
		if (__ho124)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyMultiple", boolean.class, args);
		}
		else
		{
			return super.onKeyMultiple(param_0, param_1, param_2);
		}
	}

	public boolean onKeyShortcut(int param_0, android.view.KeyEvent param_1)
	{
		if (__ho125)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyShortcut", boolean.class, args);
		}
		else
		{
			return super.onKeyShortcut(param_0, param_1);
		}
	}

	public boolean onKeyUp(int param_0, android.view.KeyEvent param_1)
	{
		if (__ho126)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onKeyUp", boolean.class, args);
		}
		else
		{
			return super.onKeyUp(param_0, param_1);
		}
	}

	public void onLowMemory()
	{
		if (__ho127)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onLowMemory", void.class, args);
		}
		else
		{
			super.onLowMemory();
		}
	}

	public boolean onMenuItemSelected(int param_0, android.view.MenuItem param_1)
	{
		if (__ho128)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onMenuItemSelected", boolean.class, args);
		}
		else
		{
			return super.onMenuItemSelected(param_0, param_1);
		}
	}

	public boolean onMenuOpened(int param_0, android.view.Menu param_1)
	{
		if (__ho129)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onMenuOpened", boolean.class, args);
		}
		else
		{
			return super.onMenuOpened(param_0, param_1);
		}
	}

	public boolean onNavigateUp()
	{
		if (__ho130)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onNavigateUp", boolean.class, args);
		}
		else
		{
			return super.onNavigateUp();
		}
	}

	public boolean onNavigateUpFromChild(android.app.Activity param_0)
	{
		if (__ho131)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onNavigateUpFromChild", boolean.class, args);
		}
		else
		{
			return super.onNavigateUpFromChild(param_0);
		}
	}

	protected void onNewIntent(android.content.Intent param_0)
	{
		if (__ho132)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onNewIntent", void.class, args);
		}
		else
		{
			super.onNewIntent(param_0);
		}
	}

	public boolean onOptionsItemSelected(android.view.MenuItem param_0)
	{
		if (__ho133)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onOptionsItemSelected", boolean.class, args);
		}
		else
		{
			return super.onOptionsItemSelected(param_0);
		}
	}

	public void onOptionsMenuClosed(android.view.Menu param_0)
	{
		if (__ho134)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onOptionsMenuClosed", void.class, args);
		}
		else
		{
			super.onOptionsMenuClosed(param_0);
		}
	}

	public void onPanelClosed(int param_0, android.view.Menu param_1)
	{
		if (__ho135)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onPanelClosed", void.class, args);
		}
		else
		{
			super.onPanelClosed(param_0, param_1);
		}
	}

	protected void onPause()
	{
		if (__ho136)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onPause", void.class, args);
		}
		else
		{
			super.onPause();
		}
	}

	protected void onPostCreate(android.os.Bundle param_0)
	{
		if (__ho137)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onPostCreate", void.class, args);
		}
		else
		{
			super.onPostCreate(param_0);
		}
	}

	protected void onPostResume()
	{
		if (__ho138)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onPostResume", void.class, args);
		}
		else
		{
			super.onPostResume();
		}
	}

	protected void onPrepareDialog(int param_0, android.app.Dialog param_1)
	{
		if (__ho139)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onPrepareDialog", void.class, args);
		}
		else
		{
			super.onPrepareDialog(param_0, param_1);
		}
	}

	protected void onPrepareDialog(int param_0, android.app.Dialog param_1, android.os.Bundle param_2)
	{
		if (__ho139)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "onPrepareDialog", void.class, args);
		}
		else
		{
			super.onPrepareDialog(param_0, param_1, param_2);
		}
	}

	public void onPrepareNavigateUpTaskStack(android.app.TaskStackBuilder param_0)
	{
		if (__ho140)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onPrepareNavigateUpTaskStack", void.class, args);
		}
		else
		{
			super.onPrepareNavigateUpTaskStack(param_0);
		}
	}

	public boolean onPrepareOptionsMenu(android.view.Menu param_0)
	{
		if (__ho141)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onPrepareOptionsMenu", boolean.class, args);
		}
		else
		{
			return super.onPrepareOptionsMenu(param_0);
		}
	}

	public boolean onPreparePanel(int param_0, android.view.View param_1, android.view.Menu param_2)
	{
		if (__ho142)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onPreparePanel", boolean.class, args);
		}
		else
		{
			return super.onPreparePanel(param_0, param_1, param_2);
		}
	}

	protected void onRestart()
	{
		if (__ho143)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onRestart", void.class, args);
		}
		else
		{
			super.onRestart();
		}
	}

	protected void onRestoreInstanceState(android.os.Bundle param_0)
	{
		if (__ho144)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onRestoreInstanceState", void.class, args);
		}
		else
		{
			super.onRestoreInstanceState(param_0);
		}
	}

	protected void onResume()
	{
		if (__ho145)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onResume", void.class, args);
		}
		else
		{
			super.onResume();
		}
	}

	public java.lang.Object onRetainNonConfigurationInstance()
	{
		if (__ho146)
		{
			Object[] args = null;
			return (java.lang.Object)com.tns.Platform.callJSMethod(this, "onRetainNonConfigurationInstance", java.lang.Object.class, args);
		}
		else
		{
			return super.onRetainNonConfigurationInstance();
		}
	}

	protected void onSaveInstanceState(android.os.Bundle param_0)
	{
		if (__ho147)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onSaveInstanceState", void.class, args);
		}
		else
		{
			super.onSaveInstanceState(param_0);
		}
	}

	public boolean onSearchRequested()
	{
		if (__ho148)
		{
			Object[] args = null;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onSearchRequested", boolean.class, args);
		}
		else
		{
			return super.onSearchRequested();
		}
	}

	protected void onStart()
	{
		if (__ho149)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onStart", void.class, args);
		}
		else
		{
			super.onStart();
		}
	}

	protected void onStop()
	{
		if (__ho150)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onStop", void.class, args);
		}
		else
		{
			super.onStop();
		}
	}

	protected void onTitleChanged(java.lang.CharSequence param_0, int param_1)
	{
		if (__ho151)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "onTitleChanged", void.class, args);
		}
		else
		{
			super.onTitleChanged(param_0, param_1);
		}
	}

	public boolean onTouchEvent(android.view.MotionEvent param_0)
	{
		if (__ho152)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onTouchEvent", boolean.class, args);
		}
		else
		{
			return super.onTouchEvent(param_0);
		}
	}

	public boolean onTrackballEvent(android.view.MotionEvent param_0)
	{
		if (__ho153)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "onTrackballEvent", boolean.class, args);
		}
		else
		{
			return super.onTrackballEvent(param_0);
		}
	}

	public void onTrimMemory(int param_0)
	{
		if (__ho154)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onTrimMemory", void.class, args);
		}
		else
		{
			super.onTrimMemory(param_0);
		}
	}

	public void onUserInteraction()
	{
		if (__ho155)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onUserInteraction", void.class, args);
		}
		else
		{
			super.onUserInteraction();
		}
	}

	protected void onUserLeaveHint()
	{
		if (__ho156)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "onUserLeaveHint", void.class, args);
		}
		else
		{
			super.onUserLeaveHint();
		}
	}

	public void onWindowAttributesChanged(android.view.WindowManager.LayoutParams param_0)
	{
		if (__ho157)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onWindowAttributesChanged", void.class, args);
		}
		else
		{
			super.onWindowAttributesChanged(param_0);
		}
	}

	public void onWindowFocusChanged(boolean param_0)
	{
		if (__ho158)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "onWindowFocusChanged", void.class, args);
		}
		else
		{
			super.onWindowFocusChanged(param_0);
		}
	}

	public android.view.ActionMode onWindowStartingActionMode(android.view.ActionMode.Callback param_0)
	{
		if (__ho159)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.view.ActionMode)com.tns.Platform.callJSMethod(this, "onWindowStartingActionMode", android.view.ActionMode.class, args);
		}
		else
		{
			return super.onWindowStartingActionMode(param_0);
		}
	}

	public void openContextMenu(android.view.View param_0)
	{
		if (__ho160)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "openContextMenu", void.class, args);
		}
		else
		{
			super.openContextMenu(param_0);
		}
	}

	public java.io.FileInputStream openFileInput(java.lang.String param_0) throws java.io.FileNotFoundException
	{
		if (__ho161)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (java.io.FileInputStream)com.tns.Platform.callJSMethod(this, "openFileInput", java.io.FileInputStream.class, args);
		}
		else
		{
			return super.openFileInput(param_0);
		}
	}

	public java.io.FileOutputStream openFileOutput(java.lang.String param_0, int param_1) throws java.io.FileNotFoundException
	{
		if (__ho162)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (java.io.FileOutputStream)com.tns.Platform.callJSMethod(this, "openFileOutput", java.io.FileOutputStream.class, args);
		}
		else
		{
			return super.openFileOutput(param_0, param_1);
		}
	}

	public void openOptionsMenu()
	{
		if (__ho163)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "openOptionsMenu", void.class, args);
		}
		else
		{
			super.openOptionsMenu();
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2, android.database.DatabaseErrorHandler param_3)
	{
		if (__ho164)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			return (android.database.sqlite.SQLiteDatabase)com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", android.database.sqlite.SQLiteDatabase.class, args);
		}
		else
		{
			return super.openOrCreateDatabase(param_0, param_1, param_2, param_3);
		}
	}

	public android.database.sqlite.SQLiteDatabase openOrCreateDatabase(java.lang.String param_0, int param_1, android.database.sqlite.SQLiteDatabase.CursorFactory param_2)
	{
		if (__ho164)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (android.database.sqlite.SQLiteDatabase)com.tns.Platform.callJSMethod(this, "openOrCreateDatabase", android.database.sqlite.SQLiteDatabase.class, args);
		}
		else
		{
			return super.openOrCreateDatabase(param_0, param_1, param_2);
		}
	}

	public void overridePendingTransition(int param_0, int param_1)
	{
		if (__ho165)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "overridePendingTransition", void.class, args);
		}
		else
		{
			super.overridePendingTransition(param_0, param_1);
		}
	}

	public android.graphics.drawable.Drawable peekWallpaper()
	{
		if (__ho166)
		{
			Object[] args = null;
			return (android.graphics.drawable.Drawable)com.tns.Platform.callJSMethod(this, "peekWallpaper", android.graphics.drawable.Drawable.class, args);
		}
		else
		{
			return super.peekWallpaper();
		}
	}

	public void recreate()
	{
		if (__ho167)
		{
			Object[] args = null;
			com.tns.Platform.callJSMethod(this, "recreate", void.class, args);
		}
		else
		{
			super.recreate();
		}
	}

	public void registerComponentCallbacks(android.content.ComponentCallbacks param_0)
	{
		if (__ho168)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerComponentCallbacks", void.class, args);
		}
		else
		{
			super.registerComponentCallbacks(param_0);
		}
	}

	public void registerForContextMenu(android.view.View param_0)
	{
		if (__ho169)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "registerForContextMenu", void.class, args);
		}
		else
		{
			super.registerForContextMenu(param_0);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1, java.lang.String param_2, android.os.Handler param_3)
	{
		if (__ho170)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "registerReceiver", android.content.Intent.class, args);
		}
		else
		{
			return super.registerReceiver(param_0, param_1, param_2, param_3);
		}
	}

	public android.content.Intent registerReceiver(android.content.BroadcastReceiver param_0, android.content.IntentFilter param_1)
	{
		if (__ho170)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (android.content.Intent)com.tns.Platform.callJSMethod(this, "registerReceiver", android.content.Intent.class, args);
		}
		else
		{
			return super.registerReceiver(param_0, param_1);
		}
	}

	public void removeStickyBroadcast(android.content.Intent param_0)
	{
		if (__ho171)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcast", void.class, args);
		}
		else
		{
			super.removeStickyBroadcast(param_0);
		}
	}

	public void removeStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1)
	{
		if (__ho172)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "removeStickyBroadcastAsUser", void.class, args);
		}
		else
		{
			super.removeStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void revokeUriPermission(android.net.Uri param_0, int param_1)
	{
		if (__ho173)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "revokeUriPermission", void.class, args);
		}
		else
		{
			super.revokeUriPermission(param_0, param_1);
		}
	}

	public void sendBroadcast(android.content.Intent param_0, java.lang.String param_1)
	{
		if (__ho174)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", void.class, args);
		}
		else
		{
			super.sendBroadcast(param_0, param_1);
		}
	}

	public void sendBroadcast(android.content.Intent param_0)
	{
		if (__ho174)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendBroadcast", void.class, args);
		}
		else
		{
			super.sendBroadcast(param_0);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2)
	{
		if (__ho175)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", void.class, args);
		}
		else
		{
			super.sendBroadcastAsUser(param_0, param_1, param_2);
		}
	}

	public void sendBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1)
	{
		if (__ho175)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendBroadcastAsUser", void.class, args);
		}
		else
		{
			super.sendBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6)
	{
		if (__ho176)
		{
			Object[] args = new Object[7];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			args[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", void.class, args);
		}
		else
		{
			super.sendOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void sendOrderedBroadcast(android.content.Intent param_0, java.lang.String param_1)
	{
		if (__ho176)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcast", void.class, args);
		}
		else
		{
			super.sendOrderedBroadcast(param_0, param_1);
		}
	}

	public void sendOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, java.lang.String param_2, android.content.BroadcastReceiver param_3, android.os.Handler param_4, int param_5, java.lang.String param_6, android.os.Bundle param_7)
	{
		if (__ho177)
		{
			Object[] args = new Object[8];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			args[6] = param_6;
			args[7] = param_7;
			com.tns.Platform.callJSMethod(this, "sendOrderedBroadcastAsUser", void.class, args);
		}
		else
		{
			super.sendOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7);
		}
	}

	public void sendStickyBroadcast(android.content.Intent param_0)
	{
		if (__ho178)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcast", void.class, args);
		}
		else
		{
			super.sendStickyBroadcast(param_0);
		}
	}

	public void sendStickyBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1)
	{
		if (__ho179)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "sendStickyBroadcastAsUser", void.class, args);
		}
		else
		{
			super.sendStickyBroadcastAsUser(param_0, param_1);
		}
	}

	public void sendStickyOrderedBroadcast(android.content.Intent param_0, android.content.BroadcastReceiver param_1, android.os.Handler param_2, int param_3, java.lang.String param_4, android.os.Bundle param_5)
	{
		if (__ho180)
		{
			Object[] args = new Object[6];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcast", void.class, args);
		}
		else
		{
			super.sendStickyOrderedBroadcast(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void sendStickyOrderedBroadcastAsUser(android.content.Intent param_0, android.os.UserHandle param_1, android.content.BroadcastReceiver param_2, android.os.Handler param_3, int param_4, java.lang.String param_5, android.os.Bundle param_6)
	{
		if (__ho181)
		{
			Object[] args = new Object[7];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			args[6] = param_6;
			com.tns.Platform.callJSMethod(this, "sendStickyOrderedBroadcastAsUser", void.class, args);
		}
		else
		{
			super.sendStickyOrderedBroadcastAsUser(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void setContentView(android.view.View param_0, android.view.ViewGroup.LayoutParams param_1)
	{
		if (__ho182)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "setContentView", void.class, args);
		}
		else
		{
			super.setContentView(param_0, param_1);
		}
	}

	public void setContentView(int param_0)
	{
		if (__ho182)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setContentView", void.class, args);
		}
		else
		{
			super.setContentView(param_0);
		}
	}

	public void setContentView(android.view.View param_0)
	{
		if (__ho182)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setContentView", void.class, args);
		}
		else
		{
			super.setContentView(param_0);
		}
	}

	public void setFinishOnTouchOutside(boolean param_0)
	{
		if (__ho183)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setFinishOnTouchOutside", void.class, args);
		}
		else
		{
			super.setFinishOnTouchOutside(param_0);
		}
	}

	public void setIntent(android.content.Intent param_0)
	{
		if (__ho184)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setIntent", void.class, args);
		}
		else
		{
			super.setIntent(param_0);
		}
	}

	public void setRequestedOrientation(int param_0)
	{
		if (__ho185)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setRequestedOrientation", void.class, args);
		}
		else
		{
			super.setRequestedOrientation(param_0);
		}
	}

	public void setTheme(int param_0)
	{
		if (__ho186)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTheme", void.class, args);
		}
		else
		{
			super.setTheme(param_0);
		}
	}

	public void setTitle(int param_0)
	{
		if (__ho187)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTitle", void.class, args);
		}
		else
		{
			super.setTitle(param_0);
		}
	}

	public void setTitle(java.lang.CharSequence param_0)
	{
		if (__ho187)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTitle", void.class, args);
		}
		else
		{
			super.setTitle(param_0);
		}
	}

	public void setTitleColor(int param_0)
	{
		if (__ho188)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setTitleColor", void.class, args);
		}
		else
		{
			super.setTitleColor(param_0);
		}
	}

	public void setVisible(boolean param_0)
	{
		if (__ho189)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setVisible", void.class, args);
		}
		else
		{
			super.setVisible(param_0);
		}
	}

	public void setWallpaper(java.io.InputStream param_0) throws java.io.IOException
	{
		if (__ho190)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", void.class, args);
		}
		else
		{
			super.setWallpaper(param_0);
		}
	}

	public void setWallpaper(android.graphics.Bitmap param_0) throws java.io.IOException
	{
		if (__ho190)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "setWallpaper", void.class, args);
		}
		else
		{
			super.setWallpaper(param_0);
		}
	}

	public boolean shouldUpRecreateTask(android.content.Intent param_0)
	{
		if (__ho191)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "shouldUpRecreateTask", boolean.class, args);
		}
		else
		{
			return super.shouldUpRecreateTask(param_0);
		}
	}

	public android.view.ActionMode startActionMode(android.view.ActionMode.Callback param_0)
	{
		if (__ho192)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.view.ActionMode)com.tns.Platform.callJSMethod(this, "startActionMode", android.view.ActionMode.class, args);
		}
		else
		{
			return super.startActionMode(param_0);
		}
	}

	public void startActivities(android.content.Intent[] param_0)
	{
		if (__ho193)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivities", void.class, args);
		}
		else
		{
			super.startActivities(param_0);
		}
	}

	public void startActivities(android.content.Intent[] param_0, android.os.Bundle param_1)
	{
		if (__ho193)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivities", void.class, args);
		}
		else
		{
			super.startActivities(param_0, param_1);
		}
	}

	public void startActivity(android.content.Intent param_0, android.os.Bundle param_1)
	{
		if (__ho194)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivity", void.class, args);
		}
		else
		{
			super.startActivity(param_0, param_1);
		}
	}

	public void startActivity(android.content.Intent param_0)
	{
		if (__ho194)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startActivity", void.class, args);
		}
		else
		{
			super.startActivity(param_0);
		}
	}

	public void startActivityForResult(android.content.Intent param_0, int param_1)
	{
		if (__ho195)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "startActivityForResult", void.class, args);
		}
		else
		{
			super.startActivityForResult(param_0, param_1);
		}
	}

	public void startActivityForResult(android.content.Intent param_0, int param_1, android.os.Bundle param_2)
	{
		if (__ho195)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "startActivityForResult", void.class, args);
		}
		else
		{
			super.startActivityForResult(param_0, param_1, param_2);
		}
	}

	public void startActivityFromChild(android.app.Activity param_0, android.content.Intent param_1, int param_2)
	{
		if (__ho196)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "startActivityFromChild", void.class, args);
		}
		else
		{
			super.startActivityFromChild(param_0, param_1, param_2);
		}
	}

	public void startActivityFromChild(android.app.Activity param_0, android.content.Intent param_1, int param_2, android.os.Bundle param_3)
	{
		if (__ho196)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			com.tns.Platform.callJSMethod(this, "startActivityFromChild", void.class, args);
		}
		else
		{
			super.startActivityFromChild(param_0, param_1, param_2, param_3);
		}
	}

	public void startActivityFromFragment(android.app.Fragment param_0, android.content.Intent param_1, int param_2)
	{
		if (__ho197)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			com.tns.Platform.callJSMethod(this, "startActivityFromFragment", void.class, args);
		}
		else
		{
			super.startActivityFromFragment(param_0, param_1, param_2);
		}
	}

	public void startActivityFromFragment(android.app.Fragment param_0, android.content.Intent param_1, int param_2, android.os.Bundle param_3)
	{
		if (__ho197)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			com.tns.Platform.callJSMethod(this, "startActivityFromFragment", void.class, args);
		}
		else
		{
			super.startActivityFromFragment(param_0, param_1, param_2, param_3);
		}
	}

	public boolean startActivityIfNeeded(android.content.Intent param_0, int param_1)
	{
		if (__ho198)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startActivityIfNeeded", boolean.class, args);
		}
		else
		{
			return super.startActivityIfNeeded(param_0, param_1);
		}
	}

	public boolean startActivityIfNeeded(android.content.Intent param_0, int param_1, android.os.Bundle param_2)
	{
		if (__ho198)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startActivityIfNeeded", boolean.class, args);
		}
		else
		{
			return super.startActivityIfNeeded(param_0, param_1, param_2);
		}
	}

	public boolean startInstrumentation(android.content.ComponentName param_0, java.lang.String param_1, android.os.Bundle param_2)
	{
		if (__ho199)
		{
			Object[] args = new Object[3];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startInstrumentation", boolean.class, args);
		}
		else
		{
			return super.startInstrumentation(param_0, param_1, param_2);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4) throws android.content.IntentSender.SendIntentException
	{
		if (__ho200)
		{
			Object[] args = new Object[5];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			com.tns.Platform.callJSMethod(this, "startIntentSender", void.class, args);
		}
		else
		{
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4);
		}
	}

	public void startIntentSender(android.content.IntentSender param_0, android.content.Intent param_1, int param_2, int param_3, int param_4, android.os.Bundle param_5) throws android.content.IntentSender.SendIntentException
	{
		if (__ho200)
		{
			Object[] args = new Object[6];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			com.tns.Platform.callJSMethod(this, "startIntentSender", void.class, args);
		}
		else
		{
			super.startIntentSender(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void startIntentSenderForResult(android.content.IntentSender param_0, int param_1, android.content.Intent param_2, int param_3, int param_4, int param_5, android.os.Bundle param_6) throws android.content.IntentSender.SendIntentException
	{
		if (__ho201)
		{
			Object[] args = new Object[7];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			args[6] = param_6;
			com.tns.Platform.callJSMethod(this, "startIntentSenderForResult", void.class, args);
		}
		else
		{
			super.startIntentSenderForResult(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void startIntentSenderForResult(android.content.IntentSender param_0, int param_1, android.content.Intent param_2, int param_3, int param_4, int param_5) throws android.content.IntentSender.SendIntentException
	{
		if (__ho201)
		{
			Object[] args = new Object[6];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			com.tns.Platform.callJSMethod(this, "startIntentSenderForResult", void.class, args);
		}
		else
		{
			super.startIntentSenderForResult(param_0, param_1, param_2, param_3, param_4, param_5);
		}
	}

	public void startIntentSenderFromChild(android.app.Activity param_0, android.content.IntentSender param_1, int param_2, android.content.Intent param_3, int param_4, int param_5, int param_6, android.os.Bundle param_7) throws android.content.IntentSender.SendIntentException
	{
		if (__ho202)
		{
			Object[] args = new Object[8];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			args[6] = param_6;
			args[7] = param_7;
			com.tns.Platform.callJSMethod(this, "startIntentSenderFromChild", void.class, args);
		}
		else
		{
			super.startIntentSenderFromChild(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7);
		}
	}

	public void startIntentSenderFromChild(android.app.Activity param_0, android.content.IntentSender param_1, int param_2, android.content.Intent param_3, int param_4, int param_5, int param_6) throws android.content.IntentSender.SendIntentException
	{
		if (__ho202)
		{
			Object[] args = new Object[7];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			args[4] = param_4;
			args[5] = param_5;
			args[6] = param_6;
			com.tns.Platform.callJSMethod(this, "startIntentSenderFromChild", void.class, args);
		}
		else
		{
			super.startIntentSenderFromChild(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
		}
	}

	public void startManagingCursor(android.database.Cursor param_0)
	{
		if (__ho203)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "startManagingCursor", void.class, args);
		}
		else
		{
			super.startManagingCursor(param_0);
		}
	}

	public boolean startNextMatchingActivity(android.content.Intent param_0, android.os.Bundle param_1)
	{
		if (__ho204)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startNextMatchingActivity", boolean.class, args);
		}
		else
		{
			return super.startNextMatchingActivity(param_0, param_1);
		}
	}

	public boolean startNextMatchingActivity(android.content.Intent param_0)
	{
		if (__ho204)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "startNextMatchingActivity", boolean.class, args);
		}
		else
		{
			return super.startNextMatchingActivity(param_0);
		}
	}

	public void startSearch(java.lang.String param_0, boolean param_1, android.os.Bundle param_2, boolean param_3)
	{
		if (__ho205)
		{
			Object[] args = new Object[4];
			args[0] = param_0;
			args[1] = param_1;
			args[2] = param_2;
			args[3] = param_3;
			com.tns.Platform.callJSMethod(this, "startSearch", void.class, args);
		}
		else
		{
			super.startSearch(param_0, param_1, param_2, param_3);
		}
	}

	public android.content.ComponentName startService(android.content.Intent param_0)
	{
		if (__ho206)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (android.content.ComponentName)com.tns.Platform.callJSMethod(this, "startService", android.content.ComponentName.class, args);
		}
		else
		{
			return super.startService(param_0);
		}
	}

	public void stopManagingCursor(android.database.Cursor param_0)
	{
		if (__ho207)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "stopManagingCursor", void.class, args);
		}
		else
		{
			super.stopManagingCursor(param_0);
		}
	}

	public boolean stopService(android.content.Intent param_0)
	{
		if (__ho208)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			return (Boolean)com.tns.Platform.callJSMethod(this, "stopService", boolean.class, args);
		}
		else
		{
			return super.stopService(param_0);
		}
	}

	public void takeKeyEvents(boolean param_0)
	{
		if (__ho209)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "takeKeyEvents", void.class, args);
		}
		else
		{
			super.takeKeyEvents(param_0);
		}
	}

	public java.lang.String toString()
	{
		if (__ho210)
		{
			Object[] args = null;
			return (java.lang.String)com.tns.Platform.callJSMethod(this, "toString", java.lang.String.class, args);
		}
		else
		{
			return super.toString();
		}
	}

	public void triggerSearch(java.lang.String param_0, android.os.Bundle param_1)
	{
		if (__ho211)
		{
			Object[] args = new Object[2];
			args[0] = param_0;
			args[1] = param_1;
			com.tns.Platform.callJSMethod(this, "triggerSearch", void.class, args);
		}
		else
		{
			super.triggerSearch(param_0, param_1);
		}
	}

	public void unbindService(android.content.ServiceConnection param_0)
	{
		if (__ho212)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unbindService", void.class, args);
		}
		else
		{
			super.unbindService(param_0);
		}
	}

	public void unregisterComponentCallbacks(android.content.ComponentCallbacks param_0)
	{
		if (__ho213)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterComponentCallbacks", void.class, args);
		}
		else
		{
			super.unregisterComponentCallbacks(param_0);
		}
	}

	public void unregisterForContextMenu(android.view.View param_0)
	{
		if (__ho214)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterForContextMenu", void.class, args);
		}
		else
		{
			super.unregisterForContextMenu(param_0);
		}
	}

	public void unregisterReceiver(android.content.BroadcastReceiver param_0)
	{
		if (__ho215)
		{
			Object[] args = new Object[1];
			args[0] = param_0;
			com.tns.Platform.callJSMethod(this, "unregisterReceiver", void.class, args);
		}
		else
		{
			super.unregisterReceiver(param_0);
		}
	}

	private void setMethodOverrides(String[] methodOverrides)
	{
		for (String m: methodOverrides)
		{
			if (m.equals("onChildTitleChanged")) __ho104= true;
			if (m.equals("getChangingConfigurations")) __ho48= true;
			if (m.equals("getObbDir")) __ho67= true;
			if (m.equals("onSearchRequested")) __ho148= true;
			if (m.equals("navigateUpToFromChild")) __ho96= true;
			if (m.equals("deleteDatabase")) __ho17= true;
			if (m.equals("checkPermission")) __ho7= true;
			if (m.equals("enforceUriPermission")) __ho31= true;
			if (m.equals("startActivityFromFragment")) __ho197= true;
			if (m.equals("databaseList")) __ho16= true;
			if (m.equals("dispatchGenericMotionEvent")) __ho19= true;
			if (m.equals("getFilesDir")) __ho58= true;
			if (m.equals("sendStickyOrderedBroadcastAsUser")) __ho181= true;
			if (m.equals("fileList")) __ho33= true;
			if (m.equals("getWallpaper")) __ho80= true;
			if (m.equals("onBackPressed")) __ho103= true;
			if (m.equals("onRestart")) __ho143= true;
			if (m.equals("shouldUpRecreateTask")) __ho191= true;
			if (m.equals("finishAffinity")) __ho38= true;
			if (m.equals("setIntent")) __ho184= true;
			if (m.equals("unregisterComponentCallbacks")) __ho213= true;
			if (m.equals("dispatchPopulateAccessibilityEvent")) __ho22= true;
			if (m.equals("getLoaderManager")) __ho63= true;
			if (m.equals("startIntentSender")) __ho200= true;
			if (m.equals("openOrCreateDatabase")) __ho164= true;
			if (m.equals("getFileStreamPath")) __ho57= true;
			if (m.equals("closeOptionsMenu")) __ho11= true;
			if (m.equals("getWallpaperDesiredMinimumHeight")) __ho81= true;
			if (m.equals("getCallingActivity")) __ho46= true;
			if (m.equals("openOptionsMenu")) __ho163= true;
			if (m.equals("onWindowAttributesChanged")) __ho157= true;
			if (m.equals("invalidateOptionsMenu")) __ho88= true;
			if (m.equals("onCreateNavigateUpTaskStack")) __ho113= true;
			if (m.equals("removeStickyBroadcast")) __ho171= true;
			if (m.equals("dispatchKeyEvent")) __ho20= true;
			if (m.equals("getCurrentFocus")) __ho52= true;
			if (m.equals("peekWallpaper")) __ho166= true;
			if (m.equals("createConfigurationContext")) __ho12= true;
			if (m.equals("getApplicationContext")) __ho41= true;
			if (m.equals("sendStickyBroadcast")) __ho178= true;
			if (m.equals("getResources")) __ho75= true;
			if (m.equals("onOptionsMenuClosed")) __ho134= true;
			if (m.equals("getSharedPreferences")) __ho76= true;
			if (m.equals("setFinishOnTouchOutside")) __ho183= true;
			if (m.equals("onAttachedToWindow")) __ho102= true;
			if (m.equals("getWallpaperDesiredMinimumWidth")) __ho82= true;
			if (m.equals("startInstrumentation")) __ho199= true;
			if (m.equals("onPrepareOptionsMenu")) __ho141= true;
			if (m.equals("getComponentName")) __ho50= true;
			if (m.equals("sendStickyBroadcastAsUser")) __ho179= true;
			if (m.equals("setWallpaper")) __ho190= true;
			if (m.equals("stopManagingCursor")) __ho207= true;
			if (m.equals("getParentActivityIntent")) __ho72= true;
			if (m.equals("onTrimMemory")) __ho154= true;
			if (m.equals("onActionModeFinished")) __ho97= true;
			if (m.equals("recreate")) __ho167= true;
			if (m.equals("sendStickyOrderedBroadcast")) __ho180= true;
			if (m.equals("onCreateDialog")) __ho112= true;
			if (m.equals("startActivityForResult")) __ho195= true;
			if (m.equals("onTitleChanged")) __ho151= true;
			if (m.equals("getActionBar")) __ho40= true;
			if (m.equals("onCreatePanelView")) __ho116= true;
			if (m.equals("onNewIntent")) __ho132= true;
			if (m.equals("onKeyMultiple")) __ho124= true;
			if (m.equals("toString")) __ho210= true;
			if (m.equals("applyOverrideConfiguration")) __ho1= true;
			if (m.equals("getFragmentManager")) __ho59= true;
			if (m.equals("onPanelClosed")) __ho135= true;
			if (m.equals("createDisplayContext")) __ho13= true;
			if (m.equals("onKeyShortcut")) __ho125= true;
			if (m.equals("dispatchTrackballEvent")) __ho24= true;
			if (m.equals("addContentView")) __ho0= true;
			if (m.equals("onActivityResult")) __ho99= true;
			if (m.equals("openFileInput")) __ho161= true;
			if (m.equals("getRequestedOrientation")) __ho74= true;
			if (m.equals("getWindowManager")) __ho84= true;
			if (m.equals("triggerSearch")) __ho211= true;
			if (m.equals("finish")) __ho35= true;
			if (m.equals("dispatchKeyShortcutEvent")) __ho21= true;
			if (m.equals("setVisible")) __ho189= true;
			if (m.equals("isDestroyed")) __ho90= true;
			if (m.equals("setTitle")) __ho187= true;
			if (m.equals("startActivities")) __ho193= true;
			if (m.equals("onKeyLongPress")) __ho123= true;
			if (m.equals("onGenericMotionEvent")) __ho121= true;
			if (m.equals("getMenuInflater")) __ho66= true;
			if (m.equals("isChangingConfigurations")) __ho89= true;
			if (m.equals("getPackageName")) __ho70= true;
			if (m.equals("setRequestedOrientation")) __ho185= true;
			if (m.equals("enforceCallingUriPermission")) __ho29= true;
			if (m.equals("getLocalClassName")) __ho64= true;
			if (m.equals("getWindow")) __ho83= true;
			if (m.equals("onRestoreInstanceState")) __ho144= true;
			if (m.equals("checkCallingOrSelfUriPermission")) __ho4= true;
			if (m.equals("setTitleColor")) __ho188= true;
			if (m.equals("getClassLoader")) __ho49= true;
			if (m.equals("closeContextMenu")) __ho10= true;
			if (m.equals("createPackageContext")) __ho14= true;
			if (m.equals("openFileOutput")) __ho162= true;
			if (m.equals("moveTaskToBack")) __ho94= true;
			if (m.equals("dispatchTouchEvent")) __ho23= true;
			if (m.equals("onActionModeStarted")) __ho98= true;
			if (m.equals("onPostCreate")) __ho137= true;
			if (m.equals("hashCode")) __ho87= true;
			if (m.equals("getMainLooper")) __ho65= true;
			if (m.equals("getDir")) __ho54= true;
			if (m.equals("deleteFile")) __ho18= true;
			if (m.equals("getTaskId")) __ho78= true;
			if (m.equals("navigateUpTo")) __ho95= true;
			if (m.equals("revokeUriPermission")) __ho173= true;
			if (m.equals("finishActivity")) __ho36= true;
			if (m.equals("finishFromChild")) __ho39= true;
			if (m.equals("takeKeyEvents")) __ho209= true;
			if (m.equals("overridePendingTransition")) __ho165= true;
			if (m.equals("checkCallingPermission")) __ho5= true;
			if (m.equals("enforceCallingPermission")) __ho28= true;
			if (m.equals("unregisterForContextMenu")) __ho214= true;
			if (m.equals("findViewById")) __ho34= true;
			if (m.equals("onNavigateUp")) __ho130= true;
			if (m.equals("enforceCallingOrSelfPermission")) __ho26= true;
			if (m.equals("removeStickyBroadcastAsUser")) __ho172= true;
			if (m.equals("onNavigateUpFromChild")) __ho131= true;
			if (m.equals("isTaskRoot")) __ho93= true;
			if (m.equals("getContentResolver")) __ho51= true;
			if (m.equals("getPackageResourcePath")) __ho71= true;
			if (m.equals("isFinishing")) __ho91= true;
			if (m.equals("getIntent")) __ho60= true;
			if (m.equals("onStop")) __ho150= true;
			if (m.equals("onWindowStartingActionMode")) __ho159= true;
			if (m.equals("enforceCallingOrSelfUriPermission")) __ho27= true;
			if (m.equals("getDatabasePath")) __ho53= true;
			if (m.equals("startActivityIfNeeded")) __ho198= true;
			if (m.equals("getCacheDir")) __ho45= true;
			if (m.equals("getTheme")) __ho79= true;
			if (m.equals("checkCallingOrSelfPermission")) __ho3= true;
			if (m.equals("setTheme")) __ho186= true;
			if (m.equals("sendOrderedBroadcastAsUser")) __ho177= true;
			if (m.equals("onCreatePanelMenu")) __ho115= true;
			if (m.equals("onPostResume")) __ho138= true;
			if (m.equals("bindService")) __ho2= true;
			if (m.equals("isRestricted")) __ho92= true;
			if (m.equals("onPreparePanel")) __ho142= true;
			if (m.equals("checkCallingUriPermission")) __ho6= true;
			if (m.equals("getSystemService")) __ho77= true;
			if (m.equals("startSearch")) __ho205= true;
			if (m.equals("getPackageCodePath")) __ho68= true;
			if (m.equals("onContextItemSelected")) __ho107= true;
			if (m.equals("onApplyThemeResource")) __ho100= true;
			if (m.equals("onPause")) __ho136= true;
			if (m.equals("finishActivityFromChild")) __ho37= true;
			if (m.equals("startActivity")) __ho194= true;
			if (m.equals("getLayoutInflater")) __ho62= true;
			if (m.equals("startActionMode")) __ho192= true;
			if (m.equals("grantUriPermission")) __ho85= true;
			if (m.equals("onRetainNonConfigurationInstance")) __ho146= true;
			if (m.equals("getAssets")) __ho43= true;
			if (m.equals("onDestroy")) __ho119= true;
			if (m.equals("onKeyUp")) __ho126= true;
			if (m.equals("onMenuOpened")) __ho129= true;
			if (m.equals("getExternalFilesDir")) __ho56= true;
			if (m.equals("startManagingCursor")) __ho203= true;
			if (m.equals("onPrepareDialog")) __ho139= true;
			if (m.equals("openContextMenu")) __ho160= true;
			if (m.equals("getCallingPackage")) __ho47= true;
			if (m.equals("equals")) __ho32= true;
			if (m.equals("onAttachFragment")) __ho101= true;
			if (m.equals("hasWindowFocus")) __ho86= true;
			if (m.equals("onSaveInstanceState")) __ho147= true;
			if (m.equals("onCreateOptionsMenu")) __ho114= true;
			if (m.equals("registerComponentCallbacks")) __ho168= true;
			if (m.equals("sendOrderedBroadcast")) __ho176= true;
			if (m.equals("sendBroadcastAsUser")) __ho175= true;
			if (m.equals("getExternalCacheDir")) __ho55= true;
			if (m.equals("startIntentSenderForResult")) __ho201= true;
			if (m.equals("onLowMemory")) __ho127= true;
			if (m.equals("onOptionsItemSelected")) __ho133= true;
			if (m.equals("onCreateThumbnail")) __ho117= true;
			if (m.equals("onStart")) __ho149= true;
			if (m.equals("startNextMatchingActivity")) __ho204= true;
			if (m.equals("onUserInteraction")) __ho155= true;
			if (m.equals("onTrackballEvent")) __ho153= true;
			if (m.equals("onTouchEvent")) __ho152= true;
			if (m.equals("onUserLeaveHint")) __ho156= true;
			if (m.equals("onResume")) __ho145= true;
			if (m.equals("getPreferences")) __ho73= true;
			if (m.equals("startService")) __ho206= true;
			if (m.equals("onContextMenuClosed")) __ho108= true;
			if (m.equals("registerReceiver")) __ho170= true;
			if (m.equals("getPackageManager")) __ho69= true;
			if (m.equals("onCreateView")) __ho118= true;
			if (m.equals("onConfigurationChanged")) __ho105= true;
			if (m.equals("onContentChanged")) __ho106= true;
			if (m.equals("onCreateContextMenu")) __ho110= true;
			if (m.equals("onPrepareNavigateUpTaskStack")) __ho140= true;
			if (m.equals("getLastNonConfigurationInstance")) __ho61= true;
			if (m.equals("onKeyDown")) __ho122= true;
			if (m.equals("sendBroadcast")) __ho174= true;
			if (m.equals("unbindService")) __ho212= true;
			if (m.equals("createPendingResult")) __ho15= true;
			if (m.equals("getBaseContext")) __ho44= true;
			if (m.equals("registerForContextMenu")) __ho169= true;
			if (m.equals("enforcePermission")) __ho30= true;
			if (m.equals("checkUriPermission")) __ho8= true;
			if (m.equals("dump")) __ho25= true;
			if (m.equals("startIntentSenderFromChild")) __ho202= true;
			if (m.equals("onCreate")) __ho109= true;
			if (m.equals("clearWallpaper")) __ho9= true;
			if (m.equals("onWindowFocusChanged")) __ho158= true;
			if (m.equals("startActivityFromChild")) __ho196= true;
			if (m.equals("onMenuItemSelected")) __ho128= true;
			if (m.equals("onCreateDescription")) __ho111= true;
			if (m.equals("getApplicationInfo")) __ho42= true;
			if (m.equals("setContentView")) __ho182= true;
			if (m.equals("unregisterReceiver")) __ho215= true;
			if (m.equals("stopService")) __ho208= true;
			if (m.equals("onDetachedFromWindow")) __ho120= true;
		}
	}
	private boolean __ho0;
	private boolean __ho1;
	private boolean __ho2;
	private boolean __ho3;
	private boolean __ho4;
	private boolean __ho5;
	private boolean __ho6;
	private boolean __ho7;
	private boolean __ho8;
	private boolean __ho9;
	private boolean __ho10;
	private boolean __ho11;
	private boolean __ho12;
	private boolean __ho13;
	private boolean __ho14;
	private boolean __ho15;
	private boolean __ho16;
	private boolean __ho17;
	private boolean __ho18;
	private boolean __ho19;
	private boolean __ho20;
	private boolean __ho21;
	private boolean __ho22;
	private boolean __ho23;
	private boolean __ho24;
	private boolean __ho25;
	private boolean __ho26;
	private boolean __ho27;
	private boolean __ho28;
	private boolean __ho29;
	private boolean __ho30;
	private boolean __ho31;
	private boolean __ho32;
	private boolean __ho33;
	private boolean __ho34;
	private boolean __ho35;
	private boolean __ho36;
	private boolean __ho37;
	private boolean __ho38;
	private boolean __ho39;
	private boolean __ho40;
	private boolean __ho41;
	private boolean __ho42;
	private boolean __ho43;
	private boolean __ho44;
	private boolean __ho45;
	private boolean __ho46;
	private boolean __ho47;
	private boolean __ho48;
	private boolean __ho49;
	private boolean __ho50;
	private boolean __ho51;
	private boolean __ho52;
	private boolean __ho53;
	private boolean __ho54;
	private boolean __ho55;
	private boolean __ho56;
	private boolean __ho57;
	private boolean __ho58;
	private boolean __ho59;
	private boolean __ho60;
	private boolean __ho61;
	private boolean __ho62;
	private boolean __ho63;
	private boolean __ho64;
	private boolean __ho65;
	private boolean __ho66;
	private boolean __ho67;
	private boolean __ho68;
	private boolean __ho69;
	private boolean __ho70;
	private boolean __ho71;
	private boolean __ho72;
	private boolean __ho73;
	private boolean __ho74;
	private boolean __ho75;
	private boolean __ho76;
	private boolean __ho77;
	private boolean __ho78;
	private boolean __ho79;
	private boolean __ho80;
	private boolean __ho81;
	private boolean __ho82;
	private boolean __ho83;
	private boolean __ho84;
	private boolean __ho85;
	private boolean __ho86;
	private boolean __ho87;
	private boolean __ho88;
	private boolean __ho89;
	private boolean __ho90;
	private boolean __ho91;
	private boolean __ho92;
	private boolean __ho93;
	private boolean __ho94;
	private boolean __ho95;
	private boolean __ho96;
	private boolean __ho97;
	private boolean __ho98;
	private boolean __ho99;
	private boolean __ho100;
	private boolean __ho101;
	private boolean __ho102;
	private boolean __ho103;
	private boolean __ho104;
	private boolean __ho105;
	private boolean __ho106;
	private boolean __ho107;
	private boolean __ho108;
	private boolean __ho109;
	private boolean __ho110;
	private boolean __ho111;
	private boolean __ho112;
	private boolean __ho113;
	private boolean __ho114;
	private boolean __ho115;
	private boolean __ho116;
	private boolean __ho117;
	private boolean __ho118;
	private boolean __ho119;
	private boolean __ho120;
	private boolean __ho121;
	private boolean __ho122;
	private boolean __ho123;
	private boolean __ho124;
	private boolean __ho125;
	private boolean __ho126;
	private boolean __ho127;
	private boolean __ho128;
	private boolean __ho129;
	private boolean __ho130;
	private boolean __ho131;
	private boolean __ho132;
	private boolean __ho133;
	private boolean __ho134;
	private boolean __ho135;
	private boolean __ho136;
	private boolean __ho137;
	private boolean __ho138;
	private boolean __ho139;
	private boolean __ho140;
	private boolean __ho141;
	private boolean __ho142;
	private boolean __ho143;
	private boolean __ho144;
	private boolean __ho145;
	private boolean __ho146;
	private boolean __ho147;
	private boolean __ho148;
	private boolean __ho149;
	private boolean __ho150;
	private boolean __ho151;
	private boolean __ho152;
	private boolean __ho153;
	private boolean __ho154;
	private boolean __ho155;
	private boolean __ho156;
	private boolean __ho157;
	private boolean __ho158;
	private boolean __ho159;
	private boolean __ho160;
	private boolean __ho161;
	private boolean __ho162;
	private boolean __ho163;
	private boolean __ho164;
	private boolean __ho165;
	private boolean __ho166;
	private boolean __ho167;
	private boolean __ho168;
	private boolean __ho169;
	private boolean __ho170;
	private boolean __ho171;
	private boolean __ho172;
	private boolean __ho173;
	private boolean __ho174;
	private boolean __ho175;
	private boolean __ho176;
	private boolean __ho177;
	private boolean __ho178;
	private boolean __ho179;
	private boolean __ho180;
	private boolean __ho181;
	private boolean __ho182;
	private boolean __ho183;
	private boolean __ho184;
	private boolean __ho185;
	private boolean __ho186;
	private boolean __ho187;
	private boolean __ho188;
	private boolean __ho189;
	private boolean __ho190;
	private boolean __ho191;
	private boolean __ho192;
	private boolean __ho193;
	private boolean __ho194;
	private boolean __ho195;
	private boolean __ho196;
	private boolean __ho197;
	private boolean __ho198;
	private boolean __ho199;
	private boolean __ho200;
	private boolean __ho201;
	private boolean __ho202;
	private boolean __ho203;
	private boolean __ho204;
	private boolean __ho205;
	private boolean __ho206;
	private boolean __ho207;
	private boolean __ho208;
	private boolean __ho209;
	private boolean __ho210;
	private boolean __ho211;
	private boolean __ho212;
	private boolean __ho213;
	private boolean __ho214;
	private boolean __ho215;
}

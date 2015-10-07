/*
 * Warning: this file may be auto-generated in future. Edit with caution.
 */
package com.tns;

import android.app.Application;

@JavaScriptImplementation(javaScriptFile = "app/myapp.js")
public class MyApp extends android.app.Application implements com.tns.NativeScriptHashCodeProvider
{
	public MyApp()
	{
	}

	protected void attachBaseContext(android.content.Context param_0) {
		super.attachBaseContext(param_0);
		
		if (Util.isDebuggableApp(param_0))
		{
			// TODO:
			//android.os.Debug.waitForDebugger();
		}
		
		new RuntimeHelper(this).initRuntime();
		
		Platform.initInstance(this);
	}
	
	public void onCreate()
	{
		appInstance = this;
		java.lang.Object[] params = null;
		com.tns.Platform.callJSMethod(this, "onCreate", void.class, params);
	}
	
	private static Application appInstance;
	public static Application getInstance(){
		return appInstance;
	}
	
	public boolean equals__super(java.lang.Object other) {
		return super.equals(other);
	}
	public int hashCode__super() {
		return super.hashCode();
	}
}

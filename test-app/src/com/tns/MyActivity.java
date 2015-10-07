/*
 * Warning: this file may be auto-generated in future. Edit with caution.
 */
package com.tns;

@JavaScriptImplementation(javaScriptFile = "app/myactivity.js")
public class MyActivity extends android.app.Activity
{
	public MyActivity()
	{
		com.tns.Platform.initInstance(this);
	}

	protected void onCreate(android.os.Bundle param_0)
	{
		Object[] args = new Object[1];
		args[0] = param_0;
		com.tns.Platform.callJSMethod(this, "onCreate", void.class, args);
	}
}

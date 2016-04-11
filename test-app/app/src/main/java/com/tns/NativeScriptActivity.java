/*
 * Warning: this file may be auto-generated in future. Edit with caution.
 */
package com.tns;

@JavaScriptImplementation(javaScriptFile = "./MyActivity.js")
public class NativeScriptActivity extends android.app.Activity
{
	public NativeScriptActivity()
	{
		com.tns.Runtime.initInstance(this);
	}

	protected void onCreate(android.os.Bundle param_0)
	{
		Object[] args = new Object[1];
		args[0] = param_0;
		com.tns.Runtime.callJSMethod(this, "onCreate", void.class, args);
	}
}
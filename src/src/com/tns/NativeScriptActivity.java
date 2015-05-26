package com.tns;

import android.util.Log;

public class NativeScriptActivity extends android.app.Activity implements com.tns.NativeScriptHashCodeProvider {
	public NativeScriptActivity() {
	}
	
	public int hashCode__super() {
		return super.hashCode();
	}
	
	public boolean equals__super(Object other) {
		return super.equals(other);
	}

	protected void onCreate(android.os.Bundle param_0) {
		if(ErrorReport.HasApplicationCreateError)
		{
			super.onCreate(param_0);
			this.finish();
			return;
		}
		
		Log.d(Platform.DEFAULT_LOG_TAG, "Creating Activity with intent: " + super.getIntent().toString() + " action:" + super.getIntent().getAction());
		
		if (BuildConfig.DEBUG && super.getIntent().getAction() == null)
		{
			super.getIntent().setAction(android.content.Intent.ACTION_MAIN);
		}
		com.tns.Platform.initInstance(this, super.getIntent());

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
			com.tns.Platform.callJSMethod(this, "onCreate", params);
		}
	}
}

package com.tns;

@JavaScriptImplementation(javaScriptFile = "./tns_modules/ui/frame/activity.js")
public class NativeScriptActivity extends android.app.Activity implements NativeScriptHashCodeProvider {
	public NativeScriptActivity(){
		super();
		Runtime.initInstance(this);
	}

	protected void onCreate(android.os.Bundle param_0)  {
		Object[] args = new Object[1];
		args[0] = param_0;
		Runtime.callJSMethod(this, "onCreate", void.class, args);
	}

	public void onCreate(android.os.Bundle param_0, android.os.PersistableBundle param_1)  {
		Object[] args = new Object[2];
		args[0] = param_0;
		args[1] = param_1;
		Runtime.callJSMethod(this, "onCreate", void.class, args);
	}

	protected void onSaveInstanceState(android.os.Bundle param_0)  {
		Object[] args = new Object[1];
		args[0] = param_0;
		Runtime.callJSMethod(this, "onSaveInstanceState", void.class, args);
	}

	public void onSaveInstanceState(android.os.Bundle param_0, android.os.PersistableBundle param_1)  {
		Object[] args = new Object[2];
		args[0] = param_0;
		args[1] = param_1;
		Runtime.callJSMethod(this, "onSaveInstanceState", void.class, args);
	}

	protected void onStart()  {
		Object[] args = null;
		Runtime.callJSMethod(this, "onStart", void.class, args);
	}

	protected void onStop()  {
		Object[] args = null;
		Runtime.callJSMethod(this, "onStop", void.class, args);
	}

	protected void onDestroy()  {
		Object[] args = null;
		Runtime.callJSMethod(this, "onDestroy", void.class, args);
	}

	public void onBackPressed()  {
		Object[] args = null;
		Runtime.callJSMethod(this, "onBackPressed", void.class, args);
	}

	public void onRequestPermissionsResult(int param_0, String[] param_1, int[] param_2)  {
		Object[] args = new Object[3];
		args[0] = param_0;
		args[1] = param_1;
		args[2] = param_2;
		Runtime.callJSMethod(this, "onRequestPermissionsResult", void.class, args);
	}

	protected void onActivityResult(int param_0, int param_1, android.content.Intent param_2)  {
		Object[] args = new Object[3];
		args[0] = param_0;
		args[1] = param_1;
		args[2] = param_2;
		Runtime.callJSMethod(this, "onActivityResult", void.class, args);
	}

	public boolean equals__super(Object other) {
		return super.equals(other);
	}

	public int hashCode__super() {
		return super.hashCode();
	}

}

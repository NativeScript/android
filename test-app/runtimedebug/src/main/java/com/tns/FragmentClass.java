package com.tns;

@JavaScriptImplementation(javaScriptFile = "./tns_modules/ui/frame/fragment.js")
public class FragmentClass extends android.app.Fragment implements NativeScriptHashCodeProvider {
	public FragmentClass(){
		super();
		Runtime.initInstance(this);
	}

	public void onHiddenChanged(boolean param_0)  {
		Object[] args = new Object[1];
		args[0] = param_0;
		Runtime.callJSMethod(this, "onHiddenChanged", void.class, args);
	}

	public android.animation.Animator onCreateAnimator(int param_0, boolean param_1, int param_2)  {
		Object[] args = new Object[3];
		args[0] = param_0;
		args[1] = param_1;
		args[2] = param_2;
		return (android.animation.Animator) Runtime.callJSMethod(this, "onCreateAnimator", android.animation.Animator.class, args);
	}

	public void onCreate(android.os.Bundle param_0)  {
		Object[] args = new Object[1];
		args[0] = param_0;
		Runtime.callJSMethod(this, "onCreate", void.class, args);
	}

	public android.view.View onCreateView(android.view.LayoutInflater param_0, android.view.ViewGroup param_1, android.os.Bundle param_2)  {
		Object[] args = new Object[3];
		args[0] = param_0;
		args[1] = param_1;
		args[2] = param_2;
		return (android.view.View) Runtime.callJSMethod(this, "onCreateView", android.view.View.class, args);
	}

	public void onSaveInstanceState(android.os.Bundle param_0)  {
		Object[] args = new Object[1];
		args[0] = param_0;
		Runtime.callJSMethod(this, "onSaveInstanceState", void.class, args);
	}

	public void onDestroyView()  {
		Object[] args = null;
		Runtime.callJSMethod(this, "onDestroyView", void.class, args);
	}

	public void onDestroy()  {
		Object[] args = null;
		Runtime.callJSMethod(this, "onDestroy", void.class, args);
	}

	public String toString()  {
		Object[] args = null;
		return (String) Runtime.callJSMethod(this, "toString", String.class, args);
	}

	public boolean equals__super(Object other) {
		return super.equals(other);
	}

	public int hashCode__super() {
		return super.hashCode();
	}

}

package com.tns;

public interface Debugger
{
	void start();
	
	void disableAgent();
	
	void enableAgent();
	
	void onConnect(JsDebugger context);
}

package com.tns.tests;

public abstract class MyAbstractClass
{
	protected abstract String echoString(String s);
	
	public String echo(String s)
	{
		return echoString(s);
	}
}

package com.tns;

public class NativeScriptException extends RuntimeException
{
	@SuppressWarnings("unused")
	private long jsValueAddress = 0;
	
	public NativeScriptException()
	{
		super();
	}
	
	public NativeScriptException(String detailMessage)
	{
		super(detailMessage);
	}

	public NativeScriptException(Throwable throwable)
	{
		super(throwable);
	}

	public NativeScriptException(String detailMessage, Throwable throwable)
	{
		super(detailMessage, throwable);
	}
	
	public NativeScriptException(String detailMessage, long jsValueAddress)
	{
		super(detailMessage);
		this.jsValueAddress = jsValueAddress;
	}
}

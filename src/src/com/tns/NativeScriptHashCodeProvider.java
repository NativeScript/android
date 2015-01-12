package com.tns;

public interface NativeScriptHashCodeProvider
{
	int hashCode__super();

	boolean equals__super(Object other);
	
	void setNativeScriptOverrides(String[] overrides);
}

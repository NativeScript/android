package com.telerik.metadata;

import com.telerik.metadata.desc.ClassDescriptor;

import java.util.Map;

public interface ClassMapProvider {
	Map<String, ClassDescriptor> getClassMap();
	
	String getPath();
}

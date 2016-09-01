package com.telerik.metadata;

import java.util.Map;

import org.apache.bcel.classfile.JavaClass;

public interface ClassMapProvider {
	Map<String, JavaClass> getClassMap();
	
	String getPath();
}

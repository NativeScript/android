package com.telerik.metadata;

import com.telerik.metadata.parsing.classes.NativeClassDescriptor;

import java.util.Map;

public interface ClassMapProvider {
    Map<String, NativeClassDescriptor> getClassMap();

    String getPath();
}

package com.telerik.metadata;

import com.telerik.metadata.parsing.NativeClassDescriptor;

import java.util.Map;

public interface ClassMapProvider {
    Map<String, NativeClassDescriptor> getClassMap();

    String getPath();
}

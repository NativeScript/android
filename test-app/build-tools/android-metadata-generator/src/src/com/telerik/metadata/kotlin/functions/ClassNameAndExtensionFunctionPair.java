package com.telerik.metadata.kotlin.functions;

import com.telerik.metadata.desc.ExtensionFunctionDescriptor;

public final class ClassNameAndExtensionFunctionPair {
    private final String className;
    private final ExtensionFunctionDescriptor extensionFunction;

    public ClassNameAndExtensionFunctionPair(String className, ExtensionFunctionDescriptor extensionFunction) {
        this.className = className;
        this.extensionFunction = extensionFunction;
    }

    public String getClassName() {
        return className;
    }

    public ExtensionFunctionDescriptor getExtensionFunction() {
        return extensionFunction;
    }
}

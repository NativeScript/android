package com.telerik.metadata.kotlin.functions;

import com.telerik.metadata.desc.ExtensionFunctionDescriptor;

import java.util.Collection;

public interface ExtensionFunctionsStorage {
    void storeExtensionFunctions(Collection<ClassNameAndExtensionFunctionPair> extensionFunctions);

    Collection<ExtensionFunctionDescriptor> retrieveExtensionFunctions(String className);
}

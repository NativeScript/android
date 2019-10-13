package com.telerik.metadata.kotlin.functions;

import com.telerik.metadata.desc.ClassDescriptor;
import java.util.Collection;

public interface ExtensionFunctionsCollector {
    Collection<ClassNameAndExtensionFunctionPair> collectExtensionFunctions(ClassDescriptor classDescriptor);
}

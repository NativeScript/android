package com.telerik.metadata.kotlin.functions.impl;

import com.telerik.metadata.desc.ExtensionFunctionDescriptor;
import com.telerik.metadata.kotlin.functions.ClassNameAndExtensionFunctionPair;
import com.telerik.metadata.kotlin.functions.ExtensionFunctionsStorage;

import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class ExtensionFunctionsStorageImpl implements ExtensionFunctionsStorage {

    private static final ExtensionFunctionsStorageImpl ourInstance = new ExtensionFunctionsStorageImpl();

    public static ExtensionFunctionsStorage getInstance() {
        return ourInstance;
    }

    private final Map<String, Collection<ExtensionFunctionDescriptor>> extensionFunctions;

    private ExtensionFunctionsStorageImpl() {
        this.extensionFunctions = new HashMap<>();
    }

    @Override
    public void storeExtensionFunctions(Collection<ClassNameAndExtensionFunctionPair> extensionFunctions) {
        Map<String, List<ExtensionFunctionDescriptor>> classConcreteExtensionFunctions = extensionFunctions
                .stream()
                .collect(Collectors
                        .groupingBy(ClassNameAndExtensionFunctionPair::getClassName,
                                Collectors.mapping(ClassNameAndExtensionFunctionPair::getExtensionFunction,
                                        Collectors.toList())));

        for (Map.Entry<String, List<ExtensionFunctionDescriptor>> classConcreteExtensionFunction : classConcreteExtensionFunctions.entrySet()) {
            String className = classConcreteExtensionFunction.getKey();

            if (this.extensionFunctions.containsKey(className)) {
                Collection<ExtensionFunctionDescriptor> existingExtensionFunctions = this.extensionFunctions.get(className);
                existingExtensionFunctions.addAll(classConcreteExtensionFunction.getValue());
            } else {
                this.extensionFunctions.put(className, classConcreteExtensionFunction.getValue());
            }
        }
    }

    @Override
    public Collection<ExtensionFunctionDescriptor> retrieveExtensionFunctions(String className) {
        Collection<ExtensionFunctionDescriptor> classConcreteExtensionFunctions = extensionFunctions.get(className);

        if (classConcreteExtensionFunctions != null) {
            return classConcreteExtensionFunctions;
        }

        return Collections.emptyList();
    }

}

package com.telerik.metadata.storage.functions.extensions;

import com.telerik.metadata.parsing.kotlin.extensions.KotlinExtensionFunctionDescriptor;
import com.telerik.metadata.parsing.kotlin.extensions.ClassNameAndFunctionPair;
import com.telerik.metadata.storage.functions.FunctionsStorage;

import org.jetbrains.annotations.NotNull;

import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class ExtensionFunctionsStorage implements FunctionsStorage<KotlinExtensionFunctionDescriptor> {

    private static final ExtensionFunctionsStorage ourInstance = new ExtensionFunctionsStorage();

    public static FunctionsStorage<KotlinExtensionFunctionDescriptor> getInstance() {
        return ourInstance;
    }

    private final Map<String, Collection<KotlinExtensionFunctionDescriptor>> extensionFunctions;

    private ExtensionFunctionsStorage() {
        this.extensionFunctions = new HashMap<>();
    }

    @Override
    public void storeFunctions(Collection<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>> extensionFunctions) {
        Map<String, List<KotlinExtensionFunctionDescriptor>> classConcreteExtensionFunctions = extensionFunctions
                .stream()
                .collect(Collectors
                        .groupingBy(ClassNameAndFunctionPair::getClassName,
                                Collectors.mapping(ClassNameAndFunctionPair::getFunction,
                                        Collectors.toList())));

        for (Map.Entry<String, List<KotlinExtensionFunctionDescriptor>> classConcreteExtensionFunction : classConcreteExtensionFunctions.entrySet()) {
            String className = classConcreteExtensionFunction.getKey();

            if (this.extensionFunctions.containsKey(className)) {
                Collection<KotlinExtensionFunctionDescriptor> existingExtensionFunctions = this.extensionFunctions.get(className);
                existingExtensionFunctions.addAll(classConcreteExtensionFunction.getValue());
            } else {
                this.extensionFunctions.put(className, classConcreteExtensionFunction.getValue());
            }
        }
    }

    @NotNull
    @Override
    public Collection<KotlinExtensionFunctionDescriptor> retrieveFunctions(@NotNull String className) {
        Collection<KotlinExtensionFunctionDescriptor> classConcreteExtensionFunctions = extensionFunctions.get(className);

        if (classConcreteExtensionFunctions != null) {
            return classConcreteExtensionFunctions;
        }

        return Collections.emptyList();
    }

}

package com.telerik.metadata.kotlin.functions.impl;

import com.telerik.metadata.ClassUtil;
import com.telerik.metadata.bcl.ExtensionFunctionInfo;
import com.telerik.metadata.desc.ClassDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;
import com.telerik.metadata.kotlin.classes.KotlinClassMetadataParser;
import com.telerik.metadata.kotlin.functions.ClassNameAndExtensionFunctionPair;
import com.telerik.metadata.kotlin.functions.ExtensionFunctionsCollector;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import kotlinx.metadata.KmFunction;
import kotlinx.metadata.jvm.JvmExtensionsKt;
import kotlinx.metadata.jvm.JvmMethodSignature;

public class ExtensionFunctionsCollectorImpl implements ExtensionFunctionsCollector {

    private final KotlinClassMetadataParser kotlinClassMetadataParser;

    public ExtensionFunctionsCollectorImpl(KotlinClassMetadataParser kotlinClassMetadataParser) {
        this.kotlinClassMetadataParser = kotlinClassMetadataParser;
    }

    @Override
    public Collection<ClassNameAndExtensionFunctionPair> collectExtensionFunctions(ClassDescriptor classDescriptor) {
        Collection<ClassNameAndExtensionFunctionPair> extensionFunctionsDescriptors = new ArrayList<>();

        List<KmFunction> extensionFunctions = kotlinClassMetadataParser.getKotlinExtensionFunctions(classDescriptor);

        for (KmFunction extensionFunction : extensionFunctions) {
            JvmMethodSignature signature = JvmExtensionsKt.getSignature(extensionFunction);

            String functionName = signature.getName();
            String functionSignature = signature.getDesc();

            MethodDescriptor extensionFunctionDescriptor = Arrays
                    .stream(classDescriptor.getMethods())
                    .filter(x -> x.getName().equals(functionName) && x.getSignature().equals(functionSignature))
                    .findFirst()
                    .get();

            if (extensionFunctionDescriptor.isStatic()) {
                TypeDescriptor receiverType = extensionFunctionDescriptor.getArgumentTypes()[0]; // kotlin extension functions' first argument is the receiver type
                String receiverClassName = ClassUtil.getCanonicalName(receiverType.getSignature());
                extensionFunctionsDescriptors.add(new ClassNameAndExtensionFunctionPair(receiverClassName, new ExtensionFunctionInfo(extensionFunctionDescriptor)));
            }
        }

        return extensionFunctionsDescriptors;
    }
}

package com.telerik.metadata.kotlin.classes;

import com.telerik.metadata.desc.ClassDescriptor;

import java.util.List;

import kotlinx.metadata.KmFunction;
import kotlinx.metadata.KmProperty;

public interface KotlinClassMetadataParser {
    boolean wasKotlinClass(ClassDescriptor clazz);
    boolean wasKotlinCompanionObject(ClassDescriptor clazz, ClassDescriptor possibleCompanion);
    List<KmProperty> getKotlinProperties(ClassDescriptor clazz);
    List<KmFunction> getKotlinExtensionFunctions(ClassDescriptor clazz);
}

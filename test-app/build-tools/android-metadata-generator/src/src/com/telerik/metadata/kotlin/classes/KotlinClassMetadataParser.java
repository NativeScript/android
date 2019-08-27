package com.telerik.metadata.kotlin.classes;

import com.telerik.metadata.desc.ClassDescriptor;

import java.util.List;

import kotlinx.metadata.KmProperty;

public interface KotlinClassMetadataParser {
    boolean wasKotlinCompanionObject(ClassDescriptor clazz, ClassDescriptor possibleCompanion);
    List<KmProperty> getKotlinProperties(ClassDescriptor clazz);
}

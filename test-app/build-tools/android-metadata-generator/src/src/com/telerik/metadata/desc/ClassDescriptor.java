package com.telerik.metadata.desc;

import java.util.Optional;

public interface ClassDescriptor {
    boolean isClass();

    boolean isInterface();

    boolean isEnum();

    boolean isStatic();

    boolean isPublic();

    boolean isProtected();

    MethodDescriptor[] getMethods();

    PropertyDescriptor[] getProperties();

    FieldDescriptor[] getFields();

    MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation();

    Optional<KotlinClassMetadataAnnotation> getKotlinClassMetadataAnnotation();

    String[] getInterfaceNames();

    String getPackageName();

    String getClassName();

    String getSuperclassName();
}

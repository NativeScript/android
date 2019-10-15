package com.telerik.metadata.desc;

public interface MethodDescriptor {
    boolean isPublic();
    boolean isProtected();
    boolean isSynthetic();
    boolean isStatic();
    boolean isAbstract();

    ClassDescriptor getDeclaringClass();

    String getName();
    String getSignature();

    TypeDescriptor[] getArgumentTypes();
    TypeDescriptor getReturnType();

    MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation();
}

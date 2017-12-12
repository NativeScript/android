package com.telerik.metadata.desc;

public interface ClassDescriptor {
    boolean isClass();
    boolean isInterface();
    boolean isEnum();

    boolean isStatic();
    boolean isPublic();
    boolean isProtected();

    MethodDescriptor[] getMethods();

    FieldDescriptor[] getFields();

    MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation();

    String[] getInterfaceNames();

    String getPackageName();
    String getClassName();
    String getSuperclassName();
}

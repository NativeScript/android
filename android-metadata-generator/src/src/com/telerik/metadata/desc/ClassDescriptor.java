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

    String[] getInterfaceNames();

    String getPackageName();
    String getClassName();
    String getSuperclassName();
}

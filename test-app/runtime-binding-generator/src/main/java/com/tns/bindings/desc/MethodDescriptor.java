package com.tns.bindings.desc;

public interface MethodDescriptor extends Descriptor {
    ClassDescriptor[] getParameterTypes();

    String getName();

    ClassDescriptor getReturnType();

    String toGenericString();

    boolean isInterfaceMethod();
    void setAsInterfaceMethod();
}

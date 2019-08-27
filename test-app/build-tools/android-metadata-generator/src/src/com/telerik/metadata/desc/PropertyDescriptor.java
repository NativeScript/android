package com.telerik.metadata.desc;

public interface PropertyDescriptor {
    String getName();
    MethodDescriptor getGetterMethod();
    MethodDescriptor getSetterMethod();
}

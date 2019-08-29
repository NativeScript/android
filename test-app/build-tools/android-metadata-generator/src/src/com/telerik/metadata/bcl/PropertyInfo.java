package com.telerik.metadata.bcl;

import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.PropertyDescriptor;

public class PropertyInfo implements PropertyDescriptor {

    private final String name;
    private final MethodDescriptor getter;
    private final MethodDescriptor setter;

    public PropertyInfo(String name, MethodDescriptor getter, MethodDescriptor setter) {
        this.name = name;
        this.getter = getter;
        this.setter = setter;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public MethodDescriptor getGetterMethod() {
        return getter;
    }

    @Override
    public MethodDescriptor getSetterMethod() {
        return setter;
    }
}

package com.telerik.metadata.desc;

public interface FieldDescriptor {
    boolean isPublic();
    boolean isProtected();
    boolean isFinal();
    boolean isStatic();
    String getName();
    TypeDescriptor getType();
}

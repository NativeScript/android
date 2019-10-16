package com.telerik.metadata.bcl;

import com.telerik.metadata.desc.ClassDescriptor;
import com.telerik.metadata.desc.ExtensionFunctionDescriptor;
import com.telerik.metadata.desc.MetadataInfoAnnotationDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;

public class ExtensionFunctionInfo implements ExtensionFunctionDescriptor {
    private final MethodDescriptor m;

    public ExtensionFunctionInfo(MethodDescriptor m) {
        this.m = m;
    }

    @Override
    public boolean isPublic() {
        return m.isPublic();
    }

    @Override
    public boolean isProtected() {
        return m.isProtected();
    }

    @Override
    public boolean isSynthetic() {
        return m.isSynthetic();
    }

    @Override
    public boolean isStatic() {
        return m.isStatic();
    }

    @Override
    public boolean isAbstract() {
        return m.isAbstract();
    }

    @Override
    public ClassDescriptor getDeclaringClass() {
        return m.getDeclaringClass();
    }

    @Override
    public String getName() {
        return m.getName();
    }

    @Override
    public String getSignature() {
        return m.getSignature();
    }

    @Override
    public TypeDescriptor[] getArgumentTypes() {
        return m.getArgumentTypes();
    }

    @Override
    public TypeDescriptor getReturnType() {
        return m.getReturnType();
    }

    @Override
    public MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation() {
        return null;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        ExtensionFunctionInfo that = (ExtensionFunctionInfo) o;

        return m.equals(that.m);
    }

    @Override
    public int hashCode() {
        return m.hashCode();
    }
}

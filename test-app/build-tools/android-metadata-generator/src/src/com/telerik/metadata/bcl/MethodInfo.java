package com.telerik.metadata.bcl;

import com.telerik.metadata.desc.MetadataInfoAnnotationDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;

public class MethodInfo implements MethodDescriptor {
    private final Method m;

    public MethodInfo(Method m) {
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
    public String getName() {
        return m.getName();
    }

    @Override
    public String getSignature() {
        return m.getSignature();
    }

    @Override
    public TypeDescriptor[] getArgumentTypes() {
        Type[] ts = m.getArgumentTypes();
        TypeDescriptor[] argTypes = new TypeDescriptor[ts.length];
        for (int i=0; i<argTypes.length; i++) {
            argTypes[i] = new TypeInfo(ts[i]);
        }
        return argTypes;
    }

    @Override
    public TypeDescriptor getReturnType() {
        return new TypeInfo(m.getReturnType());
    }

    @Override
    public MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation() {
        return null;
    }
}

package com.telerik.metadata.bcl;

import com.telerik.metadata.desc.TypeDescriptor;

import org.apache.bcel.generic.Type;

public class TypeInfo implements TypeDescriptor {
    private final Type t;

    public TypeInfo(Type t) {
        this.t = t;
    }

    @Override
    public String getSignature() {
        return t.getSignature();
    }

    @Override
    public int hashCode() {
        return getSignature().hashCode();
    }

    @Override
    public boolean equals(Object other) {
        if ((other != null) && (other instanceof TypeDescriptor)) {
            TypeDescriptor otherTypeDesc = (TypeDescriptor)other;
            return this.getSignature().equals(otherTypeDesc.getSignature());
        }
        return false;
    }
}

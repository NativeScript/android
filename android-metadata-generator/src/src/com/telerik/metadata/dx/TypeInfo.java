package com.telerik.metadata.dx;

import com.telerik.metadata.desc.TypeDescriptor;

public class TypeInfo implements TypeDescriptor {
    private final String signature;

    public TypeInfo(String signature) {
        this.signature = signature;
    }

    @Override
    public String getSignature() {
        return signature;
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

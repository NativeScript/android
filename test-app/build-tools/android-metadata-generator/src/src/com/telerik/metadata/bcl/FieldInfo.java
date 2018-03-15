package com.telerik.metadata.bcl;

import com.telerik.metadata.desc.FieldDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;

import org.apache.bcel.classfile.Field;

public class FieldInfo implements FieldDescriptor {
    private final Field f;

    public FieldInfo(Field f) {
        this.f = f;
    }

    @Override
    public boolean isPublic() {
        return f.isPublic();
    }

    @Override
    public boolean isProtected() {
        return f.isProtected();
    }

    @Override
    public boolean isFinal() {
        return f.isFinal();
    }

    @Override
    public boolean isStatic() {
        return f.isStatic();
    }

    @Override
    public String getName() {
        return f.getName();
    }

    @Override
    public TypeDescriptor getType() {
        return new TypeInfo(f.getType());
    }
}

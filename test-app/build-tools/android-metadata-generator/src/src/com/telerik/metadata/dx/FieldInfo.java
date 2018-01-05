package com.telerik.metadata.dx;

import com.android.dex.ClassData;
import com.android.dex.Dex;
import com.android.dex.FieldId;
import com.android.dx.rop.code.AccessFlags;
import com.telerik.metadata.desc.FieldDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;

import java.util.List;

public class FieldInfo implements FieldDescriptor {
    private final DexFile dexFile;
    private final ClassData.Field field;

    public FieldInfo(DexFile dexFile, ClassData.Field field) {
        this.dexFile = dexFile;
        this.field = field;
    }

    @Override
    public boolean isPublic() {
        return AccessFlags.isPublic(field.getAccessFlags());
    }

    @Override
    public boolean isProtected() {
        return AccessFlags.isProtected(field.getAccessFlags());
    }

    @Override
    public boolean isFinal() {
        boolean isFinal = (field.getAccessFlags() & AccessFlags.ACC_FINAL) == AccessFlags.ACC_FINAL;
        return isFinal;
    }

    @Override
    public boolean isStatic() {
        return AccessFlags.isStatic(field.getAccessFlags());
    }

    @Override
    public String getName() {
        Dex dex = dexFile.getDex();
        List<String> strings = dex.strings();
        int idx = dex.nameIndexFromFieldIndex(field.getFieldIndex());
        String fieldName = strings.get(idx);
        return fieldName;
    }

    @Override
    public TypeDescriptor getType() {
        Dex dex = dexFile.getDex();
        List<String> typeNames = dex.typeNames();
        FieldId fieldId = dex.fieldIds().get(field.getFieldIndex());
        int typeIdx = fieldId.getTypeIndex();
        String retTypeName = typeNames.get(typeIdx);
        return new TypeInfo(retTypeName);
    }
}

package com.telerik.metadata.dx;

import com.android.dex.Annotation;
import com.android.dex.ClassData;
import com.android.dex.ClassDef;
import com.android.dex.Dex;
import com.android.dx.rop.code.AccessFlags;
import com.android.dx.rop.cst.CstType;
import com.android.dx.rop.type.Type;
import com.telerik.metadata.desc.MetadataInfoAnnotationDescriptor;
import com.telerik.metadata.desc.ClassDescriptor;
import com.telerik.metadata.desc.FieldDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

public class ClassInfo implements ClassDescriptor {
    private final DexFile dexFile;
    private final ClassDef classDef;

    private final Comparator<MethodDescriptor> METHOD_NAME_COMPARATOR = new Comparator<MethodDescriptor>() {
        @Override
        public int compare(MethodDescriptor m1, MethodDescriptor m2) {
            return m1.getName().compareTo(m2.getName());
        }
    };

    public ClassInfo(DexFile dexFile, ClassDef classDef) {
        this.dexFile = dexFile;
        this.classDef = classDef;
    }

    @Override
    public boolean isClass() {
        return !isInterface() && !isEnum();
    }

    @Override
    public boolean isInterface() {
        return AccessFlags.isInterface(classDef.getAccessFlags());
    }

    @Override
    public boolean isEnum() {
        return AccessFlags.isEnum(classDef.getAccessFlags());
    }

    @Override
    public boolean isStatic() {
        return AccessFlags.isStatic(classDef.getAccessFlags());
    }

    @Override
    public boolean isPublic() {
        return AccessFlags.isPublic(classDef.getAccessFlags());
    }

    @Override
    public boolean isProtected() {
        return AccessFlags.isProtected(classDef.getAccessFlags());
    }

    @Override
    public MethodDescriptor[] getMethods() {
        MethodDescriptor[] methods;
        Dex dex = dexFile.getDex();
        int off = classDef.getClassDataOffset();
        if (off != 0) {
            ClassData data = dex.readClassData(classDef);
            ClassData.Method[] allMethods = data.allMethods();
            methods = new MethodDescriptor[allMethods.length];
            for (int i=0; i<allMethods.length; i++) {
                methods[i] = new MethodInfo(dexFile, allMethods[i]);
            }
        } else {
            methods = new MethodDescriptor[0];
        }
        Arrays.sort(methods, METHOD_NAME_COMPARATOR);

        return methods;
    }

    @Override
    public FieldDescriptor[] getFields() {
        FieldDescriptor[] fields;
        Dex dex = dexFile.getDex();
        int off = classDef.getClassDataOffset();
        if (off != 0) {
            ClassData data = dex.readClassData(classDef);
            ClassData.Field[] allFields = data.allFields();
            fields = new FieldDescriptor[allFields.length];
            for (int i=0; i<fields.length; i++) {
                fields[i] = new FieldInfo(dexFile, allFields[i]);
            }
        } else {
            fields = new FieldDescriptor[0];
        }

        return fields;
    }

    @Override
    public MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation() {
        int annotationsOffset = classDef.getAnnotationsOffset();
        if (annotationsOffset == 0) {
            return null;
        }

        Dex dex = dexFile.getDex();

        int classDefIndex = dex.findClassDefIndexFromTypeIndex(classDef.getTypeIndex());
        int dexAnnotationDirectoryOffset = dex.annotationDirectoryOffsetFromClassDefIndex(classDefIndex);
        if (dexAnnotationDirectoryOffset == 0) {
            return null;
        }
        Dex.Section dirSection = dex.open(dexAnnotationDirectoryOffset);
        int classSetOffset = dirSection.readInt();

        int annotationSetOffset = classSetOffset;

        if (annotationSetOffset == 0) {
            return null;
        }

        Dex.Section annSetSection = dex.open(annotationSetOffset);
        int size = annSetSection.readInt();
        for (int i = 0; i < size; i++) {
            int annotationOffset = annSetSection.readInt();
            Dex.Section annSection = dex.open(annotationOffset);
            Annotation annotation = annSection.readAnnotation();
            String annotationName = dex.strings().get(dex.typeIds().get(annotation.getTypeIndex()));
            if (annotationName.equals("Lcom/telerik/metadata/MetadataInfo;")) {
                return new MetadataInfoAnnotationInfo(dexFile, annotation);
            }
        }
        return null;
    }

    @Override
    public String[] getInterfaceNames() {
        ArrayList<String> names = new ArrayList<String>();
        short[] ifaceIndexes = classDef.getInterfaces();
        if ((ifaceIndexes != null) && (ifaceIndexes.length > 0)) {
            List<String> typeNames = dexFile.getDex().typeNames();
            for (short idx: ifaceIndexes) {
                String interfaceName = typeNames.get(idx);
                names.add(interfaceName);
            }
        }
        return names.toArray(new String[names.size()]);
    }

    @Override
    public String getPackageName() {
        String className = getClassName();
        String name = className.replace('.', '/');
        Type t = Type.internClassName(name);
        CstType c = CstType.intern(t);
        String packageName = c.getPackageName();
        return packageName;
    }

    @Override
    public String getClassName() {
        List<String> typeNames = dexFile.getDex().typeNames();
        int cdIdx = classDef.getTypeIndex();
        String typeName = typeNames.get(cdIdx);
        String name = typeName.substring(1, typeName.length()-1).replace('/', '.');
        return name;
    }

    @Override
    public String getSuperclassName() {
        List<String> typeNames = dexFile.getDex().typeNames();
        int idx = classDef.getSupertypeIndex();
        String superClassname = typeNames.get(idx);
        String name = superClassname.substring(1, superClassname.length()-1).replace('/', '.');
        return name;
    }
}

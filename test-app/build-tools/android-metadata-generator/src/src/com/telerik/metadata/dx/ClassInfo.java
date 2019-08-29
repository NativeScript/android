package com.telerik.metadata.dx;

import com.android.dex.Annotation;
import com.android.dex.ClassData;
import com.android.dex.ClassDef;
import com.android.dex.Dex;
import com.android.dx.rop.code.AccessFlags;
import com.android.dx.rop.cst.CstType;
import com.android.dx.rop.type.Type;
import com.telerik.metadata.desc.ClassDescriptor;
import com.telerik.metadata.desc.FieldDescriptor;
import com.telerik.metadata.desc.KotlinClassMetadataAnnotation;
import com.telerik.metadata.desc.MetadataInfoAnnotationDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.PropertyDescriptor;
import com.telerik.metadata.kotlin.classes.KotlinClassMetadataParser;
import com.telerik.metadata.kotlin.classes.impl.KotlinClassMetadataParserImpl;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Optional;

import kotlinx.metadata.KmProperty;
import kotlinx.metadata.jvm.JvmExtensionsKt;
import kotlinx.metadata.jvm.JvmMethodSignature;

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
            for (int i = 0; i < allMethods.length; i++) {
                methods[i] = new MethodInfo(dexFile, allMethods[i]);
            }
        } else {
            methods = new MethodDescriptor[0];
        }
        Arrays.sort(methods, METHOD_NAME_COMPARATOR);

        return methods;
    }

    @Override
    public PropertyDescriptor[] getProperties() {
        List<PropertyDescriptor> propertyDescriptors = new ArrayList<PropertyDescriptor>();
        KotlinClassMetadataParser kotlinClassMetadataParser = new KotlinClassMetadataParserImpl();
        List<KmProperty> kotlinProperties = kotlinClassMetadataParser.getKotlinProperties(this);
        for (KmProperty kmProperty : kotlinProperties) {
            String propertyName = kmProperty.getName();

            MethodDescriptor getter = null;
            JvmMethodSignature getterSignature = JvmExtensionsKt.getGetterSignature(kmProperty);
            if (getterSignature != null) {
                getter = getMethodDescriptorWithSignature(getterSignature.getName(), getterSignature.getDesc());
            }

            MethodDescriptor setter = null;
            JvmMethodSignature setterSignature = JvmExtensionsKt.getSetterSignature(kmProperty);
            if (setterSignature != null) {
                setter = getMethodDescriptorWithSignature(setterSignature.getName(), setterSignature.getDesc());
            }

            propertyDescriptors.add(new PropertyInfo(propertyName, getter, setter));

        }

        return propertyDescriptors.toArray(new PropertyDescriptor[0]);

    }

    private MethodDescriptor getMethodDescriptorWithSignature(String name, String signature) {
        Optional<MethodDescriptor> method = Arrays
                .stream(getMethods())
                .filter(m -> m.getName().equals(name)
                        && m.getSignature().equals(signature))
                .findFirst();

        return method.orElse(null);
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
            for (int i = 0; i < fields.length; i++) {
                fields[i] = new FieldInfo(dexFile, allFields[i]);
            }
        } else {
            fields = new FieldDescriptor[0];
        }

        return fields;
    }

    @Override
    public MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation() {

        Optional<Annotation> metadataInfoAnnotation = getJavaAnnotationWithName("Lcom/telerik/metadata/MetadataInfo;");
        if (metadataInfoAnnotation.isPresent()) {
            return new MetadataInfoAnnotationInfo(dexFile, metadataInfoAnnotation.get());
        }
        return null;
    }

    private Optional<Annotation> getJavaAnnotationWithName(String name) {
        int annotationsOffset = classDef.getAnnotationsOffset();
        if (annotationsOffset == 0) {
            return Optional.empty();
        }

        Dex dex = dexFile.getDex();

        int classDefIndex = dex.findClassDefIndexFromTypeIndex(classDef.getTypeIndex());
        int dexAnnotationDirectoryOffset = dex.annotationDirectoryOffsetFromClassDefIndex(classDefIndex);
        if (dexAnnotationDirectoryOffset == 0) {
            return Optional.empty();
        }
        Dex.Section dirSection = dex.open(dexAnnotationDirectoryOffset);
        int classSetOffset = dirSection.readInt();

        int annotationSetOffset = classSetOffset;

        if (annotationSetOffset == 0) {
            return Optional.empty();
        }

        Dex.Section annSetSection = dex.open(annotationSetOffset);
        int size = annSetSection.readInt();
        for (int i = 0; i < size; i++) {
            int annotationOffset = annSetSection.readInt();
            Dex.Section annSection = dex.open(annotationOffset);
            Annotation annotation = annSection.readAnnotation();
            String annotationName = dex.strings().get(dex.typeIds().get(annotation.getTypeIndex()));
            if (annotationName.equals(name)) {
                return Optional.of(annotation);
            }
        }

        return Optional.empty();
    }

    @Override
    public Optional<KotlinClassMetadataAnnotation> getKotlinClassMetadataAnnotation() {
        Optional<Annotation> kotlinMetadataAnnotation = getJavaAnnotationWithName("Lkotlin/Metadata;");
        if (kotlinMetadataAnnotation.isPresent()) {
            KotlinClassMetadataAnnotation kotlinClassMetadataAnnotation = new KotlinDexClassMetadataAnnotation(kotlinMetadataAnnotation.get(), dexFile);
            return Optional.of(kotlinClassMetadataAnnotation);
        }
        return Optional.empty();
    }

    @Override
    public String[] getInterfaceNames() {
        ArrayList<String> names = new ArrayList<String>();
        short[] ifaceIndexes = classDef.getInterfaces();
        if ((ifaceIndexes != null) && (ifaceIndexes.length > 0)) {
            List<String> typeNames = dexFile.getDex().typeNames();
            for (short idx : ifaceIndexes) {
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
        String name = typeName.substring(1, typeName.length() - 1).replace('/', '.');
        return name;
    }

    @Override
    public String getSuperclassName() {
        List<String> typeNames = dexFile.getDex().typeNames();
        int idx = classDef.getSupertypeIndex();
        String superClassname = typeNames.get(idx);
        String name = superClassname.substring(1, superClassname.length() - 1).replace('/', '.');
        return name;
    }
}

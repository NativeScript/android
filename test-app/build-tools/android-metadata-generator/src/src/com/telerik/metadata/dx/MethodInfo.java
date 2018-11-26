package com.telerik.metadata.dx;

import com.android.dex.Annotation;
import com.android.dex.ClassData;
import com.android.dex.ClassDef;
import com.android.dex.Dex;
import com.android.dex.MethodId;
import com.android.dex.ProtoId;
import com.android.dx.rop.code.AccessFlags;
import com.telerik.metadata.desc.MetadataInfoAnnotationDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;

import java.util.List;

public class MethodInfo implements MethodDescriptor {
    private final DexFile dexFile;
    private final ClassData.Method method;

    public MethodInfo(DexFile dexFile, ClassData.Method method) {
        this.dexFile = dexFile;
        this.method = method;
    }

    @Override
    public boolean isPublic() {
        return AccessFlags.isPublic(method.getAccessFlags());
    }

    @Override
    public boolean isProtected() {
        return AccessFlags.isProtected(method.getAccessFlags());
    }

    @Override
    public boolean isSynthetic() {
        boolean isSynthetic = (method.getAccessFlags() & AccessFlags.ACC_SYNTHETIC) == AccessFlags.ACC_SYNTHETIC;
        return isSynthetic;
    }

    @Override
    public boolean isStatic() {
        return AccessFlags.isStatic(method.getAccessFlags());
    }

    @Override
    public boolean isAbstract() {
        return AccessFlags.isAbstract(method.getAccessFlags());
    }

    @Override
    public String getName() {
        Dex dex = dexFile.getDex();
        List<String> strings = dex.strings();
        int idx = dex.nameIndexFromMethodIndex(method.getMethodIndex());
        String methodName = strings.get(idx);
        return methodName;
    }

    @Override
    public String getSignature() {
        Dex dex = dexFile.getDex();
        List<String> typeNames = dex.typeNames();
        MethodId methodId = dex.methodIds().get(method.getMethodIndex());
        ProtoId methodProtoId = dex.protoIds().get(methodId.getProtoIndex());
        short[] parameterTypes = dex.readTypeList(methodProtoId.getParametersOffset()).getTypes();
        StringBuilder signature = new StringBuilder("(");
        for (short paramId: parameterTypes) {
            signature.append(typeNames.get(paramId));
        }
        signature.append(")").append(getReturnType().getSignature());
        return signature.toString();
    }

    @Override
    public TypeDescriptor[] getArgumentTypes() {
        Dex dex = dexFile.getDex();
        List<String> typeNames = dex.typeNames();
        MethodId methodId = dex.methodIds().get(method.getMethodIndex());
        ProtoId methodProtoId = dex.protoIds().get(methodId.getProtoIndex());
        short[] parameterTypes = dex.readTypeList(methodProtoId.getParametersOffset()).getTypes();
        TypeDescriptor[] argTypes = new TypeDescriptor[parameterTypes.length];

        for (int i=0; i<argTypes.length; i++) {
            int paramId = parameterTypes[i];
            String typeName = typeNames.get(paramId);
            argTypes[i] = new TypeInfo(typeName);
        }

        return argTypes;
    }

    @Override
    public TypeDescriptor getReturnType() {
        Dex dex = dexFile.getDex();
        List<String> typeNames = dex.typeNames();
        MethodId methodId = dex.methodIds().get(method.getMethodIndex());
        ProtoId methodProtoId = dex.protoIds().get(methodId.getProtoIndex());
        int retTypeIdx = methodProtoId.getReturnTypeIndex();
        String retTypeName = typeNames.get(retTypeIdx);
        return new TypeInfo(retTypeName);
    }

    @Override
    public MetadataInfoAnnotationDescriptor getMetadataInfoAnnotation() {
        Dex dex = dexFile.getDex();

        MethodId methodId = dex.methodIds().get(method.getMethodIndex());
        int cdIdx = methodId.getDeclaringClassIndex();

        ClassDef classDef = null;
        for (ClassDef currClassDef : dex.classDefs()) {
            if (cdIdx == currClassDef.getTypeIndex()) {
                classDef = currClassDef;
                break;
            }
        }

        int classDefIndex = dex.findClassDefIndexFromTypeIndex(classDef.getTypeIndex());
        int directoryOffset = dex.annotationDirectoryOffsetFromClassDefIndex(classDefIndex);
        if (directoryOffset == 0) {
            return null;
        }
        Dex.Section directoryIn = dex.open(directoryOffset);
        int classSetOffset = directoryIn.readInt();
        int fieldCount = directoryIn.readInt();
        int methodsCount = directoryIn.readInt();
        directoryIn.readInt();

        for (int i = 0; i < fieldCount; i++) {
            directoryIn.readInt();
            directoryIn.readInt();
        }

        int methodIdx = method.getMethodIndex();

        int annotationSetOffset = 0;
        for (int i = 0; i < methodsCount; i++) {
            int candidateMethodIndex = directoryIn.readInt();
            int currAnnotationSetOffset = directoryIn.readInt();
            if (candidateMethodIndex == methodIdx) {
                annotationSetOffset = currAnnotationSetOffset;
            }
        }

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
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        MethodInfo that = (MethodInfo) o;

        return getSignature().equals(that.getSignature());
    }

    @Override
    public int hashCode() {
        return getSignature().hashCode();
    }
}

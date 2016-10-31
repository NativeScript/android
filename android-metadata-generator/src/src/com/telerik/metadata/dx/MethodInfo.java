package com.telerik.metadata.dx;

import com.android.dex.ClassData;
import com.android.dex.Dex;
import com.android.dex.MethodId;
import com.android.dex.ProtoId;
import com.android.dx.rop.code.AccessFlags;
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
        String signature = "(";
        for (short paramId: parameterTypes) {
            signature += typeNames.get(paramId);
        }
        signature += ")" + getReturnType().getSignature();
        return signature;
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
}

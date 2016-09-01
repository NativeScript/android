package org.nativescript.staticbindinggenerator;

import com.tns.bindings.desc.ClassDescriptor;

import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;

class MethodInfo implements com.tns.bindings.desc.MethodDescriptor
{
    private final Method m;
    private final Generator generator;

    public MethodInfo(Method m, Generator generator)
    {
        this.m = m;
        this.generator = generator;
    }

    @Override
    public ClassDescriptor[] getParameterTypes() {
        Type[] types = m.getArgumentTypes();
        ClassDescriptor[] ret = new ClassDescriptor[types.length];
        for (int i=0; i<types.length; i++) {
            ret[i] = convertType(types[i]);
        }
        return ret;
    }

    @Override
    public String getName() {
        return m.getName();
    }

    @Override
    public ClassDescriptor getReturnType() {
        return convertType(m.getReturnType());
    }

    @Override
    public String toGenericString() {
        return m.getSignature();
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
    public boolean isPrivate() {
        return m.isPrivate();
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
    public boolean isFinal() {
        return m.isFinal();
    }

    private ClassDescriptor convertType(Type t) {
        ClassDescriptor desc;
        String sig = t.getSignature();
        String className;
        int rank = 0;
        while (sig.charAt(rank) == '[') ++rank;
        if (rank > 0) {
            sig = sig.substring(rank);
        }
        if (sig.startsWith("L")) {
            className = sig.substring(1, sig.length()-1).replace('/', '.').replace("$", ".");
        } else {
            className = sig;
        }
        switch (className) {
            case "V":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.VOID, rank);
                break;
            case "Z":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.BOOLEAN, rank);
                break;
            case "C":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.CHAR, rank);
                break;
            case "B":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.BYTE, rank);
                break;
            case "S":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.SHORT, rank);
                break;
            case "I":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.INT, rank);
                break;
            case "J":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.LONG, rank);
                break;
            case "F":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.FLOAT, rank);
                break;
            case "D":
                desc = ClassInfo.fromPrimitive(ClassDescriptor.DOUBLE, rank);
                break;
            default:
                desc = new ClassInfo(generator.getClasses().get(className), rank, generator);
                break;
        }
        return desc;
    }
}

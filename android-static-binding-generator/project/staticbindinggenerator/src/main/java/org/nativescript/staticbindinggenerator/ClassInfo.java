package org.nativescript.staticbindinggenerator;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.MethodDescriptor;

import org.apache.bcel.classfile.Attribute;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.classfile.Signature;

import java.util.ArrayList;

class ClassInfo implements com.tns.bindings.desc.ClassDescriptor {
    private final ClassDescriptor primitiveType;
    private final JavaClass clazz;
    private final Generator generator;
    private final int rank;

    public ClassInfo(JavaClass clazz, Generator generator) {
        this(clazz, 0, generator);
    }

    public ClassInfo(JavaClass clazz, int rank, Generator generator) {
        if (clazz == null) {
            throw new IllegalArgumentException("clazz is null");
        }
        this.primitiveType = null;
        this.clazz = clazz;
        this.rank = rank;
        this.generator = generator;
    }

    private ClassInfo(ClassDescriptor primitiveType, int rank) {
        this.primitiveType = primitiveType;
        this.clazz = null;
        this.rank = rank;
        this.generator = null;
    }

    @Override
    public boolean isPrimitive() {
        return isArray()
                ? false
                : ((primitiveType != null)
                    ? true
                    : (!(clazz.isClass() || clazz.isEnum() || clazz.isInterface())));
    }

    @Override
    public boolean isArray() {
        return rank > 0;
    }

    @Override
    public boolean isInterface() {
        return isArray()
                ? false
                : (isPrimitive() ? false : clazz.isInterface());
    }

    @Override
    public String getSignature() {
        if (primitiveType == null) {
            Attribute[] attrs = clazz.getAttributes();
            for (Attribute a : attrs) {
                if (a instanceof Signature) {
                    return ((Signature) a).getSignature();
                }
            }
        }
        return null;
    }

    @Override
    public String getName() {
        String name = isArray() ? new String(new char[rank]).replace("\0", "[") : "";
        return  name + ((primitiveType != null) ? primitiveType.getName() : clazz.getClassName());
    }

    @Override
    public MethodDescriptor[] getConstructors() {
        if (isArray() || isPrimitive()) {
            return new MethodDescriptor[0];
        }
        ArrayList<MethodDescriptor> ctors = new ArrayList<MethodDescriptor>();
        for (Method m : clazz.getMethods()) {
            if (m.getName().equals("<init>")) {
                ctors.add(new MethodInfo(m, generator));
            }
        }
        return ctors.toArray(new MethodDescriptor[ctors.size()]);
    }

    @Override
    public ClassDescriptor getComponentType() {
        return isArray()
                ? ((primitiveType != null)
                    ? ((rank > 1) ? new ClassInfo(primitiveType, rank-1) : primitiveType)
                    : new ClassInfo(clazz, rank-1, generator))
                : null;
    }

    @Override
    public MethodDescriptor[] getDeclaredMethods() {
        if (isArray() || isPrimitive()) {
            return new MethodDescriptor[0];
        }
        ArrayList<MethodDescriptor> methods = new ArrayList<MethodDescriptor>();
        for (Method m : clazz.getMethods()) {
            methods.add(new MethodInfo(m, generator));
        }
        return methods.toArray(new MethodDescriptor[methods.size()]);
    }

    @Override
    public ClassDescriptor[] getInterfaces() {
        if (isArray() || isPrimitive()) {
            return new ClassDescriptor[0];
        }
        ArrayList<ClassDescriptor> ifaces = new ArrayList<ClassDescriptor>();
        for (String i : clazz.getInterfaceNames()) {
            String name = i.replace('$', '.');
            ifaces.add(new ClassInfo(generator.getClasses().get(name), generator));
        }
        return ifaces.toArray(new ClassDescriptor[ifaces.size()]);
    }

    @Override
    public ClassDescriptor getSuperclass() {
        ClassDescriptor superClass = null;
        if (!isArray() && !isPrimitive() && !getName().equals("java.lang.Object")) {
            superClass = new ClassInfo(generator.getClasses().get(clazz.getSuperclassName()), generator);
        }
        return superClass;
    }

    @Override
    public boolean isSynthetic() {
        return (isArray() || isPrimitive()) ? false : clazz.isSynthetic();
    }

    @Override
    public boolean isStatic() {
        return (isArray() || isPrimitive()) ? false : clazz.isStatic();
    }

    @Override
    public boolean isPrivate() {
        return (isArray() || isPrimitive()) ? false : clazz.isPrivate();
    }

    @Override
    public boolean isPublic() {
        return (isArray() || isPrimitive()) ? true : clazz.isPublic();
    }

    @Override
    public boolean isProtected() {
        return (isArray() || isPrimitive()) ? false : clazz.isProtected();
    }

    @Override
    public boolean isFinal() {
        return (isArray() || isPrimitive()) ? false : clazz.isFinal();
    }

    @Override
    public boolean isAbstract() {
        return (isArray() || isPrimitive()) ? false : clazz.isAbstract();
    }

    static ClassDescriptor fromPrimitive(ClassDescriptor primitiveType, int rank) {
        return (rank == 0) ? primitiveType : new ClassInfo(primitiveType, rank);
    }
}

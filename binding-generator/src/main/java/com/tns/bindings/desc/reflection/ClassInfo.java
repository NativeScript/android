package com.tns.bindings.desc.reflection;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.MethodDescriptor;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

public class ClassInfo implements ClassDescriptor {
    private final Class<?> clazz;

    public ClassInfo(Class<?> clazz) {
        this.clazz = clazz;
    }

    @Override
    public boolean equals(Object o) {
        if ((o == null) || !(o instanceof ClassDescriptor)) {
            return false;
        }
        ClassDescriptor other = (ClassDescriptor)o;
        return getSignature().equals(other.getSignature());
    }

    @Override
    public boolean isPrimitive() {
        return clazz.isPrimitive();
    }

    @Override
    public boolean isArray() {
        return clazz.isArray();
    }

    @Override
    public boolean isInterface() {
        return clazz.isInterface();
    }

    @Override
    public String getSignature() {
        String sig;
        if (clazz == void.class) {
            sig = "V";
        } else if (clazz == boolean.class) {
            sig = "Z";
        } else if (clazz == char.class) {
            sig = "C";
        } else if (clazz == byte.class) {
            sig = "B";
        } else if (clazz == short.class) {
            sig = "S";
        } else if (clazz == int.class) {
            sig = "I";
        } else if (clazz == long.class) {
            sig = "J";
        } else if (clazz == float.class) {
            sig = "F";
        } else if (clazz == double.class) {
            sig = "D";
        } else {
            sig = clazz.getName();
        }
        return sig;
    }

    @Override
    public String getName() {
        return clazz.getName();
    }

    @Override
    public MethodDescriptor[] getConstructors() {
        Constructor<?>[] ctors = clazz.getConstructors();
        MethodDescriptor[] ret = new MethodDescriptor[ctors.length];
        for (int i=0; i<ctors.length; i++) {
            ret[i] = new MethodInfo(ctors[i]);
        }
        return ret;
    }

    @Override
    public ClassDescriptor getComponentType() {
        return new ClassInfo(clazz.getComponentType());
    }

    @Override
    public MethodDescriptor[] getDeclaredMethods() {
        Method[] methods = clazz.getDeclaredMethods();
        MethodDescriptor[] ret = new MethodDescriptor[methods.length];
        for (int i=0; i<methods.length; i++) {
            ret[i] = new MethodInfo(methods[i]);
        }
        return ret;
    }

    @Override
    public ClassDescriptor[] getInterfaces() {
        Class<?>[] ifaces = clazz.getInterfaces();
        ClassDescriptor[] ret = new ClassDescriptor[ifaces.length];
        for (int i=0; i<ifaces.length; i++) {
            ret[i] = new ClassInfo(ifaces[i]);
        }
        return ret;
    }

    @Override
    public ClassDescriptor getSuperclass() {
        Class<?> superClass = clazz.getSuperclass();
        return (superClass != null) ? new ClassInfo(superClass) : null;
    }

    @Override
    public boolean isSynthetic() {
        return clazz.isSynthetic();
    }

    @Override
    public boolean isStatic() {
        return Modifier.isStatic(clazz.getModifiers());
    }

    @Override
    public boolean isPrivate() {
        return Modifier.isPrivate(clazz.getModifiers());
    }

    @Override
    public boolean isPublic() {
        return Modifier.isPublic(clazz.getModifiers());
    }

    @Override
    public boolean isProtected() {
        return Modifier.isProtected(clazz.getModifiers());
    }

    @Override
    public boolean isFinal() {
        return Modifier.isFinal(clazz.getModifiers());
    }

    @Override
    public boolean isAbstract() {
        return Modifier.isAbstract(clazz.getModifiers());
    }
}

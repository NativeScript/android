package com.tns.bindings.desc.reflection;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.MethodDescriptor;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

public class MethodInfo implements MethodDescriptor {
    private boolean m_isSynthetic;
    private final boolean m_isStatic;
    private final boolean m_isPrivate;
    private final boolean m_isPublic;
    private final boolean m_isProtected;
    private final boolean m_isFinal;
    private final boolean m_isAbstract;
    private Class<?>[] m_parameterTypes;
    private Class<?> m_retType;
    private String m_name;
    private String m_genericSig;
    private boolean m_isInterface;

    public MethodInfo(Method method) {
        this(method.getModifiers());
        m_isSynthetic = method.isSynthetic();
        m_parameterTypes = method.getParameterTypes();
        m_retType = method.getReturnType();
        m_name = method.getName();
        m_genericSig = method.toGenericString();
        m_isInterface = false;
    }

    public MethodInfo(Constructor<?> ctor) {
        this(ctor.getModifiers());
        m_isSynthetic = ctor.isSynthetic();
        m_parameterTypes = ctor.getParameterTypes();
        m_retType = void.class;
        m_name = ctor.getName();
        m_genericSig = ctor.toGenericString();
        m_isInterface = false;
    }

    private MethodInfo(int modifiers) {
        m_isStatic = Modifier.isStatic(modifiers);
        m_isPrivate = Modifier.isPrivate(modifiers);
        m_isPublic = Modifier.isPublic(modifiers);
        m_isProtected = Modifier.isProtected(modifiers);
        m_isFinal = Modifier.isFinal(modifiers);
        m_isAbstract = Modifier.isAbstract(modifiers);
    }

    @Override
    public ClassDescriptor[] getParameterTypes() {
        ClassDescriptor[] ret = new ClassDescriptor[m_parameterTypes.length];
        for (int i=0; i<m_parameterTypes.length; i++) {
            ret[i] = new ClassInfo(m_parameterTypes[i]);
        }
        return ret;
    }

    @Override
    public String getName() {
        return m_name;
    }

    @Override
    public ClassDescriptor getReturnType() {
        return new ClassInfo(m_retType);
    }

    @Override
    public String toGenericString() {
        return m_genericSig;
    }

    @Override
    public boolean isInterfaceMethod() {
        return m_isInterface;
    }

    @Override
    public void setAsInterfaceMethod() {
        this.m_isInterface = true;
    }

    @Override
    public boolean isSynthetic() {
        return m_isSynthetic;
    }

    @Override
    public boolean isStatic() {
        return m_isStatic;
    }

    @Override
    public boolean isPrivate() {
        return m_isPrivate;
    }

    @Override
    public boolean isPublic() {
        return m_isPublic;
    }

    @Override
    public boolean isProtected() {
        return m_isProtected;
    }

    @Override
    public boolean isFinal() {
        return m_isFinal;
    }

    @Override
    public boolean isAbstract() {
        return m_isAbstract;
    }
}

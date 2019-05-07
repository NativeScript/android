package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.apache.bcel.classfile.ExceptionTable;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;

import java.lang.reflect.Modifier;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class JavaMethodImpl implements JavaMethod {

    private final Method method;
    private final JavaClass declaringClass;

    public JavaMethodImpl(Method method, JavaClass declaringClass) {
        this.method = method;
        this.declaringClass = declaringClass;
    }

    @Override
    public Method getMethod() {
        return method;
    }

    @Override
    public String getName() {
        return method.getName();
    }

    @Override
    public boolean isFromInterface() {
        return declaringClass.isInterface();
    }

    @Override
    public boolean isConstructor() {
        return "<init>".equals(method.getName());
    }

    @Override
    public boolean isPublic() {
        return method.isPublic();
    }

    @Override
    public boolean isPrivate() {
        return method.isPrivate();
    }

    @Override
    public boolean isProtected() {
        return method.isProtected();
    }

    @Override
    public boolean isAbstract() {
        return method.isAbstract();
    }

    @Override
    public boolean isStatic() {
        return method.isStatic();
    }

    @Override
    public boolean isFinal() {
        return Modifier.isFinal(method.getModifiers());
    }

    @Override
    public boolean isSynthetic() {
        return method.isSynthetic();
    }

    @Override
    public String getSignature() {
        return method.getSignature();
    }

    @Override
    public String getGenericSignature() {
        return method.getGenericSignature();
    }

    @Override
    public boolean hasGenericSignature() {
        String signature = method.getGenericSignature();
        return signature != null && !signature.equals("");
    }

    @Override
    public Type[] getArgumentTypes() {
        return method.getArgumentTypes();
    }

    @Override
    public Type getReturnType() {
        return method.getReturnType();
    }

    @Override
    public JavaClass getDeclaringClass() {
        return declaringClass;
    }

    @Override
    public String getDeclaringClassName() {
        return declaringClass.getClassName();
    }

    @Override
    public String getDeclaringClassPackageName() {
        return declaringClass.getPackageName();
    }

    @Override
    public List<String> getDeclaredThrownExceptions() {
        ExceptionTable exceptionTable = method.getExceptionTable();
        if (exceptionTable != null) {
            String[] exceptionNames = exceptionTable.getExceptionNames();
            return Arrays.asList(exceptionNames);
        }
        return Collections.emptyList();
    }

    @Override
    public String toString() {
        return "JavaMethodImpl{" +
                "method=" + method +
                ", declaringClass=" + declaringClass.getClassName() +
                '}';
    }
}

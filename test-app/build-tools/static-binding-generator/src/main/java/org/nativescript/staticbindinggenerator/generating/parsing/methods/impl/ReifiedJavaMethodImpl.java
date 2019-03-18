package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;

import java.util.List;

public class ReifiedJavaMethodImpl implements ReifiedJavaMethod {

    private final JavaMethod javaMethod;
    private final List<String> reifiedArguments;
    private final String reifiedReturnType;

    public ReifiedJavaMethodImpl(JavaMethod javaMethod, List<String> reifiedArguments, String reifiedReturnType){
        this.javaMethod = javaMethod;
        this.reifiedArguments = reifiedArguments;
        this.reifiedReturnType = reifiedReturnType;
    }

    @Override
    public List<String> getReifiedArguments() {
        return reifiedArguments;
    }

    @Override
    public String getReifiedReturnType(){
        return reifiedReturnType;
    }

    @Override
    public Method getMethod() {
        return javaMethod.getMethod();
    }

    @Override
    public String getName() {
        return javaMethod.getName();
    }

    @Override
    public boolean isFromInterface() {
        return javaMethod.isFromInterface();
    }

    @Override
    public boolean isConstructor() {
        return javaMethod.isConstructor();
    }

    @Override
    public boolean isPublic() {
        return javaMethod.isPublic();
    }

    @Override
    public boolean isPrivate() {
        return javaMethod.isPrivate();
    }

    @Override
    public boolean isProtected() {
        return javaMethod.isProtected();
    }

    @Override
    public boolean isAbstract() {
        return javaMethod.isAbstract();
    }

    @Override
    public boolean isStatic() {
        return javaMethod.isStatic();
    }

    @Override
    public boolean isFinal() {
        return javaMethod.isFinal();
    }

    @Override
    public boolean isSynthetic() {
        return javaMethod.isSynthetic();
    }

    @Override
    public String getSignature() {
        return javaMethod.getSignature();
    }

    @Override
    public String getGenericSignature() {
        return javaMethod.getGenericSignature();
    }

    @Override
    public boolean hasGenericSignature() {
        return javaMethod.hasGenericSignature();
    }

    @Override
    public Type[] getArgumentTypes() {
        return javaMethod.getArgumentTypes();
    }

    @Override
    public Type getReturnType() {
        return javaMethod.getReturnType();
    }

    @Override
    public String getDeclaringClassName() {
        return javaMethod.getDeclaringClassName();
    }

    @Override
    public JavaClass getDeclaringClass() {
        return javaMethod.getDeclaringClass();
    }

    @Override
    public String getDeclaringClassPackageName() {
        return javaMethod.getDeclaringClassPackageName();
    }
}

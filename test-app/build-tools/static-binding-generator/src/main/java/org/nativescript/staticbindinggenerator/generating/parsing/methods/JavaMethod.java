package org.nativescript.staticbindinggenerator.generating.parsing.methods;

import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;

public interface JavaMethod {
    Method getMethod();
    String getName();
    boolean isFromInterface();
    boolean isConstructor();
    boolean isPublic();
    boolean isPrivate();
    boolean isProtected();
    boolean isAbstract();
    boolean isStatic();
    boolean isFinal();
    boolean isSynthetic();
    String getSignature();
    String getGenericSignature();
    boolean hasGenericSignature();
    Type[] getArgumentTypes();
    Type getReturnType();
    String getDeclaringClassName();
    JavaClass getDeclaringClass();
    String getDeclaringClassPackageName();
}

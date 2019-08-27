package org.nativescript.staticbindinggenerator.generating.writing;

import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;

public interface MethodsWriter extends JavaCodeWriter {
    void writeMethod(ReifiedJavaMethod method, boolean isUserImplemented);
    void writeConstructor(String className, ReifiedJavaMethod method, boolean hasUserImplementedInitMethod);
    void writeGetInstanceMethod(String className);
    void writeInternalRuntimeEqualsMethod();
    void writeInternalRuntimeHashCodeMethod();
    void writeInternalServiceOnCreateMethod();
}

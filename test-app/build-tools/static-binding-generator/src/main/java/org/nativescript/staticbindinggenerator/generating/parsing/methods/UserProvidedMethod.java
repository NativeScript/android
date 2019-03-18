package org.nativescript.staticbindinggenerator.generating.parsing.methods;

public class UserProvidedMethod {
    private final String methodName;

    public UserProvidedMethod(String methodName) {
        this.methodName = methodName;
    }

    public String getMethodName() {
        return methodName;
    }
}

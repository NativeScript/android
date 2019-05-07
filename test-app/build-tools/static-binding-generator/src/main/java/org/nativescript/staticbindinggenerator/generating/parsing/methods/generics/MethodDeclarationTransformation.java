package org.nativescript.staticbindinggenerator.generating.parsing.methods.generics;

public interface MethodDeclarationTransformation<T> {
    T transform(T input);
}

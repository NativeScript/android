package org.nativescript.staticbindinggenerator.generating.parsing.methods.generics;

import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;

public interface MethodReificationPipeline {
    MethodReificationPipeline withArgumentsDeclarationTransformation(MethodDeclarationTransformation<String> transformation);
    MethodReificationPipeline withOwnTypeParametersDeclarationTransformation(MethodDeclarationTransformation<String> transformation);
    MethodReificationPipeline withReturnTypeDeclarationTransformation(MethodDeclarationTransformation<String> transformation);
    ReifiedJavaMethod reify();
}

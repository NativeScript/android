package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.MethodReificationPipeline;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl.ArgumentsDeclarationReificationTransformation;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl.MethodReificationPipelineImpl;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl.OwnTypeParametersDeclarationTransformation;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl.ReturnTypeDeclarationReificationTransformation;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl.WildcardBoundsTransformation;

public class MethodSignatureReifier {

    private final GenericHierarchyView initialJavaClassGenericHierarchyView;


    public MethodSignatureReifier(GenericHierarchyView initialJavaClassGenericHierarchyView) {
        this.initialJavaClassGenericHierarchyView = initialJavaClassGenericHierarchyView;
    }

    public ReifiedJavaMethod transformJavaMethod(JavaMethod javaMethod) {
        if (javaMethod.hasGenericSignature()) {
            GenericParameters genericParameters = getResolvedFormalTypeParametersFromDeclaringClass(javaMethod);
            if (genericParameters != null) {
                return transformGenericJavaMethod(javaMethod, genericParameters);
            } else {
                return transformGenericJavaMethodWithoutReification(javaMethod);
            }
        } else {
            return transformRegularJavaMethod(javaMethod);
        }
    }

    private ReifiedJavaMethod transformGenericJavaMethodWithoutReification(JavaMethod javaMethod) {
        MethodReificationPipeline methodReificationPipeline = new MethodReificationPipelineImpl(javaMethod)
                .withArgumentsDeclarationTransformation(new WildcardBoundsTransformation())
                .withOwnTypeParametersDeclarationTransformation(new OwnTypeParametersDeclarationTransformation())
                .withReturnTypeDeclarationTransformation(new WildcardBoundsTransformation());

        return methodReificationPipeline.reify();
    }

    private ReifiedJavaMethod transformRegularJavaMethod(JavaMethod javaMethod) {
        MethodReificationPipeline methodReificationPipeline = new MethodReificationPipelineImpl(javaMethod);
        return methodReificationPipeline.reify();
    }

    private ReifiedJavaMethod transformGenericJavaMethod(JavaMethod javaMethod, GenericParameters classGenericParameters) {
        MethodReificationPipeline methodReificationPipeline = new MethodReificationPipelineImpl(javaMethod)
                .withArgumentsDeclarationTransformation(new ArgumentsDeclarationReificationTransformation(classGenericParameters))
                .withArgumentsDeclarationTransformation(new WildcardBoundsTransformation())
                .withOwnTypeParametersDeclarationTransformation(new OwnTypeParametersDeclarationTransformation())
                .withReturnTypeDeclarationTransformation(new ReturnTypeDeclarationReificationTransformation(classGenericParameters))
                .withReturnTypeDeclarationTransformation(new WildcardBoundsTransformation());

        return methodReificationPipeline.reify();
    }


    private GenericParameters getResolvedFormalTypeParametersFromDeclaringClass(JavaMethod javaMethod) {
        String javaMethodDeclaringClassName = javaMethod.getDeclaringClassName();

        if (initialJavaClassGenericHierarchyView.getInitialClassName().equals(javaMethodDeclaringClassName)) {
            return initialJavaClassGenericHierarchyView.getInitialClassGenericParameters();
        }

        if (javaMethod.isFromInterface()) {
            return initialJavaClassGenericHierarchyView.getAllImplementedInterfaces().get(javaMethodDeclaringClassName);
        } else {
            return initialJavaClassGenericHierarchyView.getAllParentClasses().get(javaMethodDeclaringClassName);
        }
    }


}



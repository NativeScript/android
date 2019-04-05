package org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.MethodDeclarationTransformation;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.MethodReificationPipeline;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.impl.ReifiedJavaMethodImpl;
import org.nativescript.staticbindinggenerator.naming.JavaClassNames;
import org.objectweb.asm.signature.SignatureReader;
import org.objectweb.asm.util.TraceSignatureVisitor;

import java.util.ArrayList;
import java.util.List;

public class MethodReificationPipelineImpl implements MethodReificationPipeline {

    private final JavaMethod methodToTransform;
    private final List<MethodDeclarationTransformation<String>> argumentsDeclarationTransformations;
    private final List<MethodDeclarationTransformation<String>> ownTypeParametersDeclarationTransformations;
    private final List<MethodDeclarationTransformation<String>> returnTypeDeclarationTransformations;

    public MethodReificationPipelineImpl(JavaMethod methodToTransform) {
        this.methodToTransform = methodToTransform;
        this.ownTypeParametersDeclarationTransformations = new ArrayList<>();
        this.returnTypeDeclarationTransformations = new ArrayList<>();
        this.argumentsDeclarationTransformations = new ArrayList<>();
    }

    @Override
    public MethodReificationPipeline withArgumentsDeclarationTransformation(MethodDeclarationTransformation<String> transformation) {
        argumentsDeclarationTransformations.add(transformation);
        return this;
    }

    @Override
    public MethodReificationPipeline withOwnTypeParametersDeclarationTransformation(MethodDeclarationTransformation<String> transformation) {
        ownTypeParametersDeclarationTransformations.add(transformation);
        return this;
    }

    @Override
    public MethodReificationPipeline withReturnTypeDeclarationTransformation(MethodDeclarationTransformation<String> transformation) {
        returnTypeDeclarationTransformations.add(transformation);
        return this;
    }

    @Override
    public ReifiedJavaMethod reify() {
        int declaringClassAccessFlags = methodToTransform.getDeclaringClass().getAccessFlags();
        DeclarationAndReturnType declarationAndReturnType;

        if (methodToTransform.hasGenericSignature()) {
            declarationAndReturnType = getSignatureDeclarationAndReturnType(methodToTransform.getGenericSignature(), declaringClassAccessFlags);
        } else {
            declarationAndReturnType = getSignatureDeclarationAndReturnType(methodToTransform.getSignature(), declaringClassAccessFlags);
        }

        String argumentsDeclaration = declarationAndReturnType.getDeclaration();

        for (MethodDeclarationTransformation<String> argumentsDeclarationTransformation : argumentsDeclarationTransformations) {
            argumentsDeclaration = argumentsDeclarationTransformation.transform(argumentsDeclaration);
        }

        String ownTypeParametersDeclaration = getMethodTypeParametersDeclaration(argumentsDeclaration); // should be extracted from the arguments declaration after all its transformations are applied

        for (MethodDeclarationTransformation<String> ownTypeParametersDeclarationTransformation : ownTypeParametersDeclarationTransformations) {
            ownTypeParametersDeclaration = ownTypeParametersDeclarationTransformation.transform(ownTypeParametersDeclaration);
        }

        String returnType = declarationAndReturnType.getReturnType();

        for (MethodDeclarationTransformation<String> returnTypeDeclarationTransformation : returnTypeDeclarationTransformations) {
            returnType = returnTypeDeclarationTransformation.transform(returnType);
        }

        List<String> argumentsList = getReifiedArgumentsFromReifiedDeclaration(argumentsDeclaration);
        return new ReifiedJavaMethodImpl(methodToTransform, argumentsList, ownTypeParametersDeclaration, returnType);
    }

    private String getMethodTypeParametersDeclaration(String reifiedDeclaration) {
        StringBuilder methodTypeParametersDeclarationBuilder = new StringBuilder();

        for (int i = 0; i < reifiedDeclaration.length(); i += 1) {
            char c = reifiedDeclaration.charAt(i);
            if (c == '(') {
                break;
            } else {
                methodTypeParametersDeclarationBuilder.append(c);
            }
        }

        return methodTypeParametersDeclarationBuilder.toString();
    }

    private List<String> getReifiedArgumentsFromReifiedDeclaration(String reifiedDeclaration) {
        List<String> res = new ArrayList<>();
        StringBuilder reifiedArgumentBuffer = new StringBuilder();
        int parameterCounter = 0;
        int openingAngleBracketsCounter = 0;
        int closingAngleBracketsCounter = 0;
        boolean hasMethodArgumentsParsingStarted = false;

        for (int i = 0; i < reifiedDeclaration.length(); i += 1) {
            char c = reifiedDeclaration.charAt(i);
            if (c == '(') {
                hasMethodArgumentsParsingStarted = true;
            } else if (hasMethodArgumentsParsingStarted) {
                if (c == '<') {
                    openingAngleBracketsCounter += 1;
                    reifiedArgumentBuffer.append(c);
                } else if (c == '>') {
                    closingAngleBracketsCounter += 1;
                    reifiedArgumentBuffer.append(c);
                } else if (c == ')') {
                    if (reifiedArgumentBuffer.length() > 0) {
                        reifiedArgumentBuffer.append(" param_");
                        reifiedArgumentBuffer.append(parameterCounter);
                        res.add(reifiedArgumentBuffer.toString());
                    }
                } else if (c == ',') {
                    if (openingAngleBracketsCounter == closingAngleBracketsCounter) {
                        reifiedArgumentBuffer.append(" param_");
                        reifiedArgumentBuffer.append(parameterCounter);
                        parameterCounter += 1;
                        res.add(reifiedArgumentBuffer.toString());
                        reifiedArgumentBuffer = new StringBuilder();
                        openingAngleBracketsCounter = 0;
                        closingAngleBracketsCounter = 0;
                    } else {
                        reifiedArgumentBuffer.append(c);
                    }
                } else if (c == '$') {
                    reifiedArgumentBuffer.append('.');
                } else {
                    reifiedArgumentBuffer.append(c);
                }
            }

        }

        return res;
    }

    private DeclarationAndReturnType getSignatureDeclarationAndReturnType(String signature, int declaringClassAccessFlags) {
        SignatureReader sigReader = new SignatureReader(signature);
        TraceSignatureVisitor visitor = new TraceSignatureVisitor(declaringClassAccessFlags);
        sigReader.accept(visitor);
        String declaration = visitor.getDeclaration();
        String returnType = visitor.getReturnType();

        if (signature.endsWith("Ljava/lang/Object;")) { // check if method returns java.lang.Object because ASM doesn't output the declaration for the base Java class
            return new DeclarationAndReturnType(declaration, JavaClassNames.BASE_JAVA_CLASS_NAME + returnType); // concatenate what is in 'returnType' as ASM successfully returns '[]' if the return type was 'Object[]'
        } else {
            return new DeclarationAndReturnType(declaration, returnType);
        }
    }

    class DeclarationAndReturnType {
        private final String declaration;
        private final String returnType;

        DeclarationAndReturnType(String declaration, String returnType) {
            this.declaration = declaration;
            this.returnType = returnType;
        }

        String getDeclaration() {
            return declaration;
        }

        String getReturnType() {
            return returnType;
        }
    }
}

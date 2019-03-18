package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.objectweb.asm.signature.SignatureReader;
import org.objectweb.asm.util.TraceSignatureVisitor;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MethodSignatureReifier {

    private final GenericHierarchyView initialJavaClassGenericHierarchyView;

    public MethodSignatureReifier(GenericHierarchyView initialJavaClassGenericHierarchyView) {
        this.initialJavaClassGenericHierarchyView = initialJavaClassGenericHierarchyView;
    }

    public ReifiedJavaMethod transformJavaMethod(JavaMethod javaMethod) {
        int declaringClassAccessFlags = javaMethod.getDeclaringClass().getAccessFlags();
        if(javaMethod.getName().equals("add")){
            String name = javaMethod.getName();
            String originClass = javaMethod.getDeclaringClassName();
        }

        if (javaMethod.hasGenericSignature()) {
            GenericParameters genericParameters = getResolvedFormalTypeParametersFromDeclaringClass(javaMethod);
            if (genericParameters != null) {
                return transformGenericJavaMethod(javaMethod, genericParameters, declaringClassAccessFlags);
            } else {
                return transformRegularJavaMethod(javaMethod, declaringClassAccessFlags);
            }
        } else {
            return transformRegularJavaMethod(javaMethod, declaringClassAccessFlags);
        }
    }

    private ReifiedJavaMethod transformRegularJavaMethod(JavaMethod javaMethod, int declaringClassAccessFlags) {
        DeclarationAndReturnType declarationAndReturnType = getSignatureDeclarationAndReturnType(javaMethod.getSignature(), declaringClassAccessFlags);
        String argumentsDeclaration = declarationAndReturnType.getDeclaration();
        List<String> argumentsList = getReifiedArgumentsFromReifiedDeclaration(argumentsDeclaration);
        return new ReifiedJavaMethodImpl(javaMethod, argumentsList, declarationAndReturnType.getReturnType());
    }

    private ReifiedJavaMethod transformGenericJavaMethod(JavaMethod javaMethod, GenericParameters classGenericParameters, int declaringClassAccessFlags) {
        DeclarationAndReturnType declarationAndReturnType = getSignatureDeclarationAndReturnType(javaMethod.getGenericSignature(), declaringClassAccessFlags);
        String reifiedGenericArgumentsDeclaration = reifyArgumentsDeclarationIfNecessary(declarationAndReturnType.getDeclaration(), classGenericParameters);
        List<String> reifiedGenericArguments = getReifiedArgumentsFromReifiedDeclaration(reifiedGenericArgumentsDeclaration);
        String reifiedGenericReturnType = reifyReturnTypeIfNecessary(declarationAndReturnType.getReturnType(), classGenericParameters);
        return new ReifiedJavaMethodImpl(javaMethod, reifiedGenericArguments, reifiedGenericReturnType);
    }

    private DeclarationAndReturnType getSignatureDeclarationAndReturnType(String signature, int declaringClassAccessFlags) {
        SignatureReader sigReader = new SignatureReader(signature);
        TraceSignatureVisitor visitor = new TraceSignatureVisitor(declaringClassAccessFlags);
        sigReader.accept(visitor);
        String declaration = visitor.getDeclaration();
        String returnType = visitor.getReturnType();

        if (signature.endsWith("Ljava/lang/Object;")) { // check if method returns java.lang.Object because ASM doesn't output the declaration for the base Java class
            return new DeclarationAndReturnType(declaration, "java.lang.Object" + returnType); // concatenate what is in 'returnType' as ASM successfully returns '[]' if the return type was 'Object[]'
        } else {
            return new DeclarationAndReturnType(declaration, returnType);
        }
    }

    private GenericParameters getResolvedFormalTypeParametersFromDeclaringClass(JavaMethod javaMethod) {
        if (javaMethod.isFromInterface()) {
            String interfaceName = javaMethod.getDeclaringClassName();
            return initialJavaClassGenericHierarchyView.getAllImplementedInterfaces().get(interfaceName);
        } else {
            String className = javaMethod.getDeclaringClassName();
            if(initialJavaClassGenericHierarchyView.getInitialClassName().equals(className)){
                return initialJavaClassGenericHierarchyView.getInitialClassGenericParameters();
            }
            return initialJavaClassGenericHierarchyView.getAllParentClasses().get(className);
        }
    }

    private String reifyArgumentsDeclarationIfNecessary(String declaration, GenericParameters resolvedFormalTypeParameters) {

        String reifiedDeclaration = declaration;

        for (Map.Entry<String, String> resolvedFormalTypeParameter : resolvedFormalTypeParameters.getGenericParameters().entrySet()) {
            Pattern pattern = Pattern.compile("(\\W)" + resolvedFormalTypeParameter.getKey() + "(\\W)");

            Matcher m = pattern.matcher(declaration);
            if (m.find()) {
                reifiedDeclaration = m.replaceAll("$1" + resolvedFormalTypeParameter.getValue() + "$2");
            }
        }

        return reifiedDeclaration;
    }

    private String reifyReturnTypeIfNecessary(String returnType, GenericParameters resolvedFormalTypeParameters) {
        String reifiedReturnType = returnType;

        for (Map.Entry<String, String> resolvedFormalTypeParameter : resolvedFormalTypeParameters.getGenericParameters().entrySet()) {
            if (returnType.equals(resolvedFormalTypeParameter.getKey())) {
                return resolvedFormalTypeParameter.getValue();
            } else {
                Pattern pattern = Pattern.compile("(\\W)" + resolvedFormalTypeParameter.getKey() + "(\\W)");
                Matcher m = pattern.matcher(returnType);
                if (m.find()) {
                    reifiedReturnType = m.replaceAll("$1" + resolvedFormalTypeParameter.getValue() + "$2");
                }
            }
        }

        return reifiedReturnType;
    }

    private List<String> getReifiedArgumentsFromReifiedDeclaration(String reifiedDeclaration) {
        List<String> res = new ArrayList<>();
        StringBuilder reifiedArgumentBuffer = new StringBuilder();
        int parameterCounter = 0;
        int openingAngleBracketsCounter = 0;
        int closingAngleBracketsCounter = 0;

        for (int i = 0; i < reifiedDeclaration.length(); i += 1) {
            char c = reifiedDeclaration.charAt(i);
            if (c == '<') {
                openingAngleBracketsCounter += 1;
                reifiedArgumentBuffer.append(c);
            } else if (c == '>') {
                closingAngleBracketsCounter += 1;
                reifiedArgumentBuffer.append(c);
            } else if (c == '(') {
                // do nothing
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
            } else {
                reifiedArgumentBuffer.append(c);
            }
        }

        return res;
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
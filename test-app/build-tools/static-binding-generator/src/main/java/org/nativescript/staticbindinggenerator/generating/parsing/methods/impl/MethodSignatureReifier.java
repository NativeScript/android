package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;
import org.nativescript.staticbindinggenerator.naming.JavaClassNames;
import org.objectweb.asm.signature.SignatureReader;
import org.objectweb.asm.util.TraceSignatureVisitor;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MethodSignatureReifier {

    private final GenericHierarchyView initialJavaClassGenericHierarchyView;
    private static final Pattern UNBOUNDED_WILDCARD_PATTERN = Pattern.compile("\\?(?! [es])"); // matches declarations like <?,?> but not <? extends SomeType> ot <? super SomeType>

    public MethodSignatureReifier(GenericHierarchyView initialJavaClassGenericHierarchyView) {
        this.initialJavaClassGenericHierarchyView = initialJavaClassGenericHierarchyView;
    }

    public ReifiedJavaMethod transformJavaMethod(JavaMethod javaMethod) {
        int declaringClassAccessFlags = javaMethod.getDeclaringClass().getAccessFlags();

        if (javaMethod.hasGenericSignature()) {
            GenericParameters genericParameters = getResolvedFormalTypeParametersFromDeclaringClass(javaMethod);
            if (genericParameters != null) {
                return transformGenericJavaMethod(javaMethod, genericParameters, declaringClassAccessFlags);
            } else {
//                if (!javaMethod.getDeclaringClassName().equals(initialJavaClassGenericHierarchyView.getInitialClassName())) {
                return transformGenericJavaMethodWithoutReification(javaMethod, declaringClassAccessFlags);
//                }
//                return transformRegularJavaMethod(javaMethod, declaringClassAccessFlags);
            }
        } else {
            return transformRegularJavaMethod(javaMethod, declaringClassAccessFlags);
        }
    }

    private ReifiedJavaMethod transformGenericJavaMethodWithoutReification(JavaMethod javaMethod, int declaringClassAccessFlags) {
        DeclarationAndReturnType declarationAndReturnType = getSignatureDeclarationAndReturnType(javaMethod.getGenericSignature(), declaringClassAccessFlags);

        String argumentsDeclaration = declarationAndReturnType.getDeclaration();
        argumentsDeclaration = replaceAllUnboundedWildcardsIfAny(argumentsDeclaration);

        String ownGenericArgumentsDeclaration = getMethodTypeParametersDeclaration(argumentsDeclaration);
        ownGenericArgumentsDeclaration = replaceBoundsSeparatorInMethodTypeParametersDeclaration(ownGenericArgumentsDeclaration);

        String returnType = declarationAndReturnType.getReturnType();
        returnType = replaceAllUnboundedWildcardsIfAny(returnType);

        List<String> argumentsList = getReifiedArgumentsFromReifiedDeclaration(argumentsDeclaration);
        return new ReifiedJavaMethodImpl(javaMethod, argumentsList, ownGenericArgumentsDeclaration, returnType);
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

        String ownGenericArgumentsDeclaration = getMethodTypeParametersDeclaration(reifiedGenericArgumentsDeclaration);
        ownGenericArgumentsDeclaration = replaceBoundsSeparatorInMethodTypeParametersDeclaration(ownGenericArgumentsDeclaration);

        reifiedGenericArgumentsDeclaration = replaceAllUnboundedWildcardsIfAny(reifiedGenericArgumentsDeclaration);
        List<String> reifiedGenericArguments = getReifiedArgumentsFromReifiedDeclaration(reifiedGenericArgumentsDeclaration);

        String reifiedGenericReturnType = reifyReturnTypeIfNecessary(declarationAndReturnType.getReturnType(), classGenericParameters);
        reifiedGenericReturnType = replaceAllUnboundedWildcardsIfAny(reifiedGenericReturnType);

        return new ReifiedJavaMethodImpl(javaMethod, reifiedGenericArguments, ownGenericArgumentsDeclaration, reifiedGenericReturnType);
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

    private String replaceAllUnboundedWildcardsIfAny(String declaration) {
        return UNBOUNDED_WILDCARD_PATTERN.matcher(declaration).replaceAll("? extends java.lang.Object");
    }

    private String reifyArgumentsDeclarationIfNecessary(String declaration, GenericParameters resolvedFormalTypeParameters) {

        String reifiedDeclaration = declaration;

        for (Map.Entry<String, String> resolvedFormalTypeParameter : resolvedFormalTypeParameters.getGenericParameters().entrySet()) {
            Pattern pattern = Pattern.compile("([^\\w\\.])" + resolvedFormalTypeParameter.getKey() + "(\\W)");

            Matcher m = pattern.matcher(reifiedDeclaration);
            if (m.find()) {
                String replacement = BcelNamingUtil.resolveClassName(resolvedFormalTypeParameter.getValue());
                reifiedDeclaration = m.replaceAll("$1" + replacement + "$2");
            }
        }

        return reifiedDeclaration;
    }

    private String reifyReturnTypeIfNecessary(String returnType, GenericParameters resolvedFormalTypeParameters) {
        String reifiedReturnType = returnType;

        for (Map.Entry<String, String> resolvedFormalTypeParameter : resolvedFormalTypeParameters.getGenericParameters().entrySet()) {
            if (reifiedReturnType.equals(resolvedFormalTypeParameter.getKey())) {
                return resolvedFormalTypeParameter.getValue();
            } else {
                Pattern pattern = Pattern.compile("([^\\w\\.])" + resolvedFormalTypeParameter.getKey() + "(\\W)");
                Matcher m = pattern.matcher(reifiedReturnType);
                if (m.find()) {
                    String replacement = BcelNamingUtil.resolveClassName(resolvedFormalTypeParameter.getValue());
                    reifiedReturnType = m.replaceAll("$1" + replacement + "$2");
                }
            }
        }

        return reifiedReturnType.replaceAll("\\$", ".");
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

    // <T extends android.graphics.drawable.Drawable extends android.graphics.drawable.Animatable>(int, int, T, boolean)
    // <T extends android.graphics.drawable.Drawable, android.graphics.drawable.Animatable>(int, int, T, boolean)
    // <T extends android.graphics.drawable.Drawable<K extends O> extends android.graphics.drawable.Animatable>(int, int, T, boolean)
    private String replaceBoundsSeparatorInMethodTypeParametersDeclaration(String methodTypeParametersDeclaration) {
        if (methodTypeParametersDeclaration == null || methodTypeParametersDeclaration.equals("")) {
            return methodTypeParametersDeclaration;
        }

        StringBuilder sb = new StringBuilder();
        sb.append('<');

        boolean shouldReplaceExtendsKeyword = false;
        int openingAngleBracketsCounter = 0;
        int closingAngleBracketsCounter = 0;

        for (int i = 1; i < methodTypeParametersDeclaration.length() - 1; i += 1) { // skip opening and closing angle brackets
            char c = methodTypeParametersDeclaration.charAt(i);
            if (c == '<') {
                openingAngleBracketsCounter += 1;
                sb.append(c);
            } else if (c == '>') {
                closingAngleBracketsCounter += 1;
                sb.append(c);
            } else if (c == ',') {
                if (openingAngleBracketsCounter == closingAngleBracketsCounter) {
                    shouldReplaceExtendsKeyword = false;
                    openingAngleBracketsCounter = 0;
                    closingAngleBracketsCounter = 0;
                    sb.append(" & ");
                } else {
                    sb.append(c);
                }
            } else if (c == 'e') {
                if (i + 7 < methodTypeParametersDeclaration.length() - 1) {
                    if (methodTypeParametersDeclaration.charAt(i + 1) == 'x'
                            && methodTypeParametersDeclaration.charAt(i + 2) == 't'
                            && methodTypeParametersDeclaration.charAt(i + 3) == 'e'
                            && methodTypeParametersDeclaration.charAt(i + 4) == 'n'
                            && methodTypeParametersDeclaration.charAt(i + 5) == 'd'
                            && methodTypeParametersDeclaration.charAt(i + 6) == 's'
                            && methodTypeParametersDeclaration.charAt(i + 7) == ' ') {
                        if (shouldReplaceExtendsKeyword && openingAngleBracketsCounter == closingAngleBracketsCounter) {
                            sb.append(" & ");
                            i = i + 7;
                        } else {
                            shouldReplaceExtendsKeyword = true;
                            sb.append(c);
                        }
                    } else {
                        sb.append(c);
                    }
                } else {
                    sb.append(c);
                }
            } else if (c == '$') {
                sb.append('.');
            } else {
                sb.append(c);
            }
        }

        sb.append('>');
        return sb.toString();
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
//class Parent{
//    void work(List<?> data){
//
//    }
//}
//
//class Child extends Parent{
//
//    @Override
//    void work(List data){
//
//    }
//}
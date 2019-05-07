package org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.MethodDeclarationTransformation;

public class OwnTypeParametersDeclarationTransformation implements MethodDeclarationTransformation<String> {
    @Override
    public String transform(String input) {
        return replaceBoundsSeparatorInMethodTypeParametersDeclaration(input);
    }

    private String replaceBoundsSeparatorInMethodTypeParametersDeclaration(String methodTypeParametersDeclaration) {
        /*
        Due to bugs in the TraceSignatureVisitor in the ASM lib, ensure the type parameters declaration is in the form <T extends I1 & I2> and NOT <T extends I1, I2> or <T extends I1 extends I2>
         */

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

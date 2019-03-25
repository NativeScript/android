package org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.MethodDeclarationTransformation;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;

import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ArgumentsDeclarationReificationTransformation implements MethodDeclarationTransformation<String> {

    private final GenericParameters genericParametersToUse;

    public ArgumentsDeclarationReificationTransformation(GenericParameters genericParametersToUse) {
        this.genericParametersToUse = genericParametersToUse;
    }

    @Override
    public String transform(String input) {
        return reifyArgumentsDeclarationIfNecessary(input);
    }

    private String reifyArgumentsDeclarationIfNecessary(String declaration) {

        String reifiedDeclaration = declaration;

        for (Map.Entry<String, String> resolvedFormalTypeParameter : genericParametersToUse.getGenericParameters().entrySet()) {
            Pattern pattern = Pattern.compile("([^\\w\\.])" + resolvedFormalTypeParameter.getKey() + "(\\W)");

            Matcher m = pattern.matcher(reifiedDeclaration);
            if (m.find()) {
                String replacement = BcelNamingUtil.resolveClassName(resolvedFormalTypeParameter.getValue());
                reifiedDeclaration = m.replaceAll("$1" + replacement + "$2");
            }
        }

        return reifiedDeclaration;
    }
}
